#include <string>

#include "sierrachart.h"
#include "util.h"

SCDLLName("Test");

SCSFExport scsf_ColorBarsFromStartOfSession(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
        sc.AutoLoop = 1;
        return;
    }

    int startIndex = util::DateTime::getFirstIndexOfSession(sc);

    util::Logger::log(sc, startIndex, "starting index");
    util::Logger::log(sc, sc.Index, "sc.Index index");
    util::Logger::log(sc, sc.IndexOfFirstVisibleBar, "sc.FirstVisible index");
    util::Logger::log(sc, sc.IndexOfLastVisibleBar, "sc.LastVisible index");

    if (sc.Index > startIndex) {
        Subgraph_IB[sc.Index] = startIndex;
    }
}

SCSFExport scsf_ColorFromFirstBarOfTradignDay(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
        sc.AutoLoop = 1;
        return;
    }
    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.Index]);
    SCString s = sc.FormatDateTime(TradingDayStartDateTime).GetChars();
    auto StartIndex = sc.GetFirstIndexForDate(sc.ChartNumber, TradingDayStartDateTime.GetDate());

    sc.AddMessageToLog(s, 1);

    if (sc.Index > StartIndex) {
        Subgraph_IB[sc.Index] = StartIndex;
    }
}

// SCSFExport scsf_ColorThreeHHBars(SCStudyInterfaceRef sc) {
//     SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
//     if (sc.SetDefaults) {
//         sc.GraphName = "Color Three HH bars";
//         sc.GraphRegion = 0;
//         Subgraph_IB.Name = "IB";
//         Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
//         Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
//         sc.AutoLoop = 1;
//         return;
//     }

//     if (StartIndex == sc.Index) sc.Index = 0;

//     if (sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2]) {
//         Subgraph_IB[sc.Index] = sc.Index;
//         Subgraph_IB[sc.Index - 1] = sc.Index;
//         Subgraph_IB[sc.Index - 2] = sc.Index;
//     }
// }

SCSFExport scsf_TakeTradeOnThirdRisingBar(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        sc.Input[0].Name = "Start trading at: ";
        sc.Input[0].SetTime(HMS_TIME(8, 30, 0));
        sc.Input[1].Name = "Stop trading at: ";
        sc.Input[1].SetTime(HMS_TIME(14, 00, 0));
        sc.Input[2].Name = "Flat postion at: ";
        sc.Input[2].SetTime(HMS_TIME(15, 00, 30));
        return;
    }

    s_SCPositionData pos;
    sc.GetTradePosition(pos);
    s_SCNewOrder order;
    order.OrderQuantity = 1;
    order.OrderType = SCT_ORDERTYPE_MARKET;
    order.Target1Offset = 10 * sc.TickSize;
    order.Stop1Offset = 10 * sc.TickSize;

    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
    SCString s = sc.FormatDateTime(TradingDayStartDateTime).GetChars();
    auto StartIndex = sc.GetFirstIndexForDate(sc.ChartNumber, TradingDayStartDateTime.GetDate());

    bool areTradingHours = sc.BaseDateTimeIn[sc.Index].GetTime() > sc.Input[0].GetTime() && sc.BaseDateTimeIn[sc.Index].GetTime() < sc.Input[1].GetTime();
    bool isTimeToFlat = sc.BaseDateTimeIn[sc.Index].GetTime() >= sc.Input[2].GetTime();

    bool threeHigherHighs = sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2];

    // ma resetovat ale nefunguje - nema brat pv potazv bary pred open..
    if (areTradingHours) {
        if (sc.Index >= StartIndex) {
            if (threeHigherHighs) {
                sc.BuyEntry(order);
            }
        }
    }
    if (isTimeToFlat) sc.FlattenPosition();
}

SCSFExport scsf_LogUtilsTest(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(255, 255, 255);
        sc.AutoLoop = 1;
        return;
    }

    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);

    util::Logger::log(sc, TradingDayStartDateTime);
    util::Logger::log(sc, 3.23);
}

SCSFExport scsf_GetValuesFromAnotherCharts(SCStudyInterfaceRef sc) {
    SCInputRef Input_StudySubgraphReference = sc.Input[0];

    if (sc.SetDefaults) {
        Input_StudySubgraphReference.Name = "Study And Subgraph To Display";
        Input_StudySubgraphReference.SetChartStudySubgraphValues(3, 4, 0);
        return;
    }

    SCFloatArray StudyReference;
    sc.GetStudyArrayFromChartUsingID(Input_StudySubgraphReference.GetChartNumber(), Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex(), StudyReference);
    float StudyValue = StudyReference[StudyReference.GetArraySize() - 1];

    s_UseTool t;
    t.Clear();
    t.ChartNumber = sc.ChartNumber;
    t.DrawingType = DRAWING_TEXT;
    t.FontBackColor = RGB(0, 0, 0);
    t.FontSize = 8;
    t.FontBold = false;
    t.AddMethod = UTAM_ADD_OR_ADJUST;
    t.UseRelativeVerticalValues = 1;
    t.Color = RGB(255, 255, 255);
    t.Region = 0;
    t.Text.Format("t: %.03f", StudyValue);
    t.LineNumber = 20;
    t.BeginDateTime = 1;
    t.BeginValue = 70;
    sc.UseTool(t);
}

SCSFExport scsf_CheckIfTimeInSessions(SCStudyInterfaceRef sc) {
    int IsInDaySession = sc.IsDateTimeInDaySession(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
    int IsInEveningSession = sc.IsDateTimeInEveningSession(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
    int IsInSession = sc.IsDateTimeInSession(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);

    util::Logger::log(sc, util::DateTime::isCurrentBarInDaySession(sc), " IsInDaySession");
    util::Logger::log(sc, util::DateTime::isCurrentBarInNightSession(sc), " IsInEveningSession");
    util::Logger::log(sc, util::DateTime::isCurrentBarInSession(sc), " IsInSession");
}

SCSFExport scsf_DrawTextToChart(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
    }

    util::Display::displayText(sc, "testing..", 5, 85);
}

SCSFExport scsf_LargeTextDisplayForStudyFromChart(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_TextDisplay = sc.Subgraph[0];

    SCInputRef Input_HorizontalPosition = sc.Input[0];
    SCInputRef Input_VerticalPosition = sc.Input[1];
    SCInputRef Input_StudySubgraphReference = sc.Input[3];
    SCInputRef Input_DisplaySubgraphName = sc.Input[4];
    SCInputRef Input_SubgraphOffset = sc.Input[5];
    SCInputRef Input_UseTextForNonZeroValue = sc.Input[6];
    SCInputRef Input_NonZeroValueText = sc.Input[7];

    if (sc.SetDefaults) {
        // Set the configuration and defaults

        sc.GraphName = "Text Display For Study from Chart";

        sc.AutoLoop = 0;
        sc.GraphRegion = 0;

        Subgraph_TextDisplay.Name = "Text Display";
        Subgraph_TextDisplay.LineWidth = 20;
        Subgraph_TextDisplay.DrawStyle = DRAWSTYLE_CUSTOM_TEXT;
        Subgraph_TextDisplay.PrimaryColor = RGB(0, 0, 0);  //black
        Subgraph_TextDisplay.SecondaryColor = RGB(128, 255, 255);
        Subgraph_TextDisplay.SecondaryColorUsed = true;
        Subgraph_TextDisplay.DisplayNameValueInWindowsFlags = 0;

        Input_HorizontalPosition.Name.Format("Horizontal Position From Left (1-%d)", (int)CHART_DRAWING_MAX_HORIZONTAL_AXIS_RELATIVE_POSITION);
        Input_HorizontalPosition.SetInt(20);
        Input_HorizontalPosition.SetIntLimits(1, (int)CHART_DRAWING_MAX_HORIZONTAL_AXIS_RELATIVE_POSITION);

        Input_VerticalPosition.Name.Format("Vertical Position From Bottom (1-%d)", (int)CHART_DRAWING_MAX_VERTICAL_AXIS_RELATIVE_POSITION);
        Input_VerticalPosition.SetInt(90);
        Input_VerticalPosition.SetIntLimits(1, (int)CHART_DRAWING_MAX_VERTICAL_AXIS_RELATIVE_POSITION);

        Input_StudySubgraphReference.Name = "Study And Subgraph To Display";
        Input_StudySubgraphReference.SetChartStudySubgraphValues(1, 1, 0);

        Input_DisplaySubgraphName.Name = "Display Subgraph Name";
        Input_DisplaySubgraphName.SetYesNo(false);

        Input_SubgraphOffset.Name = "Subgraph Columns Back";
        Input_SubgraphOffset.SetInt(0);

        Input_UseTextForNonZeroValue.Name = "Use Text for Non-Zero Value";
        Input_UseTextForNonZeroValue.SetYesNo(false);

        Input_NonZeroValueText.Name = "Non-Zero Value Text";
        Input_NonZeroValueText.SetString("");

        sc.TextInputName = "Prefix Text for Display Value";

        return;
    }

    SCFloatArray StudyReference;
    sc.GetStudyArrayFromChartUsingID(Input_StudySubgraphReference.GetChartNumber(), Input_StudySubgraphReference.GetStudyID(),
                                     Input_StudySubgraphReference.GetSubgraphIndex(), StudyReference);

    SCString ValueText;
    if (!sc.TextInput.IsEmpty()) {
        ValueText += sc.TextInput;
        ValueText += " ";
    }

    if (Input_DisplaySubgraphName.GetYesNo()) {
        SCString SubgraphNameFromChartStudy;
        sc.GetStudySubgraphNameFromChart(Input_StudySubgraphReference.GetChartNumber(), Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex(), SubgraphNameFromChartStudy);
        ValueText += SubgraphNameFromChartStudy;
        ValueText += " ";
    }

    int ColumnsBack = Input_SubgraphOffset.GetInt();
    float StudyValue = StudyReference[StudyReference.GetArraySize() - 1 - ColumnsBack];

    //Patch for when referencing data from the Numbers Bars Calculated Values study. This value could be used.
    if (StudyValue == -FLT_MAX)
        StudyValue = 0;

    if (Input_UseTextForNonZeroValue.GetYesNo()) {
        if (StudyValue != 0)
            ValueText += Input_NonZeroValueText.GetString();
        else
            ValueText += " ";
    } else
        ValueText += sc.FormatGraphValue(StudyValue, sc.GetValueFormat());

    // void AddAndManageSingleTextDrawingForStudy(SCStudyInterfaceRef& sc, bool DisplayInFillSpace, int HorizontalPosition, int VerticalPosition, SCSubgraphRef Subgraph, int TransparentLabelBackground, SCString& TextToDisplay, int DrawAboveMainPriceGraph, int BoldFont = 1) {

    sc.AddAndManageSingleTextDrawingForStudy(sc, false, Input_HorizontalPosition.GetInt(), Input_VerticalPosition.GetInt(), Subgraph_TextDisplay, false, ValueText, true);
}
