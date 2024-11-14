#ifndef MY_CONTROL_FLOW_GRAPH_H
#define MY_CONTROL_FLOW_GRAPH_H

#include "myParser.h"

// Типы узлов графа потока управления
typedef enum {
    CFG_NODE_BLOCK,
    CFG_NODE_STATEMENT,
    CFG_NODE_CONDITION,
    CFG_NODE_ENTRY,
    CFG_NODE_EXIT,
    CFG_NODE_BREAK
} CfgNodeType;

// Типы ребер графа потока управления
typedef enum {
    CFG_EDGE_NORMAL,
    CFG_EDGE_TRUE,
    CFG_EDGE_FALSE,
    CFG_EDGE_BREAK
} CfgEdgeType;

// Структура узла графа потока управления
typedef struct CfgNode {
    int id;                             // Уникальный идентификатор узла
    CfgNodeType type;                   // Тип узла
    AstNode* originalAstNode;           // Оригинальный узел AST
    char* label;                        // Метка/описание узла
    
    struct CfgNode** successors;        // Исходящие узлы
    struct CfgNode** predecessors;      // Входящие узлы
    int successorCount;
    int predecessorCount;
    int successorCapacity;
    int predecessorCapacity;
} CfgNode;

// Структура графа потока управления
typedef struct {
    CfgNode* entryNode;                 // Входной узел
    CfgNode* exitNode;                  // Выходной узел
    CfgNode** nodes;                    // Массив всех узлов
    int nodeCount;
    int nodeCapacity;

    // Поля для хранения информации о функции
    char* functionName;
    char* returnType;
    char** argumentNames;
    char** argumentTypes;
    int argumentCount;
} ControlFlowGraph;

// Структура для стека циклов
typedef struct CfgNodeStack{
    CfgNode* node;
    struct CfgNodeStack* next;
} CfgNodeStack;

// Функции для работы с узлами CFG
CfgNode* createCfgNode(ControlFlowGraph* cfg, CfgNodeType type, AstNode* astNode, const char* label);
void addSuccessor(CfgNode* from, CfgNode* to);
void addPredecessor(CfgNode* node, CfgNode* predecessor);

// Функции для работы с графом потока управления
ControlFlowGraph* createControlFlowGraph(AstNode* astRoot);
void destroyControlFlowGraph(ControlFlowGraph* cfg);

// Функции построения графа
CfgNode* buildCfgForFunctionBody(AstNode* funcDefNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForFuncSignature(AstNode* funcSignatureNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForBodySig(AstNode* bodySigNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForStatement(AstNode* statement, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForBlock(AstNode* blockNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForIf(AstNode* ifNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack);
CfgNode* buildCfgForWhile(AstNode* whileNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack);
CfgNode* buildCfgForDoWhile(AstNode* doWhileNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack);
CfgNode* buildCfgForBreak(AstNode* breakNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack);
// Утилиты
//void printControlFlowGraph(ControlFlowGraph* cfg);
void writeCfgAsDot(ControlFlowGraph* cfg, const char* filename);


// Работа с break

void push(CfgNodeStack** stack, CfgNode* node);
CfgNode* pop(CfgNodeStack** stack);
CfgNode* peek(CfgNodeStack* stack);
void destroyCfgNodeStack(CfgNodeStack* stack);
CfgNodeStack* createCfgNodeStack();

#endif // MY_CONTROL_FLOW_GRAPH_H
