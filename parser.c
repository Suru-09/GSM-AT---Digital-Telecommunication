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

void addChar(uint8_t c)
{
    printf("De cate ori adaug caracter? %c \n", (char)c);
    printf("Data lenCount: [%d]\n", commands.lineCount);
    uint32_t dataLen = strlen((char *)commands.data[commands.lineCount]);
    printf("Eu sunt lungimea sirului: <%d>\n", dataLen);

    if ( dataLen > AT_COMMAND_LINE_SIZE )
    {
        return;
    }

    printf("Cum de am ajuns aici: dataLen: [%d]\n", dataLen);
    commands.data[commands.lineCount][dataLen] = c;
}


AT_COMMAND_RETURN_VALUE parse(uint8_t ch)
{   
    static uint32_t state = 0;
    switch(state)
    {
        case 0:
            if ( isCR(ch) )
            {
                state = 1;
            }
            break;
        case 1:
            if ( isLF(ch) )
            {
                state = 2;
            }
            else 
            {
                return AT_READY_ERROR;
            }
            break;
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
                return AT_READY_ERROR;
            }
            break;
        case 3:
            if ( ch == 'K')
            {
                addChar(ch);
                state = 4;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 4:
            if ( isCR(ch) )
            {
                state = 5;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 5:
            if ( isLF(ch) )
            {
                // FINAL STATE RESET
                commands.lineCount++;
                return AT_READY_OK;
            }
            else 
            {
                return AT_READY_ERROR;
            }
            break;
        case 6:
            if ( ch == 'R')
            {
                addChar(ch);
                state = 7;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
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
            break;
        case 8:
            if ( ch == 'O')
            {
                addChar(ch);
                state = 9;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 9:
            if ( ch == 'R')
            {
                addChar(ch);
                state = 10;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 10:
            if ( isCR(ch) )
            {
                state = 11;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 11:
            if ( isLF(ch) )
            {
                // FINAL STATE RESET
                commands.lineCount++;
                return AT_READY_OK;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 12:
            if ( ch >= 0x20 && ch <= 0x7e)
            {
                addChar(ch);
                state = 13;
            }
            break;
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
                return AT_READY_ERROR;
            }
            break;
        case 14:
            if ( isLF(ch) )
            {
                state = 15;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 15:
            if ( isCR(ch))
            {
                state = 16;
            }
            else
            {
                return AT_READY_ERROR;
            }
            break;
        case 16:
            if ( isLF(ch) )
            {
                state = 17;
            }
            else
            {
                return AT_READY_ERROR;
            }
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
                return AT_READY_ERROR;
            }
        default:
            break;
    }
}