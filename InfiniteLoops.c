/*  
*   Dylan Baker
*   C0484294
*   COMP-166-001
*   Oct 20, 2024
*
*                     Function: InfiniteLoops.c
*           Computes and prints 5-year return on investment
*           values based on user-inputted initial investment ($____).
*
*                  Annual interest rate: 2.50%
*                Interest compounded 12 times yearly
*                    Initial Investment: $1200
*                        year 1:  $1230.36
*                        year 2:  $1261.47
*                        year 3:  $1293.37
*                        year 4:  $1326.08
*                        year 5:  $1359.61
*
*                     Ctrl-C exits program loop
*/

#include <stdio.h>
#include <math.h>

#define COMPOUND_PER_YEAR 12
#define INTEREST_RATE 0.025
#define DISPLAY_YEARS 5
#define CENTS 100

int main() {

    // Moved variables not needed in for loop
    double realRate;                 
    long balance[DISPLAY_YEARS + 1];
    realRate = 1 + INTEREST_RATE / COMPOUND_PER_YEAR;
    int year;

for(;;){
    for(;;){
        printf("\nAnnual interest rate: %.2f%%\n", INTEREST_RATE * 100);
        printf("Interest compounded %d times yearly\n", COMPOUND_PER_YEAR);
        printf("Initial Investment: $");
            scanf("%ld", balance);
            balance[0] *= CENTS;
        
        // Check if user input is greater than 0
        if (balance[0] > 0) {           
            break;                
        } else {
            puts("Invalid Input (must be greater than 0)\n"); 
        }
    }
    for (year = 1; year <= DISPLAY_YEARS; year++) {
        balance[year] = balance[year - 1];
        for (int period = 0; period < COMPOUND_PER_YEAR; period++) {
            balance[year] = lround(balance[year] * realRate);
        }
    }
    for (year = 1; year <= DISPLAY_YEARS; year++) {
        printf(
            "year %d:  $%ld.%02ld\n",
            year,
            balance[year] / CENTS,
            balance[year] % CENTS
        );
    }
    }
} // End of main