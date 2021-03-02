#include "Trend.h"

#include "sierrachart.h"

SCDLLName("Trend");

SCSFExport scsf_Trend(SCStudyInterfaceRef sc) {
    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];
    
    // SCInputRef inputShowEma = sc.get

    SCInputRef inputEmaPeriod = sc.Input[10];
    SCSubgraphRef subgraphEMA = sc.Subgraph[0];

    if (sc.SetDefaults) {
        sc.GraphName = "Trend";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        inputStartTradingAt.Name = "Start trading at: ";
        inputStartTradingAt.SetTime(HMS_TIME(8, 30, 0));
        inputStopTradingAt.Name = "Stop trading at: ";
        inputStopTradingAt.SetTime(HMS_TIME(15, 00, 0));
        inputFlatPostionAt.Name = "Flat postion at: ";
        inputFlatPostionAt.SetTime(HMS_TIME(15, 14, 30));

        subgraphEMA.Name = "Exp. Moving Average";
        subgraphEMA.DrawStyle = DRAWSTYLE_LINE;
        subgraphEMA.PrimaryColor = RGB(102, 255, 102);
        inputEmaPeriod.Name = "Exp. Moving Average Period";
        inputEmaPeriod.SetInt(10);
        
        return;
    }

    aos::trend::Trend *p_Trend = (aos::trend::Trend *)sc.GetPersistentPointer(1);

    if (sc.LastCallToFunction && p_Trend) {
        delete p_Trend;
        sc.SetPersistentPointer(1, NULL);
        return;
    }

    if (p_Trend == NULL) p_Trend = (aos::trend::Trend *)new aos::trend::Trend();

    s_SCPositionData position;
    sc.GetTradePosition(position);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Trend - Three rising bars with indicators filter";
    order.Target1Offset = 30 * sc.TickSize;
    order.Stop1Offset = 10 * sc.TickSize;

    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > inputStartTradingAt.GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < inputStopTradingAt.GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= inputFlatPostionAt.GetTime();
    bool positionOpened = position.PositionQuantity != 0;

    int emaPeriod = inputEmaPeriod.GetInt();
    
    p_Trend->showEmaSubgraph(sc, subgraphEMA, emaPeriod);


    if (areTradingHours) {
        if (p_Trend->isUp(sc, emaPeriod)) {
            int entryCheck = (int)sc.BuyEntry(order);
        }
        // if (p_Trend->isDown(sc, 7)) {
        //     int entryCheck = (int)sc.SellEntry(order);
        // }
    }

    if (positionOpened && isTimeToFlat) sc.FlattenPosition();
}
