#ifndef UART_H_
#define UART_H_


#define _125ksps              0x1
#define _250ksps              0x3
#define _500ksps              0x5
#define _1Msps                0x7


extern void configuracionuart(int clock, int baudrate);
extern char readChar(void);
extern void printChar(char c);
extern void printString(char* string);
extern void readString(char UART0_Sring[], char delimitador);




#endif /* ADC_ADC_H_ */