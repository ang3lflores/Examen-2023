#include "lib/include.h"

int main(void)
{

   /* uint16_t Result[2];
    float valor;
    float valor1;
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configura_Reg_ADC0();
    Configurar_UART0();
    */
    //printString("3");
    int clock= 20000000;
    int baudrate =28800;
    Configurar_PLL(20000000);   // Se pone el reloj interno del sistema a 20 MHz
    Configurar_ADCM0SS3();    //Habilite el SS3 porque requiero solo una muestra
    //Configurar_ADCM0SS2();    //ADC MULTICANAL
    //Configurar_ADCM0SS1();    //ADC MULTICANAL
    configuracionuart(clock, baudrate);// UART USB
    ConfigurarGPIOD();
//    IniciarIntSWLED();


    while(1)
    {
       
        /////////////////
       //readADC0SS1(adc_dataSS1);
//        Delay_ms(200);
//        readADC0SS2(adc_dataSS2);
//        Delay_ms(200);
//        readADC0SS3(adc_dataSS3);
//        Delay_ms(200);
        

    }
}

extern void GPIOB_boton(void)
{
    int adc_dataSS3; // arreglos de enteros con 3 casillas
    //int adc_dataSS2[3]; // arreglos de enteros con 3 casillas
    char num_char[16];
    if((GPIOB->RIS & (1<<3)) == (1<<3))
    { //SW1 GUARDA
        adc_dataSS3 = readADC0SS3(adc_dataSS3);
        //readADC0SS2(adc_dataSS2);
       //readADC0SS3(adc_dataSS3);
       
//        GPIOF->DATA = (1<<1); // RED LED ENCIENDE

//        writeString(UART_0,"ADC: ");

//se ocupa esta debe activarse si se escogen otros canales 
        //ltoa(adc_dataSS3[0],num_char,10);
        //printString(num_char); //Imprimimos el valor del SS3
        //printChar(',');


        enviarADCSS2SS3(adc_dataSS3); //Imprimimos el valor del SS3 
        //enviarADCSS2SS1(adc_dataSS1); //Imprimimos el valor del SS1
        printChar('\n');
    
//        GPIOF->DATA = (0<<1); // RED LED APAGA
        GPIOB->ICR |= (1<<3); //Limpiamos
    }
}

void enviarADCSS2SS3(int adc_data)
{
    int i=0;    char num_char[16];
    int num0 =0;
    int num1 =0;
    int num2 =0;
    int num3 =0;

    
    
        char numeros[9];
         numeros[0]='0';
         numeros[1]='1';
         numeros[2]='2';
         numeros[3]='3';
         numeros[4]='4';
         numeros[5]='5';
         numeros[6]='6';
         numeros[7]='7';
         numeros[8]='8';
         numeros[9]='9';
    
        
    
        //ltoa(adc_data[i],num_char,10);
    //for(i=0;i<3;i++)
        //{
        //        ltoa(adc_data[i],num_char,10);
        
        num0=(adc_data/1000);   //1.3->1.0 //4.095  //3/1000=0.0003 //147/1000=0.147
        printChar(numeros[num0]); 
        
        num1=(adc_data/100);    //1.3->1.0 //40.95  //0.003         //1.47
        num1=num1-num0*10;                       //0                      //1-0=1
        printChar(numeros[num1]); //Imprimimos el valor del nodo
        
        num2=(adc_data/10);     // 13     //409.5   //0.3           //14.7
        num2=num2-(num0*100+num1*10);  //3      //409-(400+0)=9           //14-(0+10)=4
        printChar(numeros[num2]); //Imprimimos el valor del nodo
        
        num3=(adc_data/1);     // 130     //4095   //3              //147
        num3=num3-((num0*1000+num1*100+num2*10)); //3 //4095-(4000+0+90)=5//147-(0+100+40)=7
        printChar(numeros[num3]); //Imprimimos el valor del nodo 
        
        printChar(',');
   
        //}


}