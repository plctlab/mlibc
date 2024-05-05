#ifndef MLIBC_SETJMP_H__
#define MLIBC_SETJMP_H__

#define jmp_buf_t long *

int setjmp(jmp_buf_t env);
void longjmp(jmp_buf_t env, int val);

#endif
