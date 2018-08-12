/*
 * ASTFactory.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2017 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_AST_FACTORY_H
#define XSC_AST_FACTORY_H


#include "AST.h"
#include "TypeDenoter.h"


namespace Xsc
{

class Variant;

namespace ASTFactory
{


/* ----- Make functions ----- */

CallExprPtr                     MakeIntrinsicCallExpr(
    const Intrinsic intrinsic, const std::string& ident,
    const TypeDenoterPtr& typeDenoter, const std::vector<ExprPtr>& arguments
);

CallExprPtr                     MakeTextureSamplerBindingCallExpr(const ExprPtr& textureObjectExpr, const ExprPtr& samplerObjectExpr);

// Makes a call expression to a wrapper function.
CallExprPtr                     MakeWrapperCallExpr(const std::string& funcIdent, const TypeDenoterPtr& typeDenoter, const std::vector<ExprPtr>& arguments);

InitializerExprPtr              MakeInitializerExpr(const std::vector<ExprPtr>& exprs);

// Makes a type constructor function call.
CallExprPtr                     MakeTypeCtorCallExpr(const TypeDenoterPtr& typeDenoter, const std::vector<ExprPtr>& arguments);

CastExprPtr                     MakeCastExpr(const TypeDenoterPtr& typeDenoter, const ExprPtr& valueExpr);
CastExprPtr                     MakeLiteralCastExpr(const TypeDenoterPtr& typeDenoter, const DataType literalType, const std::string& literalValue);

BinaryExprPtr                   MakeBinaryExpr(const ExprPtr& lhsExpr, const BinaryOp op, const ExprPtr& rhsExpr);

// Makes a new LiteralExpr of the specified data type and literal value.
LiteralExprPtr                  MakeLiteralExpr(const DataType literalType, const std::string& literalValue);

// Makes a new LiteralExpr if the specified variant is either a boolean, integral, or real type. Otherwise, null is returned.
LiteralExprPtr                  MakeLiteralExprOrNull(const Variant& literalValue);

AliasDeclStmntPtr               MakeBaseTypeAlias(const DataType dataType, const std::string& ident);

TypeSpecifierPtr                MakeTypeSpecifier(const StructDeclPtr& structDecl);
TypeSpecifierPtr                MakeTypeSpecifier(const TypeDenoterPtr& typeDenoter);
TypeSpecifierPtr                MakeTypeSpecifier(const DataType dataType);

VarDeclStmntPtr                 MakeVarDeclStmnt(const TypeSpecifierPtr& typeSpecifier, const std::string& ident, const ExprPtr& initializer = nullptr);
VarDeclStmntPtr                 MakeVarDeclStmnt(const DataType dataType, const std::string& ident, const ExprPtr& initializer = nullptr);

// Returns a new VarDeclStmnt with the specified VarDecl index and removes the specified VarDecl from the input statement, except there is only one VarDecl.
VarDeclStmntPtr                 MakeVarDeclStmntSplit(const VarDeclStmntPtr& varDeclStmnt, std::size_t idx);

ObjectExprPtr                   MakeObjectExpr(const ExprPtr& prefixExpr, const std::string& ident, Decl* symbolRef = nullptr);
ObjectExprPtr                   MakeObjectExpr(const std::string& ident, Decl* symbolRef = nullptr);
ObjectExprPtr                   MakeObjectExpr(Decl* symbolRef);

ArrayExprPtr                    MakeArrayExpr(const ExprPtr& prefixExpr, std::vector<ExprPtr>&& arrayIndices);
ArrayExprPtr                    MakeArrayExpr(const ExprPtr& prefixExpr, const std::vector<int>& arrayIndices);
ArrayExprPtr                    MakeArrayExpr(
                                    const ExprPtr&                              prefixExpr,
                                    const std::vector<ExprPtr>::const_iterator& arrayIndicesBegin,
                                    const std::vector<ExprPtr>::const_iterator& arrayIndicesEnd
                                );

/*
Splits the specified array expression at the specified array index location.
If 'lastPrefixArrayIndex' is zero, or greater than or equal to the number of array indices, the input expression 'arrayExpr' is returned.
Otherwise, the left hand side is splitted as prefix expression into the return expression.
Example (pseudocode): MakeArrayExprSplit('prefix[0][1][2]', 2) --> '(prefix[0][1])[2]'
*/
ArrayExprPtr                    MakeArrayExprSplit(const ArrayExprPtr& arrayExpr, std::size_t splitArrayIndex);

RegisterPtr                     MakeRegister(int slot, const RegisterType registerType = RegisterType::Undefined);

// Makes a new bracket expression with the specified sub expression (source area is copied).
BracketExprPtr                  MakeBracketExpr(const ExprPtr& expr);

// Return a list expression (or only the input expression) for the specified literal expression, so it can be used as constructor for a struct.
ExprPtr                         MakeConstructorListExpr(const ExprPtr& expr, const std::vector<TypeDenoterPtr>& listTypeDens);

// Makes an expression statement with an assignment expression.
ExprStmntPtr                    MakeAssignStmnt(const ExprPtr& lvalueExpr, const ExprPtr& rvalueExpr, const AssignOp op = AssignOp::Set);

// Makes an expression statement with an array element assignment for the specified variable, array indices, and assignment expression.
ExprStmntPtr                    MakeArrayAssignStmnt(VarDecl* varDecl, const std::vector<int>& arrayIndices, const ExprPtr& assignExpr);

ArrayDimensionPtr               MakeArrayDimension(int arraySize);

// Makes a code block statement with initial code block and the specified statement inserted.
CodeBlockStmntPtr               MakeCodeBlockStmnt(const StmntPtr& stmnt);

BasicDeclStmntPtr               MakeStructDeclStmnt(const StructDeclPtr& structDecl);

// Makes a uniform buffer declaration with the specified identifier.
UniformBufferDeclPtr            MakeUniformBufferDecl(
                                    const std::string&      ident,
                                    int                     bindingSlot = 0,
                                    const UniformBufferType bufferType  = UniformBufferType::ConstantBuffer
                                );

/* ----- Make list functions ----- */

std::vector<ExprPtr>            MakeArrayIndices(const std::vector<int>& arrayIndices);

std::vector<ArrayDimensionPtr>  MakeArrayDimensionList(const std::vector<int>& arraySizes);

/* ----- Convert functions ----- */

ExprPtr                         ConvertExprBaseType(const DataType dataType, const ExprPtr& subExpr);

ArrayDimensionPtr               ConvertExprToArrayDimension(const ExprPtr& expr);

std::vector<ArrayDimensionPtr>  ConvertExprListToArrayDimensionList(const std::vector<ExprPtr>& exprs);


} // /namespace ASTFactory

} // /namespace Xsc


#endif



// ================================================================================
