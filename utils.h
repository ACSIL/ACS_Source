
#include "sierrachart.h"

namespace utils {

class Logger {
   public:
    static void log(SCStudyInterfaceRef& sc, SCDateTime value, SCString comment = "") {
        SCString s = sc.FormatDateTime(value).GetChars();
        comment += s;
        sc.AddMessageToLog(comment, 1);
    }

    static void log(SCStudyInterfaceRef& sc, const char* value, SCString comment = "") {
        comment += value;
        sc.AddMessageToLog(comment, 1);
    }

    static void log(SCStudyInterfaceRef& sc, int value, SCString comment = "") {
        SCString strVal = std::to_string(value).c_str();
        comment += strVal;
        sc.AddMessageToLog(comment, 1);
    }
};

class DateTime {
   public:
    static int getFirstIndexOfSession(SCStudyInterfaceRef& sc) {
        SCDateTime tradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
        SCString dateTime = sc.FormatDateTime(tradingDayStartDateTime).GetChars();
        Logger::log(sc, dateTime, "ddateTimee");
        int startIndex = sc.GetFirstIndexForDate(sc.ChartNumber, tradingDayStartDateTime.GetDate());
        return startIndex;
    }
};

}  // namespace utils
