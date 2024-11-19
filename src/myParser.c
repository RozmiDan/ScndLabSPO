#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <antlr3.h>
#include "MyGrammarTestLexer.h"
#include "MyGrammarTestParser.h"
#include "myParser.h"

void freeAstNode(AstNode* node);

void addError(ParseResult* result, const char* message, int line, int position) {
    if (result->errorCount >= result->errorCapacity) {
        result->errorCapacity = result->errorCapacity > 0 ? result->errorCapacity * 2 : 4;
        result->errors = realloc(result->errors, result->errorCapacity * sizeof(ErrorInfo*));
    }
    ErrorInfo* error = (ErrorInfo*)malloc(sizeof(ErrorInfo));
    error->message = strdup(message);
    error->line = line;
    error->column = position;
    result->errors[result->errorCount++] = error;
}

AstNode* convertToAstNode(pANTLR3_BASE_TREE antlrTree) {
    if (antlrTree == NULL) {
        return NULL;
    }

    AstNode* node = (AstNode*)malloc(sizeof(AstNode));
    node->childrenCount = antlrTree->getChildCount(antlrTree);

    pANTLR3_STRING nodeText = antlrTree->toString(antlrTree);
    node->nodeName = strdup((char*)nodeText->chars);

    if (node->childrenCount > 0) {
        node->children = (AstNode**)malloc(node->childrenCount * sizeof(AstNode*));
        for (int i = 0; i < node->childrenCount; i++) {
            pANTLR3_BASE_TREE childTree = (pANTLR3_BASE_TREE)antlrTree->getChild(antlrTree, i);
            node->children[i] = convertToAstNode(childTree);
        }
    } else {
        node->children = NULL;
    }

    return node;
}

void myReportError(pANTLR3_BASE_RECOGNIZER recognizer) {
    pANTLR3_EXCEPTION ex = recognizer->state->exception;
    int line = ex->line;
    int position = ex->charPositionInLine;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Error at line %d, position %d: %s",
             line, position, (char*)ex->message);

    ParseResult* result = (ParseResult*)recognizer->state->userp;
    addError(result, buffer, line, position);
}

ParseResult* parseString(char* input) {
    ParseResult* result = (ParseResult*)malloc(sizeof(ParseResult)); 
    result->errorCapacity = 4;
    result->errors = (ErrorInfo**)malloc(result->errorCapacity * sizeof(ErrorInfo*));

    pANTLR3_INPUT_STREAM inputStream = antlr3FileStreamNew((pANTLR3_UINT8)input, ANTLR3_ENC_UTF8);
    pMyGrammarTestLexer lexer = MyGrammarTestLexerNew(inputStream);
    pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    pMyGrammarTestParser parser = MyGrammarTestParserNew(tokens);

    if (parser == NULL) {
        addError(result, "Failed to initialize parser.", 0, 0);
        return result;
    }

    parser->pParser->rec->state->userp = result;
    parser->pParser->rec->reportError = myReportError;

    MyGrammarTestParser_source_return parseResult = parser->source(parser);

    if (parser->pParser->rec->state->errorCount > 0) {
        char buffer[256];
        sprintf(buffer, "Parsing failed with %d errors.", parser->pParser->rec->state->errorCount);
        addError(result, buffer, 0, 0);
    }

    if (parseResult.tree == NULL) {
        addError(result, "Parsing failed, no tree generated.", 0, 0);
    } else {
        result->tree = convertToAstNode((pANTLR3_BASE_TREE)parseResult.tree);
    }

    tokens->free(tokens);
    lexer->free(lexer);
    inputStream->close(inputStream);

    return result;
}

void freeParseResult(ParseResult* result) {
    if (result->tree != NULL) {
        freeAstNode(result->tree);
    }

    for (int i = 0; i < result->errorCount; i++) {
        free(result->errors[i]->message);
        free(result->errors[i]);
    }
    free(result->errors);
    free(result);
}

void freeAstNode(AstNode* node) {
    if (node == NULL) {
        return;
    }

    if (node->nodeName != NULL) {
        free(node->nodeName);
    }

    for (int i = 0; i < node->childrenCount; i++) {
        freeAstNode(node->children[i]);
    }

    free(node->children);
    free(node);
}

int writeAstAsDot(ParseResult* result, char* filename){
    FILE* dotFile = fopen(filename, "w");
    if (dotFile != NULL) {
        fprintf(dotFile, "digraph ParseTree {\n");
        fprintf(dotFile, "  node [shape=box];\n");
        int nodeCounter = 0;
        writeTreeAsDot(result->tree, dotFile, &nodeCounter); 
        fprintf(dotFile, "}\n");
        fclose(dotFile);
        return 1;
    } else {
        return 0;
    }
}

void writeTreeAsDot(AstNode* node, FILE* file, int* nodeCounter) {
    if (node == NULL) {
        return;
    }

    int currentNodeId = (*nodeCounter)++;
    fprintf(file, "  node%d [label=\"%s\"];\n", currentNodeId, node->nodeName);
    
    for (int i = 0; i < node->childrenCount; i++) {
        int childNodeId = *nodeCounter;
        writeTreeAsDot(node->children[i], file, nodeCounter);
        fprintf(file, "  node%d -> node%d;\n", currentNodeId, childNodeId);
    }
}