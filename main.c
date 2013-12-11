// vim: ts=2:sw=2
#include <msp430.h>
#include <legacymsp430.h>

#define MHZ               1

#define OLED_SCLK         BIT5
#define OLED_SDIN         BIT7
#define OLED_RES          BIT2
#define OLED_DC           BIT3
#define OLED_CS           BIT4
#define OLED_CONTRAST     0x80
#define OLEDS             (OLED_SCLK | OLED_SDIN | OLED_RES | OLED_DC | OLED_CS)

#include "oled.h"

inline unsigned char oled_write_byte(unsigned char byte) {
  unsigned int j;
  for (j = 0; j < 8; j++) {
    P1OUT &= ~OLED_SCLK;
    if ((byte << j) & 0x80) {
      P1OUT |= OLED_SDIN;
    } else {
      P1OUT &= ~OLED_SDIN;
    }
    P1OUT |= OLED_SCLK;
  }
  return 0;
}

#include "pic_example1.h"

void main(void) {
  unsigned char offset = 0;
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL1 = CALBC1_1MHZ; // Set DCO
  DCOCTL = CALDCO_1MHZ;
 
  P1DIR |= (OLEDS | BIT0);
  P1OUT &= ~(OLEDS | BIT0);

  oled_initialize();
 
  while(1) {
    oled_display_image(pic_example1, offset++);
    offset %= 0x80;
    P1OUT ^= BIT0;
  }
}
