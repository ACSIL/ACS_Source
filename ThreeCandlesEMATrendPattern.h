
#include "ThreeCandlesTrendPattern.h"
#include "sierrachart.h"

namespace aos {
namespace trend {

class ThreeCandlesEMA : public ThreeCandles {
   public:
    bool isUp(SCStudyInterfaceRef &sc);
    bool isDown(SCStudyInterfaceRef &sc);

    void setEmaPeriod(const int &emaPeriod);
    void showEmaSubgraph(SCStudyInterfaceRef &sc, SCSubgraphRef &subgraphEMA);

   private:
    int emaPeriod;

    bool emaAboveCurrentPrice(SCStudyInterfaceRef &sc);
    bool emaBellowCurrentPrice(SCStudyInterfaceRef &sc);
    bool emaAboveHighOfCurrentBar(SCStudyInterfaceRef &sc);
    bool emaBellowLowOfCurrentBar(SCStudyInterfaceRef &sc);
};

bool ThreeCandlesEMA::isUp(SCStudyInterfaceRef &sc) {
    return ThreeCandles::isUp(sc) && emaBellowLowOfCurrentBar(sc);
}

bool ThreeCandlesEMA::isDown(SCStudyInterfaceRef &sc) {
    return ThreeCandles::isUp(sc) && emaAboveHighOfCurrentBar(sc);
}

inline void ThreeCandlesEMA::setEmaPeriod(const int &emaPeriod) {
    this->emaPeriod = emaPeriod;
}

inline void ThreeCandlesEMA::showEmaSubgraph(SCStudyInterfaceRef &sc, SCSubgraphRef subgraphEMA) {
    sc.ExponentialMovAvg(sc.BaseDataIn[SC_LAST], subgraphEMA.Data, emaPeriod);
}

inline bool ThreeCandlesEMA::emaAboveCurrentPrice(SCStudyInterfaceRef &sc) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[0], MOVAVGTYPE_EXPONENTIAL, emaPeriod);
    float EmaAtCurBar = sc.Subgraph[0][sc.Index];
    return EmaAtCurBar > sc.Close[sc.Index];
}

inline bool ThreeCandlesEMA::emaBellowCurrentPrice(SCStudyInterfaceRef &sc) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[1], MOVAVGTYPE_EXPONENTIAL, emaPeriod);
    float EmaAtCurBar = sc.Subgraph[1][sc.Index];
    return EmaAtCurBar < sc.Close[sc.Index];
}

inline bool ThreeCandlesEMA::emaAboveHighOfCurrentBar(SCStudyInterfaceRef &sc) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[3], MOVAVGTYPE_EXPONENTIAL, emaPeriod);
    float EmaAtCurBar = sc.Subgraph[3][sc.Index];
    return EmaAtCurBar > sc.High[sc.Index];
}

inline bool ThreeCandlesEMA::emaBellowLowOfCurrentBar(SCStudyInterfaceRef &sc) {
    sc.MovingAverage(sc.BaseDataIn[SC_LAST], sc.Subgraph[4], MOVAVGTYPE_EXPONENTIAL, emaPeriod);
    float EmaAtCurBar = sc.Subgraph[4][sc.Index];
    return EmaAtCurBar < sc.Low[sc.Index];
}

}  // namespace trend
}  // namespace aos