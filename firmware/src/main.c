/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "app.h"
#include "definitions.h"                // SYS function prototypes


int
main (void)
{
    // Initialize all modules
    SYS_Initialize(NULL);
	app_init();

    while (true)
    {
		// Run all tasks
        SYS_Tasks();
		app_task();
    }

    return EXIT_FAILURE;
}
