//
// Created by User on 06.04.2017.
//

#include <string>
#include <vector>

#include "printables.h"

#ifndef HW4_PRINTER_H
#define HW4_PRINTER_H

class Printer {
public:
    Printer();
    Printer(std::string str);
    void putChar(char c);
    void putInt(int n);
    void putStr(const char* str);
    void putStr(std::string str);
    void calculateAndPrint();
    void clear();
private:
    std::string text;
    std::string::iterator cur;
    std::string::iterator end;
    int parse();
    bool isDigit(char c);
    bool isNumeric(char c);
    bool check(char c);
    int getValue();
    int calculateCombinations(int k, int n);
    int combinations();
    int factor();
    int product();
    int expression();
};

class WrongExpression {
public:
    std::string text;
    WrongExpression(const char* str);
};

#endif //HW4_PRINTER_H
