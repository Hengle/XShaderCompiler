/*
 * GLSLConverter.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2018 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_GLSL_CONVERTER_H
#define XSC_GLSL_CONVERTER_H


#include "Converter.h"
#include <functional>
#include <set>


namespace Xsc
{


/*
GLSL AST converter.
This class modifies the AST after context analysis to be conform with GLSL,
e.g. remove arguments from intrinsic calls, that are not allowed in GLSL, such as sampler state objects.
*/
class GLSLConverter : public Converter
{

    public:

        static bool ConvertVarDeclType(VarDecl& varDecl);
        static bool ConvertVarDeclBaseTypeDenoter(VarDecl& varDecl, const DataType dataType);

    private:

        void ConvertASTPrimary(
            Program&            program,
            const ShaderInput&  inputDesc,
            const ShaderOutput& outputDesc
        ) override;

        /* ----- Functions ----- */

        // Returns true if the output shader version is VKSL (GLSL for Vulkan).
        bool IsVKSL() const;

        // Returns true if separate objects for samplers & textures should be used.
        bool UseSeparateSamplers() const;

        /* ----- Visitor implementation ----- */

        DECL_VISIT_PROC( Program           );
        DECL_VISIT_PROC( CodeBlock         );
        DECL_VISIT_PROC( SwitchCase        );
        DECL_VISIT_PROC( TypeSpecifier     );

        DECL_VISIT_PROC( VarDecl           );
        DECL_VISIT_PROC( BufferDecl        );
        DECL_VISIT_PROC( SamplerDecl       );
        DECL_VISIT_PROC( StructDecl        );

        DECL_VISIT_PROC( FunctionDecl      );
        DECL_VISIT_PROC( UniformBufferDecl );
        DECL_VISIT_PROC( VarDeclStmt       );
        DECL_VISIT_PROC( AliasDeclStmt     );

        DECL_VISIT_PROC( CodeBlockStmt     );
        DECL_VISIT_PROC( ForLoopStmt       );
        DECL_VISIT_PROC( WhileLoopStmt     );
        DECL_VISIT_PROC( DoWhileLoopStmt   );
        DECL_VISIT_PROC( IfStmt            );
        DECL_VISIT_PROC( SwitchStmt        );
        DECL_VISIT_PROC( ReturnStmt        );

        DECL_VISIT_PROC( CastExpr          );
        DECL_VISIT_PROC( CallExpr          );
        DECL_VISIT_PROC( ObjectExpr        );

        /* ----- Scope functions ----- */

        // Renames the identifier of the specified declaration object (if required) and registers its identifier.
        void RegisterDeclIdent(Decl* obj, bool global = false);

        // Registers the identifiers of all specified variables (see RegisterDeclIdent).
        void RegisterGlobalDeclIdents(const std::vector<VarDecl*>& varDecls);

        /* ----- Helper functions for conversion ----- */

        // Returns true if the specified variable declaration must be renamed.
        bool MustRenameDeclIdent(const Decl* obj) const;

        // Removes all variable declarations which have a sampler state type.
        void RemoveSamplerStateVarDeclStmts(std::vector<VarDeclStmtPtr>& stmts);

        // Renames the specified identifier if it equals a reserved GLSL intrinsic or function name.
        bool RenameReservedKeyword(Identifier& ident);

        /* ----- Function declaration ----- */

        void ConvertFunctionDecl(FunctionDecl* ast);
        void ConvertFunctionDeclDefault(FunctionDecl* ast);
        void ConvertFunctionDeclEntryPoint(FunctionDecl* ast);

        /* ----- Call expressions ----- */

        void ConvertIntrinsicCall(CallExpr* ast);
        void ConvertIntrinsicCallSaturate(CallExpr* ast);
        void ConvertIntrinsicCallTextureLOD(CallExpr* ast);
        void ConvertIntrinsicCallTextureSample(CallExpr* ast);
        void ConvertIntrinsicCallTextureSampleLevel(CallExpr* ast);
        void ConvertIntrinsicCallTextureLoad(CallExpr* ast);
        void ConvertIntrinsicCallImageAtomic(CallExpr* ast);
        void ConvertIntrinsicCallGather(CallExpr* ast);
        void ConvertIntrinsicCallSampleCmp(CallExpr* ast);
        void ConvertIntrisicCallF32toF16(CallExpr* ast);

        void ConvertFunctionCall(CallExpr* ast);

        /* ----- Entry point ----- */

        void ConvertEntryPointStructPrefix(ExprPtr& expr, ObjectExpr* objectExpr);
        void ConvertEntryPointStructPrefixObject(ExprPtr& expr, ObjectExpr* prefixExpr, ObjectExpr* objectExpr);
        void ConvertEntryPointStructPrefixArray(ExprPtr& expr, ArrayExpr* prefixExpr, ObjectExpr* objectExpr);

        void ConvertEntryPointReturnStmt(ReturnStmt& ast, StructDecl* structDecl, const TypeDenoterPtr& typeDen, const ExprPtr& typeConstructor);
        void ConvertEntryPointReturnStmtSequenceExpr(ReturnStmt& ast, StructDecl* structDecl, const TypeDenoterPtr& typeDen, const SequenceExpr& typeConstructor);
        void ConvertEntryPointReturnStmtCommonExpr(ReturnStmt& ast, StructDecl* structDecl, const TypeDenoterPtr& typeDen, const ExprPtr& typeConstructor);

        //TODO: this should be replaced by the code-injection functionality.
        void ConvertEntryPointReturnStmtToCodeBlock(StmtPtr& stmt);

        void AddMissingInterpModifiers(const std::vector<VarDecl*>& varDecls);

        /* ----- Object expressions ----- */

        void ConvertObjectExpr(ObjectExpr* objectExpr);
        void ConvertObjectExprStaticVar(ObjectExpr* objectExpr);
        void ConvertObjectExprDefault(ObjectExpr* objectExpr);

        void ConvertObjectPrefixStructMember(ExprPtr& prefixExpr, const StructDecl* ownerStructDecl, const StructDecl* callerStructDecl, bool useSelfParam);
        void ConvertObjectPrefixSelfParam(ExprPtr& prefixExpr, ObjectExpr* objectExpr);
        void ConvertObjectPrefixBaseStruct(ExprPtr& prefixExpr, ObjectExpr* objectExpr);
        void ConvertObjectPrefixNamespace(ExprPtr& prefixExpr, ObjectExpr* objectExpr);
        void ConvertObjectPrefixNamespaceStruct(ObjectExpr* prefixObjectExpr, ObjectExpr* objectExpr, const StructDecl* baseStructDecl, const StructDecl* activeStructDecl);

        void InsertBaseMemberPrefixes(ExprPtr& prefixExpr, const StructDecl* ownerStructDecl, const StructDecl* callerStructDecl);

        /* ----- Unrolling ----- */

        void UnrollStmts(std::vector<StmtPtr>& stmts);
        void UnrollStmtsVarDecl(std::vector<StmtPtr>& unrolledStmts, VarDeclStmt* ast);
        void UnrollStmtsVarDeclInitializer(std::vector<StmtPtr>& unrolledStmts, VarDecl* varDecl);

        /* ----- Misc ----- */

        void ConvertSlotRegisters(std::vector<RegisterPtr>& slotRegisters);

        /* === Members === */

        ShaderTarget                shaderTarget_       = ShaderTarget::VertexShader;
        OutputShaderVersion         versionOut_         = OutputShaderVersion::GLSL;

        Options                     options_;
        bool                        autoBinding_        = false;
        int                         autoBindingSlot_    = 0;
        bool                        separateSamplers_   = true;

        /*
        List of all variables with reserved identifiers that come from a structure that must be resolved.
        If a local variable uses a name from this list, it name must be modified with name mangling.
        */
        std::vector<const Decl*>    globalReservedDecls_;

};


} // /namespace Xsc


#endif



// ================================================================================