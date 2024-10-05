//
// Created by FindBin on 2024/4/29.
//

#include "bsp_can.h"

/// Author : FBResistor
/// Contact me : 1578557118@qq.com
/// https://github.com/FindBinZero

#if defined(STM32H723xx)

void fdcan_init() {
    FDCAN_FilterTypeDef sfdcanFilterConfig;
#if (defined(FDCAN1) & defined(USE_FDCAN1))
    /**
     * @dir Filter for FDCAN1
     * @{
     */
    sfdcanFilterConfig.IdType = FDCAN_STANDARD_ID;
    sfdcanFilterConfig.FilterIndex = 0;
    sfdcanFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sfdcanFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sfdcanFilterConfig.FilterID1 = 0x000;
    sfdcanFilterConfig.FilterID2 = 0x7fff;
    HAL_FDCAN_ConfigFilter(&hfdcan1, &sfdcanFilterConfig) == HAL_OK ? ((void) NULL) : Error_Handler();

    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,
                                 FDCAN_REJECT, FDCAN_REJECT,
                                 FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
    == HAL_OK
    ? ((void) (NULL))
    : Error_Handler();

    HAL_FDCAN_Start(&hfdcan1) == HAL_OK ? ((void) NULL) : Error_Handler();

    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK
    ? ((void) NULL)
    : Error_Handler();

/**
 * @}
 */
#endif

#if (defined(FDCAN2) & defined(USE_FDCAN2))
    /**
     * @dir Filter for FDCAN2
     * @{
     */
        sfdcanFilterConfig.IdType = FDCAN_STANDARD_ID;
        sfdcanFilterConfig.FilterIndex = 0;
        sfdcanFilterConfig.FilterType = FDCAN_FILTER_RANGE;
        sfdcanFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        sfdcanFilterConfig.FilterID1 = 0x000;
        sfdcanFilterConfig.FilterID2 = 0x7fff;
        HAL_FDCAN_ConfigFilter(&hfdcan2, &sfdcanFilterConfig) == HAL_OK ? ((void) NULL) : Error_Handler();

        HAL_FDCAN_ConfigGlobalFilter(&hfdcan2,
                                     FDCAN_REJECT, FDCAN_REJECT,
                                     FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
        == HAL_OK
        ? ((void) (NULL))
        : Error_Handler();

        HAL_FDCAN_Start(&hfdcan2) == HAL_OK ? ((void) NULL) : Error_Handler();

        HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK
        ? ((void) NULL)
        : Error_Handler();

    /**
     * @}
     */
#endif
#if (defined(FDCAN3) & defined(USE_FDCAN3))
    /**
     * @dir Filter for FDCAN3
     * @{
     */
    sfdcanFilterConfig.IdType = FDCAN_STANDARD_ID;
    sfdcanFilterConfig.FilterIndex = 0;
    sfdcanFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sfdcanFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sfdcanFilterConfig.FilterID1 = 0x000;
    sfdcanFilterConfig.FilterID2 = 0x7fff;
    HAL_FDCAN_ConfigFilter(&hfdcan3, &sfdcanFilterConfig) == HAL_OK ? ((void) (NULL)) : Error_Handler();
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan3,
                                 FDCAN_REJECT, FDCAN_REJECT,
                                 FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
    == HAL_OK
    ? ((void) (NULL))
    : Error_Handler();

    HAL_FDCAN_Start(&hfdcan3) == HAL_OK ? ((void) NULL) : Error_Handler();

    HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) == HAL_OK
    ? ((void) NULL)
    : Error_Handler();
    /**
     * @}
     */
#endif
}

void fdcan_send_msg(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t byte_size, uint8_t *pTxData) {
    byte_size > FDCAN_DLC_BYTES_64 ? byte_size = FDCAN_DLC_BYTES_64 : ((void) NULL);
    FDCAN_TxHeaderTypeDef fdcanTxHeader = {
            .Identifier = (uint32_t) (id),
            .IdType = FDCAN_STANDARD_ID,                    /// 标准帧
            .TxFrameType = FDCAN_DATA_FRAME,                /// 数据帧
            .DataLength = byte_size,                        /// 数据长度
            .ErrorStateIndicator = FDCAN_ESI_ACTIVE,        /// 错误处理
            .BitRateSwitch = FDCAN_BRS_OFF,                 /// 关闭可变波特率
            .FDFormat = FDCAN_CLASSIC_CAN,                  /// 经典模式
            .TxEventFifoControl = FDCAN_NO_TX_EVENTS,       /// 无发送事件
            .MessageMarker = 0x00,                          /// 标识当前tx的id
    };
    HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcanTxHeader, pTxData);
}

static void fdcan_trans_16_8_bit(const uint16_t *source_data, uint16_t out_data_len, uint8_t *out_data) {
    for (uint16_t i = 0, j = 0; i < out_data_len; i++, j += 2) {
        out_data[j] = source_data[i] >> 8;
        out_data[j++] = source_data[i] >> 8;
    }
}

void fdcan_send_msg_dji(FDCAN_HandleTypeDef *hfdcan, uint16_t id, const int16_t aTxData[4]) {
    uint8_t temp[8] = {0};
    fdcan_trans_16_8_bit((uint16_t *) aTxData, 8, temp);
    fdcan_send_msg(hfdcan, id, FDCAN_DLC_BYTES_8, temp);
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    __HAL_FDCAN_DISABLE_IT(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
#if (defined(FDCAN1) & defined(USE_FDCAN1))
    if (hfdcan->Instance == FDCAN1) {
        uint8_t rxMsg[FDCAN1_FIFO0_OUT_MSG_SIZE];
        FDCAN_RxHeaderTypeDef fdcanRxHeader;
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &fdcanRxHeader, rxMsg);
        fdcan1_fifo0_cb(&fdcanRxHeader, rxMsg);

    }
#endif
#if (defined(FDCAN2) & defined(USE_FDCAN2))
        if (hfdcan->Instance == FDCAN2) {

            uint8_t rxMsg[FDCAN2_FIFO0_OUT_MSG_SIZE];
            FDCAN_RxHeaderTypeDef fdcanRxHeader;
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &fdcanRxHeader, rxMsg);
            fdcan2_fifo0_cb(&fdcanRxHeader, rxMsg);
        }
#endif
#if (defined(FDCAN3) & defined(USE_FDCAN3))
        if (hfdcan->Instance == FDCAN3) {
            uint8_t rxMsg[FDCAN3_FIFO0_OUT_MSG_SIZE];
            FDCAN_RxHeaderTypeDef fdcanRxHeader;
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &fdcanRxHeader, rxMsg);
            fdcan3_fifo0_cb(&fdcanRxHeader, rxMsg);
        }
#endif
    __HAL_FDCAN_ENABLE_IT(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    __HAL_FDCAN_DISABLE_IT(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE);
#if (defined(FDCAN1) & defined(USE_FDCAN1))
    if (hfdcan->Instance == FDCAN1) {
        uint8_t rxMsg[FDCAN1_FIFO1_OUT_MSG_SIZE];
        FDCAN_RxHeaderTypeDef fdcanRxHeader;
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &fdcanRxHeader, rxMsg);
        fdcan1_fifo1_cb(&fdcanRxHeader, rxMsg);

    }
#endif
#if (defined(FDCAN2) & defined(USE_FDCAN2))
        if (hfdcan->Instance == FDCAN2) {

            uint8_t rxMsg[FDCAN2_FIFO1_OUT_MSG_SIZE];
            FDCAN_RxHeaderTypeDef fdcanRxHeader;
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &fdcanRxHeader, rxMsg);
            fdcan2_fifo1_cb(&fdcanRxHeader, rxMsg);

        }
#endif
#if (defined(FDCAN3) & defined(USE_FDCAN3))
        if (hfdcan->Instance == FDCAN3) {
            uint8_t rxMsg[FDCAN3_FIFO1_OUT_MSG_SIZE];
            FDCAN_RxHeaderTypeDef fdcanRxHeader;
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &fdcanRxHeader, rxMsg);
            fdcan3_fifo1_cb(&fdcanRxHeader, rxMsg);
        }
#endif
    __HAL_FDCAN_ENABLE_IT(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE);
}

__weak void fdcan1_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}

__weak void fdcan2_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}

__weak void fdcan3_fifo0_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}

__weak void fdcan1_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}

__weak void fdcan2_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}

__weak void fdcan3_fifo1_cb(FDCAN_RxHeaderTypeDef *hFdcanRxHeader_out, const uint8_t *pData_out) {
    UNUSED(hFdcanRxHeader_out);
    UNUSED(pData_out);
}


#endif