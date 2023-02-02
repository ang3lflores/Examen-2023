#ifndef ADC_H_
#define ADC_H_

#define _125ksps              0x1
#define _250ksps              0x3
#define _500ksps              0x5
#define _1Msps                0x7

extern void Configurar_ADC0(void);
extern void Configurar_ADCM0SS2(void);
//extern int readADC(void);
//extern readADC0SS2(int adc_data[]);

extern void Configurar_SysTick(int clock);
extern void Delay_s(uint32_t delay);
extern void Delay_ms(uint32_t delay);
extern void Delay_us(uint32_t delay);

extern void Configurar_ADCM0SS3(void);
extern void Configurar_ADCM0SS2(void);
extern void Configurar_ADCM0SS1(void);
extern int readADC0SS3(int adc_data);
extern readADC0SS2( int  adc_data[]);
extern readADC0SS1( int  adc_data[]);

#endif /* ADC_ADC_H_ */
