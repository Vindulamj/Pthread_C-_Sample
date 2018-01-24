#include "stdafx.h"
#include <pthread.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "LinkedList.h"
#include <time.h>    
#include <iostream>
using namespace std;

//fucntion initialization.
int RandomValueGenerator(struct list_node_s* head_p);
int CheckForUniqueness(struct list_node_s* head_p, int value);
void PerformOperationInSerial();
void *PerformOperationWithMutex(void* thread);
void *PerformOperationWithReadWriteLock(void* thread);
void PerformSerialProgram(int count);
void PerformWithMutex();
void PerformWithReadWriteLock();
void ShuffleArray();
void AddTimeToHolder(float time, int thrd_count, int selectType, int count);
float calculateMean(float data[], int sample_number);
float calculateSD(float data[], int sample_number);
void CommonImplementation();
void calculateSampleSize(float mean, float sd, int sample_number);
void PrintSampleSizes();

//opeartion values and types are defined here
struct opeartion_value_node
{
	int data;
	int type; // 1-insert, 2-delete, 3-member
};
//this is to hold the resulted times for mean and sd calculations for each use case.
struct time_holder
{
	float* data_1;
	float* data_2; float* data_3; float* data_4; float* data_5;
	float* data_6; float* data_7; float* data_8; float* data_9;
};


//variable initializtion

int n, m = 0;
int sample_sizes[9];
int thread_count = 1;
float m_insert, m_delete, m_member = 0.0;
opeartion_value_node *valuesForOperations = NULL;;
list_node_s *head_node_p = NULL;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
clock_t t;
long thread;
pthread_t* thread_handles;
time_holder *holder = NULL;
int suggested_samples_sizes[9];

//main thread of execution
int _tmain(int argc, char* argv[])
{
	printf("Do you want to go with default sample size ? Pres 1 for yes, Press 0 for no: ");
	int option;
	scanf_s("%d", &option);
	if (option == 1) {
		for (int i = 0; i < 9; i++){
			sample_sizes[i] = 200;
		}
	}
	else {
		printf("Sample size for instance 1: ");scanf_s("%d", &sample_sizes[0]);
		printf("Sample size for instance 2: ");scanf_s("%d", &sample_sizes[1]);
		printf("Sample size for instance 3: ");scanf_s("%d", &sample_sizes[2]);
		printf("Sample size for instance 4: "); scanf_s("%d", &sample_sizes[3]);
		printf("Sample size for instance 5: "); scanf_s("%d", &sample_sizes[4]);
		printf("Sample size for instance 6: "); scanf_s("%d", &sample_sizes[5]);
		printf("Sample size for instance 7: "); scanf_s("%d", &sample_sizes[6]);
		printf("Sample size for instance 8: "); scanf_s("%d", &sample_sizes[7]);
		printf("Sample size for instance 9: "); scanf_s("%d", &sample_sizes[8]);
	}

	printf("Enter number of initial populations (n) : ");
	scanf_s("%d", &n);
	printf("Enter number of operations (m) : ");
	scanf_s("%d", &m);
	printf("Enter m-Member : ");
	scanf_s("%f", &m_member);
	printf("Enter m-Insert : ");
	scanf_s("%f", &m_insert);
	printf("Enter m-Delete : ");
	scanf_s("%f", &m_delete);

	//set the random seed 
	srand(time(NULL));

	holder = new time_holder;
	holder->data_1 = new float[sample_sizes[0]];
	holder->data_2 = new float[sample_sizes[1]];
	holder->data_3 = new float[sample_sizes[2]];
	holder->data_4 = new float[sample_sizes[3]];
	holder->data_5 = new float[sample_sizes[4]];
	holder->data_6 = new float[sample_sizes[5]];
	holder->data_7 = new float[sample_sizes[6]];
	holder->data_8 = new float[sample_sizes[7]];
	holder->data_9 = new float[sample_sizes[8]];

	for (int i = 0; i < sample_sizes[0]; i++) {
		CommonImplementation();
		PerformSerialProgram(i);
		free(head_node_p);
		head_node_p = NULL;
		free(valuesForOperations);
		valuesForOperations = NULL;
	}
	printf("Serial program\n");
	printf("--------------\n");
	printf("Averrage : %f and Standard Deviation : %f \n", calculateMean(holder->data_1, 0), calculateSD(holder->data_1, 0));
	free(holder->data_1);

	PerformWithMutex();
	PerformWithReadWriteLock();

	printf("\n");
	printf("Do you want to calculate suggested sample sizes based on the results ? Pres 1 for yes, Press 0 for no: ");
	int choice;
	scanf_s("%d", &choice);
	if (choice == 1) {
		PrintSampleSizes();
	}
	system("pause");
}

//this includes the common code for all the 3 cases. (serial, mutex, rwlock)
void CommonImplementation(){
	int *valuesForPopulation = new int[n];

	for (int i = 0; i < n; i++) {
		valuesForPopulation[i] = RandomValueGenerator(head_node_p);
		Insert(valuesForPopulation[i], &head_node_p);
	}
	free(valuesForPopulation);

	valuesForOperations = new opeartion_value_node[m];

	for (int i = 0; i < m; i++) {
		struct opeartion_value_node* temp = (opeartion_value_node*)malloc(sizeof(struct opeartion_value_node));
		temp->data = RandomValueGenerator(head_node_p);

		if (i <= ("%.2f", m_member*m)) {
			temp->type = 3;
		}
		else if (i <= ("%.2f", m_member*m) + ("%.2f", m_insert)*m){
			temp->type = 1;
		}
		else {
			temp->type = 2;
		}
		valuesForOperations[i] = *temp;
		free(temp);
	}
	ShuffleArray();
}

//This genetates a random number and it is unique in the given linked list
int RandomValueGenerator(struct list_node_s* head_p){
	int random_number = rand() % ((int)pow((float)2, 16) - 1 - 0) + 0;
	if (!CheckForUniqueness(head_p, random_number)){
		return RandomValueGenerator(head_p);
	}
	else {
		return random_number;
	}
}

//Fucntion to check the uniqueness in the generated values 
int CheckForUniqueness(struct list_node_s* head_p, int value){
	struct list_node_s* curr_p = head_p;
	while (curr_p != NULL) {
		if (head_p->data == value) {
			return 0;
		}
		curr_p = curr_p->next;
	}
	return 1;
}

//This fucntion shuffles the generated values array
void ShuffleArray(){
	int i = m - 1;
	int j;
	opeartion_value_node temp;

	srand(time(NULL));

	while (i > 0)
	{
		j = rand() % (i + 1);
		temp = valuesForOperations[i];
		valuesForOperations[i] = valuesForOperations[j];
		valuesForOperations[j] = temp;
		i = i - 1;
	}
}

//this initaiate the serial program 
void PerformSerialProgram(int count){
	
	t = clock();
	PerformOperationInSerial();
	t = clock() - t;
	double time_taken_serial = ((double)t) / CLOCKS_PER_SEC; // in seconds

	holder->data_1[count] = time_taken_serial;
}

//this initaiate the program with mutex for list
void PerformWithMutex(){
	thread_count = 1;
	while (thread_count <= 8) {
		int number_of_required_samples = 0;
		if (thread_count == 1) {
			number_of_required_samples = sample_sizes[1];
		}
		else if (thread_count == 2) {
			number_of_required_samples = sample_sizes[2];
		}
		else if (thread_count == 4) {
			number_of_required_samples = sample_sizes[3];
		}
		else {
			number_of_required_samples = sample_sizes[4];
		}
		for (int i = 0; i < number_of_required_samples; i++){
			CommonImplementation();
			mutex = (pthread_mutex_t)malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(&mutex, NULL);
			thread_handles = (pthread_t*)malloc(thread_count*(sizeof(pthread_t)));
			t = clock();

			for (thread = 0; thread < thread_count; thread++){
				pthread_create(&thread_handles[thread], NULL, PerformOperationWithMutex, (void*)thread);
			}
			for (thread = 0; thread < thread_count; thread++){
				pthread_join(thread_handles[thread], NULL);
			}
			t = clock() - t;
			double time_taken_mutex = ((double)t) / CLOCKS_PER_SEC; // in seconds

			AddTimeToHolder(time_taken_mutex, thread_count, 0, i);
			free(thread_handles);
			free(head_node_p);
			head_node_p = NULL;
			free(valuesForOperations);
			valuesForOperations = NULL;
			pthread_mutex_destroy(&mutex);
		}
		thread_count = 2 * thread_count;
	}
	
	printf("\n");
	printf("Mutex case\n");
	printf("----------\n");
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 1, calculateMean(holder->data_2, 1), calculateSD(holder->data_2, 1));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 2, calculateMean(holder->data_3, 2), calculateSD(holder->data_3, 2));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 4, calculateMean(holder->data_4, 3), calculateSD(holder->data_4, 3));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 8, calculateMean(holder->data_5, 4), calculateSD(holder->data_5, 4));
	free(holder->data_2);
	free(holder->data_3);
	free(holder->data_4);
	free(holder->data_5);
}

//this initaiate the program with read write lock for list
void PerformWithReadWriteLock(){

	thread_count = 1;
		while (thread_count <= 8) {
			int number_of_required_samples = 0;
			if (thread_count == 1) {
				number_of_required_samples = sample_sizes[1];
			}
			else if (thread_count == 2) {
				number_of_required_samples = sample_sizes[2];
			}
			else if (thread_count == 4) {
				number_of_required_samples = sample_sizes[3];
			}
			else {
				number_of_required_samples = sample_sizes[4];
			}
			for (int i = 0; i < number_of_required_samples; i++){
				CommonImplementation();
				rwlock = (pthread_rwlock_t)malloc(sizeof(pthread_rwlock_t));
				pthread_rwlock_init(&rwlock, NULL);
				thread_handles = (pthread_t*)malloc(thread_count*(sizeof(pthread_t)));
				t = clock();

				for (thread = 0; thread < thread_count; thread++){
					pthread_create(&thread_handles[thread], NULL, PerformOperationWithReadWriteLock, (void*)thread);
				}
				for (thread = 0; thread < thread_count; thread++){
					pthread_join(thread_handles[thread], NULL);
				}
				t = clock() - t;
				double time_taken_rwlock = ((double)t) / CLOCKS_PER_SEC; // in seconds

				AddTimeToHolder(time_taken_rwlock, thread_count, 1, i);
				free(thread_handles);
				free(head_node_p);
				head_node_p = NULL;
				free(valuesForOperations);
				valuesForOperations = NULL;
				pthread_rwlock_destroy(&rwlock);
			}
		thread_count = 2 * thread_count;
	}
	
	printf("\n");
	printf("ReadWriteLock case\n");
	printf("----------\n");
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 1, calculateMean(holder->data_6, 5), calculateSD(holder->data_6, 5));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 2, calculateMean(holder->data_7, 6), calculateSD(holder->data_7, 6));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 4, calculateMean(holder->data_8, 7), calculateSD(holder->data_8, 7));
	printf("No. of threads %d Averrage : %f and Standard Deviation : %f \n", 8, calculateMean(holder->data_9, 8), calculateSD(holder->data_9, 8));
	free(holder->data_6);
	free(holder->data_7);
	free(holder->data_8);
	free(holder->data_9);
}


//This fucntion is to perform operations in serial program
void PerformOperationInSerial() {
	
	int curr_index = 0;
	while (curr_index < m){
		opeartion_value_node *value_node = &valuesForOperations[curr_index];
		if (value_node->type == 3) {
			Member(value_node->data, head_node_p);
		}
		else if (value_node->type == 1) {
			Insert(value_node->data, &head_node_p);
		}
		else {
			Delete(value_node->data, &head_node_p);
		}
		curr_index++;
	}
}
//This fuction is to perform fucntions with mutex for entire list
void *PerformOperationWithMutex(void* thread) {
	long my_rank = (long)thread;
	int factor = m / thread_count;
	int start_index = my_rank*factor;
	int end_index = start_index + factor;
	int curr_index = start_index;
	while (curr_index < end_index){
		opeartion_value_node *value_node = &valuesForOperations[curr_index];
		if (value_node->type == 3) {
			pthread_mutex_lock(&mutex);
			Member(value_node->data, head_node_p);
			pthread_mutex_unlock(&mutex);
		}
		else if (value_node->type == 1) {
			pthread_mutex_lock(&mutex);
			Insert(value_node->data, &head_node_p);
			pthread_mutex_unlock(&mutex);
		}
		else {
			pthread_mutex_lock(&mutex);
			Delete(value_node->data, &head_node_p);
			pthread_mutex_unlock(&mutex);
		}
		curr_index++;
	}
	return NULL;
}

//This fucntion is to perform opeations with read write lock
void *PerformOperationWithReadWriteLock(void* thread) {
	long my_rank = (long)thread;
	int factor = m / thread_count;
	int start_index = my_rank*factor;
	int end_index = start_index + factor;
	int curr_index = start_index;
	while (curr_index < end_index){
		opeartion_value_node *value_node = &valuesForOperations[curr_index];
		if (value_node->type == 3) {
			pthread_rwlock_rdlock(&rwlock);
			Member(value_node->data, head_node_p);
			pthread_rwlock_unlock(&rwlock);
		}
		else if (value_node->type == 2) {
			pthread_rwlock_wrlock(&rwlock);
			Insert(value_node->data, &head_node_p);
			pthread_rwlock_unlock(&rwlock);
		}
		else {
			pthread_rwlock_wrlock(&rwlock);
			Delete(value_node->data, &head_node_p);
			pthread_rwlock_unlock(&rwlock);
		}
		curr_index++;
	}
	return NULL;
}
//This adds the calculated time to relevant time array
void AddTimeToHolder(float time, int thrd_count, int selectType, int count){
	if (thrd_count == 1 && selectType == 0) {
		holder->data_2[count] = time;
	} else if (thrd_count == 1 && selectType == 1) {
		holder->data_6[count] = time;
	} else if (thrd_count == 2 && selectType == 0) {
		holder->data_3[count] = time;
	}
	else if (thrd_count == 2 && selectType == 1) {
		holder->data_7[count] = time;
	} else if (thrd_count == 4 && selectType == 0) {
		holder->data_4[count] = time;
	}
	else if (thrd_count == 4 && selectType == 1) {
		holder->data_8[count] = time;
	} else if (thrd_count == 8 && selectType == 0) {
		holder->data_5[count] = time;
	}
	else if (thrd_count == 8 && selectType == 1) {
		holder->data_9[count] = time;
	}
}

//function to calculate mean
float calculateMean(float data[], int sample_number){
	float sum = 0.0, mean, standardDeviation = 0.0;
	for (int i = 0; i < sample_sizes[sample_number]; ++i){
		sum += data[i];
	}
	mean = sum / sample_sizes[sample_number];
	return mean;
}

//function to calculte standard deviation
float calculateSD(float data[], int sample_number)
{
	float standardDeviation = 0.0;
	float mean  = calculateMean(data, sample_number);
	for (int i = 0; i < sample_sizes[sample_number]; ++i){
		standardDeviation += pow(data[i] - mean, 2);
	}
	float val =  sqrt(standardDeviation / (sample_sizes[sample_number] - 1));
	calculateSampleSize(mean, val, sample_number);
	return val;
}

//this calculates the sample sizes based on the results
void calculateSampleSize(float mean, float sd, int sample_number){
	suggested_samples_sizes[sample_number] = pow((1.960 * 100 * sd) / (5 * mean), 2);
}

//this prints the sample sizes calculated
void PrintSampleSizes(){
	for (int i = 0; i < 9; i++){
		float sample_size = suggested_samples_sizes[i] + 0.5;
		printf("Sample size for instance %d is %d ", i, (int)sample_size);
		printf("\n");
	}
}
