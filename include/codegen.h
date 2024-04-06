#pragma once

#include "stmt.h"

#include <llvm-c/Types.h>

typedef struct LLVMBackend {
    Stmt *stmts;
    int current;

    LLVMContextRef context;
    LLVMModuleRef module;
    LLVMBuilderRef builder;

    int (*generate_IR)(struct LLVMBackend *);
} LLVMBackend;

LLVMBackend backend_new(Stmt *);
void free_backend(LLVMBackend);
