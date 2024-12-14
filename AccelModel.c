/*  
*   Dylan Baker
*   C0484294
*   COMP-166-001
*   Oct 24, 2024
*
*                       Function: AccelModel.c
*           Computes and prints the height and velocity of
*                a falling bowling ball over a set time.
*
*                Time (s)    Height (m)    Velocity (m/s)
*                  0.000          2.00        0.00
*                  0.100          1.95        0.98
*                  0.200          1.81        1.93    
*
*                    Ctrl-C exits program loop
*/

#include <stdio.h>

#define DENSITY 1.204         // Air density in kg/m^3
#define GRAVITY 9.806         // Gravitational accel. in m/s^2
#define CROSSAREA 0.0366      // ross-sectional area in m^2
#define DRAG 0.5              // Drag coefficient
#define MAX_ITERATIONS 2000   // Max # of iterations program is allowed to run
#define HALF 0.5              // For division by two

int main() {
    // Input variables
    double mass;
    double initial_height, time_interval;

    // Calculation variables
    int loop_count = 0;
    double velocity = 0;
    double time_initial = 0;
    double height, accel, drag_force; 

    // Prompt/receive user input for calculations
    printf("Please input initial height (m), mass (kg) and a time interval (s)\n"
           "to calculate the height and velocity of the falling object.\n");
    
    // Check for valid user input, loop until input is valid
    do {
        printf("\nInitial Height (in meters): ");
            scanf("%lf", &initial_height);

        printf("Mass (in kilograms): ");
            scanf("%lf", &mass);

        printf("Time Interval (in seconds): ");
            scanf("%lf", &time_interval);

        if(initial_height <= 0 || mass <= 0 || time_interval <= 0){
            printf("Invalid input. Please follow all parameters.");
        }
    } while (initial_height <= 0 || mass <= 0 || time_interval <= 0);

    // Set height calculation variable equal to users inputted height
    height = initial_height;

    // Print calculation constants
    printf("\nGravitational Acceleration: %.3f m/s^2\n", GRAVITY);
    printf("Atmospheric Density: %.3f kg/m^3\n", DENSITY);
    printf("Drag Coefficient: %.1f\n", DRAG);
    printf("Mass: %.2f kg\n", mass);
    printf("Initial Height: %.2f m\n", initial_height);

    // Print header for output table
    printf("\nTime (s)    Height (m)    Velocity (m/s)\n");
    printf("%8.2f     %8.2f     %12.2f\n", time_initial, height, velocity);

    // Loop to calculate height and velocity over time
    do {
        // Calculates drag force and acceleration based on current velociy
        drag_force = HALF * (CROSSAREA * DENSITY * DRAG * velocity * velocity);
        accel = (mass * GRAVITY - drag_force) / mass;

        // Updates velocity and height using acceleration from above equation
        velocity += accel * time_interval;
        height -= (velocity * time_interval) + 
                  HALF * (accel * time_interval * time_interval);

        // Sets velocity to 0 if height is 0 (object hit the ground)
        if(height <= 0){
            height = 0;
            velocity = 0;
        }

        // Update time interval and loop count
        time_initial += time_interval;
        loop_count ++;

        printf("%8.2f     %8.2f     %12.2f\n", time_initial, height, velocity);
    
    // Calculation loop continues aslong as height is greater than 0 and
    // the loop has not reached max iterations
    } while (height > 0 && loop_count < MAX_ITERATIONS);

    printf("\nThe object has hit the ground.\n");
} // End of main