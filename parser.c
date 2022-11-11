#include "parser.h"

#include <string.h>
#include <stdio.h>

// Initialising data STRUCT which holds all the commands
AT_COMMAND_DATA commands = {
    .ok = 0,
    .lineCount = 0,
    .data = {'\0'}
};

void setOK(uint8_t bool)
{
    commands.ok = bool;
}

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
    memset(commands.data, '\0', sizeof(commands.data));
}

void addChar(uint8_t c)
{
    uint8_t dataLen = strlen((char *)commands.data[commands.lineCount]);
    // DEBUG_PRINT("Length on line [%d] is : [%d]\n", commands.lineCount, dataLen);

    if ( dataLen > AT_COMMAND_LINE_SIZE || commands.lineCount >= AT_COMMAND_MAX_LINES )
    {
        return;
    }

    if ( c == '\0' )
    {
        commands.lineCount++;
        return;
    }

    commands.data[commands.lineCount][dataLen] = c;
    commands.data[commands.lineCount][dataLen + 1] = '\0';
    // DEBUG_PRINT("CH: <%c>", commands.data[commands.lineCount][dataLen]);
}


AT_COMMAND_RETURN_VALUE parse(uint8_t ch)
{   
    static uint32_t state = 0;
    // DEBUG_PRINT("State [%d]\n", state);
    switch(state)
    {
        case 0:
            if ( isCR(ch) )
            {
                state = 1;
                return AT_IN_PROGRESS;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "CR", ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            break;
        case 1:
            if ( isLF(ch) )
            {
                state = 2;
            }
            else 
            {
                DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "LF", ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 2:
            if ( ch == 'O')
            {
                state = 3;
            }
            else if ( ch == 'E')
            {
                state = 6;
            }
            else if ( ch == '+' )
            {
                addChar(ch);
                state = 12;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c or %c or %c>, received <%c>\n", state, 'O', 'E', '+', ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 3:
            if ( ch == 'K')
            {
                setOK(1);
                state = 4;
                return AT_IN_PROGRESS;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'K', ch);
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
                DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "CR", ch);
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
                DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "LF", ch);
                reset(&state);
                return AT_READY_ERROR;
            }
        case 6:
            if ( ch == 'R')
            {
                state = 7;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'R', ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 7:
            if ( ch == 'R')
            {
                state = 8;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'R', ch);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 8:
            if ( ch == 'O')
            {
                state = 9;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'O', ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 9:
            if ( ch == 'R')
            {
                setOK(0);
                state = 10;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'R', ch);
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
                DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "CR", ch);
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
            DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "LF", ch);
            return AT_READY_ERROR;
        case 12:
            if ( ch >= 0x20 && ch <= 0x7e)
            {
                addChar(ch);
                state = 13;
                return AT_IN_PROGRESS;
            }
            DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "ASCII", ch);
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
                DEBUG_PRINT("State [%d] expected: <%c>, received <%c>\n", state, 'R', ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        case 14:
            if ( isLF(ch) )
            {
                addChar('\0');
                state = 15;
                return AT_IN_PROGRESS;
            }
            DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "LF", ch);
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
                DEBUG_PRINT("State [%d] expected: <%s or %c>, received <%c>\n", state, "CR", '+', ch);
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
            DEBUG_PRINT("State [%d] expected: <%s>, received <%c>\n", state, "LF", ch);
            reset(&state);
            return AT_READY_ERROR;
        case 17:
            if ( ch == 'E')
            {
                state = 6;
            }
            else if ( ch == 'O')
            {
                state = 3;
            }
            else
            {
                DEBUG_PRINT("State [%d] expected: <%c OR %c>, received <%c>\n", state, 'E', 'O', ch);
                reset(&state);
                return AT_READY_ERROR;
            }
            return AT_IN_PROGRESS;
        default:
            reset(&state);
            return AT_READY_ERROR;
    }
}