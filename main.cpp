#include <iostream>
#include "Printer.h"


int main() {
    std::string input;
    Printer *printer = new Printer();
    while(true) {
        std::getline(std::cin, input);
        if(input == "break")
            break;
        printer->putStr(input);
        printer->calculateAndPrint();
    }
    return 0;
}