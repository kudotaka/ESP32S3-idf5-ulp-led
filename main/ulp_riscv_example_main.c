// ULP riscv DS18B20 1wire temperature sensor example
// This example code is in the Public Domain (or CC0 licensed, at your option.)
// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include <stdio.h>
#include "esp_sleep.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc_periph.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "ulp_riscv.h"
#include "ulp_main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MY-MAIN";

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

static void start_ulp_program(void);
static void stop_ulp_program(void);
static void restart_ulp_program(void);

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(10));

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    // not a wakeup from ULP, load the firmware
    if (cause != ESP_SLEEP_WAKEUP_ULP) {
        printf("Not a ULP-RISC-V wakeup, initializing it! \n");
        start_ulp_program();
    }

    // ULP Risc-V read and detected a change in GPIO_0, prints
    if (cause == ESP_SLEEP_WAKEUP_ULP) {
        printf("ULP-RISC-V woke up the main CPU! \n");
        printf("ULP-RISC-V ulp running_cnt is %ld \n", ulp_running_cnt);
        stop_ulp_program();
        vTaskDelay(pdMS_TO_TICKS(10000));
    }

    switch (cause)
    {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_UNDEFINED");
        break;

    case ESP_SLEEP_WAKEUP_ALL:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_ALL");
        break;

    case ESP_SLEEP_WAKEUP_EXT0:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_EXT0");
        break;
    
    case ESP_SLEEP_WAKEUP_EXT1:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_EXT1");
        break;

    case ESP_SLEEP_WAKEUP_TIMER:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_TIMER");
        break;

    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_TOUCHPAD");
        break;

    case ESP_SLEEP_WAKEUP_ULP:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_ULP");
        break;

    case ESP_SLEEP_WAKEUP_GPIO:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_GPIO");
        break;

    case ESP_SLEEP_WAKEUP_UART:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_UART");
        break;

    case ESP_SLEEP_WAKEUP_WIFI:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_WIFI");
        break;

    case ESP_SLEEP_WAKEUP_COCPU:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_COCPU");
        break;

    case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG");
        break;

    case ESP_SLEEP_WAKEUP_BT:
        ESP_LOGI(TAG, "ESP_SLEEP_WAKEUP_BT");
        break;

    default:
        ESP_LOGI(TAG, "UNMATCH");
        break;
    }

    vTaskDelay( pdMS_TO_TICKS(1000) );
    // Go back to sleep, only the ULP Risc-V will run
    ESP_LOGI(TAG, "ULP-RISC-V ulp running_cnt is %ld", ulp_running_cnt);
    printf("Entering in deep sleep \n");
    // Small delay to ensure the messages are printed
    restart_ulp_program();
    vTaskDelay( pdMS_TO_TICKS(1000) );

    ESP_ERROR_CHECK( esp_sleep_enable_ulp_wakeup() );
    esp_deep_sleep_start();
}

static void start_ulp_program(void)
{
    esp_err_t err = ulp_riscv_load_binary(ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start));
    ESP_ERROR_CHECK(err);

    // Start the program
    err = ulp_riscv_run();
    ESP_ERROR_CHECK(err);
}

static void stop_ulp_program(void)
{
    ulp_riscv_timer_stop();
    vTaskDelay( pdMS_TO_TICKS(1000) );
}

static void restart_ulp_program(void)
{
    ulp_riscv_timer_resume();
    vTaskDelay( pdMS_TO_TICKS(1000) );
}