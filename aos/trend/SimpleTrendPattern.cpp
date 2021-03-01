// simple trend pattern is three candelstick, each has the high and low higher then the preceding
// shows the entry bar colored in green/red based on the direction
// idea - use it on range bars -> pt/sl is the range bar..

#include <string>

#include "sierrachart.h"
SCDLLName("Simple Trend Pattern");

void setInputs(SCInputRef &inputStartTradingAt, SCInputRef &inputStopTradingAt, SCInputRef &inputFlatPostionAt) {
    inputStartTradingAt.Name = "Start trading at: ";
    inputStartTradingAt.SetTime(HMS_TIME(8, 30, 0));
    inputStopTradingAt.Name = "Stop trading at: ";
    inputStopTradingAt.SetTime(HMS_TIME(15, 00, 0));
    inputFlatPostionAt.Name = "Flat postion at: ";
    inputFlatPostionAt.SetTime(HMS_TIME(15, 14, 30));
}

void setSubgraphs(SCSubgraphRef &subgraphUpTrend, SCSubgraphRef &subgraphDownTrend) {
    subgraphUpTrend.Name = "Up trend pattern";
    subgraphUpTrend.DrawStyle = DRAWSTYLE_ARROW_UP;
    subgraphUpTrend.PrimaryColor = RGB(0, 0, 255);

    subgraphDownTrend.Name = "Down trend pattern";
    subgraphDownTrend.DrawStyle = DRAWSTYLE_ARROW_DOWN;
    subgraphDownTrend.PrimaryColor = RGB(0, 0, 255);
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

void logMsg(SCStudyInterfaceRef &sc, s_SCNewOrder &order) {
    SCString internalOrderIDNumberString;
    internalOrderIDNumberString.Format("BuyEntry Internal Order ID: %d", order.InternalOrderID);
    sc.AddMessageToLog(internalOrderIDNumberString, 0);
}

void takeLong(SCStudyInterfaceRef sc, SCSubgraphRef &subgraphUpTrend, bool areTradingHours, bool isTimeToFlat) {
    s_SCPositionData currentPosition;
    sc.GetTradePosition(currentPosition);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Simple trend pattern";
    order.Target1Offset = 10 * sc.TickSize;
    order.Stop1Offset = 10 * sc.TickSize;

    bool currentBarHasClosed = sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
    bool hasThreeConsecutiveHigherHighs = sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2];
    bool hasThreeConsecutiveHigherLows = sc.Low[sc.Index] > sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] > sc.Low[sc.Index - 2];

    if (areTradingHours && hasThreeConsecutiveHigherHighs && hasThreeConsecutiveHigherLows & currentBarHasClosed) {
        subgraphUpTrend[sc.Index] = sc.Close[sc.Index];
        orderSuccesCheck = (int)sc.BuyEntry(order);
    }

    if (isTimeToFlat) {
        sc.FlattenPosition();
    }
}

void takeShort(SCStudyInterfaceRef &sc, SCSubgraphRef &subgraphDownTrend, bool areTradingHours, bool isTimeToFlat) {
    s_SCPositionData currentPosition;
    sc.GetTradePosition(currentPosition);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Simple trend pattern";
    order.AttachedOrderTarget1Type = SCT_ORDERTYPE_LIMIT;
    order.AttachedOrderStopAllType = SCT_ORDERTYPE_STOP;
    order.Target1Offset = 20 * sc.TickSize;
    order.Stop1Offset = 20 * sc.TickSize;

    int orderSuccesCheck = 0;

    bool currentBarHasClosed = sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
    bool hasThreeConsecutiveLowerHighs = sc.High[sc.Index] < sc.High[sc.Index - 1] && sc.High[sc.Index - 1] < sc.High[sc.Index - 2];
    bool hasThreeConsecutiveLowerLows = sc.Low[sc.Index] < sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] < sc.Low[sc.Index - 2];

    if (areTradingHours & hasThreeConsecutiveLowerHighs && hasThreeConsecutiveLowerLows & currentBarHasClosed) {
        subgraphDownTrend[sc.Index] = sc.Close[sc.Index];
        orderSuccesCheck = (int)sc.SellEntry(order);
    }

    if (isTimeToFlat) {
        sc.FlattenPosition();
    }
}

SCSFExport scsf_SimpleTrendPattern(SCStudyInterfaceRef sc) {
    SCSubgraphRef subgraphUpTrend = sc.Subgraph[0];
    SCSubgraphRef subgraphDownTrend = sc.Subgraph[1];

    SCInputRef inputStartTradingAt = sc.Input[0];
    SCInputRef inputStopTradingAt = sc.Input[1];
    SCInputRef inputFlatPostionAt = sc.Input[2];

    if (sc.SetDefaults) {
        sc.GraphName = "Simple Trend Pattern";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        setInputs(inputStartTradingAt, inputStopTradingAt, inputFlatPostionAt);
        setSubgraphs(subgraphUpTrend, subgraphDownTrend);
        setTradingOptions(sc);
        return;
    }

    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[0], MOVAVGTYPE_EXPONENTIAL, 7);
    float movingAvgAtCurBar = sc.Subgraph[0][sc.Index];

    sc.AddMessageToLog(std::to_string(movingAvgAtCurBar).c_str(), 1);

    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > inputStartTradingAt.GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < inputStopTradingAt.GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= inputFlatPostionAt.GetTime();

    // takeLong(sc, subgraphUpTrend, areTradingHours, isTimeToFlat);
    // takeShort(sc, subgraphDownTrend, areTradingHours, isTimeToFlat);
}
