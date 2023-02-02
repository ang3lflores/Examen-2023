#include "lib/include.h"
/* ConfigurarGPIOD()
 * Configura el PORTB para poder usar UN BOTON
 * EN EL PIN PB3.
 * RECUERDA QUE EN EL CIRCUITO SE LE DEBE
 * AGREGAR UNA RESISTENCIA PARA NO PONER EN CORTO
 * ESE PIN AL MOMENTO DE PRESIONAR EL BOTON
 */
extern void ConfigurarGPIOD(void)
{
    SYSCTL->RCGCGPIO|= (1<<1);
    //GPIOB->LOCK      = 0x4C4F434B;
    //GPIOB->CR        = (1<<3);
    GPIOB->DIR      |= (0<<3); // PB3 ENTRADA
    GPIOB->DEN      |= (1<<3);
    GPIOB->AFSEL    |= (0<<3);
    GPIOB->PUR      |= (1<<3);
   

    NVIC_SetPriority(GPIOB_IRQn, 4); //PRI=4 el 0 con mayor prioridad el 7 con menos
    NVIC_EnableIRQ(GPIOB_IRQn);
    GPIOB->IM  |= (0<<3); // Masked
    GPIOB->IS  |= (0<<3); // Edge-sensitive
    //GPIOB->IBE |= (1<<3); // Both edges
    GPIOB->IEV |= (0<<3); //FLANCO DE BAJADA
    GPIOB->RIS |= (0<<3); // Registro de estado
    GPIOB->IM  |= (1<<3); // Send
}

/*extern void Delay(void)
{
  unsigned long volatile time;
  time = 1600000;
  while(time)
  {
		time--;
  }
}
*/