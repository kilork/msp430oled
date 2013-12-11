void oled_initialize();
inline void oled_chip_select();
inline void oled_chip_deselect();
void oled_write_commands(unsigned char* command, unsigned int size);
void oled_write_data(unsigned char* data, unsigned int size);
void oled_display_image(unsigned char* data, unsigned char offset);

// must be implemented
inline unsigned char oled_write_byte(unsigned char byte);

void oled_initialize() {
  unsigned char commands[] = {
    0xae,//--turn off oled panel

    0xd5,//--set display clock divide ratio/oscillator frequency
    0x80,//--set divide ratio

    0xa8,//--set multiplex ratio(1 to 64)
    0x1f,//--1/32 duty

    0xd3,//-set display offset
    0x00,//-not offset

    0x8d,//--set Charge Pump enable/disable
    0x14,//--set(0x10) disable

    0x40,//--set start line address

    0xa6,//--set normal display

    0xa4,//Disable Entire Display On

    0xa1,//--set segment re-map 128 to 0

    0xC8,//--Set COM Output Scan Direction 64 to 0

    0xda,//--set com pins hardware configuration
    0x42,

    0x81,//--set contrast control register
    OLED_CONTRAST,

    0xd9,//--set pre-charge period
    0xf1,

    0xdb,//--set vcomh
    0x40,

    0xaf//--turn on oled panel
  };
  P1OUT |= OLED_RES;
  __delay_cycles(20 * MHZ);
  P1OUT &= ~OLED_RES;
  __delay_cycles(200 * MHZ);
  P1OUT |= OLED_RES;
  __delay_cycles(200 * MHZ);
  oled_chip_select();
  oled_write_commands(commands, sizeof(commands));
  oled_chip_deselect();
}

void oled_write(unsigned char* data, unsigned int size) {
  unsigned int i;
  for (i = 0; i < size; i++) {
    oled_write_byte(data[i]);
  }
}

void oled_write_commands(unsigned char* commands, unsigned int size) {
  P1OUT &= ~OLED_DC;
  oled_write(commands, size);
}

void oled_write_data(unsigned char* data, unsigned int size) {
  P1OUT |= OLED_DC;
  oled_write(data, size);
}

inline void oled_set_page_address(unsigned char page) {
  oled_write_byte(0xb0 | page);
}

inline void oled_set_column_address(unsigned char column) {
  oled_write_byte(0x10 | (column >> 4));
  oled_write_byte(0x0f & column);
}

inline void oled_chip_select() {
  P1OUT &= ~OLED_CS;
}

inline void oled_chip_deselect() {
  P1OUT |= OLED_CS;
}

void oled_display_image(unsigned char* data, unsigned char offset) {
  unsigned char i;
  unsigned char* start;
  oled_chip_select();
	for(i = 0; i < 4; i++) {
    P1OUT &= ~OLED_DC;
    oled_set_page_address(i);
    oled_set_column_address(0x00);
    P1OUT |= OLED_DC;
    start = data + 0x80 * i;
    oled_write(start + offset, 0x80 - offset);
    if (offset) {
      oled_write(start, offset);
    }
	}
  oled_chip_deselect();
}

