#include "sierrachart.h"

SCDLLName("False Consolidation Breakout");

void setSubgraphs(SCSubgraphRef &subgraphConsolidatingBar) {
    subgraphConsolidatingBar.Name = "Up trend pattern";
    subgraphConsolidatingBar.DrawStyle = DRAWSTYLE_COLOR_BAR;
    subgraphConsolidatingBar.PrimaryColor = RGB(0, 0, 255);
}

void setInputsForTradingHours(SCInputRef &inputStartTradingAt, SCInputRef &inputStopTradingAt, SCInputRef &inputFlatPostionAt) {
    inputStartTradingAt.Name = "Start trading at: ";
    inputStartTradingAt.SetTime(HMS_TIME(8, 30, 0));
    inputStopTradingAt.Name = "Stop trading at: ";
    inputStopTradingAt.SetTime(HMS_TIME(15, 00, 0));
    inputFlatPostionAt.Name = "Flat postion at: ";
    inputFlatPostionAt.SetTime(HMS_TIME(15, 14, 30));
}

void setTradingOptions(SCStudyInterfaceRef &sc) {
    sc.AllowMultipleEntriesInSameDirection = false;
    sc.MaximumPositionAllowed = 1;
    sc.SendOrdersToTradeService = false;
    sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;
    sc.SupportAttachedOrdersForTrading = false;
    sc.CancelAllOrdersOnEntriesAndReversals = true;
    sc.AllowEntryWithWorkingOrders = false;
    sc.CancelAllWorkingOrdersOnExit = false;
    sc.AllowOnlyOneTradePerBar = true;
    sc.MaintainTradeStatisticsAndTradesData = true;
}

void markConsolidation(SCStudyInterfaceRef &sc) {
    const int allowedDifferceInTicks = 3;

    sc.High[sc.Index - 1] - sc.High[sc.Index - 2] <= allowedDifferceInTicks;
}

// void markCenterOfBar(SCStudyInterfaceRef &sc) {
//     (sc.High[sc.Index] - sc.Low[sc.Index]) / 2
// }

SCSFExport scsf_FalseConsolidationBreakout(SCStudyInterfaceRef sc) {
    SCSubgraphRef subgraphConsolidatingBar = sc.Subgraph[0];
    SCSubgraphRef subgraphBarCenterMark = sc.Subgraph[1];

    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];

    if (sc.SetDefaults) {
        sc.GraphName = "False Consolidation Beakout";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        subgraphBarCenterMark.Name = "Center";
        subgraphBarCenterMark.DrawStyle = DRAWSTYLE_POINT;
        subgraphBarCenterMark.PrimaryColor = RGB(255, 255, 255);

        setSubgraphs(subgraphConsolidatingBar);
        setInputsForTradingHours(inputStartTradingAt, inputStopTradingAt, inputFlatPostionAt);
        setTradingOptions(sc);
        return;
    }

    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > inputStartTradingAt.GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < inputStopTradingAt.GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= inputFlatPostionAt.GetTime();

    // subgraphBarCenterMark[sc.Index] = (sc.High[sc.Index] - sc.Low[sc.Index]) / 2;
    subgraphBarCenterMark[sc.Index] = (sc.Low[sc.Index] + sc.High[sc.Index]) / 2;
}