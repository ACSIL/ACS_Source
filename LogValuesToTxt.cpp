#include <fstream>
#include <string>

#include "sierrachart.h"

// //string for internal and external log
// SCString logValues(SCStudyInterfaceRef sc) {
//     s_SCPositionData current_postion;
//     sc.GetTradePosition(current_postion);
//     nq.entry_price = current_postion.AveragePrice;
//     double entry_dt = current_postion.LastFillDateTime;
//     DATETIME_TO_YMDHMS(entry_dt, nq.year, nq.month, nq.day, nq.hour, nq.minute, nq.second);

//     SCString log;
//     log.Format("Cena: %f, Date and Time: %i-%i-%i  %0.2i:%0.2i:%0.2i | Daily range (in ticks): %f | ATR: %0.04f",
//                nq.entry_price, nq.year, nq.month, nq.day, nq.hour, nq.minute, nq.second, nq.range, nq.atr_val);
//     return log;
// }

// //external log into txt file
// void logToFile(SCStudyInterfaceRef sc) {
//     std::string fileName{"ES trades.txt"};  // TODO dynamicke jmeno ktere bude odpovidat symbolu
//     std::ofstream outFile{};
//     outFile.open(fileName, std::ofstream::app);
//     if (outFile.is_open()) {
//         std::string "hello";
//         outFile << test_logu << '\n';
//     }
//     // else {
//     //     SCString log_error{"Error writing into file"};
//     //     sc.AddMessageToLog(log_error, 1);
//     // }
//     outFile.close();
// }

SCDLLName("Logger")

    SCSFExport scsf_LogEntryDetailsToFile(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        sc.GraphName = "Logger";
        return;
    }

    s_SCPositionData position;
    sc.GetTradePosition(position);

    SCString stringToLog;
    stringToLog.Format("Position Price: %.02f", position.AveragePrice);

    int& p_logEntry = sc.GetPersistentInt(0);
    int& p_previousPositionQty = sc.GetPersistentInt(1);

    bool positionClosed = position.PositionQuantity == 0;
    bool positionOpened = p_previousPositionQty == 0 && position.PositionQuantity != 0;
    bool logEntryNotYetDone = p_logEntry == 0;
    
    if (positionOpened && logEntryNotYetDone) {
        sc.AddMessageToLog(stringToLog, 1);
        p_logEntry = 1;
    }
    if (positionClosed){
        p_logEntry = 0;
    }

    
}