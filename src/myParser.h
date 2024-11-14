#ifndef MYPARSE_H
#define MYPARSE_H

typedef struct AstNode {
    struct AstNode** children;
    char* nodeName;
    int childrenCount;
} AstNode;

typedef struct ErrorInfo {
    char* message;
    int line;
    int column;
} ErrorInfo;

typedef struct {
    AstNode* tree;
    ErrorInfo** errors; 
    int errorCount;     
    int errorCapacity;  
} ParseResult;

ParseResult parseString(char* input);
void freeParseResult(ParseResult* result);

#endif // MYPARSE_H
