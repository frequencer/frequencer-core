/*******************************************************************************
*  MPLAB Harmony Application Header File
*
*  Company:
*   Microchip Technology Inc.
*
*  File Name:
*   app.h
*
*  Summary:
*   This header file provides prototypes and definitions for the application.
*
*  Description:
*   Good grief they litter this thing with block comments
*******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "configuration.h"


#ifndef APP_H
#define APP_H


#define CPRINT(format) { SYS_CONSOLE_Print(sysObj.sysConsole0, format); }
#define CPRINTF(format, ...) { SYS_CONSOLE_Print(sysObj.sysConsole0, format, __VA_ARGS__); }

#define APP_START_STATE APPS_QUERY_PLL


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	APPS_NONE = 0,
	APPS_INIT,
	APPS_WAIT_USER_READY,
	APPS_QUERY_PLL,
	APPS_IDLE,
	APPS_INT_SEND_MESSAGE,
	APPS_INT_TOGGLE_LED,
}
app_states_t;


void app_init (void);

void app_task (void);


#ifdef __cplusplus
}
#endif


#endif /* APP_H */
