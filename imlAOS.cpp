
  
#include "sierrachart.h"
#include "iml aos.h"


struct data
{
    double entry_price{ };
    double range{};
    double atr_val{};
    float avg_volume{};
    int year{}, month{}, day{}, hour{}, minute{}, second{};

} nq;

//string for internal and external log
SCString log_sc_nq(SCStudyInterfaceRef sc)
{
    s_SCPositionData current_postion;
    sc.GetTradePosition(current_postion);
    nq.entry_price = current_postion.AveragePrice;
    double entry_dt = current_postion.LastFillDateTime;
    DATETIME_TO_YMDHMS(entry_dt, nq.year, nq.month, nq.day, nq.hour, nq.minute, nq.second);

    SCString log;
    log.Format("Cena: %f, Date and Time: %i-%i-%i  %0.2i:%0.2i:%0.2i | Daily range (in ticks): %f | ATR: %0.04f",
        nq.entry_price, nq.year, nq.month, nq.day, nq.hour, nq.minute, nq.second, nq.range, nq.atr_val);
    return log;
}

//external log into txt file
void log_txt_nq(SCStudyInterfaceRef sc)
{
    std::string file_name{ "NQ trades.txt" }; //calling custom function 
    std::ofstream out_file{};
    out_file.open(file_name, std::ofstream::app);
    if (out_file.is_open())
    {
        std::string test_logu{ log_sc_nq(sc) }; //calling custom function 
        out_file << test_logu << '\n';
    }
    else
    {
        SCString log_error{ "Error writing into file" };
        sc.AddMessageToLog(log_error, 1);
    }
    return;
}


SCSFExport scsf_MAIN_NQ_IML_REV_GetPersistsAndTrade(SCStudyInterfaceRef sc)
{
    SCInputRef iml_buffer_validity = sc.Input[4];
    SCInputRef minimal_range = sc.Input[6];
    SCInputRef display_ut = sc.Input[8];

    SCInputRef start_trading = sc.Input[17];
    SCInputRef stop_trading = sc.Input[18];
    SCInputRef time_to_flat_dt = sc.Input[19];

    if (sc.SetDefaults)
    {
        start_trading.Name = "Trade from: ";
        start_trading.SetTime(HMS_TIME(8, 30, 0));
        stop_trading.Name = "Trade from: ";
        stop_trading.SetTime(HMS_TIME(10, 00, 0));

        display_ut.Name = "Display Logs";
        display_ut.SetYesNo(TRUE);
        time_to_flat_dt.Name = "Time to flat";
        time_to_flat_dt.SetTime(HMS_TIME(10, 30, 0));

        sc.Input[0].SetChartStudyValues(1, 1);
        sc.Input[1].SetChartStudyValues(2, 1);
        sc.Input[2].SetChartStudyValues(3, 1);
        sc.Input[3].SetChartStudyValues(4, 1);
        sc.Input[4].SetInt(10);
        sc.Input[5].SetChartStudyValues(sc.ChartNumber, 1);
        sc.Input[6].Name = "Minimal range in ticks";
        sc.Input[6].SetInt(22);

        sc.GraphRegion = 0;
        sc.GraphName = "main: IML REV NQ - Get persists and trade";
        sc.AutoLoop = 1;
        sc.UpdateAlways = TRUE;
        sc.FreeDLL = 0;
        return;
    }
    sc.AllowMultipleEntriesInSameDirection = true;
    sc.MaximumPositionAllowed = 1000;
    sc.SupportReversals = false;
    sc.SendOrdersToTradeService = false;
    sc.AllowOppositeEntryWithOpposingPositionOrOrders = true;
    sc.SupportAttachedOrdersForTrading = true;
    sc.CancelAllOrdersOnEntriesAndReversals = false;
    sc.AllowEntryWithWorkingOrders = false;
    sc.CancelAllWorkingOrdersOnExit = false;
    sc.AllowOnlyOneTradePerBar = true;
    
    //geting the return values from the context study (break dhl)
    float return_val_from_es = sc.GetStudyPersistentFloatFromChart(sc.Input[0].GetChartNumber(), sc.Input[0].GetStudyID(), 0);
    float return_val_from_ym = sc.GetStudyPersistentFloatFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), 0);
    float return_val_from_rty = sc.GetStudyPersistentFloatFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), 0);
    float return_val_from_nq = sc.GetStudyPersistentFloatFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), 0);
    double daily_high = sc.GetStudyPersistentDoubleFromChart(sc.ChartNumber, sc.Input[5].GetStudyID(), 0);
    double daily_low = sc.GetStudyPersistentDoubleFromChart(sc.ChartNumber, sc.Input[5].GetStudyID(), 1);
    double daily_mid = sc.GetStudyPersistentDoubleFromChart(sc.ChartNumber, sc.Input[5].GetStudyID(), 2);
    double daily_low_mid = sc.GetStudyPersistentDoubleFromChart(sc.ChartNumber, sc.Input[5].GetStudyID(), 3);
    double daily_high_mid = sc.GetStudyPersistentDoubleFromChart(sc.ChartNumber, sc.Input[5].GetStudyID(), 4);

    //trade only when the range is wide enough
    nq.range = (daily_high - daily_low) / sc.TickSize;
    if (nq.range < minimal_range.GetInt())
        return;
    //setting control perzist vars
    int &bool_ready_to_trade_LONG = sc.GetPersistentInt(0);
    int &bool_ready_to_trade_SHORT = sc.GetPersistentInt(1);   
    int &bool_logged = sc.GetPersistentInt(2);
    
    //get time of bar when IMD is valid
    auto &pv_iml_valid_from_dt = sc.GetPersistentSCDateTime(0);
    int iml_valid_from = pv_iml_valid_from_dt.GetTime();
    int iml_valid_from_year, iml_valid_from_month, iml_valid_from_day, iml_valid_from_hour, iml_valid_from_minute, iml_valid_from_second;
    pv_iml_valid_from_dt.GetDateTimeYMDHMS(iml_valid_from_year, iml_valid_from_month, iml_valid_from_day, iml_valid_from_hour, iml_valid_from_minute, iml_valid_from_second);

    //get time of bar X minutes from the last time when IML was valid
    auto iml_valid_till_dt = static_cast<SCDateTime>(pv_iml_valid_from_dt + iml_buffer_validity.GetInt()*MINUTES);
    int iml_valid_till_year, iml_valid_till_month, iml_valid_till_day, iml_valid_till_hour, iml_valid_till_minute, iml_valid_till_second;
    iml_valid_till_dt.GetDateTimeYMDHMS(iml_valid_till_year, iml_valid_till_month, iml_valid_till_day, iml_valid_till_hour, iml_valid_till_minute, iml_valid_till_second);
    int iml_valid_till_bar_index = sc.GetNearestMatchForSCDateTime(sc.ChartNumber, iml_valid_till_dt);

    int &return_val_sum = sc.GetPersistentInt(10);
    return_val_sum = static_cast<int>(return_val_from_es + return_val_from_ym + return_val_from_rty);

    //IML context for --------LONG
    if (return_val_sum <= min_sum_short
        && return_val_from_es < 0
        && return_val_from_ym < 0
        && return_val_from_rty < 0
        && return_val_from_nq > 0)
    {
        bool_ready_to_trade_LONG = 1;
        pv_iml_valid_from_dt = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar];
    }
    //IML context for --------SHORT
    if (return_val_sum >= min_sum_long
        && return_val_from_es > 0
        && return_val_from_ym > 0
        && return_val_from_rty > 0
        && return_val_from_nq < 0)
    {
        bool_ready_to_trade_SHORT = 1;
        pv_iml_valid_from_dt = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar];
    }
    //set the positions
    s_SCPositionData current_postion;
    sc.GetTradePosition(current_postion);
    double sl_offset = round(daily_high_mid - daily_low_mid) / sc.TickSize;
    //set order
    s_SCNewOrder order;
    order.OrderType = SCT_ORDERTYPE_MARKET_IF_TOUCHED;
    order.OrderQuantity = 1;
    order.AttachedOrderTarget1Type = SCT_ORDERTYPE_LIMIT;
    order.AttachedOrderStopAllType = SCT_ORDERTYPE_STOP;
    order.MoveToBreakEven.Type = MOVETO_BE_ACTION_TYPE_OFFSET_TRIGGERED;
    order.MoveToBreakEven.TriggerOffsetInTicks = static_cast<int>(sl_offset / 2);
    order.MoveToBreakEven.BreakEvenLevelOffsetInTicks = 0;

    //open positions
    if (is_time_to_trade(sc))
    {
        if (bool_ready_to_trade_LONG && sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] < iml_valid_till_dt && sc.Close[sc.IndexOfLastVisibleBar] <= daily_low_mid)
        {
            order.Target1Price = daily_high_mid;
            order.StopAllOffset = current_postion.AveragePrice - sl_offset * sc.TickSize;
            t_OrderQuantity32_64 entry_check = sc.BuyEntry(order);
            if (entry_check)
            {
                sc.AddMessageToLog(log_sc_nq(sc), 0); // custom foo (internal log)
                log_txt_nq(sc); //  custom function (external log) 
                bool_ready_to_trade_LONG = 0;
                bool_logged = 0;
            }
        }
        else if (bool_ready_to_trade_SHORT && sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] < iml_valid_till_dt && sc.Close[sc.IndexOfLastVisibleBar] >= daily_high_mid)
        {
            order.Target1Price = daily_low_mid;
            order.StopAllOffset = current_postion.AveragePrice + sl_offset * sc.TickSize;
            t_OrderQuantity32_64 entry_check = sc.SellEntry(order);
            if (entry_check)
            {
                sc.AddMessageToLog(log_sc_nq(sc), 0); //  custom foo (internal log)
                log_txt_nq(sc); // calling custom function (external log) 
                bool_ready_to_trade_SHORT = 0;
                bool_logged = 0;
            }
        }
    }
    //reset pv when in position or when the time of iml validity passes over
    if (current_postion.PositionQuantity != 0 || sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] > iml_valid_till_dt)
    {
        bool_ready_to_trade_SHORT = 0;
        bool_ready_to_trade_LONG = 0;
        sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, 100);
    }

    //reset pv when when the markets are no longer discorelated enough
    if (return_val_sum > 0)
    {
        bool_ready_to_trade_LONG = 0;
        sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, 100);
    }
    if (return_val_sum < 0)
    {
        bool_ready_to_trade_SHORT = 0;
        sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, 100);
    }

    if (current_postion.PositionQuantity != 0 && sc.BaseDateTimeIn[sc.Index].GetTime() >= time_to_flat_dt.GetTime()) { sc.FlattenAndCancelAllOrders(); }
    if (sc.BaseDateTimeIn[sc.Index] == 0) { sc.ClearAllPersistentData; }

    if (display_ut.GetYesNo())
    {
        s_UseTool sum;
        sum.Clear();
        sum.ChartNumber = sc.ChartNumber;
        sum.DrawingType = DRAWING_TEXT;
        sum.FontSize = 7;
        sum.FontBackColor = RGB(0, 0, 0);
        sum.FontBold = false;
        sum.AddMethod = UTAM_ADD_OR_ADJUST;
        sum.UseRelativeVerticalValues = 1;
        sum.Color = RGB(255, 255, 255);
        sum.Region = 0;
        sum.Text.Format("ES: %.0f, YM: %.0f, RTY: %.0f -> SUM: %i | NQ: %.0f  ", return_val_from_es, return_val_from_ym, return_val_from_rty, return_val_sum, return_val_from_nq);
        sum.LineNumber = 1;
        sum.BeginDateTime = 1;
        sum.BeginValue = 93;
        sc.UseTool(sum);

        s_UseTool ready_l;
        ready_l.Clear();
        ready_l.ChartNumber = sc.ChartNumber;
        ready_l.DrawingType = DRAWING_TEXT;
        ready_l.FontBackColor = RGB(0, 0, 0);
        ready_l.FontSize = 7;
        ready_l.FontBold = false;
        ready_l.AddMethod = UTAM_ADD_OR_ADJUST;
        ready_l.UseRelativeVerticalValues = 1;
        ready_l.Color = RGB(255, 255, 255);
        ready_l.Region = 0;
        ready_l.Text.Format("READY TO TRADE IML REV LONG: %i", bool_ready_to_trade_LONG);
        ready_l.LineNumber = 2;
        ready_l.BeginDateTime = 1;
        ready_l.BeginValue = 90;
        sc.UseTool(ready_l);

        s_UseTool ready_s;
        ready_s.Clear();
        ready_s.ChartNumber = sc.ChartNumber;
        ready_s.DrawingType = DRAWING_TEXT;
        ready_s.FontBackColor = RGB(0, 0, 0);
        ready_s.FontSize = 7;
        ready_s.FontBold = false;
        ready_s.AddMethod = UTAM_ADD_OR_ADJUST;
        ready_s.UseRelativeVerticalValues = 1;
        ready_s.Color = RGB(255, 255, 255);
        ready_s.Region = 0;
        ready_s.Text.Format("READY TO TRADE IML REV SHORT: %i", bool_ready_to_trade_SHORT);
        ready_s.LineNumber = 3;
        ready_s.BeginDateTime = 1;
        ready_s.BeginValue = 87;
        sc.UseTool(ready_s);

        s_UseTool iml;
        iml.Clear();
        iml.ChartNumber = sc.ChartNumber;
        iml.DrawingType = DRAWING_TEXT;
        iml.FontBackColor = RGB(0, 0, 0);
        iml.FontSize = 8;
        iml.FontBold = false;
        iml.AddMethod = UTAM_ADD_OR_ADJUST;
        iml.UseRelativeVerticalValues = 1;
        iml.Color = RGB(255, 255, 255);
        iml.Region = 0;
        iml.LineNumber = 100;
        iml.BeginDateTime = 55;
        iml.BeginValue = 84;
        if (sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] > start_trading.GetTime() && bool_ready_to_trade_LONG == 1)
        {
            iml.Text.Format("IML REV LONG %s: %i-%i-%i %i:%0.2i:%0.2i - %i-%i-%i %i:%0.2i:%0.2i",
                static_cast<const char *>(sc.GetChartSymbol(sc.ChartNumber)), iml_valid_from_year, iml_valid_from_month, iml_valid_from_day, iml_valid_from_hour, iml_valid_from_minute, iml_valid_from_second, iml_valid_till_year, iml_valid_till_month, iml_valid_till_day, iml_valid_till_hour, iml_valid_till_minute, iml_valid_till_second);
        }
        else if (sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] > start_trading.GetTime() && bool_ready_to_trade_SHORT == 1)
        {
            iml.Text.Format("IML REV SHORT %s: %i-%i-%i %i:%0.2i:%0.2i - %i-%i-%i %i:%0.2i:%0.2i",
                static_cast<const char *>(sc.GetChartSymbol(sc.ChartNumber)), iml_valid_from_year, iml_valid_from_month, iml_valid_from_day, iml_valid_from_hour, iml_valid_from_minute, iml_valid_from_second, iml_valid_till_year, iml_valid_till_month, iml_valid_till_day, iml_valid_till_hour, iml_valid_till_minute, iml_valid_till_second);
        }
        sc.UseTool(iml);

        if (sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] >= iml_valid_till_dt) { sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, 100); }

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
        sl.Text.Format("sl: %.0f", sl_offset);
        sl.LineNumber = 20;
        sl.BeginDateTime = 1;
        sl.BeginValue = 70;
        sc.UseTool(sl);
    }
}

