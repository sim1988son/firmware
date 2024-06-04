// For OLED LCD
#define I2C_SDA 21
#define I2C_SCL 22

// GPS
#undef GPS_RX_PIN
#undef GPS_TX_PIN
#define GPS_RX_PIN 12
#define GPS_TX_PIN 15
#define GPS_UBLOX

#define BUTTON_PIN 39 // The middle button GPIO on the T-Beam
#define ADC_CHANNEL ADC1_GPIO35_CHANNEL
#define ADC_MULTIPLIER 1.85 // (R1 = 470k, R2 = 680k)
#define EXT_PWR_DETECT 4    // Pin to detect connected external power source for LILYGO® TTGO T-Energy T18 and other DIY boards
#define EXT_NOTIFY_OUT 12   // Overridden default pin to use for Ext Notify Module (#975).
#define LED_PIN 2           // add status LED (compatible with core-pcb and DIY targets)

// #define BUTTON_NEED_PULLUP // if set we need to turn on the internal CPU pullup during sleep
#undef RF95_SCK
#undef RF95_MISO
#undef RF95_MOSI
#undef RF95_NSS

// Standard OLED I2C pins
#define I2C_SDA 21
#define I2C_SCL 22

// No button or battery pin as far as I know
#undef BUTTON_PIN
#undef BATTERY_PIN

#define LORA_SCK 18
#define LORA5_MISO 19
#define LORA_MOSI 23
#define LORA_NSS 5

#define USE_SX1262
#define SX126X_CS 5 // FIXME - we really should define LORA_CS instead
#define SX126X_DIO1 26
#define SX126X_BUSY 4
#define SX126X_RESET 25
#define SX126X_RXEN 0
#define SX126X_TXEN 2
#define SX126X_E22

// supported modules list
#define USE_RF95 // RFM95/SX127x
#define USE_SX1262
#define USE_SX1268
#define USE_LLCC68

// Set lora.tx_power to 13 for Hydra or other E22 900M30S target due to PA
#define SX126X_MAX_POWER 22

#ifdef EBYTE_E22
// Internally the TTGO module hooks the SX126x-DIO2 in to control the TX/RX switch
// (which is the default for the sx1262interface code)
#define SX126X_DIO3_TCXO_VOLTAGE 1.8
#endif
