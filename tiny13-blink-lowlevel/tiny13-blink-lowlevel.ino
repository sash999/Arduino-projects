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
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/sleep.h> // здесь описаны режимы сна
#include <avr/interrupt.h> // работа с прерываниями

#define LED_BIT _BV(PB3)

int main()
{
 set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 DDRB |= LED_BIT; // OUTPUT
 
        wdt_reset(); // сбрасываем
        wdt_enable(WDTO_2S); // разрешаем ватчдог 2 сек
        WDTCR |= (1<<WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
        sei(); // разрешаем прерывания

        set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
        while(1) {
                sleep_enable(); // разрешаем сон
                sleep_cpu(); // спать!
                PORTB ^= LED_BIT; // HIGH
                
        }
  
 

  
}
