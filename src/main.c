#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

void uart_putchar(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // wait until buffer empty
    UDR0 = c;
}

void uart_print(const char *s) {
    while (*s) uart_putchar(*s++);
}

void uart_print_uint8(uint8_t x) {
    if (x >= 100) uart_putchar('0' + x / 100);
    if (x >= 10)  uart_putchar('0' + (x / 10) % 10);
    uart_putchar('0' + x % 10);
}

void uart_init(void) {
    // 16 MHz clock, 9600 baud
    UBRR0H = 0;
    UBRR0L = 103;

    UCSR0A = 0;
    UCSR0B = (1 << TXEN0);                 // enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8N1
}

int main(void) {
  // uart_init();
  // _delay_ms(1000); // let bootloader finish


  //! DDR (Data Direction Register) determines the DIRECTION of the data (Input or Output)
  // const int8_t BUILTIN_LED = (1<<PB5); // built-in LED
  // DDRB |= BUILTIN_LED; 
  DDRB |= (1<<PB1); //? Set Pin 9

  //! Set all PORTD pins as outputs
  DDRD = 0xFF; // 1 byte outpud for DAC


  // uint8_t brightness = 0;
  uint8_t brightness = 0;
  int8_t step = 1;
  while (1)
  {
    // ! Output a byte directly to the ladder
    // PORTD = brightness % 0b1000; // mod 2^2. 2 bit (using 2 pins)
    PORTD = brightness;

      // uart_print_uint8(PORTD);
      // uart_print_uint8(brightness);
      // uart_print("\n");

    //! increment brightness
    // brightness++;
    brightness += step;
    // if (brightness == 0b00 || brightness == 0b11) step = -step;
    if (brightness == 0b000 || brightness == 0b111) step = -step;
    _delay_ms(500);
  }
  
}