
#include <string.h>
#include <typedef.h>


size_t strlen(const char *s)
{
    const char *a = s;
    for (; *s; s++);
    return s-a;
}

