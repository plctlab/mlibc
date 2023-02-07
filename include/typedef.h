#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include "stdio.h"

typedef unsigned int        m_size_t;                                             // 无符号 32 bits

typedef unsigned char       m_uint8_t;                                              // 无符号  8 bits
typedef unsigned short int  m_uint16_t;                                             // 无符号 16 bits
typedef unsigned int        m_uint32_t;                                             // 无符号 32 bits
typedef unsigned long long  m_uint64_t;                                             // 无符号 64 bits

typedef signed char         m_int8_t;                                               // 有符号  8 bits
typedef signed short int    m_int16_t;                                              // 有符号 16 bits
typedef signed int          m_int32_t;                                              // 有符号 32 bits
typedef signed long long    m_int64_t;                                              // 有符号 64 bits

typedef volatile uint8      m_vuint8_t;                                             // 易变性修饰 无符号  8 bits
typedef volatile uint16     m_vuint16_t;                                            // 易变性修饰 无符号 16 bits
typedef volatile uint32     m_vuint32_t;                                            // 易变性修饰 无符号 32 bits
typedef volatile uint64     m_vuint64_t;                                            // 易变性修饰 无符号 64 bits

typedef volatile int8       m_vint8_t;                                              // 易变性修饰 有符号  8 bits
typedef volatile int16      m_vint16_t;                                             // 易变性修饰 有符号 16 bits
typedef volatile int32      m_vint32_t;                                             // 易变性修饰 有符号 32 bits
typedef volatile int64      m_vint64_t;                                             // 易变性修饰 有符号 64 bits

#endif
