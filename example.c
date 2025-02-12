/*
  Example usage of the 6502 header file
  Prints the opcode, registers and uses memory address 0xFF00 as output
*/
#include <stdio.h>
#include <stdlib.h>
#include "6502.h"


int main(int argc, char **argv)
{
    char Stepbuffer[32];

    init(argc,argv);


    printf("OPCODE\tA\tX\tY\tOUTPUT[0xFF00]\n\n");
    while (loop() != -1) //runs until BRK is called, or programcounter > 0xFFFF
    {
        printf("%02X\t%02X\t%02X\t%02X",*(mainmem + programcounter),regA,regX,regY); 
        printf("\t%04X\033[F\t\t\t",*(mainmem+0xFF00));
        scanf("%c",&Stepbuffer); //Simple way to step through program
    }

    return 0;
}
