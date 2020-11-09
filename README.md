# Recursive-Descent-Parser

A recursive descent parser that accepts a source code file as a required command-line argument and displays an appropriate error message if the argument is not provided or the file does not exist. If a user input file contains multiple syntax errors, the program will find and report the first syntax error

Author: Mostofa Adib Shakib

BNF Grammer Rule

    P   ::=     S
    S   ::=     V:=E | read(V) | write(V) | while C do S od | S;S
    C   ::=     E < E | E > E | E = E | E <> E | E <= E | E >= E
    E   ::=     T | E + T | E - T
    T   ::=     F | T * F | T / F
    F   ::=     (E) | N | V
    V   ::=     a | b | … | y | z | aV | bV | … | yV | zV
    N   ::=     0 | 1 | … | 8 | 9 | 0N | 1N | … | 8N | 9N


Lexeme & Token Pair


    |      Lexeme      |     Token     |
    | ---------------  | ------------- |
    |       :=         |  ASSIGN_OP    |
    |       +          |  ADD_OP       |
    |       do         |  KEY_DO       |
    |       <          |  LESSER_OP    |
    |       -          |  SUB_OP       |
    |       od         |  KEY_OD       |
    |       >          |  GREATER_OP   |
    |       *          |  MULT_OP      |
    |   variable name  |  IDENT        |
    |       =          |  EQUAL_OP     |
    |       /          |  DIV_OP       |
    |     integer      |  INT_LIT      |
    |       <>         |  NEQUAL_OP    |
    |      read        |  KEY_READ     |
    |       (          |  LEFT_PAREN   |
    |       <=         |  LEQUAL_OP    |
    |     write        |  KEY_WRITE    |
    |       )          |  RIGHT_PAREN  |
    |      >=          |  GEQUAL_OP    |
    |     while        |  KEY_WHILE    |
    |       ;          |  SEMICOLON    |
