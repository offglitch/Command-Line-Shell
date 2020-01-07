/**
 * debug.c
 *
 * Debugging using the preprocessor. When DEBUG is set to 0, the LOG statements
 * are completely removed from the binary produced after compliation.
 *
 * Source: https://stackoverflow.com/questions/5765175/macro-to-turn-off-printf-statements
 * 
 * To turn debug on, either:
 * - Add a line like this to your C file:
 *      #define DEBUG 1
 * - Compile as shown below
 *
 * Compile:
 *      gcc -g -Wall -o debug -DDEBUG=1 debug.c
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

/* If we haven't passed -DDEBUG=1 to gcc, then this will be set to 0: */
#ifndef DEBUG
#define DEBUG 0
#endif

#define LOG(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, __VA_ARGS__); } while (0)

#endif
