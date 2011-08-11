
b reset
b reset
b reset
b reset

reset:
    llz r3,#5
b:
    dec r3,r3
    bnz b

    halt
