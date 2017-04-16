//
// Created by User on 08.04.2017.
//
#include <iostream>
#include <string>
#include <windows.h>

#include "printables.h"
//Some constants for printing___________________________________________________________________________________________
//______________________________________________________________________________________________________________________
char SYMBOLS[7][19][6] {{{"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"    "}, {"     "}, {"     "}, {"     "}},
                        {{"*****"}, {" **  "}, {"*****"}, {"*****"}, {"*   *"}, {"*****"}, {"*****"}, {"*****"}, {"*****"}, {"*****"}, {"     "}, {"     "}, {"     "}, {"    *"}, {"  *  "}, {" *  "}, {" ****"}, {"     "}, {"     "}},
                        {{"*   *"}, {"  *  "}, {"    *"}, {"    *"}, {"*   *"}, {"*    "}, {"*    "}, {"   * "}, {"*   *"}, {"*   *"}, {"  *  "}, {"     "}, {"*   *"}, {"   * "}, {" *   "}, {"  * "}, {"*    "}, {"     "}, {"*****"}},
                        {{"*   *"}, {"  *  "}, {"*****"}, {"*****"}, {"*****"}, {"*****"}, {"*****"}, {"  *  "}, {"*****"}, {"*****"}, {"*****"}, {"*****"}, {"  *  "}, {"  *  "}, {"*    "}, {"   *"}, {"*    "}, {"     "}, {"     "}},
                        {{"*   *"}, {"  *  "}, {"*    "}, {"    *"}, {"    *"}, {"    *"}, {"*   *"}, {"  *  "}, {"*   *"}, {"    *"}, {"  *  "}, {"     "}, {"*   *"}, {" *   "}, {" *   "}, {"  * "}, {"*    "}, {"     "}, {"*****"}},
                        {{"*****"}, {"*****"}, {"*****"}, {"*****"}, {"    *"}, {"*****"}, {"*****"}, {"  *  "}, {"*****"}, {"*****"}, {"     "}, {"     "}, {"     "}, {"*    "}, {"  *  "}, {" *  "}, {" ****"}, {"     "}, {"     "}},
                        {{"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"     "}, {"    "}, {"     "}, {"     "}, {"     "}}};

const int HEIGHT = 7;
const int WIDTH = 5;
int MAX_NESTING = 0;
std::string::iterator cur, end;

//Some useful functions_________________________________________________________________________________________________
//______________________________________________________________________________________________________________________
int nthOfTwo(int n) {
    int res = 1;
    while(n--) {
        res*=2;
    }
    return res;
}

bool check(char c) {
    if(cur < end && *cur == c) {
        ++cur;
        return true;
    }
    return false;
}

//Definition of Printable class_________________________________________________________________________________________
//______________________________________________________________________________________________________________________
int Printable::printLine(int line) {return 0;}

int Printable::calculateLength() {return 0;}

int Printable::getNumOfChar(char c) {
    switch(c) {
        case '+':
            return 10;
        case '-':
            return 11;
        case '*':
            return 12;
        case '/':
            return 13;
        case '(':
            return 14;
        case ')':
            return 15;
        case 'C':
            return 16;
        case ' ':
            return 17;
        case '=':
            return 18;
        default:
            return c - '0';
    }
}

int Printable::getLength() {return length;}


//Definition of class UsualSymbol_______________________________________________________________________________________
//______________________________________________________________________________________________________________________
UsualSymbol::UsualSymbol(char c, int n) {
    symbol = c;
    nesting = n;
    length = 1;
}

int UsualSymbol::printLine(int line) {
    for(int i = 0; i < WIDTH; ++i)
        for(int j = 0; j <= MAX_NESTING - nesting; ++j) {
            std::cout << SYMBOLS[line/ nthOfTwo(MAX_NESTING - nesting)][getNumOfChar(symbol)][i];
        }
    std::cout << " ";
    return length;
}

int UsualSymbol::calculateLength() {
    length = (MAX_NESTING - nesting + 1) * WIDTH + 1;
    return length;
}

//Definition of class ASCIIText_________________________________________________________________________________________
//______________________________________________________________________________________________________________________

//Public methods:_______________________________________________
ASCIIText::ASCIIText(int n) {
    nesting = n;
    length = 0;
}

ASCIIText::~ASCIIText() {
    for(Printable* x : content)
        delete x;
}

int ASCIIText::printLine(int line) {
    int printed = 0;
    for(Printable* x: content) {
        printed += x->printLine(line);
    }
    return printed;
}

int ASCIIText::calculateLength() {
    int l = 0;
    for(Printable* x : content) {
        l += x->calculateLength();
    }
    length = l;
    return length;
}

void ASCIIText::putPrintable(Printable* p)  {
    content.push_back(p);
}

int ASCIIText::getLength() {return length;}

void ASCIIText::printText(ASCIIText* text) {
    text->calculateLength();
    for(int i = 0; i < HEIGHT* nthOfTwo(MAX_NESTING - 1); ++i) {
        text->printLine(i);
        std::cout << std::endl;
    }
}

//Definition of class CombinationSymbol_________________________________________________________________________________
//______________________________________________________________________________________________________________________

//Public methods:_______________________________________________
CombinationSymbol::CombinationSymbol(int n, ASCIIText* a, ASCIIText* b) {
    nesting = n;
    upper = a;
    lower = b;
}

CombinationSymbol::~CombinationSymbol() {
    delete upper;
    delete lower;
}

int CombinationSymbol::printLine(int line) {
    for(int i = 0; i < WIDTH; ++i)
        for(int j = 0; j <= MAX_NESTING - nesting; ++j) {
            std::cout << SYMBOLS[line/ nthOfTwo(MAX_NESTING - nesting)][getNumOfChar('C')][i];
        }
    std::cout << " ";
    int printed = (MAX_NESTING - nesting + 1) * WIDTH + 1;
    if(line < HEIGHT* nthOfTwo(MAX_NESTING - nesting)/2) {
        printed += upper->printLine(line);
    } else {
        printed += lower->printLine(line - HEIGHT* nthOfTwo(MAX_NESTING - nesting)/2);
    }
    while(printed < length) {
        std::cout << ' ';
        ++printed;
    }
    return printed;
}

int CombinationSymbol::calculateLength() {
    int l = std::max(upper->calculateLength(), lower->calculateLength());
    length = l + (MAX_NESTING - nesting + 1) * WIDTH + 1;
    return length;
}

UsualSymbol* makeUsualSymbol(int n) {
    return new UsualSymbol(*(cur++), n);
}

CombinationSymbol* makeCombinationSymbol(int n) {
    ++cur;
    ASCIIText* upper = makeASCIIText(n+1);
    ASCIIText* lower = makeASCIIText(n+1);
    return new CombinationSymbol(n, upper, lower);
}

ASCIIText* makeASCIIText(int n) {
    MAX_NESTING = std::max(MAX_NESTING, n);
    ASCIIText* text = new ASCIIText(n);
    Printable* p;
    int balance = -1;
    while(cur < end) {
        if(check('c') || check('C')) {
            p = makeCombinationSymbol(n);
        } else if (check(',')) {
            break;
        } else if(*cur == '(') {
            --balance;
            p = makeUsualSymbol(n);
        } else if (*cur == ')') {
            ++balance;
            if(balance == 0){
                ++cur;
                return text;
            }
            p = makeUsualSymbol(n);
        } else {
            p = makeUsualSymbol(n);
        }
        text->putPrintable(p);
    }
    return text;
}

ASCIIText* createText(std::string::iterator b, std::string::iterator e) {
    MAX_NESTING = 0;
    cur = b, end = e;
    return makeASCIIText(1);
}