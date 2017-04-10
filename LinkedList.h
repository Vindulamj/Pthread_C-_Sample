#include "stdafx.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//fucntion initialization.
int Delete(int value, struct list_node_s** head_pp);
int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int traverse(struct list_node_s** head_pp);
void PrintLinkedList(struct list_node_s* head_p);

//linked list nodes are defined here
struct list_node_s
{
	int data;
	struct list_node_s *next;
};

//check if the given value is a member of the function or not
int Member(int value, struct list_node_s* head_p){

	struct list_node_s* curr_p = head_p;

	while (curr_p != NULL && curr_p->data < value){
		curr_p = curr_p->next;
	}

	if (curr_p == NULL || curr_p->data > value){
		return 0;
	}
	else {
		return 1;
	}
}

//this fucntion is to perform insert opration in the given linked list
int Insert(int value, struct list_node_s** head_pp){
	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	struct list_node_s* temp_p;

	while (curr_p != NULL && curr_p->data < value){
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p == NULL || curr_p->data > value){
		temp_p = (list_node_s*)malloc(sizeof(struct list_node_s));
		temp_p->data = value;
		temp_p->next = curr_p;
		if (pred_p == NULL) {
			*head_pp = temp_p;
		}
		else {
			pred_p->next = temp_p;
		}
		return 1;
	}
	else {
		return 0;
	}
}

//This fucntion is to perform delete operation in the linked list.
int Delete(int value, struct list_node_s** head_pp){
	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value){
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p != NULL && curr_p->data == value){
		if (pred_p == NULL) {
			*head_pp = curr_p->next;
			free(curr_p);
		}
		else {
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	}
	else {
		return 0;
	}
}

//This prints the values of the given linked list
void PrintLinkedList(struct list_node_s* head_p){
	while (head_p != NULL) {
		printf("%d", head_p->data);
		printf(",");
		head_p = head_p->next;
	}
}
