
// HLSL Translator: Type Test 1
// 13/11/2016

struct S0
{
	float x;
};


cbuffer buffer1 : register(b0)
{
	vector<float, /*(4 > 2 ? (3+1)/4+3 : 2)*/4> v0;
	matrix<double, 2, 3> m0;
};

matrix<float, 3, 3> f1()
{
	return (float3x3)0;
}

texture tex0;

#define FILTER_STATE MIPFILTER=NONE;MINFILTER=NONE;MAGFILTER=NONE;
sampler texSampler1 = sampler_state { texture = (tex0); FILTER_STATE };
sampler texSampler2 = sampler_state { texture = <tex0>; FILTER_STATE };

/*typedef int DWORD;
typedef float FLOAT; 
typedef vector <float, 4> VECTOR;
typedef matrix <float, 4, 4> MATRIX;
typedef string STRING;*/
//typedef texture TEXTURE;
//typedef pixelshader PIXELSHADER;
//typedef vertexshader VERTEXSHADER;

/* --- <typedef struct tests> --- */

typedef struct S1
{
	float x, y;
}
S1_t1, S1_t1_a[5], S1_t1_b[1][2][3];

typedef struct
{
	float y, x;
}
S1_t2, S1_t2_a[3];

typedef S1_t2_a S1_t2_a_3[2];

typedef struct S1 S1_t3;

typedef S1 S1_t4;

/* --- </typedef struct tests> --- */

//void f2(Texture2D t[5])
int f2(int x)
{
	return 0;
}

//static S1_t1_a f3()
S1_t1_a f3()
{
	int x = 1 + (1 + (((3))) - 1) * 5;
	S1_t1_a a;
	return (a);
}

static const int g_2 = 5;
float g_1 = 0.0;

const struct { struct { int inner2; } inner1; } g_anomym_s1;

struct { float x; } f4()
{
	return (struct { float x; })0;
}

S1_t1_a f5(int x);

float4 VS() : SV_Position
{
	struct S1
	{
		float f, g;
	}
	s1, s2, s3;

	float3 f2_v = f2(0).xxx;
	
	float3 f3_v = f3()[0].x.xxx;
	
	float f4_x = g_anomym_s1.inner1.inner2;
	
	int a = 0, b = (dword)0;
	int c = (a += b);
	
	int f5_v = 1 + 4 + f5(3)[2].x;
	
	DWORD x = 0;
	FLOAT x1 = 0;
	S1_t2 x2 = (S1_t2)0;
	S1_t2_a x3;
	S1_t2_a_3 x4;
	S1_t2_a_3 x5[1];
//	return VECTOR(v0 + f1(), 1);
	return float4(v0 * f1()._11);
}

S1_t1_a f5(int x)
{
    S1_t1_a y = (S1_t1_a)0;//{ x, 1, 2, 3, 4 };
    return y;
}

