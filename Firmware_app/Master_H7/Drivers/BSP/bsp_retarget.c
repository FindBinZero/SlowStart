//
// Created by FindBin on 2023/2/10.
//

#include "bsp_retarget.h"

#ifdef HAL_UART_MODULE_ENABLED      ///是否启动了串口外设

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
    gHuart = huart;

    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

int _write(int fd, char *ptr, int len) {
    HAL_StatusTypeDef hstatus;

    if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
        hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
            return len;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

int _close(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

int _lseek(int fd, int ptr, int dir) {
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

int _read(int fd, char *ptr, int len) {
    HAL_StatusTypeDef hstatus;

    if (fd == STDIN_FILENO) {
        hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
            return 1;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat *st) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)

#endif //#ifdef HAL_UART_MODULE_ENABLED     ///是否启动了串口外设


#ifdef USB      ///是否启动usb通信

#if defined(USB_USE_CDC)    ///是否启动usb的CDC模式

#include "usbd_cdc_if.h"
#include "stdarg.h"

/** Received data over USB are stored in this buffer      */
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

void usb_printf(const char *format, ...) {

    va_list args;

    uint32_t length;

    va_start(args, format);

    length = vsnprintf((char *) UserTxBufferFS, APP_TX_DATA_SIZE, (char *) format, args);

    va_end(args);

    CDC_Transmit_FS(UserTxBufferFS, length);

}
#endif  //USB_USE_CDC
#endif //USB        ///是否启动usb通信

#ifdef USE_VOFA_TRANSMIT    ///是否使用VOFA进行数据的输出


uint8_t send_buf[VOFA_MAX_BUFFER_SIZE];
uint32_t cnt = 0;
#define BYTE0(dw_temp)     (*(uint8_t *)(&dw_temp))
#define BYTE1(dw_temp)     (*((uint8_t*)(&dw_temp) + 1))
#define BYTE2(dw_temp)     (*((uint8_t*)(&dw_temp) + 2))
#define BYTE3(dw_temp)     (*((uint8_t*)(&dw_temp) + 3))

void vofa_transmit(uint8_t *buf, uint16_t len) {
#if VOFA_WAY
    HAL_UART_Transmit(&VOFA_UART_PORT, (uint8_t *) buf, len, HAL_MAX_DELAY);
#else
#if defined(USB_USE_CDC)
    CDC_Transmit_FS((uint8_t *) buf, len);
#endif
#endif
}

void vofa_sendframetail() {
    send_buf[cnt++] = 0x00;
    send_buf[cnt++] = 0x00;
    send_buf[cnt++] = 0x80;
    send_buf[cnt++] = 0x7f;

    /* 将数据和帧尾打包发送 */
    vofa_transmit((uint8_t *) send_buf, cnt);
    cnt = 0;// 每次发送完帧尾都需要清零
}

void vofa_printf_data(_Bool send_trail, uint16_t _id, float32_t data) {
    send_buf[cnt++] = BYTE0(data);
    send_buf[cnt++] = BYTE1(data);
    send_buf[cnt++] = BYTE2(data);
    send_buf[cnt++] = BYTE3(data);
    if (send_trail) {
        vofa_sendframetail();
    }
}
//void vofa_demo(void) {
//    static float scnt = 0.0f;
//
//    scnt += 0.01f;
//
//    if (scnt >= 360.0f)
//        scnt = 0.0f;
//
//    float32_t v1 = scnt;
//    float32_t v2 = sinf((float32_t) scnt / 180 * 3.14159) * 180 + 180;
//    float32_t v3 = sinf((float32_t) (scnt + 120) / 180 * 3.14159) * 180 + 180;
//    float32_t v4 = sinf((float32_t) (scnt + 240) / 180 * 3.14159) * 180 + 180;
//    float32_t v5 = cosf((float32_t) (scnt + 240) / 180 * 3.14159) * 180 + 180;
//    float32_t v6 = tanf((float32_t) (scnt + 240) / 180 * 3.14159) * 180 + 180;
//
//    // Call the function to store the data in the buffer
//    vofa_printf_data(0, v1);
//    vofa_printf_data(1, v2);
//    vofa_printf_data(2, v3);
//    vofa_printf_data(3, v4);
//    vofa_printf_data(4, v5);
//    vofa_printf_data(4, v6);
//
//    vofa_sendframetail();
//}
#endif //USE_VOFA_TRANSMIT      ///是否使用VOFA进行数据的输出