
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





}
