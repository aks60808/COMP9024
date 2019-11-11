/*H
 Discrition:   Interface of COMP9024-Assignment 1 
 Authir:    Heng-Chuan Lin (z5219960@unsw.edu.au)
 Written Date:  2019-06-27   
 */




typedef struct darray *Darray;
Darray create_array(char* inputstr,int num_of_digit);
char* userinput();
int size_counting(char* str);
char* solve(Darray start_array,Darray goal_array,int start_size, int goal_size);
void cleaneveything(Darray p_array, char* str);
void printarray(Darray p_array, int array_size);