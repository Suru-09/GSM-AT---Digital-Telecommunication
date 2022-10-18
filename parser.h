#ifndef __AT_PARSER__
#define __AT_PARSER__

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_LINE_SIZE 100

#include <stdint.h>

typedef struct AT_COMMAND_DATA
{
    uint8_t ok;
    uint32_t  data[AT_COMMAND_MAX_LINES][AT_COMMAND_LINE_SIZE + 1];
    uint8_t lineCount;
};

typedef enum AT_COMMAND_RETURN_VALUE
{
    AT_READY_NOK = 0,
    AT_READY_OK = 1,
    AT_READY_ERROR = 2,
};

uint8_t parse(uint8_t ch);

#endif