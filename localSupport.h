#ifndef LOCALSUPPORT_H
#define LOCALSUPPORT_H
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "node.h"
#include "token.h"
using namespace std;

typedef struct symbolTable{
    Token id;
}SymbolTable;

class Semantics{
    private:
        SymbolTable holder;
        vector<SymbolTable>globalSt;
        vector<SymbolTable>localSt;

        stack <int>blockStack;

        void checkDec(Token tk);
        void checkDef(Token tk);
    
    public:
        void verify(Node *node);
};

#endif