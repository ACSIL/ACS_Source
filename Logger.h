#pragma once

#include <fstream>
#include <iostream>

#include "sierrachart.h"

class Logger {
   public:
    Logger(const std::string &fileName);

    void setHeader(const std::string &fileHeader);
    void writeEntryDetailsToFileAfterOpeningNewTrade(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta);
    void writeToMessageLog();

   private:
    std::string fileName;
    std::string fileHeader;

    bool fileExists();
    bool fileIsEmpty();
    void writeHeader();
    void writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog);
};

inline Logger::Logger(const std::string &fileName) {
    this->fileName = fileName;
}

inline void Logger::setHeader(const std::string &fileHeader) {
    this->fileHeader = fileHeader;
}

inline void Logger::writeEntryDetailsToFileAfterOpeningNewTrade(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta) {
    int &p_logEntryDone = sc.GetPersistentInt(2);
    int &p_previousPositionQty = sc.GetPersistentInt(3);

    bool positionClosed = position.PositionQuantity == 0;
    bool newPositionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntryDone == 0;

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

    if (newPositionOpened && logEntryNotYetDone) {
        if (!fileExists()) writeHeader();
        writeEntryData(sc, stringToLog);
    }
    
    p_logEntryDone = 1;
    if (positionClosed) p_logEntryDone = 0;
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
        outFile << fileHeader << '\n';
    }
    outFile.close();
}

inline void Logger::writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog) {
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        std::string data{stringToLog};
        outFile << data << '\n';
        outFile.close();
    } else {
        sc.AddMessageToLog("Error writing into file", 1);
    }
}