// Logger study responsible for writing entry details to csv file.
// Besides basic infor as entry price, qty, market, it also logs some basic market stats [ATR, Volume, Cum.Delta] and these can be extended.
// The point is to use this study with every trading system, colect the data and run some data-analysis on them

#include "Logger.h"

#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>


#include "sierrachart.h"

SCDLLName("Nirvikalpa's Logger");

SCSFExport scsf_Logger(SCStudyInterfaceRef sc) {
    SCInputRef messageLogAllowed = sc.Input[10];
    SCInputRef fileLogAllowed = sc.Input[11];

    if (sc.SetDefaults) {
        messageLogAllowed.Name = "Log to SC message log";
        messageLogAllowed.SetYesNo(0);
        fileLogAllowed.Name = "Log to external file";
        fileLogAllowed.SetYesNo(1);

        sc.Input[1].Name = "1. study";
        sc.Input[1].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[2].Name = "2. study";
        sc.Input[2].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[3].Name = "3. study";
        sc.Input[3].SetChartStudySubgraphValues(0, 0, 0);

        sc.GraphRegion = 0;
        sc.GraphName = "Logger";
        return;
    }

    SCFloatArray study01;
    SCFloatArray study02;
    SCFloatArray study03;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), sc.Input[1].GetSubgraphIndex(), study01);
    sc.GetStudyArrayFromChartUsingID(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), sc.Input[2].GetSubgraphIndex(), study02);
    sc.GetStudyArrayFromChartUsingID(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), sc.Input[3].GetSubgraphIndex(), study03);
	
    study::log::Logger* p_Logger = (study::log::Logger*)sc.GetPersistentPointer(1);
    if (sc.LastCallToFunction && p_Logger != NULL) {
        delete p_Logger;
        sc.SetPersistentPointer(1, NULL);
        return;
    }

    if (p_Logger == NULL) p_Logger = (study::log::Logger*)new study::log::Logger(sc);

    std::string chartName {sc.GetChartName(sc.ChartNumber)};
    std::string::iterator endPos = std::remove(chartName.begin(), chartName.end(), ' ');
    chartName.erase(endPos, chartName.end());
    std::string fileName =  chartName + ".csv";

    if (fileLogAllowed.GetBoolean() == 1) {
        // p_Logger->setFileName(fileName);
        p_Logger->writeToFile(sc, study01, study02, study03);
    }

    // if (messageLogAllowed.GetBoolean() == 1) {
    //     p_Logger->writeEntryDataToMessageLog(sc, position, study01, study02, study03);
    // }
}

// std::string getChartName(SCStudyInterfaceRef sc){
//     std::string chartName {sc.GetChartName(sc.ChartNumber)};
//     std::string::iterator endPos = std::remove(chartName.begin(), chartName.end(), ' ');
//     chartName.erase(endPos, chartName.end());
//     return chartName;
// }

// std::string getCurrentTime() {
//     auto t = std::time(nullptr);
//     auto tm = *std::localtime(&t);
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
//     return oss.str();
// }

