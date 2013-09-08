#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRA |= _BV(DD0);
    while (1) {
        PORTA ^= _BV(PORT0);
        _delay_ms(200);
    }
}
