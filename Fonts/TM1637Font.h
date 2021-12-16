#pragma once

#define TM_DOT 0x80

#define TM_MINUS 0x40
#define TM_PLUS 0x44
#define TM_BLANK 0x00
#define TM_DEGREES 0x63
#define TM_UNDERSCORE 0x08
#define TM_EQUALS 0x48
#define TM_CHAR_ERR 0x49

static inline uint8_t TM1637_map_char(const char ch) {
    switch (ch) {
        case '-':
            return TM_MINUS;
        case '+':
            return TM_PLUS;
        case ' ':
            return TM_BLANK;
        case '^':
            return TM_DEGREES;
        case '_':
            return TM_UNDERSCORE;
        case '=':
            return TM_EQUALS;
        default:
            return 0;
    }
}

//      Bits:                 Hex:
//        -- 0 --               -- 01 --
//       |       |             |        |
//       5       1            20        02
//       |       |             |        |
//        -- 6 --               -- 40 --
//       |       |             |        |
//       4       2            10        04
//       |       |             |        |
//        -- 3 --  .7           -- 08 --   .80

static const uint8_t TM_DIGITS[] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F, // 9

        0x77, // A
        0x7C, // b
        0x39, // C
        0x5E, // d
        0x79, // E
        0x71, // F

        // HEX DIGITS END

        0x3D, // G
        0x76, // H
        0x06, // I
        0x1F, // J
        0x76, // K (same as H)
        0x38, // L
        0x15, // M
        0x54, // n
        0x3F, // O
        0x73, // P
        0x67, // Q
        0x50, // r
        0x6D, // S
        0x78, // t
        0x3E, // U
        0x1C, // V
        0x2A, // W
        0x76, // X (same as H)
        0x6E, // Y
        0x5B  // Z
};