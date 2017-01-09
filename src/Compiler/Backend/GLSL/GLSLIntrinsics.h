/*
 * GLSLIntrinsics.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_GLSL_INTRINSICS_H
#define XSC_GLSL_INTRINSICS_H


#include "Token.h"
#include "ASTEnums.h"
#include <string>
#include <set>


namespace Xsc
{


// Returns GLSL keyword for the specified intrinsic.
const std::string* IntrinsicToGLSLKeyword(const Intrinsic intr);

// Returns the set of all reserved GLSL names.
const std::set<std::string>& ReservedGLSLNames();


} // /namespace Xsc


#endif



// ================================================================================