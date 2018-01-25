//
//  Serial Program.c
//  Assignment - Serial Program
//
//  Created by Vindula Jayawardana on 1/24/18.
//  Copyright © 2018 Vindula Jayawardana. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

int main(int argc, const char * argv[]) {
    
    // number of samples to run
    int sample_size = atoi(argv[1]);
    float time_array[ sample_size];
    float pi_value_array[ sample_size];
    
    for (int i = 0; i < sample_size; i++ ) {
        
        int npoints = 100000000;
        int circle_count = 0;
        float upper_bound = 1.0;
    
        //set random seed
        srand((unsigned int)time(NULL));
    
        struct timeval t1, t2;
        double elapsedTime;
    
        // start timer
        gettimeofday(&t1, NULL);
    
        //iterate over 100 M trials
        for(int j = 1; j <= npoints; j++)
        {
            //random x,y coordinates
            float xcoordinate  = ((float)rand()/(float)(RAND_MAX)) * upper_bound;
            float ycoordinate = ((float)rand()/(float)(RAND_MAX)) * upper_bound;
            //check if the cordinated points are within the circle area
            if (pow(xcoordinate,2) + pow(ycoordinate,2) < 1){
                circle_count++;
            }
        }
        //final PI value calculation
        float PI = 4.0 * circle_count/npoints;
    
        // stop timer
        gettimeofday(&t2, NULL);
    
        // compute the elapsed time in millisec
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        
        //add current iteration values to the arrays
        time_array[i] = elapsedTime;
        pi_value_array[i] = PI;

    }
    
    float final_time_taken = 0;
    float final_pi_value = 0;
    //calculate final pi value and time taken
    for (int k = 0; k < sample_size; k++) {
        
        final_pi_value = final_pi_value + pi_value_array[k];
        final_time_taken = final_time_taken + time_array[k];
    }
    
    //print final results
    printf("%s%.6f%s","Time elapsed : ", final_time_taken/sample_size, " ms\n");
    printf("%s%.6f","Value of Pi : ", final_pi_value/sample_size);
    
    putchar('\n');
    return 0;
    
}
