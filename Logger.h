#pragma once

#include <fstream>
#include <iostream>

class Logger {
   public:
    Logger(const std::string &fileName);
    // ~Logger();

    void setHeader(const std::string &fileHeader);
    std::string getHeader();
    // void writeToFile();
    // void writeToMessageLog();

   private:
    std::string fileName;
    std::string fileHeader;

//     bool fileExists();
//     bool fileIsEmpty();
//     void writeHeader();
//     void writeEntryData();
};

inline Logger::Logger(const std::string &fileName) {
    this->fileName = fileName;
}

inline void Logger::setHeader(const std::string &fileHeader){
    this->fileHeader = fileHeader;
}

inline std::string Logger::getHeader(){
    return fileHeader;
}