// vim: ts=2:sw=2
#include <msp430.h>
#include <legacymsp430.h>

#define MHZ               1

#define OLED_SCLK         BIT5
#define OLED_SDIN         BIT7
#define OLED_RES          BIT2
#define OLED_DC           BIT3
#define OLED_CS           BIT4
// this is fake output, not connected 
#define OLED_SDOUT        BIT6
#define OLED_CONTRAST     0x80
#define OLEDS             (OLED_RES | OLED_DC | OLED_CS)

#include "oled.h"

inline unsigned char oled_write_byte(unsigned char byte) {
  UCB0TXBUF = byte;
  while (UCB0STAT & UCBUSY);
  return UCB0RXBUF;
}

#include "pic_example1.h"

void main(void) {
  unsigned char offset = 0;
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL1 = CALBC1_1MHZ; // Set DCO
  DCOCTL = CALDCO_1MHZ;
 
  P1DIR |= (OLEDS | BIT0 | OLED_SCLK);
  P1OUT &= ~(OLEDS | BIT0);

  //USCI setup
  UCB0CTL1 = UCSWRST | UCSSEL_2; // stop USCI and choose SCLK as source
  UCB0CTL0 = UCCKPH | UCMODE_0 | UCMSB | UCSYNC | UCMST; // logic triggered before rising clock, msb, spi, master
  UCB0BR0 |= 1; // send bit on every clock tick
  UCB0BR1 = 0;
  P1SEL |= OLED_SCLK | OLED_SDIN | OLED_SDOUT; // enalbe USCI functions on ports 
  P1SEL2 |= OLED_SCLK | OLED_SDIN | OLED_SDOUT; // missing this on launchpad will make code not working
  UCB0CTL1 &= ~UCSWRST; // start USCI

  oled_initialize();
 
  while(1) {
    oled_display_image(pic_example1, offset++);
    offset %= 0x80;
    P1OUT ^= BIT0;
  }
}
