/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <string.h>
#include <sys/lock.h>
#include <sys/param.h>

#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "soc/soc_caps.h"

#include "sdkconfig.h"

#include "driver/gpio.h"
#include "esp_private/gpio.h"
#include "esp_private/sleep_gpio.h"

static const char *TAG = "sleep";

#if SOC_GPIO_SUPPORT_SLP_SWITCH

#if CONFIG_GPIO_ESP32_SUPPORT_SWITCH_SLP_PULL
void gpio_sleep_mode_config_apply(void)
{
    for (gpio_num_t gpio_num = GPIO_NUM_0; gpio_num < GPIO_NUM_MAX; gpio_num++) {
        if (GPIO_IS_VALID_GPIO(gpio_num)) {
            gpio_sleep_pupd_config_apply(gpio_num);
        }
    }
}

IRAM_ATTR void gpio_sleep_mode_config_unapply(void)
{
    for (gpio_num_t gpio_num = GPIO_NUM_0; gpio_num < GPIO_NUM_MAX; gpio_num++) {
        if (GPIO_IS_VALID_GPIO(gpio_num)) {
            gpio_sleep_pupd_config_unapply(gpio_num);
        }
    }
}
#endif

void esp_sleep_config_gpio_isolate(void)
{
    ESP_LOGI(TAG, "Configure to isolate all GPIO pins in sleep state");
    for (gpio_num_t gpio_num = GPIO_NUM_0; gpio_num < GPIO_NUM_MAX; gpio_num++) {
        if (GPIO_IS_VALID_GPIO(gpio_num)) {
            gpio_sleep_set_direction(gpio_num, GPIO_MODE_DISABLE);
            gpio_sleep_set_pull_mode(gpio_num, GPIO_FLOATING);
        }
    }
}

void esp_sleep_enable_gpio_switch(bool enable)
{
    ESP_LOGI(TAG, "%s automatic switching of GPIO sleep configuration", enable ? "Enable" : "Disable");
    for (gpio_num_t gpio_num = GPIO_NUM_0; gpio_num < GPIO_NUM_MAX; gpio_num++) {
        if (GPIO_IS_VALID_GPIO(gpio_num)) {
            if (enable) {
                gpio_sleep_sel_en(gpio_num);
            } else {
                gpio_sleep_sel_dis(gpio_num);
            }
        }
    }
}

#endif // SOC_GPIO_SUPPORT_SLP_SWITCH
