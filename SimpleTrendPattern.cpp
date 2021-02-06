// simple trend pattern is three candelstick, each has the high and low higher then the preceding
// shows the entry bar colored in green/red based on the direction
// idea - use it on range bars -> pt/sl is the range bar..

#include "sierrachart.h"

SCDLLName("Simple Trend Pattern");

SCSFExport scsf_SimpleTrendPattern(SCStudyInterfaceRef sc) {
    SCSubgraphRef upTrend = sc.Subgraph[0];
    SCSubgraphRef downTrend = sc.Subgraph[1];

    if (sc.SetDefaults) {
        sc.GraphName = "Simple Trend Pattern";
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        upTrend.Name = "Up trend pattern";
        upTrend.DrawStyle = DRAWSTYLE_COLOR_BAR;
        upTrend.PrimaryColor = RGB(0, 255, 0);

        downTrend.Name = "Up trend pattern";
        downTrend.DrawStyle = DRAWSTYLE_COLOR_BAR;
        downTrend.PrimaryColor = RGB(255, 0, 0);

        return;
    }
    // bool barHasEntrySignal = false;
    bool hasThreeConsecutiveHigherHighs = sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2];
    bool hasThreeConsecutiveHigherLows = sc.Low[sc.Index] > sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] > sc.Low[sc.Index - 2];

    if (hasThreeConsecutiveHigherHighs && hasThreeConsecutiveHigherLows) {
        upTrend[sc.Index] = sc.Index;
    }

    bool hasThreeConsecutiveLowerHighs = sc.High[sc.Index] < sc.High[sc.Index - 1] && sc.High[sc.Index - 1] < sc.High[sc.Index - 2];
    bool hasThreeConsecutiveLowerLows = sc.Low[sc.Index] < sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] < sc.Low[sc.Index - 2];

    if (hasThreeConsecutiveLowerHighs && hasThreeConsecutiveLowerLows) {
        downTrend[sc.Index] = sc.Index;
    }

    
}