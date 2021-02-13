#include "sierrachart.h"
#include <fstream>
#include <string>

SCDLLName("Logger")

//dat do grafu kde vsechno vypisu 
SCSFExport scsf_ReadValuesFromPerzistVarsAndLogIntoTXT(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.Input[0].Name = "Chart for Tick";
        sc.Input[0].SetChartStudyValues(1, 0);
        sc.Input[1].Name = "Chart for ATR";
        sc.Input[1].SetChartStudyValues(2, 0);
        sc.Input[2].Name = "Chart for WTR";
        sc.Input[2].SetChartStudyValues(3, 0);
        sc.Input[3].Name = "Chart for DOM Ask-Bid";
        sc.Input[3].SetChartStudyValues(4, 0);
        sc.Input[4].Name = "Chart for DOM Bid-Ask";
        sc.Input[4].SetChartStudyValues(5, 0);
        sc.GraphRegion = 0;//same as main
        sc.GraphName = "Read the perzists and log into txt";
        sc.FreeDLL = 1;
        sc.UpdateAlways = 1;
        return;
    }
    //get the perzist data
    int tick_value = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[0].GetChartNumber(), sc.Input[0].GetStudyID(), 0));
    float atr_value = sc.GetStudyPersistentFloatFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), 0);
    int wtr_value = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), 0));
    int dom_quotes_ab = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), 0));
    int dom_quotes_ba = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[4].GetChartNumber(), sc.Input[4].GetStudyID(), 0));

    int dom{};
    if (dom_quotes_ab > abs(dom_quotes_ba)) dom = dom_quotes_ab;
    else dom = dom_quotes_ba;

    //set the internal sierra logs message details

    int &previous_qt_perzist = sc.GetPersistentInt(0);
    int &already_logged_perzist = sc.GetPersistentInt(1);
    SCString log;
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);
    std::string symbol{ PositionData.Symbol };
    t_OrderQuantity32_64 qty{ PositionData.PositionQuantity };

    auto last_entry_dt = PositionData.LastEntryDateTime;
    int Year, Month, Day, Hour, Minute, Second;
    last_entry_dt.GetDateTimeYMDHMS(Year, Month, Day, Hour, Minute, Second);
    log.Format("Entry DateTime: %i-%i-%i, %0.2i:%0.2i:%0.2i,  Entry price: %3.2f, Symbol: %s, Quantity: %3.0f, ATR: %3.2f, WTR: %i, DOM: %i", Year, Month, Day, Hour, Minute, Second, PositionData.AveragePrice, symbol, qty, atr_value, wtr_value, dom);



    if (previous_qt_perzist == 0 && PositionData.PositionQuantity != 0)
    {
        if (already_logged_perzist == 0)
        {
            if (symbol.at(0) == 'N' || symbol.at(5) == 'N') //log details for NQ entries into separate txt file
            {
                std::string file_name{ "Entry logs NQ.txt" }; //custom foo crating string
                std::ofstream out_file{};
                out_file.open(file_name, std::ofstream::app);
                if (out_file.is_open())
                {
                    std::string test_logu{ log };
                    out_file << test_logu << '\n';
                }
                else
                {
                    SCString log_error{ "Error writing into file" };
                    sc.AddMessageToLog(log_error, 1);
                }
                out_file.close();
            }
            else if (symbol.at(0) == 'Y' || symbol.at(5) == 'Y') //log details for YM entries into separate txt file
            {
                std::string file_name{ "Entry logs YM.txt" };
                std::ofstream out_file{};
                out_file.open(file_name, std::ofstream::app);
                if (out_file.is_open())
                {
                    std::string test_logu{ log };
                    out_file << test_logu << '\n';
                }
                else
                {
                    SCString log_error{ "Error writing into file" };
                    sc.AddMessageToLog(log_error, 1);
                }
                out_file.close();
            }
            else if (symbol.at(0) == 'R' || symbol.at(5) == 'R') //log details for RTY entries into separate txt file
            {
                std::string file_name{ "Entry logs RTY.txt" };
                std::ofstream out_file{};
                out_file.open(file_name, std::ofstream::app);
                if (out_file.is_open())
                {
                    std::string test_logu{ log };
                    out_file << test_logu << '\n';
                }
                else
                {
                    SCString log_error{ "Error writing into file" };
                    sc.AddMessageToLog(log_error, 1);
                }
                out_file.close();
            }
            //internal sierra logs
            sc.AddMessageToLog(log, 0);
            already_logged_perzist = 1;
        }
    }
    if (PositionData.PositionQuantity == 0) { already_logged_perzist = 0; }	//reset the persist var for loging to zero again

    //draw the data into chart																		
    {
        s_UseTool tick;
        tick.Clear();
        tick.ChartNumber = sc.ChartNumber;
        tick.DrawingType = DRAWING_TEXT;
        tick.FontSize = 10;
        tick.FontBold = false;
        tick.AddMethod = UTAM_ADD_OR_ADJUST;
        tick.UseRelativeVerticalValues = 1;
        tick.BeginDateTime = 7;
        tick.BeginValue = 90;
        tick.Color = RGB(255, 255, 255);
        tick.Region = 0;
        tick.Text.Format("Tick: %i", tick_value);
        tick.LineNumber = 1;
        sc.UseTool(tick);

        s_UseTool atr;
        atr.Clear();
        atr.ChartNumber = sc.ChartNumber;
        atr.DrawingType = DRAWING_TEXT;
        atr.FontSize = 10;
        atr.FontBold = false;
        atr.AddMethod = UTAM_ADD_OR_ADJUST;
        atr.UseRelativeVerticalValues = 1;
        atr.BeginDateTime = 7;
        atr.BeginValue = 87;
        atr.Color = RGB(255, 255, 255);
        atr.Region = 0;
        atr.Text.Format("ATR: %3.2f", atr_value);
        atr.LineNumber = 2;
        sc.UseTool(atr);

        s_UseTool wtr;
        wtr.Clear();
        wtr.ChartNumber = sc.ChartNumber;
        wtr.DrawingType = DRAWING_TEXT;
        wtr.FontSize = 10;
        wtr.FontBold = false;
        wtr.AddMethod = UTAM_ADD_OR_ADJUST;
        wtr.UseRelativeVerticalValues = 1;
        wtr.BeginDateTime = 7;
        wtr.BeginValue = 84;
        wtr.Color = RGB(255, 255, 255);
        wtr.Region = 0;
        wtr.Text.Format("WTR: %i", wtr_value);
        wtr.LineNumber = 3;
        sc.UseTool(wtr);
    }
}