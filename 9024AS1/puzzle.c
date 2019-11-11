/*H
 Discrition:   Client of COMP9024-Assignment 1 
 Aurthor:    Heng-Chuan Lin (z5219960@unsw.edu.au)
 Written Date:  2019-06-27   
 */
#include <stdio.h>
#include <stdlib.h>
#include "boardADT.h"
#include <string.h>

int main(){

    Darray start_array, goal_array;
    char* start_input = NULL;
    int start_size;
    char* goal_input = NULL;
    int goal_size;
    char* result = NULL;

    start_input = userinput();
    goal_input = userinput();   
    start_size = size_counting(start_input);
    goal_size = size_counting(goal_input);
    start_array = create_array(start_input, start_size);
    goal_array = create_array(goal_input, goal_size);
    result = solve(start_array,goal_array, start_size,goal_size);
    printf("start: ");
    printarray(start_array, start_size);
    printf("goal: ");
    printarray(goal_array, goal_size);
    printf("%s\n", result);
    cleaneveything(start_array, start_input);
    cleaneveything(goal_array, goal_input);
    return EXIT_SUCCESS;
}