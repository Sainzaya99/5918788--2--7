#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 100         
#define MAX_EDGES 4000 

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

int matrix[V][V]; 
Node* list[V];     
int nodeCount;     

int nb[V];
int nbCount;

int edges[MAX_EDGES][2];

void reset_graphs(void) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) matrix[i][j] = 0;
        list[i] = NULL;
    }
    nodeCount = 0;
}

void free_list(void) {
    for (int i = 0; i < V; i++) {
        Node* p = list[i];
        while (p != NULL) {
            Node* next = p->next;
            free(p);
            p = next;
        }
        list[i] = NULL;
    }
}

void list_push_front(int u, int v) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->vertex = v;
    n->next = list[u];
    list[u] = n;
    nodeCount++;
}

void make_graph(int edgeCount) {
    reset_graphs();
    int made = 0;
    while (made < edgeCount) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v) continue;         
        if (matrix[u][v] == 1) continue; 

        matrix[u][v] = 1;
        matrix[v][u] = 1;
        
        list_push_front(u, v);
        list_push_front(v, u);

        edges[made][0] = u;
        edges[made][1] = v;
        made++;
    }
}

int matrix_insert(int u, int v) {
    int cmp = 1;
    if (matrix[u][v] == 0) {
        matrix[u][v] = 1;
        matrix[v][u] = 1;
    }
    return cmp;
}

int matrix_delete(int u, int v) {
    int cmp = 1;
    if (matrix[u][v] == 1) {
        matrix[u][v] = 0;
        matrix[v][u] = 0;
    }
    return cmp;
}

int matrix_connected(int u, int v, int* result) {
    *result = matrix[u][v];
    return 1;
}

int matrix_adjacent(int u) {
    int cmp = 0;
    nbCount = 0;
    for (int j = 0; j < V; j++) {
        cmp++;
        if (matrix[u][j] == 1) nb[nbCount++] = j;
    }
    return cmp;
}

int list_insert(int u, int v) {
    int cmp = 0;
    int found = 0;
    for (Node* p = list[u]; p != NULL; p = p->next) {
        cmp++;
        if (p->vertex == v) {
            found = 1;
            break;
        }
    }
    if (!found) {
        list_push_front(u, v);
        list_push_front(v, u);
    }
    return cmp;
}

void list_remove_one(int a, int b, int* cmp) {
    Node* prev = NULL;
    Node* p = list[a];
    while (p != NULL) {
        (*cmp)++;
        if (p->vertex == b) {
            if (prev == NULL) list[a] = p->next;
            else prev->next = p->next;
            free(p);
            nodeCount--;
            return;
        }
        prev = p;
        p = p->next;
    }
}

int list_delete(int u, int v) {
    int cmp = 0;
    list_remove_one(u, v, &cmp);
    list_remove_one(v, u, &cmp);
    return cmp;
}

int list_connected(int u, int v, int* result) {
    int cmp = 0;
    *result = 0;
    for (Node* p = list[u]; p != NULL; p = p->next) {
        cmp++;
        if (p->vertex == v) {
            *result = 1;
            break;
        }
    }
    return cmp;
}

int list_adjacent(int u) {
    int cmp = 0;
    nbCount = 0;
    for (Node* p = list[u]; p != NULL; p = p->next) {
        cmp++;
        nb[nbCount++] = p->vertex;
    }
    return cmp;
}

void print_neighbors(int u, const char* kind) {
    printf("Adjacent vertices of %d (%s) :", u, kind);
    for (int i = 0; i < nbCount; i++) printf(" %d", nb[i]);
    printf("\n\n");
}

void run_experiment(const char* name, int edgeCount, int caseNo) {
    make_graph(edgeCount);

    long matrixMem = sizeof(matrix);
    long listMem = sizeof(list) + (long)nodeCount * sizeof(Node);

    int iu, iv; 
    do {
        iu = rand() % V;
        iv = rand() % V;
    } while (iu == iv || matrix[iu][iv] == 1);

    int k = rand() % edgeCount; 
    int du = edges[k][0];
    int dv = edges[k][1];

    int cu = rand() % V; 
    int cv;
    do {
        cv = rand() % V;
    } while (cv == cu);

    int av = rand() % V; 

    printf("=== %s Graph (V=%d, E=%d) ===\n", name, V, edgeCount);
    printf("Insert (%d,%d) / Delete (%d,%d) / Connect (%d,%d) / Adjacent of %d\n\n",
        iu, iv, du, dv, cu, cv, av);

    int mIns = matrix_insert(iu, iv);
    int mDel = matrix_delete(du, dv);
    int mRes;
    int mCon = matrix_connected(cu, cv, &mRes);
    int mAdj = matrix_adjacent(av);
    printf("Case %d : %s Graph - Adjacency Matrix\n", caseNo, name);
    printf("Memory                         : %ld Bytes\n", matrixMem);
    printf("Edge insertion comparisons     : %d\n", mIns);
    printf("Edge deletion comparisons      : %d\n", mDel);
    printf("Connectivity check comparisons : %d\n", mCon);
    printf("Adjacent vertices comparisons  : %d\n", mAdj);
    print_neighbors(av, "matrix");

    int lIns = list_insert(iu, iv);
    int lDel = list_delete(du, dv);
    int lRes;
    int lCon = list_connected(cu, cv, &lRes);
    int lAdj = list_adjacent(av);
    printf("Case %d : %s Graph - Adjacency List\n", caseNo + 1, name);
    printf("Memory                         : %ld Bytes\n", listMem);
    printf("Edge insertion comparisons     : %d\n", lIns);
    printf("Edge deletion comparisons      : %d\n", lDel);
    printf("Connectivity check comparisons : %d\n", lCon);
    printf("Adjacent vertices comparisons  : %d\n", lAdj);
    print_neighbors(av, "list");

    free_list();
}

int main(void) {
    srand(time(NULL)); 

    run_experiment("Sparse", 100, 1);
    run_experiment("Dense", 4000, 3);
    return 0;
}