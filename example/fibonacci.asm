;6502 assembly fibonacci sequence up to 233
LDA #$E9
STA $FF1D
LDA #$00
LDA #$01
ADC $FF02
STA $FF00
CMP $FF1D
BEQ $FF
LDX $FF02
STA $FF02
STX $FF03
LDA $FF03
JMP $0024
LDA $#09  ;Simple way to provide JMP address for JMP in line 14
