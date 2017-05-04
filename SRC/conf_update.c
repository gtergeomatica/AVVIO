#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "conf_update.h"

#define MAX_LENGTH   3000
#define MAX_SNR_L1   9
#define FNAME_CNF_RTKLIB "./narv2.conf"
#define FNAME_JSON "./conf_prms.json"
#define FNAME_CNF_NARV "./narvalo_conf.dat"

/*
 * this function checks whether the i-th token parsed by the parser is equal to one of the strings we need to update in the conf file
 */

extern int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	char *aux;
	int i;

	aux = (char *) malloc(sizeof(*aux)*(tok->end - tok->start));
	if (aux == NULL) {
		printf("Something wrong with dynamic allocation"); 
		return -1;
	}
	
	for(i=0; i< (tok->end - tok->start); i++)   
		aux[i] = json[tok->start+i];     /* auxiliary string for the token    */

	if (tok->type == JSMN_PRIMITIVE && (strstr(aux, s) != NULL)) {   /* here it is checked if the token is equal to the string s; if yes, free memory and returns to the caller*/
		free(aux);		
		return 0;
	}
	free(aux);
	return -1;
}
/* this function identify which token is equal to the string we need to update in the conf file; if found returns its position, otherwise -1    */
extern int idx_token_conf(const char *str, jsmntok_t *token, int *count, const char *json_string) {
	int i;

	/* Loop over all keys of the root object searching for */
	for (i = 1; i < *count; i++) {
		if (jsoneq(json_string, &token[i], str) == 0) {
			printf("- Eccolo: %.*s, %d, \n", token[i+2].end-token[i+2].start,
					json_string + token[i+2].start, i);
			return i;
		} 	
	}
	return -1;
}


/* This function changes the configuration parameters of the file .conf used by rtkrcv according to the json message received by the "Concentratore"  */
extern int conf_update(int *count, jsmntok_t *token, const char *json_string)    {

	FILE *f_conf;
	int i=0, N = 141, k, j, l=1, f_pos_end=0, f_pos_start=0, flag=0, f=0, t=0;
	char *str, *par;

	/* load the conf file of rtkrcv   */	
	f_conf = fopen(FNAME_CNF_RTKLIB, "r+");
	if (f_conf == NULL)  {
		printf("Error in opening the file conf\n");
		return -1;
	}
	
	/*read each line of the configuration file,check if contains the parameter to be
	updated and modify it according to the value in the json message */ 
	str = malloc(sizeof(*str)*N);
	do {		
		str = fgets(str, N, f_conf);  /* import the string in the conf file in an auxiliary string */  
		if(feof(f_conf))  break;
		f_pos_start=f_pos_end;
		f_pos_end = ftell(f_conf); /* two variables to know the beginning and end of each string in the conf file */
		fseek(f_conf, (f_pos_start - f_pos_end), SEEK_CUR);  		/* after copying the string, move the file pointer at the beginning of the string   */

		if(strstr(str, "pos1-elmask") != NULL)   {    /*it checks if the present string contains the substring pos1-elmask  */
			par = strrchr(str, '=');       /* if yes, it identifies where is the equal  */  
			l=1;		
			k = idx_token_conf("pos1-elmask", token, count, json_string);     /* finds the right token   */
			if (k == -1) continue;			/*if no token is found jumps to next iteration*/
			
			printf("Giusto valore: %.*s\n", token[k+2].end-token[k+2].start,
				json_string + token[k+2].start);
			for(j=0;j<(token[k+2].end-token[k+2].start);j++)    {
				if((json_string[token[k+2].start+j] == '\\') || (json_string[token[k+2].start+j] == '\"'))  continue;
				else {   
					*(par+l) = json_string[token[k+2].start+j];    /* the string of the conf file is updated with the new value sent by the Concentratore */
					l++;
				}
			}
			printf("Nuova stringa da inserire nel file conf: %s\n", str);
			if(fputs(str, f_conf)==EOF)  {     /* the updated string is put into the conf file, to update it  */
				printf("Hoops...something went wrong...\n");
				return -1;
			}
		}
		else if(strstr(str, "pos1-snrmask_r") != NULL)   {   /*the structure of this alternative is exactly the same as that for pos1-elmask */
			par = strrchr(str, '=');
			l=1;		
			k = idx_token_conf("pos1-snrmask_r", token, count, json_string);
			if (k == -1) continue;

			printf("Giusto valore: %.*s\n", token[k+2].end-token[k+2].start,
				json_string + token[k+2].start);
			for(j=0;j<(token[k+2].end-token[k+2].start);j++)    {
				if((json_string[token[k+2].start+j] == '\\') || (json_string[token[k+2].start+j] == '\"'))  continue;
				else {   
					*(par+l) = json_string[token[k+2].start+j];  /* the string of the conf file is updated with the new value sent by the Concentratore */
					l++;
				}
			}
			*(par+l)= ' ';			
			printf("Nuova stringa da inserire nel file conf: %s\n", str);
			if(fputs(str, f_conf)==EOF)  {    /* the updated string is put into the conf file, to update it  */
				printf("Hoops...something went wrong...\n");
				return -1;
			}
		}
		else if(strstr(str, "pos1-snrmask_L1") != NULL)   {
			par = strrchr(str, '=');
			l=1;		
			k = idx_token_conf("pos1-snrmask_L1", token, count, json_string);
			if (k == -1) continue;

			printf("Giusto valore: %.*s\n", token[k+2].end-token[k+2].start,
				json_string + token[k+2].start);
			do    {
				for(j=0;j<(token[k+2].end-token[k+2].start);j++)    {
					if((json_string[token[k+2].start+j] == '\\') || (json_string[token[k+2].start+j] == '\"'))  continue;
					else {   
						*(par+l) = json_string[token[k+2].start+j];
						l++;
					}
				}
				*(par+l) = '.';  /* this new components are needed in order to format correctly all values of the snr mask*/
				l++;
				*(par+l) = '0';
				l++;
				*(par+l)= ','; 
				l++;
				t++;   
			} while (t < MAX_SNR_L1);
			*(par+(l-1))= ' ';	/*this eliminates the last comma */
			printf("Nuova stringa da inserire nel file conf: %s\n", str);
			if(fputs(str, f_conf)==EOF)  {
				printf("Hoops...something went wrong...\n");
				return -1;
			}
		}				
		else if((strstr(str, "pos1-mdp") != NULL) && (flag==0))   {  /*the structure of this alternative is exactly the same as that for pos1-elmask */
			par = strrchr(str, '=');
			l=1;
			flag=1;		
			k = idx_token_conf("pos1-mdp", token, count, json_string);
			if (k == -1) continue;
			
			printf("Giusto valore: %.*s\n", token[k+2].end-token[k+2].start,
				json_string + token[k+2].start);
			for(j=0;j<(token[k+2].end-token[k+2].start);j++)    {
				if((json_string[token[k+2].start+j] == '\\') || (json_string[token[k+2].start+j] == '\"'))  continue;
				else {   
					*(par+l) = json_string[token[k+2].start+j];
					l++;
				}
			}
			*(par+l)= ' ';			
			printf("Nuova stringa da inserire nel file conf: %s\n", str);
			if(fputs(str, f_conf)==EOF)  {
				printf("Hoops...something went wrong...\n");
				return -1;
			}
		}
		else if(strstr(str, "pos1-mdp_sigma") != NULL)   {  /*the structure of this alternative is exactly the same as that for pos1-elmask */
			par = strrchr(str, '=');
			l=1;		
			k = idx_token_conf("pos1-mdp_sigma", token, count, json_string);
			if (k == -1) continue;			
			
			printf("Giusto valore: %.*s\n", token[k+2].end-token[k+2].start,
				json_string + token[k+2].start);
			for(j=0;j<(token[k+2].end-token[k+2].start);j++)    {
				if((json_string[token[k+2].start+j] == '\\') || (json_string[token[k+2].start+j] == '\"'))  continue;
				else {   
					*(par+l) = json_string[token[k+2].start+j];
					l++;
				}
			}
			printf("Nuova stringa da inserire nel file conf: %s", str);
			if(fputs(str, f_conf)==EOF)  {
				printf("Hoops...something went wrong...");
				return -1;
			}
		}
		else   {   /*this erases all not needed in each string text and move the file pointer to the end of the current string */
		 	for (f = 0; f<(f_pos_end -f_pos_start); f++)   
				*(str+f) = ' ';
			fseek(f_conf, (f_pos_end -f_pos_start), SEEK_CUR);		
		}
		i++;	
	} while(i < MAX_LENGTH);    

	fclose(f_conf); free(str);   
	return 1;
}
/* ----------------------------------------------------------------------------------------

This is the function that realizes the json parsing and the creation of the configuration file for NARVALO. In particular it creates a conf file with the following information
	- ID of the device
	- type of the device
	- IP address of the CC
	- IP port of the CC

-------------------------------------------------------------------------------------------    */

extern int conf_parse(void ) {
	FILE *f_json, *f_narv;  /* Pointer to the file where the json string has been downloaded and of the conf file for NARVALO */
	int i, r, json_length=0, flag=0;
	/*char filename[]= "./conf_prms.json";*/   /* string of the filename of the json string */
	/*char filename_narv[]= "./narvalo_conf.dat"; */  /* string of the filename of conf file NARVALO */
	char *json_string; /* string that stores the json string read from file */		
	jsmn_parser p;
	jsmntok_t t[100]; 
	
	jsmn_init(&p);

	f_json = fopen(FNAME_JSON,"r");   /* read the json string from file */
	if (f_json == NULL)  {
		printf("Error in opening the file json\n");
		return -1;
	}

	f_narv=fopen(FNAME_CNF_NARV, "a");  /* create the file with conf info for NARVALO  */
	if (f_narv == NULL)  {
		printf("Error in creating the configuration file for NARVALO system\n");
		return -1;
	}	

	while(fgetc(f_json) != EOF) { /* computes json string length */ 
		/* move the file position cursor by one and increment json_length 		
		fseek(f_json, ++json_length, SEEK_SET);   */
		json_length++;		
	}	

	/* dynamic allocation of memory for json_string */
	json_string = (char *) malloc(sizeof(*json_string)*(json_length));

	if (json_string == NULL)   {
		printf("Something wrong with dynamic allocation of memory\n");
		return -1;
	}
	
	/* create string with the content of the json string read from file 
	rewind(f_json);	*/
	/*json_string = fgets(json_string, json_length, f_json);   
	
	n_ele = fread(json_string, sizeof(*json_string), json_length, f_json); */
	i=0; fseek(f_json, 1, SEEK_SET); 	
	do {
		json_string[i] = fgetc(f_json);
		if(feof(f_json)) break;
		if((i>=1) && (json_string[i] == '"') && (json_string[i-1] == '}'))   {
			json_string[i] = '\0';
			break;
		}
		i++;
	}while(i < (json_length-1));   

	/* deletion of the possible redudancy of final characters \0!!!!
	for(i=n_ele;;--i)    {
		if(json_string[i] == '"')   {
			json_string[i] = 0;
 			break;
		}	
		else continue;
	}    */
		
	/*r = jsmn_parse(&p, JSON_STRING, json_length, t, sizeof(t)/sizeof(t[0]));    */
	r = jsmn_parse(&p, json_string, (json_length-3), t, sizeof(t)/sizeof(t[0]));   
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return -1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return -1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(json_string, &t[i], "conf") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- Conf: %.*s\n", t[i+1].end-t[i+1].start,
					json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "status") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Status: %.*s\n", t[i+1].end-t[i+1].start,
					json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "params") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- params: %.*s\n", t[i+1].end-t[i+1].start,
				json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "id") == 0 && !flag) {
			/* We find where the device's id is and store it in f_narv */
			printf("- id: %.*s\n", t[i+1].end-t[i+1].start,
				json_string + t[i+1].start);
			fprintf(f_narv, "%.*s\n", t[i+1].end-t[i+1].start, json_string + t[i+1].start); 
			i++;
			flag=1; 
		} else if (jsoneq(json_string, &t[i], "idAddress") == 0) {
			/* We find where the IP address of the CC is and store it in f_narv */
			printf("- IP Address: %.*s\n", t[i+1].end-t[i+1].start,
				json_string + t[i+1].start);
			fprintf(f_narv, "%.*s\n", t[i+1].end-t[i+1].start, json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "type") == 0) {
			/* We find where the device's type is and store it in f_narv */
			printf("- type: %.*s\n", t[i+1].end-t[i+1].start,
				json_string + t[i+1].start);
			fprintf(f_narv, "%.*s\n", t[i+1].end-t[i+1].start, json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "ipPort") == 0) {
			/* We find where the connection port of the CC is and store it in f_narv */
			printf("- IP Port: %.*s\n", t[i+1].end-t[i+1].start,
				json_string + t[i+1].start);
			fprintf(f_narv, "%.*s\n", t[i+1].end-t[i+1].start, json_string + t[i+1].start);
			i++;
		} else if (jsoneq(json_string, &t[i], "rows") == 0) {
			int j;
			printf("- Rows:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, json_string + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("- Key: %.*s\n", t[i].end-t[i].start,
				json_string + t[i].start);
		}
	}

	if(conf_update(&r, t, json_string) == -1) return -1;   /*the function for updating the conf file is called here */   

	fclose(f_json);	fclose(f_narv); free(json_string);
	return 1;
}

