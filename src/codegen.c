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

static int generate_FN_IR(Stmt stmt, LLVMModuleRef module,
                          LLVMBuilderRef builder) {
    LLVMValueRef func = LLVMGetNamedFunction(module, stmt.data.Fn.name);

    if (!func) {
        LLVMTypeRef ret_type;
        if (strcmp(stmt.data.Fn.name, "main") == 0) {
            ret_type = LLVMInt32Type();
        } else {
            ret_type = LLVMVoidType();
        }

        LLVMTypeRef *arg_type = NULL;
        unsigned int arg_num = 0;

        LLVMTypeRef func_type =
            LLVMFunctionType(ret_type, arg_type, arg_num, 0);

        func = LLVMAddFunction(module, stmt.data.Fn.name, func_type);
    }

    if (strcmp(stmt.data.Fn.name, "main") == 0) {
        LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(func, "entry");
        LLVMPositionBuilderAtEnd(builder, entry_block);
        int ret_val = 0;

        // function body
        for (int i = 0; i < stmt.data.Fn.stmt_n; i++) {
            // recursive
            generate_STMT_IR(stmt.data.Fn.stmts[i], module, builder);
        }

        LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), ret_val, 0));
    }

    return 0;
}

static int generate_STMT_IR(Stmt stmt, LLVMModuleRef module,
                            LLVMBuilderRef builder) {
    switch (stmt.type) {
    case STMT_FN:
        if (generate_FN_IR(stmt, module, builder) != 0) {
            return 1;
        }
        break;
    default:
        error("other statements are not yet implemented");
        return 1;
    }

    return 0;
}

static int generate_IR(LLVMBackend *self) {
    while (self->stmts[self->current].type != STMT_EOF) {
        generate_STMT_IR(self->stmts[self->current], self->module,
                         self->builder);
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
