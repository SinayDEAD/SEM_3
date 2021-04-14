#include "graphs.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(void){
    struct Graph *graph = CreateGraph(5);
    AddEdge(graph, 0, 1);
    AddEdge(graph, 0, 2);
    AddEdge(graph, 1, 2);
    AddEdge(graph, 1, 3);
    AddEdge(graph, 2, 4);
    AddEdge(graph, 3, 4);
    PrintGraph(graph);
    BFS(graph, 0);
    RemoveEdge(graph, 0, 2);
    PrintGraph(graph);
    RemoveVertex(graph, 2);
    PrintGraph(graph);
    AddVertex(graph);
    PrintGraph(graph);
    AddEdge(graph, 1, 4);
    PrintGraph(graph);
    DeleteGraph(&graph);
    PrintGraph(graph);
    return 0;
}
