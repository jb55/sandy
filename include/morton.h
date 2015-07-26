#ifndef MORTON_H
#define MORTON_H

#include <stdint.h>

uint32_t
morton_part_1by1(uint32_t x);


uint32_t
morton_encode2(uint32_t x, uint32_t y);


uint32_t
morton_compact_1by1(uint32_t x);


uint32_t
morton_decode_2x(uint32_t code);


uint32_t
morton_decode_2y(uint32_t code);


#endif /* MORTON_H */
