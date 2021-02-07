// simple trend pattern is three candelstick, each has the high and low higher then the preceding
// shows the entry bar colored in green/red based on the direction
// idea - use it on range bars -> pt/sl is the range bar..

#include "sierrachart.h"

SCDLLName("Simple Trend Pattern");

void setSubgraphs(SCSubgraphRef &Subgraph_upTrend,
                  SCSubgraphRef &Subgraph_downTrend
                //   SCSubgraphRef &Subgraph_buyEntry,
                //   SCSubgraphRef &Subgraph_buyExit,
                //   SCSubgraphRef &Subgraph_sellEntry,
                //   SCSubgraphRef &Subgraph_sellExit
                  ) {
    Subgraph_upTrend.Name = "Up trend pattern";
    Subgraph_upTrend.DrawStyle = DRAWSTYLE_ARROW_UP;
    Subgraph_upTrend.PrimaryColor = RGB(0, 0, 255);

    Subgraph_downTrend.Name = "Down trend pattern";
    Subgraph_downTrend.DrawStyle = DRAWSTYLE_ARROW_DOWN;
    Subgraph_downTrend.PrimaryColor = RGB(0, 0, 255);

    // Subgraph_buyEntry.Name = "Buy Entry";
    // Subgraph_buyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
    // Subgraph_buyEntry.PrimaryColor = RGB(0, 255, 0);
    // Subgraph_buyEntry.LineWidth = 2;
    // Subgraph_buyEntry.DrawZeros = false;

    // Subgraph_buyExit.Name = "Buy Exit";
    // Subgraph_buyExit.DrawStyle = DRAWSTYLE_ARROW_DOWN;
    // Subgraph_buyExit.PrimaryColor = RGB(255, 128, 128);
    // Subgraph_buyExit.LineWidth = 2;
    // Subgraph_buyExit.DrawZeros = false;

    // Subgraph_sellEntry.Name = "Sell Entry";
    // Subgraph_sellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
    // Subgraph_sellEntry.PrimaryColor = RGB(255, 0, 0);
    // Subgraph_sellEntry.LineWidth = 2;
    // Subgraph_sellEntry.DrawZeros = false;

    // Subgraph_sellExit.Name = "Sell Exit";
    // Subgraph_sellExit.DrawStyle = DRAWSTYLE_ARROW_UP;
    // Subgraph_sellExit.PrimaryColor = RGB(128, 255, 128);
    // Subgraph_sellExit.LineWidth = 2;
    // Subgraph_sellExit.DrawZeros = false;
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

SCSFExport scsf_SimpleTrendPattern(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_upTrend = sc.Subgraph[0];
    SCSubgraphRef Subgraph_downTrend = sc.Subgraph[1];
    SCSubgraphRef Subgraph_sellEntry = sc.Subgraph[2];
    SCSubgraphRef Subgraph_sellExit = sc.Subgraph[3];
    SCSubgraphRef Subgraph_buyEntry = sc.Subgraph[4];
    SCSubgraphRef Subgraph_buyExit = sc.Subgraph[5];

    if (sc.SetDefaults) {
        sc.GraphName = "Simple Trend Pattern";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        setSubgraphs(Subgraph_upTrend, Subgraph_downTrend);
        // setTradingOptions(sc);

        return;
    }
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

    float lastTradePrice = sc.Close[sc.Index];
    s_SCPositionData currentPosition;
    sc.GetTradePosition(currentPosition);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Simple trend pattern";
    int64_t &buyEntryInternalOrderID = sc.GetPersistentInt64(1);
    int orderSuccesCheck = 0;

    bool currentBarHasClosed = sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
    bool hasThreeConsecutiveHigherHighs = sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2];
    bool hasThreeConsecutiveHigherLows = sc.Low[sc.Index] > sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] > sc.Low[sc.Index - 2];
    bool hasThreeConsecutiveLowerHighs = sc.High[sc.Index] < sc.High[sc.Index - 1] && sc.High[sc.Index - 1] < sc.High[sc.Index - 2];
    bool hasThreeConsecutiveLowerLows = sc.Low[sc.Index] < sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] < sc.Low[sc.Index - 2];

    if (hasThreeConsecutiveHigherHighs && hasThreeConsecutiveHigherLows & currentBarHasClosed) {
        Subgraph_upTrend[sc.Index] = sc.Close[sc.Index];
        orderSuccesCheck = (int)sc.BuyEntry(order);
        buyEntryInternalOrderID = order.InternalOrderID;
        SCString internalOrderIDNumberString;
        internalOrderIDNumberString.Format("BuyEntry Internal Order ID: %d", buyEntryInternalOrderID);
        sc.AddMessageToLog(internalOrderIDNumberString, 0);
        Subgraph_buyEntry[sc.Index] = sc.Low[sc.Index];
    }

    // if (hasThreeConsecutiveLowerHighs && hasThreeConsecutiveLowerLows) {
    //     Subgraph_downTrend[sc.Index] = sc.Index;
    // }
}
