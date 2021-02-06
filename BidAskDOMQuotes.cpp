#include "sierrachart.h"
SCDLLName("Bid Ask DOM Quotes")

    SCSFExport scsf_BidAskDomQuotesNumberRefined(SCStudyInterfaceRef sc) {
    // arrays holding ask and bid dom values for N last indexes
    SCSubgraphRef askArrayDOM = sc.Subgraph[0];
    SCSubgraphRef bidArrayDOM = sc.Subgraph[1];

    // arrays holding output values that will be shown on the chart
    SCSubgraphRef outputSubgraphAsk = sc.Subgraph[10];
    SCSubgraphRef outputSubgraphBid = sc.Subgraph[11];

    // input for setting how many levels to calculate
    SCInputRef levels = sc.Input[0];
    SCInputRef indexes = sc.Input[1];

    if (sc.SetDefaults) {
        sc.GraphName = "DOM Bid/Ask Quotes Number - re-refined";

        levels.Name = "Number of DOM levels";
        levels.SetInt(10);
        levels.SetIntLimits(1, 100);

        indexes.Name = "Number of indexes back to calculate the average";
        indexes.SetInt(10);
        indexes.SetIntLimits(1, 100);

        for (int i = 0; i <= levels.GetInt(); ++i) askArrayDOM.Arrays[i];
        for (int i = 0; i <= levels.GetInt(); ++i) bidArrayDOM.Arrays[i];
        askArrayDOM.DrawStyle = DRAWSTYLE_IGNORE;
        bidArrayDOM.DrawStyle = DRAWSTYLE_IGNORE;

        // set how the output subgraph will be plotted
        outputSubgraphAsk.Name = "Ask";
        outputSubgraphAsk.LineLabel |= LL_DISPLAY_NAME | LL_DISPLAY_VALUE;
        outputSubgraphAsk.DrawStyle = DRAWSTYLE_LINE;
        outputSubgraphAsk.PrimaryColor = RGB(0, 255, 0);

        outputSubgraphBid.Name = "Bid";
        outputSubgraphBid.LineLabel |= LL_DISPLAY_NAME | LL_DISPLAY_VALUE;
        outputSubgraphBid.DrawStyle = DRAWSTYLE_LINE;
        outputSubgraphBid.PrimaryColor = RGB(255, 0, 0);

        sc.UsesMarketDepthData = TRUE;
        sc.AutoLoop = 1;
        return;
    }

    std::vector<s_MarketDepthEntry> numberOfAskLevelsOnDOM(levels.GetInt());
    std::vector<s_MarketDepthEntry> numberOfBidLevelsOnDOM(levels.GetInt());

    for (int level = 0; level < levels.GetInt(); ++level)
        sc.GetAskMarketDepthEntryAtLevel(numberOfAskLevelsOnDOM.at(level), level);
    for (int level = 0; level < levels.GetInt(); ++level)
        sc.GetBidMarketDepthEntryAtLevel(numberOfBidLevelsOnDOM.at(level), level);

    auto askSum = 0;
    auto bidSum = 0;
    for (int level = 0; level < levels.GetInt(); ++level) {
        auto askAtLevel = (float)numberOfAskLevelsOnDOM[level].Quantity;
        auto bidAtLevel = (float)numberOfBidLevelsOnDOM[level].Quantity;
        askSum += askAtLevel;
        bidSum += bidAtLevel;
    }

    askArrayDOM[sc.Index] = (float)askSum;
    bidArrayDOM[sc.Index] = (float)bidSum;

    // make sum from the last N indexes for bids and asks
    auto asks = askArrayDOM[sc.Index];
    auto bids = bidArrayDOM[sc.Index];

    for (int index = 0; index < indexes.GetInt(); ++index) {
        asks += askArrayDOM[sc.Index - index];
        bids += bidArrayDOM[sc.Index - index];
    }

    outputSubgraphAsk[sc.Index] = asks;
    outputSubgraphBid[sc.Index] = bids;
}
