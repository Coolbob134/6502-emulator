#include <stdio.h>
#include <stdlib.h>

// b7,b6,b5,b4,b3,b2,b1,b0 //6502 bit order
// Status |= 0b00010000 // set BRK 

#define memsize    0xFFFF
#define stackaddr  0x01FF

static unsigned char regA = 0x00, regX = 0x00, regY = 0x00,Status = 0b00100000, StackPointer = 0xFD, tempvalue;
static unsigned short int programcounter;
static unsigned char* mainmem;

static void (*instructionArr[])();


int init(int argc, char **argv) //Initialization
{
    
    mainmem = (unsigned char*)malloc(memsize*sizeof(int));
    FILE *rom = fopen("programmem.bin","r");    //default file to be executed //replace programmem.bin with another default file
    if (argc >= 2)
    {
        FILE *rom = fopen(argv[2],"r");
    }
    
    fread(mainmem, 1,memsize,rom);
    fclose(rom);
}

int loop() //Main loop
{
    if (programcounter < memsize && ((Status & 0b00010000) == 0 ))
    {
        (*instructionArr[*(mainmem + programcounter)])();
    }
    else
    {
        return -1;
    }

}

unsigned short int dbyte(int address) //function to convert from little endian to big endian from given address and address + 1
{
    unsigned char lowbyte = *(mainmem+address), highbyte = *(mainmem+address+1);
    return ((highbyte << 8) | lowbyte);
}

void pushstack(unsigned char inbyte)
{
    *(mainmem+0x100+StackPointer) = inbyte;
    StackPointer--;
}

char calcrelative(unsigned char operand) // calculate relative offset
{
    if (operand >= 0b10000000)
    {
        return -( (~operand) +1);
    }
    return operand;
}

unsigned char pullstack() //pull from stack
{
    StackPointer++;
    return *(mainmem+0x100+StackPointer);
}


void BRK()
{
    Status |= 0b00010000;
}


void NOP()
{
    programcounter++;
}

void JAM() //Jams the cpu
{
    Status |= 0b00010000;
}

//=======================================================================================
//LDA

void LDA_a()
{
    regA = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void LDA_aX()
{
    regA = *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void LDA_aY()
{
    regA = *(mainmem+dbyte(programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void LDA_I()
{
    regA = *(mainmem+programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=2;
}

void LDA_zpg()
{
    regA = *(mainmem+*(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=2;
}

void LDA_zpgX()
{
    regA = *(mainmem+*(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=2;
}

void LDA_ind_Y()
{
    regA = *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=2;
}

void LDA_X_ind()
{
    regA = *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=2;
}

//=======================================================================================
//LDX

void LDX_a()
{
    regX = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter+=3;
}

void LDX_aY()
{
    regX = *(mainmem+dbyte(programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter+=3;
}

void LDX_I()
{
    regX = *(mainmem+programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter+=2;
}

void LDX_zpg()
{
    regX = *(mainmem+*(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter+=2;
}

void LDX_zpgY()
{
    regX = *(mainmem+*(mainmem+programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter+=2;
}

//=======================================================================================
//LDY

void LDY_a()
{
    regY = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter+=3;
}

void LDY_aX()
{
    regY = *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter+=3;
}

void LDY_I()
{
    regY = *(mainmem+programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter+=2;
}

void LDY_zpg()
{
    regY = *(mainmem+*(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter+=2;
}

void LDY_zpgX()
{
    regY = *(mainmem+*(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter+=2;
}
//=======================================================================================
//STA

void STA_a()
{
    *(mainmem+dbyte(programcounter+1)) = regA;
    programcounter+=3;
}

void STA_aX()
{
    *(mainmem+dbyte(programcounter+1)+regX) = regA;
    programcounter+=3;
}

void STA_aY()
{
    *(mainmem+dbyte(programcounter+1)+regY) = regA;
    programcounter+=3;
}

void STA_zpg()
{
    *(mainmem+*(mainmem+programcounter+1)) = regA;
    programcounter+=2;
}

void STA_zpgX()
{
    *(mainmem+*(mainmem+programcounter+1)+regX) = regA;
    programcounter+=2;
}

void STA_ind_Y()
{
    *(mainmem+dbyte(*(mainmem+programcounter+1))+regY) = regA;
    programcounter+=2;
}

void STA_X_ind()
{
    *(mainmem+dbyte(*(mainmem+programcounter+1)+regX)) = regA;
    programcounter+=2;
}

//=======================================================================================
//STX

void STX_a()
{
    *(mainmem+dbyte(programcounter+1)) = regX;
    programcounter+=3;
}

void STX_zpgY()
{
    *(mainmem+*(mainmem+programcounter+1)+regY) = regX;
    programcounter+=2;
}

void STX_zpg()
{
    *(mainmem+*(mainmem+programcounter+1)) = regX;
    programcounter+=2;
}

//=======================================================================================
//STY

void STY_a()
{
    *(mainmem+dbyte(programcounter+1)) = regY;
    programcounter+=3;
}

void STY_zpgX()
{
    *(mainmem+*(mainmem+programcounter+1)+regX) = regY;
    programcounter+=2;
}

void STY_zpg()
{
    *(mainmem+*(mainmem+programcounter+1)) = regY;
    programcounter+=2;
}

//=======================================================================================
//ADC

void ADC_a()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter+1));
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void ADC_aX()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter+1)+regX);
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void ADC_aY()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter+1)+regY);
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void ADC_I()
{
    
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + programcounter+1);
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void ADC_zpg()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + *(mainmem+programcounter+1));
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void ADC_zpgX()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + *(mainmem+programcounter+1)+regX);
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void ADC_ind_Y()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void ADC_X_ind()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    result = regA +  tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

//=======================================================================================
//SBC

void SBC_a()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter + 1));
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void SBC_aX()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter + 1)+regX);
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void SBC_aY()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + dbyte(programcounter + 1)+regY);
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=3;
}

void SBC_I()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + programcounter + 1);
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;

}

void SBC_zpg()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + *(mainmem+programcounter + 1));
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void SBC_zpgX()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem + *(mainmem+programcounter + 1)+regX);
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void SBC_ind_Y()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

void SBC_X_ind()
{
    short unsigned int result;
    tempvalue = (Status & 0b1) + *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    tempvalue = (tempvalue ^ 0xFF) + 0x01;
    result = regA + tempvalue;
    Status &= ~(0b11000011);
    Status |= ((result & 0b10000000)|((result & 0b100000000)>>8)|((result==0)<<1));
    Status |= (((( tempvalue & 0b10000000)==(regA & 0b10000000))) && ((regA & 0b10000000)!=(result & 0b10000000)));
    regA = (unsigned char)((result<<8)>>8);
    programcounter+=2;
}

//=======================================================================================
//INC

void INC_a()
{
    *(mainmem + dbyte(programcounter+1)) += 1;
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000)); 
    programcounter+=3;
}

void INC_aX()
{
    *(mainmem + dbyte(programcounter+1)+regX) += 1;
    tempvalue = *(mainmem + dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000)); 
    programcounter+=3;
}

void INC_zpg()
{
    *(mainmem +*(mainmem+programcounter+1)) += 1;
    tempvalue = *(mainmem +*(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));  
    programcounter+=2;
}

void INC_zpgX()
{
    *(mainmem +*(mainmem+programcounter+1)+regX) += 1;
    tempvalue = *(mainmem +*(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000)); 
    programcounter+=2;
}

//=======================================================================================
//INX

void INX()
{
    regX++;
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter++;
}

//=======================================================================================
//INY

void INY()
{
    regY++;
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter++;
}

//=======================================================================================
//DEC

void DEC_a()
{
    
    *(mainmem + dbyte(programcounter+1)) -= 1;
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

void DEC_aX()
{
    *(mainmem + dbyte(programcounter+1)+regX) -= 1;
    tempvalue = *(mainmem + dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

void DEC_zpg()
{
    *(mainmem +*(mainmem+programcounter+1)) -= 1;  

    tempvalue = *(mainmem +*(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000)); 
    programcounter+=2;
}

void DEC_zpgX()
{
    *(mainmem +*(mainmem+programcounter+1)+regX) -= 1;
    tempvalue = *(mainmem +*(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000)); 
    programcounter+=2;
}

//=======================================================================================
//DEX

void DEX()
{
    regX--;
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter++;
}

//=======================================================================================
//DEY

void DEY()
{
    regY--;
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter++;
}

//=======================================================================================
//ASL

void ASL_zpg()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    *(mainmem + programcounter+1)=tempvalue;

    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ASL_A()
{
    Status &= ~ (0b10000011);
    Status = Status | ((regA & 0b10000000)>>7);
    regA = regA << 1;

    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter++;
}

void ASL_a()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    *(mainmem + dbyte(programcounter+1))=tempvalue;

    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

void ASL_zpgX()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    *(mainmem + programcounter+1+regX)=tempvalue;

    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ASL_aX()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    *(mainmem + dbyte(programcounter+1)+regX)=tempvalue;

    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

//=======================================================================================
//LSR

void LSR_zpg()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b1));
    tempvalue = tempvalue >> 1;
    *(mainmem + programcounter+1)=tempvalue;

    Status |= ((tempvalue == 0) << 1);
    programcounter+=2;
}

void LSR_A()
{
    Status &= ~ (0b10000011);
    Status = Status | ((regA & 0b1));
    regA = regA >> 1;

    Status |= ((regA == 0) << 1);
    programcounter++;
}

void LSR_a()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b1));
    tempvalue = tempvalue >> 1;
    *(mainmem + dbyte(programcounter+1))=tempvalue;

    Status |= ((tempvalue == 0) << 1);
    programcounter+=3;
}

void LSR_zpgX()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b1));
    tempvalue = tempvalue >> 1;
    *(mainmem + programcounter+1+regX)=tempvalue;

    Status |= ((tempvalue == 0) << 1);
    programcounter+=2;
}

void LSR_aX()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b1));
    tempvalue = tempvalue >> 1;
    *(mainmem + dbyte(programcounter+1)+regX)=tempvalue;
    
    Status |= ((tempvalue == 0) << 1);
    programcounter+=3;
}

//=======================================================================================
//ROL

void ROL_zpg()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + programcounter+1)=tempvalue;
    
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ROL_A()
{
    Status &= ~ (0b10000011);
    Status = Status | ((regA & 0b10000000)>>7);
    regA = regA << 1;
    regA |= (Status & 0b1);

    
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));

    programcounter++;
}

void ROL_a()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + dbyte(programcounter+1))=tempvalue;
    
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

void ROL_zpgX()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + programcounter+1+regX)=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ROL_aX()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | ((tempvalue & 0b10000000)>>7);
    tempvalue = tempvalue << 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + dbyte(programcounter+1)+regX)=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

//=======================================================================================
//ROR

void ROR_zpg()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1));
    Status &= ~ (1);
    Status = Status | (tempvalue & 0b1);
    tempvalue = tempvalue >> 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + programcounter+1)=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ROR_A()
{
    Status &= ~ (0b10000011);
    Status = Status | (regA & 0b1);
    regA = regA >> 1;
    regA |= (Status & 0b1);

    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter++;
}

void ROR_a()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status = Status | (tempvalue & 0b1);
    tempvalue = tempvalue >> 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + dbyte(programcounter+1))=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

void ROR_zpgX()
{
    
    tempvalue = *(mainmem +*(mainmem + programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status = Status | (tempvalue & 0b1);
    tempvalue = tempvalue >> 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + programcounter+1+regX)=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=2;
}

void ROR_aX()
{
    
    tempvalue = *(mainmem + dbyte(programcounter+1) + regX);
    Status &= ~ (0b10000011);
    Status = Status | (tempvalue & 0b1);
    tempvalue = tempvalue >> 1;
    tempvalue |= (Status & 0b1);
    *(mainmem + dbyte(programcounter+1) + regX)=tempvalue;
    Status |= (((tempvalue == 0) << 1) | (tempvalue & 0b10000000));
    programcounter+=3;
}

//=======================================================================================
//AND

void AND_a()
{
    regA &= *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void AND_aX()
{
    regA &= *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 3;
}

void AND_aY()
{
    regA &= *(mainmem+dbyte(programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 3;
}

void AND_I()
{
    regA &= *(mainmem+programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void AND_zpg()
{
    regA &= *(mainmem + *(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void AND_zpgX()
{
    regA &= *(mainmem + *(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void AND_ind_Y()
{
    regA &= *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter +=2;
}

void AND_X_ind()
{
    regA &= *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

//=======================================================================================
//ORA

void ORA_a()
{
    regA |= *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void ORA_aX()
{
    regA |= *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
    
}

void ORA_aY()
{
    regA |= *(mainmem+dbyte(programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
    
}

void ORA_I()
{
    regA |= *(mainmem + programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void ORA_zpg()
{
    regA |= *(mainmem + *(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void ORA_zpgX()
{
    regA |= *(mainmem + *(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
    
}

void ORA_ind_Y()
{
    regA &= *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter +=2;
}

void ORA_X_ind()
{
    regA |= *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

//=======================================================================================
//EOR
void EOR_a()
{
    regA ^= *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void EOR_aX()
{
    regA ^= *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void EOR_aY()
{
    regA ^= *(mainmem+dbyte(programcounter+1)+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter+=3;
}

void EOR_I()
{
    regA ^= *(mainmem + programcounter+1);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void EOR_zpg()
{
    regA ^= *(mainmem + *(mainmem+programcounter+1));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void EOR_zpgX()
{
    regA ^= *(mainmem + *(mainmem+programcounter+1)+regX);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

void EOR_ind_Y()
{
    regA ^= *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter +=2;
}

void EOR_X_ind()
{
    regA ^= *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter += 2;
}

//=======================================================================================
//CMP

void CMP_a()
{
    
    tempvalue = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=3;
}

void CMP_aX()
{
    
    tempvalue = *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=3;
}

void CMP_aY()
{
    
    tempvalue = *(mainmem+dbyte(programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=3;
}

void CMP_I()
{
    
    tempvalue = *(mainmem+programcounter+1);
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=2;
}

void CMP_zpg()
{
    
    tempvalue = *(mainmem+*(mainmem+ programcounter+1));
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=2;
}

void CMP_zpgX()
{
    
    tempvalue = *(mainmem+*(mainmem+ programcounter+1)+regX);
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=2;
}

void CMP_ind_Y()
{
    
    tempvalue = *(mainmem+dbyte(*(mainmem+programcounter+1))+regY);
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=2;
}

void CMP_X_ind()
{
    
    tempvalue = *(mainmem+dbyte(*(mainmem+programcounter+1)+regX));
    Status &= ~ (0b10000011);
    Status |= ((regA >= tempvalue)|((regA == tempvalue)<<1)|((regA < tempvalue)<<7));
    programcounter +=2;
}

//=======================================================================================
//CPX

void CPX_a()
{
    
    tempvalue = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status |= ((regX >= tempvalue)|((regX == tempvalue)<<1)|((regX < tempvalue)<<7));
    programcounter +=3;
}

void CPX_I()
{
    
    tempvalue = *(mainmem+ programcounter +1);
    Status &= ~ (0b10000011);
    Status |= ((regX >= tempvalue)|((regX == tempvalue)<<1)|((regX < tempvalue)<<7));
    programcounter +=2;
    
}

void CPX_zpg()
{
    
    tempvalue = *(mainmem+*(mainmem+ programcounter +1));
    Status &= ~ (0b10000011);
    Status |= ((regX >= tempvalue)|((regX == tempvalue)<<1)|((regX < tempvalue)<<7));
    programcounter +=2;
}

//=======================================================================================
//CPY

void CPY_a()
{
    
    tempvalue = *(mainmem+dbyte(programcounter+1));
    Status &= ~ (0b10000011);
    Status |= ((regY >= tempvalue)|((regY == tempvalue)<<1)|((regY < tempvalue)<<7));
    programcounter +=3;
}

void CPY_I()
{
    
    tempvalue = *(mainmem+ programcounter +1);
    Status &= ~ (0b10000011);
    Status |= ((regY >= tempvalue)|((regY == tempvalue)<<1)|((regY < tempvalue)<<7));
    programcounter +=2;
}

void CPY_zpg()
{
    
    tempvalue = *(mainmem+*(mainmem+ programcounter +1));
    Status &= ~ (0b10000011);
    Status |= ((regY >= tempvalue)|((regY == tempvalue)<<1)|((regY < tempvalue)<<7));
    programcounter +=2;
}

//=======================================================================================
//BIT

void BIT_a()
{
    Status &= 0b00111101;
    Status |= (*(mainmem+dbyte(programcounter+1)) & 0b11000000);
    Status |= ((*(mainmem+dbyte(programcounter+1)) && regA) == 0) << 1;
    programcounter += 3;
}

void BIT_I()
{
    Status &= 0b00111101;
    Status |= (*(mainmem+programcounter+1) & 0b11000000);
    Status |= ((*(mainmem+programcounter+1) && regA) == 0) << 1;
    programcounter += 2;
}

void BIT_zpg()
{
    Status &= 0b00111101;
    Status |= (*(mainmem+*(mainmem+programcounter+1)) & 0b11000000);
    Status |= ((*(mainmem+*(mainmem+programcounter+1)) && regA) == 0) << 1;
    programcounter += 2;;
}

//=======================================================================================
//BCC

void BCC()
{
    if ((Status & 0b00000001) == 0)
    {
        programcounter += calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BCS

void BCS()
{
    if ((Status & 0b00000001) == 1)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BNE

void BNE()
{
    if (((Status & 0b00000010) >> 1) == 0)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BEQ

void BEQ()
{
    if (((Status & 0b00000010) >> 1) == 1)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BPL

void BPL()
{
    if ((Status >> 7) == 0)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BMI

void BMI()
{
    if ((Status >> 7) == 1)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BVC

void BVC()
{
    if (((Status & 0b01000000) >> 6) == 0)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//BVS

void BVS()
{
    if (((Status & 0b01000000) >> 6) == 1)
    {
        programcounter+= calcrelative(*(mainmem+programcounter+1));
    }
    programcounter+=2;
}

//=======================================================================================
//TAX

void TAX()
{
    regX = regA;
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter++;
}

//=======================================================================================
//TXA

void TXA()
{
    regA = regX;
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter++;
}

//=======================================================================================
//TAY

void TAY()
{
    regY = regA;
    Status &= ~ (0b10000010);
    Status |= (((regY == 0) << 1) | (regY & 0b10000000));
    programcounter++;
}

//=======================================================================================
//TYA
void TYA()
{
    regA = regY;
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter++;
}

//=======================================================================================
//TSX

void TSX()
{
    regX = StackPointer;
    Status &= ~ (0b10000010);
    Status |= (((regX == 0) << 1) | (regX & 0b10000000));
    programcounter++;
}

//=======================================================================================
//TXS

void TXS()
{
    StackPointer = regX;
    programcounter++;
}

//=======================================================================================
//PHA

void PHA()
{
    pushstack(regA);
    programcounter++;
}

//=======================================================================================
//PLA

void PLA()
{
    regA = pullstack();
    Status &= ~ (0b10000010);
    Status |= (((regA == 0) << 1) | (regA & 0b10000000));
    programcounter++;
}

//=======================================================================================
//PHP

void PHP()
{
    tempvalue = Status;
    tempvalue |= 0b00110000;
    pushstack(tempvalue);
    programcounter++;
}

//=======================================================================================
//PLP

void PLP()
{
    Status = pullstack() & 0b11101111;
    programcounter++;
}

//=======================================================================================
//JMP

void JMP_a_ind()
{
    
    tempvalue = dbyte(programcounter+1);
    if ((tempvalue ) == 0x00FF)
    {
        tempvalue = *(mainmem+(dbyte(programcounter+1) & 0xFF00));
        
    }
    else
    {
        tempvalue = *(mainmem + dbyte(programcounter+1));
    }

    programcounter = *(mainmem+dbyte(programcounter+1));
    programcounter |= tempvalue <<8;
    
}

void JMP_a()
{
    programcounter = dbyte(programcounter+1);
}

//=======================================================================================
//JSR

void JSR()
{
    tempvalue = programcounter >> 8;
    pushstack(tempvalue);
    tempvalue = programcounter+2;
    pushstack(tempvalue);
    programcounter = *(mainmem+dbyte(programcounter+1));
}

//=======================================================================================
//RTS

void RTS()
{
    programcounter = pullstack();
    programcounter += pullstack() << 8 +1;
}

//=======================================================================================
//RTI

void RTI()
{
    Status = pullstack() & 0b11101111;
    programcounter = pullstack();
    programcounter += pullstack() << 8;
}

//=======================================================================================
//CLC

void CLC()
{
    Status &= 0b11111110;
}

//=======================================================================================
//SEC

void SEC()
{
    Status |= 0b00000001;
}

//=======================================================================================
//CLD

void CLD()
{
    Status &= 0b11110111;
}

//=======================================================================================
//SED

void SED()
{
    Status |= 0b00001000;
}

//=======================================================================================
//CLI

void CLI()
{
    Status &= 0b11111011;
}

//=======================================================================================
//SEI

void SEI()
{
    Status |= 0b00000100;
}

//=======================================================================================
//CLV

void CLV()
{
    Status &= 0b10111111;
}

//=======================================================================================



static void (*instructionArr[])() = {
    /*      -0      -1          -2      -3      -4          -5          -6          -7      -8      -9      -A      -B      -C          -D      -E      -F*/
    /*00-*/ BRK,    ORA_X_ind,  JAM,    NULL,   NOP,        ORA_zpg,    ASL_zpg,    NULL,   PHP,    ORA_I,  ASL_A,  NULL,   NOP,        ORA_a,  ASL_a,  NULL,
    /*01-*/ BPL,    ORA_ind_Y,  JAM,    NULL,   NOP,        ORA_zpgX,   ASL_zpgX,   NULL,   CLC,    ORA_aX, NOP,    NULL,   NOP,        ORA_aX, ASL_aX, NULL,
    /*02-*/ JSR,    AND_X_ind,  JAM,    NULL,   BIT_zpg,    AND_zpg,    ROL_zpg,    NULL,   PLP,    AND_I,  ROL_A,  NULL,   BIT_a,      AND_a,  ROL_a,  NULL,
    /*03-*/ BMI,    AND_ind_Y,  JAM,    NULL,   NOP,        AND_zpgX,   ROL_zpgX,   NULL,   SEC,    AND_aY, NOP,    NULL,   NOP,        AND_aX, ROL_aX, NULL,
    /*04-*/ RTI,    EOR_X_ind,  JAM,    NULL,   NOP,        EOR_zpg,    LSR_zpg,    NULL,   PHA,    EOR_I,  LSR_A,  NULL,   JMP_a,      EOR_a,  LSR_a,  NULL,
    /*05-*/ BVC,    EOR_ind_Y,  JAM,    NULL,   NOP,        EOR_zpgX,   LSR_zpgX,   NULL,   CLI,    EOR_aY, NOP,    NULL,   NOP,        EOR_aX, LSR_aX, NULL,
    /*06-*/ RTS,    ADC_X_ind,  JAM,    NULL,   NOP,        ADC_zpg,    ROR_zpg,    NULL,   PLA,    ADC_I,  ROR_A,  NULL,   JMP_a_ind,  ADC_a,  ROR_a,  NULL,
    /*07-*/ BVS,    ADC_ind_Y,  JAM,    NULL,   NOP,        ADC_zpgX,   ROR_zpgX,   NULL,   SEI,    ADC_aY, NOP,    NULL,   NOP,        ADC_aX, ROR_aX, NULL,
    /*08-*/ NOP,    STA_X_ind,  NOP,    NULL,   STY_zpg,    STA_zpg,    STX_zpg,    NULL,   DEY,    NOP,    TXA,    NULL,   STY_a,      STA_a,  STX_a,  NULL,
    /*09-*/ BCC,    STA_ind_Y,  JAM,    NULL,   STY_zpgX,   STA_zpgX,   STX_zpgY,   NULL,   TYA,    STA_aY, TXS,    NULL,   NULL,       STA_aX, NULL,   NULL,
    /*0A-*/ LDY_I,  LDA_X_ind,  LDX_I,  NULL,   LDY_zpg,    LDA_zpg,    LDX_zpg,    NULL,   TAY,    LDA_I,  TAX,    NULL,   LDY_a,      LDA_a,  LDX_a,  NULL,
    /*0B-*/ BCS,    LDA_ind_Y,  JAM,    NULL,   LDY_zpgX,   LDA_zpgX,   LDX_zpgY,   NULL,   CLV,    LDA_aY, TSX,    NULL,   LDY_aX,     LDA_aX, LDA_aY, NULL,
    /*0C-*/ CPY_I,  CMP_X_ind,  NOP,    NULL,   CPY_zpg,    CMP_zpg,    DEC_zpg,    NULL,   INY,    CMP_I,  DEX,    NULL,   CPY_a,      CMP_a,  DEC_a,  NULL,
    /*0D-*/ BNE,    CMP_ind_Y,  JAM,    NULL,   NOP,        CMP_zpgX,   DEC_zpgX,   NULL,   CLD,    CMP_aY, NOP,    NULL,   NULL,       CMP_aX, DEC_aX, NULL,
    /*0E-*/ CPX_I,  SBC_X_ind,  NOP,    NULL,   CPX_zpg,    SBC_zpg,    INC_zpg,    NULL,   INX,    SBC_I,  NOP,    NULL,   CPX_a,      SBC_a,  INC_a,  NULL,
    /*0F-*/ BEQ,    SBC_ind_Y,  JAM,    NULL,   NOP,        SBC_zpgX,   INC_zpgX,   NULL,   SED,    SBC_aX, NOP,    NULL,   NOP,        SBC_aX, INC_aX, NULL
};
