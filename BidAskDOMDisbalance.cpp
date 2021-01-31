// checking the moments when quotting values on Bid vs. Ask (for defined number of levels) on DOM exceeds a defined values

#include <vector>;

#include "sierrachart.h"

SCDLLName("Bid Ask DOM Disbalance")

    SCSFExport scsf_DOMBidAskDisbalance(SCStudyInterfaceRef sc) {
    const int LEVELS = 5;

    // arrays holding ask and bid dom values for N last indexes
    SCSubgraphRef askArrayDOM = sc.Subgraph[0];
    SCSubgraphRef bidArrayDOM = sc.Subgraph[1];

    // arrays holding output values that will be shown on the chart
    SCSubgraphRef outputAsk = sc.Subgraph[10];
    SCSubgraphRef outputBid = sc.Subgraph[11];

    if (sc.SetDefaults) {
        sc.GraphName = "DOM Bid/Ask Disbaance";

        for (int i = 0; i <= LEVELS; ++i) askArrayDOM.Arrays[i];
        for (int i = 0; i <= LEVELS; ++i) bidArrayDOM.Arrays[i];
        askArrayDOM.DrawStyle = DRAWSTYLE_IGNORE;
        bidArrayDOM.DrawStyle = DRAWSTYLE_IGNORE;

        outputBid.Name = "Ask";
        outputAsk.LineLabel |= LL_DISPLAY_NAME | LL_DISPLAY_VALUE;
        outputAsk.DrawStyle = DRAWSTYLE_LINE;
        outputAsk.PrimaryColor = RGB(0, 255, 0);

        outputBid.Name = "Bid";
        outputBid.LineLabel |= LL_DISPLAY_NAME | LL_DISPLAY_VALUE;
        outputBid.DrawStyle = DRAWSTYLE_LINE;
        outputBid.PrimaryColor = RGB(255, 0, 0);

        sc.UsesMarketDepthData = TRUE;
        sc.AutoLoop = 1;

        return;
    }

    s_MarketDepthEntry askOnDomLevelOne;
    s_MarketDepthEntry askOnDomLevelTwo;
    s_MarketDepthEntry askOnDomLevelThree;
    s_MarketDepthEntry askOnDomLevelFour;

    for (int i = 0; i <= LEVELS; ++i) {
    }
}

std::vector<std::string> createMarketDeptEntriesVariables(const int howMany) {
    std::string askDomAtLevel = "";
    std::string bidDomAtLevel = "";

    for (int i = 0; i <= howMany; ++i) {
    }
}
