
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
    s_SCNewOrder& exitAtEMACross(SCStudyInterfaceRef sc, s_SCNewOrder& order);


   private:
    int emaPeriod;
    bool emaAboveCurrentPrice(SCStudyInterfaceRef &sc);
    bool emaBellowCurrentPrice(SCStudyInterfaceRef &sc);
    bool emaAboveHighOfCurrentBar(SCStudyInterfaceRef &sc);
    bool emaBellowLowOfCurrentBar(SCStudyInterfaceRef &sc);
};

bool ThreeCandlesEMA::isUp(SCStudyInterfaceRef &sc) {
    return emaBellowLowOfCurrentBar(sc) && areBarsInDaySession(sc) && threeConsecutiveHigherHighs(sc) && threeConsecutiveHigherLows(sc);
}

bool ThreeCandlesEMA::isDown(SCStudyInterfaceRef &sc) {
    return emaAboveHighOfCurrentBar(sc) && areBarsInDaySession(sc) && threeConsecutiveLowerHighs(sc) && threeConsecutiveLowerLows(sc);
}

inline void ThreeCandlesEMA::setEmaPeriod(const int &emaPeriod) {
    this->emaPeriod = emaPeriod;
}

inline void ThreeCandlesEMA::showEmaSubgraph(SCStudyInterfaceRef &sc, SCSubgraphRef subgraphEMA) {
    sc.ExponentialMovAvg(sc.BaseDataIn[SC_LAST], subgraphEMA.Data, emaPeriod);
}

inline s_SCNewOrder& ThreeCandlesEMA::exitAtEMACross(SCStudyInterfaceRef sc, s_SCNewOrder& order){
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.TextTag = "Three Candles Trend Pattern";

    return order;
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