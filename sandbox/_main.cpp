#include <ctime>
#include <fstream>
#include <iomanip>
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
    outFile.close();
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

// int main() {
//     void getCurrentTime();
// }

std::string getCurrentTime() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    return oss.str();
}

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

// zobrazuje nulu kdyz vyskocim z loopu protoze cin vynuluje ten proud
void circularArray(){
    int arr[3] = {};
    int input;
    int arrLength = sizeof(arr) / sizeof(arr[0]);
    int index = 0;
    
    while (cin) {
        cin >> input;
        arr[index++] = input;
        if (index >= arrLength) index = 0;
        
    }
    cout << "=============\n";
    for (int i : arr) cout << i << "\n";
}



int main() {

   
}