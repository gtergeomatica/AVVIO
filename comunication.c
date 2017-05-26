/* GTER copyleft */
/* Author: Alessandro Fermi */
/**  This is the file managing the SW modules responsible of the comunication between the server "Concentratore" and the devices for what concerns the exchange of the configuration parameters and the exchange of diagnostics parameters. 

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf_update.h"

/* main program calling the configuration SW routines and managing the diagnostics of each device      */
int main() {
	
	int status;
	
	status = conf_parse();  /* call to configuration SW suite   */
	if (status == -1)
		return EXIT_FAILURE;
	else 
		return EXIT_SUCCESS;
}


