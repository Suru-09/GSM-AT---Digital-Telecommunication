#include "parser.h"

#include <string.h>
#include <stdio.h>

// Initialising data STRUCT which holds all the commands
AT_COMMAND_DATA commands = {
    .ok = 0,
    .lineCount = 0,
    .data = {'\0'}
};

uint8_t isCR(uint8_t c)
{
    // CARRIAGE RETURN == 0xD
    return c == 0xD ? 1 : 0;
}

uint8_t isLF(uint8_t c)
{
    // Line Feed = 0x0A (a.k.a new line)
    return c == 0x0A ? 1 : 0;
}

void reset(uint32_t* state)
{
    (*state) = 0;
    // Resets the memory for the current line of the buffer
    memset(commands.data[commands.lineCount], '\0', AT_COMMAND_LINE_SIZE);
}

void addChar(uint8_t c)
{
    uint8_t dataLen = strlen((char *)commands.data[commands.lineCount]);
    DEBUG_PRINT("Length on line [%d] is : [%d]\n", commands.lineCount, dataLen);

    if ( dataLen > AT_COMMAND_LINE_SIZE || commands.lineCount >= AT_COMMAND_MAX_LINES )
    {
        return;
    }

    if ( c == '\0' )
    {
        DEBUG_PRINT("WTF\n");
        commands.lineCount++;
        return;
    }

    // DEBUG_PRINT("commands.lineCount: [%d]\n", commands.lineCount);
    commands.data[commands.lineCount][dataLen] = c;
    commands.data[commands.lineCount][dataLen + 1] = '\0';
}


AT_COMMAND_RETURN_VALUE parse(uint8_t ch)
{   
    static uint32_t state = 0;
    DEBUG_PRINT("State [%d]\n", state);
    switch(state)
    {
        case 0:
            if ( isCR(ch) )
            {
                state = 1;
                return AT_IN_PROGRESS;
            }
            break;
        case 1:
            if ( isLF(ch) )
            {
                state = 2;
            }
            else 
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 2:
            if ( ch == 'O')
            {
                addChar(ch);
                state = 3;
            }
            else if ( ch == 'E')
            {
                addChar(ch);
                state = 6;
            }
            else if ( ch == '+' )
            {
                addChar(ch);
                state = 12;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 3:
            if ( ch == 'K')
            {
                addChar(ch);
                state = 4;
                return AT_IN_PROGRESS;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
        case 4:
            if ( isCR(ch) )
            {
                state = 5;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 5:
            if ( isLF(ch) )
            {
                // FINAL STATE RESET
                addChar('\0');
                state = 0;
                return AT_READY_OK;
            }
            else 
            {
                reset(&state);
                return AT_READY_ERROR;
            }
        case 6:
            if ( ch == 'R')
            {
                reset(&state);
                addChar(ch);
                state = 7;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 7:
            if ( ch == 'R')
            {
                addChar(ch);
                state = 8;
            }
            else
            {
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 8:
            if ( ch == 'O')
            {
                addChar(ch);
                state = 9;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 9:
            if ( ch == 'R')
            {
                addChar(ch);
                state = 10;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 10:
            if ( isCR(ch) )
            {
                state = 11;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 11:
            if ( isLF(ch) )
            {
                // FINAL STATE RESET
                addChar('\0');
                state = 0;
                return AT_READY_OK;
            }
            return AT_READY_ERROR;
        case 12:
            if ( ch >= 0x20 && ch <= 0x7e)
            {
                addChar(ch);
                state = 13;
                return AT_IN_PROGRESS;
            }
            reset(&state);
            return AT_READY_ERROR;
        case 13:
            if ( ch >= 0x20 && ch <= 0x7e )
            {
                // Loop on state 13 (be careful for AT_MAX_LINE_SIZE)
                addChar(ch);
                state = 13;
            }
            else if ( isCR(ch) )
            {
                state = 14;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 14:
            if ( isLF(ch) )
            {
                addChar('\0');
                state = 15;
                return AT_READY_OK;
            }
            reset(&state);
            return AT_READY_ERROR;
        case 15:
            if ( isCR(ch))
            {
                state = 16;
            }
            else if ( ch == '+')
            {
                state = 12;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 16:
            if ( isLF(ch) )
            {
                state = 17;
                return AT_IN_PROGRESS;
            }
            reset(&state);
            return AT_READY_ERROR;
        case 17:
            if ( ch == 'E')
            {
                addChar(ch);
                state = 6;
            }
            else if ( ch == 'O')
            {
                addChar(ch);
                state = 3;
            }
            else
            {
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        default:
            reset(&state);
            return AT_READY_ERROR;
    }
}