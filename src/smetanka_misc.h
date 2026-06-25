#pragma once

#include <stdint.h>

#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif

static inline uint32_t get_handle_id(uint64_t handle) {
    return (uint32_t)handle;
}

static inline uint32_t get_handle_gen(uint64_t handle) {
    return (uint32_t)(handle >> 32);
}

static inline uint64_t pack_handle(uint32_t id, uint32_t gen) {
    return (((uint64_t)gen) << 32) | ((uint64_t)id);
}

static inline uint64_t increase_handle_gen(uint64_t handle) {
    return pack_handle(get_handle_id(handle), get_handle_gen(handle) + 1);
}

static inline uint8_t aabb_intersect(
    float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh)
{
    return ax < bx + bw &&
           ax + aw > bx &&
           ay < by + bh &&
           ay + ah > by;
}
