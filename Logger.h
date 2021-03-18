#pragma once

#include <fstream>
#include <iostream>

#include "sierrachart.h"

namespace study {
namespace log {

class Logger {
   public:
    Logger(SCStudyInterfaceRef &sc);

    void writeToFile(SCStudyInterfaceRef &sc, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03);
    void writeEntryDataToMessageLog(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03);

   private:
    std::string fileName;
    std::string fileHeader;
  
    bool fileExists();
    bool fileIsEmpty();
    void writeHeader(SCStudyInterfaceRef &sc);
    void writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog);
    void writeProfitLoss(SCStudyInterfaceRef &sc, s_SCPositionData &position);
    SCString createString(SCStudyInterfaceRef &sc, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03);
    // std::string getChartName(SCStudyInterfaceRef &sc);
    // std::string getCurrentTime();
    // void createFileName(SCStudyInterfaceRef &sc);

};

inline Logger::Logger(SCStudyInterfaceRef &sc) {
    std::string chartName {sc.GetChartName(sc.ChartNumber)};
    std::string::iterator endPos = std::remove(chartName.begin(), chartName.end(), ' ');
    chartName.erase(endPos, chartName.end());
    this->fileName = chartName + ".csv";
}


inline SCString Logger::createString(SCStudyInterfaceRef &sc, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03) {
    s_SCPositionData position;
    sc.GetTradePosition(position);

    SCDateTime entryDateTime = position.LastEntryDateTime;
    SCDateTime exitDateTime = position.LastExitDateTime;
    double entryPrice = position.AveragePrice;
    double qty = position.PositionQuantity;

    int yearEntry, monthEntry, dayEntry, hourEntry, minuteEntry, secondEntry;
    int yearExit, monthExit, dayExit, hourExit, minuteExit, secondExit;

    entryDateTime.GetDateTimeYMDHMS(yearEntry, monthEntry, dayEntry, hourEntry, minuteEntry, secondEntry);
    exitDateTime.GetDateTimeYMDHMS(yearExit, monthExit, dayExit, hourExit, minuteExit, secondExit);

    float study01OnPrevBar = study01[study01.GetArraySize() - 2];
    float study02OnPrevBar = study02[study02.GetArraySize() - 2];
    float study03OnPrevBar = study03[study03.GetArraySize() - 2];

    sc.AddMessageToLog(std::to_string(study03OnPrevBar).c_str(), 1);

    SCString stringToLog;
    stringToLog.Format(
        "%i-%i-%i %0.2i:%0.2i:%0.2i,"  // entry dt
        "%3.2f,"                       //entry price
        "%3.0f,"                       // qty
        "%.02f,"                       // s1
        "%.02f,"                       // s2
        "%.02f,",                      // s3
        yearEntry, monthEntry, dayEntry, hourEntry, minuteEntry, secondEntry,
        entryPrice,
        qty,
        study01OnPrevBar,
        study02OnPrevBar,
        study03OnPrevBar);

    return stringToLog;
}

inline void Logger::writeToFile(SCStudyInterfaceRef &sc, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03) {
    auto stringToLog = createString(sc, study01, study02, study03);
    int &p_logEntryDone = sc.GetPersistentInt(-1);      // 0
    int &p_logExitDone = sc.GetPersistentInt(-2);       // 0
    int &p_previousPosition = sc.GetPersistentInt(-3);  // 0

    s_SCPositionData position;
    sc.GetTradePosition(position);

    bool positionOpened = p_previousPosition == 0 && position.PositionQuantity != 0;
    bool positionClosed = position.PositionQuantity == 0;
    bool logEntryNotDone = p_logEntryDone == 0;
    bool logExitNotDone = p_logExitDone == 0;

    if (positionOpened && logEntryNotDone) {

        if (!fileExists()) writeHeader(sc);
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
inline void Logger::writeEntryDataToMessageLog(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &study01, SCFloatArray &study02, SCFloatArray &study03) {
    int &p_logEntryDone = sc.GetPersistentInt(0);
    int &p_previousPositionQty = sc.GetPersistentInt(1);

    bool positionClosed = position.PositionQuantity == 0;
    bool newPositionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntryDone == 0;

    float study01OnPrevBar = study01[study01.GetArraySize() - 2];
    float study02OnPrevBar = study02[study02.GetArraySize() - 2];
    double study03OnPrevBar = study03[study03.GetArraySize() - 2];

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

inline void Logger::writeHeader(SCStudyInterfaceRef &sc) {
    std::ofstream outFile;
    // sc.AddMessageToLog("in write header", 1);

    SCString study01Name = sc.GetStudyNameFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID());
    SCString study02Name = sc.GetStudyNameFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID());
    SCString study03Name = sc.GetStudyNameFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID());

    SCString header = "Entry DateTime, Entry Price, Qty,";
    header += study01Name.Append(",");
    header += study02Name.Append(",");
    header += study03Name.Append(", PL");

    // sc.AddMessageToLog(study01Name, 1);

    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) outFile << header << std::endl;
    outFile.close();
}

inline void Logger::writeEntryData(SCStudyInterfaceRef &sc, SCString &stringToLog) {
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        outFile << stringToLog;
        outFile.close();
    } else {
        sc.AddMessageToLog("Error writing EntryData into file", 1);
    }
}

inline void Logger::writeProfitLoss(SCStudyInterfaceRef &sc, s_SCPositionData &position) {
    std::ofstream outFile;
    double profitLoss = position.LastTradeProfitLoss;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        outFile << profitLoss << std::endl;
        outFile.close();
    } else {
        // flush the buffer kdyz studii sundam

        sc.AddMessageToLog("Error writing PL into file", 1);
    }
}

inline std::string getChartName(SCStudyInterfaceRef sc){
    std::string chartName {sc.GetChartName(sc.ChartNumber)};
    std::string::iterator endPos = std::remove(chartName.begin(), chartName.end(), ' ');
    chartName.erase(endPos, chartName.end());
    return chartName;
}

// inline std::string getCurrentTime() {
//     auto t = std::time(nullptr);
//     auto tm = *std::localtime(&t);
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
//     return oss.str();
// }

// inline void Logger::createFileName(SCStudyInterfaceRef sc) {
//     std::string chartName = getChartName(sc);
//     std::string currentTime = getCurrentTime();
//     this->fileName = chartName + currentTime + ".csv";
// }
}  // namespace log

}  // namespace study
