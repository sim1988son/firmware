
#define BUTTON_PIN 0
#define PIN_BUTTON2 21             // Second built-in button
#define ALT_BUTTON_PIN PIN_BUTTON2 // Send the up event

// I2C
#define I2C_SDA SDA
#define I2C_SCL SCL
// GPS
#undef GPS_RX_PIN
#undef GPS_TX_PIN
#define GPS_RX_PIN 44
#define GPS_TX_PIN 43

#define PIN_GPS_EN 5
#define GPS_EN_ACTIVE 1
#define GPS_L76K

// ST7789 TFT LCD
#define ST7789_CS 39
#define ST7789_RS 47  // DC
#define ST7789_SDA 48 // MOSI
#define ST7789_SCK 38
#define ST7789_RESET 40
#define ST7789_MISO 4
#define ST7789_BUSY -1
#define ST7789_BL 17
#define ST7789_SPI_HOST SPI3_HOST
// #define TFT_BL 17
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 16000000
#define TFT_HEIGHT 320
#define TFT_WIDTH 176
#define TFT_OFFSET_X 35
#define TFT_OFFSET_Y 0
#define TFT_OFFSET_ROTATION 2 //=====

// #define TFT_MESH COLOR565(0x6B, 0xB2, 0x42) //4f9924
// #define TFT_MESH COLOR565(0x4F, 0x99, 0x24)
#define TFT_MESH_OVERRIDE COLOR565(0x4F, 0x99, 0x24)

#define VTFT_CTRL 7

#define SCREEN_ROTATE
#define SCREEN_TRANSITION_FRAMERATE 15
#define BRIGHTNESS_DEFAULT 68 // Medium Low Brightnes   

#define PIN_BUZZER 4

// SPI
#define SPI_INTERFACES_COUNT 2
#define PIN_SPI_MISO 11
#define PIN_SPI_MOSI 10
#define PIN_SPI_SCK 9

// Power
#define VEXT_ENABLE 5
#define VEXT_ON_VALUE HIGH
#define ADC_CTRL 46
#define ADC_CTRL_ENABLED HIGH
#define BATTERY_PIN 6
#define ADC_CHANNEL ADC1_GPIO6_CHANNEL
#define ADC_MULTIPLIER 4.9 * 1.03        // Voltage divider is roughly 1:1
#define ADC_ATTENUATION ADC_ATTEN_DB_2_5 // Voltage divider output is quite high

// LoRa
#define USE_SX1262

#define LORA_DIO0 RADIOLIB_NC // a No connect on the SX1262 module
#define LORA_RESET 12
#define LORA_DIO1 14 // SX1262 IRQ
#define LORA_DIO2 13 // SX1262 BUSY
#define LORA_DIO3    // Not connected on PCB, but internally on the TTGO SX1262, if DIO3 is high the TXCO is enabled

#define LORA_SCK 9
#define LORA_MISO 11
#define LORA_MOSI 10
#define LORA_CS 8

#define SX126X_CS LORA_CS
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY LORA_DIO2
#define SX126X_RESET LORA_RESET

#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

#define INPUTBROKER_PEMATRIX_TYPE 1
#define CANNED_MESSAGE_MODULE_ENABLE 1
