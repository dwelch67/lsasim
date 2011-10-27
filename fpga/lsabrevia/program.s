    b lesson07
    b lesson07
lesson07:
    lhz r7,#0xF1

    llz r5,#0x00
    llz r8,#0x00
    llz r9,#0x10
main:
    dec r8,r8
    bnz main
    dec r9,r9
    bnz main
    inc r5,r5
    stw [r7],r5
    llz r9,#0x10
    b main


