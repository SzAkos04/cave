#include "codegen.h"
#include "debug.h"
#include "parser.h"

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void generate_FN_IR(LLVMBackend *self) {
    Stmt stmt = self->stmts[self->current];
    LLVMTypeRef func_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
    LLVMValueRef func =
        LLVMAddFunction(self->module, stmt.data.Fn.name, func_type);

    if (strcmp(stmt.data.Fn.name, "main") == 0) {
        LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(func, "entry");
        LLVMPositionBuilderAtEnd(self->builder, entry_block);
        LLVMBuildRet(self->builder, LLVMConstInt(LLVMInt32Type(), 0, 0));
    } else {
        error("not yet implemented");
    }
}

static void generate_IR(LLVMBackend *self) {
    while (self->stmts[self->current].type != STMT_EOF) {
        switch (self->stmts[self->current].type) {
        case STMT_FN:
            generate_FN_IR(self);
            break;
        default:
            error("not yet implemented");
            break;
        }
        self->current++;
    }
}

LLVMBackend backend_new(Stmt *stmts) {
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithNameInContext("cave", context);
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
    return (LLVMBackend){
        .stmts = stmts,
        .current = 0,

        .context = context,
        .module = module,
        .builder = builder,

        .generate_IR = generate_IR,
    };
}

void free_backend(LLVMBackend backend) {
    LLVMDisposeBuilder(backend.builder);
    LLVMDisposeModule(backend.module);
    LLVMContextDispose(backend.context);
}
