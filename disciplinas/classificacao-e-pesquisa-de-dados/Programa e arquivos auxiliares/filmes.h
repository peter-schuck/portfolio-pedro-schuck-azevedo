#include <stdio.h>
#include <stdbool.h>
#define M 16 // Maximum degree of the B-tree


//estruturas para a árvore B de char
typedef struct chave
{
    char titulo[256];        //titulo do filme, padronizado no maximo = 256
    long pos;                //posicao dos dados do filme em forma de offset do arquivo binairo principal
} chave;

typedef struct BTreeNode
{
    int num_keys; // Number of keys currently in the node
    chave keys[M-1]; // Array of keys
    struct BTreeNode *children[M]; // Array of child pointers
    bool is_leaf; // True if node is a leaf
} BTreeNode;

//estruturas para a árvore B de int
typedef struct chave_num
{
    int number;
    int pos;
} chave_num;

typedef struct BTreeNum
{
    int num_keys; // Number of keys currently in the node
    chave_num keys[M-1]; // Array of keys
    struct BTreeNum *children[M]; // Array of child pointers
    bool is_leaf; // True if node is a leaf
} BTreeNum;

//estruturas para a árvore B de float
typedef struct chave_numf
{
    float number;
    int pos;
} chave_numf;

typedef struct BTreeNumf
{
    int num_keys; // Number of keys currently in the node
    chave_numf keys[M-1]; // Array of keys
    struct BTreeNumf *children[M]; // Array of child pointers
    bool is_leaf; // True if node is a leaf
} BTreeNumf;

// 📦 Funções de decodificação e utilitárias
void decod_gen(int gen);
int char_to_int(char* source);
void* get_field_array(void* array_source, int tam_array, int tam_struct, int field_offset, int tam_field);

// 📦 Árvore B de char*
BTreeNode* createNode(bool is_leaf);
void splitChild(BTreeNode* parent, int index);
void insertNonFull(BTreeNode* node, chave key);
void insert(BTreeNode** root, chave key);
void traverse(BTreeNode* root);
long busca_titulo(BTreeNode* root, char titulo[256], long x);
int busca_titulo2(BTreeNode* root, char titulo[256]);
int busca_sucessor(BTreeNode* root, char titulo[256], int vet[10]);
int busca_sucessor2(BTreeNode* root, char titulo[256], int vet[10]);
void traverse_backwards(BTreeNode* root);
void delete_Btree(BTreeNode* root);
void traverse_file(BTreeNode* root, FILE* arq);

// 📦 Árvore B de int
BTreeNum* createNodeN(bool is_leaf);
void splitChildN(BTreeNum* parent, int index);
void insertNonFullN(BTreeNum* node, chave_num key);
void insertN(BTreeNum** root, chave_num key);
int busca_id(BTreeNum* root, int id);
void traverseN(BTreeNum* root);
void traverse_backwardsN(BTreeNum* root);
void delete_BtreeN(BTreeNum* root);
void traverse_fileN(BTreeNum* root, FILE* arq);
void traverse_titleN(BTreeNum* root, FILE* filmes, FILE* titulos);
void traverse_title_backwardsN(BTreeNum* root, FILE* filmes, FILE* titulos);
//BTreeNum cria_e_grava_arvore_B_int(FILE* arq_estrutura, FILE* arq_arvore, int tam_of_struct, int offset_of_field);


// 📦 Árvore B de float
BTreeNumf* createNodeF(bool is_leaf);
void splitChildF(BTreeNumf* parent, int index);
void insertNonFullF(BTreeNumf* node, chave_numf key);
void insertF(BTreeNumf** root, chave_numf key);
void traverseF(BTreeNumf* root);
void traverse_backwardsF(BTreeNumf* root);
void delete_BtreeF(BTreeNumf* root);
void traverse_fileF(BTreeNumf* root, FILE* arq);
void traverse_titleF(BTreeNumf* root, FILE* filmes, FILE* titulos);
void traverse_title_backwardsF(BTreeNumf* root, FILE* filmes, FILE* titulos);
//BTreeNumf cria_e_grava_arvore_B_float(FILE* arq_estrutura, FILE* arq_arvore, int tam_of_struct, int offset_of_field);

void inicializa_arvores(BTreeNode* root, BTreeNum* raiz, BTreeNumf* rootf ,FILE* filmes, FILE* titulos, FILE* arq, FILE* arq4, FILE* arq5);