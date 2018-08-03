/*
 * ReflectionAnalyzer.cpp
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2017 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ReflectionAnalyzer.h"
#include "ExprEvaluator.h"
#include "AST.h"
#include "Helper.h"
#include "ReportIdents.h"


namespace Xsc
{


ReflectionAnalyzer::ReflectionAnalyzer(Log* log) :
    reportHandler_ { log }
{
}

void ReflectionAnalyzer::Reflect(
    Program& program, const ShaderTarget shaderTarget, Reflection::ReflectionData& reflectionData, bool enableWarnings)
{
    /* Copy parameters */
    shaderTarget_   = shaderTarget;
    program_        = (&program);
    data_           = (&reflectionData);
    enableWarnings_ = enableWarnings;

    /* Visit program AST */
    Visit(program_);
}


/*
 * ======= Private: =======
 */

void ReflectionAnalyzer::Warning(const std::string& msg, const AST* ast)
{
    if (enableWarnings_)
        reportHandler_.Warning(false, msg, program_->sourceCode.get(), (ast ? ast->area : SourceArea::ignore));
}

int ReflectionAnalyzer::GetBindingPoint(const std::vector<RegisterPtr>& slotRegisters) const
{
    if (auto slotRegister = Register::GetForTarget(slotRegisters, shaderTarget_))
        return slotRegister->slot;
    else
        return -1;
}

int ReflectionAnalyzer::EvaluateConstExprInt(Expr& expr)
{
    /* Evaluate expression and return as integer */
    ExprEvaluator exprEvaluator;
    return static_cast<int>(exprEvaluator.EvaluateOrDefault(expr, Variant::IntType(0)).ToInt());
}

float ReflectionAnalyzer::EvaluateConstExprFloat(Expr& expr)
{
    /* Evaluate expression and return as integer */
    ExprEvaluator exprEvaluator;
    return static_cast<float>(exprEvaluator.EvaluateOrDefault(expr, Variant::RealType(0.0)).ToReal());
}

/* ------- Visit functions ------- */

#define IMPLEMENT_VISIT_PROC(AST_NAME) \
    void ReflectionAnalyzer::Visit##AST_NAME(AST_NAME* ast, void* args)

IMPLEMENT_VISIT_PROC(Program)
{
    /* Visit both active and disabled code */
    Visit(ast->globalStmnts);
    Visit(ast->disabledAST);

    if (auto entryPoint = ast->entryPointRef)
    {
        /* Reflect input attributes */
        for (auto varDecl : entryPoint->inputSemantics.varDeclRefs)
            data_->inputAttributes.push_back({ varDecl->ident, varDecl->semantic.Index() });
        for (auto varDecl : entryPoint->inputSemantics.varDeclRefsSV)
            data_->inputAttributes.push_back({ varDecl->semantic.ToString(), varDecl->semantic.Index() });

        /* Reflect output attributes */
        for (auto varDecl : entryPoint->outputSemantics.varDeclRefs)
            data_->outputAttributes.push_back({ varDecl->ident, varDecl->semantic.Index() });
        for (auto varDecl : entryPoint->outputSemantics.varDeclRefsSV)
            data_->outputAttributes.push_back({ varDecl->semantic.ToString(), varDecl->semantic.Index() });

        if (entryPoint->semantic.IsSystemValue())
            data_->outputAttributes.push_back({ entryPoint->semantic.ToString(), entryPoint->semantic.Index() });
    }
}

/* --- Declarations --- */

IMPLEMENT_VISIT_PROC(SamplerDecl)
{
    if (ast->samplerValues.empty())
    {
        /* Reflect sampler state */
        Reflection::SamplerState samplerState;
        {
            samplerState.type = SamplerTypeToResourceType(ast->GetSamplerType());
            samplerState.name = ast->ident;
            samplerState.slot = GetBindingPoint(ast->slotRegisters);
        }
        data_->samplerStates.push_back(samplerState);
    }
    else
    {
        /* Reflect static sampler state */
        Reflection::StaticSamplerState samplerState;
        {
            samplerState.type = SamplerTypeToResourceType(ast->GetSamplerType());
            samplerState.name = ast->ident;
            for (auto& value : ast->samplerValues)
                ReflectSamplerValue(value.get(), samplerState.desc);
        }
        data_->staticSamplerStates.push_back(samplerState);
    }
}

/* --- Declaration statements --- */

IMPLEMENT_VISIT_PROC(FunctionDecl)
{
    if (ast->flags(FunctionDecl::isEntryPoint))
        ReflectAttributes(ast->declStmntRef->attribs);

    Visitor::VisitFunctionDecl(ast, args);
}

IMPLEMENT_VISIT_PROC(UniformBufferDecl)
{
    if (ast->flags(AST::isReachable))
    {
        /* Reflect constant buffer binding */
        Reflection::ConstantBuffer constantBuffer;
        {
            constantBuffer.type = UniformBufferTypeToResourceType(ast->bufferType);
            constantBuffer.name = ast->ident;
            constantBuffer.slot = GetBindingPoint(ast->slotRegisters);
            if (!ast->AccumAlignedVectorSize(constantBuffer.size, constantBuffer.padding))
                constantBuffer.size = ~0;
        }
        data_->constantBuffers.push_back(constantBuffer);
    }
}

IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
{
    if (ast->flags(AST::isReachable))
    {
        for (auto& bufferDecl : ast->bufferDecls)
        {
            if (bufferDecl->flags(AST::isReachable))
            {
                /* Reflect texture or storage-buffer binding */
                Reflection::Resource resource;
                {
                    resource.type = BufferTypeToResourceType(ast->typeDenoter->bufferType);
                    resource.name = bufferDecl->ident;
                    resource.slot = GetBindingPoint(bufferDecl->slotRegisters);
                };
                data_->resources.push_back(resource);
            }
        }
    }
}

IMPLEMENT_VISIT_PROC(VarDecl)
{
    if (ast->flags(AST::isReachable))
    {
        if (auto typeSpecifier = ast->FetchTypeSpecifier())
        {
            if (typeSpecifier->isUniform)
            {
                /* Add variable as uniform */
                Reflection::Attribute attribute;
                {
                    attribute.name = ast->ident;
                    attribute.slot = GetBindingPoint(ast->slotRegisters);
                }
                data_->uniforms.push_back(attribute);
            }
        }
    }
}

#undef IMPLEMENT_VISIT_PROC

/* --- Helper functions for code reflection --- */

void ReflectionAnalyzer::ReflectSamplerValue(SamplerValue* ast, Reflection::SamplerStateDesc& desc)
{
    const auto& name = ast->name;

    /* Assign value to sampler state */
    if (auto literalExpr = ast->value->As<LiteralExpr>())
    {
        const auto& value = literalExpr->value;

        if (name == "MipLODBias")
            desc.mipLODBias = FromStringOrDefault<float>(value);
        else if (name == "MaxAnisotropy")
            desc.maxAnisotropy = static_cast<unsigned int>(FromStringOrDefault<unsigned long>(value));
        else if (name == "MinLOD")
            desc.minLOD = FromStringOrDefault<float>(value);
        else if (name == "MaxLOD")
            desc.maxLOD = FromStringOrDefault<float>(value);
    }
    else if (auto objectExpr = ast->value->As<ObjectExpr>())
    {
        const auto& value = objectExpr->ident;

        if (name == "Filter")
            ReflectSamplerValueFilter(value, desc.filter, ast);
        else if (name == "AddressU")
            ReflectSamplerValueTextureAddressMode(value, desc.addressU, ast);
        else if (name == "AddressV")
            ReflectSamplerValueTextureAddressMode(value, desc.addressV, ast);
        else if (name == "AddressW")
            ReflectSamplerValueTextureAddressMode(value, desc.addressW, ast);
        else if (name == "ComparisonFunc")
            ReflectSamplerValueComparisonFunc(value, desc.comparisonFunc, ast);
    }
    else if (name == "BorderColor")
    {
        try
        {
            if (auto callExpr = ast->value->As<CallExpr>())
            {
                if (callExpr->typeDenoter && callExpr->typeDenoter->IsVector() && callExpr->arguments.size() == 4)
                {
                    /* Evaluate sub expressions to constant floats */
                    for (std::size_t i = 0; i < 4; ++i)
                        desc.borderColor[i] = EvaluateConstExprFloat(*callExpr->arguments[i]);
                }
                else
                    throw std::string(R_InvalidTypeOrArgCount);
            }
            else if (auto castExpr = ast->value->As<CastExpr>())
            {
                /* Evaluate sub expression to constant float and copy into four sub values */
                auto subValueSrc = EvaluateConstExprFloat(*castExpr->expr);
                for (std::size_t i = 0; i < 4; ++i)
                    desc.borderColor[i] = subValueSrc;
            }
            else if (auto initExpr = ast->value->As<InitializerExpr>())
            {
                if (initExpr->exprs.size() == 4)
                {
                    /* Evaluate sub expressions to constant floats */
                    for (std::size_t i = 0; i < 4; ++i)
                        desc.borderColor[i] = EvaluateConstExprFloat(*initExpr->exprs[i]);
                }
                else
                    throw std::string(R_InvalidArgCount);
            }
        }
        catch (const std::string& s)
        {
            Warning(R_FailedToInitializeSamplerValue(s, "BorderColor"), ast->value.get());
        }
    }
}

void ReflectionAnalyzer::ReflectSamplerValueFilter(const std::string& value, Reflection::Filter& filter, const AST* ast)
{
    try
    {
        filter = StringToFilter(value);
    }
    catch (const std::invalid_argument& e)
    {
        Warning(e.what(), ast);
    }
}

void ReflectionAnalyzer::ReflectSamplerValueTextureAddressMode(const std::string& value, Reflection::TextureAddressMode& addressMode, const AST* ast)
{
    try
    {
        addressMode = StringToTexAddressMode(value);
    }
    catch (const std::invalid_argument& e)
    {
        Warning(e.what(), ast);
    }
}

void ReflectionAnalyzer::ReflectSamplerValueComparisonFunc(const std::string& value, Reflection::ComparisonFunc& comparisonFunc, const AST* ast)
{
    try
    {
        comparisonFunc = StringToCompareFunc(value);
    }
    catch (const std::invalid_argument& e)
    {
        Warning(e.what(), ast);
    }
}

void ReflectionAnalyzer::ReflectAttributes(const std::vector<AttributePtr>& attribs)
{
    for (const auto& attr : attribs)
    {
        switch (attr->attributeType)
        {
            case AttributeType::NumThreads:
                ReflectAttributesNumThreads(attr.get());
                break;
            default:
                break;
        }
    }
}

void ReflectionAnalyzer::ReflectAttributesNumThreads(Attribute* ast)
{
    /* Reflect "numthreads" attribute for compute shader */
    if (shaderTarget_ == ShaderTarget::ComputeShader && ast->arguments.size() == 3)
    {
        /* Evaluate attribute arguments */
        data_->numThreads.x = EvaluateConstExprInt(*ast->arguments[0]);
        data_->numThreads.y = EvaluateConstExprInt(*ast->arguments[1]);
        data_->numThreads.z = EvaluateConstExprInt(*ast->arguments[2]);
    }
}


} // /namespace Xsc



// ================================================================================