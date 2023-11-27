/*
 * simple_encrypt.c
 * 
 * Copyright 2023 Reslashd
 * 
 * A simple practice program to encrypt en decrypt a text file by adding
 * (encrypting) or decreasing (decrypting) each read char by one.
 * 
 * Encrypted files contain the signature S_ENCRYPT23 on the last line
 * so we can check if a file is encrypted or not.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 1024
#define MAX_LENGTH 1024
#define FILENAME_LENGTH 100
#define WRITE_MODE "w"
#define READ_MODE "r"

// * Function prototypes

void addSignature(char file_content[][MAX_LENGTH], int lines_read);
int checkSignature(char file_content[][MAX_LENGTH], int lines_read);
void closeFile(FILE * file_ptr);
void encryptData(char file_content[][MAX_LENGTH], int lines_read);
void decryptData(char file_content[][MAX_LENGTH], int lines_read);
void getFilename(char* file_name);
FILE* openFile(char* file_name, char* file_mode);
int readFile(char file_content[][MAX_LENGTH], FILE * file_ptr);
void removeSignature(char file_content[][MAX_LENGTH], int lines_read);
void showMenu(int has_signature, char file_content[][MAX_LENGTH], int lines_read, FILE * file_ptr, char* file_name);
void writeFile(char file_content[][MAX_LENGTH], FILE * file_ptr, int lines_read);

int main(void)
{
	FILE *file_ptr = NULL;
	char file_name[FILENAME_LENGTH] = {'\0'};		
	char file_content[MAX_LINES][MAX_LENGTH] = {'\0'};
	int lines_read = 0;
	int has_signature = 0;
	
	
	printf("*** SIMPLE ENCRYPT 1.0 by RESLASHD ***\n");
	getFilename(file_name);	
	printf("\nAttempting to open file %s ...\n", file_name);
	
	if (( file_ptr = openFile(file_name, READ_MODE)) != NULL){
		printf("\n*** FILE OPENED ***\n");
		
		// Read file and store in array
		lines_read = readFile(file_content, file_ptr);	
		closeFile(file_ptr);
	
		// Set signature flag for showMenu
		has_signature = checkSignature(file_content, lines_read);
	
		showMenu(has_signature, file_content, lines_read, file_ptr, file_name);		
	}
	printf("\n*** EXITING PROGRAM ***\n");
	return 0;
}

void addSignature(char file_content[][MAX_LENGTH], int lines_read){
	strcpy(file_content[lines_read], "S_ENCRYPT23");
}

int checkSignature(char file_content[][MAX_LENGTH], int lines_read){
	if(strcmp(file_content[lines_read-1],"S_ENCRYPT23") == 0){
		// File is encrypted
		return 1;
	} else{
		// File is not encrypted
		return 0;
	}		
}

void closeFile(FILE * file_ptr)
{	
	// NULL means no file is open so nothing to close
	if ( file_ptr != NULL ){
		fclose(file_ptr);
	}
}

void encryptData(char file_content[][MAX_LENGTH], int lines_read){
	int row = 0; // line number (0 is the first line)
	int column = 0; // character number (0 is the first character on a line)
	
	for(row = 0 ; row < lines_read ; row++){
		column = 0;
		while(file_content[row][column] != '\0'){
			if(file_content[row][column] != '\n'){
				file_content[row][column] = file_content[row][column]+1;
				column++;
			} else{
				column++;
			}		
		}
	}
	addSignature(file_content, lines_read);
}

void decryptData(char file_content[][MAX_LENGTH], int lines_read){
	int row = 0; // line number (0 is the first line)
	int column = 0; // character number (0 is the first character on a line)
	lines_read = lines_read - 1;
	for(row = 0 ; row <= lines_read ; row++){
		column = 0;
		while(file_content[row][column] != '\0'){
			if(file_content[row][column] != '\n'){
				file_content[row][column] = file_content[row][column]-1;
				column++;
			} else{
				column++;
			}		
		}
	}
	lines_read = lines_read + 1;
	removeSignature(file_content, lines_read);
}

void getFilename(char* file_name){
	
	printf("\nPlease enter the name of the file you want encrypt/decrypt  > ");
	fgets(file_name, 100, stdin);		
	file_name[strcspn(file_name, "\n")] = 0;	
}

FILE* openFile(char* file_name, char* file_mode)
{
	FILE *file_ptr = NULL;
	if (( file_ptr = fopen(file_name, file_mode )) == NULL ){
		printf("\n*** ERROR OPENING FILE ***\n");
	}
	return file_ptr;
}

int readFile(char file_content[][MAX_LENGTH], FILE * file_ptr){
	int lines_read = 0;
	
	// While the end of the file is not reached yet AND there are no errors reading the file
	while (!feof(file_ptr) && !ferror(file_ptr)){
		if (fgets(file_content[lines_read], MAX_LENGTH, file_ptr) != NULL){
			lines_read++;	
		}
	 }
	return lines_read;
}

void removeSignature(char file_content[][MAX_LENGTH], int lines_read){
	strcpy(file_content[lines_read-1], "\0");
}

void showMenu(int has_signature, char file_content[][MAX_LENGTH], int lines_read, FILE * file_ptr, char* file_name){
	char choice = '\0';	
	
	printf("\n********* MENU *********\n");
	if( has_signature == 0) {
		printf("1 = Encrypt File \n");	
	}else{
		printf("1 = Decrypt File \n");	
	}
	printf("2 = Close file and exit\n\n");
	printf("************************\n");
	choice = getchar();
	
	if( has_signature == 0 && choice == '1'){
		printf("\nEncrypting file...\n");
		//Open file for writing (erasing all contents!)
		openFile(file_name, WRITE_MODE); 		
		encryptData(file_content, lines_read);	
		writeFile(file_content, file_ptr, lines_read);
		closeFile(file_ptr); 		
		printf("\n*** FILE ENCRYPTED ***\n");	
	} else if(has_signature == 1 && choice == '1'){
		printf("\nDecrypting file...\n");
		//Open file for writing (erasing all contents!)
		openFile(file_name, WRITE_MODE); 		
		decryptData(file_content, lines_read);
		writeFile(file_content, file_ptr, lines_read);
		closeFile(file_ptr); 	
		printf("\n*** FILE DECRYPTED ***\n");	
	}	
	else{
		printf("\n*** NO CHANGES MADE TO FILE ***\n");		
	}
}

void writeFile(char file_content[][MAX_LENGTH], FILE * file_ptr, int lines_read){
	int row = 0; // line number (0 is the first line)
	int column = 0; // character number (0 is the first character on a line)
	lines_read = lines_read + 1;
	
	for(row = 0 ; row < lines_read ; row++){
		
		column = 0;
		while(file_content[row][column] != '\0'){	
			fprintf(file_ptr, "%c", file_content[row][column]);			
			column++;
		}		
	}
}
