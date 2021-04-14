#ifndef graphs_h
#define graphs_h

#define SIZE 254

struct Node* CreateNode(int v); //создаём

struct Node{
   int vertex;             //id
   int nv;                 //
   struct Node *next;      //adjecentVertexIds
};

struct Graph{
   int *visited;           //visited
   struct Node **adjlists; //vertices
   int numvertices;        //nVertices
};

struct Graph *CreateGraph(int v); //создаём граф

void AddEdge(struct Graph *graph, int a, int b); // добавка ребра

void PrintGraph(struct Graph *graph); //печать

void DeleteGraph(struct Graph **graph); // удаляем

void RemoveEdge(struct Graph *graph, int a, int b); // убрать ребро

void RemoveVertex(struct Graph *graph, int v); // удаляем вершину

void AddVertex(struct Graph *graph); // добавляем вершину

void DFS(struct Graph*, int v); //обход в в глубину

struct Turn {            // структура для обхода в ширину
    int it[SIZE];
    int f;
    int r;
};

struct Turn *CreateQueue(void); //создание

void Enq(struct Turn *q, int); //для переполнения

int Deq(struct Turn *q);  //для печати

int Empty(struct Turn *q); //проверка на пустоту

void PrintQueue(struct Turn *q); //печать обхода в очереди

void BFS(struct Graph *graph, int s); //обход в ширину

void PrintMenu(void); // Менюшка

int GetVariant(int c); //для взятия варианта

#endif
