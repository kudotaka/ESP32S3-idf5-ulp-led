/* ULP-RISC-V example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
   This code runs on ULP-RISC-V  coprocessor
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ulp_riscv.h"
#include "ulp_riscv_utils.h"
#include "ulp_riscv_gpio.h"
#include "soc/rtc_cntl_reg.h"

#define GPIO_LED GPIO_NUM_5

int main (void)
{
    ulp_riscv_gpio_init(GPIO_LED);
    ulp_riscv_gpio_output_enable(GPIO_LED);
    ulp_riscv_gpio_set_output_mode(GPIO_LED, RTCIO_MODE_OUTPUT);

    while(1) {
        ulp_riscv_gpio_output_level(GPIO_LED, 1);
        ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * 1000); // wait 1000 ms
        ulp_riscv_gpio_output_level(GPIO_LED, 0);
        ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * 1000);
    }
    
    return 0;
}
