#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;


// Global Variables

char currentChar;
FILE *myfile;
string nextToken;
string nextLexeme;

// Local function signatures

void getChar();
void getNonBlank();
void lex();
bool isSpecial();
bool isAllLower();
bool isTwoCharSpecial();
int getSpecialCharIndex();
int getReservedKeywordIndex();

void parseStatement();
void parseConstant();
void parseVariable();
void parseNumber();
void errorMessage();
void parseExpression();
void parseFactor();
void parseTerm();


int main(int argc, char ** argv) 
{

  if (argc == 2) {

    myfile = fopen (argv[1] , "r");
        
    if ( myfile != NULL ) {
      cout << "DanC Parser :: R11545508" << endl;

      getChar();

      while (  !feof(myfile) ) {
        lex();
        parseStatement();
    }

    fclose (myfile);    // After we have read all the lines from the text file we simply close the file reader
    }

    else {
      cout << "Unable to open file" << endl;
      exit(3);
    }
  }
  else {
    cout << "Not enough arguments passed" << endl;
    exit(2);
  }
}


// Utility function


// A helper function that checks to is if the current character is anyone of the special characters if not the function returns false

bool isSpecial(char character) 
{  
  if ( ':' == character || '<' == character || '>' == character || '=' == character || '+' == character || '-' == character || '*' == character  || '/' == character || '('  == character || ')' == character || ';' == character ) {
    return true;
  }
  return false;
}

// A helper function that checks if a given string is a special character of length 2 or not

bool isTwoCharSpecial(string word)
{
  if ( ":=" == word || "<>" == word || ">=" == word || "<=" == word ) {
    return true;
  }
  return false;
}

// A helper function that returns the index of a specific word

int getSpecialCharIndex(string word)
{
  if (word == ":=") {
    return 0;
  }
  else if (word == "<>") {
    return 1;
  }
  else if (word == ">=") {
    return 2;
  } 
  else {
    return 3;
  }
}

// a helper function that returns the index of a specific word or 100 if the word is not found

int getReservedKeywordIndex(string word)
{
  if (word == "read") {
    return 0;
  }
  else if (word == "write") {
    return 1;
  }
  else if (word == "while") {
    return 2;
  } 
  else if (word == "do") {
    return 3;
  } 
  else if (word == "od") {
    return 4;
  }
  return 100;
}


// A helper function that checks if all the characters of a string is lower cased if not the function returns false

bool isAllLower(string word)
{
  int length = word.length();

  for (int i = 0; i < length; i++){
    if (islower(word[i])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

void lex() {
    getNonBlank();

    if ( isdigit(currentChar) ) {
        int digitNumber = 0;

        while (isdigit(currentChar)) {
            int num = (int) currentChar - 48;
            digitNumber = digitNumber*10 + num;
            getChar();
        }

        nextToken = "INT_LIT";
        nextLexeme = std::to_string(digitNumber);
    }

    else if ( isalpha(currentChar) ) {
        string currentString = "";

        while ( isalpha(currentChar) ) {
          currentString += currentChar;
          getChar();
        }

        if ( isAllLower(currentString) == false) {
            nextToken = "UNKNOWN";
            errorMessage();
        }

        int reservedKeywordIndex = getReservedKeywordIndex(currentString);

        switch (reservedKeywordIndex) {
            case 0:
                nextToken = "KEY_READ";
                nextLexeme = currentString;
                break;
            case 1:
                nextToken = "KEY_WRITE";
                nextLexeme = currentString;
                break;
            case 2:
                nextToken = "KEY_WHILE";
                nextLexeme = currentString;
                break;
            case 3:
                nextToken = "KEY_DO";
                nextLexeme = currentString;
                break;
            case 4:
                nextToken = "KEY_OD";
                nextLexeme = currentString;
                break;
            default:
                nextToken = "IDENT";
                nextLexeme = currentString;
                break;
        }
    }

    else if ( isSpecial(currentChar) == true )  {
        char singleString = currentChar;
        string specialString = "";
        specialString += currentChar;
        getChar();
        specialString += currentChar;

        if ( isTwoCharSpecial(specialString) == true ) {
            int specialStringIndex = getSpecialCharIndex(specialString);
            getChar();

            switch (specialStringIndex) {
                case 0:
                    nextToken = "ASSIGN_OP";
                    nextLexeme = specialString;
                    break;
                case 1:
                    nextToken =  "NEQUAL_OP";
                    nextLexeme = specialString;
                    break;
                case 2:
                    nextToken = "GEQUAL_OP";
                    nextLexeme = specialString;
                    break;
                case 3:
                    nextToken = "LEQUAL_OP";
                    nextLexeme = specialString;
                    break;
            }
        }

        else {
            switch (singleString) {
                case '<':
                    nextToken = "LESSER_OP";
                    nextLexeme = singleString;
                    break;
                case '>':
                    nextToken = "GREATER_OP";
                    nextLexeme = singleString;
                    break;
                case '=':
                    nextToken = "EQUAL_OP";
                    nextLexeme = singleString;
                    break;
                case '+':
                    nextToken = "ADD_OP";
                    nextLexeme = singleString;
                    break;
                case '-':
                    nextToken = "LESSER_OP";
                    nextLexeme = singleString;
                    break;
                case '*':
                    nextToken = "MULT_OP";
                    nextLexeme = singleString;
                    break;
                case '/':
                    nextToken = "DIV_OP";
                    nextLexeme = singleString;
                    break;
                case '(':
                    nextToken = "LEFT_PAREN";
                    nextLexeme = singleString;
                    break;
                case ')':
                    nextToken = "RIGHT_PAREN";
                    nextLexeme = singleString;
                    break;
                case ';':
                    nextToken = "SEMICOLON";
                    nextLexeme = singleString;
                    break;
                default:
                    nextToken = "UNKNOWN";
                    errorMessage();
            }
        }
    }

    else if ( feof(myfile) ) {
        cout << "Syntax Validated" << endl;
        exit(0);
    }

    else {
        errorMessage();
    }
}

void getNonBlank() {
    while ( isspace(currentChar) ) {
        getChar();
    }
}

void getChar() {
    if ( currentChar != EOF ) {
        currentChar = getc(myfile);
    }
}


void parseStatement(){
    if ( nextToken == "IDENT" ) {
        parseVariable();

        if ( nextToken != "ASSIGN_OP" ) {
            errorMessage();
        }

        lex();
        parseExpression();


        if ( nextToken == "SEMICOLON"){
            lex();
            parseStatement();
        }
    }

    else if ( nextToken == "KEY_WRITE" ) {
        lex();

        if ( nextToken != "LEFT_PAREN" ) {
            errorMessage();
        }

        lex();
        parseVariable();

        if ( nextToken != "RIGHT_PAREN" ) {
            errorMessage();
        }

        lex();

        if ( nextToken == "SEMICOLON"){
            lex();
            parseStatement();
        }
    }

    else if ( nextToken == "KEY_READ" ) {
        lex();

        if ( nextToken != "LEFT_PAREN" ) {
            errorMessage();
        }

        lex();
        parseVariable();

        if ( nextToken != "RIGHT_PAREN" ) {
            errorMessage();
        }

        lex();

        if ( nextToken == "SEMICOLON"){
            lex();
            parseStatement();
        }
    }

    else if ( nextToken == "KEY_WHILE") {
        lex();
        parseConstant();

        if ( nextToken == "KEY_DO") {
            lex();
            parseStatement();

            if ( nextToken == "KEY_OD" ){
                lex();
            }
            else {
                errorMessage();
            }
        }
        else {
            errorMessage();
        }
    }
    else {
        errorMessage();
    }
}


void parseConstant(){
    if ( nextToken == "LEFT_PAREN" || nextToken == "IDENT" || nextToken == "INT_LIT" ){
        parseExpression();

        if (nextToken == "LESSER_OP" || nextToken == "GREATER_OP" || nextToken == "EQUAL_OP" || nextToken == "NEQUAL_OP" || nextToken == "LEQUAL_OP" || nextToken == "GEQUAL_OP" ){
            lex();
            parseExpression();
        }

        else {
            errorMessage();
        }
    }
}

void parseExpression(){
    parseTerm();

    while (true) {
        if ( nextToken == "ADD_OP" ) {
            lex();
            parseTerm();
        }
        else if (nextToken == "SUB_OP") {
            lex();
            parseTerm();
        }
        else {
            return;
        }
    }
}

void parseFactor() {
    if ( nextToken == "INT_LIT" ) {
        parseNumber();
    }

    else if ( nextToken == "IDENT" ){
        parseVariable();
    }

    else if ( nextToken == "LEFT_PAREN" ) {
        lex();
        parseExpression();
        lex();
    }
    else {
        errorMessage();
    }
}

void parseTerm() {
    parseFactor();

    while (true) {
        if ( nextToken == "MULT_OP") {
            lex();
            parseFactor();
        }
        else if (nextToken == "DIV_OP") {
            lex();
            parseFactor();
        }
        else {
            return;
        }
    }
}

void parseVariable(){
    if ( nextToken == "IDENT"  ) {
        lex();
        parseVariable();
    }
}

void parseNumber(){
    if ( nextToken == "INT_LIT" ) {
        lex();
        parseNumber();
    }
}

void errorMessage(){
    cout << "Error encounter: The next lexeme was " << nextLexeme <<  " the next token was  " << nextToken << endl;
    exit(1);
}