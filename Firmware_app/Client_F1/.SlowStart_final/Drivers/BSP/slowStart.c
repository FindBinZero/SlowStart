//
// Created by FindBin on 2024/6/9.
//

#include "slowStart.h"
#include "config.h"
#include "tim.h"
#include "can.h"

volatile _Bool is_error = 0;

void slowStart_init() {}

static void function_ss_open_close_action(const uint8_t *data, GPIO_PinState gpioPinState) {
    switch (data[Member_byte]) {
        case c1:
            c1_set(gpioPinState);
            break;
        case c2:
            c2_set(gpioPinState);
            break;
        case c3:
            c3_set(gpioPinState);
            break;
        case c4:
            c4_set(gpioPinState);
            break;
        case c5:
            c5_set(gpioPinState);
            break;
        case c6:
            c6_set(gpioPinState);
            break;
        case c7:
            c7_set(gpioPinState);
            break;
        case c8:
            c8_set(gpioPinState);
            break;
        case c9:
            c9_set(gpioPinState);
            break;
        case c10:
            c10_set(gpioPinState);
            break;
        default:
            is_error = 1;
            break;
    }
}

static void function_ss_control(uint8_t *data) {
    switch (data[Action_byte]) {
        case OPEN_Action:
            function_ss_open_close_action(data, OPEN_MODE);
            break;
        case CLOSE_Action:
            function_ss_open_close_action(data, CLOSE_MODE);
            break;
        default:
            is_error = 1;
    }
}

static void function_ss_all_close(const uint8_t *data) {
    UNUSED(data);
    all_set(CLOSE_MODE);
}

static void function_ss_all_open(const uint8_t *data) {
    UNUSED(data);
    all_set(OPEN_MODE);
}

void ss_can_cb(uint8_t data[8]) {       /// can callback
    switch (data[FunctionCode_byte]) {
        case CONTROL:
            function_ss_control(data);
            break;
        case ALL_CLOSE:
            function_ss_all_close(data);
            break;
        case ALL_OPEN:
            function_ss_all_open(data);
            break;
        default:
            is_error = 1;
            break;
    }
}

volatile uint16_t times = 400u;
volatile static uint8_t id = 0;
#define WAIT_TIME (4500)
volatile static uint16_t wait_time = 0u;

uint32_t led_pwm = 0u;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {               /// 0.05ms          check error
        static volatile uint16_t times_14 = 0u;
        if (is_error) {
            all_set(CLOSE_MODE);
            HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
            HAL_CAN_DeactivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
            if (times_14++ >= 1200) {
                times_14 = 0u;
                led_pwm == 0u ? (led_pwm = htim1.Init.Period) : (led_pwm = 0u);
                htim1.Instance->CCR3 = led_pwm;
                times_14 = 0;
            }
        }
    } else if (htim->Instance == TIM3) {        /// 0.5ms       id
        times++;
        if ((times >= 400) && (id < (user_id * 2))) {
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            times = 0;
            id++;
        } else if (wait_time++ >= WAIT_TIME) {
            wait_time = 0;
            id = 0;
        }
    } else if (htim->Instance == TIM2) {        /// report state
        static uint8_t *pSend = NULL;
        uint16_t data = all_get();
        uint8_t send[8] = {data & 0x00ff, (data & 0xff00) >> 8, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb};

        static uint8_t err_send[8] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee};
        pSend = send;
        if (is_error) {
            pSend = err_send;
        }
        CAN_Send_Msg(&TxMsg, sstart_final_id, pSend);
    }
}