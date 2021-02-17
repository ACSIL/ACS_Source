#include <iostream>
#include <string>
#include <string>

void splitString() {
    std::string s = "ESDF[5] 60m #2";
    std::cout << s.substr(0, s.rfind(' '));
}


int main() {
    splitString();

    return 0;
}