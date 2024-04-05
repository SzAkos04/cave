#include "codegen.h"
#include "debug.h"
#include "parser.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int generate_STMT_IR(Stmt stmt, LLVMModuleRef module,
                            LLVMBuilderRef builder);

// returns 0 if successful, 1 otherwise
static int generate_FN_IR(Stmt stmt, LLVMModuleRef module,
                          LLVMBuilderRef builder) {

    if (LLVMGetNamedFunction(module, stmt.data.Fn.name)) {
        char error_msg[64];
        sprintf(error_msg, "function `%s` redefined", stmt.data.Fn.name);
        error(error_msg);
        return 1;
    }

    LLVMTypeRef ret_type;
    if (strcmp(stmt.data.Fn.name, "main") == 0) {
        ret_type = LLVMInt32Type();
    } else {
        ret_type = LLVMVoidType();
    }

    LLVMTypeRef *arg_type = NULL;
    unsigned int arg_num = 0;

    LLVMTypeRef func_type = LLVMFunctionType(ret_type, arg_type, arg_num, 0);

    LLVMValueRef func = LLVMAddFunction(module, stmt.data.Fn.name, func_type);

    if (strcmp(stmt.data.Fn.name, "main") == 0) {
        LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(func, "entry");
        LLVMPositionBuilderAtEnd(builder, entry_block);
        int ret_val = 0;

        // function body
        for (int i = 0; i < stmt.data.Fn.stmt_n; i++) {
            // recursive
            if (generate_STMT_IR(stmt.data.Fn.stmts[i], module, builder) != 0) {
                return 1;
            }
        }

        LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), ret_val, 0));
    }

    return 0;
}

// returns 0 if successful, 1 otherwise
static int generate_STMT_IR(Stmt stmt, LLVMModuleRef module,
                            LLVMBuilderRef builder) {
    switch (stmt.type) {
    case STMT_FN:
        return generate_FN_IR(stmt, module, builder);
    default:
        error("other statements are not yet implemented");
        return 1;
    }

    return 0;
}

// returns 0 if successful, 1 otherwise
static int generate_IR(LLVMBackend *self) {
    while (self->stmts[self->current].type != STMT_EOF) {
        if (generate_STMT_IR(self->stmts[self->current], self->module,
                             self->builder) != 0) {
            return 1;
        }
        self->current++;
    }

    /* LLVMDumpModule(self->module); */

    char *error = NULL;
    if (LLVMVerifyModule(self->module, LLVMReturnStatusAction, &error)) {
        error(error);
        LLVMDisposeMessage(error);
        return 1;
    };
    LLVMDisposeMessage(error);

    return 0;
}

LLVMBackend backend_new(Stmt *stmts) {
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithName("main");
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
