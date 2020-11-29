#include "localSupport.h"

int inMain = 0;
int numOfBlocks = 0;

void Semantics::checkDec(Token tk){
    int isDec = -1;
    if(inMain == 0){
        for(unsigned int i = 0; i < globalSt.size(); i++){
            if(globalSt[i].id.val == tk.val){
                isDec = i;
                break;
            }
        }

        if(isDec > -1){
            cout << "SEMANTICS ERROR: Varable: " << tk.val << " on line " << tk.lineNum << " was already declared on line " << globalSt[isDec].id.lineNum << endl;
            exit(EXIT_FAILURE);
        }else{
            holder.id = tk;
            globalSt.push_back(holder);
            cout << holder.id.val << "pushed onto global stack" << endl;
        }
    }else{
        for(unsigned int i = 0; i < localSt.size(); i++){
            if(localSt[i].id.val == tk.val){
                isDec = i;
                break;
            }
        }

        if(isDec > -1){
            cout << "SEMANTICS ERROR: Varable: " << tk.val << " on line " << tk.lineNum << " was already declared on line " << localSt[isDec].id.lineNum << endl;
            exit(EXIT_FAILURE);
        }else{
            holder.id = tk;
            localSt.push_back(holder);
            cout << holder.id.val << "pushed onto local stack" << endl;
            blockStack.top()++;
        }
    }
}

void Semantics::checkDef(Token tk){
    int isDef = -1;

    for(unsigned int i = 0; i < globalSt.size(); i++){
        if(globalSt[i].id.val == tk.val){
            isDef = i;
            break;
        }
    }

    for(unsigned int i = 0; i < localSt.size(); i++){
        if(localSt[i].id.val == tk.val){
            isDef = i;
            break;
        }
    }

    if(isDef < 0){
        cout << "SEMANTICS ERROR: Variable: " << tk.val << " on line: " << tk.lineNum << " is not defined or declared on the stack yet" << endl;
        exit(EXIT_FAILURE);
    }
}

void Semantics::verify(Node* node){
    if(node == NULL){
        return;
    }

    cout << "in " << node->name << endl;

    if(node->name == "<block>"){
        inMain = 1;
        blockStack.push(0);
        verify(node->n1);
        verify(node->n2);
        verify(node->n3);
        verify(node->n4);
        if(inMain == 1){
            for(unsigned int i = 0; i < blockStack.top(); i++){
                cout << localSt.back().id.val << " popped off" << endl;
                localSt.pop_back();
            }
        }
        if(blockStack.size() > 0){
            blockStack.pop();
        }

        return;
    }else if(node->name == "<vars>"){
        for(unsigned int i = 0; i < node->tokens.size(); i++){
            if(node->tokens[i].id == idTK && node->tokens[i].val != "EMPTY"){
                checkDec(node->tokens[i]);
            }else{
                cout << node->tokens[i].val << endl;
            }
        }
    }else{
        vector<string> names = {"<R>", "<assign>"};
        for(unsigned int i = 0; i < names.size(); i++){
            if(node->name == names[i]){
                for(unsigned int j = 0; j < node->tokens.size(); j++){
                    if(node->tokens[j].id == idTK){
                        checkDef(node->tokens[j]);
                    }
                }
            }
        }
    }

    verify(node->n1);
    verify(node->n2);
    verify(node->n3);
    verify(node->n4);

    cout << "out " << node->name << endl;
}