#pragma once

#include "parser.h"
#include <llvm-c/Types.h>

typedef struct LLVMBackend {
    Stmt *stmts;
    int current;

    LLVMContextRef context;
    LLVMModuleRef module;
    LLVMBuilderRef builder;

    void (*generate_IR)(struct LLVMBackend *);
} LLVMBackend;

LLVMBackend backend_new(Stmt *);
void free_backend(LLVMBackend);
