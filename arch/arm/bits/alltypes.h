#define _REDIR_TIME64 1
#define _Addr int
#define _Int64 long long
#define _Reg int

#if __ARMEB__
#define __BYTE_ORDER 4321
#else
#define __BYTE_ORDER 1234
#endif

#define __LONG_MAX 0x7fffffffL


typedef float float_t;
typedef double double_t;

typedef struct { long long __ll; long double __ld; } max_align_t;