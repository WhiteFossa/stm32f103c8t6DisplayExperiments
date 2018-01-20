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

int main(int argc, char* argv[]);

/**
 * Changes led state.
 */
void ToggleLed(void);


#endif /* MAIN_H_ */
