#include "threads.h"

/*
 * Function prototypes.
 */

light_settings_t light_settings = {LIGHT_STATE_OFF, 250};
void updateLight(light_settings_t s);

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/*
 * Actual functions.
 */

void startLight(void)
{

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1; // 1Khz clock
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
    TIM16->CR1 &= ~TIM_CR1_CEN;

    /* TIM16 OC1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = light_settings.duration; // period in ms
    TIM_OC1Init(TIM16, &TIM_OCInitStructure);

    serDbg("startLight Complete\n");

    while (true)
    {
        if (settings.data.functions & SETTINGS_FUNCTION_LED)
        {
            updateLight(light_settings);
        }
        nilThdSleepMilliseconds(60);
    }
}

void updateLight(light_settings_t s)
{
    switch (s.state) {
        case LIGHT_STATE_OFF:
            // Turn light off
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        case LIGHT_STATE_STILL:
            // Turn light on
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        case LIGHT_STATE_BLINK:
            // Blink light
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
            TIM_OCInitStructure.TIM_Pulse = s.duration; // period in ms
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        case LIGHT_STATE_PULSE:
            // Blink light
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
            TIM_OCInitStructure.TIM_Pulse = s.duration; // period in ms
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        default:
            // Turn light off
            break;
    }
}
