#include "graphs.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node* CreateNode(int v){
   struct Node *newnode = malloc(sizeof(struct Node));
   newnode->vertex = v;
   newnode->nv = 0;
   newnode->next = NULL;
   return newnode;
}

void AddVertex(struct Graph *graph){
    graph->numvertices += 1;
    graph->adjlists = realloc(graph->adjlists, graph->numvertices * sizeof(struct Graph));
    graph->adjlists[graph->numvertices - 1] = NULL;
}

struct Graph *CreateGraph(int v){
    if( v == 0){
        printf("Граф не существует\n");
        exit(0);
    }
    else{
        struct Graph *graph = malloc(sizeof(struct Graph));
        graph->numvertices = v;
        graph->adjlists = malloc(v * sizeof(struct Node*));
        graph->visited = malloc(v * sizeof(int));
        for (int i = 0; i < v; ++i){
            graph->adjlists[i] = NULL;
            graph->visited[i] = 0;
        }
        return graph;
    }
}

void AddEdge(struct Graph *graph, int a, int b) //добавляем
{
    if((a >= graph->numvertices) || (b >= graph->numvertices)){
        return;
    }
    else{
        struct Node *newnode = CreateNode(a);
        newnode->next = graph->adjlists[b];
        newnode->nv++;
        graph->adjlists[b] = newnode;
        
        newnode = CreateNode(b); //
        newnode->next = graph->adjlists[a];
        newnode->nv++;
        graph->adjlists[a] = newnode;
    }
}

void PrintGraph(struct Graph *graph) //печать в формате списка смежность
{
    if (!graph){
        printf("Граф не существтует\n");
        return;
    }
   for (int v = 0; v < graph->numvertices; ++v){
       struct Node *t = graph->adjlists[v];
       printf("\nСписок смежности вершины %d\n ", v);
       while (t){
           printf("%d -> ", t->vertex);
           t = t->next;
       }
       printf("\n");
   }
    printf("\n");
}

void DeleteGraph(struct Graph **graph){
    if (!*graph)
        return;
    for (int i = 0; i < (*graph)->numvertices; i++){
        struct Node *pre = (*graph)->adjlists[i];
        if (pre == NULL)
            continue;
        while(pre){
            pre =pre->next;
            free((*graph)->adjlists[i]);
            (*graph)->adjlists[i] = pre;
        }
    }
    free((*graph)->visited);
    free((*graph)->adjlists);
    free(*graph);
    *graph = NULL;
}
    
void RemoveEdge(struct Graph *graph, int a, int b){
    if((a >= graph->numvertices)||(b >= graph->numvertices)){
        return;
    }
    struct Node *cur = graph->adjlists[a];
    struct Node *pre = graph->adjlists[a];
    while (cur != NULL){
        if (cur->vertex == b){
            if (pre == cur){
                cur = cur->next;
                free(pre);
                pre = cur;
                graph->adjlists[a] = cur;
                continue;
            }
            pre->next = cur->next;
            free(cur);
            cur = pre;
        }
        pre = cur;
        cur = cur->next;
    }
    cur = graph->adjlists[b];
    pre = graph->adjlists[b];
    while (cur != NULL){
        if (cur->vertex == a){
            if (pre == cur){
                cur = cur->next;
                free(pre);
                pre = cur;
                graph->adjlists[b] = cur;
                continue;
            }
            pre->next = cur->next;
            free(cur);
            cur = pre;
        }
        pre = cur;
        cur = cur->next;
    }
}

void RemoveVertex(struct Graph *graph, int v){
    if(v >= graph->numvertices){
        return;
    }
    struct Node *pre, *cur;
    int i;
    pre = graph->adjlists[v];
    if (pre != NULL){
        while (pre != NULL){
            cur = pre->next;
            free(pre);
            pre = cur;
        }
    }
    for (i = v + 1; i < graph->numvertices; ++i)
    graph->adjlists[i - 1] = graph->adjlists[i];
    graph->numvertices -= 1;
    for (i = 0; i < graph->numvertices; ++i){
        cur = graph->adjlists[i];
        pre = graph->adjlists[i];
        while (cur != NULL){
            if (cur->vertex == v){
                if (pre == cur){
                    cur = cur->next;
                    free(pre);
                    pre = cur;
                    graph->adjlists[i] = cur;
                    continue;
                }
                pre->next = cur->next;
                free(cur);
                cur = pre;
            }
            else if (cur->vertex > v){
                cur->vertex -= 1;
            }
            pre = cur;
            cur = cur->next;
        }
    }
}

void DFS(struct Graph *graph, int v){
    struct Node *adjlist = graph->adjlists[v];
    struct Node *t = adjlist;
    graph->visited[v] = 1;
    printf("Посетил %d \n", v);
    while(t != NULL){
        int connectedvertex = t->vertex;
        if(graph->visited[connectedvertex] == 0){
            DFS(graph, connectedvertex);
        }
        t = t->next;
    }
}

void BFS(struct Graph *graph, int s) {
    if(s > graph->numvertices){
        printf("Неверная вершина\n");
        return;
    }
    struct Turn *q = CreateQueue();
    graph->visited[s] = 1;
    Enq(q, s);
    while(!Empty(q)){
        PrintQueue(q);
        int currentvertex = Deq(q);
        printf("Посетил %d\n", currentvertex);
        struct Node *t = graph->adjlists[currentvertex];
        while(t){
            int adjvertex = t->vertex;
            if(graph->visited[adjvertex] == 0){
                graph->visited[adjvertex] = 1;
                Enq(q, adjvertex);
            }
            t = t->next;
       }
    }
}

struct Turn *CreateQueue(void) {
    struct Turn *q = malloc(sizeof(struct Turn));
    q->f = -1;
    q->r = -1;
    return q;
}

int Empty(struct Turn *q) {
    if(q->r == -1)
        return 1;
    else
        return 0;
}

void Enq(struct Turn *q, int v){
    if(q->r == SIZE-1)
        printf("\nОчередь заполнена!!");
    else {
        if(q->f == -1)
            q->f = 0;
        q->r++;
        q->it[q->r] = v;
    }
}

int Deq(struct Turn *q){
    int item;
    if(Empty(q)){
        printf("\nОчередь пуста");
        item = -1;
    }
    else{
        item = q->it[q->f];
        q->f++;
        if(q->f > q->r){
            printf("\nСброс очереди\n");
            q->f = q->r = -1;
        }
    }
    return item;
}

void PrintQueue(struct Turn *q){
    int i = q->f;
    if(Empty(q)) {
        printf("\nОчередь пуста");
    } else {
        printf("\nОчередь содержит \n");
        for(i = q->f; i < q->r + 1; ++i) {
                printf("%d\n", q->it[i]);
        }
    }
}

void PrintMenu(void){
    printf("Привет\n");
    printf("Сделаем ка что-то с графом\n");
    printf("1. Добавим рёбра\n");
    printf("2. Добавим вершину\n");
    printf("3. Удалим ребро\n");
    printf("4. Удалим вершину\n");
    printf("5. Обход в ширину с выводом\n");
    printf("6. Обход в длину с выводом\n");
    printf("7. Дэфолтно\n");
    printf("8. Печать\n");
    printf("9. Усё\n");
    printf("\n");
}
/*
int GetVariant(int c){
    int variant;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку
    // пока ввод некорректен, сообщаем об этом и просим повторить его
    while (sscanf(s, "%d", &variant) != 1 || variant < 1 || variant > c){
        printf("Некорректно введено число. Попробуйте ещё раз: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }
    return variant;
}
*/

