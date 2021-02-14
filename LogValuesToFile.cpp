#include <fstream>
#include <string>

#include "sierrachart.h"


SCDLLName("Logger");

void printToMessageLog(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta) {
    int &p_logEntryDone = sc.GetPersistentInt(0);
    int &p_previousPositionQty = sc.GetPersistentInt(1);

    bool positionClosed = position.PositionQuantity == 0;
    bool newPositionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntryDone == 0;

    float volumeOnPrevBar = volume[volume.GetArraySize() - 2];
    float atrOnPrevBar = atr[atr.GetArraySize() - 2];
    float cumDeltaOnPrevBar = cumDelta[cumDelta.GetArraySize() - 2];

    SCString stringToLog;
    stringToLog.Format("Entry Price: %.02f, Volume: %.02f, ATR (exponencial, 7): %.02f, Cum Delta (from start of trading day) %.02f",
                       position.AveragePrice, volumeOnPrevBar, atrOnPrevBar, cumDeltaOnPrevBar);

    if (newPositionOpened && logEntryNotYetDone) {
        sc.AddMessageToLog(stringToLog, 1);
        p_logEntryDone = 1;
    }
    if (positionClosed) {
        p_logEntryDone = 0;
    }
}

// dat do hlavicky utils
std::string convertScStringToStdString(SCString input) {
    std::string output{input};
    return output;
}

void printToCSVFile(SCStudyInterfaceRef &sc, s_SCPositionData &position, SCFloatArray &volume, SCFloatArray &atr, SCFloatArray &cumDelta) {
    int &p_logEntryDone = sc.GetPersistentInt(2);
    int &p_previousPositionQty = sc.GetPersistentInt(3);

    bool positionClosed = position.PositionQuantity == 0;
    bool newPositionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntryDone == 0;

    float volumeOnPrevBar = volume[volume.GetArraySize() - 2];
    float atrOnPrevBar = atr[atr.GetArraySize() - 2];
    float cumDeltaOnPrevBar = cumDelta[cumDelta.GetArraySize() - 2];

    SCDateTime entryDateTime = position.LastEntryDateTime;
    int year, month, day, hour, minute, second;
    entryDateTime.GetDateTimeYMDHMS(year, month, day, hour, minute, second);

    SCString stringToLog;
    stringToLog.Format(
        "%i-%i-%i %0.2i:%0.2i:%0.2i,"
        "%3.2f,"
        "%s,"
        "%3.0f,"
        "%.02f,"
        "%.02f,"
        "%.02f,",
        year, month, day, hour, minute, second,
        position.AveragePrice,
        convertScStringToStdString(position.Symbol),
        position.PositionQuantity,
        volumeOnPrevBar,
        atrOnPrevBar,
        cumDeltaOnPrevBar);

    std::string fileName{"ES_log.csv"};  // variable pres input
    std::ofstream outFile{};
    outFile.open(fileName, std::ofstream::app);
    if (newPositionOpened && logEntryNotYetDone) {
        sc.AddMessageToLog("New Pos opened", 1);
        if (outFile.is_open()) {
            sc.AddMessageToLog("Opened file", 1);
            std::string log{stringToLog};
            outFile << log << '\n';
        } else {
            sc.AddMessageToLog("Error writing into file", 1);
        }
        outFile.close();
    }
    p_logEntryDone = 1;

    if (positionClosed) {
        p_logEntryDone = 0;
    }
}

class Params {
   public:
    SCFloatArray volume;
    SCFloatArray atr;
    SCFloatArray cumDelta;

    Params(SCFloatArray volume, SCFloatArray atr, SCFloatArray cumDelta) {
        this->volume = volume;
        this->atr = atr;
        this->cumDelta = cumDelta;
    }
};

SCSFExport scsf_LogEntryDetails(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.Input[1].Name = "Volume";
        sc.Input[1].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[2].Name = "ATR";
        sc.Input[2].SetChartStudySubgraphValues(0, 0, 0);
        sc.Input[3].Name = "Cumulative delta";
        sc.Input[3].SetChartStudySubgraphValues(0, 0, 0);

        sc.GraphRegion = 0;
        sc.GraphName = "Logger";
        return;
    }

    s_SCPositionData position;
    sc.GetTradePosition(position);

    SCFloatArray volume;
    SCFloatArray atr;
    SCFloatArray cumDelta;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), sc.Input[1].GetSubgraphIndex(), volume);
    sc.GetStudyArrayFromChartUsingID(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), sc.Input[2].GetSubgraphIndex(), atr);
    sc.GetStudyArrayFromChartUsingID(sc.Input[2].GetChartNumber(), sc.Input[3].GetStudyID(), sc.Input[3].GetSubgraphIndex(), cumDelta);

    printToCSVFile(sc, position, volume, atr, cumDelta);
    printToMessageLog(sc, position, volume, atr, cumDelta);
}
