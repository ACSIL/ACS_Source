// Logger study responsible for writing entry details to csv file.
// Besides basic infor as entry price, qty, market, it also logs some basic market stats [ATR, Volume, Cum.Delta] and these can be extended.
// The point is to use this study with every trading system, colect the data and run some data-analysis on them

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

    s_SCPositionData position;
    sc.GetTradePosition(position);

    SCFloatArray volume;
    SCFloatArray atr;
    SCFloatArray cumDelta;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), sc.Input[1].GetSubgraphIndex(), volume);
    sc.GetStudyArrayFromChartUsingID(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), sc.Input[2].GetSubgraphIndex(), atr);
    sc.GetStudyArrayFromChartUsingID(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), sc.Input[3].GetSubgraphIndex(), cumDelta);

    Logger* p_Logger = (Logger*)sc.GetPersistentPointer(1);

    if (sc.LastCallToFunction) {
        if (p_Logger != NULL) {
            delete p_Logger;
            sc.SetPersistentPointer(1, NULL);
        }
        return;
    }
    if (p_Logger == NULL) p_Logger = (Logger*)new Logger("myLoggerFile.csv");  // via input

    p_Logger->setHeader("datetime, price, symbol, qty, volume, atr, delta");  // via input
    p_Logger->writeEntryDetailsToFileAfterOpeningNewTrade(sc, position, volume, atr, cumDelta);
}