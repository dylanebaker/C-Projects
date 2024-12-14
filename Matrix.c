/*  
*   Dylan Baker
*   C0484294
*   COMP-166-001
*   Oct 20, 2024
*
*                       Function: Matrix.c
*           Computes and prints a user-defined matrix. 
*           User chooses matrix shape, size, and design.
*
*                           #               Shape: Pyramid
*                          ###              Size: 3
*                         #####             Symbol: #
*
*                    Ctrl-C exits program loop
*/

#include <string.h>
#include <stdio.h>

// Defines constant strings for matrix shapes
const char *STAIRCASE = "staircase";
const char *PYRAMID = "pyramid";
const char *DIAMOND = "diamond";

int main() {
    // Input variables
    int matrix_size;
    char matrix_symbol[2]; 
    char matrix_shape[15];

    // Counting loop variables
    int i, j; 

    // Infinite loop checking for valid input
    for(;;){
        for(;;){
            // Prompt/receive user input for matrix size, symbol and shape
            printf(
                "\nMatrix Builder: When prompted, the matrix shape options"
                " you may choose from are:\n- staircase\n- pyramid\n- diamond\n"
            );
            printf(
                "Please enter a positive integer value, a single character symbol," 
                " and a shape, seperated by spaces.\n"
            ); 

            scanf("%d %s %15s", &matrix_size, matrix_symbol, matrix_shape);      
            printf("\n");

                // Check user input for positive matrix size, single character symbol
                // and that shape is a pre-defined option
                if(matrix_size > 0 && 
                   matrix_symbol[1] == '\0' && 
                   (strcmp(matrix_shape, STAIRCASE) == 0 || 
                    strcmp(matrix_shape, PYRAMID) == 0 ||
                    strcmp(matrix_shape, DIAMOND) == 0)) {
                break;
                } else {
                    puts("Please ensure these input criteria are met: \n"
                        "- Matrix Shape is 'staircase', 'pyramid', or 'diamond'.\n"
                        "- Matrix Size is a positive integer.\n"
                        "- Matrix Symbol is one character."); 
                }
        }

        // If loop to print pyramid matrix
        if(strcmp(matrix_shape, PYRAMID) == 0){ 
            for(i = 1; i <= matrix_size; i++){ 
                // Prints leading spaces to center symbols
                for(j = matrix_size; j > i; j--){ 
                    printf(" ");                
                }
                // Print symbols for the current row
                for(j = 1; j <=(2 * i - 1); j++){ 
                    printf("%c", matrix_symbol[0]); 
                }          
                printf("\n");
            }
        }
        // If loop to print staircase matrix
        else if(strcmp(matrix_shape, STAIRCASE) == 0){
            for(i = 1; i <= matrix_size; i++){
                // Print symbols for the current row
                for(j = 1; j <= i; j++){
                    printf("%c", matrix_symbol[0]);
                }
                printf("\n");
            }
        }
        // If loop to print diamond matrix
        else if(strcmp(matrix_shape, DIAMOND) == 0){
            // First loop to print top half of the diamond matrix
            for(i = 1; i <= matrix_size; i++){
                // Prints leading spaces to center symbols
                for(j = matrix_size; j > i; j--){
                    printf(" ");
                }
                // Print symbols for the current row
                for(j = 1; j <= (2 * i - 1); j++){
                    printf("%c", matrix_symbol[0]);
                }             
                printf("\n");
            }
            // Second loop to print the bottom half of the diamond matrix
            for(i = matrix_size - 1; i >= 1; i--){
                // Prints leading spaces to center symbols
                for(j = matrix_size; j > i; j--){
                    printf(" ");
                }
                // Print symbols for the current row
                for(j = 1; j <= (2 * i - 1); j++){
                    printf("%c", matrix_symbol[0]);
                }
                printf("\n");
            }
        }   
    }
} // End of main