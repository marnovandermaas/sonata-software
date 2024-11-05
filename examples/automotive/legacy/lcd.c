// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "lcd.h"

#include "../../../third_party/display_drivers/core/lucida_console_10pt.h"
#include "../../../third_party/display_drivers/core/m3x6_16pt.h"
#include "../../../third_party/display_drivers/st7735/lcd_st7735.h"
#include "../../../third_party/sonata-system/sw/legacy/common/sonata_system.h"
#include "../../../third_party/sonata-system/sw/legacy/common/spi.h"
#include "../../../third_party/sonata-system/sw/legacy/common/timer.h"

static void timer_delay(uint32_t ms)
{
	uint32_t timeout = get_elapsed_time() + ms;
	while (get_elapsed_time() < timeout)
	{
		__asm__ volatile("wfi");
	}
}

static uint32_t spi_write(void *handle, uint8_t *data, size_t len)
{
	spi_tx((spi_t *)handle, data, len);
	spi_wait_idle((spi_t *)handle);
	return len;
}

static uint32_t gpio_write(void *handle, bool cs, bool dc)
{
	spi_set_cs((spi_t *)handle, LcdDcLine, dc);
	spi_set_cs((spi_t *)handle, LcdCsLine, cs);
	return 0;
}

int lcd_init(spi_t *spi, pwm_t lcd_bl, St7735Context *lcd, LCD_Interface *interface)
{
	// Set the initial state of the LCD control pins
	spi_set_cs(spi, LcdDcLine, 0x0);
	set_pwm(lcd_bl, 1, 255);
	spi_set_cs(spi, LcdCsLine, 0x0);

	// Reset the LCD
	spi_set_cs(spi, LcdRstLine, 0x0);
	timer_delay(150);
	spi_set_cs(spi, LcdRstLine, 0x1);

	// Init LCD Driver, and set the SPI driver
	interface->handle      = spi;         // SPI handle.
	interface->spi_write   = spi_write;   // SPI write callback.
	interface->gpio_write  = gpio_write;  // GPIO write callback.
	interface->timer_delay = timer_delay; // Timer delay callback.
	lcd_st7735_init(lcd, interface);

	// Set the LCD orientation
	lcd_st7735_set_orientation(lcd, LCD_Rotate180);

	// Setup text font bitmaps to be used and the colors.
	lcd_st7735_set_font(lcd, &m3x6_16ptFont);
	lcd_st7735_set_font_colors(lcd, BGRColorWhite, BGRColorBlack);

	// Clean display with a white rectangle.
	lcd_st7735_clean(lcd);

	return 0;
}
