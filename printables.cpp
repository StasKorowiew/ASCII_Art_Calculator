//
// Created by User on 08.04.2017.
//

#include <iostream>
#include <string>
#include <windows.h>

#include "printables.h"

using namespace printables;

//Some constants for printing___________________________________________________________________________________________
//______________________________________________________________________________________________________________________
char SYMBOLS[9][19][7] {{{"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"     "}, {"      "}, {"      "}, {"      "}},
                        {{"***** "}, {" **   "}, {"***** "}, {"***** "}, {"*   * "}, {"***** "}, {"***** "}, {"***** "}, {"***** "}, {"***** "}, {"      "}, {"      "}, {"      "}, {"      "}, {"  *   "}, {" *   "}, {"  *** "}, {"      "}, {"      "}},
                        {{"*   * "}, {"  *   "}, {"    * "}, {"    * "}, {"*   * "}, {"*     "}, {"*     "}, {"    * "}, {"*   * "}, {"*   * "}, {"  *   "}, {"      "}, {"*   * "}, {"    * "}, {" *    "}, {"  *  "}, {" *    "}, {"      "}, {"      "}},
                        {{"*   * "}, {"  *   "}, {"    * "}, {"    * "}, {"*   * "}, {"*     "}, {"*     "}, {"   *  "}, {"*   * "}, {"*   * "}, {"  *   "}, {"      "}, {" * *  "}, {"   *  "}, {"*     "}, {"   * "}, {"*     "}, {"      "}, {"***** "}},
                        {{"*   * "}, {"  *   "}, {"***** "}, {"***** "}, {"***** "}, {"***** "}, {"***** "}, {"  *   "}, {"***** "}, {"***** "}, {"***** "}, {"***** "}, {"  *   "}, {"  *   "}, {"*     "}, {"   * "}, {"*     "}, {"      "}, {"      "}},
                        {{"*   * "}, {"  *   "}, {"*     "}, {"    * "}, {"    * "}, {"    * "}, {"*   * "}, {"  *   "}, {"*   * "}, {"    * "}, {"  *   "}, {"      "}, {" * *  "}, {" *    "}, {"*     "}, {"   * "}, {"*     "}, {"      "}, {"***** "}},
                        {{"*   * "}, {"  *   "}, {"*     "}, {"    * "}, {"    * "}, {"    * "}, {"*   * "}, {"  *   "}, {"*   * "}, {"    * "}, {"  *   "}, {"      "}, {"*   * "}, {"*     "}, {" *    "}, {"  *  "}, {" *    "}, {"      "}, {"      "}},
                        {{"***** "}, {"***** "}, {"***** "}, {"***** "}, {"    * "}, {"***** "}, {"***** "}, {"  *   "}, {"***** "}, {"***** "}, {"      "}, {"      "}, {"      "}, {"      "}, {"  *   "}, {" *   "}, {"  *** "}, {"      "}, {"      "}},
                        {{"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"      "}, {"     "}, {"      "}, {"      "}, {"      "}}};

const int HEIGHT = 9;
const int WIDTH = 6;
int SCALE = 4;
int MAX_NESTING = 0;
std::string::iterator cur, end;
std::fstream out;

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
int Printable::printLine(int line, PrintMode mode) {return 0;}

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

int UsualSymbol::printLine(int line, PrintMode mode) {
    for(int i = 0; i < WIDTH; ++i) {
        for(int j = 0; j < nthOfTwo(MAX_NESTING - nesting); ++j) {
            char ch = SYMBOLS[line / nthOfTwo(MAX_NESTING - nesting)][getNumOfChar(symbol)][i];
            switch (mode) {
                case CONSOLE:
                    putToConsole(ch);
                    break;
                case FILE:
                    if (ch == '*')
                        ch = (char) 255;
                    else
                        ch = '\0';
                    putToFile(ch);
                    break;
            }
        }
    }
    return length;
}

int UsualSymbol::calculateLength() {
    length = nthOfTwo(MAX_NESTING - nesting) * WIDTH;
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

int ASCIIText::printLine(int line, PrintMode mode) {
    int printed = 0;
    for(Printable* x: content) {
        printed += x->printLine(line, mode);
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

void ASCIIText::printText(ASCIIText *text, PrintMode mode) {

    text->calculateLength();
    switch (mode) {
        case CONSOLE:
            for (int i = 0; i < HEIGHT * nthOfTwo(MAX_NESTING - 1); ++i) {
                text->printLine(i, CONSOLE);
                std::cout << std::endl;
            };
            break;
        case FILE:
            out.open("result.bmp", std::ios::out | std::ios::binary);

            BITMAPFILEHEADER bmfh;
            BITMAPINFOHEADER bmif;

            bmfh.bfType = 19778;
            bmfh.bfSize = (DWORD) 54 + text->length * HEIGHT * nthOfTwo(MAX_NESTING - 1) * 2 * SCALE * SCALE;
            bmfh.bfReserved1 = 0;
            bmfh.bfReserved2 = 0;
            bmfh.bfOffBits = 54;

            bmif.biSize = 40;
            bmif.biWidth = text->length * SCALE;
            bmif.biHeight = HEIGHT * nthOfTwo(MAX_NESTING - 1) * SCALE;
            bmif.biPlanes = 1;
            bmif.biBitCount = 16;
            bmif.biCompression = 0;
            bmif.biSizeImage = 0;
            bmif.biXPelsPerMeter = 0;
            bmif.biYPelsPerMeter = 0;
            bmif.biClrUsed = 0;
            bmif.biClrImportant = 0;

            out.write((char*)&bmfh, sizeof(bmfh));
            out.write((char*)&bmif, sizeof(bmif));

            for(int i = HEIGHT * nthOfTwo(MAX_NESTING - 1) * SCALE - 1; i >= 0; --i) {
                text->printLine(i / SCALE, mode);
            }

            out.close();
    };
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

int CombinationSymbol::printLine(int line, PrintMode mode) {
    for(int i = 0; i < WIDTH; ++i)
        for(int j = 0; j < nthOfTwo(MAX_NESTING - nesting); ++j) {
            char ch = SYMBOLS[line/ nthOfTwo(MAX_NESTING - nesting)][getNumOfChar('C')][i];
            switch (mode) {
                case CONSOLE:
                    putToConsole(ch);
                    break;
                case FILE:
                    if (ch == '*')
                        ch = (char) 255;
                    else
                        ch = '\0';
                    putToFile(ch);
                    break;
            }
        }
    int printed = nthOfTwo(MAX_NESTING - nesting) * WIDTH;
    if(line < HEIGHT* nthOfTwo(MAX_NESTING - nesting)/2) {
        printed += upper->printLine(line, mode);
    } else {
        printed += lower->printLine(line - HEIGHT* nthOfTwo(MAX_NESTING - nesting)/2, mode);
    }
    while(printed < length) {
        switch (mode) {
            case CONSOLE:
                std::cout << ' ';
                break;
            case FILE:
                putToFile('\0');
                break;
        }
        ++printed;
    }
    return printed;
}

int CombinationSymbol::calculateLength() {
    int l = std::max(upper->calculateLength(), lower->calculateLength());
    length = l + nthOfTwo(MAX_NESTING - nesting) * WIDTH;
    return length;
}

UsualSymbol* printables::makeUsualSymbol(int n) {
    return new UsualSymbol(*(cur++), n);
}

CombinationSymbol* printables::makeCombinationSymbol(int n) {
    ++cur;
    ASCIIText* upper = printables::makeASCIIText(n+1);
    ASCIIText* lower = printables::makeASCIIText(n+1);
    return new CombinationSymbol(n, upper, lower);
}

ASCIIText* printables::makeASCIIText(int n) {
    MAX_NESTING = std::max(MAX_NESTING, n);
    ASCIIText* text = new ASCIIText(n);
    Printable* p;
    int balance = -1;
    while(cur < end) {
        if(check('c') || check('C')) {
            p = printables::makeCombinationSymbol(n);
        } else if (check(',')) {
            break;
        } else if(*cur == '(') {
            --balance;
            p = printables::makeUsualSymbol(n);
        } else if (*cur == ')') {
            ++balance;
            if(balance == 0){
                ++cur;
                return text;
            }
            p = printables::makeUsualSymbol(n);
        } else {
            p = printables::makeUsualSymbol(n);
        }
        text->putPrintable(p);
    }
    return text;
}

ASCIIText* printables::createText(std::string::iterator b, std::string::iterator e) {
    MAX_NESTING = 0;
    cur = b, end = e;
    return printables::makeASCIIText(1);
}

void printables::putToConsole(char c) {
    std::cout << c;
}

void printables::putToFile(char c) {
    for(int k = 0; k < SCALE; ++k) {
        out.put(c);
        out.put(c);
    }
}