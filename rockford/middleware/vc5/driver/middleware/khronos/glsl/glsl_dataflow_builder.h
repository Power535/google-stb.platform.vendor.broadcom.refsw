/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.

Project  :  glsl
Module   :

FILE DESCRIPTION
=============================================================================*/

#ifndef GLSL_DATAFLOW_BUILDER_H
#define GLSL_DATAFLOW_BUILDER_H

#include "glsl_ast.h"
#include "glsl_basic_block.h"

//
// AST to dataflow conversion.
// Calculates the Dataflow graphs for all the numeric and boolean variables in ast.
//

void glsl_expr_calculate_function_call_args(BasicBlock* ctx, Symbol *function, ExprChain* args);

// Prototype for the function that brings dataflow calculation together, so that the individual functions can recurse.
void glsl_expr_calculate_dataflow(BasicBlock* ctx, Dataflow **scalar_values, Expr *expr);

#endif // DATAFLOW_BUILDER_H
