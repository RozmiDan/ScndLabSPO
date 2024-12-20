/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : MyGrammarTest.g
 *     -                            On : 2024-11-19 22:42:43
 *     -                for the parser : MyGrammarTestParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
MyGrammarTestParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pMyGrammarTestParser, which is returned from a call to MyGrammarTestParserNew().
 *
 * The methods in pMyGrammarTestParser are  as follows:
 *
 *  - 
 MyGrammarTestParser_source_return
      pMyGrammarTestParser->source(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_sourceItem_return
      pMyGrammarTestParser->sourceItem(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_funcDef_return
      pMyGrammarTestParser->funcDef(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_body_return
      pMyGrammarTestParser->body(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_list_identifier_return
      pMyGrammarTestParser->list_identifier(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_typeRef_return
      pMyGrammarTestParser->typeRef(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_builtin_return
      pMyGrammarTestParser->builtin(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_custom_return
      pMyGrammarTestParser->custom(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_arrayType_return
      pMyGrammarTestParser->arrayType(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_elems_return
      pMyGrammarTestParser->elems(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_funcSignature_return
      pMyGrammarTestParser->funcSignature(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_list_argDef_return
      pMyGrammarTestParser->list_argDef(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_argDef_return
      pMyGrammarTestParser->argDef(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_typeRefDef_return
      pMyGrammarTestParser->typeRefDef(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_statement_return
      pMyGrammarTestParser->statement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_ifStatement_return
      pMyGrammarTestParser->ifStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_blockStatement_return
      pMyGrammarTestParser->blockStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_whileStatement_return
      pMyGrammarTestParser->whileStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_doStatement_return
      pMyGrammarTestParser->doStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_breakStatement_return
      pMyGrammarTestParser->breakStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_expressionStatement_return
      pMyGrammarTestParser->expressionStatement(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_expr_return
      pMyGrammarTestParser->expr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_assignExpr_return
      pMyGrammarTestParser->assignExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_logicOrExpr_return
      pMyGrammarTestParser->logicOrExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_logicAndExpr_return
      pMyGrammarTestParser->logicAndExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_inclusOrExpr_return
      pMyGrammarTestParser->inclusOrExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_xorExpr_return
      pMyGrammarTestParser->xorExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_andExpr_return
      pMyGrammarTestParser->andExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_equalExpr_return
      pMyGrammarTestParser->equalExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_relatExpr_return
      pMyGrammarTestParser->relatExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_shiftExpr_return
      pMyGrammarTestParser->shiftExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_addExpr_return
      pMyGrammarTestParser->addExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_multExpr_return
      pMyGrammarTestParser->multExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_unaryExpr_return
      pMyGrammarTestParser->unaryExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_primaryExpr_return
      pMyGrammarTestParser->primaryExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_sufExpr_return
      pMyGrammarTestParser->sufExpr(pMyGrammarTestParser)
 *  - 
 MyGrammarTestParser_baseExpr_return
      pMyGrammarTestParser->baseExpr(pMyGrammarTestParser)
 * 
 * 
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_MyGrammarTestParser_H
#define _MyGrammarTestParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct MyGrammarTestParser_Ctx_struct MyGrammarTestParser, * pMyGrammarTestParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

typedef struct MyGrammarTestParser_source_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_source_return;



typedef struct MyGrammarTestParser_sourceItem_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_sourceItem_return;



typedef struct MyGrammarTestParser_funcDef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_funcDef_return;



typedef struct MyGrammarTestParser_body_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_body_return;



typedef struct MyGrammarTestParser_list_identifier_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_list_identifier_return;



typedef struct MyGrammarTestParser_typeRef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_typeRef_return;



typedef struct MyGrammarTestParser_builtin_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_builtin_return;



typedef struct MyGrammarTestParser_custom_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_custom_return;



typedef struct MyGrammarTestParser_arrayType_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_arrayType_return;



typedef struct MyGrammarTestParser_elems_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_elems_return;



typedef struct MyGrammarTestParser_funcSignature_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_funcSignature_return;



typedef struct MyGrammarTestParser_list_argDef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_list_argDef_return;



typedef struct MyGrammarTestParser_argDef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_argDef_return;



typedef struct MyGrammarTestParser_typeRefDef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_typeRefDef_return;



typedef struct MyGrammarTestParser_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_statement_return;



typedef struct MyGrammarTestParser_ifStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_ifStatement_return;



typedef struct MyGrammarTestParser_blockStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_blockStatement_return;



typedef struct MyGrammarTestParser_whileStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_whileStatement_return;



typedef struct MyGrammarTestParser_doStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_doStatement_return;



typedef struct MyGrammarTestParser_breakStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_breakStatement_return;



typedef struct MyGrammarTestParser_expressionStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_expressionStatement_return;



typedef struct MyGrammarTestParser_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_expr_return;



typedef struct MyGrammarTestParser_assignExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_assignExpr_return;



typedef struct MyGrammarTestParser_logicOrExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_logicOrExpr_return;



typedef struct MyGrammarTestParser_logicAndExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_logicAndExpr_return;



typedef struct MyGrammarTestParser_inclusOrExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_inclusOrExpr_return;



typedef struct MyGrammarTestParser_xorExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_xorExpr_return;



typedef struct MyGrammarTestParser_andExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_andExpr_return;



typedef struct MyGrammarTestParser_equalExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_equalExpr_return;



typedef struct MyGrammarTestParser_relatExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_relatExpr_return;



typedef struct MyGrammarTestParser_shiftExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_shiftExpr_return;



typedef struct MyGrammarTestParser_addExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_addExpr_return;



typedef struct MyGrammarTestParser_multExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_multExpr_return;



typedef struct MyGrammarTestParser_unaryExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_unaryExpr_return;



typedef struct MyGrammarTestParser_primaryExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_primaryExpr_return;



typedef struct MyGrammarTestParser_sufExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_sufExpr_return;



typedef struct MyGrammarTestParser_baseExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MyGrammarTestParser_baseExpr_return;




/** Context tracking structure for 
MyGrammarTestParser

 */
struct MyGrammarTestParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     MyGrammarTestParser_source_return
     (*source)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_sourceItem_return
     (*sourceItem)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_funcDef_return
     (*funcDef)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_body_return
     (*body)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_list_identifier_return
     (*list_identifier)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_typeRef_return
     (*typeRef)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_builtin_return
     (*builtin)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_custom_return
     (*custom)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_arrayType_return
     (*arrayType)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_elems_return
     (*elems)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_funcSignature_return
     (*funcSignature)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_list_argDef_return
     (*list_argDef)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_argDef_return
     (*argDef)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_typeRefDef_return
     (*typeRefDef)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_statement_return
     (*statement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_ifStatement_return
     (*ifStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_blockStatement_return
     (*blockStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_whileStatement_return
     (*whileStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_doStatement_return
     (*doStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_breakStatement_return
     (*breakStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_expressionStatement_return
     (*expressionStatement)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_expr_return
     (*expr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_assignExpr_return
     (*assignExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_logicOrExpr_return
     (*logicOrExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_logicAndExpr_return
     (*logicAndExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_inclusOrExpr_return
     (*inclusOrExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_xorExpr_return
     (*xorExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_andExpr_return
     (*andExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_equalExpr_return
     (*equalExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_relatExpr_return
     (*relatExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_shiftExpr_return
     (*shiftExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_addExpr_return
     (*addExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_multExpr_return
     (*multExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_unaryExpr_return
     (*unaryExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_primaryExpr_return
     (*primaryExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_sufExpr_return
     (*sufExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     MyGrammarTestParser_baseExpr_return
     (*baseExpr)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred27_MyGrammarTest)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred54_MyGrammarTest)	(struct MyGrammarTestParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred58_MyGrammarTest)	(struct MyGrammarTestParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct MyGrammarTestParser_Ctx_struct * ctx);
    void	    (*free)   (struct MyGrammarTestParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pMyGrammarTestParser MyGrammarTestParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pMyGrammarTestParser MyGrammarTestParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__50      50
#define T__51      51
#define T__52      52
#define T__53      53
#define T__54      54
#define T__55      55
#define T__56      56
#define T__57      57
#define T__58      58
#define T__59      59
#define T__60      60
#define T__61      61
#define T__62      62
#define T__63      63
#define T__64      64
#define T__65      65
#define T__66      66
#define T__67      67
#define T__68      68
#define T__69      69
#define T__70      70
#define T__71      71
#define T__72      72
#define T__73      73
#define T__74      74
#define T__75      75
#define T__76      76
#define T__77      77
#define T__78      78
#define T__79      79
#define T__80      80
#define T__81      81
#define T__82      82
#define T__83      83
#define T__84      84
#define T__85      85
#define T__86      86
#define T__87      87
#define T__88      88
#define T__89      89
#define T__90      90
#define T__91      91
#define T__92      92
#define T__93      93
#define T__94      94
#define T__95      95
#define T__96      96
#define T__97      97
#define T__98      98
#define Args      4
#define Argument      5
#define ArrayAccess      6
#define ArrayType      7
#define BITS_LITERAL      8
#define BOOL_LITERAL      9
#define BlockStatement      10
#define BodySig      11
#define Braces      12
#define BreakStatement      13
#define BuiltinType      14
#define CHAR      15
#define CallExpr      16
#define Condition      17
#define CustomType      18
#define DEC_LITERAL      19
#define DoStatement      20
#define ESC_SEQ      21
#define Elems      22
#define Else      23
#define Expression      24
#define ExpressionStatement      25
#define FuncDef      26
#define FuncSignature      27
#define FunctionCall      28
#define HEX_DIGIT      29
#define HEX_LITERAL      30
#define IDENTIFIER      31
#define Identifier      32
#define IfStatement      33
#define Indexer      34
#define Indices      35
#define LINE_COMMENT      36
#define LITERAL      37
#define ListArgDefs      38
#define ListExpr      39
#define ListIdentifier      40
#define Literal      41
#define STRING_LIT      42
#define Source      43
#define SourceItem      44
#define Then      45
#define TypeRef      46
#define VarsDefenition      47
#define WS      48
#define WhileStatement      49
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for MyGrammarTestParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
