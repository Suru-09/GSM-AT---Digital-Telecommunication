#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

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
        printf("Character : <%c> \n", c != 0xD && c != 0x0A ? c : '9');
        uint8_t val;
        if ( (val = parse(c)) )
        {
            printf("Eu sunt val: <%c>\n", val);
            printf("Sunt READY_OK ? <%d>\n", val == AT_READY_OK);
            printf("Something is happening here: <%s>!\n", (char *)commands.data[0]);
        }
    }

    fclose(fp);
}