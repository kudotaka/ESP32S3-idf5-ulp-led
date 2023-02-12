// ULP-RISC-V example
// This example code is in the Public Domain (or CC0 licensed, at your option.)
// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.
// This code runs on ULP-RISC-V  coprocessor

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ulp_riscv.h"
#include "ulp_riscv_utils.h"
#include "ulp_riscv_gpio.h"
#include "soc/rtc_cntl_reg.h"

#define GPIO_BUTTON GPIO_NUM_4
#define GPIO_LED GPIO_NUM_5
#define ULP_LOOP_MAX_COUNT 20

uint32_t running_cnt = 0;
uint32_t loop_cnt = 0;
static bool gpio_level = false;
bool gpio_level_previous = false;

int main (void)
{
//    running_cnt += 1;
    running_cnt++;
//    loop_cnt = 0;
    gpio_level = (bool)ulp_riscv_gpio_get_level(GPIO_BUTTON);
    gpio_level_previous = gpio_level;

    ulp_riscv_gpio_init(GPIO_BUTTON);
    ulp_riscv_gpio_input_enable(GPIO_BUTTON);
    ulp_riscv_gpio_init(GPIO_LED);
    ulp_riscv_gpio_output_enable(GPIO_LED);
    ulp_riscv_gpio_set_output_mode(GPIO_LED, RTCIO_MODE_OUTPUT);

    while(1) {
        gpio_level = (bool)ulp_riscv_gpio_get_level(GPIO_BUTTON);
        if (gpio_level != gpio_level_previous) {
//            gpio_level_previous = gpio_level;
            ulp_riscv_wakeup_main_processor();
            break;
        }
        // ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * 10);

        if (loop_cnt < ULP_LOOP_MAX_COUNT) {
            ulp_riscv_gpio_output_level(GPIO_LED, 1);
            ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * 1000); // wait 1000 ms
            ulp_riscv_gpio_output_level(GPIO_LED, 0);
            ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * 1000);
            loop_cnt++;
        } else {
            gpio_level_previous = (bool)ulp_riscv_gpio_get_level(GPIO_BUTTON);
            loop_cnt = 0;
            ulp_riscv_wakeup_main_processor();
            break;
        }
    }

    // ulp_riscv_halt() is called automatically when main exits
    return 0;
}
