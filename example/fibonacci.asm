;6502 assembly fibonacci sequence up to 233
LDA #$09
STA $FF1D
LDA #$00
LDA #$01
ADC $FF02
STA $FF00  ;Display
BCS $FF    ;Terminate the program when carry flag is set, i.e. an unsigned overflow occured
LDX $FF02
STA $FF02
STX $FF03
LDA $FF03
JMP $FF1D  ;Loop
