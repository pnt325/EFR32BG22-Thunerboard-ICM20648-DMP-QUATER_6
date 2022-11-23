/*
 * ________________________________________________________________________________________________________
 * Copyright (c) 2017 InvenSense Inc. All rights reserved.
 *
 * This software, related documentation and any modifications thereto (collectively �Software�) is subject
 * to InvenSense and its licensors' intellectual property rights under U.S. and international copyright
 * and other intellectual property rights laws.
 *
 * InvenSense and its licensors retain all intellectual property and proprietary rights in and to the Software
 * and any use, reproduction, disclosure or distribution of the Software without an express license agreement
 * from InvenSense is strictly prohibited.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE SOFTWARE IS
 * PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * INVENSENSE BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 * ________________________________________________________________________________________________________
 */
/*
 * Select communication between Atmel and INV device
 */
#define USE_SPI_NOT_I2C				1		/* Default configuration - I2C */

#define COMPASS_IS_AK9912			0		/* Set to 1 when external compass connected on the board */

#if COMPASS_IS_AK9912
#define AK0991x_DEFAULT_I2C_ADDR	0x0C	/* The default I2C address for AK0991x Magnetometers */
#define AK0991x_SECONDARY_I2C_ADDR  0x0E	/* The secondary I2C address for AK0991x Magnetometers */
#endif

#define EXTERNAL_SENSOR				0		/* Default configuration is on-board sensor and this flag need not have to be changed */
#define TDK_BOARD_INBUILT_SENSOR	1		/* Default configuration is on-board sensor. Set 0 for external, sensor board on TDK SmartMotion Board */
#define TDK_BOARD_REVISION			1		/* Default configuration is TDK-SmartMotion-RevB. Set 0 for TDK-SmartMotion-RevA */
											/* Configure J7 pins closed on TDK SmartMotion Board RevB with external sensor board for SPI configuration */
#if (EXTERNAL_SENSOR == 1)
#define ICM_I2C_ADDR				0x68	/* I2C slave address for external INV device  */
#define CHIP_SELECT					0
#else
#define ICM_I2C_ADDR				0x69	/* I2C slave address for on-board INV device  */
#define CHIP_SELECT					1
#endif

/*External Interrupt setup */
#if ((TDK_BOARD_INBUILT_SENSOR == 0) && (TDK_BOARD_REVISION==1))
#define PIN_EXT_INTERRUPT			{PIO_PA30, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT | PIO_IT_RISE_EDGE}
#define PIN_EXT_INTERRUPT_MASK		PIO_PA30
#define PIN_EXT_INTERRUPT_PIO		PIOA
#define PIN_EXT_INTERRUPT_ID		ID_PIOA
#define PIN_EXT_INTERRUPT_TYPE		PIO_INPUT
#define PIN_EXT_INTERRUPT_ATTR		(PIO_DEFAULT | PIO_IT_RISE_EDGE)
#define PIN_EXT_INTERRUPT_IRQn		PIOA_IRQn
#else
//#define PIN_EXT_INTERRUPT			{PIO_PB3, PIOB, ID_PIOB, PIO_INPUT, PIO_DEFAULT | PIO_IT_RISE_EDGE}
//#define PIN_EXT_INTERRUPT_MASK		PIO_PB3
//#define PIN_EXT_INTERRUPT_PIO		PIOB
//#define PIN_EXT_INTERRUPT_ID		ID_PIOB
//#define PIN_EXT_INTERRUPT_TYPE		PIO_INPUT
//#define PIN_EXT_INTERRUPT_ATTR		(PIO_DEFAULT | PIO_IT_RISE_EDGE)
//#define PIN_EXT_INTERRUPT_IRQn		PIOB_IRQn
#endif

/*
 * Select communication between Atmel and INV device by setting 0/1 to one of the following defines
 */
#define SERIF_TYPE_SPI				(USE_SPI_NOT_I2C)
#define SERIF_TYPE_I2C				!(USE_SPI_NOT_I2C)

/* The system timer period in microseconds */
#define SYSTIMER_PERIOD_US			10
#define MILLISECONDS_PER_SECOND		1000
#define MICROSECONDS_PER_50ms		50000

void ext_interrupt_handler(void);
void configure_console(void);

#if SERIF_TYPE_SPI
void spi_master_initialize(void);
#endif

int idd_io_hal_read_reg(void * context, uint8_t reg, uint8_t * rbuffer, uint32_t rlen);
int idd_io_hal_write_reg(void * context, uint8_t reg, const uint8_t * wbuffer, uint32_t wlen);
void interface_initialize(void);
inv_bool_t interface_is_SPI(void);
void ext_int_initialize(void (*handler_function)(void));
uint64_t InvEMDFrontEnd_getTimestampUs(void);
void hw_timer_start(uint32_t timer_freq);
void hw_timer_stop(void);
void check_rc(int rc, const char * msg_context);

extern RingByteBuffer uart_rx_rb;
extern volatile uint32_t ul_ticks;
extern InvScheduler 	scheduler;
extern uint8_t I2C_Address;
