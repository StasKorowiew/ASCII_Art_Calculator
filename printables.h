//
// Created by User on 08.04.2017.
//
#include <fstream>
#include <vector>

#ifndef HW4_1_PRINTABLES_H
#define HW4_1_PRINTABLES_H

enum PrintMode {
    CONSOLE, BMP
};

class Printable {
public:
    virtual int printLine(int line, PrintMode mode);

    virtual int calculateLength();

    int getLength();

protected:
    int nesting;
    int length;

    static int getNumOfChar(char c);
};

class UsualSymbol : public Printable {
public:
    UsualSymbol(char c, int n);

    int printLine(int line, PrintMode mode);

    int calculateLength();

private:
    char symbol;
};

class ASCIIText {
public:
    ASCIIText(int n);

    ~ASCIIText();

    int printLine(int line, PrintMode mode);

    int calculateLength();

    void putPrintable(Printable *p);

    int getLength();

    static void printText(ASCIIText *text, PrintMode mode);

private:
    int nesting;
    int length;
    std::vector<Printable *> content;
};


class CombinationSymbol : public Printable {
public:
    CombinationSymbol(int n, ASCIIText *a, ASCIIText *b);

    ~CombinationSymbol();

    int printLine(int line, PrintMode mode);

    int calculateLength();

private:
    ASCIIText *upper, *lower;
};

UsualSymbol* makeUsualSymbol(int n);

CombinationSymbol* makeCombinationSymbol(int nesting);

ASCIIText* makeASCIIText(int nesting);

ASCIIText* createText(std::string::iterator cur, std::string::iterator end);

void putToConsole(char c);

void putToFile(char c);

#endif //HW4_1_PRINTABLES_H
