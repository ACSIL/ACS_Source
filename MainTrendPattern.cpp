#include "ThreeCandlesEMATrendPattern.h"
#include "ThreeCandlesTrendPattern.h"
#include "sierrachart.h"

SCDLLName("Nirvikalpa's Trend Patterns");

SCSFExport scsf_TrendPatterns(SCStudyInterfaceRef sc) {
    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];

    // SCInputRef inputShowEma = sc.get

    SCInputRef inputEmaPeriod = sc.Input[10];
    SCSubgraphRef subgraphEMA = sc.Subgraph[0];

    if (sc.SetDefaults) {
        sc.GraphName = "TrendPatterns";
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

    aos::trend::ThreeCandlesEMA* p_ThreeCandlesEMA = (aos::trend::ThreeCandlesEMA *)sc.GetPersistentPointer(1);

    if (p_ThreeCandlesEMA != NULL && sc.LastCallToFunction) {
        delete p_ThreeCandlesEMA;
        sc.SetPersistentPointer(1, NULL);
        return;
    }

    if (p_ThreeCandlesEMA == NULL)
        p_ThreeCandlesEMA = (aos::trend::ThreeCandlesEMA *)new aos::trend::ThreeCandlesEMA();

    int emaPeriod = inputEmaPeriod.GetInt();
    p_ThreeCandlesEMA->setEmaPeriod(emaPeriod);
    p_ThreeCandlesEMA->showEmaSubgraph(sc, subgraphEMA);
    
    s_SCPositionData position;
    sc.GetTradePosition(position);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Three Candles Trend Pattern";
    order.Target1Offset = 60 * sc.TickSize;
    order.Stop1Offset = 30 * sc.TickSize;

    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > inputStartTradingAt.GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < inputStopTradingAt.GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= inputFlatPostionAt.GetTime();
    bool positionOpened = position.PositionQuantity != 0;

    if (areTradingHours) {
        if (p_ThreeCandlesEMA->isUp(sc)) {
            int entryCheck = (int)sc.BuyEntry(order);
        }
        if (p_ThreeCandlesEMA->isDown(sc)) {
            int entryCheck = (int)sc.SellEntry(order);
        }
    }

    if (positionOpened && isTimeToFlat) sc.FlattenPosition();
}
