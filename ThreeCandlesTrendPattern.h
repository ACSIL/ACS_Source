#pragma once

#include <fstream>
#include <iostream>

#include "sierrachart.h"

namespace aos {
namespace trend {

class ThreeCandles {
   private:
    const int OFFSET = 3;  // pattern is formed by three candles -> looking 3 bars back

   public:
    bool isUp(SCStudyInterfaceRef &sc);
    bool isDown(SCStudyInterfaceRef &sc);

   protected:
    bool threeConsecutiveHigherHighs(SCStudyInterfaceRef &sc);
    bool threeConsecutiveHigherLows(SCStudyInterfaceRef &sc);
    bool threeConsecutiveLowerHighs(SCStudyInterfaceRef &sc);
    bool threeConsecutiveLowerLows(SCStudyInterfaceRef &sc);
    bool barHasClosed(SCStudyInterfaceRef &sc);
    bool areBarsInDaySession(SCStudyInterfaceRef &sc);
};

inline bool ThreeCandles::isUp(SCStudyInterfaceRef &sc) {
    return  areBarsInDaySession(sc) && threeConsecutiveHigherHighs(sc) && threeConsecutiveHigherLows(sc);
}

inline bool ThreeCandles::isDown(SCStudyInterfaceRef &sc) {
    return areBarsInDaySession(sc) && threeConsecutiveLowerHighs(sc) && threeConsecutiveLowerLows(sc);
}

inline bool ThreeCandles::threeConsecutiveHigherHighs(SCStudyInterfaceRef &sc) {
    return sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2] && barHasClosed(sc);;
}

inline bool ThreeCandles::threeConsecutiveHigherLows(SCStudyInterfaceRef &sc) {
    return sc.Low[sc.Index] > sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] > sc.Low[sc.Index - 2] && barHasClosed(sc);
}

inline bool ThreeCandles::threeConsecutiveLowerHighs(SCStudyInterfaceRef &sc) {
    return sc.High[sc.Index] < sc.High[sc.Index - 1] && sc.High[sc.Index - 1] < sc.High[sc.Index - 2] && barHasClosed(sc);
}

inline bool ThreeCandles::threeConsecutiveLowerLows(SCStudyInterfaceRef &sc) {
    return sc.Low[sc.Index] < sc.Low[sc.Index - 1] && sc.Low[sc.Index - 1] < sc.Low[sc.Index - 2] && barHasClosed(sc);;
}

inline bool ThreeCandles::barHasClosed(SCStudyInterfaceRef &sc) {
    return sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED;
}

inline bool ThreeCandles::areBarsInDaySession(SCStudyInterfaceRef &sc) {
    return sc.IsDateTimeInDaySession(sc.BaseDateTimeIn[sc.Index - OFFSET]);
}

}  // namespace trend
}  // namespace aos
