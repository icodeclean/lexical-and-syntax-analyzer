/* main.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char goodLexemes[1000][100];
char nextChar;
int lexLen;
int goodLexemesLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int getNextToken(void);
void error(char ch);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define DOT 3

/* Token codes */
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define IF_CODE 31
#define ELSE_CODE 32
#define FOR_CODE 30
#define WHILE_CODE 33
#define ELSE_CODE 32
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37
#define LEFT_BRACE 40
#define RIGHT_BRACE 41


/******************************************************/
/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	getNextToken();
	 } while (nextToken != EOF);
	 }
	 /* Print list of proper tokens */
	for(int i = 0; i < 1000; i+=100)
      	printf("%s ", *goodLexemes+i);
	 return 0;
}
/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		else if (nextChar == '.')
		 	charClass = DOT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

/******************************************************/
/* getNextToken - a simple lexical analyzer for arithmetic
 expressions */
int getNextToken(void) {
	 lexLen = 0;
	 getNonBlank();
	 switch (charClass) {
		/* Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 if (strcmp(lexeme,"for") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = FOR_CODE;
			 break;
			 }
			if (strcmp(lexeme,"if") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = IF_CODE;
			 break;
			 }
			if (strcmp(lexeme,"else") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = ELSE_CODE;
			 break;
			 }
			if (strcmp(lexeme,"while") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = WHILE_CODE;
			 break;
			 }
			if (strcmp(lexeme,"do") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = DO_CODE;
			 break;
			 }
			if (strcmp(lexeme,"switch") == 0) {
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = SWITCH_CODE;
			 break;
			 }
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = IDENT;
			 break;
		/* Integer literals */
		 case DIGIT:
			 addChar();
			 getChar();
			 while (charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			if (charClass == DOT) {
				addChar();
				getChar();
				while (charClass == DIGIT) {
				 	addChar();
				 	getChar();
			 	}
			 	strcpy(goodLexemes[goodLexemesLen++],lexeme);
				nextToken = FLOAT_CODE;
				break;
			}
			strcpy(goodLexemes[goodLexemesLen++],lexeme);
			nextToken = INT_CODE;
		 	break;
		/* Parentheses and operators */
		case UNKNOWN:
			 lookup(nextChar);
			 getChar();
			 break;
		/* DOT */
		case DOT:
		    printf("Error: . by itself is an error!");
    		exit(0);

		/* EOF */
		case EOF:
			 strcpy(goodLexemes[goodLexemesLen++],lexeme);
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
	 printf("Next token is: %d, Next lexeme is %s\n",
	 nextToken, lexeme);
	 return nextToken;
} /* End of function getNextToken */

void error(char ch) { 
    printf("Expected token % is missing\n", ch);
    exit(0);
}

/* if_statement
 Parses strings in the language generated by the rule:
 <if_statement> -> if '(' <boolexpr> ')' <statement>
 [else <statement>]
 */
void if_statement(void) {
    if (nextToken != IF_CODE)
        error(IF_CODE);
    else {
        getNextToken();
        if (nextToken != LEFT_PAREN)
            error(LEFT_PAREN);
        else {
            getNextToken();
            boolexpr();
            if (nextToken != RIGHT_PAREN)
                error(RIGHT_PAREN);
            else {
                getNextToken();
                statement();
                if (nextToken == ELSE_CODE) {
                    getNextToken();
                    statement();
                }
            }
        }
    }
}


/* while_statement
 Parses strings in the language generated by the rule:
 <while_statement> -> while '(' <boolexpr> ')' <statement>

 */
void while_statement(void) {
    if (nextToken != WHILE_CODE)
 	    error(WHILE_CODE);
    else {
        getNextToken();
        if (nextToken != LEFT_PAREN)
 		    error(LEFT_PAREN);
 	    else {
 		    getNextToken();
 		    boolexpr();
 		    if (nextToken != RIGHT_PAREN)
 			    error(RIGHT_PAREN);
 		    else {
                getNextToken();
		 	statement();
            }
        }
    }
}


/* for_statement
 Parses strings in the language generated by the rule:
 <for_statement> -> for '(' [expr] ';' [<boolexpr>] ';' [expr] ')' <statement>

 */
void for_statement(void) {
    if (nextToken != FOR_CODE)
 	    error(FOR_CODE);
    else {
        getNextToken();
        if (nextToken != LEFT_PAREN)
 		    error(LEFT_PAREN);
 	    else {
 		    getNextToken();
            expr();
            if (nextToken != SEMI_COLON)
 		        error(SEMI_COLON);
            else {
                getNextToken();
                boolexpr();
                if (nextToken != SEMI_COLON)
 		            error(SEMI_COLON);
                else {
                    getNextToken();
                    expr();
                    if (nextToken != RIGHT_PAREN)
 			            error(RIGHT_PAREN);
 		            else {
                        getNextToken();
		 	            statement();
                    }
                }
            }
        }
    }
}

/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */
void expression(void) {
	 printf("Enter <expression>\n");
	/* Parse the first term */
	 term();
	/* As long as the next token is + or -, get
	 the next token and parse the next term */
	 while (nextToken == ADD_OP || nextToken == SUB_OP) {
		 getNextToken();
		 term();
	 }
	 printf("Exit <expression>\n");
}

/* term
 Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | / | %) <factor>}
 */
void term(void) {
	 printf("Enter <term>\n");
	/* Parse the first factor */
	 factor();
	/* As long as the next token is * or / or %, get the
	 next token and parse the next factor */
	 while (nextToken == MULT_OP || nextToken == DIV_OP) {
		 getNextToken();
		 factor();
	 }
	 printf("Exit <term>\n");
}

/* factor
 Parses strings in the language generated by the rule:
 <factor> -> ident | int | ( <expr> )
 */
void factor(void) {
	 printf("Enter <factor>\n");
	/* Determine which RHS */
	 if (nextToken == IDENT || nextToken == INT_CODE)
		/* Get the next token */
		 getNextToken();
		/* If the RHS is ( <expr> ), call lex to pass over the
		 left parenthesis, call expr, and check for the right
		 parenthesis */
	 else { if (nextToken == LEFT_PAREN) {
		 getNextToken();
		 expr();
		 if (nextToken == RIGHT_PAREN)
		 	getNextToken();
		 else
		 	error(RIGHT_PAREN);
		 }
		/* It was not an id, an integer literal, or a left
		 parenthesis */
		 else
		 error(UNKNOWN);
	 }
	 printf("Exit <factor>\n");
}

