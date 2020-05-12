/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_gpio.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : May 7, 2020 1.00
 *******************************************************************************/
 
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_gpio_if.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/ 

/******************************************************************************
 Macro definitions
 ******************************************************************************/
 
/*****************************************************************************
 Constant Data
 ******************************************************************************/
 
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/


/** Initialize the GPIO pin
 *
 * @param pin   The pin name
 */
void gpio_init(PinName pin ) {
	int n = pin >> 4;
	int shift = pin;
	int bitmask = 1 << shift;

	/* Set to Port Mode */
	rza_io_reg_write_16( PMC(n),  0, shift, bitmask);
}

/** Set the input pin mode
 *
 *  @param pin   The pin name
 * @param mode The pin mode to be set
 */
void gpio_mode(PinName pin, PinMode mode){

}

/** Set the pin direction
 *
 *  @param pin   The pin name
 * @param direction The pin direction to be set
 */
void gpio_dir(PinName pin, PinDirection direction){
	int n = pin >> 4;
	int shift = pin & 0xF;
	int bitmask = 1 << shift;

	/* Set to Port Mode */
	switch ( direction) {
		case PIN_INPUT :
			rza_io_reg_write_32( PMSR(n),  1, shift+16, bitmask<<16);
			rza_io_reg_write_32( PMSR(n),  1, shift, bitmask);
			rza_io_reg_write_16( PIBC(n),  1, shift, bitmask);
			break;
		case PIN_OUTPUT :
			rza_io_reg_write_32( PMSR(n),  1, shift+16, bitmask<<16);
			rza_io_reg_write_32( PMSR(n),  0, shift, bitmask);
			rza_io_reg_write_16( PIBC(n),  0, shift, bitmask);
			break;
		default :
			break;
	}

}

/** Set the output value
 *
 * @param pin   The pin name
 * @param value The value to be set
 */
void gpio_write(PinName pin, int value){
	int n = pin >> 4;
	int shift = pin;
	int bitmask = 1 << shift;

	/* Set to Port Mode */
	rza_io_reg_write_32( PSR(n),  1, shift+16, bitmask<<16);
	rza_io_reg_write_32( PSR(n),  (value & 0x0001), shift, bitmask);
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
int gpio_read(PinName pin){
	int n = pin >> 4;
	int shift = pin;
	int bitmask = 1 << shift;
	int value = 0;

	/* Set to Port Mode */
	value = rza_io_reg_read_32( PSR(n),  0, shift, bitmask);

	return value;
}
