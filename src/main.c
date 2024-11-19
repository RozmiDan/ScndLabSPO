#include <stdio.h>
#include <stdlib.h>
#include "myParser.h"
#include "myControlFlowGraph.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_dot_file>\n", argv[0]);
        return 1;
    }

    ParseResult* result = parseString(argv[1]);

    if (result->errorCount > 0) {
        for (int i = 0; i < result->errorCount; i++) {
            printf("Error: %s\n", result->errors[i]->message);
        }
        return 0;
    }

    if (result->tree != NULL) {
        printf("Tree generated successfully.\n");
        if(writeAstAsDot(result, argv[2])){
            printf("Dot file generated: %s\n", argv[2]);
        } else {
            printf("Failed to create dot file.\n");
            return 0;
        }
    } else {
        printf("Failed to generate tree.\n");
        return 0;
    }

    //CFG
    ControlFlowGraph* cfg = createControlFlowGraph(result->tree);

    if(cfg->errors->head != NULL){
        for (size_t i = 0; i < cfg->errors->count; i++) {
            printf("%s\n",cfg->errors->head->message);
        }
    }

    if(cfg->arguments) {
        for (size_t i = 0; i < cfg->argumentCount; i++) {
            printf("Variable: %s, Type: ", cfg->arguments[i]->name);
            if (cfg->arguments[i]->type == NULL) {
                printf("%s\n", "undefined");
            } else {
                printf("%s\n", cfg->arguments[i]->type);
            }
        }
    }

    writeCfgAsDot(cfg, "control_flow_graph.dot");

    destroyControlFlowGraph(cfg);
    freeParseResult(result);

    printf("Finishing programm\n");

    return 0;
}

