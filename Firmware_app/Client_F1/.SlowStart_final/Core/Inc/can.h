/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */
typedef struct {
    uint32_t mailbox;
    CAN_TxHeaderTypeDef TxMessage;
    uint8_t sendData[8];
} CANTxMsg_t;

typedef struct {
    CAN_RxHeaderTypeDef RxMessage;
    uint8_t receiveData[8];
} CANRxMsg_t;
extern CANTxMsg_t TxMsg;

void CAN1_Filter_Init();

_Bool CAN_Send_Msg(CANTxMsg_t *msg, uint16_t mailbox_id, const uint8_t *aData);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

