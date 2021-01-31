#include "sierrachart.h"
SCDLLName("Tapereader (WTR)");

SCSFExport scsf_TapereaderClassic(SCStudyInterfaceRef sc) {
    SCSubgraphRef Buyers = sc.Subgraph[0];
    SCSubgraphRef Sellers = sc.Subgraph[1];
    SCSubgraphRef Reference = sc.Subgraph[2];

    if (sc.SetDefaults) {
        sc.GraphName = "Tapereader Classic";
        sc.StudyDescription = "Tapereader - classic WTR";
        sc.GraphRegion = 1;

        sc.AutoLoop = 1;

        Buyers.Name = "Buyers";
        Buyers.PrimaryColor = RGB(0, 255, 0);
        Buyers.DrawStyle = DRAWSTYLE_LINE;
        Buyers.LineWidth = 1;

        Sellers.Name = "Sellers";
        Sellers.PrimaryColor = RGB(255, 0, 0);
        Sellers.DrawStyle = DRAWSTYLE_LINE;
        Sellers.LineWidth = 1;

        Reference.Name = "Reference";
        Reference.PrimaryColor = RGB(128, 128, 255);
        Reference.DrawStyle = DRAWSTYLE_LINE;
        Reference.LineWidth = 1;
    }

    float Ask = sc.AskVolume[sc.Index] * 2;
    float Bid = sc.BidVolume[sc.Index] * 2;

    for (int n = 1; n < 10; n++) {
        Ask += sc.AskVolume[sc.Index - n];
        Bid += sc.BidVolume[sc.Index - n];
    }

    Buyers[sc.Index] = Ask * 6;
    Sellers[sc.Index] = Bid * 6;

    auto ref = sc.Volume[sc.Index] * 2;

    for (int m = 1; m < 60; m++)
        ref += sc.Volume[sc.Index - m];

    Reference[sc.Index] = ref;
}

SCSFExport scsf_TapereaderExtremes(SCStudyInterfaceRef sc) {
    SCSubgraphRef Buyers = sc.Subgraph[0];
    SCSubgraphRef Sellers = sc.Subgraph[1];
    SCSubgraphRef Reference = sc.Subgraph[2];
    SCSubgraphRef ExtremBuy = sc.Subgraph[3];
    SCSubgraphRef ExtremSell = sc.Subgraph[4];

    if (sc.SetDefaults) {
        sc.GraphName = "Tapereader Extrems";
        sc.StudyDescription = "Presna kopie Wemovo WTR";
        sc.GraphRegion = 1;

        sc.AutoLoop = 1;

        ExtremBuy.Name = "Extrem Buyers";
        ExtremBuy.PrimaryColor = RGB(0, 200, 0);
        ExtremBuy.DrawStyle = DRAWSTYLE_FILL_TO_ZERO;
        ExtremBuy.LineWidth = 1;

        ExtremSell.Name = "Extrem Sellers";
        ExtremSell.PrimaryColor = RGB(200, 0, 0);
        ExtremSell.DrawStyle = DRAWSTYLE_FILL_TO_ZERO;
        ExtremSell.LineWidth = 1;
    }

    auto Ask = sc.AskVolume[sc.Index] * 2;
    auto Bid = sc.BidVolume[sc.Index] * 2;

    for (int n = 1; n < 10; n++) {
        Ask += sc.AskVolume[sc.Index - n];
        Bid += sc.BidVolume[sc.Index - n];
    }

    auto Ref = sc.Volume[sc.Index] * 2;

    for (int m = 1; m < 60; m++) {
        Ref += sc.Volume[sc.Index - m];
    }

    Buyers[sc.Index] = Ask * 6;
    Sellers[sc.Index] = Bid * 6;
    Reference[sc.Index] = Ref;

    if (Buyers[sc.Index] > Reference[sc.Index])
        ExtremBuy[sc.Index] = Buyers[sc.Index];
    if (Sellers[sc.Index] > Reference[sc.Index])
        ExtremSell[sc.Index] = -Sellers[sc.Index];
}

SCSFExport scsf_TapereaderDelta(SCStudyInterfaceRef sc) {
    SCSubgraphRef Buyers = sc.Subgraph[0];
    SCSubgraphRef Sellers = sc.Subgraph[1];
    SCSubgraphRef Reference = sc.Subgraph[2];
    SCSubgraphRef WTRDelta = sc.Subgraph[3];

    if (sc.SetDefaults) {
        sc.GraphName = "Tapereader Delta";
        sc.StudyDescription = "Delta WTR";
        sc.GraphRegion = 1;

        sc.AutoLoop = 1;

        WTRDelta.Name = "Tapereader Delta";
        WTRDelta.AutoColoring = AUTOCOLOR_POSNEG;
        WTRDelta.PrimaryColor = RGB(0, 200, 0);
        WTRDelta.SecondaryColor = RGB(200, 0, 0);
        WTRDelta.DrawStyle = DRAWSTYLE_FILL_TO_ZERO;
        WTRDelta.LineWidth = 1;
    }

    float ask = sc.AskVolume[sc.Index] * 2;
    float bid = sc.BidVolume[sc.Index] * 2;

    for (int n = 1; n < 10; n++) {
        ask += sc.AskVolume[sc.Index - n];
        bid += sc.BidVolume[sc.Index - n];
    }

    float ref = sc.Volume[sc.Index] * 2;

    for (int m = 1; m < 60; m++)
        ref = ref + sc.Volume[sc.Index - m];

    Buyers[sc.Index] = ask * 6;
    Sellers[sc.Index] = bid * 6;
    Reference[sc.Index] = ref;

    WTRDelta[sc.Index] = Buyers[sc.Index] - Sellers[sc.Index];
}
