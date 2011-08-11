
void hang ( void );
void PUT16 ( unsigned int a, unsigned int d );
unsigned int GET16 ( unsigned int a );

void uart_init ( void )
{
}


void uart_send ( unsigned char c )
{
    /* *((volatile unsigned char *)0x77) = c;*/

    PUT16(0xF000,c);
}


unsigned int fun ( unsigned int a )
{
    unsigned int b;
    b=a&7;
    b=b+5;
    return(b);
}



void hexstring ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=16;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x0D);
    uart_send(0x0A);
}


void reset ( void )
{
    /*
    uart_send(0x30);
    uart_send(0x31);
    uart_send(0x32);
    uart_send(0x33);
    uart_send(0x0D);
    uart_send(0x0A);
    */
    hexstring(0x1234);
    hang();
}

