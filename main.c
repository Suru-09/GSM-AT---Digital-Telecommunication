#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// non-C standard imports
#include "parser.h"

void print_matrix(uint8_t start)
{
    for(int i = start; i < commands.lineCount; ++i)
    {
        if (!strlen((char *)commands.data[i]))
        {
            continue;
        }
        DEBUG_PRINT("<%s>\n", commands.data[i]);
    }   
}

void reset_matrix()
{
    memset(commands.data, '\0', sizeof(commands.data));
}

void test_parser(char* file_name)
{
    FILE* fp;
    fp = fopen(file_name, "r");

    if (!fp)
    {
        DEBUG_PRINT("Could not open the test file: <%s>\n", file_name);
        exit(1);
    }

    char c;
    uint8_t ok = 1, lastPrinted = 0;
    while( (c = fgetc(fp)) != EOF )
    {
        // DEBUG_PRINT("Character : <%c> \n", c != 0xD && c != 0x0A ? c : '9');
        uint8_t val = parse(c);
        if ( val == AT_READY_OK)
        {
            if ( ok )
            {
                print_matrix(lastPrinted++);
                if (commands.lineCount >= 100)
                {
                    ok = 0;
                } 
            }
            DEBUG_PRINT("[commands.ok] == <%s>\n", commands.ok == 1 ? "OK" : "ERROR");
        }
        else if ( val == AT_IN_PROGRESS)
        {
            continue;
        }
        else if ( val == AT_READY_ERROR )
        {
            DEBUG_PRINT("AT_READY_ERROR!\n");
            break;
        }  
    }
    reset_matrix();
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Invalid use! Try: %s <file_name>\n", argv[0]);
        exit(1);
    }

    test_parser(argv[1]);
    return 0;
}