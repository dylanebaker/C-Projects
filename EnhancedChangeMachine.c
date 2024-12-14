/**
 * @file EnhancedChangeMachine.c
 * @author Dylan Baker
 * 
 * @brief Enhanced Change Machine
 * This program allows the user to input an amount of money in dollars
 * and receive the equivalent amount in change using the available 
 * inventory of bills and coins. The user can specify the inventory 
 * amounts for each denomination, and the program will attempt to 
 * dispense the required change while updating the inventory.
 * It includes functions for converting inputs, managing inventory, 
 * calculating the change to be given, and printing the results.
 * This program continues to run until the user enters an amount that 
 * cannot be fully dispensed, after which it displays the remaining 
 * inventory.
 * 
 * @version 0.1
 * @date 2024-11-04
 * @copyright Copyright (c) 2024
 */

#include <stdio.h>

// Declaration of functions to be used in main

int inputToCents();
void getInventory(int inventory[]);
int centsToBills(int cents, int arr[], int inventory[]);
void printChange(int arr[]);
void printInventory(int inventory[]);

int main(){
    int arr[6] = {0};
    int inventory[6] = {0};

    getInventory(inventory);

    for(;;){
        int cents = inputToCents();

        if(cents == -1){
            continue;
        }
        int leftOver = centsToBills(cents, arr, inventory);
            if(leftOver > 0){
                printf("Unable to fulfill that amount. " 
                "You are owed: $%.2f\n", leftOver / 100.0);
                break;
            }
        printInventory(inventory);
    }
    return 0;
} // End of main

/**
 * @brief Converts user input from dollars to cents.
 * This function prompts the user to enter an amount of money in dollars
 * and converts it into cents. If the user enters a negative amount,
 * it prompts an error message and returns -1.
 * @return int 
 */

int inputToCents() {
    double remainder;

    printf("\nAmount to exchange: $");
        scanf("%lf", &remainder);
        
        if (remainder < 0) {
            printf("Please enter a positive amount.\n");
            return -1;
        } else {
            return (int)(remainder * 100);
        }
} 

/**
 * @brief Prompts the user to enter amounts for the inventory of change
 * This function asks the user for the amount of various denominations 
 * of change to be available in the inventory.
 * @param inventory 
 */

void getInventory(int inventory[]){
    printf("How much change is in the inventory?\n");
    printf("$10 bills: ");
        scanf("%d", &inventory[1]);
    printf("Toonies: ");
        scanf("%d", &inventory[2]);
    printf("Quarters: ");
        scanf("%d", &inventory[3]);
    printf("Dimes: ");
        scanf("%d", &inventory[4]);
    printf("Pennies: ");
        scanf("%d", &inventory[5]);
} 

/**
 * @brief Calculates the change to be given in bills and coins.
 * This function takes an amount in cents and attempts to dispense change
 * using the available inventory. It updates the inventory and returns 
 * any leftover cents that could not be dispensed.
 * @param cents 
 * @param arr 
 * @param inventory 
 * @return int 
 */

int centsToBills (int cents, int arr[], int inventory[]){
    int values[] = {1000, 200, 25, 10, 1};
    
    for(int i = 0; i < 5; i++){
        
        int denomination = values[i];
        int required = cents / denomination;

        if (required <= inventory[i+1]){
            arr[i + 1] = required;
            inventory[i + 1] -= required;
            cents -= required * denomination;
        } 
        else {
            arr[i + 1] = inventory[i + 1];
            cents -= inventory[i + 1] * denomination;
            inventory[i + 1] = 0;
        }
    }
    printChange(arr);
    return cents;
} 

/**
 * @brief Prints the change that has been given to the user.
 * This function displays the quantity of each denomination of change 
 * that the user will receive.
 * @param arr 
 */

void printChange(int arr[]){
    printf("\nHere is your change: \n");
    printf("-------------------------\n");
    printf("$10 Bills: %d\n", arr[1]);
    printf("Toonies: %d\n", arr[2]);
    printf("Quarters: %d\n", arr[3]);
    printf("Dimes: %d\n", arr[4]);
    printf("Pennies: %d\n", arr[5]);
    printf("-------------------------\n");
} 

/**
 * @brief Prints the current inventory of change remaining.
 * This function displays the quantities of each type of change that 
 * remain in the inventory after dispensing.
 * @param inventory 
 */

void printInventory(int inventory[]){
    printf("Left in inventory: \n");
    printf("-----------------------\n");
    printf("$10 Bills: %d\n", inventory[1]);
    printf("Toonies: %d\n", inventory[2]);
    printf("Quarters: %d\n", inventory[3]);
    printf("Dimes: %d\n", inventory[4]);
    printf("Pennies: %d\n", inventory[5]);
    printf("-----------------------\n");
}