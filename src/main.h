/*
 * main.h
 *
 *  Created on: 17 янв. 2018 г.
 *      Author: fossa
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "helpers/stm32_macroses.h"
#include "hal/clock.h"
#include "hal/display/driver_prototype.h"
#include "hal/display/driver_implementation.h"

int main(int argc, char* argv[]);

#endif /* MAIN_H_ */
