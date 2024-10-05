//
// Created by FindBin on 2023/2/10.
//

#pragma once

#include "stm32f1xx_hal.h"
#include <errno.h>
#include <sys/times.h>
#include <stdint.h>

#if defined(USB)
#ifdef STM32H7
#if defined(USB_USE_CDC)
#define CDC_Transmit_FS(a, b) CDC_Transmit_HS(a, b)
#define UserTxBufferFS UserTxBufferHS
#endif
#endif
#endif

#define VOFA_USB_WAY 0
#define VOFA_UART_WAY 1
#define VOFA_WAY VOFA_USB_WAY

#if VOFA_WAY
#define VOFA_UART_PORT (huart3)
#endif

#define VOFA_UT_SIZE 1024

#ifdef HAL_UART_MODULE_ENABLED

#include "usart.h"

#define USE_VOFA_TRANSMIT
#define VOFA_MAX_BUFFER_SIZE VOFA_UT_SIZE

#endif

#ifdef USBD_FS_SPEED

#define USE_VOFA_TRANSMIT
#define VOFA_MAX_BUFFER_SIZE VOFA_UT_SIZE

#endif

#ifdef HAL_UART_MODULE_ENABLED      ///是否启动了串口外设

#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);

#endif //#ifdef HAL_UART_MODULE_ENABLED     ///是否启动了串口外设

#ifdef USB       ///是否启动usb通信
#if defined(USB_USE_CDC)

void usb_printf(const char *format, ...);

#endif
#endif //USB      ///是否启动usb通信

#ifdef USE_VOFA_TRANSMIT    ///是否使用VOFA进行数据的输出

typedef float float32_t;

#ifdef __cplusplus
#define _Bool bool
#endif

void vofa_printf_data(_Bool send_trail, uint16_t _id, float32_t data);

#endif //USE_VOFA_TRANSMIT      ///是否使用VOFA进行数据的输出


