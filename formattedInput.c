/**
 * @file formattedInput.c
 * @author Dylan Baker
 * 
 * @brief Formatted Input
 * This program takes input of sensor data readings from a file or from standard
 * input, and analyzes the data to calculate the mean, standard deviation, as well
 * as at what time the maximum and minimum data readings were logged.
 * 
 * @version 0.1
 * @date 2024-11-23
 * @copyright Copyright (c) 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 500
#define MAX_SENSORS 100
#define BUFFER 50

// Declaration of functions 
FILE *openFile(char *fileName);
FILE *writeFile(char *fileName);
int isValidSensorReading(char *str);
void readSensorData(FILE *inputFile, FILE *outputFile);
void calcSensorStats(float readings[], int count, float *mean, float *std_dev);
void printData(FILE *outputFile, float maxReading, char *maxTimestamp, float minReading, 
               char *minTimestamp, int sensorCount, float *means, float *std_devs);

/**
 * @brief main
 * 
 * This function determines whether data will be read 
 * from standard input or a file based on command-line arguments.
 * It processes the sensor data, calculates statistics, 
 * and writes the results to the specified output.
 * The program terminates if there are errors in file reading, writing, 
 * or if the data format is incorrect.
 * 
 * @param argc The number of command line arguments passed to the program.
 * @param argv Array of command line arguments.
 * @return int Exit status code; 0 indicates success; 1 indictates an error.
 */
int main (int argc, char *argv[]) {
    //Default to reading and writing from stdio.
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    FILE *file;

    char buffer[MAX_SENSORS];
    // If loop checks whether to read from stdin or from a file.
    if(argc == 1){
        char *fileName = "dataInput.txt";

        file = writeFile(fileName);
        printf("Reading sensor data from STDIN. Please enter data, line by line." 
               "Type 'exit' to stop.\n");
            while (1) {
                printf("> ");
                fgets(buffer, sizeof(buffer), stdin);

                if(strncmp(buffer, "exit", 4) == 0 && (buffer[4] == '\n' || buffer[4] == '\0')) {
                    break;
                }
            fputs(buffer, file);
            }
    fclose(file);
    
    inputFile = openFile(fileName);
    outputFile = stdout;
    readSensorData(inputFile, outputFile);
    fclose(inputFile);
    }

    // Given one argument, read from a file and output to stdout.
    else if(argc == 2){
        inputFile = openFile(argv[1]);
        if(!inputFile) {
            exit(1);
        }
    outputFile = stdout;
    readSensorData(inputFile, outputFile);
    fclose(inputFile);
    }
    
    // Given two arguments, read from a file and output to a file.
    else if(argc == 3) {
        inputFile = openFile(argv[1]);
        if(!inputFile) {
            exit(1);
        }
        outputFile = writeFile(argv[2]);
        if(!outputFile) {
            fclose(inputFile);
            exit(1);
        }
    readSensorData(inputFile, outputFile);
    fclose(inputFile);
    fclose(outputFile);

    } else {

    // Terminate program if more than two arguments are given
        fprintf(stderr, "Warning: Terminating program (too many arguments).\n");
        exit(1);
    }

    //Close files if they were opened.
    if (inputFile != stdin) {
        fclose(inputFile);
    }
    if(outputFile != stdout) {
        fclose(outputFile);
    }
}

/**
 * @brief Opens a file for reading.
 * 
 * This function attempts to open a file for reading 
 * and checks if the file is successfully opened.
 * If the file cannot be opened, an error message is displayed, 
 * and the program terminates.
 * 
 * @param fileName The name of the file to be opened
 * @return FILE* A pointer to the opened file, or NULL if the file cannot be opened.
 */
FILE *openFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if(file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'. Please check file path.\n", fileName);
        exit(1);
    }
    return file;
}

/**
 * @brief Opens a file for writing.
 * 
 * This function attempts to open a file for writing. If the file cannot be opened, 
 * an error message is displayed, and the program terminates.
 * 
 * @param fileName The name of the file to be opened.
 * @return FILE*  A pointer to the opened file, or NULL if the file cannot be opened.
 */
FILE *writeFile(char *fileName) {
    FILE *file = fopen(fileName, "w");
    if(file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'. Please check file path.\n", fileName);
        exit(1);
    }
    return file;
}

/**
 * @brief Reads sensor data from the input file, calculates statistics and
 * writes the results to the output file.
 * 
 * This function reads sensor data line by line from the input file, processes the data, and computes the maximum,
 * minimum, mean, and standard deviation for each sensor. The results are written to the output file.
 * 
 * @param inputFile The file from which sensor data is read.
 * @param outputFile The file where the processed data is written.
 */
void readSensorData(FILE *inputFile, FILE *outputFile) {
        char timeStamp[BUFFER];             // Stores the timestamp of each reading
        char line[MAX_LINE_LENGTH];         // Buffer for reading each line of data
        float readings[MAX_SENSORS];        // Array to store sensor readings
        int numSensors = 0;                 // Track number of sensors in a line
        float maxReading = -INFINITY;       // Track maximum sensor reading
        float minReading = INFINITY;        // Track minimum sensor reading
        char maxTimestamp[BUFFER] = " ";    // Timestamp of maximum reading
        char minTimestamp[BUFFER] = " ";    // Timestamp of minimum reading
        float allReadings[MAX_SENSORS][MAX_LINE_LENGTH]; // Array to store all sensor readings
        int totalReadings = 0;              // Total number of readings processed
        int expectedSensorCount = -1;       // Track expected number of sensors in a line

    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {

        // Skip empty or whitespace-only lines
        if (line[0] == '\0' || strspn(line, " \t\n\r") == strlen(line)) {
            continue;
        }

        numSensors = 0;
        
        // Extract timestamp from first token in line
        char *token = strtok(line, " \t");
        if (token) {
            strcpy(timeStamp, token); // Store timestamp
        } else {
            fprintf(stderr, "Warning: Terminating program (improperly formatted data)");
            exit(1);
        }

        token = strtok(NULL, " \t");      
        
        // Process sensor readings in a line
        while(token != NULL) {
            if(numSensors >= MAX_SENSORS) { // Check for consistent amount of sensor readings
                fprintf(stderr, "Warning: Terminating program (inconsistent sensor readings).");
                exit(1);
            }

            // Check each sensor reading for validity
            if (!isValidSensorReading(token)) {
                fprintf(stderr, "Warning: Terminating program (Invalid sensor reading '%s').\n", token);
                exit(1);
            }

            // Convert string toke to float
            readings[numSensors] = atof(token);

            // Store for later calculations
            allReadings[numSensors][totalReadings] = readings[numSensors];

            if(readings[numSensors] > maxReading) {
                maxReading = readings[numSensors];
                strcpy(maxTimestamp, timeStamp);
            }
            if(readings[numSensors] < minReading) {
                minReading = readings[numSensors];
                strcpy(minTimestamp, timeStamp);
            }
            
            numSensors++;                 // Move to next sensor reading
            token = strtok(NULL, " \t");  // Gets next sensors data
        }

        // Check for consistent sensor readings
        if(expectedSensorCount == -1) {
            expectedSensorCount = numSensors;
        } else if (numSensors != expectedSensorCount) {
            fprintf(stderr, "Warning: Terminating program (inconsistent sensor readings).");
            exit(1);
        }
        totalReadings++; // Increment number of readings processed
    }

    if (totalReadings == 0) {
        fprintf(stderr, "Warning: Terminating program (no sensor data to process).\n");
        exit(1);
    }

    // Arrays to store mean and standard deviation for each sensor
    float means[MAX_SENSORS];
    float std_devs[MAX_SENSORS];

    // Send stats from each sensor calculation function
    for (int i = 0; i < numSensors; i++) {
        calcSensorStats(allReadings[i], totalReadings, &means[i], &std_devs[i]);
    }

    // Send required stats to print function
    printData(outputFile, maxReading, maxTimestamp, minReading, minTimestamp,
              numSensors, means, std_devs);
}

/**
 * @brief Validates if a string represents a valid sensor reading.
 * 
 * This function checks if the input string is a valid sensor reading, defined as:
 * (May contain one '.', one 'e' or one 'E', one '+' or one '-')
 * 
 * @param str The input string representing the sensor reading to be validated.
 * @return int Returns 1 if the string is a valid sensor reading, otherwise returns 0.
 */
int isValidSensorReading(char *str) {
    int hasDigit = 0;
    int hasEe = 0;
    int hasDot = 0;
    int i = 0;

    if (str[i] == '+' || str[i] == '-'){
        i++;
    }

    while(str[i] != '\0' && str[i] != '\n') {
        if(isdigit(str[i])) {
            hasDigit = 1;
        } 
        
        else if (str[i] == '.') {
            if (hasDot || hasEe) {
                return 0;
            }
            hasDot = 1;
        }

        else if (str[i] == 'e' || str[i] == 'E') {
            if (hasEe || !hasDigit) {
                return 0;
            }
            hasEe = 1;
            i++;

            if (str[i] == '+' || str[i] == '-'){
                i++;
            }
        if(!isdigit(str[i])) {
            return 0;
        }
        }
        
        else {
            return 0;
        }

    i++;
    }

return hasDigit;
}

/**
 * @brief Calculates the mean and standard deviation for a set of sensor readings.
 * 
 * This function calculates the mean and standard deviation of the given sensor readings. 
 * If the count is 0, both the mean and standard deviation are set to 0. 
 * If there is only one reading, the standard deviation is also set to 0.
 * 
 * @param readings Array of sensor readings.
 * @param count The number of sensor readings.
 * @param mean A pointer to the variable where the mean will be stored
 * @param std_dev A pointer to the variable where the standard deviation will be stored.
 */
void calcSensorStats(float readings[], int count, float *mean, float *std_dev) {
    float sum_x = 0.0;  // Sum of all sensor readings
    float sum_x2 = 0.0; // Sum of squared sensor readings

    // If there are no readings, mean and std dev are 0
    if (count == 0) {
        *mean = 0.0;
        *std_dev = 0.0;
        return;
    }

    // Calculate sum and sum of squares for the readings
    for(int i = 0; i < count; i++) {
        sum_x += readings[i];
        sum_x2 += readings[i] * readings[i];
    }

    *mean = sum_x / count;

    // For more than one reading, calculate the standard deviaton
    if (count > 1) {
        float variance = (count * sum_x2 - sum_x * sum_x) / (count * (count - 1));
        *std_dev = sqrt(variance);
    } else {
        *std_dev = 0.0; // std dev is 0 for one reading
    }
}

/**
 * @brief Prints the analyzed sensor data to the output file
 * 
 * This function prints the maximum and minimum readings along with there respective
 * time stamps, followed by the mean and standard deviation for each sensor.
 * 
 * @param outputFile The file where the data will be printed.
 * @param maxReading The maximum sensor reading.
 * @param maxTimestamp The timestamp where the maximum reading occured.
 * @param minReading The minimum sensor reading.
 * @param minTimestamp The timestamp where the minimum reading occured.
 * @param sensorCount The total number of sensors.
 * @param means Array containing the mean value for each sensor.
 * @param std_devs Array containing the standard deviation for each sensor.
 */
void printData(FILE *outputFile, float maxReading, char *maxTimestamp, float minReading, 
               char *minTimestamp, int sensorCount, float *means, float *std_devs) {

    // Print the max and min readings with respective timestamps to output file            
    fprintf(outputFile, "Maximum recorded at %s (%g)\n", maxTimestamp, maxReading);
    fprintf(outputFile, "Minimum recorded at %s (%g)\n\n", minTimestamp, minReading);

    // Print the mean and standard deviation of each sensor to output file
    for (int i = 0; i < sensorCount; i++) {
        fprintf(outputFile, "Sensor %d:\n", i + 1);
        fprintf(outputFile, "  - mean: %.2f\n", means[i]);
        fprintf(outputFile, "  - deviation: %.2f\n", std_devs[i]);
    }
}