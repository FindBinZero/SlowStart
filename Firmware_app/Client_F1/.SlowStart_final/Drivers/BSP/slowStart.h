//
// Created by FindBin on 2024/6/9.
//

#pragma once

#include "main.h"

#define RESET_RETRY_NUB 0xce
#define CLOSE_MODE GPIO_PIN_RESET
#define OPEN_MODE (GPIO_PIN_SET)
#define GET_BIT(x, bit) ((x >> bit) & 0x01)
#define BIT(x) (1 << x)

enum {
    FunctionCode_byte,
    Action_byte,
    Member_byte,
};
typedef enum {
    OPEN_Action = 0xfb,
    CLOSE_Action = 0xbf,
} ActionCode;
typedef enum {
    RESET_RETRY = 0xa1,
    GET,
    TEST,
    ALL_OPEN,
    ALL_CLOSE,
    CONTROL
} FunctionCode;
typedef enum {
    c1 = 0,
    c2,
    c3,
    c4,
    c5,
    c6,
    c7,
    c8,
    c9,
    c10,
} MemberCode;

__attribute__((always_inline)) static inline _Bool c1_get() {
    return HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c2_get() {
    return HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c3_get() {
    return HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c4_get() {
    return HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin) == OPEN_MODE;

}

__attribute__((always_inline)) static inline _Bool c5_get() {
    return HAL_GPIO_ReadPin(C5_GPIO_Port, C5_Pin) == OPEN_MODE;

}

__attribute__((always_inline)) static inline _Bool c6_get() {
    return HAL_GPIO_ReadPin(C6_GPIO_Port, C6_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c7_get() {
    return HAL_GPIO_ReadPin(C7_GPIO_Port, C7_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c8_get() {
    return HAL_GPIO_ReadPin(C8_GPIO_Port, C8_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c9_get() {
    return HAL_GPIO_ReadPin(C9_GPIO_Port, C9_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline _Bool c10_get() {
    return HAL_GPIO_ReadPin(C10_GPIO_Port, C10_Pin) == OPEN_MODE;
}

__attribute__((always_inline)) static inline uint16_t all_get() {
    uint16_t state = 0x0000;
    c1_get() == 1 ? (state |= BIT(c1)) : (state &= (~BIT(c1)));
    c2_get() == 1 ? (state |= BIT(c2)) : (state &= (~BIT(c2)));
    c3_get() == 1 ? (state |= BIT(c3)) : (state &= (~BIT(c3)));
    c4_get() == 1 ? (state |= BIT(c4)) : (state &= (~BIT(c4)));
    c5_get() == 1 ? (state |= BIT(c5)) : (state &= (~BIT(c5)));
    c6_get() == 1 ? (state |= BIT(c6)) : (state &= (~BIT(c6)));
    c7_get() == 1 ? (state |= BIT(c7)) : (state &= (~BIT(c7)));
    c8_get() == 1 ? (state |= BIT(c8)) : (state &= (~BIT(c8)));
    c9_get() == 1 ? (state |= BIT(c9)) : (state &= (~BIT(c9)));
    c10_get() == 1 ? (state |= BIT(c10)) : (state &= (~BIT(c10)));
    return state;
}


__attribute__((always_inline)) static inline void c1_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, PinState);
}

__attribute__((always_inline)) static inline void c2_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, PinState);
}

__attribute__((always_inline)) static inline void c3_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, PinState);
}

__attribute__((always_inline)) static inline void c4_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, PinState);
}

__attribute__((always_inline)) static inline void c5_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C5_GPIO_Port, C5_Pin, PinState);
}

__attribute__((always_inline)) static inline void c6_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C6_GPIO_Port, C6_Pin, PinState);
}

__attribute__((always_inline)) static inline void c7_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C7_GPIO_Port, C7_Pin, PinState);
}

__attribute__((always_inline)) static inline void c8_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C8_GPIO_Port, C8_Pin, PinState);
}

__attribute__((always_inline)) static inline void c9_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C9_GPIO_Port, C9_Pin, PinState);
}

__attribute__((always_inline)) static inline void c10_set(GPIO_PinState PinState) {
    HAL_GPIO_WritePin(C10_GPIO_Port, C10_Pin, PinState);
}

__attribute__((always_inline)) static inline void all_set(GPIO_PinState PinState) {
    c1_set(PinState);
    c2_set(PinState);
    c3_set(PinState);
    c4_set(PinState);
    c5_set(PinState);
    c6_set(PinState);
    c7_set(PinState);
    c8_set(PinState);
    c9_set(PinState);
    c10_set(PinState);
}

void slowStart_init();

void ss_can_cb(uint8_t data[8]);