
// Expression Test 5
// 13/12/2016

[foobar]
inline float f1()
{
	return 0;
}

float4 VS() : SV_Position
{
	int a = 0;
	bool b = true;
	
	f1();
	f1() + f1();
	
	a + 1;
	a > 0 ? 1 : 2;
	
	[branch]
	b ? 1 : 2;
	
	if (false) [loop];
	
	[foo(a)]
	{
	}
	
	return (float4)1;
}
