//
// Created by FindBin on 2024/6/9.
//
/// Author : FBResistor
/// Contact me : 1578557118@qq.com
/// https://github.com/FindBinZero
#pragma once

#include "main.h"
/// do not alter
#define base_id                 (0x7a0)
#define sstart_final_id(id)     (base_id | id)

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

void sstart_allopen(FDCAN_HandleTypeDef *hfdcan, uint8_t id);

void sstart_allclose(FDCAN_HandleTypeDef *hfdcan, uint8_t id);

void sstart_ctrl(FDCAN_HandleTypeDef *hfdcan, uint8_t id, uint8_t port_id, uint8_t state);