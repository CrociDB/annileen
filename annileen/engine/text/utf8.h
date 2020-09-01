/*
* Mostly copied from utf8 in bgfx examples
*/

#pragma once

#include <stdint.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

uint32_t utf8_decode(uint32_t* _state, uint32_t* _codep, uint8_t _ch);
