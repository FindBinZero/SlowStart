/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 8;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
CANRxMsg_t RxMsg;  //定义接收邮件实体
#include "config.h"
#include "slowStart.h"

CANTxMsg_t TxMsg;  //定义发送邮件实体
//筛选器配置
void CAN1_Filter_Init() {
    CAN_FilterTypeDef CAN1_FilerConf = {0};

    CAN1_FilerConf.FilterIdHigh = (sstart_final_id << 5);                       //具体Id要求高16位
    CAN1_FilerConf.FilterIdLow = 0x00;                       //具体Id要求低16位
    CAN1_FilerConf.FilterMaskIdHigh = 0X0000;                //掩码高16位全设置为0，表示对所有位报文Id高16位都不关心
    CAN1_FilerConf.FilterMaskIdLow = 0X0000;                 //掩码低16位全设置为0，表示对所有位报文Id低16位都不关心
    CAN1_FilerConf.FilterFIFOAssignment = CAN_FILTER_FIFO0;  //筛选器接收到的报文放入到FIFO0中，即为接收邮箱0
    CAN1_FilerConf.FilterActivation = ENABLE;                //筛选器使能（开启）
    CAN1_FilerConf.FilterMode = CAN_FILTERMODE_IDLIST;       //筛选器掩码模式
    CAN1_FilerConf.FilterScale = CAN_FILTERSCALE_32BIT;      //掩码用32位表示
    CAN1_FilerConf.FilterBank = 0;
    CAN1_FilerConf.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &CAN1_FilerConf) != HAL_OK) {
        Error_Handler();
    }
}

_Bool CAN_Send_Msg(CANTxMsg_t *msg, uint16_t mailbox_id, const uint8_t *aData) {

    msg->TxMessage.StdId = mailbox_id;                  //邮箱id号
    msg->TxMessage.IDE = CAN_ID_STD;                    //邮件的id格式（标准为CAN_ID_STD	|	  拓展为CAM_ID_EXT）
    msg->TxMessage.DLC = 0x08;                          //邮件数据长度
    msg->TxMessage.RTR = CAN_RTR_DATA;                  //数据帧 一般都是数据帧
    msg->TxMessage.TransmitGlobalTime = DISABLE;        //默认DISABLE

    msg->sendData[0] = aData[0];
    msg->sendData[1] = aData[1];
    msg->sendData[2] = aData[2];
    msg->sendData[3] = aData[3];
    msg->sendData[4] = aData[4];
    msg->sendData[5] = aData[5];
    msg->sendData[6] = aData[6];
    msg->sendData[7] = aData[7];

    if (HAL_CAN_AddTxMessage(&hcan, &msg->TxMessage, msg->sendData, &msg->mailbox) != HAL_OK)
        return 0;
    else
        return 1;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

    if (hcan->Instance == CAN1) {
        HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMsg.RxMessage, RxMsg.receiveData);
        if (RxMsg.RxMessage.StdId == sstart_final_id) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            ss_can_cb(RxMsg.receiveData);
        }
        if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
            Error_Handler();
        }
    }
}
/* USER CODE END 1 */
