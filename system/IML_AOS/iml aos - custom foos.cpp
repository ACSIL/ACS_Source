#include "sierrachart.h"
#include "iml aos.h"

bool is_time_to_trade(SCStudyInterfaceRef sc)
{
    if (sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetTime() > sc.Input[17].GetTime() && sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetTime() < sc.Input[18].GetTime())
        return 1;
    return 0;
}










//
//SCString log_sc_nq(SCStudyInterfaceRef sc)
//{
//    SCString log;
//    log.Format("Price: %f, Date and Time: %i-%i-%i  %0.2i:%0.2i:%0.2i | Daily range (in ticks): %i | ATR: %0.04f",
//        nq.entry_price, nq.year, nq.month, nq.day, nq.hour, nq.minute, nq.second, nq.range, nq.atr_val);
//    return log;
//}
//SCString log_sc_es(SCStudyInterfaceRef sc)
//{
//    SCString log;
//    log.Format("Price: %f, Date and Time: %i-%i-%i  %0.2i:%0.2i:%0.2i | Daily range (in ticks): %i | ATR: %0.04f",
//        es.entry_price, es.year, es.month, es.day, es.hour, es.minute, es.second, es.range, es.atr_val);
//    return log;
//}
//SCString log_sc_rty(SCStudyInterfaceRef sc)
//{
//    SCString log;
//    log.Format("Price: %f, Date and Time: %i-%i-%i  %0.2i:%0.2i:%0.2i | Daily range (in ticks): %i | ATR: %0.04f",
//        rty.entry_price, rty.year, rty.month, rty.day, rty.hour, rty.minute, rty.second, rty.range, rty.atr_val);
//    return log;
//}



//void log_txt_nq(SCStudyInterfaceRef sc)
//{
//    std::string file_name{ "test nq" }; //calling custom function 
//    std::ofstream out_file{};
//    out_file.open(file_name, std::ofstream::app);
//    if (out_file.is_open())
//    {
//        std::string test_logu{ log_sc_nq(sc) }; //calling custom function 
//        out_file << test_logu << '\n';
//    }
//    else
//    {
//        SCString log_error{ "Error writing into file" };
//        sc.AddMessageToLog(log_error, 1);
//    }
//    return;
//}
//void log_txt_es(SCStudyInterfaceRef sc)
//{
//    std::string file_name{ "test es" }; //calling custom function 
//    std::ofstream out_file{};
//    out_file.open(file_name, std::ofstream::app);
//    if (out_file.is_open())
//    {
//        std::string test_logu{ log_sc_es(sc) }; //calling custom function 
//        out_file << test_logu << '\n';
//    }
//    else
//    {
//        SCString log_error{ "Error writing into file" };
//        sc.AddMessageToLog(log_error, 1);
//    }
//    return;
//}
//void log_txt_rty(SCStudyInterfaceRef sc)
//{
//    std::string file_name{ "test rty" }; //calling custom function 
//    std::ofstream out_file{};
//    out_file.open(file_name, std::ofstream::app);
//    if (out_file.is_open())
//    {
//        std::string test_logu{ log_sc_rty(sc) }; //calling custom function 
//        out_file << test_logu << '\n';
//    }
//    else
//    {
//        SCString log_error{ "Error writing into file" };
//        sc.AddMessageToLog(log_error, 1);
//    }
//    return;
//}
