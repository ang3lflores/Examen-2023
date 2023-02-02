#include "lib/include.h"
extern void configuracionuart(int clock, int baudrate)

{
    int BRD_I;
    float BRD_F;
    int data = 0x3;


        SYSCTL->RCGCUART|= (1<<5); // habilita el Uart5 
        
        SYSCTL->RCGCGPIO|= (1<<4); // habilita el puerto E
        //                 Tx E5      Rx E4
        GPIOE->AFSEL  |= (1<<5) | (1<<4);
        GPIOE->DIR    |= (1<<5) | (0<<4);
        GPIOE->PUR    |= (0<<5) | (0<<4);
        GPIOE->PDR    |= (0<<5) | (0<<4);
        GPIOE->AMSEL  |= (0<<5) | (0<<4);
        GPIOE->DEN    |= (1<<5) | (1<<4);
        GPIOE->PCTL   &= 0xFF00FFFF;
        GPIOE->PCTL   |= 0x00110000;
        GPIOE->CR     |= 0x00000000; // lock
        /*
         * Baudrate 28800 frecuencia reloj 20 000 000
         * BRD = 20 000 000 / ( 16 * 28800 ) = 43.40277
         * UARTFBRD[DIVFRAC] = integer( 0.40277 * 64 + 0.5 ) = 26
         */
        //    int ck = 16000000;
        //    int ck = 25000000;
        BRD_I = 43;
        BRD_F = 26;
        //BRD_F = lround(BRD_F * 64 + 0.5);

        // Control      RXE     TXE     UARTEN
        UART5->CTL  = (0<<9) | (0<<8) | (0<<0);
        UART5->IBRD = 43;      // Integer Baud-Rate Divisor
        UART5->FBRD = 26; // Fractional Baud-Rate Divisor
        UART5->LCRH = (data<<5) | (1<<4); //bits data & Enable FIFOs
        //The UART has two 16x8 FIFOs; one for transmit and one for receive
        UART5->CC   = 0x0; // Clock Configuration = System clock
        UART5->CTL  = (1<<9) | (1<<8) | (1<<0); //send

        

}
extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART5->FR & (1<<4)) != 0 );
    v = UART5->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern void readString(char UART5_Sring[], char delimitador)
{

   int i=0;
   char string [20]; //= (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       (UART5_Sring[i]) = c;
       i++;
       /*if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       */

       c = readChar();
   }

   //UART7_Sring = string;
     UART5_Sring[i]= '\0';

}