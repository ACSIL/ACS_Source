#pragma once

#include <fstream>
#include <iostream>

#include "sierrachart.h"

namespace aos {
namespace trend {

class Trend {
   private:
    const int OFFSET = 3;  // pattern is formed by three candles -> looking 3 bars back

   public:
    bool isUp(SCStudyInterfaceRef &sc, const int &period);
    bool isDown(SCStudyInterfaceRef &sc, const int &period);
    void showEma(SCStudyInterfaceRef &sc);

   private:
    bool threeConsecutiveHigherHighs(SCStudyInterfaceRef &sc);
    bool threeConsecutiveHigherLows(SCStudyInterfaceRef &sc);
    bool threeConsecutiveLowerHighs(SCStudyInterfaceRef &sc);
    bool threeConsecutiveLowerLows(SCStudyInterfaceRef &sc);

    bool emaAboveCurrentPrice(SCStudyInterfaceRef &sc, const int &period);
    bool emaBellowCurrentPrice(SCStudyInterfaceRef &sc, const int &period);
    bool emaAboveHighOfCurrentBar(SCStudyInterfaceRef &sc, const int &period);
    bool emaBellowLowOfCurrentBar(SCStudyInterfaceRef &sc, const int &period);
    bool barHasClosed(SCStudyInterfaceRef &sc);
    bool areBarsInDaySession(SCStudyInterfaceRef &sc);
};

inline bool Trend::isUp(SCStudyInterfaceRef &sc, const int &period) {
    return threeConsecutiveHigherHighs(sc) && threeConsecutiveHigherLows(sc) && emaBellowLowOfCurrentBar(sc, period) && areBarsInDaySession(sc);
}

inline bool Trend::isDown(SCStudyInterfaceRef &sc, const int &period) {
    return threeConsecutiveLowerHighs(sc) && threeConsecutiveLowerLows(sc) && emaAboveHighOfCurrentBar(sc, period) && areBarsInDaySession(sc);
}

inline bool Trend::threeConsecutiveHigherHighs(SCStudyInterfaceRef &sc) {
    return sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2];
}

inline bool Trend::threeConsecutiveHigherLows(SCStudyInterfaceRef &sc) {
    return sc.Low[sc.Index] > sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] > sc.Low[sc.Index - 2];
}

inline bool Trend::threeConsecutiveLowerHighs(SCStudyInterfaceRef &sc) {
    return sc.High[sc.Index] < sc.High[sc.Index - 1] && sc.High[sc.Index - 1] < sc.High[sc.Index - 2];
}

inline bool Trend::threeConsecutiveLowerLows(SCStudyInterfaceRef &sc) {
    return sc.Low[sc.Index] < sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] < sc.Low[sc.Index - 2];
}

inline bool Trend::emaAboveCurrentPrice(SCStudyInterfaceRef &sc, const int &period) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[0], MOVAVGTYPE_EXPONENTIAL, period);
    float EmaAtCurBar = sc.Subgraph[0][sc.Index];
    return EmaAtCurBar > sc.Close[sc.Index];
}

inline bool Trend::emaBellowCurrentPrice(SCStudyInterfaceRef &sc, const int &period) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[1], MOVAVGTYPE_EXPONENTIAL, period);
    float EmaAtCurBar = sc.Subgraph[1][sc.Index];
    return EmaAtCurBar < sc.Close[sc.Index];
}

inline bool Trend::emaAboveHighOfCurrentBar(SCStudyInterfaceRef &sc, const int &period) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[3], MOVAVGTYPE_EXPONENTIAL, period);
    float EmaAtCurBar = sc.Subgraph[3][sc.Index];
    return EmaAtCurBar > sc.High[sc.Index];
}

inline bool Trend::emaBellowLowOfCurrentBar(SCStudyInterfaceRef &sc, const int &period) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[4], MOVAVGTYPE_EXPONENTIAL, period);
    float EmaAtCurBar = sc.Subgraph[4][sc.Index];
    return EmaAtCurBar < sc.Low[sc.Index];
}

inline bool Trend::barHasClosed(SCStudyInterfaceRef &sc) {
    return sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
}

inline bool Trend::areBarsInDaySession(SCStudyInterfaceRef &sc) {
    return sc.IsDateTimeInDaySession(sc.BaseDateTimeIn[sc.Index - OFFSET]);
}



}  // namespace trendaos
}  // namespace system
