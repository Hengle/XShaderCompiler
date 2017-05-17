/*
 * SourceViewLanguageHLSL.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2017 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_SOURCE_VIEW_LANGUAGE_HLSL_H
#define XSC_SOURCE_VIEW_LANGUAGE_HLSL_H


namespace Xsc
{


static const char* keywordsHLSL0 =
(
    "break continue if else switch case default return for while do typedef namespace true false compile " \
    "const void struct static extern register packoffset volatile inline target nointerpolation noperspective shared uniform " \
    "row_major column_major unroll loop flatten branch earlydepthstencil domain instance maxtessfactor " \
    "outputcontrolpoints outputtopology partitioning patchconstantfunc numthreads maxvertexcount precise" \
    "abs acos all AllMemoryBarrier AllMemoryBarrierWithGroupSync any asdouble asfloat asin asint asuint atan atan2 " \
    "ceil clamp clip cos cosh countbits cross ddx ddx_coarse ddx_fine ddy ddy_coards ddy_fine degrees determinant " \
    "DeviceMemoryBarrier DeviceMemoryBarrierWithGroupSync distance dot dst EvaluateAttributeAtCentroid " \
    "EvaluateAttributeAtSample EvaluateAttributeSnapped exp exp2 f16tof32 f32tof16 faceforward firstbithigh " \
    "firstbitlow floor fmod frac frexp fwidth GetRenderTargetSampleCount GetRenderTargetSamplePosition " \
    "GroupMemoryBarrier GroupMemoryBarrierWithGroupSync InterlockedAdd InterlockedAnd InterlockedCompareExchange " \
    "InterlockedExchange InterlockedMax InterlockedMin IntterlockedOr InterlockedXor isfinite isinf isnan ldexp " \
    "length lerp lit log log10 log2 mad max min modf mul normalize pow Process2DQuadTessFactorsAvg " \
    "Process2DQuadTessFactorsMax Process2DQuadTessFactorsMin ProcessIsolineTessFactors ProcessQuadTessFactorsAvg " \
    "ProcessQuadTessFactorsMax ProcessQuadTessFactorsMin ProcessTriTessFactorsAvg ProcessTriTessFactorsMax " \
    "ProcessTriTessFactorsMin radians rcp reflect refract reversebits round rsqrt saturate sign sin sincos sinh " \
    "smoothstep sqrt step tan tanh transpose trunc Append RestartStrip CalculateLevelOfDetail " \
    "CalculateLevelOfDetailUnclamped GetDimensions GetSamplePosition Load Sample SampleBias SampleCmp " \
    "SampleCmpLevelZero SampleGrad SampleLevel Load2 Load3 Load4 Consume Store Store2 Store3 Store4 DecrementCounter " \
    "IncrementCounter mips Gather GatherRed GatherGreen GatherBlue GatherAlpha GatherCmp GatherCmpRed GatherCmpGreen " \
    "GatherCmpBlue GatherCmpAlpha"
);

static const char* keywordsHLSL1 =
(
    "snorm unorm string bool bool1 bool2 bool3 bool4 int int1 int2 int3 int4 uint uint1 " \
    "uint2 uint3 uint4 dword dword1 dword2 dword3 dword4 half half1 half2 half3 half4 float float1 float2 float3 " \
    "float4 double double1 double2 double3 double4 matrix bool1x1 bool1x2 bool1x3 bool1x4 bool2x1 bool2x2 bool2x3 " \
    "bool2x4 bool3x1 bool3x2 bool3x3 bool3x4 bool4x1 bool4x2 bool4x3 bool4x4 int1x1 int1x2 int1x3 int1x4 int2x1 " \
    "int2x2 int2x3 int2x4 int3x1 int3x2 int3x3 int3x4 int4x1 int4x2 int4x3 int4x4 uint1x1 uint1x2 uint1x3 uint1x4 " \
    "uint2x1 uint2x2 uint2x3 uint2x4 uint3x1 uint3x2 uint3x3 uint3x4 uint4x1 uint4x2 uint4x3 uint4x4 half1x1 half1x2 " \
    "half1x3 half1x4 half2x1 half2x2 half2x3 half2x4 half3x1 half3x2 half3x3 half3x4 half4x1 half4x2 half4x3 half4x4 " \
    "float1x1 float1x2 float1x3 float1x4 float2x1 float2x2 float2x3 float2x4 float3x1 float3x2 float3x3 float3x4 " \
    "float4x1 float4x2 float4x3 float4x4 double1x1 double1x2 double1x3 double1x4 double2x1 double2x2 double2x3 " \
    "double2x4 double3x1 double3x2 double3x3 double3x4 double4x1 double4x2 double4x3 double4x4 dword1x1 dword1x2 " \
    "dword1x3 dword1x4 dword2x1 dword2x2 dword2x3 dword2x4 dword3x1 dword3x2 dword3x3 dword3x4 dword4x1 dword4x2 " \
    "dword4x3 dword4x4 cbuffer tbuffer groupshared SamplerState SamplerComparisonState in out inout vector matrix interface " \
    "class point triangle line lineadj triangleadj Texture Texture1D Texture1DArray Texture2D Texture2DArray " \
    "Texture2DMS Texture2DMSArray Texture3D TextureCube TextureCubeArray RWTexture1D RWTexture1DArray RWTexture2D " \
    "RWTexture2DArray RWTexture3D Buffer StructuredBuffer AppendStructuredBuffer ConsumeStructuredBuffer RWBuffer " \
    "RWStructuredBuffer ByteAddressBuffer RWByteAddressBuffer PointStream TriangleStream LineStream InputPatch " \
    "OutputPatch SV_ClipDistance SV_CullDistance SV_Target SV_DispatchThreadID SV_DomainLocation SV_GroupID " \
    "SV_GroupIndex SV_GroupThreadID SV_GSInstanceID SV_TessFactor SV_InsideTessFactor SV_OutputControlPointID " \
    "SV_Coverage SV_Depth SV_Position SV_IsFrontFace SV_RenderTargetArrayIndex SV_SampleIndex " \
    "SV_ViewportArrayIndex SV_InstanceID SV_PrimitiveID SV_VertexID " \
    "min16float min16float1 min16float1x1 min10float min10float1 min10float1x1 min16int " \
    "min16int1 min16int1x1 min12int min12int1 min12int1x1 min16uint min16uint1 min16uint1x1 " \
    "min16float2 min16float3 min16float4 min10float2 min10float3 min10float4 min16int2 " \
    "min16int3 min16int4 min12int2 min12int3 min12int4 min16uint2 min16uint3 min16uint4 " \
    "min16float2x2 min16float2x3 min16float2x4 min16float3x2 min16float3x3 min16float3x4 " \
    "min16float4x2 min16float4x3 min16float4x4 min10float2x2 min10float2x3 min10float2x4 " \
    "min10float3x2 min10float3x3 min10float3x4 min10float4x2 min10float4x3 min10float4x4 " \
    "min16int2x2 min16int2x3 min16int2x4 min16int3x2 min16int3x3 min16int3x4 min16int4x2 " \
    "min16int4x3 min16int4x4 min12int2x2 min12int2x3 min12int2x4 min12int3x2 min12int3x3 " \
    "min12int3x4 min12int4x2 min12int4x3 min12int4x4 min16uint2x2 min16uint2x3 min16uint2x4 " \
    "min16uint3x2 min16uint3x3 min16uint3x4 min16uint4x2 min16uint4x3 min16uint4x4"
);


} // /namespace Xsc


#endif



// ================================================================================