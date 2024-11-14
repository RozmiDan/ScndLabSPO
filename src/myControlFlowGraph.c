#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "myControlFlowGraph.h"

CfgNode* createCfgNode(ControlFlowGraph* cfg, CfgNodeType type, AstNode* astNode, const char* label) {
    CfgNode* node = malloc(sizeof(CfgNode));
    node->id = cfg->nodeCount; 
    node->type = type;
    node->originalAstNode = astNode;
    node->label = label ? strdup(label) : NULL;
    
    node->successors = NULL;
    node->predecessors = NULL;
    node->successorCount = 0;
    node->predecessorCount = 0;
    node->successorCapacity = 0;
    node->predecessorCapacity = 0;
    
    //new
    if (cfg->nodeCount >= cfg->nodeCapacity) {
        cfg->nodeCapacity = cfg->nodeCapacity ? cfg->nodeCapacity * 2 : 4;
        cfg->nodes = realloc(cfg->nodes, sizeof(CfgNode*) * cfg->nodeCapacity);
    }
    cfg->nodes[cfg->nodeCount++] = node;


    return node;
}

CfgNodeStack* createCfgNodeStack() {
    // Создаем начальный элемент стека
    CfgNodeStack* stack = malloc(sizeof(CfgNodeStack));
    
    // Проверка, что память была успешно выделена
    if (!stack) {
        return NULL; 
    }
    
    // Инициализация нового стека
    stack->node = NULL;    
    stack->next = NULL;    

    return stack;
}

void addSuccessor(CfgNode* from, CfgNode* to) {
    if (!from || !to) return;
    
    // Проверка на существующую связь
    for (int i = 0; i < from->successorCount; i++) {
        if (from->successors[i] == to) {
            return;  
        }
    }

    if (from->successorCount >= from->successorCapacity) {
        from->successorCapacity = from->successorCapacity ? from->successorCapacity * 2 : 4;
        from->successors = realloc(from->successors, sizeof(CfgNode*) * from->successorCapacity);
    }
    
    from->successors[from->successorCount++] = to;
}

void addPredecessor(CfgNode* node, CfgNode* predecessor) {
    if (!node || !predecessor) return;
    
    for (int i = 0; i < node->predecessorCount; i++) {
        if (node->predecessors[i] == predecessor) {
            return;  // Связь уже существует, добавлять не нужно
        }
    }

    if (node->predecessorCount >= node->predecessorCapacity) {
        node->predecessorCapacity = node->predecessorCapacity ? node->predecessorCapacity * 2 : 4;
        node->predecessors = realloc(node->predecessors, sizeof(CfgNode*) * node->predecessorCapacity);
    }
    
    node->predecessors[node->predecessorCount++] = predecessor;
}

ControlFlowGraph* createControlFlowGraph(AstNode* astRoot) {
    if (!astRoot) return NULL;
    
    ControlFlowGraph* cfg = malloc(sizeof(ControlFlowGraph));
    cfg->nodeCapacity = 4;  // Начальная емкость для хранения узлов графа
    cfg->nodes = malloc(sizeof(CfgNode*) * cfg->nodeCapacity);
    cfg->nodeCount = 0;

    // Создаем узлы входа и выхода
    cfg->entryNode = createCfgNode(cfg, CFG_NODE_ENTRY, NULL, "Entry");
    cfg->exitNode = createCfgNode(cfg, CFG_NODE_EXIT, NULL, "Exit");
    
    // Создаем стек для обработки циклов
    CfgNodeStack* loopStack = createCfgNodeStack();

    // Начинаем построение графа с узла "FuncDef"
    AstNode* childNode = astRoot->children[0];
    
    while (strcmp(childNode->nodeName, "FuncDef") != 0) {
        childNode = childNode->children[0];
    }

    CfgNode* lastNode = buildCfgForFunctionBody(childNode, cfg->entryNode, cfg, loopStack);
    addSuccessor(lastNode, cfg->exitNode);

    destroyCfgNodeStack(loopStack);

    return cfg;
}

CfgNode* buildCfgForFunctionBody(AstNode* funcDefNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!funcDefNode || strcmp(funcDefNode->nodeName, "FuncDef") != 0) {
        return previousNode;
    }

    // Обрабатываем "FuncSignature"
    AstNode* funcSignatureNode = funcDefNode->children[0];
    CfgNode* currentNode = buildCfgForFuncSignature(funcSignatureNode, previousNode, cfg, loopStack);

    // Обрабатываем "BodySig"
    if (funcDefNode->childrenCount > 1) {
        AstNode* bodySigNode = funcDefNode->children[1];
        currentNode = buildCfgForBodySig(bodySigNode, currentNode, cfg, loopStack);
    }

    return currentNode;
}

CfgNode* buildCfgForFuncSignature(AstNode* funcSignatureNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!funcSignatureNode || strcmp(funcSignatureNode->nodeName, "FuncSignature") != 0) {
        return previousNode;
    }

    // Название функции (первый потомок FuncSignature)
    AstNode* functionNameNode = funcSignatureNode->children[0];
    cfg->functionName = strdup(functionNameNode->nodeName);

    // Аргументы функции (второй потомок FuncSignature)
    AstNode* listArgDefsNode = funcSignatureNode->children[1];
    if (strcmp(listArgDefsNode->nodeName, "ListArgDefs") == 0) {
        cfg->argumentCount = listArgDefsNode->childrenCount;
        cfg->argumentNames = malloc(cfg->argumentCount * sizeof(char*));
        cfg->argumentTypes = malloc(cfg->argumentCount * sizeof(char*));

        for (int i = 0; i < cfg->argumentCount; i++) {
            AstNode* argNode = listArgDefsNode->children[i];
            if (strcmp(argNode->nodeName, "Argument") == 0) {
                AstNode* argNameNode = argNode->children[0];
                AstNode* argTypeNode = argNode->children[1];

                cfg->argumentNames[i] = strdup(argNameNode->nodeName);
                while(argTypeNode->children != NULL){
                    argTypeNode = argTypeNode->children[0];
                }
                cfg->argumentTypes[i] = strdup(argTypeNode->nodeName);
            }
        }
    }

    // Тип возвращаемого значения (третий потомок FuncSignature)
    if (funcSignatureNode->childrenCount > 2) {
        AstNode* returnTypeNode = funcSignatureNode->children[2];
        if (strcmp(returnTypeNode->nodeName, "TypeRef") == 0) {
            cfg->returnType = strdup(returnTypeNode->nodeName);
        }
    } else {
        cfg->returnType = strdup("void");  // Если тип не указан, предполагаем "void"
    }

    //Создаем узел для сигнатуры функции
    // CfgNode* signatureNode = createCfgNode(cfg, CFG_NODE_STATEMENT, funcSignatureNode, "Function Signature");
    // addSuccessor(previousNode, signatureNode);

    return previousNode;
}

CfgNode* buildCfgForBodySig(AstNode* bodySigNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!bodySigNode || strcmp(bodySigNode->nodeName, "BodySig") != 0) {
        return previousNode;
    }
    
    if(bodySigNode->childrenCount == 2){
        
        //------------------------- Todo обработать Vars_list если он есть
        
        // Обрабатываем тело функции как блок операторов
        return buildCfgForBlock(bodySigNode->children[1], previousNode, cfg, loopStack);
    } else if(bodySigNode->childrenCount == 1){
        return buildCfgForBlock(bodySigNode->children[0], previousNode, cfg, loopStack);
    }
}

//-----------------------------
//Ф-ии построения ГПУ для STATEMENT

CfgNode* buildCfgNodeByType(AstNode* node, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!node) return previousNode;

    // Определяем тип узла по имени и диспетчеризуем 
    if (strcmp(node->nodeName, "BlockStatement") == 0) {
        return buildCfgForBlock(node, previousNode, cfg, loopStack);
    } else if (strcmp(node->nodeName, "IfStatement") == 0) {
        return buildCfgForIf(node, previousNode, cfg, loopStack);
    } else if (strcmp(node->nodeName, "WhileStatement") == 0) {
        return buildCfgForWhile(node, previousNode, cfg, &loopStack);
    } else if (strcmp(node->nodeName, "ExpressionStatement") == 0) {
        return buildCfgForStatement(node, previousNode, cfg, loopStack);
    } else if (strcmp(node->nodeName, "DoStatement") == 0) {
        return buildCfgForDoWhile(node, previousNode, cfg, &loopStack);
    } else if (strcmp(node->nodeName, "BreakStatement") == 0) {
        return buildCfgForBreak(node, previousNode, cfg, &loopStack);
    }
    return previousNode;
}

CfgNode* buildCfgForStatement(AstNode* statement, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!statement || strcmp(statement->nodeName, "ExpressionStatement") != 0) {
        return previousNode;
    }

    // Переходим к обработке потомка "Expression"
    if (statement->childrenCount > 0) {
        AstNode* expressionNode = statement->children[0]; // Потомок Expression
        CfgNode* expressionCfgNode = createCfgNode(cfg, CFG_NODE_STATEMENT, expressionNode, "Expression");
        addSuccessor(previousNode, expressionCfgNode);
        return expressionCfgNode; // Возвращаем узел выражения для дальнейшей цепочки
    }

    // Если потомка нет, возвращаем узел "Expression statement"
    return previousNode;
}

CfgNode* buildCfgForBlock(AstNode* blockNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!blockNode || strcmp(blockNode->nodeName, "BlockStatement") != 0 ) { 
        return previousNode;
    }

    // Создаем узел для входа в блок
    CfgNode* blockEntryNode = createCfgNode(cfg, CFG_NODE_BLOCK, NULL, "Block");
    addSuccessor(previousNode, blockEntryNode);

    CfgNode* currentNode = blockEntryNode;

    // Обработка всех операторов внутри блока
    for (int i = 0; i < blockNode->childrenCount; i++) {
        AstNode* childStatement = blockNode->children[i];

        // Рекурсивный вызов диспетчера для каждого дочернего узла
        currentNode = buildCfgNodeByType(childStatement, currentNode, cfg, loopStack);

        // Проверяем, если текущий узел является `break`, прерываем дальнейшую обработку блока
        if (currentNode->type == CFG_NODE_BREAK) {
            // Возвращаем узел `break`, чтобы внешняя функция могла корректно обработать его
            return currentNode;
        }
    }

    return currentNode;
}

CfgNode* buildCfgForIf(AstNode* ifNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!ifNode || strcmp(ifNode->nodeName, "IfStatement") != 0) {
        return previousNode;
    }

    // В AST для If: 
    // children[0] - условие
    // children[1] - then-ветка 
    // children[2] - else-ветка (optional)
    AstNode* conditionNode = ifNode->children[0];
    AstNode* thenStatementNode = ifNode->children[1];
    AstNode* elseStatementNode = (ifNode->childrenCount > 2) ? ifNode->children[2] : NULL;

    // Создаем узел условия
    CfgNode* conditionCfgNode = createCfgNode(cfg, CFG_NODE_CONDITION, conditionNode, "IF Condition");
    addSuccessor(previousNode, conditionCfgNode);

    // Создаем узлы для ветвления
    CfgNode* thenEndNode = buildCfgNodeByType(thenStatementNode, conditionCfgNode, cfg, loopStack);
    
    if (thenEndNode->type == CFG_NODE_BREAK && elseStatementNode == NULL){
        return thenEndNode;
        printf("norm1\n");
    }

    if (elseStatementNode != NULL){

        CfgNode* elseEndNode = buildCfgNodeByType(elseStatementNode, conditionCfgNode, cfg, loopStack);
   
        if (elseEndNode->type == CFG_NODE_BREAK && thenEndNode->type == CFG_NODE_BREAK){
            // Сценарий 2: Оба содержат break, возвращаем любой из них
            return thenEndNode; // или elseEndNode - в зависимости от того, что более подходит
        }

        if (elseEndNode->type != CFG_NODE_BREAK && thenEndNode->type == CFG_NODE_BREAK){
            // Сценарий 3: Только ветка Then содержит break, Else продолжается
            return elseEndNode;
        }

        if (elseEndNode->type == CFG_NODE_BREAK && thenEndNode->type != CFG_NODE_BREAK){
            // Сценарий 4: Только ветка Else содержит break, Then продолжается
            return thenEndNode;
        }

        if (elseEndNode->type != CFG_NODE_BREAK && thenEndNode->type != CFG_NODE_BREAK){
            // Стандартная обработка обеих ветвей, создаем узел слияния
            CfgNode* mergeNode = createCfgNode(cfg, CFG_NODE_STATEMENT, NULL, "IF Merge");
            addSuccessor(thenEndNode, mergeNode);
            addSuccessor(elseEndNode, mergeNode);
            return mergeNode;
        }
    } else {
        // Если else-ветка отсутствует, создаем узел слияния только для then-ветки
        CfgNode* mergeNode = createCfgNode(cfg, CFG_NODE_STATEMENT, NULL, "IF Merge");
        addSuccessor(thenEndNode, mergeNode);
        return mergeNode;
    }

    // На случай если else отсутствует и thenEndNode не содержит break
    return thenEndNode;
    
}

CfgNode* buildCfgForWhile(AstNode* whileNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack) {
    if (!whileNode || strcmp(whileNode->nodeName, "WhileStatement") != 0) {
        return previousNode;
    }
    printf("While\n");
    // Создаем узел для условия while
    AstNode* conditionNode = whileNode->children[0];
    CfgNode* conditionNodeCfg = createCfgNode(cfg, CFG_NODE_CONDITION, conditionNode, "While Condition");
    addSuccessor(previousNode, conditionNodeCfg);

    // Создаем узел "After While" для выхода из цикла
    CfgNode* afterWhileNode = createCfgNode(cfg, CFG_NODE_STATEMENT, NULL, "After While");

    // Добавляем узел выхода из цикла в стек для break
    push(loopStack, afterWhileNode);

    // Строим граф для тела цикла
    AstNode* bodyNode = whileNode->children[1];
    CfgNode* bodyEndNode = buildCfgNodeByType(bodyNode, conditionNodeCfg, cfg, *loopStack);

    printf("%s\n", bodyEndNode->label);
    // Проверяем, был ли `break`
    if (bodyEndNode->type == CFG_NODE_BREAK) {
        // Если `break` найден, связать условие с узлом выхода и завершить цикл
        printf("sdfsdfds");
        addSuccessor(conditionNodeCfg, afterWhileNode);
        pop(loopStack);
        return afterWhileNode;
    }

    // Добавляем связь от конца тела к условию, чтобы вернуться и проверить условие снова
    addSuccessor(bodyEndNode, conditionNodeCfg);

    // Условие ведет в узел выхода, если проверка завершена
    addSuccessor(conditionNodeCfg, afterWhileNode);

    // Удаляем узел из стека после завершения обработки тела цикла
    pop(loopStack);

    return afterWhileNode;
}

CfgNode* buildCfgForDoWhile(AstNode* doWhileNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack) {
    if (!doWhileNode || strcmp(doWhileNode->nodeName, "DoStatement") != 0) {
        return previousNode;
    }

    // В AST для do-while:
    // children[0] - тело цикла
    // children[1] - условие

    // Создаем узел для тела цикла
    AstNode* bodyNode = doWhileNode->children[0];
    CfgNode* bodyNodeCfg = createCfgNode(cfg, CFG_NODE_BLOCK, bodyNode, "DoWhile Body");
    addSuccessor(previousNode, bodyNodeCfg);

    CfgNode* endNodeCfg = createCfgNode(cfg, CFG_NODE_STATEMENT, bodyNode, "After DoWhile");


    // Добавляем узел выхода из цикла в стек для break
    push(loopStack, endNodeCfg);

    // Строим граф для тела цикла
    CfgNode* bodyEndNode = buildCfgNodeByType(bodyNode, bodyNodeCfg, cfg, *loopStack);
    
    if(bodyEndNode->type == CFG_NODE_BREAK){
        pop(loopStack);
        return endNodeCfg;
    } 
    
    // Создаем узел для условия
    AstNode* conditionNode = doWhileNode->children[1];
    CfgNode* conditionNodeCfg = createCfgNode(cfg, CFG_NODE_CONDITION, conditionNode, "DoWhile Condition");
    addSuccessor(bodyEndNode, conditionNodeCfg);
    addSuccessor(conditionNodeCfg, bodyNodeCfg);
    addSuccessor(conditionNodeCfg, endNodeCfg);
    pop(loopStack);
    return endNodeCfg;

}

CfgNode* buildCfgForBreak(AstNode* breakNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack** loopStack) {
    if (!breakNode || strcmp(breakNode->nodeName, "BreakStatement") != 0) {
        return previousNode;
    }
    printf("Break\n");
    // Создаем узел для BreakStatement
    CfgNode* breakNodeCfg = createCfgNode(cfg, CFG_NODE_BREAK, breakNode, "Break Statement");
    addSuccessor(previousNode, breakNodeCfg);

    // Найти ближайший узел выхода из цикла из стека
    CfgNode* targetLoopExitNode = peek(*loopStack);
    if (targetLoopExitNode) {
        // Добавляем связь от break к узлу выхода из цикла
        addSuccessor(breakNodeCfg, targetLoopExitNode);
    } else {
        // Ошибка: оператор break находится вне контекста цикла
        fprintf(stderr, "Error: 'break' statement found outside of a loop context.\n");
    }

    // Возвращаем узел выхода из цикла, так как после break дальнейшая цепочка не обрабатывается
    return breakNodeCfg;
}
//-----------------------------
// Ф-ии работы со стеком вызовов циклов

void push(CfgNodeStack** stack, CfgNode* node) {
    CfgNodeStack* newElement = malloc(sizeof(CfgNodeStack));
    newElement->node = node;
    newElement->next = *stack;
    *stack = newElement;
}

CfgNode* pop(CfgNodeStack** stack) {
    if (*stack == NULL) return NULL;

    CfgNodeStack* top = *stack;
    CfgNode* node = top->node;
    *stack = top->next;
    free(top);
    return node;
}

CfgNode* peek(CfgNodeStack* stack) {
    return stack ? stack->node : NULL;
}

//-----------------------------
// Ф-ии освобождения памяти и построения ГПУ

void destroyControlFlowGraph(ControlFlowGraph* cfg) {
    if (!cfg) return;
    
    // Освобождаем память для каждого узла
    for (int i = 0; i < cfg->nodeCount; i++) {
        free(cfg->nodes[i]->label);
        free(cfg->nodes[i]->successors);
        free(cfg->nodes[i]->predecessors);
        free(cfg->nodes[i]);
    }
    
    free(cfg->nodes);

    // Освобождаем память для информации о функции
    if (cfg->functionName) {
        free(cfg->functionName);
    }

    if (cfg->returnType) {
        free(cfg->returnType);
    }

    if (cfg->argumentNames) {
        for (int i = 0; i < cfg->argumentCount; i++) {
            free(cfg->argumentNames[i]);
        }
        free(cfg->argumentNames);
    }

    if (cfg->argumentTypes) {
        for (int i = 0; i < cfg->argumentCount; i++) {
            free(cfg->argumentTypes[i]);
        }
        free(cfg->argumentTypes);
    }

    // Освобождаем сам граф
    free(cfg);
}

void writeCfgAsDot(ControlFlowGraph* cfg, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open file for writing DOT");
        return;
    }

    fprintf(file, "digraph ControlFlowGraph {\n");
    fprintf(file, "  node [shape=box];\n");

    // Проходим по каждому узлу и записываем его метку
    for (int i = 0; i < cfg->nodeCount; i++) {
        
        CfgNode* node = cfg->nodes[i];
        fprintf(file, "  node%d [label=\"%s\"];\n", node->id, node->label ? node->label : "Unnamed Node");
    }

    // Добавляем связи между узлами
    for (int i = 0; i < cfg->nodeCount; i++) {
        CfgNode* fromNode = cfg->nodes[i];
        for (int j = 0; j < fromNode->successorCount; j++) {
            CfgNode* toNode = fromNode->successors[j];
            fprintf(file, "  node%d -> node%d;\n", fromNode->id, toNode->id);
        }
    }

    fprintf(file, "}\n");
    fclose(file);
    printf("DOT file saved: %s\n", filename);
}

void destroyCfgNodeStack(CfgNodeStack* stack) {
    if (!stack) return;

    // Освобождаем все узлы стека
    while (stack != NULL) {
        CfgNodeStack* temp = stack;  // Сохраняем текущий элемент стека
        stack = stack->next;         // Переходим к следующему элементу
        free(temp);                  // Освобождаем память текущего элемента
    }
}

//TODO
// break, do while, expression
// подумать над созданием ф-ии для парсинга типа (для случаев сложных типов)
// отказаться от хранения указателя на AstNode в Cfg
