/**
 * @file fileEcho.c
 * @author Dylan Baker
 * 
 * @brief File Echo
 * This program takes input from a file, or a list of files, and prints 
 * the data from each file, and each file named in a list of files,
 * to standard output.
 * 
 * @version 0.1
 * @date 2024-11-23
 * @copyright Copyright (c) 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255 // Define buffer size for reading lines

FILE *openFile(char *fileName);
void printFileContent(FILE *inputFile);
void printFileFromList(FILE *inputFile);

/**
 * @brief Main
 * 
 * Main processes command line arguments. If no files are specified, the user
 * receives a usage example. It prints the contents of individual files, or 
 * prints the content of files contained within a list of files.
 * 
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Exit status code; 0 indicates success; 1 indictates an error.
 */
int main (int argc, char *argv[]) {
    // Check if no arguments were provided (other than program name).
    if(argc == 1){
        printf("Usage: ./a.exe [-a file__list.txt] [file__.txt] [file__.txt]");
    }
    
    // Loop to read files
    for(int i = 1; i < argc; i++){
        // If argument is "-a", handle the file list case
        if(strcmp(argv[i], "-a") == 0){
            if(i + 1 >= argc) {
                // Check for a file name after list call
                fprintf(stderr, "Error: Missing file list name after -a.\n");
                continue;
            }
            
            // Retrieve file list
            char *fileListName = argv[++i];
            FILE *inputFile = openFile(fileListName);
           
            if(!inputFile){
                fprintf(stderr, "Error: Could not open file list '%s'.\n", fileListName);
                continue;
            }

            printFileFromList(inputFile);
            fclose(inputFile);
        } else {

        // If the argument is not a list call, retrieve file name
        char *fileName = argv[i];
        FILE *inputFile = openFile(fileName);
        
        if(!inputFile){
            fprintf(stderr, "Error: Could not open file '%s'.\n", fileName);
            continue;
        }
        
        printFileContent(inputFile);
        fclose(inputFile);
        }    
    }
}

/**
 * @brief Opens a file for reading.
 * 
 * This function attempts to open a file specified by name.
 * It returns a file pointer if successful, or NULL if there is an error.
 * 
 * @param fileName Name of the file to open.
 * @return FILE* A pointer to the opened file, or NULL.
 */
FILE *openFile(char *fileName) {
    FILE* file = fopen(fileName, "r");
    return file;
}

/**
 * @brief Prints the contents of a file.
 * 
 * This function reads the contents of a file line by line,
 * printing each line to standard output.
 * 
 * @param inputFile A pointer to the file to read from.
 */
void printFileContent(FILE *inputFile){
    // Buffer to hold each line of the file
    char buffer[SIZE];
    
    // Read and print each line of the file
    while(fgets(buffer, SIZE, inputFile) != NULL) {
        printf("%s", buffer);
    }
    if(ferror(inputFile)) {
        fprintf(stderr, "Error reading from file.\n");
    }
    printf("\n");
}

/**
 * @brief Prints the contents of files listed in a file.
 * 
 * This functions reads each line of a file list, where each line contains the
 * name of a file to open and print. 
 * 
 * @param inputFile A pointer to the file that contains the list of filenames.
 */
void printFileFromList(FILE *inputFile) {
    // Buffer to hold each line from the file list
    char fileName[SIZE];

    while(fgets(fileName, SIZE, inputFile) != NULL){
        // Remove newline from the filename
        fileName[strcspn(fileName, "\n")] = 0;

        FILE *fileToPrint = openFile(fileName);
        
        if(!fileToPrint) {
            fprintf(stderr, "Error: Could not open file '%s' from file list.\n", fileName);
            continue;
        }
        printFileContent(fileToPrint);
        fclose(fileToPrint);
        }
    }