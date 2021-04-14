#include "graphs.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(void) {
    int variant, w, a, b;
    printf("Введите кол-во вершин: ");
    scanf("%d", &w);
    struct Graph *graph = CreateGraph(w);
    do {
        PrintMenu(); // выводим меню на экран
        variant = GetVariant(9); // получаем номер выбранного пункта меню
        switch (variant){
            case 1:
                printf("Добавить ребро от x к y\n");
                scanf("%d%d", &a, &b);
                AddEdge(graph, a, b);
                break;
            case 2:
                AddVertex(graph);
                break;
            case 3:
                printf("Убрать ребро от x к y\n ");
                scanf("%d%d", &a, &b);
                RemoveEdge(graph, a, b);
                break;
            case 4:
                printf("Убрать вершину  ");
                scanf("%d", &a);
                RemoveVertex(graph, a);
                break;
            case 5:
                printf("С какого начать обход  ");
                scanf("%d", &b);
                DFS(graph, b);
                break;
            case 6:
                printf("С какого начать обход  ");
                scanf("%d", &a);
                BFS(graph, a);
                break;
            case 7:
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
                break;
            case 8:
                PrintGraph(graph);
                break;
        }
        if (variant != 9)
            system("pause");
    }
    while (variant != 9);
    DeleteGraph(&graph);
    PrintGraph(graph);
    return 0;
}
