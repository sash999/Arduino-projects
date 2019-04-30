#include <avr/io.h>
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/sleep.h> // здесь описаны режимы сна
#include <avr/interrupt.h> // работа с прерываниями
#define F_CPU 1200000UL

volatile uint8_t i;
ISR (WDT_vect) {
 if ((++i%4) == 0)
         PORTB |= (1<<PB4); // включаем светодиод
 else
         PORTB &= ~(1<<PB4); // выключаем светодиод

         WDTCR |= (1<<WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
}

int main() {

        DDRB = (1<<PB4); // на этом пине висит светодиод
        i=0;

        //инициализация ватчдога
        wdt_reset(); // сбрасываем
        wdt_enable(WDTO_1S); // разрешаем ватчдог 1 сек
        WDTCR |= (1<<WDTIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
        sei(); // разрешаем прерывания

        set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную
        while(1) {
                sleep_enable(); // разрешаем сон
                sleep_cpu(); // спать!
        }
}
