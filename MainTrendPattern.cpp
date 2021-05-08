#include "ThreeCandlesEMATrendPattern.h"
#include "ThreeCandlesTrendPattern.h"
#include "sierrachart.h"

SCDLLName("Nirvikalpa's Trend Patterns");

SCSFExport scsf_TrendPatterns(SCStudyInterfaceRef sc) {
    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];

    SCInputRef inputEmaPeriod = sc.Input[10];
    SCInputRef inputStudySubgraphReference = sc.Input[11];

    SCSubgraphRef subgraphEMA = sc.Subgraph[0];
    SCSubgraphRef subgraphValidEntryBar = sc.Subgraph[1];
    SCSubgraphRef subgraphBuyEntry = sc.Subgraph[10];
    SCSubgraphRef subgraphSellEntry = sc.Subgraph[11];

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

        inputEmaPeriod.Name = "Exp. Moving Average Period";
        inputEmaPeriod.SetInt(10);

        inputStudySubgraphReference.Name = "Volatility Trend Indicator Subgraph to Display";
        inputStudySubgraphReference.SetStudySubgraphValues(7, 1);

        subgraphValidEntryBar.Name = "Bar Marker";
        subgraphValidEntryBar.DrawStyle = DRAWSTYLE_COLOR_BAR;
        subgraphValidEntryBar.PrimaryColor = RGB(0, 0, 255);

        subgraphBuyEntry.Name = "Buy Entry";
        subgraphBuyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
        subgraphBuyEntry.PrimaryColor = RGB(255, 255, 255);
        subgraphBuyEntry.LineWidth = 2;

        subgraphSellEntry.Name = "Sell Entry";
        subgraphSellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
        subgraphSellEntry.PrimaryColor = RGB(255, 255, 255);
        subgraphSellEntry.LineWidth = 2;

        subgraphEMA.Name = "Exp. Moving Average";
        subgraphEMA.DrawStyle = DRAWSTYLE_LINE;
        subgraphEMA.PrimaryColor = RGB(102, 255, 102);


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

    // for further modifications
    int64_t& r_BuyEntryInternalOrderID = sc.GetPersistentInt64(1);
    int64_t& r_SellEntryInternalOrderID = sc.GetPersistentInt64(2);


    SCFloatArray volatilityTrendIndivator;
    sc.GetStudyArrayUsingID(inputStudySubgraphReference.GetStudyID(), inputStudySubgraphReference.GetSubgraphIndex(), volatilityTrendIndivator);
    float volatilityTrendIndivatorValue = volatilityTrendIndivator[sc.Index];
    bool isVolatilityTrendIndicatorAboveCurrentPrice = volatilityTrendIndivatorValue > sc.Close[sc.Index];
    bool isVolatilityTrendIndicatorBellowCurrentPrice = volatilityTrendIndivatorValue < sc.Close[sc.Index];

    if (areTradingHours) {
        if (p_ThreeCandlesEMA->isUp(sc) && isVolatilityTrendIndicatorBellowCurrentPrice) {
            subgraphValidEntryBar[sc.Index] = sc.Index;
            int entryCheck = (int)sc.BuyEntry(newOrder);
            if (entryCheck > 0) {
                SCString entryCheckStr;
                entryCheckStr.Format("EntryCheck: %d", entryCheck);
                sc.AddMessageToLog(entryCheckStr, 0);

                r_BuyEntryInternalOrderID = newOrder.InternalOrderID;
                SCString internalOrderIDNumberString;
                internalOrderIDNumberString.Format("BuyEntry Internal Order ID: %d", r_BuyEntryInternalOrderID);
                sc.AddMessageToLog(internalOrderIDNumberString, 1);

                subgraphBuyEntry[sc.Index] = sc.Low[sc.Index];
            }
        } else if (positionOpened && priceCrossedEmaFromBellow) {
            (int)sc.BuyExit(newOrder);
        }

        // } else if (p_ThreeCandlesEMA->isDown(sc)) {
        //     subgraphValidEntryBar[sc.Index] = sc.Index;
        //     int entryCheck = (int)sc.SellEntry(newOrder);
        //     if (entryCheck > 0) {
        //         sc.AddMessageToLog(entryCheck, 1);
        //         subgraphSellEntry[sc.Index] = sc.High[sc.Index];
        //     }
        // } else if (positionOpened && priceCrossedEmaFromBellow) {
        //     (int)sc.SellExit(newOrder);
        // }
    }

    if (positionOpened && isTimeToFlat) sc.FlattenPosition();
}
