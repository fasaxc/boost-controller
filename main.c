#include <avr/io.h>
#include <util/delay.h>

#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s

#define PWM_FREQ 62500
#define PWM_RESOLUTION (F_CPU / PWM_FREQ)
#define MIN_DUTY_CYCLE 0.40
#define MAX_DUTY_CYCLE 0.80
const uint8_t MIN_PWM_LEVEL = PWM_RESOLUTION * MIN_DUTY_CYCLE;
const uint8_t MAX_PWM_LEVEL = PWM_RESOLUTION * MAX_DUTY_CYCLE;

#define VREF 1.1
#define DESIRED_VOUT 20.0
#define DIVIDER_RATIO 30.0
#define DESIRED_ADC_IN_V (DESIRED_VOUT / DIVIDER_RATIO)

const uint8_t DESIRED_ADC_RESULT = 255 * DESIRED_ADC_IN_V / VREF;

#define DUTY_CYCLE_REG OCR0B

#define ADC_ENABLE() (ADCSRA |= _BV(ADEN))
#define ADC_START_CONVERSION() (ADCSRA |= _BV(ADSC))

int main(void) {
    /* Set A7 as an output.  (Needed for PWM.) */
    DDRA |= _BV(DD7);
    PORTA = 0;

    /* Let input power stabilize... */
    _delay_ms(500);

    /*
     * Configure Timer0 as a fast PWM.  It will
     * - turn on the output pin at the start of each cycle
     * - turn it off when the value hits DUTY_CYCLE_REG
     * - wrap to 0 when it hits OCR0A
     */
    TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    OCR0A = PWM_RESOLUTION;
    /* Start with 40% duty cycle and ramp up to avoid inrush. */
    DUTY_CYCLE_REG = (uint8_t)(PWM_RESOLUTION * 0.40);
    /* Set Timer0 clock source to be main oscillator. This enables the timer. */
    TCCR0B = _BV(CS00) | _BV(WGM02);

    /*
     * Turn on the ADC,
     * - use internal voltage ref.
     * - configure ADC0 as our source
     * - left-adjust the result, 8-bits is enough for us
     * - disable digital input buffer on pin
     * - enable the ADC.
     */
    ADMUX = /* REF = */ _BV(REFS1) | /* INPUT = */ 0;
    ADCSRA |= /* PRESCALER = 16 = 2^ */ 4;
    ADCSRB |= /* LEFT-ADJUST */ _BV(ADLAR);
    DDRA &= ~_BV(DD0);
    DIDR0 |= _BV(ADC0D);
    ADC_ENABLE();
    _delay_ms(1);

    /*
     * Now enter our main loop.  Monitor the output voltage and manipulate
     * the duty cycle to control it.
     */
    while (1) {
      /* Wait for the Timer0 to overflow... */
      loop_until_bit_is_set(TIFR0, TOV0);
      /* End of our OFF period, should be peak voltage... */
      TIFR0 |= _BV(TOV0);  /* Clear the flag. */

      /* Check the output voltage. */
      ADC_START_CONVERSION();
      loop_until_bit_is_clear(ADCSRA, ADSC);
      uint8_t adc_result = ADCH;

      if (adc_result < DESIRED_ADC_RESULT &&
          DUTY_CYCLE_REG < MAX_PWM_LEVEL) {
        DUTY_CYCLE_REG++;
      }
      else if (adc_result > DESIRED_ADC_RESULT &&
               DUTY_CYCLE_REG > MIN_PWM_LEVEL) {
        DUTY_CYCLE_REG--;
      }
    }
}
