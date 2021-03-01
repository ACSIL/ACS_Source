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

    util::Display::display(sc, "testing..", 10, 90, "string");
    util::Display::display(sc, 1111, 10, 88, "int");
    util::Display::display(sc, 3.2124, 10, 86, "double");
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
