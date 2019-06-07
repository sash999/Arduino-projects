/*
 * Tiny13_app1.c
 *
 * Created: 09.04.2019 15:33:00
 * Author : sasha
 */ 

// 1.2 MHz (default) built in resonator
#define F_CPU 1200000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define LED_BIT _BV(PB4)

int main()
{
 set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 DDRB |= LED_BIT; // OUTPUT

  for (int i=3; i>0; i--)
  {
    PORTB |= LED_BIT; // HIGH
    _delay_ms(1000);
    PORTB &= ~LED_BIT; // LOW
    _delay_ms(1000);
  }
 // sleep_enable();
 // sleep_cpu();
 while(1)
 {
   PORTB |= LED_BIT; // HIGH
    _delay_ms(300);
    PORTB &= ~LED_BIT; // LOW
    _delay_ms(100);
 }

  
}
