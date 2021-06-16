/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
	Microchip Technology Inc.

  File Name:
	app.h

  Summary:
	This header file provides prototypes and definitions for the application.

  Description:
	Good grief they litter this thing with block comments
 *******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"


#ifndef APP_H
#define APP_H


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	APPS_NONE = 0,
	APPS_INIT,
	APPS_IDLE,
	APPS_SEND_MESSAGE,
}
app_states_t;


void app_init (void);

void app_task (void);


#ifdef __cplusplus
}
#endif


#endif /* APP_H */
