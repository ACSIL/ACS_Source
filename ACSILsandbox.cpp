#include <string>

#include "sierrachart.h"
#include "util.h"

SCDLLName("ACIL Sandbox");
/*============================================================================*/
SCSFExport scsf_LargeTextDisplayForStudy(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_TextDisplay = sc.Subgraph[0];

    SCInputRef Input_HorizontalPosition = sc.Input[0];
    SCInputRef Input_VerticalPosition = sc.Input[1];
    SCInputRef Input_StudySubgraphReference = sc.Input[3];
    SCInputRef Input_DisplaySubgraphName = sc.Input[4];
    SCInputRef Input_SubgraphOffset = sc.Input[5];
    SCInputRef Input_PrefixTextForDisplayValue = sc.Input[6];
    SCInputRef Input_SuffixTextForDisplayValue = sc.Input[7];
    SCInputRef Input_UseTextForNonZeroValue = sc.Input[8];
    SCInputRef Input_NonZeroValueText = sc.Input[9];

    if (sc.SetDefaults) {
        // Set the configuration and defaults

        sc.GraphName = "Text Display For Study";

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

        Input_StudySubgraphReference.Name = "Study and Subgraph to Display";
        Input_StudySubgraphReference.SetStudySubgraphValues(1, 0);

        Input_DisplaySubgraphName.Name = "Display Subgraph Name";
        Input_DisplaySubgraphName.SetYesNo(false);

        Input_SubgraphOffset.Name = "Subgraph Columns Back";
        Input_SubgraphOffset.SetInt(0);

        Input_PrefixTextForDisplayValue.Name = "Prefix Text for Display Value";
        Input_PrefixTextForDisplayValue.SetString("");

        Input_SuffixTextForDisplayValue.Name = "Suffix Text for Display Value";
        Input_SuffixTextForDisplayValue.SetString("");

        Input_UseTextForNonZeroValue.Name = "Use Text for Non-Zero Value";
        Input_UseTextForNonZeroValue.SetYesNo(false);

        Input_NonZeroValueText.Name = "Non-Zero Value Text";
        Input_NonZeroValueText.SetString("");

        return;
    }

    // Do data processing

    SCFloatArray StudyReference;
    sc.GetStudyArrayUsingID(Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex(), StudyReference);

    int ColumnsBack = Input_SubgraphOffset.GetInt();
    SCString ValueText;
    if (Input_PrefixTextForDisplayValue.GetString()[0] != '\0') {
        ValueText += Input_PrefixTextForDisplayValue.GetString();
        ValueText += " ";
    }

    // if (Input_DisplaySubgraphName.GetYesNo()) {
    //     const char* SubgraphName = sc.GetStudySubgraphName(Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex());

    //     if (SubgraphName != NULL) {
    //         ValueText += SubgraphName;
    //         ValueText += ": ";
    //     }
    // }

    float StudyValue = StudyReference[StudyReference.GetArraySize() - 1 - ColumnsBack];

    if (StudyValue == -FLT_MAX)  //Patch for when referencing data from the Numbers Bars Calculated Values study. This value could be used.
        StudyValue = 0;

    if (Input_UseTextForNonZeroValue.GetYesNo()) {
        if (StudyValue != 0)
            ValueText += Input_NonZeroValueText.GetString();
        else
            ValueText += " ";
    } else
        ValueText += sc.FormatGraphValue(StudyValue, sc.GetValueFormat());

    ValueText += Input_SuffixTextForDisplayValue.GetString();

    sc.AddAndManageSingleTextDrawingForStudy(sc, false, Input_HorizontalPosition.GetInt(), Input_VerticalPosition.GetInt(), Subgraph_TextDisplay, false, ValueText, true);
}

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

    // util::Logger::log(sc, startIndex, "starting index");
    // util::Logger::log(sc, sc.Index, "sc.Index index");
    // util::Logger::log(sc, sc.IndexOfFirstVisibleBar, "sc.FirstVisible index");
    // util::Logger::log(sc, sc.IndexOfLastVisibleBar, "sc.LastVisible index");

    if (sc.Index > startIndex) {
        Subgraph_IB[sc.Index] = startIndex;
    }
}

// SCSFExport scsf_ColorBarsThatHaveCloseAboveEMA(SCStudyInterfaceRef sc) {
//     SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
//     if (sc.SetDefaults) {
//         sc.GraphRegion = 0;
//         Subgraph_IB.Name = "IB";
//         Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
//         Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
//         sc.AutoLoop = 1;
//         return;
//     }

//     int startIndex = util::DateTime::getFirstIndexOfSession(sc);

//     util::Logger::log(sc, startIndex, "starting index");
//     util::Logger::log(sc, sc.Index, "sc.Index index");
//     util::Logger::log(sc, sc.IndexOfFirstVisibleBar, "sc.FirstVisible index");
//     util::Logger::log(sc, sc.IndexOfLastVisibleBar, "sc.LastVisible index");

//     if (sc.Index > startIndex) {
//         Subgraph_IB[sc.Index] = startIndex;
//     }
// }

SCSFExport scsf_ColorThreeHHBars(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_IB = sc.Subgraph[0];
    if (sc.SetDefaults) {
        sc.GraphName = "Color Three HH bars";
        sc.GraphRegion = 0;
        Subgraph_IB.Name = "IB";
        Subgraph_IB.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_IB.PrimaryColor = RGB(0, 0, 255);
        sc.AutoLoop = 1;
        return;
    }

    // if (StartIndex == sc.Index) sc.Index = 0;

    if (sc.High[sc.Index] > sc.High[sc.Index - 1] && sc.High[sc.Index - 1] > sc.High[sc.Index - 2]) {
        Subgraph_IB[sc.Index] = sc.Index;
        Subgraph_IB[sc.Index - 1] = sc.Index;
        Subgraph_IB[sc.Index - 2] = sc.Index;
    }
}

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
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
    }
    SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);

    // util::Logger::log(sc, TradingDayStartDateTime, "datetime");
    // util::Logger::log(sc, 22, "int");
    // util::Logger::log(sc, 3.23, "double");
    // util::Logger::log(sc, "hello work", "string");
}

SCSFExport scsf_GetLastValueFromStudyOnSameChart(SCStudyInterfaceRef sc) {
    SCInputRef Input_StudySubgraphReference = sc.Input[0];

    if (sc.SetDefaults) {
        sc.UpdateAlways = 1;
        sc.GraphRegion = 0;

        Input_StudySubgraphReference.Name = "Study and Subgraph to Display";
        Input_StudySubgraphReference.SetStudySubgraphValues(7, 1);
        return;
    }

    SCFloatArray StudyReference;
    sc.GetStudyArrayUsingID(Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex(), StudyReference);
    float StudyValue = StudyReference[sc.IndexOfLastVisibleBar];

    s_UseTool t;
    t.Clear();
    t.ChartNumber = sc.ChartNumber;
    t.DrawingType = DRAWING_TEXT;
    t.FontBackColor = RGB(0, 0, 0);
    t.FontSize = 20;
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

SCSFExport scsf_GetNLastValuesFromStudyOnTheSameChartUsingRawAwway(SCStudyInterfaceRef sc) {
    SCInputRef Input_StudySubgraphReference = sc.Input[0];

    if (sc.SetDefaults) {
        sc.UpdateAlways = 1;
        sc.GraphRegion = 0;

        Input_StudySubgraphReference.Name = "Study and Subgraph to Display";
        Input_StudySubgraphReference.SetStudySubgraphValues(7, 1);
        return;
    }

    // get some study reference
    SCFloatArray StudyReference;
    sc.GetStudyArrayUsingID(Input_StudySubgraphReference.GetStudyID(), Input_StudySubgraphReference.GetSubgraphIndex(), StudyReference);
    double StudyValueLast = StudyReference[sc.IndexOfLastVisibleBar];

    // create an array (or a vector or etc.. ) on the heap
    int const ARR_SIZE = 5;
    double* p_myArray = (double*)sc.GetPersistentPointer(1);
    p_myArray = new double[ARR_SIZE];
    
    // make sure you delete the array after study is removed
    if (p_myArray != nullptr && sc.LastCallToFunction) {
        delete[] p_myArray;
        sc.SetPersistentPointer(1, NULL);
        return;
    }

    // get get value of close for last 5 bars
    for (int i = 0; i < ARR_SIZE; ++i) {
        p_myArray[i] = sc.Close[sc.IndexOfLastVisibleBar - i];
        p_myArray[i] = StudyReference[sc.IndexOfLastVisibleBar - i]; // int the same way you could get value of some indicator for the last five indexes

        util::Logger::log(sc, p_myArray[i], 0, "value  ");
    }

    // now i have values in the array so i can do some calculations.. (it is actually better to use stl:: collection)
    double sum = 0;
    for (double const &d: p_myArray) sum +=d;
    

    util::Logger::log(sc, "outside while.. ", 0);
}

SCSFExport scsf_readLastNValuesToArray(SCStudyInterfaceRef sc) {
    SCInputRef Input_StudySubgraphReference = sc.Input[0];

    if (sc.SetDefaults) {
        sc.UpdateAlways = 1;
        sc.GraphRegion = 0;

        Input_StudySubgraphReference.Name = "Study and Subgraph to Display";
        Input_StudySubgraphReference.SetStudySubgraphValues(7, 1);
        return;
    }

    int* arr = (int*)sc.GetPersistentPointer(1);
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

    // util::Logger::log(sc, util::DateTime::isCurrentBarInDaySession(sc), " IsInDaySession");
    // util::Logger::log(sc, util::DateTime::isCurrentBarInNightSession(sc), " IsInEveningSession");
    // util::Logger::log(sc, util::DateTime::isCurrentBarInSession(sc), " IsInSession");
}

SCSFExport scsf_DrawTextToChart(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
    }

    util::Display::display(sc, "testing..", 10, 90, "string");
    util::Display::display(sc, 1111, 10, 88, "int");
    util::Display::display(sc, 3.2124, 10, 86, "double");
}

SCSFExport scsf_GetFirstAndLastVisibleBar(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.UpdateAlways = 1;  // true
        sc.GraphRegion = 0;
    }

    double value = sc.High[sc.IndexOfLastVisibleBar];

    s_UseTool t;
    t.Clear();
    t.ChartNumber = sc.ChartNumber;
    t.DrawingType = DRAWING_TEXT;
    t.FontBackColor = RGB(0, 0, 0);
    t.FontSize = 15;
    t.FontBold = false;
    t.AddMethod = UTAM_ADD_OR_ADJUST;
    t.UseRelativeVerticalValues = 1;
    t.Color = RGB(255, 255, 255);
    t.Region = 0;
    t.Text.Format("t: %.03i", sc.IndexOfLastVisibleBar);  // nebo sem muzu dat to value high
    t.LineNumber = 20;
    t.BeginDateTime = 1;
    t.BeginValue = 70;
    sc.UseTool(t);
}

SCSFExport scsf_DrawStudy(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.Subgraph[0].Name = "Ema";
        sc.Subgraph[0].PrimaryColor = RGB(102, 255, 102);
        sc.Subgraph[0].DrawStyle = DRAWSTYLE_LINE;

        sc.Input[0].Name = "Moving Average";
        sc.Input[0].SetFloat(10.0f);
        sc.AutoLoop = 1;
        sc.FreeDLL = 1;
        sc.GraphRegion = 0;
        return;
    }
    // int InMALength = sc.Input[0].GetInt();
    // sc.DataStartIndex = InMALength - 1; // timhle to zacne az od 9. indexu

    sc.SimpleMovAvg(sc.BaseDataIn[SC_LAST], sc.Subgraph[0].Data, 10);
}

SCSFExport scsf_MovingAverageExample(SCStudyGraphRef sc) {
    if (sc.SetDefaults) {
        sc.Subgraph[0].Name = "Simple Moving Average";
        sc.Subgraph[0].PrimaryColor = RGB(102, 255, 102);  // Optionally, you may predefine your graph's colors
        sc.Subgraph[0].LineStyle = LINESTYLE_DOT;          // Optionally, you may predefine a line style

        sc.Input[0].Name = "Moving Average Length";  // Your Input variables
        sc.Input[0].SetFloat(10.0f);                 // Here we set its default value to 10

        sc.GraphName = "Moving Average Example 1";

        sc.StudyDescription = "Insert study description here.";

        sc.AutoLoop = 1;  // true
        sc.FreeDLL = 1;

        sc.GraphRegion = 0;
        return;
    }

    int InMALength = sc.Input[0].GetInt();  // Input 0 - "Moving Average Length"
                                            //  2 - "Plot Exponential Moving Average"

    // Set the index of the first array element to begin drawing at
    sc.DataStartIndex = InMALength - 1;

    // We calculate the simple moving average of the last trade prices of the chart bars ('SC_LAST')
    // in the chart.  Subgraph 0 is used to store and display the simple moving average.
    sc.SimpleMovAvg(
        sc.BaseDataIn[SC_LAST],  // This is the input array or source data. This is using the Last / Close from each bar.
        sc.Subgraph[0].Data,     // This designates your output array (subgraph/plot). This is using Subgraph 0.
        InMALength);             // This is the length for the simple moving average, which is from Input 0.
}

SCSFExport scsf_ColorBarsThatHaveCloseAboveEMA(SCStudyInterfaceRef sc) {
    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        sc.AutoLoop = 1;

        sc.Subgraph[0].Name = "";
        sc.Subgraph[0].DrawStyle = DRAWSTYLE_COLOR_BAR;
        sc.Subgraph[0].PrimaryColor = RGB(0, 0, 255);

        sc.Subgraph[1].Name = "Simple Moving Average";
        sc.Subgraph[1].PrimaryColor = RGB(102, 255, 102);
        sc.Subgraph[1].LineStyle = LINESTYLE_DOT;

        sc.Input[1].Name = "Moving Average Length";
        sc.Input[1].SetFloat(10.0f);

        return;
    }

    int InMALength = sc.Input[0].GetInt();

    sc.DataStartIndex = InMALength - 1;

    sc.SimpleMovAvg(
        sc.BaseDataIn[SC_LAST],  // This is the input array or source data. This is using the Last / Close from each bar.
        sc.Subgraph[1].Data,     // This designates your output array (subgraph/plot). This is using Subgraph 1.
        InMALength);

    float EmaAtCurBar = sc.Subgraph[1][sc.Index];

    if (EmaAtCurBar > sc.Close[sc.Index])
        sc.Subgraph[0][sc.Index] = sc.Index;
}

SCSFExport scsf_ColorBarWithCloseAboveEma(SCStudyInterfaceRef sc) {
    SCSubgraphRef Subgraph_Average = sc.Subgraph[0];
    SCSubgraphRef Subgraph_ColoredBar = sc.Subgraph[1];
    SCInputRef Input_Length = sc.Input[0];

    if (sc.SetDefaults) {
        sc.GraphRegion = 0;
        sc.ValueFormat = VALUEFORMAT_INHERITED;
        Subgraph_Average.Name = "Average";

        Subgraph_ColoredBar.Name = "dfdfsdfsd";
        Subgraph_ColoredBar.DrawStyle = DRAWSTYLE_COLOR_BAR;
        Subgraph_ColoredBar.PrimaryColor = RGB(0, 0, 255);

        // Set the color, style and line width for the subgraph
        Subgraph_Average.PrimaryColor = RGB(0, 255, 0);
        Subgraph_Average.DrawStyle = DRAWSTYLE_LINE;
        Subgraph_Average.LineWidth = 2;

        // Set the Length input and default it to 30
        Input_Length.Name = "Length";
        Input_Length.SetInt(30);
        Input_Length.SetIntLimits(1, MAX_STUDY_LENGTH);
        Input_Length.SetDescription("The number of bars to average.");

        sc.AutoLoop = 1;

        sc.AlertOnlyOncePerBar = true;

        // Must return before doing any data processing if sc.SetDefaults is set
        return;
    }
    sc.DataStartIndex = Input_Length.GetInt() - 1;
    sc.Add
    sc.SimpleMovAvg(sc.Close, Subgraph_Average, Input_Length.GetInt());

    if (sc.Close[sc.Index] > Subgraph_Average[sc.Index]) {
        Subgraph_ColoredBar[sc.Index] = sc.Index;
    }
}

/*==========================================================================*/
SCSFExport scsf_TradingExample(SCStudyInterfaceRef sc) {
    //Define references to the Subgraphs and Inputs for easy reference
    SCSubgraphRef Subgraph_BuyEntry = sc.Subgraph[0];
    SCSubgraphRef Subgraph_BuyExit = sc.Subgraph[1];
    SCSubgraphRef Subgraph_SellEntry = sc.Subgraph[2];
    SCSubgraphRef Subgraph_SellExit = sc.Subgraph[3];

    SCSubgraphRef Subgraph_SimpMovAvg = sc.Subgraph[4];

    if (sc.SetDefaults) {
        // Set the study configuration and defaults.

        sc.GraphName = "Trading Example: Entry and Exit on EMA crossower";

        Subgraph_BuyEntry.Name = "Buy Entry";
        Subgraph_BuyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
        Subgraph_BuyEntry.PrimaryColor = RGB(0, 255, 0);
        Subgraph_BuyEntry.LineWidth = 2;
        Subgraph_BuyEntry.DrawZeros = false;

        Subgraph_BuyExit.Name = "Buy Exit";
        Subgraph_BuyExit.DrawStyle = DRAWSTYLE_ARROW_DOWN;
        Subgraph_BuyExit.PrimaryColor = RGB(255, 128, 128);
        Subgraph_BuyExit.LineWidth = 2;
        Subgraph_BuyExit.DrawZeros = false;

        Subgraph_SellEntry.Name = "Sell Entry";
        Subgraph_SellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
        Subgraph_SellEntry.PrimaryColor = RGB(255, 0, 0);
        Subgraph_SellEntry.LineWidth = 2;
        Subgraph_SellEntry.DrawZeros = false;

        Subgraph_SellExit.Name = "Sell Exit";
        Subgraph_SellExit.DrawStyle = DRAWSTYLE_ARROW_UP;
        Subgraph_SellExit.PrimaryColor = RGB(128, 255, 128);
        Subgraph_SellExit.LineWidth = 2;
        Subgraph_SellExit.DrawZeros = false;

        Subgraph_SimpMovAvg.Name = "Simple Moving Average";
        Subgraph_SimpMovAvg.DrawStyle = DRAWSTYLE_LINE;
        Subgraph_SimpMovAvg.DrawZeros = false;

        sc.StudyDescription = "Opens and closes on EMA crossover.";

        sc.AutoLoop = 1;
        sc.GraphRegion = 0;

        //Any of the following variables can also be set outside and below the sc.SetDefaults code block

        sc.AllowMultipleEntriesInSameDirection = false;
        sc.MaximumPositionAllowed = 10;
        sc.SupportReversals = false;

        // This is false by default. Orders will go to the simulation system always.
        sc.SendOrdersToTradeService = false;
        sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;
        sc.SupportAttachedOrdersForTrading = false;
        sc.CancelAllOrdersOnEntriesAndReversals = true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = false;

        // Only 1 trade for each Order Action type is allowed per bar.
        sc.AllowOnlyOneTradePerBar = true;

        //This needs to be set to true when a trading study uses trading functions.
        sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }

    SCFloatArrayRef Last = sc.Close;

    // Calculate the moving average
    sc.SimpleMovAvg(Last, Subgraph_SimpMovAvg, sc.Index, 10);

    if (sc.IsFullRecalculation)
        return;

    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);

    float LastTradePrice = sc.Close[sc.Index];
    int64_t& r_BuyEntryInternalOrderID = sc.GetPersistentInt64(1);

    // Create an s_SCNewOrder object.
    s_SCNewOrder NewOrder;
    NewOrder.OrderQuantity = 1;
    NewOrder.OrderType = SCT_ORDERTYPE_MARKET;
    NewOrder.TextTag = "Trading example tag";
    NewOrder.TimeInForce = SCT_TIF_GOOD_TILL_CANCELED;

    int Result = 0;

    bool TradingAllowed = true;

    if (TradingAllowed && sc.CrossOver(Last, Subgraph_SimpMovAvg) == CROSS_FROM_BOTTOM && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {
        Result = (int)sc.BuyEntry(NewOrder);
        if (Result > 0) {
            r_BuyEntryInternalOrderID = NewOrder.InternalOrderID;
            SCString InternalOrderIDNumberString;
            InternalOrderIDNumberString.Format("BuyEntry Internal Order ID: %d", r_BuyEntryInternalOrderID);
            sc.AddMessageToLog(InternalOrderIDNumberString, 0);

            Subgraph_BuyEntry[sc.Index] = sc.Low[sc.Index];
        }
    }

    else if (PositionData.PositionQuantity > 0 && sc.CrossOver(Last, Subgraph_SimpMovAvg) == CROSS_FROM_TOP && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {
        Result = (int)sc.BuyExit(NewOrder);

        //If there has been a successful order entry, then draw an arrow at the high of the bar.
        if (Result > 0) {
            Subgraph_BuyExit[sc.Index] = sc.High[sc.Index];
        }
    }
}

/*==========================================================================*/

SCSFExport scsf_TradingExampleWithAttachedOrders(SCStudyInterfaceRef sc) {
    //Define references to the Subgraphs and Inputs for easy reference
    SCSubgraphRef Subgraph_BuyEntry = sc.Subgraph[0];
    SCSubgraphRef Subgraph_SellEntry = sc.Subgraph[2];
    SCSubgraphRef Subgraph_SimpMovAvg = sc.Subgraph[4];

    SCInputRef Input_Enabled = sc.Input[0];

    if (sc.SetDefaults) {
        // Set the study configuration and defaults.

        sc.GraphName = "Trading Example: With Trade Window Attached Orders";

        Subgraph_BuyEntry.Name = "Buy Entry";
        Subgraph_BuyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
        Subgraph_BuyEntry.PrimaryColor = RGB(0, 255, 0);
        Subgraph_BuyEntry.LineWidth = 2;
        Subgraph_BuyEntry.DrawZeros = false;

        Subgraph_SellEntry.Name = "Sell Entry";
        Subgraph_SellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
        Subgraph_SellEntry.PrimaryColor = RGB(255, 0, 0);
        Subgraph_SellEntry.LineWidth = 2;
        Subgraph_SellEntry.DrawZeros = false;

        Subgraph_SimpMovAvg.Name = "Simple Moving Average";
        Subgraph_SimpMovAvg.DrawStyle = DRAWSTYLE_LINE;
        Subgraph_SimpMovAvg.DrawZeros = false;

        Input_Enabled.Name = "Enabled";
        Input_Enabled.SetYesNo(0);

        // sc.StudyDescription = "This study function is an example of how to use the ACSIL Trading Functions.
        //                           This example uses the Attached Orders defined on the chart Trade Window.This function will display a simple moving average and
        //                           perform a Buy Entry
        //                               when the Last price crosses the moving average from below and
        //                           a Sell Entry
        //                               when the Last price crosses the moving average from above.A new entry cannot occur
        //                                   until the Target or
        //                       Stop has been hit.When an order is sent,
        // a corresponding
        //     arrow will appear on the chart to show that an order was sent.This study will do nothing until the Enabled Input is set to Yes.";

        sc.AutoLoop = 1;
        sc.GraphRegion = 0;

        //Any of the following variables can also be set outside and below the sc.SetDefaults code block

        sc.AllowMultipleEntriesInSameDirection = false;
        sc.MaximumPositionAllowed = 10;
        sc.SupportReversals = false;

        // This is false by default. Orders will go to the simulation system always.
        sc.SendOrdersToTradeService = false;

        sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;
        sc.SupportAttachedOrdersForTrading = true;

        // This line can be within sc.SetDefaults or outside of it.
        //sc.TradeWindowConfigFileName = "Test_2.twconfig";

        sc.CancelAllOrdersOnEntriesAndReversals = true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = true;

        // Only 1 trade for each Order Action type is allowed per bar.
        sc.AllowOnlyOneTradePerBar = true;

        //This needs to be set to true when a trading study uses trading functions.
        sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }

    if (!Input_Enabled.GetYesNo())
        return;

    // Calculate the moving average
    sc.SimpleMovAvg(sc.Close, Subgraph_SimpMovAvg, sc.Index, 10);

    if (sc.IsFullRecalculation)
        return;

    // Create an s_SCNewOrder object.
    s_SCNewOrder NewOrder;
    NewOrder.OrderQuantity = 1;
    NewOrder.OrderType = SCT_ORDERTYPE_MARKET;
    NewOrder.TimeInForce = SCT_TIF_GOOD_TILL_CANCELED;

    // Buy when the last price crosses the moving average from below.
    if (sc.CrossOver(sc.Close, Subgraph_SimpMovAvg) == CROSS_FROM_BOTTOM && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {
        //Buy Entry. Attached orders defined on Trade Window will be used.
        int Result = (int)sc.BuyEntry(NewOrder);
        if (Result > 0)  //If there has been a successful order entry, then draw an arrow at the low of the bar.
            Subgraph_BuyEntry[sc.Index] = sc.Low[sc.Index];
    }

    // Sell when the last price crosses the moving average from above.
    else if (sc.CrossOver(sc.Close, Subgraph_SimpMovAvg) == CROSS_FROM_TOP && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {
        //Sell Entry. Attached orders defined on Trade Window will be used.
        int Result = (int)sc.SellEntry(NewOrder);
        if (Result > 0)  //If there has been a successful order entry, then draw an arrow at the high of the bar.
            Subgraph_SellEntry[sc.Index] = sc.High[sc.Index];
    }
}

/*==========================================================================*/
SCSFExport scsf_TradingExampleWithAttachedOrdersDirectlyDefined(SCStudyInterfaceRef sc) {
    // Define references to the Subgraphs and Inputs for easy reference
    SCSubgraphRef Subgraph_BuyEntry = sc.Subgraph[0];
    SCSubgraphRef Subgraph_SellEntry = sc.Subgraph[2];
    SCSubgraphRef Subgraph_SimpMovAvg = sc.Subgraph[4];

    SCInputRef Enabled = sc.Input[0];

    if (sc.SetDefaults) {
        // Set the study configuration and defaults.

        sc.GraphName = "Trading Example: With Hardcoded Attached Orders";

        Subgraph_BuyEntry.Name = "Buy Entry";
        Subgraph_BuyEntry.DrawStyle = DRAWSTYLE_ARROW_UP;
        Subgraph_BuyEntry.PrimaryColor = RGB(0, 255, 0);
        Subgraph_BuyEntry.LineWidth = 2;
        Subgraph_BuyEntry.DrawZeros = false;

        Subgraph_SellEntry.Name = "Sell Entry";
        Subgraph_SellEntry.DrawStyle = DRAWSTYLE_ARROW_DOWN;
        Subgraph_SellEntry.PrimaryColor = RGB(255, 0, 0);
        Subgraph_SellEntry.LineWidth = 2;
        Subgraph_SellEntry.DrawZeros = false;

        Subgraph_SimpMovAvg.Name = "Simple Moving Average";
        Subgraph_SimpMovAvg.DrawStyle = DRAWSTYLE_LINE;
        Subgraph_SimpMovAvg.DrawZeros = false;

        Enabled.Name = "Enabled";
        Enabled.SetYesNo(0);

        // sc.StudyDescription = "This study function is an example of how to use the ACSIL Trading Functions.
        //  This example uses Attached Orders defined directly within this function.
        //  This function will display a simple moving average and perform a Buy Entry
        //  when the Last price crosses the moving average from below and a Sell Entry
        //  when the Last price crosses the moving average from above. A new entry cannot
        //   occur until the Target or Stop has been hit. When an order is sent, a
        //    corresponding arrow will appear on the chart to show that an order was sent.
        //    This study will do nothing until the Enabled Input is set to Yes.";

        sc.AllowMultipleEntriesInSameDirection = false;
        sc.MaximumPositionAllowed = 5;
        sc.SupportReversals = false;

        // This is false by default. Orders will go to the simulation system always.
        sc.SendOrdersToTradeService = false;

        sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;

        // This can be false in this function because we specify Attached Orders directly with the order which causes this to be considered true when submitting an order.
        sc.SupportAttachedOrdersForTrading = false;

        sc.CancelAllOrdersOnEntriesAndReversals = true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = true;

        // Only 1 trade for each Order Action type is allowed per bar.
        sc.AllowOnlyOneTradePerBar = true;

        //This needs to be set to true when a trading study uses trading functions.
        sc.MaintainTradeStatisticsAndTradesData = true;

        sc.AutoLoop = 1;
        sc.GraphRegion = 0;

        return;
    }

    if (!Enabled.GetYesNo())
        return;

    // Use persistent variables to remember attached order IDs so they can be modified or canceled.
    int32_t& Target1OrderID = sc.GetPersistentInt(1);
    int32_t& Stop1OrderID = sc.GetPersistentInt(2);

    // Calculate the moving average
    sc.SimpleMovAvg(sc.Close, Subgraph_SimpMovAvg, sc.Index, 10);

    if (sc.IsFullRecalculation)
        return;

    // Create an s_SCNewOrder object.
    s_SCNewOrder NewOrder;
    NewOrder.OrderQuantity = 1;
    NewOrder.OrderType = SCT_ORDERTYPE_MARKET;
    NewOrder.TimeInForce = SCT_TIF_GOOD_TILL_CANCELED;

    //Specify a Target and a Stop with 8 tick offsets. We are specifying one Target and one Stop, additional targets and stops can be specified as well.
    NewOrder.Target1Offset = 8 * sc.TickSize;
    NewOrder.Stop1Offset = 8 * sc.TickSize;
    NewOrder.OCOGroup1Quantity = 1;  // If this is left at the default of 0, then it will be automatically set.

    // Buy when the last price crosses the moving average from below.
    if (sc.CrossOver(sc.Close, Subgraph_SimpMovAvg) == CROSS_FROM_BOTTOM && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {
        int Result = (int)sc.BuyEntry(NewOrder);
        if (Result > 0)  //If there has been a successful order entry, then draw an arrow at the low of the bar.
        {
            Subgraph_BuyEntry[sc.Index] = sc.Low[sc.Index];

            // Remember the order IDs for subsequent modification and cancellation
            Target1OrderID = NewOrder.Target1InternalOrderID;
            Stop1OrderID = NewOrder.Stop1InternalOrderID;
        }
    }

    // This code block serves as an example of how to modify an attached order.  In this example it is set to false and never runs.
    bool ExecuteModifyOrder = false;
    if (ExecuteModifyOrder && (sc.Index == sc.ArraySize - 1))  //Only  do a modify on the most recent bar
    {
        double NewLimit = 0.0;

        // Get the existing target order
        s_SCTradeOrder ExistingOrder;
        if (sc.GetOrderByOrderID(Target1OrderID, ExistingOrder) != SCTRADING_ORDER_ERROR) {
            if (ExistingOrder.BuySell == BSE_BUY)
                NewLimit = sc.Close[sc.Index] - 5 * sc.TickSize;
            else if (ExistingOrder.BuySell == BSE_SELL)
                NewLimit = sc.Close[sc.Index] + 5 * sc.TickSize;

            // We can only modify price and/or quantity

            s_SCNewOrder ModifyOrder;
            ModifyOrder.InternalOrderID = Target1OrderID;
            ModifyOrder.Price1 = NewLimit;

            sc.ModifyOrder(ModifyOrder);
        }
    }

    ////Target 1
    //NewOrder.Target1Offset =10*sc.TickSize;
    //NewOrder.AttachedOrderTarget1Type = SCT_ORDERTYPE_LIMIT;

    ////Target 2
    //NewOrder.Target2Offset = 10*sc.TickSize;
    //NewOrder.AttachedOrderTarget2Type = SCT_ORDERTYPE_LIMIT;

    ////Common Stop
    //NewOrder.StopAllOffset = 10* sc.TickSize;
    //NewOrder.AttachedOrderStopAllType = SCT_ORDERTYPE_STOP;
}
