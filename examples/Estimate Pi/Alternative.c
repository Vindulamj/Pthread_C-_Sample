//
//  Alternative.c
//  Assignment - Alternative Program
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
    float pi_value_array[ sample_size];
    
    for (int i = 0; i < sample_size; i++ ) {
        
        //initial value setup
        float a = 1;
        float b = 1/sqrt(2);
        float t = 0.25;
        float p = 1;
        
        float temp_a = 0;
        float temp_b = 0;
        float temp_t = 0;
        float temp_p = 0;
        int count  = 0;
        
        while(fabsf(a-b) > 0.00001) {
            temp_a = a;
            temp_b = b;
            temp_t = t;
            temp_p = p;
            a = (temp_a + temp_b)/2;
            b = sqrt(temp_a*temp_b);
            t = temp_t - temp_p*pow(temp_a-a,2);
            p = 2*temp_p;
            count++;

        }
        float PI = pow(a+b,2)/(4*t);
        
        pi_value_array[i] = PI;

    }
    
    float final_pi_value = 0;
    
    //calculate final pi value and time taken
    for (int k = 0; k < sample_size; k++) {
        final_pi_value = final_pi_value + pi_value_array[k];
    }
    
    //print final results
    printf("%s%.16f","Value of Pi : ", final_pi_value/sample_size);
    
    putchar('\n');
    return 0;
    
}
