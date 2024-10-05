//
// Created by FindBin on 2024/4/29.
//

/// Author : FBResistor
/// Contact me : 1578557118@qq.com
/// https://github.com/FindBinZero
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#if defined(STM32H723xx)
#if (defined(FDCAN1) | defined(FDCAN2) | defined(FDCAN3))

/** @defgroup use which fdcan
 * {@
 */
#define USE_FDCAN1
//#define USE_FDCAN2
//#define USE_FDCAN3
/**
 * @}
 */

#include "fdcan.h"

/**
 * fdcan initialize
 */
void fdcan_init();

/**
 * fdcan send message
 * @param hfdcan struct pointer to the fdcan to send
 * @param id identify label
 * @param byte_size Specifies the length of the frame that will be transmitted
 *                  This parameter can be a value of @ref FDCAN_data_length_code
 * @param pTxData pointer to the data to send
 */
void fdcan_send_msg(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t byte_size, uint8_t *pTxData);

/**
 *
 * @param hfdcan struct pointer to the fdcan to send
 * @param id identify label
 * @param aTxData array to the data to send
 */
void fdcan_send_msg_dji(FDCAN_HandleTypeDef *hfdcan, uint16_t id, const int16_t aTxData[4]);

/**@defgroup fdcan out data buffer size
 * {@
 */
#define FDCAN1_FIFO0_OUT_MSG_SIZE 8
#define FDCAN2_FIFO0_OUT_MSG_SIZE 8
#define FDCAN3_FIFO0_OUT_MSG_SIZE 8

#define FDCAN1_FIFO1_OUT_MSG_SIZE 8
#define FDCAN2_FIFO1_OUT_MSG_SIZE 8
#define FDCAN3_FIFO1_OUT_MSG_SIZE 8

/**
 * @}
 */
/**
 * @weakgroup weak callback function
 */
void fdcan1_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);

void fdcan2_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);

void fdcan3_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);

void fdcan1_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);

void fdcan2_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);

void fdcan3_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out);


#endif
#endif

#ifdef __cplusplus
}
#endif

