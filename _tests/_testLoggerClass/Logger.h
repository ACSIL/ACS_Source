#pragma once

#include <fstream>
#include <iostream>

class Logger {
   public:
    Logger(const std::string &fileName);
    // ~Logger();

    void setHeader(const std::string &fileHeader);
    void writeToFile();
    void writeToMessageLog();

   private:
    std::string fileName;
    std::string fileHeader; // setter nebo konstruktor

    bool fileExists();
    bool fileIsEmpty();
    void writeHeader();
    void writeEntryData();
};


inline Logger::Logger(const std::string &fileName) {
    this->fileName = fileName;
}

inline void Logger::setHeader(const std::string &fileHeader){
    this->fileHeader = fileHeader;
}

inline void Logger::writeToFile() {
    if (!fileExists()) writeHeader();
    writeEntryData();
}

inline void Logger::writeToMessageLog() {
    return;
}

inline bool Logger::fileIsEmpty() {
    std::ifstream inFile(fileName);
    return inFile.peek() == std::ifstream::traits_type::eof();
}

inline bool Logger::fileExists() {
    std::ifstream file(fileName);
    return file.good();
}

inline void Logger::writeHeader() {
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        outFile << fileHeader << '\n';
    }
    outFile.close();
}

inline void Logger::writeEntryData(){
    std::ofstream outFile;
    outFile.open(fileName, std::ofstream::app);
    if (outFile.is_open()) {
        std::string data = "4, 432.231, ESH, 34";
        int i = 5;
        while (i != 0) {
            outFile << data << '\n';
            --i;
        }
        outFile.close();
    }
}

