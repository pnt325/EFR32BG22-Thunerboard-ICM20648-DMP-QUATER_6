/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "em_cmu.h"
#include "blink.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_led.h"
#include "sl_simple_button.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "sl_icm20648.h"
#include "sl_imu.h"
#include "app_assert.h"
#include "sl_board_control.h"

#include "Invn/Devices/Drivers/Icm20648/Icm20648.h"
#include "Invn/Devices/Drivers/Ak0991x/Ak0991x.h"
#include "Invn/Devices/SensorTypes.h"
#include "Invn/Devices/SensorConfig.h"
#include "Invn/EmbUtils/InvScheduler.h"
#include "Invn/EmbUtils/RingByteBuffer.h"
#include "Invn/EmbUtils/Message.h"
#include "Invn/EmbUtils/ErrorHelper.h"
#include "Invn/EmbUtils/DataConverter.h"
#include "Invn/EmbUtils/RingBuffer.h"
#include "Invn/DynamicProtocol/DynProtocol.h"
#include "Invn/DynamicProtocol/DynProtocolTransportUart.h"
#include "system.h"
#include "sensor.h"
#include "gpiointerrupt.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
volatile uint32_t tick_cnt = 0;

extern void delay_us(uint32_t us);

static void icm20648_on_change(uint8_t interrupt_no, void *ctx);

void app_init(void) {
	int rc = 0;

	// IMU Interrupt configure
	unsigned int interrupt;
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortB, 3, gpioModeInput, 0);
	interrupt = GPIOINT_CallbackRegisterExt(3,
			(GPIOINT_IrqCallbackPtrExt_t) icm20648_on_change,
			NULL);

	// IMU init
	sl_status_t sc;
	sc = sl_board_enable_sensor(SL_BOARD_SENSOR_IMU);
	app_assert_status(sc);

	sc = sl_icm20648_init();
	app_assert_status(sc);

	//========================================================================//
	// eMD Library init
	//========================================================================//
	struct inv_icm20648_serif icm20648_serif;
	icm20648_serif.context   = 0; /* no need */
	icm20648_serif.read_reg  = idd_io_hal_read_reg;
	icm20648_serif.write_reg = idd_io_hal_write_reg;
	icm20648_serif.max_read  = 1024*16; /* maximum number of bytes allowed per serial read */
	icm20648_serif.max_write = 1024*16; /* maximum number of bytes allowed per serial write */

	icm20648_serif.is_spi    = interface_is_SPI();
	inv_icm20648_reset_states(&icm_device, &icm20648_serif);

	/*
	 * Setup the icm20648 device
	 */
	icm20648_sensor_setup();

	/*
	* Now that Icm20648 device was initialized, we can proceed with DMP image loading
	* This step is mandatory as DMP image are not store in non volatile memory
	*/
	rc += load_dmp3();
	check_rc(rc, "Error sensor_setup/DMP loading.");

	// Enable interrupt
	GPIO_ExtIntConfig(gpioPortB, 3, interrupt,
	false,
	true,
	true);

	inv_icm20648_enable_dmp_quater_6();
}

void InvEMDFrontEnd_busyWaitUsHook(uint32_t us){
	delay_us(us);
}

int InvEMDFrontEnd_isHwFlowCtrlSupportedHook(void){
	return 0;
}

int InvEMDFrontEnd_putcharHook(int c){
	return c;
//	if(usart_serial_putchar(CONF_UART, (uint8_t)c))
//		return c;
//	else
//		return -1;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
volatile uint8_t icm20648_int = 0;
void app_process_action(void)
{
	if(icm20648_int)
	{
		inv_icm20648_poll_sensor(&icm_device, (void *)0, build_sensor_event_data);

		__disable_irq();
		icm20648_int = 0;
		__enable_irq();
	}
}

uint64_t inv_icm20648_get_time_us(void) {
	uint64_t time_ms = sl_sleeptimer_get_tick_count64() * 1000
			/ sl_sleeptimer_get_timer_frequency();
	return time_ms * 1000;	// convert time to us
}

static void icm20648_on_change(uint8_t interrupt_no, void *ctx)
{
	(void)interrupt_no;
	(void)ctx;
	icm20648_int = 1;

}
