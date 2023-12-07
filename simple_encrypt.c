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
#include <stdbool.h> 

#define MAX_LINES 1024
#define MAX_LENGTH 1024
#define FILENAME_LENGTH 100

void addSignature(char file_content[][MAX_LENGTH], size_t lines_read);
bool checkSignature(char file_content[][MAX_LENGTH], size_t lines_read);
void closeFile(FILE * file_ptr);
void crypt(char file_content[][MAX_LENGTH], size_t lines_read, size_t operation);
void encryptData(char file_content[][MAX_LENGTH], size_t lines_read);
void decryptData(char file_content[][MAX_LENGTH], size_t lines_read);
void getFilename(char* file_name);
FILE* openFile(char* file_name, const char* file_mode);
size_t readFile(char file_content[][MAX_LENGTH], FILE * file_ptr);
void removeSignature(char file_content[][MAX_LENGTH], size_t lines_read);
void showMenu(char file_content[][MAX_LENGTH], FILE * file_ptr, char* file_name, size_t lines_read);
void writeFile(char file_content[][MAX_LENGTH], FILE * file_ptr, size_t lines_read);

int main(void)
{
	FILE *file_ptr = NULL;
	char file_name[FILENAME_LENGTH] = {'\0'};		
	char file_content[MAX_LINES][MAX_LENGTH] = {'\0'};
	size_t lines_read = 0;
		
	puts("\n*** SIMPLE ENCRYPT 2.0 by Reslashd ***\n");
	getFilename(file_name);
	
	if (( file_ptr = openFile(file_name, "r")) != NULL){
		puts("\n*** FILE OPENED ***");						
		// Read file contents and store in file_content
		lines_read = readFile(file_content, file_ptr);
		closeFile(file_ptr);		
		
		// Stop further program execution if the line limit is exceeded
		if ( lines_read >= MAX_LINES ){
			printf("File exceeds max allowed lines (%d)\n", MAX_LINES);
		}
		else{		
			showMenu(file_content, file_ptr, file_name, lines_read);
		}		
	} else{
		puts("\n*** ERROR OPENING FILE ***");	
	}	
	
	puts("\n*** EXITING PROGRAM ***\n");
	return EXIT_SUCCESS;
}

void addSignature(char file_content[][MAX_LENGTH], size_t lines_read){
	strcpy(file_content[lines_read], "S_ENCRYPT23");
}

bool checkSignature(char file_content[][MAX_LENGTH], size_t lines_read){
	// return result of comparison 
	return strcmp(file_content[lines_read - 1], "S_ENCRYPT23") == 0;
}

void closeFile(FILE * file_ptr)
{	
	// NULL means no file is open so nothing to close
	if ( file_ptr != NULL ){
		fclose(file_ptr);
	}
}

void crypt(char file_content[][MAX_LENGTH], size_t lines_read, size_t operation) {
   size_t row = 0; // line number (0 is the first line)
   size_t column = 0; // character number (0 is the first character on a line)

   for (row = 0; row < lines_read; row++) {
        column = 0;
        while (file_content[row][column] != '\0') {
            if (file_content[row][column] != '\n') {
				// if operation = 1 then encrypt (char + 1) else decrypt (char -1)
                file_content[row][column] = (operation == 1) ? file_content[row][column] + 1 : file_content[row][column] - 1;
                column++;
            } else {
                column++;
            }
        }
    }
}

void encryptData(char file_content[][MAX_LENGTH], size_t lines_read){
	crypt(file_content, lines_read, 1);
	addSignature(file_content, lines_read);
}

void decryptData(char file_content[][MAX_LENGTH], size_t lines_read){
	crypt(file_content, lines_read, 0);
	removeSignature(file_content, lines_read);
}

void getFilename(char* file_name){	
	fputs("Please enter the name of the file you want encrypt/decrypt  > ", stdout);
	fgets(file_name, FILENAME_LENGTH, stdin);	
		if( file_name != NULL ){
		// remove newline character from file_name string
		file_name[strcspn(file_name, "\n")] = 0;	
		printf("\nAttempting to open file %s ...\n", file_name);
		}
		else{
		puts("\nError reading input");
		}		
}

FILE* openFile(char* file_name, const char* file_mode)
{
	return fopen(file_name, file_mode);	
}

size_t readFile(char file_content[][MAX_LENGTH], FILE * file_ptr){
	size_t lines_read = 0;
	
	// While the end of the file is not reached yet AND there are no errors reading the file
	while (!feof(file_ptr) && !ferror(file_ptr)){
		if (fgets(file_content[lines_read], MAX_LENGTH, file_ptr) != NULL){
			//stop reading if lines_read is equal to or more than MAX_LINES
			if( lines_read >= MAX_LINES ){
				printf("Maximum line limit reached, stop reading file.\n");
			}
			else{
			lines_read++;	
			}
		}
	 }
	return lines_read;
}

void removeSignature(char file_content[][MAX_LENGTH], size_t lines_read){
	strcpy(file_content[lines_read-1], "\0");
}

void showMenu(char file_content[][MAX_LENGTH], FILE * file_ptr, char* file_name, size_t lines_read){
	int choice = 0;	
	bool has_signature = 0;
	
	// Set signature flag 	
	has_signature = checkSignature(file_content, lines_read);
	
	// Show menu options
	puts("\n********* MENU *********\n");
	if( has_signature == 0) {
		puts("1 = Encrypt File");	
	}else{
		puts("1 = Decrypt File");	
	}
	puts("2 = Close file and exit\n");
	puts("************************");
	choice = getchar();
	
	if( has_signature == 0 && choice == '1'){
		puts("\nEncrypting file...");
		encryptData(file_content, lines_read);	
		//Open file for writing (erasing all contents!)
		file_ptr = openFile(file_name, "w"); 	
		writeFile(file_content, file_ptr, lines_read);
		closeFile(file_ptr); 		
		puts("\n*** FILE ENCRYPTED ***");	
	} else if(has_signature == 1 && choice == '1'){
		puts("\nDecrypting file...");		
		decryptData(file_content, lines_read);
		//Open file for writing (erasing all contents!)
		file_ptr = openFile(file_name, "w"); 
		writeFile(file_content, file_ptr, lines_read);
		closeFile(file_ptr); 	
		puts("\n*** FILE DECRYPTED ***");	
	}	
	else{
		puts("\n*** NO CHANGES MADE TO FILE ***");		
	}	
}

void writeFile(char file_content[][MAX_LENGTH], FILE * file_ptr, size_t lines_read){
	size_t line_number = 0; // line number (0 is the first line)
	
	for (line_number = 0 ; line_number <= lines_read ; line_number ++){
		fputs(file_content[line_number], file_ptr);	
	}
}
