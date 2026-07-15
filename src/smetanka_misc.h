#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__cplusplus)
    #define CLITERAL(type) type
#else
    #define CLITERAL(type) (type)
#endif

#define ABORT() \
    do { \
        fprintf(stderr, "info: %s:%d\n", __FILE__, __LINE__); \
        abort(); \
    } while (0)

static inline uint32_t getHandleId(uint64_t handle) {
    return (uint32_t)handle;
}

static inline uint32_t getHandleGen(uint64_t handle) {
    return (uint32_t)(handle >> 32);
}

static inline uint64_t packHandle(uint32_t id, uint32_t gen) {
    return (((uint64_t)gen) << 32) | ((uint64_t)id);
}

static inline uint64_t increaseHandleGen(uint64_t handle) {
    return packHandle(getHandleId(handle), getHandleGen(handle) + 1);
}

static inline void logAbort() {
        fprintf(stderr, "info: %s:%d\n", __FILE__, __LINE__); \
        abort(); \
}
