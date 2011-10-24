    b lesson07
    b lesson07
lesson07:
    lhz r7,#0xF1

    llz r5,#0x00
    llz r8,#0x20
main:
    dec r8,r8
    bnz main
    inc r5,r5
    stw [r7],r5
    llz r8,#0x20
    b main


