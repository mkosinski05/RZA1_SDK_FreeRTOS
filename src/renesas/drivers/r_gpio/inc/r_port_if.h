/*
 * r_port_if.h
 *
 *  Created on: May 6, 2020
 *      Author: Oceania
 */

#ifndef RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_
#define RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_

#include "PinNames.h"

typedef struct {
    PinName 		pin;
    PinFunc 		function;
    PinDirection	dir;
} PinMap;

void set_pin_function( const PinMap *map);

#endif /* RENESAS_DRIVERS_R_GPIO_R_PORT_IF_H_ */
