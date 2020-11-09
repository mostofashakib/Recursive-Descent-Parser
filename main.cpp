
/*
  Author: Mostofa Adib Shakib
  Date: 11/02/2020
  Language: C++
  Description: This program takes in an executable file and an input file and validates the syntax of a source code file as defined by the BNF rule
*/

// Import the necessary header files

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;

// Global Variables

char currentChar;
FILE *myfile;
string previousToken;
string previousLexeme;
string nextToken;
string nextLexeme;

// Local function signatures

void adjustTokens(string prevToken, string prevLexeme, string nexToken, string nexLexeme);
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

    myfile = fopen (argv[1] , "r");  // opens a file and assigns it to the file myfile
    
    // checks if the file can be opened

    if ( myfile != NULL ) {
        cout << "DanC Parser :: R11545508" << endl;

        getChar();      // gets the initial character from the datastream

        while (  !feof(myfile) ) {
            lex();
            parseStatement();
        }

        fclose (myfile);    // After we have read all the lines from the text file we simply close the file reader
        cout << "Syntax Validated" << endl;
        exit(0);
    }
    // returns appropriate error if the file cannot be opened
    else {
      cout << "Unable to open file" << endl;
      exit(3);
    }
  }
  // returns appropriate error if two arguments isn't passed
  else {
    cout << "Not enough arguments passed" << endl;
    exit(2);
  }
}

// Utility functions

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

// A helper function that maps a lexeme with a token

void lex() {
	
    getNonBlank(); // gets the next non-space character

    // checks if the current character is a digit or not

    if ( isdigit(currentChar) ) {

        int digitNumber = 0;

        // for integers greater than 9

        while (isdigit(currentChar)) {
            int num = (int) currentChar - 48;
            digitNumber = digitNumber*10 + num;
            getChar();
        }

        adjustTokens(nextToken, nextLexeme, "INT_LIT", std::to_string(digitNumber));
    }

    // checks if the current character is an alphabet or not

    else if ( isalpha(currentChar) ) {
        string currentString = "";

        // for strings longer than 1 character

        while ( isalpha(currentChar) ) {
          currentString += currentChar;
          getChar();
        }

        // checks if all the characters in a string are lower cased or not

        if ( isAllLower(currentString) == false) {
            adjustTokens(nextToken, nextLexeme, "UNKNOWN", currentString);
            errorMessage();
        }

        // gets the index of the reserved keyword using the helper function getReservedKeywordIndex

        int reservedKeywordIndex = getReservedKeywordIndex(currentString);

        switch (reservedKeywordIndex) {
            case 0:
                adjustTokens(nextToken, nextLexeme, "KEY_READ", currentString);
                break;
            case 1:
                adjustTokens(nextToken, nextLexeme, "KEY_WRITE", currentString);
                break;
            case 2:
                adjustTokens(nextToken, nextLexeme, "KEY_WHILE", currentString);
                break;
            case 3:
                adjustTokens(nextToken, nextLexeme, "KEY_DO", currentString);
                break;
            case 4:
                adjustTokens(nextToken, nextLexeme, "KEY_OD", currentString);
                break;
            default:
                adjustTokens(nextToken, nextLexeme, "IDENT", currentString);
                break;
        }
    }

    // checks if current character is a special character or not

    else if ( isSpecial(currentChar) == true )  {
        char singleString = currentChar;
        string specialString = "";
        specialString += currentChar;
        getChar();
        specialString += currentChar;

        // checks if the special character is of length two or not

        if ( isTwoCharSpecial(specialString) == true ) {
            int specialStringIndex = getSpecialCharIndex(specialString);   // gets the index of the special character of length two
            getChar();

            switch (specialStringIndex) {
                case 0:
                    adjustTokens(nextToken, nextLexeme, "ASSIGN_OP", specialString);
                    break;
                case 1:
                    adjustTokens(nextToken, nextLexeme, "NEQUAL_OP", specialString);
                    break;
                case 2:
                    adjustTokens(nextToken, nextLexeme, "GEQUAL_OP", specialString);
                    break;
                case 3:
                    adjustTokens(nextToken, nextLexeme, "LEQUAL_OP", specialString);
                    break;
            }
        }

        // maps a special character of length one to it's appropriate token and lexeme

        else {
            switch (singleString) {
                case '<':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "LESSER_OP";
                    nextLexeme = singleString;
                    break;
                case '>':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "GREATER_OP";
                    nextLexeme = singleString;
                    break;
                case '=':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "EQUAL_OP";
                    nextLexeme = singleString;
                    break;
                case '+':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "ADD_OP";
                    nextLexeme = singleString;
                    break;
                case '-':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "SUB_OP";
                    nextLexeme = singleString;
                    break;
                case '*':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "MULT_OP";
                    nextLexeme = singleString;
                    break;
                case '/':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "DIV_OP";
                    nextLexeme = singleString;
                    break;
                case '(':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "LEFT_PAREN";
                    nextLexeme = singleString;
                    break;
                case ')':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "RIGHT_PAREN";
                    nextLexeme = singleString;
                    break;
                case ';':
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "SEMICOLON";
                    nextLexeme = singleString;
                    break;
                default:
                    previousToken = nextToken;
                    previousLexeme = nextLexeme;
                    nextToken = "UNKNOWN";
                    errorMessage();
            }
        }
    }

    else if ( feof(myfile) ) {
        // If we have reached the end of file and the last character is not in our grammar then error out
        if ( nextToken != "RIGHT_PAREN" && nextToken != "INT_LIT"  && nextToken != "IDENT" &&  nextToken != "KEY_OD") {
            cout << "EOF" <<endl;
            exit(1);
        }
    }

    else {
        errorMessage();
    }
}

// A helper function that gets the next non-space character from the input stream

void getNonBlank() {
    while ( isspace(currentChar) ) {
        getChar();
    }
}

// A helper function that gets the next character from the input stream

void getChar() {
    if ( currentChar != EOF ) {
        currentChar = getc(myfile);
    }
}

// A helper function for the production rule S

void parseStatement(){

    if ( nextToken == "IDENT" ) {
        lex();

        if ( nextToken == "ASSIGN_OP" ) {
            lex();
            parseExpression();
        }
        else {
            errorMessage();
        }
    }
    else if ( nextToken == "KEY_READ" ) {
        lex();

        if ( nextToken == "LEFT_PAREN" ) {
            lex();
            parseVariable();

            if ( nextToken == "RIGHT_PAREN" ) {
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
    else if ( nextToken == "KEY_WRITE" ) {
        lex();

        if ( nextToken == "LEFT_PAREN" ) {
            lex();
            parseVariable();

            if ( nextToken == "RIGHT_PAREN" ) {
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
    else if ( nextToken == "KEY_WHILE" ) {
        lex();
        parseConstant();

        if ( nextToken == "KEY_DO" ) {
            lex();
            parseStatement();

            if ( nextToken == "KEY_OD" ) {
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

    // for parseStatementPrime

    while(true) {
        if (nextToken == "SEMICOLON" ) {
            lex();
            parseStatement();
        }
        else {
            return;  // for epsilon
        }
    }
}

// A helper function for the production rule C

void parseConstant(){
    parseExpression();

    if (nextToken == "LESSER_OP" || nextToken == "GREATER_OP" || nextToken == "EQUAL_OP" || nextToken == "NEQUAL_OP" || nextToken == "LEQUAL_OP" || nextToken == "GEQUAL_OP" ){
        lex();
    }
    else {
        errorMessage();
    }

    parseExpression();
}

// A helper function for the production rule E

void parseExpression(){
    parseTerm();

    while (true) {
        if ( nextToken == "ADD_OP" || nextToken == "SUB_OP") {
            lex();
            parseTerm();
        }
        else {
            return; // for epsilon
        }
    }
}

// A helper function for the production rule F

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

        if ( nextToken == "RIGHT_PAREN" ) {
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

// A helper function for the production rule T

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
            return; // for epsilon
        }
    }
}

// A helper function for the production rule V

void parseVariable(){
    if ( nextToken == "IDENT" && currentChar != EOF && previousToken != nextToken ) {
        lex();
        parseVariable();
    }
}

// A helper function for the production rule N

void parseNumber(){
    if ( nextToken == "INT_LIT" && currentChar != EOF && previousToken != nextToken ) {
        lex();
        parseNumber();
    }
}

// A helper function to print the error message with the appropriate lexeme and token

void errorMessage(){
    cout << "Error encounter: The next lexeme was " << previousLexeme <<  " the next token was  " << previousToken << endl;
    exit(1);
}

// A helper function to update the previous as well as next token and lexeme values

void adjustTokens(string prevToken, string prevLexeme, string nexToken, string nexLexeme){
    previousToken = prevToken;
    previousLexeme = prevLexeme;
    nextToken = nexToken;
    nextLexeme = nextLexeme;
}