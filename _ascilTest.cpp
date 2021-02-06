#include "sierrachart.h"

SCDLLName("Test")

SCSFExport scsf_InsideBar(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];

    // Set configuration variables

    if (sc.SetDefaults) {
        // Set the configuration and defaults

        sc.GraphName = "Inside Bar";

        sc.GraphRegion = 0;

        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(255, 128, 0);

        sc.AutoLoop = 1;

        return;
    }

    // Array references
    SCFloatArrayRef High = sc.High;
    SCFloatArrayRef Low = sc.Low;

    // Do data processing

    if (High[sc.Index] > 3850)
    Subgraph_IB[sc.Index] = High[sc.Index];
}