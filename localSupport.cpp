#include "localSupport.h"

int inMain = 0;         // Basically binary value for if we have been hit by the main keyword yet. If we have, it turns to 0
int numOfBlocks = 0;    // Number of blocks currently being accessed
int scopeLevel = 0;     // Level of current scope

// This function checks if a variable(token) has been declared yet
void Semantics::checkDec(Token tk){
    
    // Will start as not declared
    int isDec = -1;
    // Cycle through stack and see if variable matches a token
    for(unsigned int i = 0; i < st.size(); i++){
        if(st[i].id.val == tk.val && st[i].scope == scopeLevel){
            // If it does, set i to that number
            isDec = i;
            break;
        }
    }

    // If varible is not declared, then throw error and exit
    if(isDec > -1){
        cout << "SEMANTICS ERROR: Varable: " << tk.val << " on line " << tk.lineNum << " was already declared on line " << st[isDec].id.lineNum << endl;
        exit(EXIT_FAILURE);
    }else{
        // Otherwise put tk information ainto holder object and send to stack
        holder.id = tk;
        holder.scope = scopeLevel;
        st.push_back(holder);
        //cout << holder.id.val << "pushed onto stack in scope " << holder.scope << endl;
    }
}

// This function checks if a given token has been defined
void Semantics::checkDef(Token tk){
    // Starts as not defined
    int isDef = -1;
    //cout << tk.val << endl;

    // Cycle through stack and see if variable matches a token with same value, and has a scope level greater then the current
    for(unsigned int i = 0; i < st.size(); i++){
        if(st[i].id.val == tk.val && st[i].scope <= scopeLevel){
            //cout << "Current scope: " << scopeLevel << " Variable defined " << tk.val << " on scope level " << st[i].scope << endl;
            isDef = i;
            break;
        }
    }

    // If the variable hasnt been defined yet throw error
    if(isDef < 0){
        cout << "SEMANTICS ERROR: Variable: " << tk.val << " on line: " << tk.lineNum << " is not defined or declared on the stack yet" << endl;
        //cout << "Variable scope level: " << tk.val << " curent scope level is: " << scopeLevel << endl;
        exit(EXIT_FAILURE);
    }
}

// This function verifies node tokens recurrsevily
void Semantics::verify(Node* node){
    // If node is NULL then exit immeditly
    if(node == NULL){
        return;
    }

    //cout << "in " << node->name << endl;

    // If the node we are in is a block node
    if(node->name == "<block>"){
        inMain = 1;             // Set inMain to 1
        scopeLevel++;           // Increase scope level
        blockStack.push(0);     // Push this node onto block stack

        // Verifiy all possible nodes of this block node
        verify(node->n1);
        verify(node->n2);
        verify(node->n3);
        verify(node->n4);

        // If we are in main
        if(inMain == 1){
            // Pop off values in local stack equal to the amount of variables in that block
            for(unsigned int i = 0; i < blockStack.top(); i++){
                //cout << st.back().id.val << " popped off" << endl;
                st.pop_back();
            }
        }
        
        // If block stack size is greater then 0, pop off one of them since its not longer in use
        if(blockStack.size() > 0){
            blockStack.pop();
            scopeLevel--;
        }

        return;
    // If we are in a varse node
    }else if(node->name == "<vars>"){
        // For each token in this vars
        for(unsigned int i = 0; i < node->tokens.size(); i++){
            // Check if its an idTK that is not empty
            if(node->tokens[i].id == idTK && node->tokens[i].val != "EMPTY"){
                checkDec(node->tokens[i]);
            }
        }
    // All other tokens
    }else{
        vector<string> names = {"<R>", "<assign>", "<in>"};     // Nodes that will have Identity values
        // See if we are in one of these
        for(unsigned int i = 0; i < names.size(); i++){
            // If we are
            if(node->name == names[i]){
                // Cycle through the tokens in it and make sure they are defined
                for(unsigned int j = 0; j < node->tokens.size(); j++){
                    // If they are and identity token
                    if(node->tokens[j].id == idTK){
                        checkDef(node->tokens[j]);
                    }
                }
            }
        }
    }

     // Check all possible attached nodes
    verify(node->n1);
    verify(node->n2);
    verify(node->n3);
    verify(node->n4);

    //cout << "out " << node->name << endl;
}