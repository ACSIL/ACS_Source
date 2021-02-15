
#include "Logger.h"

#include <fstream>
#include <string>

#include "sierrachart.h"

SCDLLName("Logger");

SCSFExport scsf_Logger(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.Input[1].Name = "Volume";
        sc.Input[1].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[2].Name = "ATR";
        sc.Input[2].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[3].Name = "Cumulative delta";
        sc.Input[3].SetChartStudySubgraphValues(0, 0, 0);

        sc.GraphRegion = 0;
        sc.GraphName = "Logger";
        return;
    }

    Logger* p_Logger = (Logger*)sc.GetPersistentPointer(1);

    // do cleanup
    if (sc.LastCallToFunction) {
        if (p_Logger != NULL) {
            delete p_Logger;
            sc.SetPersistentPointer(1, NULL);
        }
        return;
    }

    // create the object on heap
    if (p_Logger == NULL) {
        p_Logger = (Logger*)new Logger("myLoggerFile.txt");
    }

    std::string header {};
    p_Logger->setHeader("ahoj, tady je muj pes");
    std::string header = p_Logger->getHeader();
    sc.AddMessageToLog(header.c_str(), 1);
}