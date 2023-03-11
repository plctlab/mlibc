#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
                                          
#define NULL 0
#define opt_size 8

typedef unsigned long long     size_t;
typedef unsigned char          uint8_t;                                               
typedef unsigned short int     uint16_t;                                             
typedef unsigned int           uint32_t;                                         
typedef unsigned long long     uint64_t;                                           

typedef signed char            int8_t;                                              
typedef signed short int       int16_t;                                     
typedef signed int             int32_t;                                           
typedef signed long long       int64_t;                                              

typedef volatile uint8_t       vuint8_t;                                       
typedef volatile uint16_t      vuint16_t;                                 
typedef volatile uint32_t      vuint32_t;                                    
typedef volatile uint64_t      vuint64_t;                                 

typedef volatile int8_t        vint8_t;                                         
typedef volatile int16_t       vint16_t;
typedef volatile int32_t       vint32_t;
typedef volatile int64_t       vint64_t;

typedef long		           _fpos_t;

typedef int64_t               intmax_t;
typedef uint64_t              uintmax_t;


#endif
