#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void writeData(const char* fileName) {
    std::ofstream outFile;
    outFile.open(fileName, ofstream::app);
    if (outFile.is_open()) {
        string data = "4, 432.231, ESH, 34";
        int i = 5;
        while (i != 0) {
            outFile << data << '\n';
            --i;
        }
        outFile.close();
    }
}

void createFileAndWriteHeader(const char* fileName) {
    std::ofstream outFile;
    outFile.open(fileName, ofstream::app);
    string header = "positin, price, market, volume";
    if (outFile.is_open()) {
        outFile << header << '\n';
    }
}

bool fileIsEmpty(const char* fileName) {
    std::ifstream inFile(fileName);
    return inFile.peek() == std::ifstream::traits_type::eof();
}

bool fileExists(const char* fileName) {
    ifstream file(fileName);
    return file.good();
}

void writeToFile() {
    const char* fileName = "test.txt";
    if (!fileExists(fileName)) createFileAndWriteHeader(fileName);
    writeData(fileName);
}

int main() {
    writeToFile();
}