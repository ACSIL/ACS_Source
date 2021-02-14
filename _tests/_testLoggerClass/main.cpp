#include <iostream>

#include "Logger.h"

int main() {
    Logger logger("myFillle.txt");
    logger.setHeader("sds,sdsa,dsasasa,sfsaf,fsfsaf");
    logger.writeToFile();
    return 0;
}