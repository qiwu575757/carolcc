
#ifndef EASYCC_ERROR_H
#define EASYCC_ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define __DEBUG 
#define __WARN
#define __TRACE
#define __INFO

#ifdef __DEBUG
#define ERROR(format, ...) \
        do{\
                printf("\x1b[32m""%s:%d " format "\x1b[0m\n",__FILE__,__LINE__,##__VA_ARGS__);\
                exit(1);\
        }\
        while(0)
#else 
#define ERROR(format, ...) 
#endif

#ifdef __WARN
#define WARNNING(format, ...) \
            printf("\x1b[93m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define WARNNING(format, ...) 
#endif


#ifdef __INFO
#define INFO(format, ...) \
            printf("\x1b[34m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define INFO(format, ...) 
#endif

#ifdef __TRACE
#define TRACE(format, ...) \
            printf("\x1b[90m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define TRACE(format, ...) 
#endif

#endif // EASYCC_ERROR_H
