
#include "sierrachart.h"

namespace util {

class Logger {
   public:
    static void log(SCStudyInterfaceRef& sc, SCDateTime value, int line, SCString comment = "")  {
        SCString s = sc.FormatDateTime(value).GetChars();
        comment += s;
        sc.AddMessageToLog(comment, line);
    }

    static void log(SCStudyInterfaceRef& sc, const char* value, int line, SCString comment = "") {
        comment += value;
        sc.AddMessageToLog(comment, line);
    }

    static void log(SCStudyInterfaceRef& sc, int value, int line, SCString comment = "") {
        SCString strVal = std::to_string(value).c_str();
        comment += strVal;
        sc.AddMessageToLog(comment, line);
    }

    static void log(SCStudyInterfaceRef& sc, double value, int line, SCString comment = "") {
        SCString strVal = std::to_string(value).c_str();
        comment += strVal;
        sc.AddMessageToLog(comment, line);
    }
};

class Display {
   public:
    static void display(SCStudyInterfaceRef& sc, const SCString& value, int xCoordinate, int yCoordinate, SCString description = "") {
        int uniqueID = xCoordinate + yCoordinate;
        int& p_DrawingTextLineNumber = sc.GetPersistentInt(11110000 + uniqueID);

        if (sc.LastCallToFunction) {
            if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) > 0) {
                sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            }
            return;
        }

        if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) == 0) {
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && p_DrawingTextLineNumber != 0) {
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;

        SCString text;
        text += description;
        text += " ";
        text += value;

        s_UseTool t;
        t.ChartNumber = sc.ChartNumber;
        t.DrawingType = DRAWING_TEXT;
        t.FontBackColor = RGB(0, 0, 0);
        t.FontSize = 10;
        t.FontBold = false;
        t.AddMethod = UTAM_ADD_OR_ADJUST;
        t.UseRelativeVerticalValues = 1;
        t.Color = RGB(255, 255, 255);
        t.Region = 0;
        t.Text = text;
        t.LineNumber = uniqueID;
        t.BeginDateTime = xCoordinate;
        t.BeginValue = yCoordinate;
        t.AddAsUserDrawnDrawing = true;

        if (p_DrawingTextLineNumber != 0)
            t.LineNumber = p_DrawingTextLineNumber;

        if (sc.UseTool(t) > 0)
            p_DrawingTextLineNumber = t.LineNumber;
    }

    static void display(SCStudyInterfaceRef& sc, const int& value, int xCoordinate, int yCoordinate, SCString description = "") {
        int uniqueID = xCoordinate + yCoordinate;
        int& p_DrawingTextLineNumber = sc.GetPersistentInt(11110000 + uniqueID);

        if (sc.LastCallToFunction) {
            if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) > 0) {
                sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            }
            return;
        }

        if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) == 0) {
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && p_DrawingTextLineNumber != 0) {
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;

        SCString text;
        SCString strValue = std::to_string(value).c_str();
        text += description;
        text += strValue;
        text += "";

        s_UseTool t;
        t.ChartNumber = sc.ChartNumber;
        t.DrawingType = DRAWING_TEXT;
        t.FontBackColor = RGB(0, 0, 0);
        t.FontSize = 10;
        t.FontBold = false;
        t.AddMethod = UTAM_ADD_OR_ADJUST;
        t.UseRelativeVerticalValues = 1;
        t.Color = RGB(255, 255, 255);
        t.Region = 0;
        t.Text = text;
        t.LineNumber = uniqueID;
        t.BeginDateTime = xCoordinate;
        t.BeginValue = yCoordinate;
        t.AddAsUserDrawnDrawing = true;
        sc.UseTool(t);

        if (p_DrawingTextLineNumber != 0)
            t.LineNumber = p_DrawingTextLineNumber;

        if (sc.UseTool(t) > 0)
            p_DrawingTextLineNumber = t.LineNumber;
    }

    static void display(SCStudyInterfaceRef& sc, const double& value, int xCoordinate, int yCoordinate, SCString description = "") {
        int uniqueID = xCoordinate + yCoordinate;
        int& p_DrawingTextLineNumber = sc.GetPersistentInt(11110000 + uniqueID);

        if (sc.LastCallToFunction) {
            if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) > 0) {
                sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            }
            return;
        }

        if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) == 0) {
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && p_DrawingTextLineNumber != 0) {
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, p_DrawingTextLineNumber);
            p_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;


        SCString text;
        SCString strValue = std::to_string(value).c_str();
        text += description;
        text += " ";
        text += strValue;

        s_UseTool t;
        t.ChartNumber = sc.ChartNumber;
        t.DrawingType = DRAWING_TEXT;
        t.FontBackColor = RGB(0, 0, 0);
        t.FontSize = 10;
        t.FontBold = false;
        t.AddMethod = UTAM_ADD_OR_ADJUST;
        t.UseRelativeVerticalValues = 1;
        t.Color = RGB(255, 255, 255);
        t.Region = 0;
        t.Text = text;
        t.LineNumber = uniqueID;
        t.BeginDateTime = xCoordinate;
        t.BeginValue = yCoordinate;
        t.AddAsUserDrawnDrawing = true;
        sc.UseTool(t);

        if (p_DrawingTextLineNumber != 0)
            t.LineNumber = p_DrawingTextLineNumber;

        if (sc.UseTool(t) > 0)
            p_DrawingTextLineNumber = t.LineNumber;
    }

};

class DateTime {
   public:
    // nevim jestli funguje ok..
    static int getFirstIndexOfSession(SCStudyInterfaceRef& sc) {
        SCDateTime tradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
        SCString dateTime = sc.FormatDateTime(tradingDayStartDateTime).GetChars();
        int startIndex = sc.GetFirstIndexForDate(sc.ChartNumber, tradingDayStartDateTime.GetDate());
        return startIndex;
    }

    static int isCurrentBarInDaySession(SCStudyInterfaceRef& sc, int offset = 0) {
        return sc.IsDateTimeInDaySession(sc.BaseDateTimeIn[sc.Index - offset]);
    }

    static int isCurrentBarInNightSession(SCStudyInterfaceRef& sc, int offset = 0) {
        return sc.IsDateTimeInEveningSession(sc.BaseDateTimeIn[sc.Index - offset]);
    }

    static int isCurrentBarInSession(SCStudyInterfaceRef& sc, int offset = 0) {
        return sc.IsDateTimeInSession(sc.BaseDateTimeIn[sc.Index - offset]);
    }
};

}  // namespace util
