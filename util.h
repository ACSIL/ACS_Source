
#include "sierrachart.h"

namespace util {

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

class Display {
   public:
    static void displayText(SCStudyInterfaceRef& sc, const SCString& textToDisplay, int horizontalPosition, int verticalPosition, SCString description = "") {
        int& r_DrawingTextLineNumber = *static_cast<int*>(sc.StorageBlock);

        if (sc.LastCallToFunction) {
            // If the Chartbook is being closed, the Chartbook will have already been saved first by the time we enter this block.  So this particular chart drawing if it exists, will have already been saved.
            if (r_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, r_DrawingTextLineNumber) > 0) {
                // be sure to delete user drawn type drawing when study removed
                sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, r_DrawingTextLineNumber);
            }

            return;
        }

        //Reset the line number if the drawing no longer exists.
        if (r_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, r_DrawingTextLineNumber) == 0) {
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && r_DrawingTextLineNumber != 0) {
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, r_DrawingTextLineNumber);
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;

        SCString text;
        text += description;
        text += " ";
        text += textToDisplay;

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
        t.BeginDateTime = horizontalPosition;
        t.BeginValue = verticalPosition;
        t.AddAsUserDrawnDrawing = true;

        if (r_DrawingTextLineNumber != 0)
            t.LineNumber = r_DrawingTextLineNumber;

        if (sc.UseTool(t) > 0)
            r_DrawingTextLineNumber = t.LineNumber;
    }

    static void displayNumber(SCStudyInterfaceRef& sc, const int& value, int horizontalPosition, int verticalPosition, SCString description = "") {
        int& p_DrawingTextLineNumber = sc.GetPersistentInt(11110000);

        if (sc.LastCallToFunction) {
            // If the Chartbook is being closed, the Chartbook
            //will have already been saved first by the time we enter this block.
            //So this particular chart drawing if it exists, will have already been saved.
            if (p_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, p_DrawingTextLineNumber) > 0) {
                // be sure to delete user drawn type drawing when study removed
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
        t.LineNumber = horizontalPosition + verticalPosition;  // to make it unique for each instance in a chart
        t.BeginDateTime = horizontalPosition;
        t.BeginValue = verticalPosition;
        sc.UseTool(t);

        if (p_DrawingTextLineNumber != 0)
            t.LineNumber = p_DrawingTextLineNumber;

        if (sc.UseTool(t) > 0)
            p_DrawingTextLineNumber = t.LineNumber;
    }

    static void display(SCStudyInterfaceRef& sc, int horizontalPosition, int verticalPosition, const int& value, int DrawAboveMainPriceGraph, SCString description = "") {
        int& r_DrawingTextLineNumber = *static_cast<int*>(sc.StorageBlock);

        if (sc.LastCallToFunction) {
            // If the Chartbook is being closed, the Chartbook
            //will have already been saved first by the time we enter this block.
            //So this particular chart drawing if it exists, will have already been saved.
            if (r_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, r_DrawingTextLineNumber) > 0) {
                // be sure to delete user drawn type drawing when study removed
                sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, r_DrawingTextLineNumber);
            }
            return;
        }

        if (r_DrawingTextLineNumber != 0 && sc.UserDrawnChartDrawingExists(sc.ChartNumber, r_DrawingTextLineNumber) == 0) {
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && r_DrawingTextLineNumber != 0) {
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, r_DrawingTextLineNumber);
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;

        SCString text;
        SCString strValue = std::to_string(value).c_str();
        text += description;
        text += strValue;
        text += "";

        s_UseTool Tool;
        Tool.ChartNumber = sc.ChartNumber;
        Tool.DrawingType = DRAWING_TEXT;
        Tool.Region = sc.GraphRegion;
        Tool.AddMethod = UTAM_ADD_OR_ADJUST;

        Tool.BeginDateTime = horizontalPosition;
        Tool.BeginDateTime = -3;

        Tool.BeginValue = (float)verticalPosition;

        Tool.UseRelativeVerticalValues = 1;
        Tool.Color = RGB(0, 0, 0);  //black
        Tool.FontBackColor = RGB(128, 255, 255);
        Tool.TransparentLabelBackground = false;

        Tool.ReverseTextColor = 0;
        Tool.FontBold = 0;
        Tool.FontSize = 14;
        Tool.FontFace = sc.GetChartTextFontFaceName();

        Tool.Text = text;

        Tool.DrawUnderneathMainGraph = DrawAboveMainPriceGraph ? 0 : 1;

        if (r_DrawingTextLineNumber != 0)
            Tool.LineNumber = r_DrawingTextLineNumber;

        if (sc.UseTool(Tool) > 0)
            r_DrawingTextLineNumber = Tool.LineNumber;
    }

    static void dspl(SCStudyInterfaceRef& sc, bool DisplayInFillSpace, int HorizontalPosition, int VerticalPosition, int TransparentLabelBackground, const SCString& TextToDisplay, int DrawAboveMainPriceGraph, int BoldFont = 1) {
        SCSubgraphRef Subgraph = sc.Subgraph[0];

        Subgraph.Name = "Text Display";
        Subgraph.LineWidth = 20;
        Subgraph.DrawStyle = DRAWSTYLE_CUSTOM_TEXT;
        Subgraph.PrimaryColor = RGB(0, 0, 0);  //black
        Subgraph.SecondaryColor = RGB(128, 255, 255);
        Subgraph.SecondaryColorUsed = true;
        Subgraph.DisplayNameValueInWindowsFlags = 0;

        int& r_DrawingTextLineNumber = sc.GetPersistentInt(11110000);

        if (sc.IsFullRecalculation)
            r_DrawingTextLineNumber = 0;

        //Reset the line number if the drawing no longer exists.
        if (r_DrawingTextLineNumber != 0 && sc.ChartDrawingExists(sc.ChartNumber, r_DrawingTextLineNumber) == 0) {
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy && r_DrawingTextLineNumber != 0) {
            sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, r_DrawingTextLineNumber);
            r_DrawingTextLineNumber = 0;
        }

        if (sc.HideStudy)
            return;

        s_UseTool Tool;
        Tool.ChartNumber = sc.ChartNumber;
        Tool.DrawingType = DRAWING_TEXT;
        Tool.Region = sc.GraphRegion;
        Tool.AddMethod = UTAM_ADD_OR_ADJUST;

        if (!DisplayInFillSpace)
            Tool.BeginDateTime = HorizontalPosition;
        else
            Tool.BeginDateTime = -3;

        Tool.BeginValue = (float)VerticalPosition;

        Tool.UseRelativeVerticalValues = 1;
        Tool.Color = Subgraph.PrimaryColor;
        Tool.FontBackColor = Subgraph.SecondaryColor;
        Tool.TransparentLabelBackground = TransparentLabelBackground;

        Tool.ReverseTextColor = 0;
        Tool.FontBold = BoldFont;
        Tool.FontSize = Subgraph.LineWidth;
        Tool.FontFace = sc.GetChartTextFontFaceName();

        Tool.Text = TextToDisplay;

        Tool.DrawUnderneathMainGraph = DrawAboveMainPriceGraph ? 0 : 1;

        if (r_DrawingTextLineNumber != 0)
            Tool.LineNumber = r_DrawingTextLineNumber;

        if (sc.UseTool(Tool) > 0)
            r_DrawingTextLineNumber = Tool.LineNumber;
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
