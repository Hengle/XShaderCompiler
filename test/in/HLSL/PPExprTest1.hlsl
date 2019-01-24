
// HLSL Translator: Preprocessor Expression Test 1
// 09/11/2016


#define A (3*2)

#define F1(a, b, c) (a+()*c)
#define F2(a, b) a/b

F1(1, 2, F2(0, 1))

#if 5 > 2 ? 0 : 1
#	error YES {}
#else
#	error NO []
#endif

#define TOSTR(NAME) #NAME

#define VARG_F1(a, ...) a*2 { __VA_ARGS__ }

#define MyPrintf(STR, ...) printf(STR, __VA_ARGS__)

/*VARG_F1*/
VARG_F1(1, 2, 3, /*foo*/-foo)

MyPrintf("x = %i, y = %s", 5, "test")

TOSTR(TOSTR)

#define MERGE(TYPE, SIZE) TYPE  ##   SIZE

MERGE  (  MERGE(float, 4 ) , x4  )

#define CURSOR(top, bottom) (((top) << 8) | (bottom))

CURSOR(25, -32)


#if 5 > V1*3 && (x | 2 != f)// f 2
#	pragma message "1: IF BRANCH"
#else
#	pragma message "1: ELSE BRANCH"
#	if 0 != V2
#		pragma message "1.1: IF BRANCH (0 != V2)"
#	elif 5*2
#		pragma message "1.1: ELIF BRANCH (5*2)"
#	else
#		pragma message "1.1: ELSE BRANCH"
#	endif
#endif

#if 5 > 2 || defined A
#	pragma message "2: IF BRANCH"
#else
#	pragma message "2: ELSE BRANCH"
#endif


