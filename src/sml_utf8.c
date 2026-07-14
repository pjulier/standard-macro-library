/**
 * @file sml_utf8.c
 * @brief Utf-8 validation and decoding
 * 
 * Based on shif-based DFA approach by:
 * https://github.com/chansen/c-utf8
 * and
 * https://gist.github.com/pervognsen/218ea17743e1442e59bb60d29b1aa725 
 * 
 */

#include "SML/sml_utf8.h"

#define SML_S_ERROR   SML_UTF8_ERROR
#define SML_S_OK      SML_UTF8_OK
#define SML_S_TAIL1  12
#define SML_S_TAIL2  18
#define SML_S_TAIL3  24
#define SML_S_E0     30
#define SML_S_ED     36
#define SML_S_F0     42
#define SML_S_F4     48

#define SML_DFA_ROW(mask_, f4_, f0_, ed_, e0_, tail3_, tail2_, tail1_, ok_, error_) \
  ( ((uint64_t)(mask_)   << 56) \
  | ((uint64_t)(f4_)     << SML_S_F4) \
  | ((uint64_t)(f0_)     << SML_S_F0) \
  | ((uint64_t)(ed_)     << SML_S_ED) \
  | ((uint64_t)(e0_)     << SML_S_E0) \
  | ((uint64_t)(tail3_)  << SML_S_TAIL3) \
  | ((uint64_t)(tail2_)  << SML_S_TAIL2) \
  | ((uint64_t)(tail1_)  << SML_S_TAIL1) \
  | ((uint64_t)(ok_)     << SML_S_OK) \
  | ((uint64_t)(error_)  << SML_S_ERROR))

/*
 *                     Value mask for this byte | --> Previous state
 * Incoming byte (class)                   mask     SML_S_F4     SML_S_F0     SML_S_ED     SML_S_E0  SML_S_TAIL3  SML_S_TAIL2  SML_S_TAIL1  SML_S_OK     SML_S_ERROR
 */
#define SML_DFA_ERROR         SML_DFA_ROW( 0x00, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR)
#define SML_DFA_ASCII         SML_DFA_ROW( 0x7F, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_OK,    SML_S_ERROR)
#define SML_DFA_HEAD2         SML_DFA_ROW( 0x1F, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_TAIL1, SML_S_ERROR)
#define SML_DFA_HEAD3         SML_DFA_ROW( 0x0F, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_TAIL2, SML_S_ERROR)
#define SML_DFA_HEAD4         SML_DFA_ROW( 0x07, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_TAIL3, SML_S_ERROR)
#define SML_DFA_E0            SML_DFA_ROW( 0x0F, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_E0,    SML_S_ERROR)
#define SML_DFA_ED            SML_DFA_ROW( 0x0F, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ED,    SML_S_ERROR)
#define SML_DFA_F0            SML_DFA_ROW( 0x07, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_F0,    SML_S_ERROR)
#define SML_DFA_F4            SML_DFA_ROW( 0x07, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_ERROR, SML_S_F4,    SML_S_ERROR)
#define SML_DFA_80_8F         SML_DFA_ROW( 0x3F, SML_S_TAIL2, SML_S_ERROR, SML_S_TAIL1, SML_S_ERROR, SML_S_TAIL2, SML_S_TAIL1, SML_S_OK,    SML_S_ERROR, SML_S_ERROR)
#define SML_DFA_90_9F         SML_DFA_ROW( 0x3F, SML_S_ERROR, SML_S_TAIL2, SML_S_TAIL1, SML_S_ERROR, SML_S_TAIL2, SML_S_TAIL1, SML_S_OK,    SML_S_ERROR, SML_S_ERROR)
#define SML_DFA_A0_BF         SML_DFA_ROW( 0x3F, SML_S_ERROR, SML_S_TAIL2, SML_S_ERROR, SML_S_TAIL1, SML_S_TAIL2, SML_S_TAIL1, SML_S_OK,    SML_S_ERROR, SML_S_ERROR)

static const uint64_t g_sml_utf8_dfa[256] = {
    [0x00] = SML_DFA_ASCII, [0x01] = SML_DFA_ASCII, [0x02] = SML_DFA_ASCII, [0x03] = SML_DFA_ASCII,
    [0x04] = SML_DFA_ASCII, [0x05] = SML_DFA_ASCII, [0x06] = SML_DFA_ASCII, [0x07] = SML_DFA_ASCII,
    [0x08] = SML_DFA_ASCII, [0x09] = SML_DFA_ASCII, [0x0A] = SML_DFA_ASCII, [0x0B] = SML_DFA_ASCII,
    [0x0C] = SML_DFA_ASCII, [0x0D] = SML_DFA_ASCII, [0x0E] = SML_DFA_ASCII, [0x0F] = SML_DFA_ASCII,
    [0x10] = SML_DFA_ASCII, [0x11] = SML_DFA_ASCII, [0x12] = SML_DFA_ASCII, [0x13] = SML_DFA_ASCII,
    [0x14] = SML_DFA_ASCII, [0x15] = SML_DFA_ASCII, [0x16] = SML_DFA_ASCII, [0x17] = SML_DFA_ASCII,
    [0x18] = SML_DFA_ASCII, [0x19] = SML_DFA_ASCII, [0x1A] = SML_DFA_ASCII, [0x1B] = SML_DFA_ASCII,
    [0x1C] = SML_DFA_ASCII, [0x1D] = SML_DFA_ASCII, [0x1E] = SML_DFA_ASCII, [0x1F] = SML_DFA_ASCII,
    [0x20] = SML_DFA_ASCII, [0x21] = SML_DFA_ASCII, [0x22] = SML_DFA_ASCII, [0x23] = SML_DFA_ASCII,
    [0x24] = SML_DFA_ASCII, [0x25] = SML_DFA_ASCII, [0x26] = SML_DFA_ASCII, [0x27] = SML_DFA_ASCII,
    [0x28] = SML_DFA_ASCII, [0x29] = SML_DFA_ASCII, [0x2A] = SML_DFA_ASCII, [0x2B] = SML_DFA_ASCII,
    [0x2C] = SML_DFA_ASCII, [0x2D] = SML_DFA_ASCII, [0x2E] = SML_DFA_ASCII, [0x2F] = SML_DFA_ASCII,
    [0x30] = SML_DFA_ASCII, [0x31] = SML_DFA_ASCII, [0x32] = SML_DFA_ASCII, [0x33] = SML_DFA_ASCII,
    [0x34] = SML_DFA_ASCII, [0x35] = SML_DFA_ASCII, [0x36] = SML_DFA_ASCII, [0x37] = SML_DFA_ASCII,
    [0x38] = SML_DFA_ASCII, [0x39] = SML_DFA_ASCII, [0x3A] = SML_DFA_ASCII, [0x3B] = SML_DFA_ASCII,
    [0x3C] = SML_DFA_ASCII, [0x3D] = SML_DFA_ASCII, [0x3E] = SML_DFA_ASCII, [0x3F] = SML_DFA_ASCII,
    [0x40] = SML_DFA_ASCII, [0x41] = SML_DFA_ASCII, [0x42] = SML_DFA_ASCII, [0x43] = SML_DFA_ASCII,
    [0x44] = SML_DFA_ASCII, [0x45] = SML_DFA_ASCII, [0x46] = SML_DFA_ASCII, [0x47] = SML_DFA_ASCII,
    [0x48] = SML_DFA_ASCII, [0x49] = SML_DFA_ASCII, [0x4A] = SML_DFA_ASCII, [0x4B] = SML_DFA_ASCII,
    [0x4C] = SML_DFA_ASCII, [0x4D] = SML_DFA_ASCII, [0x4E] = SML_DFA_ASCII, [0x4F] = SML_DFA_ASCII,
    [0x50] = SML_DFA_ASCII, [0x51] = SML_DFA_ASCII, [0x52] = SML_DFA_ASCII, [0x53] = SML_DFA_ASCII,
    [0x54] = SML_DFA_ASCII, [0x55] = SML_DFA_ASCII, [0x56] = SML_DFA_ASCII, [0x57] = SML_DFA_ASCII,
    [0x58] = SML_DFA_ASCII, [0x59] = SML_DFA_ASCII, [0x5A] = SML_DFA_ASCII, [0x5B] = SML_DFA_ASCII,
    [0x5C] = SML_DFA_ASCII, [0x5D] = SML_DFA_ASCII, [0x5E] = SML_DFA_ASCII, [0x5F] = SML_DFA_ASCII,
    [0x60] = SML_DFA_ASCII, [0x61] = SML_DFA_ASCII, [0x62] = SML_DFA_ASCII, [0x63] = SML_DFA_ASCII,
    [0x64] = SML_DFA_ASCII, [0x65] = SML_DFA_ASCII, [0x66] = SML_DFA_ASCII, [0x67] = SML_DFA_ASCII,
    [0x68] = SML_DFA_ASCII, [0x69] = SML_DFA_ASCII, [0x6A] = SML_DFA_ASCII, [0x6B] = SML_DFA_ASCII,
    [0x6C] = SML_DFA_ASCII, [0x6D] = SML_DFA_ASCII, [0x6E] = SML_DFA_ASCII, [0x6F] = SML_DFA_ASCII,
    [0x70] = SML_DFA_ASCII, [0x71] = SML_DFA_ASCII, [0x72] = SML_DFA_ASCII, [0x73] = SML_DFA_ASCII,
    [0x74] = SML_DFA_ASCII, [0x75] = SML_DFA_ASCII, [0x76] = SML_DFA_ASCII, [0x77] = SML_DFA_ASCII,
    [0x78] = SML_DFA_ASCII, [0x79] = SML_DFA_ASCII, [0x7A] = SML_DFA_ASCII, [0x7B] = SML_DFA_ASCII,
    [0x7C] = SML_DFA_ASCII, [0x7D] = SML_DFA_ASCII, [0x7E] = SML_DFA_ASCII, [0x7F] = SML_DFA_ASCII,
    [0x80] = SML_DFA_80_8F, [0x81] = SML_DFA_80_8F, [0x82] = SML_DFA_80_8F, [0x83] = SML_DFA_80_8F,
    [0x84] = SML_DFA_80_8F, [0x85] = SML_DFA_80_8F, [0x86] = SML_DFA_80_8F, [0x87] = SML_DFA_80_8F,
    [0x88] = SML_DFA_80_8F, [0x89] = SML_DFA_80_8F, [0x8A] = SML_DFA_80_8F, [0x8B] = SML_DFA_80_8F,
    [0x8C] = SML_DFA_80_8F, [0x8D] = SML_DFA_80_8F, [0x8E] = SML_DFA_80_8F, [0x8F] = SML_DFA_80_8F,
    [0x90] = SML_DFA_90_9F, [0x91] = SML_DFA_90_9F, [0x92] = SML_DFA_90_9F, [0x93] = SML_DFA_90_9F,
    [0x94] = SML_DFA_90_9F, [0x95] = SML_DFA_90_9F, [0x96] = SML_DFA_90_9F, [0x97] = SML_DFA_90_9F,
    [0x98] = SML_DFA_90_9F, [0x99] = SML_DFA_90_9F, [0x9A] = SML_DFA_90_9F, [0x9B] = SML_DFA_90_9F,
    [0x9C] = SML_DFA_90_9F, [0x9D] = SML_DFA_90_9F, [0x9E] = SML_DFA_90_9F, [0x9F] = SML_DFA_90_9F,
    [0xA0] = SML_DFA_A0_BF, [0xA1] = SML_DFA_A0_BF, [0xA2] = SML_DFA_A0_BF, [0xA3] = SML_DFA_A0_BF,
    [0xA4] = SML_DFA_A0_BF, [0xA5] = SML_DFA_A0_BF, [0xA6] = SML_DFA_A0_BF, [0xA7] = SML_DFA_A0_BF,
    [0xA8] = SML_DFA_A0_BF, [0xA9] = SML_DFA_A0_BF, [0xAA] = SML_DFA_A0_BF, [0xAB] = SML_DFA_A0_BF,
    [0xAC] = SML_DFA_A0_BF, [0xAD] = SML_DFA_A0_BF, [0xAE] = SML_DFA_A0_BF, [0xAF] = SML_DFA_A0_BF,
    [0xB0] = SML_DFA_A0_BF, [0xB1] = SML_DFA_A0_BF, [0xB2] = SML_DFA_A0_BF, [0xB3] = SML_DFA_A0_BF,
    [0xB4] = SML_DFA_A0_BF, [0xB5] = SML_DFA_A0_BF, [0xB6] = SML_DFA_A0_BF, [0xB7] = SML_DFA_A0_BF,
    [0xB8] = SML_DFA_A0_BF, [0xB9] = SML_DFA_A0_BF, [0xBA] = SML_DFA_A0_BF, [0xBB] = SML_DFA_A0_BF,
    [0xBC] = SML_DFA_A0_BF, [0xBD] = SML_DFA_A0_BF, [0xBE] = SML_DFA_A0_BF, [0xBF] = SML_DFA_A0_BF,
    [0xC0] = SML_DFA_ERROR, [0xC1] = SML_DFA_ERROR, [0xC2] = SML_DFA_HEAD2, [0xC3] = SML_DFA_HEAD2,
    [0xC4] = SML_DFA_HEAD2, [0xC5] = SML_DFA_HEAD2, [0xC6] = SML_DFA_HEAD2, [0xC7] = SML_DFA_HEAD2,
    [0xC8] = SML_DFA_HEAD2, [0xC9] = SML_DFA_HEAD2, [0xCA] = SML_DFA_HEAD2, [0xCB] = SML_DFA_HEAD2,
    [0xCC] = SML_DFA_HEAD2, [0xCD] = SML_DFA_HEAD2, [0xCE] = SML_DFA_HEAD2, [0xCF] = SML_DFA_HEAD2,
    [0xD0] = SML_DFA_HEAD2, [0xD1] = SML_DFA_HEAD2, [0xD2] = SML_DFA_HEAD2, [0xD3] = SML_DFA_HEAD2,
    [0xD4] = SML_DFA_HEAD2, [0xD5] = SML_DFA_HEAD2, [0xD6] = SML_DFA_HEAD2, [0xD7] = SML_DFA_HEAD2,
    [0xD8] = SML_DFA_HEAD2, [0xD9] = SML_DFA_HEAD2, [0xDA] = SML_DFA_HEAD2, [0xDB] = SML_DFA_HEAD2,
    [0xDC] = SML_DFA_HEAD2, [0xDD] = SML_DFA_HEAD2, [0xDE] = SML_DFA_HEAD2, [0xDF] = SML_DFA_HEAD2,
    [0xE0] = SML_DFA_E0,    [0xE1] = SML_DFA_HEAD3, [0xE2] = SML_DFA_HEAD3, [0xE3] = SML_DFA_HEAD3,
    [0xE4] = SML_DFA_HEAD3, [0xE5] = SML_DFA_HEAD3, [0xE6] = SML_DFA_HEAD3, [0xE7] = SML_DFA_HEAD3,
    [0xE8] = SML_DFA_HEAD3, [0xE9] = SML_DFA_HEAD3, [0xEA] = SML_DFA_HEAD3, [0xEB] = SML_DFA_HEAD3,
    [0xEC] = SML_DFA_HEAD3, [0xED] = SML_DFA_ED,    [0xEE] = SML_DFA_HEAD3, [0xEF] = SML_DFA_HEAD3,
    [0xF0] = SML_DFA_F0,    [0xF1] = SML_DFA_HEAD4, [0xF2] = SML_DFA_HEAD4, [0xF3] = SML_DFA_HEAD4,
    [0xF4] = SML_DFA_F4,    [0xF5] = SML_DFA_ERROR, [0xF6] = SML_DFA_ERROR, [0xF7] = SML_DFA_ERROR,
    [0xF8] = SML_DFA_ERROR, [0xF9] = SML_DFA_ERROR, [0xFA] = SML_DFA_ERROR, [0xFB] = SML_DFA_ERROR,
    [0xFC] = SML_DFA_ERROR, [0xFD] = SML_DFA_ERROR, [0xFE] = SML_DFA_ERROR, [0xFF] = SML_DFA_ERROR
};

#undef SML_DFA_ERROR
#undef SML_DFA_ASCII
#undef SML_DFA_HEAD2
#undef SML_DFA_HEAD3
#undef SML_DFA_HEAD4
#undef SML_DFA_E0
#undef SML_DFA_ED
#undef SML_DFA_F0
#undef SML_DFA_F4
#undef SML_DFA_80_8F
#undef SML_DFA_90_9F
#undef SML_DFA_A0_BF

#undef SML_DFA_ROW

#undef SML_S_ERROR
#undef SML_S_OK
#undef SML_S_TAIL1
#undef SML_S_TAIL2
#undef SML_S_TAIL3
#undef SML_S_E0
#undef SML_S_ED
#undef SML_S_F0
#undef SML_S_F4

static inline uint64_t SML_utf8_dfa_step(uint64_t state, uint8_t byte)
{
    return (g_sml_utf8_dfa[byte] >> state) & 0x3F;
}

static inline uint64_t SML_utf8_dfa_run(uint64_t state, const uint8_t *bytes, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        state = g_sml_utf8_dfa[bytes[i]] >> (state & 0x3F);
    }
    return state & 0x3F;
}

bool SML_utf8_validate(const char *src, uint64_t len)
{
    const uint8_t *bytes = (const uint8_t *)src;
    uint64_t state = SML_UTF8_OK;

    state = SML_utf8_dfa_run(state, bytes, len);

    if (state == SML_UTF8_OK) {
        return true;
    }
    return false;
}