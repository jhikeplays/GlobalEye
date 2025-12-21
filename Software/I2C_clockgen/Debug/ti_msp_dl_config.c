/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0C110X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMER_FRM_init();
    SYSCFG_DL_TIMER_EXP_init();
    SYSCFG_DL_I2C_init();
    SYSCFG_DL_TIMER_Cross_Trigger_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_TimerA_reset(TIMER_FRM_INST);
    DL_TimerG_reset(TIMER_EXP_INST);
    DL_I2C_reset(I2C_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_TimerA_enablePower(TIMER_FRM_INST);
    DL_TimerG_enablePower(TIMER_EXP_INST);
    DL_I2C_enablePower(I2C_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SDA,
        GPIO_I2C_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SCL,
        GPIO_I2C_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SCL);

    DL_GPIO_initDigitalOutput(TEXP_TEXP1_IOMUX);

    DL_GPIO_clearPins(TEXP_PORT, TEXP_TEXP1_PIN);
    DL_GPIO_enableOutput(TEXP_PORT, TEXP_TEXP1_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (3000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   30000 Hz = 3000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_FRMClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 99U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_FRM_INST_LOAD_VALUE = (66.666 ms * 30000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_FRMTimerConfig = {
    .period     = TIMER_FRM_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_FRM_init(void) {

    DL_TimerA_setClockConfig(TIMER_FRM_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_FRMClockConfig);

    DL_TimerA_initTimerMode(TIMER_FRM_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_FRMTimerConfig);
    DL_TimerA_enableInterrupt(TIMER_FRM_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableShadowFeatures(TIMER_FRM_INST);
    DL_TimerA_enableClock(TIMER_FRM_INST);





    DL_TimerA_configCrossTrigger(TIMER_FRM_INST, DL_TIMER_CROSS_TRIG_SRC_ZERO,
	DL_TIMER_CROSS_TRIGGER_INPUT_ENABLED, DL_TIMER_CROSS_TRIGGER_MODE_ENABLED
		);

    DL_TimerA_setCaptureCompareInput(TIMER_FRM_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);

    /*
     * Determines the external triggering event to trigger the module (self-triggered in main configuration)
     * and triggered by specific timer in secondary configuration
     */
    DL_TimerA_setExternalTriggerEvent(TIMER_FRM_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_0);
    DL_TimerA_enableExternalTrigger(TIMER_FRM_INST);
    uint32_t temp;
    temp = DL_TimerA_getCaptureCompareCtl(TIMER_FRM_INST, DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareCtl(TIMER_FRM_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_0_INDEX);
}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (4800000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   400000 Hz = 4800000 Hz / (5 * (11 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_EXPClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_5,
    .prescale    = 11U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_EXP_INST_LOAD_VALUE = (5ms * 400000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_EXPTimerConfig = {
    .period     = TIMER_EXP_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_EXP_init(void) {

    DL_TimerG_setClockConfig(TIMER_EXP_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_EXPClockConfig);

    DL_TimerG_initTimerMode(TIMER_EXP_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_EXPTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_EXP_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_EXP_INST);






    DL_TimerG_setCaptureCompareInput(TIMER_EXP_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);

    /*
     * Determines the external triggering event to trigger the module (self-triggered in main configuration)
     * and triggered by specific timer in secondary configuration
     */
    DL_TimerG_setExternalTriggerEvent(TIMER_EXP_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_0);
    DL_TimerG_enableExternalTrigger(TIMER_EXP_INST);
    uint32_t temp;
    temp = DL_TimerG_getCaptureCompareCtl(TIMER_EXP_INST, DL_TIMER_CC_0_INDEX);
    DL_TimerG_setCaptureCompareCtl(TIMER_EXP_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_0_INDEX);
}

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_Cross_Trigger_init(void) {
}

static const DL_I2C_ClockConfig gI2CClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2C_init(void) {

    DL_I2C_setClockConfig(I2C_INST,
        (DL_I2C_ClockConfig *) &gI2CClockConfig);
    DL_I2C_disableAnalogGlitchFilter(I2C_INST);

    /* Configure Target Mode */
    DL_I2C_setTargetOwnAddress(I2C_INST, I2C_TARGET_OWN_ADDR);
    DL_I2C_setTargetTXFIFOThreshold(I2C_INST, DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setTargetRXFIFOThreshold(I2C_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);

    DL_I2C_enableTargetClockStretching(I2C_INST);

    /* Workaround for errata I2C_ERR_04 */
    DL_I2C_disableTargetWakeup(I2C_INST);
    /* Configure Interrupts */
    DL_I2C_enableInterrupt(I2C_INST,
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_TARGET_START |
                           DL_I2C_INTERRUPT_TARGET_STOP |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY |
                           DL_I2C_INTERRUPT_TARGET_TX_DONE);


    /* Enable module */
    DL_I2C_enableTarget(I2C_INST);


}

