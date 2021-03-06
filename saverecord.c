#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "saverecord.h"
#include "arrangerecord.h"
#include "makepw.h"
#include "encryption.h" 
#define BUFSIZE 256
#define ISIZE 50
#define NSIZE 25


int saverecordmain(void) {

	account *record = malloc(sizeof(account));
	while (enter_record("Enter the website\n", "Enter the username\n", "Enter the password\n", record) != 0) {
	}
	free(record);

	return 0;
}

int confirmation(char *username, char *password, char *site, char *confirm, account *prec){
	char line[BUFSIZE];
    while (1) {
        printf("Is this correct?(Y/N)\n");
        printf("%s %s %s\n", site, username, password);

        if (!fgets(line, BUFSIZE, stdin)) {
            clearerr(stdin);
            return 0;
        }
        sscanf(line, "%[YyNn]", confirm);

        if (strcmp(confirm, "Y") == 0 || strcmp(confirm, "y") == 0) {
            encrypt(password);
            strcpy(prec->username, username);
            strcpy(prec->password, password);
            strcpy(prec->site, site);
        
                printf("stored %s %s %s\n", prec->site, prec->username, prec->password);
        
                printf("stored\n");
            
                return 1;
        }

        if (strcmp(confirm, "N") == 0 || strcmp(confirm, "n") == 0) {
            printf("cleared");
            clearerr(stdin);
            return 0;
        }
    }
}

int passwordvalid(const char *prompt, char *username, char *password, char *site, char *confirm, account *prec){
	int length;
	char line[BUFSIZE];
	char pass[NSIZE];
	printf("%s", prompt);
	while (1) {
		if (!fgets(line, BUFSIZE, stdin)) {
			clearerr(stdin);
			return 0;
		}
		if (sscanf(line, "%s", password) != 1) {
			clearerr(stdin);
			printf("Invalid. %s\n", prompt);
		}
		else {
			/*Enter encrypt function right here.
			And then store it into the struct.*/
			if (password[0] == '@') {
				return 0;
			}
			if (password[0] == 'r' && password[1] == 'd') {
				length = get_valid_int("Enter a Password Length", 0, 5, 55);
				makepw(length, pass);
				strcpy(password, pass);
			}
			if(confirmation(username, password, site, confirm, prec)) {
				removeSpaces(prec);
				store_record(prec);
				printf("success!\n");
			}
		}
	}
}

int usernamevalid(const char *prompt, const char *prompt2, char *username, char *password, char *site, char *confirm, account *prec) {
	char line[BUFSIZE];
	printf("%s", prompt);
	while (1) {
		if (!fgets(line, BUFSIZE, stdin)) {
			clearerr(stdin);
			return 0;
		}
		if (sscanf(line, "%s", username) != 1) {
			clearerr(stdin);
			printf("Invalid. %s\n", prompt);
		}
		else {
			if (username[0] == '@') {
				return 0;
			}
			passwordvalid(prompt2, username, password, site, confirm, prec);
		}
	}
}

int enter_record(const char *prompt1, const char *prompt2, const char *prompt3, account *prec) {
	char line[BUFSIZE];
	char site[ISIZE];
	char username[NSIZE];
	char password[NSIZE];
	char confirm[NSIZE];

	printf("%s", prompt1);
	while (1) {
		if (!fgets(line, BUFSIZE, stdin)) {
			clearerr(stdin);
			return 0;
		}
		if (sscanf(line, "%[^\n]s", site) != 1) {
			clearerr(stdin);
			printf("Invalid. %s\n", prompt1);
		}
		else {
			if (site[0] == '@') {
				return 0;
			}
			usernamevalid(prompt2, prompt3, username, password, site, confirm, prec);
		}
	}
	return 0;
}

/*Print function to print the struct into the file here*/
int store_record(const account *prec) {
	char path[BUFSIZE] = "./login/";
	FILE *fp;
	strcat(path, prec->site);
	if ((fp = fopen(path, "a+")) == 0) {
		perror("fopen");
		return 1;
	}

	fprintf(fp, "%s %s ", prec->username, prec->password);

	if (fclose(fp) != 0) {
		perror("fclose");
		return 2;
	}

	return 0;
}
