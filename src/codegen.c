#include "codegen.h"
#include "debug.h"
#include "expr.h"
#include "literal.h"
#include "stmt.h"
#include "token.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static LLVMValueRef generate_literal_IR(Literal literal,
                                        LLVMContextRef context) {
    switch (literal.type) {
    case LITERAL_STRING:
        return LLVMConstStringInContext(context, literal.data.Str,
                                        strlen(literal.data.Str), 1);
    case LITERAL_INTEGER:
        return LLVMConstInt(LLVMInt32Type(), literal.data.Integer, 1);
    case LITERAL_FLOAT:
        return LLVMConstReal(LLVMFloatType(), literal.data.Float);
    case LITERAL_BOOLEAN:
        return LLVMConstInt(LLVMInt1Type(), literal.data.Boolean ? 1 : 0, 0);
    case LITERAL_IDENTIFIER:
        error("not yet implemented");
        return NULL;
    case LITERAL_NULL: {
        LLVMTypeRef null_type = LLVMPointerType(LLVMInt32Type(), 0);
        return LLVMConstNull(null_type);
    }
    }
}

static LLVMTypeRef generate_type_IR(Token token) {
    switch (token.type) {
    case TT_VOID:
        return LLVMVoidType();
    case TT_I32:
        return LLVMInt32Type();
    default:
        error("unexpected type");
        return NULL;
    }
}

LLVMValueRef generate_expression_IR(Expr expr, LLVMContextRef context,
                                    LLVMModuleRef module) {
    switch (expr.type) {
    case EXPR_UNARY:
        error("unary expressions not yet implemented");
        return NULL;
    case EXPR_BINARY:
        error("binary expressions not yet implemented");
        return NULL;
    case EXPR_LITERAL:
        return generate_literal_IR(expr.data.Literal, context);
    case EXPR_VARIABLE:
        return LLVMGetInitializer(
            LLVMGetNamedGlobal(module, expr.data.Variable));
    case EXPR_ASSIGNMENT:
        error("assignment expressions not yet implemented");
        return NULL;
    }
}

static int generate_STMT_IR(Stmt stmt, LLVMContextRef context,
                            LLVMModuleRef module, LLVMBuilderRef builder);

static int generate_CONST_IR(Stmt stmt, LLVMContextRef context,
                             LLVMModuleRef module, LLVMBuilderRef builder) {
    LLVMValueRef value = NULL;
    switch (stmt.data.Const.type) {
    case CONST_I32: {
        value = generate_expression_IR(stmt.data.Const.value, context, module);
        LLVMValueRef global_var = LLVMAddGlobal(module, LLVMTypeOf(value),
                                                stmt.data.Const.name.data.Str);
        LLVMSetInitializer(global_var, value);
        LLVMSetLinkage(global_var, LLVMInternalLinkage);

        LLVMSetGlobalConstant(global_var, 1);
        LLVMSetAlignment(global_var, 4);

        break;
    }
    }
    (void)stmt;
    (void)context;
    (void)module;
    (void)builder;
    return 0;
}

// returns 0 if successful, 1 otherwise
static int generate_FN_IR(Stmt stmt, LLVMContextRef context,
                          LLVMModuleRef module, LLVMBuilderRef builder) {

    if (LLVMGetNamedFunction(module, stmt.data.Fn.name)) {
        char error_msg[64];
        sprintf(error_msg, "function `%s` redefined", stmt.data.Fn.name);
        error(error_msg);
        return 1;
    }

    LLVMTypeRef ret_type = generate_type_IR(stmt.data.Fn.ret_type);
    if (!ret_type) {
        return 1;
    }

    LLVMTypeRef *arg_type = NULL;
    unsigned int arg_num = 0;

    LLVMTypeRef func_type = LLVMFunctionType(ret_type, arg_type, arg_num, 0);
    if (!func_type) {
        error("`LLVMFunctionType` failed");
        return 1;
    }

    LLVMValueRef func = LLVMAddFunction(module, stmt.data.Fn.name, func_type);
    if (!func) {
        error("`LLVMAddFunction` failed");
        return 1;
    }

    if (strcmp(stmt.data.Fn.name, "main") == 0) {
        if (ret_type != LLVMInt32Type()) {
            error("main function should return an `i32`");
            return 1;
        }
        LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(func, "entry");
        if (!entry_block) {
            error("`LLVMAppendBasicBlock` failed");
            return 1;
        }
        LLVMPositionBuilderAtEnd(builder, entry_block);

        // function body
        for (int i = 0; i < stmt.data.Fn.stmt_n; i++) {
            // recursive
            if (generate_STMT_IR(stmt.data.Fn.stmts[i], context, module,
                                 builder) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

static int generate_RETURN_IR(Stmt stmt, LLVMContextRef context,
                              LLVMModuleRef module, LLVMBuilderRef builder) {

    LLVMValueRef ret_value =
        generate_expression_IR(stmt.data.Return.value, context, module);
    if (!ret_value) {
        return 1;
    }

    // get the current function being built
    LLVMValueRef current_func = LLVMGetLastFunction(module);
    if (!current_func || !LLVMIsAFunction(current_func)) {
        error("return statement found outside of a function");
        return 1;
    }

    // the return type is checked later, after codegen

    LLVMBuildRet(builder, ret_value);

    return 0;
}

// returns 0 if successful, 1 otherwise
static int generate_STMT_IR(Stmt stmt, LLVMContextRef context,
                            LLVMModuleRef module, LLVMBuilderRef builder) {
    switch (stmt.type) {
    case STMT_CONST:
        return generate_CONST_IR(stmt, context, module, builder);
    case STMT_FN:
        return generate_FN_IR(stmt, context, module, builder);
    case STMT_RETURN:
        return generate_RETURN_IR(stmt, context, module, builder);
    default:
        error("other statements are not yet implemented");
        return 1;
    }

    return 0;
}

// returns 0 if successful, 1 otherwise
static int generate_IR(LLVMBackend *self) {
    while (self->stmts[self->current].type != STMT_EOF) {
        if (generate_STMT_IR(self->stmts[self->current], self->context,
                             self->module, self->builder) != 0) {
            return 1;
        }
        self->current++;
    }

#if DEBUG
    LLVMDumpModule(self->module);
#endif

    char *error = NULL;
    if (LLVMVerifyModule(self->module, LLVMReturnStatusAction, &error)) {
        char error_msg[128];
        sprintf(error_msg, "LLVM error: \n%s", error);
        error(error_msg);
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
