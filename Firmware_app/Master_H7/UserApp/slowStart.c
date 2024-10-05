//
// Created by FindBin on 2024/6/9.
//
/// Author : FBResistor
/// Contact me : 1578557118@qq.com
/// https://github.com/FindBinZero
#include "slowStart.h"
#include "bsp_can.h"

void sstart_allopen(FDCAN_HandleTypeDef *hfdcan, uint8_t id) {
    uint8_t send_msg[8] = {ALL_OPEN};
    fdcan_send_msg(hfdcan, sstart_final_id(id), 8, send_msg);
}

void sstart_allclose(FDCAN_HandleTypeDef *hfdcan, uint8_t id) {
    uint8_t send_msg[8] = {ALL_CLOSE};
    fdcan_send_msg(hfdcan, sstart_final_id(id), 8, send_msg);
}

void sstart_ctrl(FDCAN_HandleTypeDef *hfdcan, uint8_t id, uint8_t port_id, uint8_t state) {
    uint8_t send_msg[8] = {CONTROL, port_id, state};
    fdcan_send_msg(hfdcan, sstart_final_id(id), 8, send_msg);
}
