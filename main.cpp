#include <iostream>
#include <string>
#include <cmath>
#include <cctype>

#include "token.h"

using namespace std;


struct memoryInfo {
    string name;
    string type;
    string svalue;
    int ivalue;
};


//
// alreadyExists
// 
// Given int value of ProgramSize, variableName, and an array of memory memoryInfo structure, 
// searches for the variableName in the memory array structure inside the name element 
// and returns the index when found. If not found, returns -1. 
//
int alreadyExists(string variableName, memoryInfo memory[], int ProgramSize){ // check if variable already exists and return the index
    for (int i = 0; i < ProgramSize; ++i){
        if(memory[i].name == variableName){
            return i;
        }
    }
    
    return -1;  
}


//
// print
// 
// Given int value of ProgramSize, pIndex(printIndex value), an array of memory memoryInfo structure, 
// and an array of program Token structure, loops through the line and accesses the variable values from memory when needed
// to output either string, integer or space. No return type.
//
void print(Token program[], int ProgramSize, memoryInfo memory[], int pIndex){
    int pInitialLine = program[pIndex].Line;
    pIndex += 2; // moves index to point at the element after open parantheses
    
    while(program[pIndex].ID != TokenID::RIGHT_PAREN && program[pIndex].Line == pInitialLine){ 
       if(program[pIndex].ID == TokenID::IDENTIFIER && program[pIndex].Value != "print"){
           int memIndexExist = alreadyExists(program[pIndex].Value, memory, ProgramSize);
           
           if (memIndexExist != -1){
               if (memory[memIndexExist].svalue == ""){
                   cout << memory[memIndexExist].ivalue;
               } else {
                   cout << memory[memIndexExist].svalue;
               }
           }
           
       } else if (program[pIndex].ID == TokenID::STR_LITERAL){
           cout << program[pIndex].Value;
           
       } else if (program[pIndex].ID == TokenID::COMMA){
           cout << " ";
           
       } else if (program[pIndex].ID == TokenID::INT_LITERAL){
           cout << program[pIndex].Value;
       }
       pIndex += 1;
    }
    
    cout << endl;
}


//
// input
// 
// Given int value of index and an array of program Token structure, prints the command
// to console if there is any after left paranthesis and uses getline command to get input from user. 
// returns the input that user entered.
//
string input(Token program[], int index){
    index += 2; //refers to the first element or ')' right parantheses
    string line = "";
    
    if(program[index].ID != TokenID::RIGHT_PAREN){ // exactly one parameter
        cout << program[index].Value;
        getline(cin, line);
    } else { // no parameter
        getline(cin, line);
    }
    
    return line;
}


//
// printErrorInputFunction
// 
// Given int value of varExist and index, string literal(variable name), and an array of program Token structure,
// prints the error statement to console with error of cannot convert string literal to int. There is a different error 
// statement if there is a variable that contains string. No return type.
//
void printErrorInputFunction(Token program[], string literal, int varExist, int index){
    if(varExist == 1){
        cout << "**Error: cannot convert " << program[index].Value << " containing '" << literal << "' @ (" 
            << program[index].Line << ", " << program[index].Col << ") to 'int'" << endl;
    } else { // varExist = 0
        cout << "**Error: cannot convert string '" << literal << "' @ (" 
            << program[index].Line << ", " << program[index].Col << ") to 'int'" << endl;
    }
}


//
// integerConversion
// 
// Given int index and ProgramSize, an array of program Token structure, and an array of memoryInfo memory structure,
// converts int literal to integer and a variable that holds integer or string type with numbers to integer.
// Function call is prompted if one of the characters in string is not a number. Returns the result integer.
//
int integerConversion(Token program[], memoryInfo memory[], int ProgramSize, int index){
    index += 2; // refers to the parameter 
    int result = 0;
    int varExist = 0; // initially no variable exist in parameter
    
    if(program[index].ID == TokenID::INT_LITERAL){
        result = stoi(program[index].Value);
        
    } else if (program[index].ID == TokenID::STR_LITERAL){
        int totalChars = (program[index].Value).length();
        string sliteral = program[index].Value;
        
        for (int z = 0; z < totalChars; ++z){
            if (!isdigit(sliteral[z])){ // one of the characters is not integer
               printErrorInputFunction(program, sliteral, varExist, index);
               return result;
            }
        }
        result = stoi(program[index].Value);
    } else if (program[index].ID == TokenID::IDENTIFIER){
        int varParameter = alreadyExists(program[index].Value, memory, ProgramSize);
        int varExist = 1;
        
        if(memory[varParameter].type == "int"){
            result = memory[varParameter].ivalue;
            
        } else if(memory[varParameter].type == "str") {
            int totalChars = (memory[varParameter].svalue).length();
            string sliteral = memory[varParameter].svalue;
            
            for (int z = 0; z < totalChars; ++z){
                if (!isdigit(sliteral[z])){ // one of the characters is not integer
                   printErrorInputFunction(program, sliteral, varExist, index);
                   return result;
                }
            }
            result = stoi(memory[varParameter].svalue);
        }
    }
    
    return result;
}


//
// stringConversion
// 
// Given int index and ProgramSize, an array of program Token structure, and an array of memoryInfo memory structure,
// converts int literal, string literal, or a variable that holds string or integer to a string in result variable.
// Returns the result string.
//
string stringConversion(Token program[], memoryInfo memory[], int ProgramSize, int index){
    index += 2; // refers to the parameter 
    string result = "";
    
    if(program[index].ID == TokenID::INT_LITERAL){
        result = program[index].Value;
        
    } else if (program[index].ID == TokenID::STR_LITERAL){
        result = program[index].Value;
        
    } else if (program[index].ID == TokenID::IDENTIFIER){
        int varParameter = alreadyExists(program[index].Value, memory, ProgramSize);
        if(memory[varParameter].type == "int"){
            result = to_string(memory[varParameter].ivalue);
        } else if(memory[varParameter].type == "str") {
            result = memory[varParameter].svalue;
        }
    }
    
    return result;
}


//
// type
// 
// Given int index and ProgramSize, an array of program Token structure, and an array of memoryInfo memory structure,
// returns the type through string result variable that holds either "str" or "int" value. If it's int literal or variable
// that holds integer, then it stores "int" in result. If it's str literal or variable that holds string, it stores "str" in result.
//
string type(Token program[], memoryInfo memory[], int ProgramSize, int index){
    index += 2; // refers to the parameter 
    string result = "";
    
    if(program[index].ID == TokenID::INT_LITERAL){
        result = "int";
        
    } else if (program[index].ID == TokenID::STR_LITERAL){
        result = "str";
        
    } else if (program[index].ID == TokenID::IDENTIFIER){
        int varParameter = alreadyExists(program[index].Value, memory, ProgramSize);
        if(memory[varParameter].type == "int"){
            result = "int";
            
        } else if(memory[varParameter].type == "str") {
            result = "str";
        }
    }
    
    return result;
}


//
// printErrorBinaryExpressions
// 
// Given int operandIndex, an array of program Token structure, and string type1 and type 2, 
// prints the error statement to console for combining string and integer through an operation. No return type.
//
void printErrorBinaryExpressions(Token program[], int operandIndex, string type1, string type2){
    cout << "**Error: type error with " << "'" << program[operandIndex].Value << "' @ (" << program[operandIndex].Line << ", " 
        << program[operandIndex].Col << "), expr is " << type1 << " + " << type2 << endl;
}


//
// addingVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// adds same type literals or variable and stores in either ivalue or svalue of memory array based on the type. 
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void addingVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue + memory[secondVariable].ivalue;
               memory[memoryIndex].svalue = "";
               memory[memoryIndex].type = "int";
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // both string
               memory[memoryIndex].svalue = memory[firstVariable].svalue + memory[secondVariable].svalue;
               memory[memoryIndex].type = "str";
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue + stoi(program[secondElement].Value);
               memory[memoryIndex].svalue = "";
               memory[memoryIndex].type = "int";
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // both string
               memory[memoryIndex].svalue = memory[firstVariable].svalue + program[secondElement].Value;
               memory[memoryIndex].type = "str";
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) + memory[secondVariable].ivalue;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = "";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // both string
               memory[memoryIndex].svalue = program[i].Value + memory[secondVariable].svalue;
               memory[memoryIndex].type = "str";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) + stoi(program[secondElement].Value);
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = "";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // both string
               memory[memoryIndex].svalue = program[i].Value + program[secondElement].Value;
               memory[memoryIndex].type = "str";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = ""; // empty svalue since it gave error
           }
       }
    }
}


//
// subtractingVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// subtracts same type(integer) literals or variable and stores in ivalue of memory array.
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void subtractingVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = ""; // sets an empty string for the string value
    memory[memoryIndex].type = "int"; // setting the type as int
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue - memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue - stoi(program[secondElement].Value);
               memory[memoryIndex].type = "int";
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) - memory[secondVariable].ivalue;
               memory[memoryIndex].type = "int";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) - stoi(program[secondElement].Value);
               memory[memoryIndex].type = "int";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// multiplyingVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// multiplies same type(integer) literals or variable and stores in ivalue of memory array.
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void multiplyingVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int"; // setting the type as int
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue * memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue * stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) * memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) * stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// dividingVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// divides same type(integer) literals or variable and stores in ivalue of memory array.
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void dividingVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               if(memory[secondVariable].ivalue == 0){ // division by zero 
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = memory[firstVariable].ivalue / memory[secondVariable].ivalue;
               }
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               if(stoi(program[secondElement].Value) == 0){ // division by zero
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = memory[firstVariable].ivalue / stoi(program[secondElement].Value);
               }
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               if (memory[secondVariable].ivalue == 0){ // division by zero
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = stoi(program[i].Value) / memory[secondVariable].ivalue;
               }
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               if(stoi(program[secondElement].Value) == 0){ // division by zero
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = stoi(program[i].Value) / stoi(program[secondElement].Value);
               }
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// powerVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// raises same type(integer) literals or variable to a power and stores in ivalue of memory array.
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void powerVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = pow(memory[firstVariable].ivalue, memory[secondVariable].ivalue);
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = pow(memory[firstVariable].ivalue, stoi(program[secondElement].Value));
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = pow(stoi(program[i].Value), memory[secondVariable].ivalue);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = pow(stoi(program[i].Value), stoi(program[secondElement].Value));
               memory[memoryIndex].type = "int";
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// modulusVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// gives modulus of same type(integer) literals or variable and stores in ivalue of memory array.
// It retrieves the value from memory array if it exists. If they are different types, it calls printErrorBinaryExpressions.
// No return type.
//
void modulusVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               if(memory[secondVariable].ivalue == 0){ // modulus by zero 
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = memory[firstVariable].ivalue % memory[secondVariable].ivalue;
               }
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               if(stoi(program[secondElement].Value) == 0){ // modulus by zero
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = memory[firstVariable].ivalue % stoi(program[secondElement].Value);
               }
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               if (memory[secondVariable].ivalue == 0){ // modulus by zero
                   memory[memoryIndex].ivalue = 0;
               } else {
                   memory[memoryIndex].ivalue = stoi(program[i].Value) % memory[secondVariable].ivalue;
               }
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               if(stoi(program[secondElement].Value) == 0){ // modulus by zero
                   memory[memoryIndex].ivalue = 0; 
               } else {
                   memory[memoryIndex].ivalue = stoi(program[i].Value) % stoi(program[secondElement].Value);
               }
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semnatic error: both string
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'str'");
               memory[memoryIndex].ivalue = 0;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic error: string and int
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semnatic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// lessThanVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is less than right hand side(rhs) else 0
// in ivalue memory array based on the type. It retrieves the value from memory array if it exists. 
// If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void lessThanVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue < memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue < memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue < stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue < program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) < memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value < memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) < stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value < program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// lessThanEqualVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is less than or equal to right hand side(rhs) else 0
// in ivalue memory array based on the type. It retrieves the value from memory array if it exists. 
// If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void lessThanEqualVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue <= memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue <= memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue <= stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue <= program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) <= memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value <= memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) <= stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value <= program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// greaterThanVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is greater than right hand side(rhs) else 0
// in ivalue memory array based on the type. It retrieves the value from memory array if it exists. 
// If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void greaterThanVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue > memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue > memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue > stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue > program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) > memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value > memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) > stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value > program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// greaterThanEqualVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is greater than or equal 
// to right hand side(rhs) else 0 in ivalue memory array based on the type. It retrieves the value from 
// memory array if it exists. If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void greaterThanEqualVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue >= memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue >= memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue >= stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue >= program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) >= memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value >= memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) >= stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value >= program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// equalEqualVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is equal to right hand side(rhs) else 0
// in ivalue memory array based on the type. It retrieves the value from memory array if it exists. 
// If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void equalEqualVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue == memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue == memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue == stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue == program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) == memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value == memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) == stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value == program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// notEqualVars
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and memoryIndex and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is not equal to right hand side(rhs) else 0
// in ivalue memory array based on the type. It retrieves the value from memory array if it exists. 
// If they are different types, it calls printErrorBinaryExpressions. No return type.
//
void notEqualVars(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i){
    memory[memoryIndex].svalue = "";
    memory[memoryIndex].type = "int";
    
    int secondElement = i + 2; 
    int operandIndex = i + 1; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue != memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue != memory[secondVariable].svalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "int" ){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = memory[firstVariable].ivalue != stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = memory[firstVariable].svalue != program[secondElement].Value;
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) != memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value != memory[secondVariable].svalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "int"){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               memory[memoryIndex].ivalue = stoi(program[i].Value) != stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               memory[memoryIndex].ivalue = program[i].Value != program[secondElement].Value;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // semantic checking.. calling error function
               printErrorBinaryExpressions(program, operandIndex, "'str'", "'int'");
               memory[memoryIndex].ivalue = 0;
               
           } else { // semantic error: first type is int and second type is string
               printErrorBinaryExpressions(program, operandIndex, "'int'", "'str'");
               memory[memoryIndex].ivalue = 0;
           }
       }
    }
}


//
// lessThanExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is less than right hand side(rhs) else 0
// in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int lessThanExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue < memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue < memory[secondVariable].svalue;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue < stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue < program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) < memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value < memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) < stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value < program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// lessThanEqualExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is less than or equal to right hand side(rhs)
//  else 0 in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int lessThanEqualExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue <= memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue <= memory[secondVariable].svalue;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue <= stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue <= program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) <= memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value <= memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) <= stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value <= program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// greaterThanExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is greater than right hand side(rhs)
//  else 0 in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int greaterThanExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue > memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue > memory[secondVariable].svalue;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue > stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue > program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) > memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value > memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) > stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value > program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// greaterThanEqualExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is greater than or equal to right hand side(rhs)
//  else 0 in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int greaterThanEqualExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue >= memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue >= memory[secondVariable].svalue;
           }
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue >= stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue >= program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) >= memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value >= memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) >= stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value >= program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// equalEqualExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is equal to right hand side(rhs)
//  else 0 in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int equalEqualExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2;
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue == memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue == memory[secondVariable].svalue;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue == stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue == program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) == memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value == memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) == stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value == program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// notEqualExpression
// 
// Given an array of Token program structure, memoryInfo memory, int ProgramSize and i,
// compares same type literals or variable and stores 1 if left hand side(lhs) is not equal to right hand side(rhs)
//  else 0 in result. It retrieves the value from memory array if it exists. Returns result variable.
//
int notEqualExpression(Token program[], memoryInfo memory[], int ProgramSize, int i){
    int result = 0;
    int secondElement = i + 2; 
    
    if (program[i].ID == TokenID::IDENTIFIER){ // checking if first element after = is variable
       int  firstVariable = alreadyExists(program[i].Value, memory, ProgramSize); 
        
       if(program[secondElement].ID == TokenID::IDENTIFIER){ // checking if element after operation is also variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (memory[firstVariable].type == "int" && memory[secondVariable].type == "int"){ // both int
               result = memory[firstVariable].ivalue != memory[secondVariable].ivalue;
               
           } else if (memory[firstVariable].type == "str" && memory[secondVariable].type == "str"){ // semantic error: both string
               result = memory[firstVariable].svalue != memory[secondVariable].svalue;
           }
           
       } else { // the second element is not variable
           if (memory[firstVariable].type == "int" && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = memory[firstVariable].ivalue != stoi(program[secondElement].Value);
               
           } else if (memory[firstVariable].type == "str" && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = memory[firstVariable].svalue != program[secondElement].Value;
           }
       }
        
    } else { // meaning first element is not variable 
       if(program[secondElement].ID == TokenID::IDENTIFIER){ //second element is variable
           int secondVariable = alreadyExists(program[secondElement].Value, memory, ProgramSize);
           
           if (program[i].ID == TokenID::INT_LITERAL && memory[secondVariable].type == "int"){ // both int
               result = stoi(program[i].Value) != memory[secondVariable].ivalue;
               
           } else if (program[i].ID == TokenID::STR_LITERAL && memory[secondVariable].type == "str"){ // semantic error: both string
               result = program[i].Value != memory[secondVariable].svalue;
           }
           
       } else { // first and second element is not variable 
           if (program[i].ID == TokenID::INT_LITERAL && program[secondElement].ID == TokenID::INT_LITERAL){ // both int
               result = stoi(program[i].Value) != stoi(program[secondElement].Value);
               
           } else if (program[i].ID == TokenID::STR_LITERAL && program[secondElement].ID == TokenID::STR_LITERAL){ // semantic error: both string
               result = program[i].Value != program[secondElement].Value;
           }
       }
    }
    
    return result;
}


//
// evaluateOneTermExpression
// 
// Given int value of ProgramSize and index, memoryIndex, an array of memory memoryInfo structure, 
// and an array of program Token structure, checks if it's a int literal is 1 or variable that holds integer 1
// and stores in result variable. result is initially set as 0.
// Returns result variable.
//
int evaluateOneTermExpression(Token program[], memoryInfo memory[], int ProgramSize, int index){
    int result = 0;
    
    if(program[index].ID == TokenID::INT_LITERAL){
        if(stoi(program[index].Value) == 1){
            result = 1;
        }
        
    } else if (program[index].ID == TokenID::IDENTIFIER){
        int variableIndex = alreadyExists(program[index].Value, memory, ProgramSize);
        
        if(memory[variableIndex].ivalue == 1){
            result = 1;
        }
    }
    
    return result;
}


//
// storingVariables
// 
// Given int value of ProgramSize, i(index value), memoryIndex, an array of memory memoryInfo structure, 
// and an array of program Token structure, finds the type for the variable and stores it in memory. 
// Performs integer and string operations and stores the value by calling the different operation functions. 
// No return type.
//
void storingVariables(Token program[], memoryInfo memory[], int ProgramSize, int memoryIndex, int i) {
    int firstLine = program[i].Line; // current line
    i += 2; // now refers to the first element after equal sign '='
    int firstElement = i; 
    
    i += 1; 
    int op = i; // refers to the operand and serves as test for statements that are like "i = 2"
    
    if (program[firstElement].Value == "input"){
        memory[memoryIndex].svalue = input(program, firstElement); // returns a string
        memory[memoryIndex].type = "str";
        
    } else if (program[firstElement].Value == "int"){
        memory[memoryIndex].ivalue = integerConversion(program, memory, ProgramSize, firstElement);
        memory[memoryIndex].svalue = "";
        memory[memoryIndex].type = "int";
        
    } else if (program[firstElement].Value == "str"){
        memory[memoryIndex].svalue = stringConversion(program, memory, ProgramSize, firstElement);
        memory[memoryIndex].type = "str";
        
    } else if (program[firstElement].Value == "type"){
        memory[memoryIndex].svalue = type(program, memory, ProgramSize, firstElement);
        memory[memoryIndex].type = "str";
    }
    
    if (program[op].Line != firstLine){ // checking to see if operand is part of the line, first type of assignment
        if (program[firstElement].ID == TokenID::INT_LITERAL){ // first element is int literal
               memory[memoryIndex].type = "int";
               memory[memoryIndex].svalue = "";
               memory[memoryIndex].ivalue = stoi(program[firstElement].Value);
            
        } else if (program[firstElement].ID == TokenID::IDENTIFIER && program[firstElement].Value != "print") { // first element is variable
            if (alreadyExists(program[firstElement].Value, memory, ProgramSize) != -1){ // checks if this statement is accessing old variable in memory
                int varLocation = alreadyExists(program[firstElement].Value, memory, ProgramSize);
                
                if(memory[varLocation].type == "int"){ // storing the approriate string or int value in memory array
                    memory[memoryIndex].ivalue = memory[varLocation].ivalue;
                    memory[memoryIndex].type = memory[varLocation].type;
                    memory[memoryIndex].svalue = "";
                } else if (memory[varLocation].type == "str"){
                    memory[memoryIndex].svalue = memory[varLocation].svalue;
                    memory[memoryIndex].type = memory[varLocation].type;
                }
            }
            
        } else if (program[firstElement].ID == TokenID::STR_LITERAL){
            memory[memoryIndex].type = "str";
            memory[memoryIndex].svalue = program[firstElement].Value;
        }
        
    } else {
        if (program[op].ID == TokenID::PLUS){ // add for int and string
           addingVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::MINUS){ // minus
           subtractingVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::MULT){ // MULT
           multiplyingVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::POWER){ // POWER
           powerVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::DIV){ // DIV
           dividingVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::MOD){ // MOD
           modulusVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::LT){ // LT
           lessThanVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::LTE){ // LTE
           lessThanEqualVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::GT){ // GT
           greaterThanVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::GTE){ // GTE
           greaterThanEqualVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::EQUAL_EQUAL){ // EQUAL_EQUAL
           equalEqualVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        } else if (program[op].ID == TokenID::NOT_EQUAL){ // NOT_EQUAL
           notEqualVars(program, memory, ProgramSize, memoryIndex, firstElement);
            
        }
        i += 2; // completes the line
    }
}


//
// conditionChecker
// 
// Given int of ProgramSize and refrence variable index, an array of memory memoryInfo structure, 
// and an array of program Token structure, checks if the condition is true(1) or false(0) by calling 
// different expression functions for different operations.
// Returns ifConditionValid variable.
//
int conditionChecker(Token program[], memoryInfo memory[], int ProgramSize, int& index){
    index += 1; // points the first element
    int firstElement = index;
    
    index += 1;
    int op = index; // points to either operation or colon
    
    int ifConditionValid = 0; // 0 refers to false condition and 1 refers to true
    
    if(program[op].ID == TokenID::COLON){ // only one term expression
        ifConditionValid = evaluateOneTermExpression(program, memory, ProgramSize, firstElement);
        index += 1;
        
    } else if (program[op].ID == TokenID::LT){
        ifConditionValid = lessThanExpression(program, memory, ProgramSize, firstElement);
        index += 3;
        
    } else if (program[op].ID == TokenID::LTE){
        ifConditionValid = lessThanEqualExpression(program, memory, ProgramSize, firstElement);
        index += 3;
        
    }  else if (program[op].ID == TokenID::GT){
        ifConditionValid = greaterThanExpression(program, memory, ProgramSize, firstElement);
        index += 3;
        
    } else if (program[op].ID == TokenID::GTE){
        ifConditionValid = greaterThanEqualExpression(program, memory, ProgramSize, firstElement);
        index += 3;
        
    } else if (program[op].ID == TokenID::EQUAL_EQUAL){
        ifConditionValid = equalEqualExpression(program, memory, ProgramSize, firstElement);
        index += 3;
        
    }  else if (program[op].ID == TokenID::NOT_EQUAL){
        ifConditionValid = notEqualExpression(program, memory, ProgramSize, firstElement);
        index += 3;
    }
    
    return ifConditionValid;
    
}


//
// executeTrueCondition
// 
// Given int of ProgramSize, refrence variable index and refrence variable memIndex, an array of memory memoryInfo structure, 
// and an array of program Token structure, executes the commands from indent to undent by calling the storingVariables 
// function and passing in memindex and index.
// No return type
//
void executeTrueCondition(Token program[], memoryInfo memory[], int ProgramSize, int& index, int& memIndex){
    if(program[index].ID == TokenID::INDENT){ // only care about when it refers to indent
        index += 1; // now refers to the identifer, which is after indent
        
        while(program[index].ID != TokenID::UNDENT){
            if(program[index].ID == TokenID::IDENTIFIER){
                if (program[index].Value == "print"){
                    print(program, ProgramSize, memory, index);
                    
                } else { // now refers to storing variables/ function calls
                    int isEqual = index + 1; // refers to equal sign 
                    
                    if(program[isEqual].ID == TokenID::EQUAL){ // so this confirms it's an assignment
                        if (alreadyExists(program[index].Value, memory, ProgramSize) != -1){ // variable exists in memory
                            int existingVarLocation = alreadyExists(program[index].Value, memory, ProgramSize);
                            storingVariables(program, memory, ProgramSize, existingVarLocation, index);
                            
                        } else { // new variable
                            memIndex += 1; // adding 1 to memIndex counter.. reference variable
                            memory[memIndex].name = program[index].Value;
                            storingVariables(program, memory, ProgramSize, memIndex, index);
                        }
                    }
                }
            }
            index += 1;// moves to the next token until undent
        }
    }
}


//
// executeElseTrueCondition
// 
// Given int of ProgramSize, refrence variable index and refrence variable memIndex, an array of memory memoryInfo structure, 
// and an array of program Token structure, executes the else block commands from indent to undent 
// by calling the storingVariables function and passing in memindex and index.
// No return type
//
void executeElseTrueCondition(Token program[], memoryInfo memory[], int ProgramSize, int& index, int& memIndex){
    index += 2; // refers to the indent token
    
    if(program[index].ID == TokenID::INDENT){ // only care about when it refers to indent
        index += 1; // now refers to the identifer, which is after indent
        
        while(program[index].ID != TokenID::UNDENT){
            if(program[index].ID == TokenID::IDENTIFIER){
                if (program[index].Value == "print"){
                    print(program, ProgramSize, memory, index);
                    
                } else { // now refers to storing variables/ function calls
                    int isEqual = index + 1; // refers to equal sign 
                    if(program[isEqual].ID == TokenID::EQUAL){ // so this confirms it's an assignment
                        if (alreadyExists(program[index].Value, memory, ProgramSize) != -1){ // variable exists in memory
                            int existingVarLocation = alreadyExists(program[index].Value, memory, ProgramSize);
                            storingVariables(program, memory, ProgramSize, existingVarLocation, index);
                            
                        } else { // new variable
                            memIndex += 1; // adding 1 to memIndex counter.. reference variable
                            memory[memIndex].name = program[index].Value;
                            storingVariables(program, memory, ProgramSize, memIndex, index);
                        }
                    }
                }
            }
            index += 1;// moves to the next token until undent
        }
    }
}


//
// skipInsideIndentUndent
// 
// Given int of refrence variable index and an array of program Token structure, skips all the commands by incrementing
// index by 1 til it reaches undent token through use of while loop. No return type
//
void skipInsideIndentUndent(Token program[], int& index){
    while(program[index].ID != TokenID::UNDENT){
        index += 1;
    }
}


//
// skipElseIfElse
// 
// Given int of refrence variable index and an array of program Token structure, skips all the commands by incrementing
// index by 1 til it reaches an identifier token through use of while loop. Calls the function skipInsideIndentUndent to 
// skip commands in the elif and else blocks. No return type
//
void skipElseIfElse(Token program[], int& index){
    while (program[index].ID == TokenID::UNDENT){
        int isIdentifier = index + 1;
        
        if(program[isIdentifier].ID == TokenID::IDENTIFIER){
            break; // index refers to undent
            
        } else {
            index += 1;
            
            if(program[index].ID == TokenID::ELIF_KEYW){
                int op = index + 2;
                
                if(program[op].ID == TokenID::COLON){
                    index += 3;
                    if(program[index].ID == TokenID::INDENT){
                        skipInsideIndentUndent(program, index); // updates index to undent
                    }
                    
                } else { // it's operation
                    index += 5;
                    if(program[index].ID == TokenID::INDENT){
                        skipInsideIndentUndent(program, index); // updates index to undent
                    }
                }
                
             } else if (program[index].ID == TokenID::ELSE_KEYW){
                index += 2;// refers to indent now
                if(program[index].ID == TokenID::INDENT){
                    skipInsideIndentUndent(program, index); // updates index to undent
                }
            }
        }
    }
}


//
// memoryAccess
// 
// Given int value of ProgramSize, an array of memory memoryInfo structure, and an array of program Token structure, 
// looks for identifier token and the print value in program array strucure and prints the line out by calling the print function.
// Else, calls storingVariables function with the appropriate memory index location for either updating old variable in 
// memory or for creating new variables in memory array. If not identifier, looks for if, elif, and else to call the appropriate
// function to check if condition is true to execute the statements and skip elif and else blocks. No return type.
//
void memoryAccess(Token program[], memoryInfo memory[], int ProgramSize) {
    int mIndex = -1; //keeping track of memory index and serves as a counter
    
    for (int i = 0; i < ProgramSize; ++i){
       if (program[i].ID == TokenID::IDENTIFIER){
           if (program[i].Value == "print"){
               print(program, ProgramSize, memory, i);
           }
           
           else { // so it's a variable
               int testingI = i + 1; //testing index to see if this variable is inside the print function
               if (program[testingI].ID == TokenID::EQUAL){ // proves that this is a variable statement 
                   if (alreadyExists(program[i].Value, memory, ProgramSize) != -1){ // for updating an old variable in memory
                       int memoryLocation = alreadyExists(program[i].Value, memory, ProgramSize);
                       storingVariables(program, memory, ProgramSize, memoryLocation, i);
                       
                   } else { // for new variables
                       mIndex += 1; // so it starts memory index 0 and increments by 1 when next variable is found
                       memory[mIndex].name = program[i].Value;
                       storingVariables(program, memory, ProgramSize, mIndex, i);
                   }
                   
               } else { // variable was inside the print function like print(x)
                   continue; // next iteration
               }
         } 
           
      } else if(program[i].ID == TokenID::INDENT){
           skipInsideIndentUndent(program, i);
           
      } else if(program[i].ID == TokenID::IF_KEYW){
           int ifValid = conditionChecker(program, memory, ProgramSize, i); // return 0 or 1 and updates i to indent token
           if(ifValid == 1){
               executeTrueCondition(program, memory, ProgramSize, i, mIndex);
               skipElseIfElse(program, i); // i now refers to undent before identifier.. skipped and else and else if
               
           } else if (ifValid == 0){ // if cond. not true
               skipInsideIndentUndent(program, i); // i refers to the undent
           }
           
      } else if (program[i].ID == TokenID::ELIF_KEYW){
           int elifValid = conditionChecker(program, memory, ProgramSize, i); // 0 or 1  and i refers to indent token
           if (elifValid == 1){
               executeTrueCondition(program, memory, ProgramSize, i, mIndex); // updates i to undent token
               skipElseIfElse(program, i); // i refers to undent before identifier.. skipped all else and else if
           } else if (elifValid == 0){
               skipInsideIndentUndent(program, i); // i refers to the undent
           }
           
      } else if (program[i].ID == TokenID::ELSE_KEYW){
           executeElseTrueCondition(program, memory, ProgramSize, i, mIndex);
      }
   }
}


// ##################################################################################
//
// main filename memorysize
//
int main(int argc, char* argv[])
{
   
   // 
   // 1. process command-line args:
   //
   // argv[0]: ./main
   // argv[1]: filename.py
   // argv[2]: memorysize
   //
   if (argc != 3)
   {
      cout << "usage: ./main filename.py memorysize" << endl;
      return 0;
   }

   string filename(argv[1]);  // this is your SimplePy program file
//    int MemorySize = stoi(argv[2]);  // this is size for Memory array
   
   //
   // TODO: call inputSimplePy to input the SimplePy program, and
   // you'll get back an array of Tokens.  Allocate another array
   // for your memory, and then start executing!
   //
    
   int ProgramSize;
   
   Token* program = inputSimplePy(filename, ProgramSize);
   
   if (program == nullptr) {
       cout << "**Error, unable to open program file '" << filename << "'" << endl;
       return 0;
   }
   
   memoryInfo* memory = new memoryInfo[ProgramSize];
   
   memoryAccess(program, memory, ProgramSize);
   
   delete[] program;
   delete[] memory;
   return 0;
}
