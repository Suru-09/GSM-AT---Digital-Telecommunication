#ifndef __AT_PARSER__
#define __AT_PARSER__

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_LINE_SIZE 100


#ifndef DEBUG
#define DEBUG 0
#endif

// DEBUG_PRINTING
// Run with -DDEBUG in order to activate it (modify CFLAGS in makefile)
// it does not generate any code otherwise
#define DEBUG_PRINT(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "[%s]:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)

#include <stdint.h>

typedef struct AT_COMMAND
{
    uint8_t ok;
    uint8_t  data[AT_COMMAND_MAX_LINES][AT_COMMAND_LINE_SIZE + 1];
    uint32_t lineCount;
}AT_COMMAND_DATA;

extern AT_COMMAND_DATA commands;

typedef enum AT_COMMAND_RET
{
    AT_READY_OK = 0,
    AT_READY_ERROR = 1,
    AT_IN_PROGRESS = 2
}AT_COMMAND_RETURN_VALUE;

AT_COMMAND_RETURN_VALUE parse(uint8_t ch);

#endif