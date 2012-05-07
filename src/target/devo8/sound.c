/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Foobar is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "target.h"

void SOUND_Init()
{
    rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO1);

    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Period */
    timer_set_period(TIM2, 65535);

    /* Prescaler */
    timer_set_prescaler(TIM2, 5);
    timer_generate_event(TIM2, TIM_EGR_UG);

    /* ---- */
    /* Output compare 1 mode and preload */
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_preload(TIM2, TIM_OC2);

    /* Polarity and state */
    timer_set_oc_polarity_low(TIM2, TIM_OC2);
    timer_enable_oc_output(TIM2, TIM_OC2);

    /* Capture compare value */
    timer_set_oc_value(TIM2, TIM_OC2, 0x8000);
    /* ---- */
    /* ARR reload enable */
    timer_enable_preload(TIM2);
}

void SOUND_SetPeriod(u16 period)
{
    timer_set_period(TIM2, period);
    timer_set_oc_value(TIM2, TIM_OC2, period >> 1);
}

void SOUND_Enable()
{
    timer_enable_counter(TIM2);
}

void SOUND_Disable()
{
    timer_disable_counter(TIM2);
}
