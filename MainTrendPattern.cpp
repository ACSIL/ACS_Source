#include "ThreeCandlesEMATrendPattern.h"
#include "ThreeCandlesTrendPattern.h"
#include "sierrachart.h"

SCDLLName("Nirvikalpa's Trend Patterns");


// s_SCNewOrder& exitAtFixedTargetLoss(SCStudyInterfaceRef sc, s_SCNewOrder& order) {
//     order.OrderQuantity = 1;
//     order.OrderType = SCT_ORDERTYPE_MARKET;
//     order.TextTag = "Three Candles Trend Pattern";
//     order.Target1Offset = sc.Input[3].GetInt() * sc.TickSize;
//     order.Stop1Offset = sc.Input[4].GetInt() * sc.TickSize;
//     return order;
// }


SCSFExport scsf_TrendPatterns(SCStudyInterfaceRef sc) {
    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];

    SCInputRef inputEmaPeriod = sc.Input[10];
    SCSubgraphRef subgraphEMA = sc.Subgraph[0];

    if (sc.SetDefaults) {
        sc.GraphName = "TrendPatterns";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        inputStartTradingAt.Name = "Start trading at: ";
        inputStartTradingAt.SetTime(HMS_TIME(8, 30, 0));
        inputStopTradingAt.Name = "Stop trading at: ";
        inputStopTradingAt.SetTime(HMS_TIME(14, 00, 0));
        inputFlatPostionAt.Name = "Flat postion at: ";
        inputFlatPostionAt.SetTime(HMS_TIME(14, 30, 00));

        sc.Input[3].Name = "Profit in ticks";
        sc.Input[3].SetInt(30);
        sc.Input[4].Name = "Loss in ticks";
        sc.Input[4].SetInt(30);

        sc.Subgraph[1].Name = "visible";
        sc.Subgraph[1].DrawStyle = DRAWSTYLE_COLOR_BAR;
        sc.Subgraph[1].PrimaryColor = RGB(0, 0, 255);

        subgraphEMA.Name = "Exp. Moving Average";
        subgraphEMA.DrawStyle = DRAWSTYLE_LINE;
        subgraphEMA.PrimaryColor = RGB(102, 255, 102);
        inputEmaPeriod.Name = "Exp. Moving Average Period";
        inputEmaPeriod.SetInt(10);


		sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }

    aos::trend::ThreeCandlesEMA *p_ThreeCandlesEMA = (aos::trend::ThreeCandlesEMA *)sc.GetPersistentPointer(1);

    if (p_ThreeCandlesEMA != NULL && sc.LastCallToFunction) {
        delete p_ThreeCandlesEMA;
        sc.SetPersistentPointer(1, NULL);
        return;
    }

    if (p_ThreeCandlesEMA == NULL)
        p_ThreeCandlesEMA = (aos::trend::ThreeCandlesEMA *)new aos::trend::ThreeCandlesEMA();
   
    s_SCPositionData position;
    sc.GetTradePosition(position);

    // ema
    int emaPeriod = inputEmaPeriod.GetInt();
    p_ThreeCandlesEMA->setEmaPeriod(emaPeriod);
    p_ThreeCandlesEMA->showEmaSubgraph(sc, subgraphEMA);

    // orders
    s_SCNewOrder newOrder;
    newOrder.OrderQuantity = 1;
    newOrder.OrderType = SCT_ORDERTYPE_MARKET;
    newOrder.TimeInForce = SCT_TIF_GOOD_TILL_CANCELED;
    newOrder.TextTag = "Three Candles Trend Pattern";
    
    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > inputStartTradingAt.GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < inputStopTradingAt.GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= inputFlatPostionAt.GetTime();
    bool positionOpened = position.PositionQuantity != 0;
    bool priceCrossedEmaFromBellow = sc.CrossOver(sc.Close, subgraphEMA) == CROSS_FROM_TOP && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
    
    
    if (areTradingHours) {
        if (p_ThreeCandlesEMA->isUp(sc)) {
            int entryCheck = (int)sc.BuyEntry(newOrder);
            sc.Subgraph[1][sc.Index] = sc.Index;
        }
        else if (positionOpened && priceCrossedEmaFromBellow) {
            (int)sc.BuyExit(newOrder);
        }
        // if (p_ThreeCandlesEMA->isDown(sc)) {
        //     int entryCheck = (int)sc.SellEntry(order);
        //     sc.Subgraph[1][sc.Index] = sc.Index;
        // }
    }

    if (positionOpened && isTimeToFlat) sc.FlattenPosition();
}
