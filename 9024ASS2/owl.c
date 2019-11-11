/*H
 Discrition:   Implementation of COMP9024-Assignment 2 
               Using DFS-Recursion to get the longest path and print them all
               with Quack (Stack mainly);
 Aurthor:    Heng-Chuan Lin (z5219960@unsw.edu.au)
 Written Date:  2019-07-31   
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"
#include "Quack.h"
#define WHITESPACE 100
#define NUMBER 1000
#define STARTVERTEX 0
#define UNVISITED -1
#define VISITED 1

//DFS-related funciton//
int getMaxLen_dfs(Graph, Vertex, int);
void getMaxLen_dfsR(Graph, Vertex, int, int *, int *,Quack);
void printLongestLadder_dfs(Graph, Vertex, int, int, char **);
void printLongestLadder_dfsR(Graph, Vertex, int, int, int *, int *, int *, Quack, char **);
//memoryAllocation
int *createVisArray(int);
char **CreateDictionary();
//phase 1 function
bool differByOne(char *, char *);
//Graph-related function
void readGraph(int, Graph, char**);
int getNumV(char **);
//Quack-related function
bool inQuack(Quack, int);
int nitem_Quack(Quack);
//display-related funciton
void DisplayDictionary(char **, int);
void printladder(Quack, int, int,int *, char**);
//other funciton
void cleanUp(Graph,char**);

int main(void){

    int numV;                                                                                       
    int maxLen;
    char **dictionary = CreateDictionary();
    if ((numV = getNumV(dictionary)) > 0) {
        Graph owl = newGraph(numV);
        DisplayDictionary(dictionary, numV);
        readGraph(numV, owl, dictionary);
        showGraph(owl);
        maxLen = getMaxLen_dfs(owl, STARTVERTEX, numV);
        printLongestLadder_dfs(owl, STARTVERTEX, numV, maxLen, dictionary);        
        cleanUp(owl, dictionary);
    }else{
        printf("please input the file with right formate\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char **CreateDictionary() {

    /*function description:
        create the malloc space for further input words
        return char** dictionary
    */

    char **array = malloc(sizeof(char *) * NUMBER);                                        //1000 nodes (vertices) provided based on spec
    if (array == NULL) {                                                                   // Error handling
        fprintf(stderr, "CreateDictionary: out of memory\n");
        exit(EXIT_FAILURE);
     } 
    return array;
}

int *createVisArray(int numV) {

    /*function description:
        takes: number of vertices
        create the malloc space for int array 
        return int* visited array
    */

    int *array = malloc(numV * sizeof(int));                                               // Create memory                                    
    if (array == NULL) {                                                                   // Error handling
        fprintf(stderr, "createVisArray: out of memory\n");
        exit(EXIT_FAILURE);
    } 
    for (int i = 0; i < numV; i++) {                                                       //Intial array value
        array[i] = UNVISITED;
    } 
    return array;
}

int getNumV(char **array) {

    /*function description:
        takes: string array (dictionary)
        get the string from Stdin and do the whitespace cleaning 
        return the number of vertices
    */

    int v = 0;                                                                             // Initial V number
    char useless[WHITESPACE];                                                              // Container for the undesired syntax like \s \t \n
    char word[NUMBER];                                                                     // Array for String to further writing the dictionary
    scanf("%[ \t\n]s", useless);                                                            // Scanf to get rid of all useless syntax ref: lecture note
    while (scanf("%s", word) != EOF) {
        int word_len = strlen(word);                                                       // Error handing for string len is 0 case
        if (word_len > 0) {
            array[v] = malloc((word_len + 1) * sizeof(char));                              // Create the space for dictionary[i] +1 for '\0'
            if (array[v] == NULL) {                                                        // Error handling
                fprintf(stderr, "getNumV: out of memory\n");
                exit(EXIT_FAILURE);
            } 
            if( v == 0){
                strcpy(array[v], word);                                                    // Copy the word to dictionary
                v++;
            }else if (v != 0 && (strcmp(array[v-1], word) != 0) ){                          // make sure no duplicate
                strcpy(array[v], word);
                v++;
            }                                                       
            
        }
    }
    return v;
}

void DisplayDictionary(char **array,int numV) {

    /*function description:
        print the words from dictionary
    */

    printf("Dictionary\n");
    for(int i = 0; i < numV; i++) {
        printf("%d: %s\n", i, array[i]);
    }
    return;    
}

void readGraph(int numV, Graph owl, char** dictionary) {

    /*function description:
        takes: the words in dictionary
        using Differbyone function as rule for inserting the edge into the graph 
    */

    printf("Ordered Word Ladder Graph\n");
    for (int i = 0; i < numV; i++) {                                                         // Vertices scanning     
        for (int j = i + 1; j < numV; j++) {
            if (differByOne(dictionary[i], dictionary[j])) {
                insertEdge(newEdge(i,j), owl);                
            }
        }
    }
    return;
}

bool differByOne(char *s1, char *s2){

    /*function description:
        takes: two string, check is valid for word ladder rule ()
        return bool
    */

    int len_1 = strlen(s1);                                                                   // Get the length 
    int len_2 = strlen(s2);
    int len_changed;                                                                          // Observe the changes of len, word, order 
    int word_changed;
    int order_changed = 0;
    int same = 0;
    char *ptr1 = s1;
    char *ptr2 = s2;
    if (len_1 > len_2 ) {                                                                     // Defferentiate by the length
        for (int i = 0, j = 0; *(ptr2 + i) != '\0'; i++, j++) {
            if (*(ptr2 + i) == *(ptr1 + j) ) {
                same++;                                                                       // Count the same character
            }
            else if (!order_changed && *(ptr2 + i) == *(ptr1 + j + 1) ) {                     // Skip one char for first time if matching the next char 
                j += 1;
                order_changed = 1;
                same++;
            }     
        }
        len_changed = len_1 - len_2;                                                         // Sumerize the changes
        word_changed = len_1 - same;
    }else if ( len_1 < len_2) {
        for (int i = 0,j = 0; *(ptr1 + i) != '\0'; i++, j++) {
            if (*(ptr1 + i) == *(ptr2 + j)) {
                same++;
            }
            else if (!order_changed && *(ptr1 + i) == *(ptr2 + j + 1)) {
                j += 1;
                order_changed = 1;
                same++;
            }       
        }
        len_changed = len_2 - len_1;
        word_changed = len_2 - same;
    }else{
        for (int i = 0; *(ptr2 + i) != '\0'; i++) {            
            if (*(ptr2 + i) == *(ptr1 + i) ) {
                same++;
            }    
        }
        len_changed = len_1 - len_2;
        word_changed = len_1 - same;
    }
    if ((len_changed == 0 && word_changed == 1 )                                            // Only vaild if just one changed
        || (len_changed == 1 && word_changed == 1 )) {
        return true;
    }else{
        return false;
    }
}

bool inQuack(Quack q, int v){

    /*function description:
        since the client cant reach the ADT inside-structure,
        this function is implemented for check the number(vertice) is inside given Quack ornot
        by creating the temporary Quack.
        return bool
    */

    int found = false;                                                                     // Initial (not) found 
    Quack tmp = createQuack();                                                             // Create tmp Quack
    while(!isEmptyQuack(q)) {
        int p_num = pop(q);                                                                // Grab the popped number
        if(p_num == v){
            found = true;                                                                   // Found it!
        }
        qush(p_num, tmp);                                                                    // Use qush to keep the order
    }
    while(!isEmptyQuack(tmp)) {
        int p_num = pop(tmp);
        qush(p_num, q);                                                                      // Qush back
    }
    destroyQuack(tmp);                                                                      // Clean tmp Quack
    return found;
}

int nitem_Quack(Quack q){

    /*function description:
        since the client cant reach the ADT inside-structure,
        this function is implemented for get the number of element inside given Quack 
        by creating the temporary Quack.
    return the total number inside the given Quack
    */    

    int retval = 0;                                                                         // Initial number
    Quack tmp = createQuack();                                                              // Create tmp Quack
    while(!isEmptyQuack(q)) {
        int p_num = pop(q);
        qush(p_num, tmp);                                                                    // Qush to keep the order
        retval++;                                                                           // Count the times
    }
    while(!isEmptyQuack(tmp)) {
        int p_num = pop(tmp);
        qush(p_num, q);                                                                      // Qush it back
    }
    destroyQuack(tmp);                                                                       // Clean tmp Quack
    return retval;
}

int getMaxLen_dfs(Graph owl, Vertex rootv, int numV) {                                          

    /*function description:
    reference from Lecture note
        Initiate the DFS to get the longest path first.
        Cannot get the entire info (maxLength only) for this function (I'm so dumb)
    return the longest number of path
    */ 

    int *visited = createVisArray(numV);                                                         // Create visited array
    Quack Qs = createQuack();                                                                    // I use the stack-style Quack to process path
    int maxLen = 0;                                                                              // Intial maxLength of ladder
    Vertex startv = rootv;                                                                       // This is the starting vertex
    int allVis = 0;                                                                              // Handles disconnected graphs                   
    while (!allVis) {                                                                            // Assume not all visited
        getMaxLen_dfsR(owl, startv, numV, &maxLen, visited, Qs);                                 // Get into the recursion ( due to Recur, use & to make int to int*)
        allVis = 1;                     
        for (Vertex w = 0; w < numV && allVis; w++) {                                            // Disconnected section
            if (visited[w] == UNVISITED) { 
                allVis = 0;               
                startv = w;              
            }
        }
    }  
    free(visited);                                                                               // Clean section
    destroyQuack(Qs);
    return maxLen;
}

void getMaxLen_dfsR(Graph owl, Vertex v, int numV, int *maxLen, int *visited, Quack Qs) {

    /*function description:
    reference from Lecture note
        Recursion of the DFS to get the longest path.
        I used push to put the vertice into the stack and then pop to take a backtrack
        meanwhile I collected the max number in Quack i.e. the longest path number
    */ 

    if (*maxLen == numV) {                                                                        // terminate the recursion
        return;
    }
    push(v,Qs);                                                                                  // Push to the Quack 
    visited[v] = VISITED;                                                                        // Marked as visited
    for (Vertex w = 0; w < numV; w++ ){
        if (isEdge(newEdge(v, w), owl)  && v < w && !inQuack(Qs, w)) {                           // Check the element in Quack ornot                       
            getMaxLen_dfsR(owl, w, numV, maxLen, visited, Qs);
            if (*maxLen == numV){
                return;
            }                                   // Get into Recursion again
            int nitem = nitem_Quack(Qs);                                                         // Rrecord the number of element in Quack   
            if (nitem > *maxLen) {                                                                // Change the max if nitem > old max
                *maxLen = nitem;
                
            }
            pop(Qs);                                                                             // Using pop to do the backtracking 
        }
    } 
   return ;
}

void printLongestLadder_dfs(Graph owl, Vertex rootv,int numV, int maxLen, char ** dictionary) {

    /*function description:
        this function does almost the same thing as the previous dfs function
        In this funciton , a known maxlen has been given
        initiate the recursion again to get the quack sequence with maxlen
        a counter is created for counting the numbers of longest ladder
    */

    int *visited = createVisArray(numV);                                                        // Create visited list
    Quack Qs = createQuack();                                                                   // I use the stack-style Quack to process path
    Vertex startv = rootv; 
    int counter = 1;                                                                            // Count the amount of nLadders
    int allVis = 0;                                                                             // Diconnected graph setup
    int test = 0;
    if(maxLen == 0){
        printf("Longest ladder length: 1\n"); 
    }else{
        printf("Longest ladder length: %d\n",maxLen);                                            // print the max length
    }                                                 
    printf("Longest ladders:\n");
    while (!allVis) {               
        printLongestLadder_dfsR(owl, startv, numV, maxLen,&test, &counter, visited, Qs, dictionary);  // Call the recursion ( & the counter)    
        allVis = 1;                     
        for (Vertex w = 0; w < numV && allVis; w++) { 
            if (visited[w] == UNVISITED) { 
                allVis = 0;               
                startv = w;             
            }
        }
    }
    if( maxLen == 0) {                                                                      // handle if the maxLen == 0
        for(int i = 0; i < numV; i++) {
            printf("%2d: %s\n", i+1, dictionary[i]);
        }
    }   
    free(visited);                                                                          // Clean the memory
    destroyQuack(Qs);
    return;
}

void printLongestLadder_dfsR(Graph owl, Vertex v,int  numV, int maxLen, int *end, int *counter, int *visited, Quack Qs, char** dictionary) {

    /*function description:
        this function does almost the same thing as the previous dfsR function
        since known maxlen has been given,
        if the Quack with the Maxlen is detected, call the print function to display the word ladder
    */

    if(*end) {                                                                                               //terminate condition (different compared to the previous dfsR)
        return;
    }
    push(v, Qs);
    visited[v] = VISITED;                
    for (Vertex w = 0; w < numV && !(*end); w++ ) {
        if (isEdge(newEdge(v, w), owl)  && v < w && !inQuack(Qs,w)) {        
            printLongestLadder_dfsR(owl, w, numV, maxLen, end, counter, visited, Qs, dictionary);
            int num = nitem_Quack(Qs);                                                                      // Get the length of Quack
            if (num == maxLen) {                                                                            // If founded 
               printladder(Qs, numV, maxLen,counter ,dictionary);                                           // Call the print function
               *counter = *counter + 1;                                                                     // Counter the numbers of longest ladder
               if(maxLen == numV){                                                                          // termintate the recursion if reach the end;
                   *end = 1;
                   pop(Qs);                                                                                 // pop the element make sure terminate further recursion
                   return;
               }                                                               
            }              
            pop(Qs);                                                                                        // normal backtrack
        }
    } 
   return ;
}

void printladder(Quack q, int numV, int max ,int *n ,char** dictionary) {

    /*function description:
        this function would take the vertieces in given Quack
        then print them all.
        finally, using a tmp Quack to recover the given Quack
    */

    Quack tmp = createQuack();                                                  // Create tmp Quack 
    int index = 1;                                                              // Use the index to arrange the word
    while(!isEmptyQuack(q)){
        int p_num = pop(q);    
        push(p_num,tmp);
    }
    while(!isEmptyQuack(tmp)){
        int p_num = pop(tmp);
        if (index == 1 ){
        printf("%2d: %s ->",*n,dictionary[p_num]);
        }
        else if (index < max){
            printf(" %s ->",dictionary[p_num]);
        }else{
            printf(" %s\n",dictionary[p_num]);
        }
        push(p_num,q);
        index++;
    }
    destroyQuack(tmp);                                                           // Clean the tmp Quack
    
 
    return;
}

void cleanUp(Graph owl,char** dictionary){

    /*function description:
        free the variables
    */

    freeGraph(owl);
    free(dictionary);
    return;
}















