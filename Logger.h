#pragma once

#include <fstream>
#include <iostream>

#include "sierrachart.h"

class Logger {
   public:
    Logger(SCStudyInterfaceRef &sc);

    void setHeader(const SCString &fileHeader);
    void writeEntryDataToFile(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta);
    void writeEntryDataToMessageLog(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta);
    std::string getFileName();

   private:
    std::string fileName;
    std::string fileHeader;

    bool fileExists();
    bool fileIsEmpty();
    void writeHeader();
    void writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog);
    void writeProfitLoss(SCStudyInterfaceRef &sc, s_SCPositionData &position);
};

inline Logger::Logger(SCStudyInterfaceRef &sc) {
    SCString chartName = sc.GetChartName(sc.ChartNumber);
    int index = chartName.LastIndexOf(' ', chartName.GetLength());
    SCString name = chartName.GetSubString(index - 1, 0);
    fileName = name += ".csv";
}

inline void Logger::setHeader(const SCString &fileHeader) {
    this->fileHeader = fileHeader;
}

inline std::string Logger::getFileName() {
    return fileName;
}

// refactor - not so much params
inline void Logger::writeEntryDataToFile(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta) {
    float volumeOnPrevBar = volume[volume.GetArraySize() - 2];
    float atrOnPrevBar = atr[atr.GetArraySize() - 2];
    float cumDeltaOnPrevBar = cumDelta[cumDelta.GetArraySize() - 2];

    SCDateTime entryDateTime = position.LastEntryDateTime;
    int year, month, day, hour, minute, second;
    entryDateTime.GetDateTimeYMDHMS(year, month, day, hour, minute, second);

    SCString stringToLog;
    stringToLog.Format(
        "%i-%i-%i %0.2i:%0.2i:%0.2i,"
        "%3.2f,"
        "%s,"
        "%3.0f,"
        "%.02f,"
        "%.02f,"
        "%.02f,",
        year, month, day, hour, minute, second,
        position.AveragePrice,
        position.Symbol,
        position.PositionQuantity,
        volumeOnPrevBar,
        atrOnPrevBar,
        cumDeltaOnPrevBar);

    int &p_logEntryDone = sc.GetPersistentInt(-1);      // 0
    int &p_logExitDone = sc.GetPersistentInt(-2);       // 0
    int &p_previousPosition = sc.GetPersistentInt(-3);  // 0

    bool positionOpened = p_previousPosition == 0 && position.PositionQuantity != 0;
    bool positionClosed = position.PositionQuantity == 0;
    bool logEntryNotDone = p_logEntryDone == 0;
    bool logExitNotDone = p_logExitDone == 0;

    if (positionOpened && logEntryNotDone) {
        if (!fileExists()) writeHeader();
        writeEntryData(sc, stringToLog);
        p_logExitDone = 0;
        p_logEntryDone = 1;
    }

    if (positionClosed && logExitNotDone) {
        writeProfitLoss(sc, position);
        p_logEntryDone = 0;
        p_logExitDone = 1;
    }
}

// refactor - not so much params
inline void Logger::writeEntryDataToMessageLog(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta) {
    int &p_logEntryDone = sc.GetPersistentInt(0);
    int &p_previousPositionQty = sc.GetPersistentInt(1);

    bool positionClosed = position.PositionQuantity == 0;
    bool newPositionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntryDone == 0;

    float volumeOnPrevBar = volume[volume.GetArraySize() - 2];
    float atrOnPrevBar = atr[atr.GetArraySize() - 2];
    float cumDeltaOnPrevBar = cumDelta[cumDelta.GetArraySize() - 2];

    SCString stringToLog;
    stringToLog.Format("last position result: %0.02f", position.LastTradeProfitLoss);

    if (newPositionOpened && logEntryNotYetDone) {
        sc.AddMessageToLog(stringToLog, 1);
        p_logEntryDone = 1;
    }
    if (positionClosed) {
        p_logEntryDone = 0;
    }
}

inline bool Logger::fileIsEmpty() {
    std::ifstream inFile(fileName);
    return inFile.peek() == std::ifstream::traits_type::eof();
}

inline bool Logger::fileExists() {
    std::ifstream file(fileName);
    return file.good();
}

inline void Logger::writeHeader() {
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        outFile << fileHeader << std::endl;
    }
    outFile.close();
}

inline void Logger::writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog) {
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        std::string data{stringToLog};
        outFile << data;
        outFile.close();
    } else {
        sc.AddMessageToLog("Error writing Data into file", 1);
    }
}

inline void Logger::writeProfitLoss(SCStudyInterfaceRef &sc, s_SCPositionData &position) {
    std::ofstream outFile;
    double profitLoss = position.LastTradeProfitLoss;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        outFile << ", " << profitLoss << std::endl;
        outFile.close();
    } else {
        // flush the buffer kdyz studii sundam

        sc.AddMessageToLog("Error writing PL into file", 1);
    }
}