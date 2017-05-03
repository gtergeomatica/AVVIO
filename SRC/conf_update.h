/** this the header file for libraries, constants and functions that manage the configuration update process for rtkrcv at system's boot       
**/

#ifndef __CONF_UPDATE_H
#define __CONF_UPDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  constants used in the conf update process  */

#define MAX_LENGTH   3000     /* maximum number of iterations     */
#define MAX_SNR_L1   9     /* macro to update the field pos1-snrmask_L1   */

/* declarations of functions for the update of rtkrcv configuration file    */
extern int jsoneq(const char *json, jsmntok_t *tok, const char *s);
extern int idx_token_conf(const char *str, jsmntok_t *token, int *count, const char *json_string);
extern int conf_update(int *count, jsmntok_t *token, const char *json_string);
extern int conf_parse(void );

#ifdef __cplusplus
}
#endif

#endif /* __CONF_UPDATE_H */
