//
// Created by User on 06.04.2017.
//
#include <cstdio>
#include <iostream>

#include "Printer.h"

//Variables for calculating number of combinations______________________________________________________________________
//______________________________________________________________________________________________________________________
std::vector<std::vector<int>> C_N_K;
int CURRENT_N = -1;

//Definition of class Printer___________________________________________________________________________________________
//______________________________________________________________________________________________________________________

//Public methods:_______________________________________________
Printer::Printer() {};

Printer::Printer(std::string str) {putStr(str);}

void Printer::putChar(char c) {text.push_back(c);}

void Printer::putInt(int n) {char num[20]; sprintf(num, "%d", n); putStr(num);}

void Printer::putStr(const char *str) {
    for(int i = 0; str[i]; ++i)
        if(str[i] != ' ')
            text.push_back(str[i]);
}

void Printer::putStr(std::string str) {
    for(char c : str)
        if(c != ' ')
            text.push_back(c);
}

void Printer::calculateAndPrint() {
    int result;
    try {
        result = parse();
    } catch(WrongExpression exc) {
        std::cout << exc.text << std::endl;
        clear();
        return;
    }

    putChar('=');
    putInt(result);

    printables::ASCIIText* toPrint = printables::createText(text.begin(), text.end());
    printables::ASCIIText::printText(toPrint, printables::CONSOLE);
    printables::ASCIIText::printText(toPrint, printables::FILE);
    clear();
    delete toPrint;
}

void Printer::clear() {text.clear();}

//Private methods:______________________________________________
int Printer::parse() {
    if(text.empty()) throw WrongExpression("Empty string");
    cur = text.begin();
    end  = text.end();
    int result = expression();
    if(cur != end) {
        throw WrongExpression("Parsing error: incorrect input");
    }
    return result;
}

bool Printer::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool Printer::isNumeric(char c) {
    return isDigit(c) || c == '+' || c == '-';
}

bool Printer::check(char c) {
    if(cur < end && *cur == c) {
        ++cur;
        return true;
    }
    return false;
}

int Printer::getValue() {
    char num[20];
    char *c = num;
    if(*cur == '-' || *cur == '+')
        *(c++) = *(cur++);
    if(!isDigit(*cur)) throw WrongExpression("Parsing error: incorrect input");
    while(isDigit(*cur)) {
        *(c++) = *(cur++);
    }
    *c = '\0';
    int res = 0;
    sscanf(num, "%d", &res);
    return  res;
}

int Printer::calculateCombinations(int k, int n) {
    if(n > CURRENT_N) {
        try{
            CURRENT_N = n;
            C_N_K.resize(n+1);
            for(std::vector<int>& x : C_N_K)
                x.resize(n+1);
            C_N_K[0][0] = 1;
            for(int i = 1; i <= n; ++i) {
                C_N_K[0][i] = 1;
                C_N_K[i][0] = 0;
            }
            for(int i = 1; i <= n; ++i)
                for(int j = 1; j <= n; ++j) {
                    C_N_K[j][i] = C_N_K[j][i-1] + C_N_K[j-1][i-1];
                }
        } catch(std::bad_alloc e) {
            C_N_K.resize(0);
            CURRENT_N = -1;
            throw WrongExpression("Calculation error: too large numbers for combinations");
        }
    }
    return C_N_K[k][n];
}

int Printer::combinations() {
    if(check('(')) {
        int k = expression();
        if(!check(',')) throw WrongExpression("Parsing error: incorrect input");
        int n = expression();
        if(!check(')')) throw WrongExpression("Parsing error: incorrect input");
        if(k < 0 || n < 0 || n < k)
            throw WrongExpression("Calculating error: incorrect values for combinations");
        return calculateCombinations(k, n);
    } else throw WrongExpression("Parsing error: incorrect input");
}

int Printer::factor() {
    if(isNumeric(*cur))
        return getValue();
    if(check('C') || check('c'))
        return combinations();
    if(check('(')) {
        int result = expression();
        if(!check(')'))
            throw WrongExpression("Parsing error: wrong parentheses order");
        return result;
    }
    throw WrongExpression("Parsing error: incorrect input");
}

int Printer::product() {
    int left = factor();
    while(cur < end) {
        if(check('*') || *cur == '(') {
            left = left * factor();
        }
        else if(check('/')) {
            int right = factor();
            if(right == 0) throw WrongExpression("Calculating error: division by zero");
            left = left / right;
        } else return left;
    }
    return left;
}

int Printer::expression() {
    int left = product();
    while(cur < end) {
        if(check('+')) {
            left = left + product();
        } else if(check('-')) {
            left = left - product();
        } else return left;
    }
    return left;
}

//Definition of class WrongExpression___________________________________________________________________________________
//______________________________________________________________________________________________________________________
WrongExpression::WrongExpression(const char *str) {text = str;}