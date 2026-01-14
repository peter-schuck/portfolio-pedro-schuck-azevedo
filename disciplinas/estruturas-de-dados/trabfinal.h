#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Gera_dados(void);

//abp:
typedef int TipoInfo;

typedef struct TNodoA pNodoA;

struct TNodoA{           //TAD da abp
int user;                //usuário, usado como chave
char senha[20];          //senha de até 20 caracteres
struct TNodoA *esq;      //ponteiro para o filho à esquerda
struct TNodoA *dir;      //ponteiro para o filho à direita
};

pNodoA * InsereArvore(pNodoA * a, int ch, char senha[20]);                   //insere na abp (com recursão)
pNodoA* InsereArvore2(pNodoA* a, int ch, char senha[20]);                    //insere um elemento na árvore de pesquisa binária
pNodoA* consulta(pNodoA *a, int chave, int *compabp);                        //consulta um nodo na árvore
int Verifica_senhabp(pNodoA *a, int user, char senha[20], int *compabp);     //procura uma senha na árvore
void Destroiabp(pNodoA *t);                                                  //destrói a árvore

//lse:

typedef struct TipoInfoNo InfoNo;
typedef struct TipoPtNo PtNo;

struct TipoPtNo{           //TAD da lse
       int user;           //usuário
       char senha[20];     //senha de até 20 caracteres
       PtNo *prox;         //ponteiro para o próximo nodo
       };

PtNo* cria_lista(void);                                                    //cria uma lista vazia
PtNo* insere_ord (PtNo *l, int user, char senha[]);                        //coloca um elemento no final da lista
PtNo* destroi(PtNo* l);                                                    //destrói a lista
int Verifica_senhalse(PtNo* l, int user, char senha[20], int *complse);    //procura uma senha dentro da lista
