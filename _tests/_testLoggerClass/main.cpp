#include <iostream>

#include "Logger.hpp"

int main() {
    Logger logger("myFillle.txt");
    logger.setHeader("ahoj, tady, je, muj, pes");
    
    std::string header = logger.getHeader();

    std::cout << header;


    return 0;
}