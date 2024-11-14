#include <stdio.h>
#include <stdlib.h>
#include "myParser.h"
#include "myControlFlowGraph.h"

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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_dot_file>\n", argv[0]);
        return 1;
    }

    ParseResult result = parseString(argv[1]);

    if (result.errorCount > 0) {
        for (int i = 0; i < result.errorCount; i++) {
            printf("Error: %s\n", result.errors[i]->message);
        }
    }

    if (result.tree != NULL) {
        printf("Tree generated successfully.\n");

        FILE* dotFile = fopen(argv[2], "w");
        if (dotFile != NULL) {
            fprintf(dotFile, "digraph ParseTree {\n");
            fprintf(dotFile, "  node [shape=box];\n");
            int nodeCounter = 0;
            writeTreeAsDot(result.tree, dotFile, &nodeCounter); 
            fprintf(dotFile, "}\n");
            fclose(dotFile);
            printf("Dot file generated: %s\n", argv[2]);
        } else {
            printf("Failed to create dot file.\n");
        }
    } else {
        printf("Failed to generate tree.\n");
    }

    //CFG
    ControlFlowGraph* cfg = createControlFlowGraph(result.tree);
    writeCfgAsDot(cfg, "control_flow_graph.dot");
    destroyControlFlowGraph(cfg);

    freeParseResult(&result);

    return 0;
}

