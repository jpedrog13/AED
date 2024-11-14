
typedef struct graph Graph;

Graph *inicializa(int V);

void insereA(Graph *G, int i, int j, double c);

void backbone(Graph *G, int st[], double val[]);

int caminho(int st[], int i, int j);

void escreve(Graph *G, FILE *f, int st[], double val[]);

void A1(Graph *G, FILE *f);

void B1(Graph *G, FILE *f, int v1, int v2);

void C1(Graph *G, FILE *f, int v1, int v2);

void D1(Graph *G, FILE *f, int v1);

void E1(Graph *G, FILE *f);

void liberta(Graph *G);



