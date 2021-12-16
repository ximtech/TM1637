#include "TM1637.h"

#define TM_CLK_LOW()  LL_GPIO_ResetOutputPin(TM_PORT, TM_CLK_PIN)
#define TM_CLK_HIGH() LL_GPIO_SetOutputPin(TM_PORT, TM_CLK_PIN)
#define TM_DAT_LOW()  LL_GPIO_ResetOutputPin(TM_PORT, TM_DAT_PIN)
#define TM_DAT_HIGH() LL_GPIO_SetOutputPin(TM_PORT, TM_DAT_PIN)

// Instructions
#define TM_DATA_CMD 0x40
#define TM_DISP_CTRL 0x80
#define TM_ADDR_CMD 0xC0

// Data command set
#define TM_WRITE_DISP 0x00
#define TM_FIXED_ADDR 0x04

// Display control command
#define TM_DISP_PWM_MASK 0x07 // First 3 bits are brightness (PWM controlled)
#define TM_DISP_ENABLE 0x08

#define DELAY_US 1

static uint8_t dotMask = 0;

static void start();
static void stop();
static void send(uint8_t b);
static void send_cmd(uint8_t cmd);
static void send_data(uint8_t addr, uint8_t data);


void initTM1637() {
    dwtDelayInit();

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_AHB1_GRP1_EnableClock(TM_PERIPHERAL);// GPIO Ports Clock Enable

    LL_GPIO_ResetOutputPin(TM_PORT, TM_CLK_PIN | TM_DAT_PIN);

    GPIO_InitStruct.Pin = TM_CLK_PIN | TM_DAT_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(TM_PORT, &GPIO_InitStruct);

    LL_GPIO_SetOutputPin(TM_PORT, TM_CLK_PIN);

	send_cmd(TM_DATA_CMD | TM_WRITE_DISP);
	send_cmd(TM_DISP_CTRL | TM_DISP_ENABLE | TM_DISP_PWM_MASK);
    clearTM1637();
}

void clearTM1637() {
	for (uint8_t i = 0; i < TM1637_DIGITS; ++i) {
        send_data(i, 0x00);
	}
}

void setByteTM1637(uint8_t position, uint8_t byte) {
	send_data(position, byte | (dotMask & (1 << position) ? TM_DOT : 0));
}

void setDigitTM1637(uint8_t position, uint8_t digit) {
    setByteTM1637(position, TM_DIGITS[digit & 0xF]);
}

void setNumberTM1637(uint32_t number) {
	uint8_t offset = TM1637_DIGITS - 1;
	do {
        setDigitTM1637(offset--, number % 10);
		number /= 10;
	} while (number && offset != 0xFF);
}

void setNumberPadTM1637(uint32_t number, uint8_t offset, uint8_t width) {
	do {
        setDigitTM1637(offset--, number % 10);
		number /= 10;
	} while (number && width-- && offset != 0xFF);

	while (width-- && offset != 0xFF) {
        setByteTM1637(offset--, TM_PAD_SPACE);
	}
}

void setNumberHexTM1637(uint32_t number, uint8_t offset, uint8_t width) {
	do {
        setDigitTM1637(offset--, number & 0x0F);
		number >>= 4;
	} while (number && width-- && offset != 0xFF);

	while (width-- && offset != 0xFF) {
        setByteTM1637(offset--, TM_PAD_SPACE);
	}
}

void setCharTM1637(uint8_t position, char ch) {
	const uint8_t byte = TM1637_map_char(ch);
	if (byte || ch == ' ') {
        setByteTM1637(position, byte);
	} else if (ch >= 'a' && ch <= 'z') {
        setByteTM1637(position, TM_DIGITS[ch - 'a' + 10]);
	} else if (ch >= 'A' && ch <= 'Z') {
        setByteTM1637(position, TM_DIGITS[ch - 'A' + 10]);
	} else if (ch >= '0' && ch <= '9') {
        setByteTM1637(position, TM_DIGITS[ch - '0']);
	}
}

void setStringTM1637(const char *value) {
	uint8_t offset = 0;
	while (*value) {
        setCharTM1637(offset++, *value++);
	}
}

void scrollStringTM1637(const char *value, uint16_t scrollSpeed) {
    for (uint32_t i = 0; value[i] != '\0'; ++i) {
        for (uint8_t j = 0; j < TM1637_DIGITS; ++j) {
            setCharTM1637(j, value[j + i]);

            if (value[j + i + 1] == '\0') {
                delay_ms(scrollSpeed);  // last symbol display delay
                return;
            }
        }
        delay_ms(scrollSpeed);
    }
}

void setDotsTM1637(uint8_t mask) {
	dotMask = mask;
}

void setBrightnessTM1637(uint8_t brightness) {
	send_cmd(TM_DISP_CTRL | TM_DISP_ENABLE | (brightness & TM_DISP_PWM_MASK));
}

static void start() {
	TM_CLK_HIGH();
	TM_DAT_HIGH();
	delay_us(DELAY_US);

	TM_DAT_LOW();
	delay_us(DELAY_US);
}

static void stop() {
	TM_CLK_LOW();
	delay_us(DELAY_US);

	TM_CLK_HIGH();
	TM_DAT_LOW();
	delay_us(DELAY_US);

	TM_DAT_HIGH();
}

static void send(uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++, byte >>= 1) {// Clock data bits
		TM_CLK_LOW();
		if (byte & 1) {
            TM_DAT_HIGH();
		} else {
            TM_DAT_LOW();
		}
		delay_us(DELAY_US);

		TM_CLK_HIGH();
		delay_us(DELAY_US);
	}

	// Clock out ACK bit; not checking if it worked...
	TM_CLK_LOW();
	TM_DAT_LOW();
	delay_us(DELAY_US);

	TM_CLK_HIGH();
	delay_us(DELAY_US);
}

static void send_cmd(uint8_t cmd) {
	start();
	send(cmd);
	stop();
}

static void send_data(uint8_t addr, uint8_t data) {
	send_cmd(TM_DATA_CMD | TM_FIXED_ADDR);

	start();
	send(TM_ADDR_CMD | addr);
	send(data);
	stop();

    delay_us(DELAY_US);
}