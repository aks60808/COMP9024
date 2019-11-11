
#include <stdlib.h>
void dfsQuack(Graph g,Graph final, Vertex rootv, int numV) {
    int *visited = createVisArray(numV);
    int *parent = createVisArray(numV);
    Quack s = createQuack();
    push(rootv, s);
    showQuack(s);
    int order = 0;
    int allVis = 0; 
    int oldv=0;
    int firsttime =1;
    int ct = 0;
    while (!isEmptyQuack(s) && order >= 0) {      // FOUND TELLS US TO STOP
        int p = 0;
        
      Vertex v = pop(s);
      if(!firsttime && v == 0){
          for(int i = 0; i<numV;i++){
              if( !isEdge(newEdge(0,i), g )){
                  visited[i] = -1;
              }
          }
      }
      if (v > oldv){
          
          printf("%d - %d\n",oldv,v);
          insertEdge(newEdge(oldv,v),final);
          printf("longest is      %d\n",ct);
          ct ++;
      }else{
          ct = 1;
      }
      
      printf(" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ now   v is  %d and oldv is      %d\n",v,oldv);
      printf("visited: ---------------------- ");
      printarray(visited,numV);
      printf("parent:  ---------------------- ");
      printarray(parent,numV);
      printf("%d is the order now\n",order);
      if (visited[v] == UNVISITED) {   // we visit only unvisited vertices
         visited[v] = order++;
         
         for (Vertex w = numV - 1; w >= 0; w--) { //push adjacent vertices
            if (isEdge(newEdge(v,w), g) && v< w ) {        // ... in reverse order
               push (w, s);
               printf("push %d\n",w);
               parent[w] = v; 
               p=1;                      // ... onto the stack
            }
            if(!p && w == 0){
             printf("-----------------------------this is the dead end -----------------------------\n");
             printarray(visited,numV);
             printarray(parent,numV);
             order--;
             printf("decrease the order to %d\n",order);
             
             printf("***********************so i push %d ***************\n",parent[v]);
             push(parent[v],s);
             if (parent[v] != 0){
                 visited[v] = -2;
             }
         }
         }
         
      }else{
          printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ive already visited %d \n",v);
          printarray(visited,numV);
          printarray(parent,numV);
          if ( v != 0){
              int yes = 0;
              for (Vertex w = numV - 1; w >= 0; w--) { //push adjacent vertices
                if (isEdge(newEdge(v,w), g) && v < w ) { 
                    
                    if( (visited[w] == UNVISITED || visited[w] == -2) && w!=oldv && parent[w] != v){
                        printf("the w is %d\n",w);
                        printarray(parent,numV);
                        yes = 1;
                        push(w,s);
                        order++;
                    }       // ... in reverse order
                    parent[w] = v;        
                }
              
              }
              if(!yes){
                  printf("***********************so i push %d ***************\n",parent[v]);
                  visited[v] = -2;
                  order--;
                  printf("decrease the order to %d\n",order);
                  push(parent[v],s);
              }else{
                  printf("*********************** keep going ***************\n");
              }
          }
      }
      showQuack(s);
        firsttime = 0;
    oldv = v;
   }
   printarray(visited,numV);
    printarray(parent,numV);
    showGraph(final);
   free(visited);
   return;
}


