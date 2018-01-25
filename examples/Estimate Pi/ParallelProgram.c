//
//  Parallel Program.c
//  Assignment - Parallel Program
//
//  Created by Vindula Jayawardana on 1/24/18.
//  Copyright © 2018 Vindula Jayawardana. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#define NPOINTS 100000000

//struct for argument parsing for threads
struct thread_data{
    long thread_id;
    long thread_count;
};

//Function to calculate number of circle hits
int *CalculateCount(void *thread_obj)
{
    struct thread_data *thread_object;
    thread_object = (struct thread_data *) thread_obj;

    long tid = (long)thread_object->thread_id;
    long tcount = (long)thread_object->thread_count;

    int circle_count = 0;
    float upper_bound = 1.0;
    
    //iterate over given number of trials
    for(int j = tid * (NPOINTS/tcount) ; j < (tid + 1)*(NPOINTS/tcount) ; j++)
    {
        //random x,y coordinates
        float xcoordinate  = ((float)rand()/(float)(RAND_MAX)) * upper_bound;
        float ycoordinate = ((float)rand()/(float)(RAND_MAX)) * upper_bound;
        //check if the cordinated points are within the circle area
        if (pow(xcoordinate,2) + pow(ycoordinate,2) < 1){
            circle_count++;
        }
    }
    return circle_count;
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    
    //number of threads
    int thread_size = atoi(argv[1]);
    
    //number of samples to run
    int sample_size = atoi(argv[2]);
    
    float time_array[ sample_size];
    float pi_value_array[ sample_size];
    
    for (int i = 0; i < sample_size; i++ ){
    
        long global_count = 0;
    
        pthread_t threads[thread_size];
        int rc;
        pthread_attr_t attr;
        void *status;
    
        //set random seed
        srand((unsigned int)time(NULL));
        struct timeval t1, t2;
        // start timer
        gettimeofday(&t1, NULL);
    
        // Initialize and set thread detached attribute
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
        for(long t = 0; t < thread_size; t++){

            struct thread_data thread_obj;
            thread_obj.thread_id = t;
            thread_obj.thread_count = thread_size;
        
            rc = pthread_create(&threads[t], NULL, CalculateCount, (void *)&thread_obj);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
    
        // Free attribute and wait for the other threads
        pthread_attr_destroy(&attr);
    
        for(long t = 0; t < thread_size; t++) {
            rc = pthread_join(threads[t], &status);
            if (rc) {
                printf("ERROR; return code from pthread_join() is %d\n", rc);
                exit(-1);
            }
            global_count = global_count + (long)status;
        }
    
        //final PI value calculation by main
        float PI = 4.0 * global_count/NPOINTS;
               
        // stop timer
        gettimeofday(&t2, NULL);
    
        // compute the elapsed time in millisec
        float elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
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
    printf("%s%d%s","Number of threads : ", thread_size, " \n");
    printf("%s%.6f%s","Time elapsed : ", final_time_taken/sample_size, " ms\n");
    printf("%s%.6f","Value of Pi : ", final_pi_value/sample_size);
    
    putchar('\n');
    pthread_exit(NULL);
    
}
