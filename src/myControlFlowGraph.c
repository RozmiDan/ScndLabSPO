#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "myControlFlowGraph.h"

CfgNode* createCfgNode(ControlFlowGraph* cfg, CfgNodeType type, AstNode* astNode, const char* label) {
    CfgNode* node = malloc(sizeof(CfgNode));
    node->id = cfg->nodeCount; 
    node->type = type;
    node->operations = malloc(sizeof(OperationTree*) * 2);
    node->label = label ? strdup(label) : NULL;
    
    node->successors = NULL;
    node->predecessors = NULL;
    node->successorCount = 0;
    node->predecessorCount = 0;
    node->successorCapacity = 0;
    node->predecessorCapacity = 0;
    node->operationCount = 0;
    node->operationCapacity = 2;
    
    
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

void addCfgError(ErrorCollection* collection, const char* message) {
    ErrorInfoCfg* newError = malloc(sizeof(ErrorInfoCfg));
    newError->message = strdup(message);
    newError->next = NULL;

    if (!collection->head) {
        collection->head = newError;
        collection->tail = newError;
    } else {
        collection->tail->next = newError;
        collection->tail = newError;
    }
    collection->count++;
}

void addOperationToCfgNode(CfgNode* node, OperationTree* operation) {
    if (node->operationCount >= node->operationCapacity) {
        node->operationCapacity *= 2;
        node->operations = realloc(node->operations, sizeof(OperationTree*) * node->operationCapacity);
    }
    node->operations[node->operationCount++] = operation;
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
    cfg->arguments = NULL;
    cfg->argumentCount = 0;
    cfg->calledFunctionCount = 0;

    cfg->calledFunctions = NULL;

    // Создаем узлы входа и выхода
    cfg->entryNode = createCfgNode(cfg, CFG_NODE_ENTRY, NULL, "Entry");
    cfg->exitNode = createCfgNode(cfg, CFG_NODE_EXIT, NULL, "Exit");
    
    // Инициализация коллекции ошибок
    cfg->errors = malloc(sizeof(ErrorCollection));
    if (!cfg->errors) {
        free(cfg);
        return NULL;
    }
    cfg->errors->head = NULL;
    cfg->errors->count = 0;

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
        cfg->arguments = malloc(cfg->argumentCount * sizeof(VariableInfo*));

        for (int i = 0; i < cfg->argumentCount; i++) {
            AstNode* argNode = listArgDefsNode->children[i];
            if (strcmp(argNode->nodeName, "Argument") == 0) {
                VariableInfo* variable = malloc(sizeof(VariableInfo));

                if (argNode->childrenCount == 2) {
                    // Первый потомок — имя переменной, второй — тип
                    AstNode* argNameNode = argNode->children[0];
                    AstNode* argTypeNode = argNode->children[1];

                    variable->name = strdup(argNameNode->nodeName);
                    //variable->type = strdup(argTypeNode->nodeName);
                    variable->type = handleVariableType(argTypeNode, cfg);
                } else if (argNode->childrenCount == 1) {
                    // Один потомок — считаем его именем, тип не задается
                    AstNode* argNameNode = argNode->children[0];

                    variable->name = strdup(argNameNode->nodeName);  // Имя переменной отсутствует
                    variable->type = NULL;
                }
                cfg->arguments[i] = variable;
            }
        }
    }

    // Тип возвращаемого значения (третий потомок FuncSignature)
    if (funcSignatureNode->childrenCount > 2) {
        AstNode* returnTypeNode = funcSignatureNode->children[2];
        if (strcmp(returnTypeNode->nodeName, "TypeRef") == 0) {
            cfg->returnType = strdup(returnTypeNode->nodeName);
        }
        ////--------------------------------------------------------------------
        else {
            cfg->returnType = strdup("void"); 
        }
        ////-------------------------------------------------------------
    } else {
        cfg->returnType = strdup("void");  // Если тип не указан, предполагаем "void"
    }

    return previousNode;
}

CfgNode* buildCfgForBodySig(AstNode* bodySigNode, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!bodySigNode || strcmp(bodySigNode->nodeName, "BodySig") != 0) {
        return previousNode;
    }

    CfgNode* currentNode = previousNode;
    AstNode* blockStatementNode = NULL;

    // Проходим по всем потомкам узла "BodySig"
    for (int i = 0; i < bodySigNode->childrenCount; i++) {
        AstNode* childNode = bodySigNode->children[i];

        // Если потомок - "VarsDefenition"
        if (strcmp(childNode->nodeName, "VarsDefenition") == 0) {
            // Обработка объявления переменных
            handleVarsDefenition(childNode, cfg);
        } else if (strcmp(childNode->nodeName, "BlockStatement") == 0) {
            // Запоминаем узел "BlockStatement" для дальнейшей обработки
            blockStatementNode = childNode;
        }
    }

    // Обрабатываем "BlockStatement", если он был найден
    if (blockStatementNode) {
        currentNode = buildCfgForBlock(blockStatementNode, currentNode, cfg, loopStack);
    }

    return currentNode;
}

void handleVarsDefenition(AstNode* varsDefNode, ControlFlowGraph* cfg) {
    if (!varsDefNode || strcmp(varsDefNode->nodeName, "VarsDefenition") != 0) {
        return;
    }

    VariableInfo** temporaryVariables = NULL;  // Временный массив для хранения добавляемых переменных
    int temporaryVariableCount = 0;

    // Итерируем по всем потомкам "VarsDefenition"
    for (int i = 0; i < varsDefNode->childrenCount; i++) {
        AstNode* childNode = varsDefNode->children[i];

        if (strcmp(childNode->nodeName, "ListIdentifier") == 0) {
            // Обработка списка идентификаторов
            for (int j = 0; j < childNode->childrenCount; j++) {
                AstNode* identifierNode = childNode->children[j];
                
                // Создаем новую переменную с именем из ListIdentifier и типом NULL (пока не установлен)
                VariableInfo* newVariable = malloc(sizeof(VariableInfo));
                newVariable->name = strdup(identifierNode->nodeName);
                newVariable->type = NULL;

                // Добавляем переменную во временный массив
                temporaryVariables = realloc(temporaryVariables, sizeof(VariableInfo*) * (temporaryVariableCount + 1));
                if (!temporaryVariables) {
                    perror("Failed to allocate memory for variables");
                }

                temporaryVariables[temporaryVariableCount] = newVariable;
                temporaryVariableCount++;
            }
        } else if (strcmp(childNode->nodeName, "BuiltinType") == 0 ||
                   strcmp(childNode->nodeName, "CustomType") == 0 ||
                   strcmp(childNode->nodeName, "ArrayType") == 0) {
            // Обработка типа данных
            char* variableType = handleVariableType(childNode, cfg);

            // Присваиваем тип всем временно добавленным переменным
            for (int k = 0; k < temporaryVariableCount; k++) {
                if (temporaryVariables[k]->type == NULL) {
                    temporaryVariables[k]->type = strdup(variableType);
                }
            }

            free(variableType);
        } else {
            // Ошибка: неизвестный потомок в "VarsDefenition"
            addCfgError(cfg->errors, "Unknown node in VarsDefenition");
        }
    }
    // После обработки всех потомков добавляем временные переменные в граф
    if (temporaryVariableCount > 0) {
        cfg->argumentCount += temporaryVariableCount;
        cfg->arguments = realloc(cfg->arguments, sizeof(VariableInfo*) * cfg->argumentCount);
        if (!cfg->arguments) {
            perror("Failed to allocate memory for variables in CFG");
            // exit(EXIT_FAILURE);
        }

        // Копируем временные переменные в массив переменных CFG
        for (int k = 0; k < temporaryVariableCount; k++) {
            cfg->arguments[cfg->argumentCount - temporaryVariableCount + k] = temporaryVariables[k];
        }

        // Освобождаем временный массив указателей (но не сами переменные)
        free(temporaryVariables);
    }
}

// Функция для обработки типа данных
char* handleVariableType(AstNode* typeNode, ControlFlowGraph* cfg) {
    if (!typeNode) return NULL;

    if (strcmp(typeNode->nodeName, "BuiltinType") == 0) {
        // Обработка для "BuiltinType"
        if (typeNode->childrenCount > 0) {
            AstNode* type = typeNode->children[0];
            return strdup(type->nodeName);
        } else {
            return NULL;
        }

    } else if (strcmp(typeNode->nodeName, "CustomType") == 0) {
        // Обработка для "CustomType"
        if (typeNode->childrenCount > 0) {
            AstNode* type = typeNode->children[0];
            return strdup(type->nodeName);
        } else {
            return NULL;
        }

    } else if (strcmp(typeNode->nodeName, "ArrayType") == 0) {
        // Начинаем с базового типа
        char* baseType = handleVariableType(typeNode->children[0], cfg);
        if (!baseType) {
            addCfgError(cfg->errors, "Failed to determine base type in ArrayType");
            return NULL;
        }

        // Обрабатываем узел "Elems" для получения размерности массива
        char* dimensions = NULL;
        for (int i = 1; i < typeNode->childrenCount; i++) {
            if (strcmp(typeNode->children[i]->nodeName, "Elems") == 0) {
                AstNode* elemsNode = typeNode->children[i];
                
                // Собираем размерности
                char* currentDimensions = malloc(elemsNode->childrenCount * 2);  // Размеры вида ",," + '\0'
                if (!currentDimensions) {
                    perror("Failed to allocate memory for dimensions");
                    free(baseType);
                    return NULL;
                }

                for (int j = 0; j < elemsNode->childrenCount; j++) {
                    currentDimensions[j] = ',';
                }
                currentDimensions[elemsNode->childrenCount] = '\0';

                if (dimensions) {
                    // Комбинируем с уже существующими размерностями
                    size_t newSize = strlen(dimensions) + strlen(currentDimensions) + 3;  // Existing + "[]" + '\0'
                    char* newDimensions = malloc(newSize);
                    if (!newDimensions) {
                        perror("Failed to allocate memory for new dimensions");
                        free(baseType);
                        free(dimensions);
                        free(currentDimensions);
                        return NULL;
                    }
                    snprintf(newDimensions, newSize, "%s[%s]", dimensions, currentDimensions);
                    free(dimensions);
                    free(currentDimensions);
                    dimensions = newDimensions;
                } else {
                    // Начинаем с первой размерности
                    size_t newSize = strlen(currentDimensions) + 3;  // "[]" + '\0'
                    dimensions = malloc(newSize);
                    if (!dimensions) {
                        perror("Failed to allocate memory for dimensions");
                        free(baseType);
                        free(currentDimensions);
                        return NULL;
                    }
                    snprintf(dimensions, newSize, "[%s]", currentDimensions);
                    free(currentDimensions);
                }
            }
        }

        // Если не нашли узла "Elems", просто создаем пустую размерность
        if (!dimensions) {
            dimensions = strdup("[]");
        }

        // Формируем итоговую строку для "ArrayType"
        size_t arrayTypeSize = strlen(baseType) + strlen(dimensions) + 12;  // "array of " + baseType + dimensions + '\0'
        char* arrayType = malloc(arrayTypeSize);
        if (!arrayType) {
            perror("Failed to allocate memory for arrayType");
            free(baseType);
            free(dimensions);
            return NULL;
        }
        snprintf(arrayType, arrayTypeSize, "array %s of %s", dimensions, baseType);

        free(baseType);
        free(dimensions);

        return arrayType;

    } else {
        // Ошибка: неизвестный тип данных
        addCfgError(cfg->errors, "Unknown type in variable definition");
        return NULL;
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
        return buildCfgForExprStatement(node, previousNode, cfg, loopStack);
    } else if (strcmp(node->nodeName, "DoStatement") == 0) {
        return buildCfgForDoWhile(node, previousNode, cfg, &loopStack);
    } else if (strcmp(node->nodeName, "BreakStatement") == 0) {
        return buildCfgForBreak(node, previousNode, cfg, &loopStack);
    }
    return previousNode;
}

CfgNode* buildCfgForExprStatement(AstNode* statement, CfgNode* previousNode, ControlFlowGraph* cfg, CfgNodeStack* loopStack) {
    if (!statement || strcmp(statement->nodeName, "ExpressionStatement") != 0) {
        return previousNode;
    }

    // Переходим к обработке потомка "Expression"
    if (statement->childrenCount > 0) {
        AstNode* expressionNode = statement->children[0]; // Потомок Expression
        CfgNode* expressionCfgNode = createCfgNode(cfg, CFG_NODE_STATEMENT, expressionNode, "Expression Statement");
        addSuccessor(previousNode, expressionCfgNode);

        // Используем buildOperTreeForExpr для обработки узла Expression и добавления дерева операций к узлу CFG
        buildOperTreeForExpr(expressionNode, expressionCfgNode, cfg);
        
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
    
    buildOperTreeForExpr(conditionNode, conditionCfgNode, cfg);

    // Создаем узлы для ветвления
    CfgNode* thenEndNode = buildCfgNodeByType(thenStatementNode, conditionCfgNode, cfg, loopStack);
    
    if (thenEndNode->type == CFG_NODE_BREAK && elseStatementNode == NULL){
        return thenEndNode;
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
    // Создаем узел для условия while
    AstNode* conditionNode = whileNode->children[0];
    CfgNode* conditionNodeCfg = createCfgNode(cfg, CFG_NODE_CONDITION, conditionNode, "While Condition");
    addSuccessor(previousNode, conditionNodeCfg);

    // Строим дерево операций для условия и прикрепляем к узлу CFG
    buildOperTreeForExpr(conditionNode, conditionNodeCfg, cfg);

    // Создаем узел "After While" для выхода из цикла
    CfgNode* afterWhileNode = createCfgNode(cfg, CFG_NODE_STATEMENT, NULL, "After While");

    // Добавляем узел выхода из цикла в стек для break
    push(loopStack, afterWhileNode);

    // Строим граф для тела цикла
    AstNode* bodyNode = whileNode->children[1];
    CfgNode* bodyEndNode = buildCfgNodeByType(bodyNode, conditionNodeCfg, cfg, *loopStack);

    // Проверяем, был ли `break`
    if (bodyEndNode->type == CFG_NODE_BREAK) {
        // Если `break` найден, связать условие с узлом выхода и завершить цикл
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
    
    // Добавляем дерево операций
    buildOperTreeForExpr(conditionNode, conditionNodeCfg, cfg);
    
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
        addCfgError(cfg->errors, "'break' statement found outside of a loop context.");
    }

    // Возвращаем узел выхода из цикла, так как после break дальнейшая цепочка не обрабатывается
    return breakNodeCfg;
}

//-----------------------------
//Ф-ии построения Дерева Операций 

OperationTree* createOperationTree(const char* operation, int childCount) {
    OperationTree* node = malloc(sizeof(OperationTree));

    node->operation = strdup(operation);
    node->childCount = childCount;
    if (childCount > 0) {
        node->children = malloc(sizeof(OperationTree*) * childCount);
        for (int i = 0; i < childCount; i++) {
            node->children[i] = NULL;  // Инициализируем потомков как NULL
        }
    } else {
        node->children = NULL;
    }
    return node;
}

OperationTree* buildOperationTreeFromAst(AstNode* astNode, ControlFlowGraph* cfg) {
    if (!astNode) {
        return NULL;
    }

    if (strcmp(astNode->nodeName, "Identifier") == 0 && astNode->childrenCount == 1) {
        // Создаем узел для чтения переменной
        OperationTree* readNode = createOperationTree("read", 1);
        readNode->children[0] = createOperationTree(astNode->children[0]->nodeName, 0);
        return readNode;
    }

    if (strcmp(astNode->nodeName, "Literal") == 0 && astNode->childrenCount == 1) {
        // Создаем узел для константы
        OperationTree* constNode = createOperationTree("const", 1);
        constNode->children[0] = createOperationTree(astNode->children[0]->nodeName, 0);
        return constNode;
    }

    if ((strcmp(astNode->nodeName, "Expression") == 0 || strcmp(astNode->nodeName, "Braces") == 0) 
        && astNode->childrenCount == 1) {
        // Переходим к единственному потомку узла 
        return buildOperationTreeFromAst(astNode->children[0], cfg);
    }

    // Обрабатываем узел типа "FunctionCall"
    if (strcmp(astNode->nodeName, "FunctionCall") == 0) {
        // Начинаем с создания узла "call"
        int childCount = astNode->childrenCount;
        OperationTree* callNode = createOperationTree("call", childCount);

        // Инициализируем массив потомков
        callNode->children = malloc(sizeof(OperationTree*) * childCount);

        int processedChildren = 0;

        // Обрабатываем всех потомков "FunctionCall"
        for (int i = 0; i < astNode->childrenCount; i++) {
            AstNode* currentChild = astNode->children[i];

            if (i == 0) {
                // Первый потомок — это название функции, добавляем его как обычный узел
                callNode->children[processedChildren++] = buildOperationTreeFromAst(currentChild, cfg);
                // Добавляем название функции в коллекцию вызванных функций графа потока управления
                cfg->calledFunctionCount++;
                cfg->calledFunctions = realloc(cfg->calledFunctions, sizeof(char*) * cfg->calledFunctionCount);
                if (!cfg->calledFunctions) {
                    perror("Failed to allocate memory for called functions");
                    // exit(EXIT_FAILURE);
                }
                cfg->calledFunctions[cfg->calledFunctionCount - 1] = strdup(currentChild->nodeName);

            } else if (strcmp(currentChild->nodeName, "Args") == 0) {
                // Второй или любой другой потомок, который является "Args", добавляем его аргументы
                for (int j = 0; j < currentChild->childrenCount; j++) {
                    callNode->children = realloc(callNode->children, sizeof(OperationTree*) * (processedChildren + 1));
                    callNode->children[processedChildren++] = buildOperationTreeFromAst(currentChild->children[j], cfg);
                }
            } else {
                // Обрабатываем других потомков, которые не являются "Args"
                callNode->children[processedChildren++] = buildOperationTreeFromAst(currentChild, cfg);
            }
        }

        // Обновляем количество потомков после обработки всех узлов
        callNode->childCount = processedChildren;

        return callNode;
    }

    if (strcmp(astNode->nodeName, "ArrayAccess") == 0) {
        // Создаем узел для доступа к элементу массива
        OperationTree* getElemNode = createOperationTree("getElem", astNode->childrenCount);

        // Обрабатываем потомков ноды "ArrayAccess"
        int childIndex = 0;
        for (int i = 0; i < astNode->childrenCount; i++) {
            if (strcmp(astNode->children[i]->nodeName, "Indices") == 0) {
                // Обрабатываем потомков "Indices"
                AstNode* indicesNode = astNode->children[i];
                for (int j = 0; j < indicesNode->childrenCount; j++) {
                    getElemNode->children[childIndex++] = buildOperationTreeFromAst(indicesNode->children[j], cfg);
                }
            } else {
                // Обрабатываем остальных потомков как обычно
                getElemNode->children[childIndex++] = buildOperationTreeFromAst(astNode->children[i], cfg);
            }
        }

        return getElemNode;
    }

    // Отображаем операцию AST на более информативное название
    const char* operationName = mapAstOperationToOperationName(astNode->nodeName);
    // Создаем узел для текущей операции
    OperationTree* operationNode = createOperationTree(operationName, astNode->childrenCount);

    // Рекурсивно строим дерево для каждого дочернего узла
    for (int i = 0; i < astNode->childrenCount; i++) {
        operationNode->children[i] = buildOperationTreeFromAst(astNode->children[i], cfg);
    }

    return operationNode;
}

void buildOperTreeForExpr(AstNode* expressionNode, CfgNode* previousNode, ControlFlowGraph* cfg) {
    if (!expressionNode || strcmp(expressionNode->nodeName, "Expression") != 0) {
        return ;
    }

    OperationTree* operationTree = buildOperationTreeFromAst(expressionNode->children[0], cfg);
   
    // Расширяем массив operations для хранения нового дерева операций
    previousNode->operations = realloc(previousNode->operations, sizeof(OperationTree*) * (previousNode->operationCount + 1));
    if (previousNode->operations == NULL) {
        perror("Failed to allocate memory for operations");
        //exit(EXIT_FAILURE);
    }
    previousNode->operations[previousNode->operationCount] = operationTree;
    previousNode->operationCount += 1;
}

const char* mapAstOperationToOperationName(const char* astOperation) {
    if (strcmp(astOperation, ":=") == 0) { //
        return "set";
    } else if (strcmp(astOperation, "==") == 0) { //
        return "eq";
    } else if (strcmp(astOperation, "!=") == 0) { //
        return "not_eq";
    } else if (strcmp(astOperation, "<") == 0) { //
        return "less";
    } else if (strcmp(astOperation, "<=") == 0) { //
        return "less_eq";
    } else if (strcmp(astOperation, ">") == 0) { //
        return "greater";
    } else if (strcmp(astOperation, ">=") == 0) { //
        return "greater_eq";
    } else if (strcmp(astOperation, "+") == 0) { //
        return "add";
    } else if (strcmp(astOperation, "-") == 0) { //
        return "sub";
    } else if (strcmp(astOperation, "*") == 0) { //
        return "mul";
    } else if (strcmp(astOperation, "/") == 0) { //
        return "div";
    } else if (strcmp(astOperation, "%") == 0) { //
        return "mod";
    } else if (strcmp(astOperation, "&&") == 0) { //
        return "and";
    } else if (strcmp(astOperation, "||") == 0) { //
        return "or";
    } else if (strcmp(astOperation, "!") == 0) {
        return "not";
    } else if (strcmp(astOperation, "^") == 0) { //
        return "xor";
    } else if (strcmp(astOperation, "&") == 0) { //
        return "b_and";
    } else if (strcmp(astOperation, "|") == 0) { //
        return "b_or";
    } else if (strcmp(astOperation, "~") == 0) { //???
        return "b_not";
    } else if (strcmp(astOperation, "<<") == 0) { //
        return "shift_left";
    } else if (strcmp(astOperation, ">>") == 0) { //
        return "shift_right";
    }
    // Возвращаем исходную строку, если она не найдена в списке
    return astOperation;
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
        CfgNode* node = cfg->nodes[i];

        // Освобождаем память для операций в узле
        for (int j = 0; j < node->operationCount; j++) {
            destroyOperationTree(node->operations[j]);
        }
        free(node->operations);

        // Освобождаем остальные поля узла
        free(node->label);
        free(node->successors);
        free(node->predecessors);
        free(node);
    }
    
    // Освобождаем массив узлов
    free(cfg->nodes);

    // Освобождаем струтуру коллекции ошибок
    destroyErrorCollection(cfg->errors);
    free(cfg->errors);

    // Освобождаем память для информации о функции
    if (cfg->functionName) {
        free(cfg->functionName);
    }

    if (cfg->returnType) {
        free(cfg->returnType);
    }

    if (cfg->arguments) {
        for (int i = 0; i < cfg->argumentCount; i++) {
            free(cfg->arguments[i]->name);
            free(cfg->arguments[i]->type);
            free(cfg->arguments[i]);
        }
        free(cfg->arguments);
    }

    if (cfg->calledFunctions) {
        for (int i = 0; i < cfg->calledFunctionCount; i++) {
            free(cfg->calledFunctions[i]);
        }
        free(cfg->calledFunctions);
    }

    free(cfg);
}

void writeOperationTreeAsDot(OperationTree* operation, FILE* file, const char* parentLabel) {
    if (!operation) return;

    // Создаем узел для текущей операции
    fprintf(file, "  %s [label=\"%s\", shape=ellipse, style=filled, fillcolor=lightgray];\n", parentLabel, operation->operation ? operation->operation : "Unknown Operation");

    // Рекурсивно добавляем дочерние узлы
    for (int i = 0; i < operation->childCount; i++) {
        if (operation->children[i]) {
            char childLabel[256];
            snprintf(childLabel, sizeof(childLabel), "%s_child%d", parentLabel, i);
            writeOperationTreeAsDot(operation->children[i], file, childLabel);
            fprintf(file, "  %s -> %s [style=dashed, color=blue];\n", parentLabel, childLabel);
        }
    }
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
        fprintf(file, "  node%d [label=\"%s\"]\n", node->id, node->label ? node->label : "Unnamed Node");

        // Добавляем информацию о деревьях операций
        for (int j = 0; j < node->operationCount; j++) {
            char operationLabel[256];
            snprintf(operationLabel, sizeof(operationLabel), "OpTree%d_%d", node->id, j);
            writeOperationTreeAsDot(node->operations[j], file, operationLabel);
            fprintf(file, "  node%d -> %s;\n", node->id, operationLabel);
        }
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

void destroyErrorCollection(ErrorCollection* collection) {
    ErrorInfoCfg* current = collection->head;
    while (current) {
        ErrorInfoCfg* toDelete = current;
        current = current->next;
        free(toDelete->message);
        free(toDelete);
    }
    collection->head = NULL;
    collection->tail = NULL;
    collection->count = 0;
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

void destroyOperationTree(OperationTree* node) {
    if (!node) return;
    
    // Удаляем всех потомков
    for (int i = 0; i < node->childCount; i++) {
        destroyOperationTree(node->children[i]);
    }
    
    free(node->children);
    free(node->operation);
    free(node);
}

