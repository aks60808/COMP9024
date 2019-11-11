/*H
 Discrition:   Implementation of COMP9024-Assignment 1 
                checking the input board from stdin is solvable 
                compared to the goal.
 Aurthor:    Heng-Chuan Lin (z5219960@unsw.edu.au)
 Written Date:  2019-06-27   
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "boardADT.h"
#define defaultsize 256

char* userinput();
int size_counting(char* str);
Darray create_array(char* inputstr, int num_of_digit);
char* solve(Darray start_array, Darray goal_array, int start_size, int goal_size);
int countinversion(Darray p_array, int array_size);
int countrownum(Darray p_array, int array_size);
int issolovalble(Darray p_array, int array_size);
void cleaneveything(Darray p_array, char* str);
void printarray(Darray p_array, int array_size);

typedef struct darray{
    char *input_elem;
    int *array;
    int size;
} darray;

char* userinput(){

    /* this function return input string from stdin */

    int i =0;
    char *input = malloc(defaultsize * sizeof(char));
    if (input == NULL) {
        fprintf(stderr, "error: memory issue\n");
        exit(EXIT_FAILURE);
    }
    char *str = malloc(defaultsize * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "error: memory issue\n");
        exit(EXIT_FAILURE);
    }  
    fgets(input,defaultsize,stdin);
    for (;i < strlen(input); i++) {        
        if (input[i] == '\t'){
            str[i] = ' ';
        }else{
            str[i] = input[i];
        }
    }
    str[i] = '\0';
    free(input);
    input = NULL;
    return str;
}

int size_counting(char* str){

    /* this function takes input string and 
    return its number of element by splitting space */

    int num_of_digit = 0;
    int N;
    double root;
    int len = strlen(str);   
    for (int i = 0;i<len;i++) {        
        if (*(str) != ' ' && *(str) != '\n' && (*(str + 1) == ' ' 
        || *(str + 1) == '\n' || *(str + 1) == '\0')) {         
            num_of_digit++;
        }
        str++;       
    }
    root = sqrt((double)num_of_digit);
    N = root;    
    if (N != root || N < 2) {
        printf("error: input number %d invalid\n",num_of_digit);
        exit(EXIT_FAILURE);
    }    
    return num_of_digit;
}


Darray create_array(char* inputstr, int num_of_digit){

    /* this function takes input string and number of input element, then
    return its dynamic array structure */

    int index = 0;
    const char splitter[2] = " ";
    char *token;
    int num_of_b = 0;
    Darray p_array = NULL;
    p_array = malloc(sizeof(darray));
    if (p_array == NULL) {
        fprintf(stderr, "error: memory issue with malloc\n");
        exit(EXIT_FAILURE);
    }
    p_array->input_elem = inputstr;    
    p_array->size = num_of_digit;
    p_array->array = calloc(num_of_digit, sizeof(int));
    if (p_array == NULL) {
        fprintf(stderr,"error: memory issue\n");
        exit(EXIT_FAILURE);
    }
    token  = strtok(p_array->input_elem, splitter);
    while (token != NULL) {      
        if (atoi(token) != 0) {
            p_array->array[index] = atoi(token);
        }
        else if (strcmp(token,"b") == 0 || strcmp(token,"b\n") == 0 ) {
            p_array->array[index] = 0;
        }
        else if ((strcmp(token, "\n") == 0)) {
            ;
        }else{           
            printf("error: wrong input (use digit and 'b')\n");
            exit(EXIT_FAILURE);
        }
        index++;    
        token = strtok(NULL, splitter);
    }
    for (int i = 0; i < num_of_digit; i++) {      
        if (p_array->array[i] == 0) {
                num_of_b++;
        }
        for (int j = num_of_digit-i; j < num_of_digit; j++) {
            if (i != j && p_array->array[i] == p_array->array[j]
            && p_array->array[i] != 0) {                
                printf("error: duplicate digit detected\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    if (num_of_b != 1) {
        printf("error: 0 or more b detected\n");
        exit(EXIT_FAILURE);
    } 
    for (int i = 1; i < num_of_digit; i++) {
        for (int j = 0 ; j < num_of_digit; j++) {           
            if (p_array->array[j] == i || p_array->array[j] == '0') {
                break;
            }
            else if (p_array->array[j] != i && j == num_of_digit -1 ) {
                printf("error: input digit out of range in [1 to N*N-1]\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return p_array;
}


char* solve(Darray start_array, Darray goal_array, int start_size, int goal_size){

    /* this function takes tow ADT array and their sizes, then
    return string of sovable or not */

    int start_array_solvable = issolovalble(start_array, start_size);
    int goal_array_solvable = issolovalble(goal_array, goal_size);    
    if (start_size != goal_size) {
        printf("error: different size of array\n");
        exit(EXIT_FAILURE);
    }
    if (start_array_solvable && goal_array_solvable) {
        return "sovable";
    }else{
        return "unsovable";
    }
}

int countinversion(Darray p_array, int array_size){

    /* this function takes ADT array and its sizes, then
    return the inversion number */

    int invcount = 0;
    for (int i = 0; i < array_size; i++) {
        for (int j = i + 1; j < array_size; j++) {
          if (p_array->array[i] > p_array->array[j]
          && p_array->array[i] != 0 && p_array->array[j] != 0) {
              invcount++;
          }  
        }
    }
    return invcount;
}

int countrownum(Darray p_array, int array_size){

    /* this function takes ADT array and its sizes, then
    return the row number of the blank */

    int position;
    int rownum;
    int N = sqrt((double)array_size);
    for (int i = 0; i < array_size; i++) {
        if (p_array->array[i] == 0) {
            position = i;
        }
    }
    if (position % N == 0) {
        rownum = N - (position / N) + 1;
    }else{
        rownum = N - (position / N);
    }
    return rownum;
}

int issolovalble(Darray p_array, int array_size){

    /* this function takes ADT array and its sizes, then
    return the int 1 for sovable or 0 for unsovable */

    int N = sqrt((double)array_size);
    int inversion_num = countinversion(p_array, array_size);
    if (N % 2 == 1 && inversion_num % 2 == 0) {
        return 1;
    }
    else if (N % 2 == 0) {
        int rownumofb = countrownum(p_array, array_size);
        if ((rownumofb % 2 == 0 && inversion_num % 2 == 1)
        ||(rownumofb % 2 == 1 && inversion_num % 2 == 0)) {
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}   

void cleaneveything(Darray p_array, char* str){

    /* this function takes ADT array and input string, then
    free the memory */

    free(p_array->array);
    free(p_array);
    p_array = NULL;
    free(str);
    str = NULL;
}

void printarray(Darray p_array, int array_size){

    /* this function takes ADT array and its sizes, then
    return the array syntax for display */

    for (int i = 0; i < array_size; i++){
        if (i != array_size - 1){
            if (p_array->array[i] == 0){
                printf("b ");
            }else{
                printf("%d ",p_array->array[i]);
            }
        }else{
            if (p_array->array[i] == 0){
                printf("b");
            }else{
                printf("%d", p_array->array[i]);
            }
        }
    }
    printf("\n");
}