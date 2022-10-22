#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

/**
 * @brief   1. Create a reset function when the Parser returns an error.
 *          2. 
 */

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Invalid use! Try: %s <file_name>", argv[0]);
        exit(1);
    }

    FILE* fp;
    fp = fopen(argv[1], "r");

    char c;
    while( (c = fgetc(fp)) != EOF )
    {
        // DEBUG_PRINT("Character : <%c> \n", c != 0xD && c != 0x0A ? c : '9');
        uint8_t val;
        if ( (val = parse(c)) )
        {
            if ( val == AT_READY_OK && commands.lineCount < AT_COMMAND_MAX_LINES)
            {
                DEBUG_PRINT("RETURN STRING: <%s>!\n", commands.data[commands.lineCount - 1]);
                // DEBUG_PRINT("Line Count: [%d]\n", commands.lineCount);
            }
            else if ( val == AT_IN_PROGRESS)
            {
                // DEBUG_PRINT("PROGRESS STRING: <%s>!\n", commands.data[commands.lineCount]);
                continue;
            }
            else if ( val == AT_READY_NOK )
            {
                DEBUG_PRINT("AT_READY_NOK!\n");
            }
            else if ( val == AT_READY_ERROR )
            {
                DEBUG_PRINT("AT_READY_ERROR!\n");
            }
            else {
                // DEBUG_PRINT("PLM!\n");
            }
        }
    }
    fclose(fp);
}