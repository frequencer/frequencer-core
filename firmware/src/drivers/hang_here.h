/*
 * File:   hang_here.h
 * Author: jrowley
 *
 * Created on June 15, 2021, 8:45 PM
 */

#include <config/default/peripheral/coretimer/plib_coretimer.h>
#include <config/default/peripheral/gpio/plib_gpio.h>
#include <stdbool.h>

#ifndef HANG_HERE_H
#define HANG_HERE_H

#ifdef  __cplusplus
extern "C" {
#endif


#define HANG_HERE() { while (true){ LED3_Toggle(); CORETIMER_DelayMs(50); }\
}


#ifdef  __cplusplus
}
#endif

#endif /* HANG_HERE_H */
