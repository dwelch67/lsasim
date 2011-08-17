    b reset
    b reset
reset:
    lhz r7,#0xFF

    llz r5,#0
outer:
    llz r6,#0x00
inner:
    dec r6,r6
    bnz inner
    inc r5,r5
    stw [r7],r5
    b outer

