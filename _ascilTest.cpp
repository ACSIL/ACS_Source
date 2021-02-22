#include <string>

#include "sierrachart.h"
#include "utils.h"

SCDLLName("Test");

SCSFExport scsf_ColorBarsAbovePrice(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphName = "Color bars above price limit";
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
        sc.AutoLoop = 1;
        return;
    }

    int startIndex = utils::DateTime::getFirstIndexOfSession(sc);

    utils::Logger::log(sc, startIndex, "starting index");
    utils::Logger::log(sc, sc.Index, "sc.Index index");
    utils::Logger::log(sc, sc.IndexOfFirstVisibleBar, "sc.FirstVisible index");
    utils::Logger::log(sc, sc.IndexOfLastVisibleBar, "sc.LastVisible index");

    if (sc.Index > startIndex) {
        Subgraph_IB[sc.Index] = startIndex;
    }
}

SCSFExport scsf_ColorFirstBarOfTradignDay(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphName = "Color First Bar of Trading Day";
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(255, 255, 255);
        sc.AutoLoop = 1;
        return;
    }
    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
    SCString dateTime = sc.FormatDateTime(TradingDayStartDateTime).GetChars();
    auto StartIndex = sc.GetFirstIndexForDate(sc.ChartNumber, TradingDayStartDateTime.GetDate());

    sc.AddMessageToLog(std::to_string(StartIndex).c_str(), 1);

    Subgraph_IB[StartIndex] = StartIndex;
}

SCSFExport scsf_LogUtilsTest(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphName = "Log Utils Test";
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(255, 255, 255);
        sc.AutoLoop = 1;
        return;
    }

    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);

    utils::Logger::log(sc, TradingDayStartDateTime);
    //     utils::Logger::log(sc, "asdsd");
    utils::Logger::log(sc, 3.23);
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

    s_UseTool sl;
    sl.Clear();
    sl.ChartNumber = sc.ChartNumber;
    sl.DrawingType = DRAWING_TEXT;
    sl.FontBackColor = RGB(0, 0, 0);
    sl.FontSize = 8;
    sl.FontBold = false;
    sl.AddMethod = UTAM_ADD_OR_ADJUST;
    sl.UseRelativeVerticalValues = 1;
    sl.Color = RGB(255, 255, 255);
    sl.Region = 0;
    sl.Text.Format("sl: %.03f", StudyValue);
    sl.LineNumber = 20;
    sl.BeginDateTime = 1;
    sl.BeginValue = 70;
    sc.UseTool(sl);
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

    sc.AddAndManageSingleTextDrawingForStudy(sc, false, Input_HorizontalPosition.GetInt(), Input_VerticalPosition.GetInt(), Subgraph_TextDisplay, false, ValueText, true);
}