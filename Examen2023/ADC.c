#include "lib/include.h"

extern void Configurar_SysTick(int clock){
//    Time delay = (Reload Value + 1) / XTAL
//    Reload Value = (XTAL * Time delay) - 1
    int ms = 0;
    ms = (clock * 0.000001) - 1; //1us
    SysTick->CTRL = (0<<0);
    SysTick->LOAD = ms;
    SysTick->VAL  = 0x0;
    //Precision internal oscillator (PIOSC) divided by 4
    //              CLK_SRC  ENABLE
    SysTick->CTRL = (1<<2) | (1<<0);
}

extern void Delay_us(uint32_t delay){
    uint32_t i;
    for(i=0;i<delay;i++){
        while((SysTick->CTRL & 0x10000)==0);
    }
}

extern void Delay_ms(uint32_t delay){
    delay = delay * 1000;
    Delay_us(delay);
}

extern void Delay_s(uint32_t delay){
    delay = delay * 1000;
    Delay_ms(delay);
}

extern void Configurar_ADCM0SS3(void){ //Setup_ADC_SS3 AIN10
    //Se va a configurar A UNA VELOCIDAD DE 1 Msps
    //SS3 activacion del convertido por software

    // 1. Enable the ADC clock (SYSCTL_RGCGADC)
    SYSCTL->RCGCADC |= (1<<1); // Enable modulo 1
    SYSCTL->RCGCGPIO |= (1<<3); // Enable Puerto D
    GPIOD->DIR      |= (0<<3) ; // PIN 3
    GPIOD->DEN      |= (0<<3);
    GPIOD->AFSEL    |= (0<<3);
    GPIOD->AMSEL    |= (1<<3);
   
   
    /*
     * Se configuran los pines a usar:
     * los secuenciadores pueden ser multicanales
     * SS3 --> realiza 1 muestra
     * SS2 --> realiza 4 muestras
     * SS1 --> realiza 4 muestras
     * SS0 --> realiza 8 muestras
     * Por lo que el SS0 el dato_adc0 = SS0/8;
     * Es decir que solo el SS3 no puede ser multicanal.
     */
//    while ((SYSCTL->PRADC & 0x10) != 0x10);
    // 0. Enable Sample Sequencer
    //        ADC0->ACTSS |= (0<<3);
    // 2. Configure max sampling rate (ADCn_PC_R)
    ADC1->PC = 0x07;
    /* Peripheral Configuration (ADCPC)
     * 0x1 125 ksps
     * 0x3 250 ksps
     * 0x5 500 ksps
     * 0x7 1 Msps
     */
    // 3. Configure Sequencer # priority
    ADC1->SSPRI = 0x0123;
    /* ADC Sample Sequencer Priority (ADCSSPRI)
     * Este registro establece la prioridad para cada uno de los
     * secuenciadores de muestras. Fuera de reinicio, el secuenciador
     * 0 tiene el la prioridad más alta y el secuenciador 3 tiene
     * la prioridad más baja.
     * SS3 - SS0
     */
    // 4. DConfigaracion del secuenciador
    ADC1->ACTSS &= ~(1<<3);
    // 5. Configure triggle mode
    ADC1->EMUX &= (0x0<<12);  // EM3: Software triggle
//    ADC0->EMUX &= (0xF<<12);  // EM3: Continuo
    // 6. Configure ADC chanel
    ADC1->SSMUX3 = (ADC1->SSMUX3 & 0xFFFFFFF0) + 4; // Use chanel 4
    // 7. Set flag on sample capture
    ADC1->SSCTL3 = 0x0006;    //no TS0 D0, yes IE0 END0
    /* ADC Sample Sequence Control 3 (ADCSSCTL3)
     * TS0  = 1st Sample Temp Sensor Select
     * IE0  = Sample Interrupt Enable
     * END0 = End of Sequence
     * D0   = Sample Differential Input Select
     */
    // 8. COnfigure interrupt
    ADC1->IM &= ~0x0008;      // Disable SS3 interrupt
    // 9. Enable Sample Sequencer
    ADC1->ACTSS |= (1<<3);
}

extern void Configurar_ADCM0SS2(void){ //Setup_ADC_SS2 AIN1, AIN2, AIN3
    //Se va a configurar A UNA VELOCIDAD DE 1 Msps
    //SS2 activacion del convertido por software

    // 1. Enable the ADC clock (SYSCTL_RGCGADC)
    SYSCTL->RCGCADC |= (1<<1); // Enable modulo 1
    SYSCTL->RCGCGPIO |= (1<<1); // Enable Puerto B
    GPIOB->DIR      |= (0<<5) ; // PIN 5
    GPIOB->DEN      |= (0<<5);
    GPIOB->AFSEL    |= (0<<5);
    GPIOB->AMSEL    |= (1<<5);
    // 1. Enable the ADC clock (SYSCTL_RGCGADC)
    SYSCTL->RCGCADC |= (1<<0); // Enable modulo 0
    SYSCTL->RCGCGPIO |= (1<<4); // Enable Puerto E
    GPIOE->DIR      |= (0<<4) | (0<<2) | (0<<0); // PE4 PE2
    GPIOE->DEN      |= (0<<4) | (0<<2) | (0<<0); //  PE0
    GPIOE->AFSEL    |= (0<<4) | (0<<2) | (0<<0);
    GPIOE->AMSEL    |= (1<<4) | (1<<2) | (1<<0);
    while ((SYSCTL->PRADC & 0x01) != 0x01);

    // 0. Enable Sample Sequencer
        ADC0->ACTSS |= (0<<2);

    // 2. Configure max sampling rate (ADCn_PC_R)
    ADC0->PC = 0x07; //Peripheral Configuration (ADCPC) 500 ksps
    // 3. Configure Sequencer # priority
    ADC0->SSPRI = 0x3012; // ADC Sample Sequencer Priority (ADCSSPRI)
    // MAYOR -> SS3 -> SS2 -> SS1 -> SS0 -> MENOR
    // 4. DConfigaracion del secuenciador
    ADC0->ACTSS &= ~(1<<2);
    // 5. Configure triggle mode
    ADC0->EMUX &= (0x0<<8);    // EM2: Software triggle
//    ADC0->EMUX &= (0xF<<12);  // EM3: Continuo
    // 6. Configure ADC chanel
    ADC0->SSMUX2  = (ADC0->SSMUX2 & 0xFFFF0000) + 1; // Use chanel 1 PE2
    ADC0->SSMUX2 |= (2<<4);                          // Use chanel 2 PE1
    ADC0->SSMUX2 |= (3<<8);                          // Use chanel 3 PE0
    //ADC0->SSMUX2 |= (9<<12);                         // Use chanel 9 PE4
    // 7. Set flag on sample capture
    ADC0->SSCTL2 |= (1<<9) | (1<<6) | (1<<2);  //no TS D, yes IE END
    /* ADC Sample Sequence Control
     * TS  = 1st Sample Temp Sensor Select
     * IE  = Sample Interrupt Enable
     * END = End of Sequence
     * D   = Sample Differential Input Select
     */
    // 8. COnfigure interrupt
    ADC0->IM &= ~0x0004;   // Disable SS2 interrupt
    // 9. Enable Sample Sequencer
    ADC0->ACTSS |= (1<<2);
}
extern void Configurar_ADCM0SS1(void){ //Setup_ADC_SS1 AIN4, AIN5, AIN6 y AIN7
    //Se va a configurar A UNA VELOCIDAD DE 1 Msps
    //SS1 activacion del convertido por software

    // 1. Enable the ADC clock (SYSCTL_RGCGADC)
    SYSCTL->RCGCADC |= (1<<1); // Enable modulo 1
    SYSCTL->RCGCGPIO |= (1<<1); // Enable Puerto B
    GPIOB->DIR      |= (0<<5) ; // PIN 5
    GPIOB->DEN      |= (0<<5);
    GPIOB->AFSEL    |= (0<<5);
    GPIOB->AMSEL    |= (1<<5);
    // 1. Enable the ADC clock (SYSCTL_RGCGADC)
//    SYSCTL->RCGCADC |= (1<<0); // Enable modulo 0
    SYSCTL->RCGCGPIO |= (1<<3); // Enable Puerto D
    GPIOD->DIR      |= (0<<3) | (0<<2); // PD3 PD2
    GPIOD->DEN      |= (0<<3) | (0<<2); // 
    GPIOD->AFSEL    |= (0<<3) | (0<<2);
    GPIOD->AMSEL    |= (1<<3) | (1<<2);
    while ((SYSCTL->PRADC & 0x01) != 0x01);
    // 2. Configure max sampling rate (ADCn_PC_R)
//    ADC0->PC = 0x07; //Peripheral Configuration (ADCPC) 500 ksps
    // 3. Configure Sequencer # priority
//    ADC0->SSPRI = 0x0123; // ADC Sample Sequencer Priority (ADCSSPRI)
    // MAYOR -> SS3 -> SS2 -> SS1 -> SS0 -> MENOR

    // 0. Enable Sample Sequencer
        ADC0->ACTSS |= (0<<1);

    // 4. DConfigaracion del secuenciador
    ADC0->ACTSS &= ~(1<<1);
    // 5. Configure triggle mode
    ADC0->EMUX &= (0x0<<4);    // EM1: Software triggle
//    ADC0->EMUX &= (0xF<<12);  // EM3: Continuo
    // 6. Configure ADC chanel
    ADC0->SSMUX1  = (ADC0->SSMUX1 & 0xFFFF0000) + 0xB54; // Use chanel 4 PD3
//    ADC0->SSMUX1 |= (5<<4);                          // Use chanel 5 PD2
//    ADC0->SSMUX1 |= (11<<8);                          // Use chanel 11 PB5
    // 7. Set flag on sample capture
    ADC0->SSCTL1 |= (1<<9) | (1<<6) | (1<<2);  //no TS0 D0, yes IE0 END0
    /* ADC Sample Sequence Control 3 (ADCSSCTL3)
     * TS0  = 1st Sample Temp Sensor Select
     * IE0  = Sample Interrupt Enable
     * END0 = End of Sequence
     * D0   = Sample Differential Input Select
     */
    // 8. COnfigure interrupt
    ADC0->IM &= ~(1<<1);   // Disable SS1 interrupt
    // 9. Enable Sample Sequencer
    ADC0->ACTSS |= (1<<1);
}
extern int readADC0SS3(int adc_data)
{
    //int adc_data;
    // Inicia convercion SS3
    ADC1->PSSI = (1<<3); // EM3: Software triggle
    // Waiting for conversion done
    while ((ADC1->RIS & 0x08) != 0x08);
    adc_data = ADC1->SSFIFO3 & 0xFFF;     // Read 12-bit result
    ADC1->ISC |= (0<<3); // EM3: Software triggle
    return adc_data;
}
extern readADC0SS2(int adc_data[]){
//    int i;
    // Inicia convercion SS2
    ADC0->PSSI = (1<<2); // EM2: Software triggle
    // Waiting for conversion done
    while ((ADC0->RIS & (1<<2)) != (1<<2));
    adc_data[0] = ADC0->SSFIFO2 & 0xFFF;
    //Delay_ms(20);
    adc_data[1] = ADC0->SSFIFO2 & 0xFFF;
    //Delay_ms(20);
    adc_data[2] = ADC0->SSFIFO2 & 0xFFF;
    //Delay_ms(20);
    adc_data[3] = ADC0->SSFIFO2 & 0xFFF;
    //Delay_ms(20);
//    for (i=0;i<=4;i++)  *(adc_data + i) = ADC0->SSFIFO2 & 0xFFF;

    ADC0->ISC |= (1<<2); // EM2: Software triggle
}
extern readADC0SS1(int adc_data[]){
    // Inicia convercion SS2
    ADC0->PSSI = (1<<1); // EM2: Software triggle
    // Waiting for conversion done
    while ((ADC0->RIS & (1<<1)) != (1<<1));
//    Delay_ms(10);
    adc_data[0] = ADC0->SSFIFO1 & 0xFFF;     // Read 12-bit result
    //Delay_ms(20);
    adc_data[1] = ADC0->SSFIFO1 & 0xFFF;     // Read 12-bit result
    //Delay_ms(20);
    adc_data[2] = ADC0->SSFIFO1 & 0xFFF;     // Read 12-bit result
    //Delay_ms(20);
    adc_data[3] = ADC0->SSFIFO1 & 0xFFF;     // Read 12-bit result
    //Delay_ms(20);
    ADC0->ISC |= (1<<1);  // EM2: Software triggle
}