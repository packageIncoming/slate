#pragma once
#include <cstdint>

//  aliases:
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

//  utility functions

//  sign extends to 64 bits
u64 sext64(u64 original, u32 orig_width);