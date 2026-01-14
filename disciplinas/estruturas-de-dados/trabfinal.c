#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "trabfinal.h"

void Gera_dados(void)             //função para gerar os dados em txt de mil, dez mil e cem mil linhas
{
    srand(time(NULL));
    FILE *arq3;
    int x = 0, user=1, tam, cont=0, loun;
    char senha[20];
    if ((arq3 = fopen("algo_para_o_video.txt", "w")) == NULL)     //abre o arquivo para escrita
    {
        printf("erro na abertura do arquivo3");
    }
    for(x=0; x<10; x++)                            //repete o processo cem vezes
    {
        tam = 5 + (rand() % 15);                      //randomiza um tamanho entre 5 e 20
        for(cont=0; cont<tam; cont++)                  //repete o mesmo número de vezes que o tamanho
        {
            loun = rand() % 3;                        //randomiza um número para definir se será letra ou número na senha
            if (loun == 0 || loun == 1)                    //66% de chance
            {
                senha[cont] = 97 + (rand() % 26);         //aleatoriza uma letra em ascii e coloca em senha
            }
            else
            {
                senha[cont] = 48 + (rand() % 10);         //33% de chance de colocar 0 a 9 em ascii e colocar em senha
            }
        }
        senha[cont] = '\0';                               //fecha a string senha
        fprintf(arq3, "%d %s\n", user, senha);            //escreve no arquivo
        user = user + 1;                                   //soma um a user para passar para o próximo usuário
    }
    fclose(arq3);                                         //fecha o arquivo
}

//abp:


pNodoA* InsereArvore(pNodoA* a, int ch, char senha[20])    //função adquirida no moodle
{
  if (a == NULL)
  {
    a = (pNodoA*) malloc(sizeof(pNodoA));
    a->user = ch;
    strcpy(a->senha, senha);
    a->esq = NULL;
    a->dir = NULL;
  }
  else
  if (ch < (a->user))
    a->esq = InsereArvore(a->esq,ch, senha);
  else
    a->dir = InsereArvore(a->dir,ch, senha);
    return a;
}

pNodoA* InsereArvore2(pNodoA* a, int ch, char senha[20])   //função adquirida no site stack overflow (ver referências bibliográficas); adaptações: alocação dinâmica de memória
{
    pNodoA **aux = &a,*novo;                   //aux recebe o endereço da raiz
    while(*aux)                               //enquanto o ponteiro de ponteiro não for nulo
    {
        if (ch < (*aux)->user)
        {
            aux = &(*aux)->esq;               //se a chave for menor que o user de aux, aux aponta para seu filho à esquerda
        }
        else
        {
            aux = &(*aux)->dir;               //se a chave for maior ou igual que o user de aux, aux aponta para seu filho à direita
        }
    }
    novo = (pNodoA*) malloc(sizeof(pNodoA));  //aloca memória para o novo nodo
    novo->user = ch;                          //põem em seu user a chave passada como parâmetro
    strcpy(novo->senha, senha);               //põem em sua senha a senha passada como parâmetro
    novo->esq = NULL;                         //seu ponteiro esquerdo aponta para nulo
    novo->dir = NULL;                         //seu ponteiro direito aponta para nulo
    *aux = novo;                              //aux aponta para o novo nodo
    return a;                                 //retorna a raiz da árvore
}


pNodoA* consulta(pNodoA *a, int chave, int *compabp)    //função adquirida no moodle; adaptações: receber um ponteiro como parâmetro para contabilizar o número de comparações
{

    while (a!=NULL)                       //enquanto a não for nulo
    {
        *compabp += 1;
        if (a->user == chave )           //se o user do nodo for igual a chave passada como parâmetro
        {
            *compabp += 1;
            return a;                     //achou entao retorna o ponteiro para o elemento
        }
        else
        {
            *compabp += 1;
            if (a->user > chave)          //se o user do elemento for maior que a chave
            {
                *compabp += 1;
                a = a->esq;                //passa para o elemento da esquerda
            }
            else                           //se o user do elemento for menor ou igual que a chave
            {
                *compabp += 1;
                a = a->dir;                 //passa para o elemento da direita
            }
        }
    }
    *compabp += 1;
    return NULL;                   //se nao achou retorna nulo
}

int Verifica_senhabp(pNodoA *a, int user, char senha[20], int *compabp)
{
    int x=0;
    pNodoA *b;
    b = consulta(a, user, compabp);                    //consulta um nodo
    if (b != NULL)                                     //se ele não for nulo
    {
        *compabp += 1;
        if (strcmp(b->senha,senha) == 0)                   //compara a senha dele com a senha passado como parâmetro
        {
            *compabp += 1;
            x=1;                                          //retorna 1 se as senhas forem iguais
        }
        else
        {
            *compabp += 1;
            x = -1;                                           //retorna -1 se as senhas não forem iguais
        }
    }
    *compabp += 1;
    return x;                                         //retorna zero se não encontrar o nodo
}

void Destroiabp(pNodoA* t)   //função adquirida no moodle
{
    if(t->esq != NULL) Destroiabp(t->esq);            //se o nodo esquerdo não for nulo, destroi ele
    if(t->dir != NULL) Destroiabp(t->dir);           //se o nodo direito não for nulo, destroi ele
    free(t);                                         //libera o elemento
};

//lse:


PtNo* cria_lista(void)          //função adquirida no moodle
{
       return NULL;             //cria uma lista vazia
}

PtNo* insere_ord (PtNo* l, int dados, char senha[20])   //função adquirida no moodle; adaptações: receber uma string como parâmetro para colocá-la no novo nodo
{
       PtNo *novo; //novo elemento
       PtNo *ant = NULL; //ponteiro auxiliar para a posição anterior
       PtNo *ptaux = l; //ponteiro auxiliar para percorrer a lista

       /*aloca um novo nodo */
       novo = (PtNo*) malloc(sizeof(PtNo));

       /*insere a userrmação no novo nodo*/
       novo->user = dados;
       strcpy(novo->senha, senha);


       /*procurando a posição de inserção*/
       while (ptaux!=NULL) //se info.titulo < dados.titulo então strcmp retorna um valor menor que zero tinha: && (strcmp(ptaux->user.titulo,dados.titulo)<0)
       {
             ant = ptaux;
             ptaux = ptaux->prox;
       }

       /*encaeia o elemento*/
       if (ant == NULL) /*o anterior não existe, logo o elemento será inserido na primeira posição*/
       {
               novo->prox = l;
               l = novo;
       }
       else /*elemento inserido no final da lista*/
       {
            novo->prox = ant->prox;
            ant->prox = novo;
       }

       return l;
}



PtNo* destroi(PtNo* l)   //função adquirida no moodle
{
   PtNo *ptaux; //ponteiro auxiliar para percorrer a lista
   while (l != NULL)
   {
         ptaux = l;
         l = l->prox;           //libera o elemento e passa para o próximo
         free(ptaux);
   }
   free(l);
   return NULL;
}

int Verifica_senhalse(PtNo* l, int user, char senha[20], int *complse)
{
    int x=0;
    PtNo* aux = l;
    while (aux != NULL)                          //enquanto aux não chegar ao final
    {
        *complse += 1;
        if (aux->user == user)                   //se o user de aux for igual ao user passado como parâmetro
        {
            *complse += 1;
            if (strcmp(aux->senha, senha) == 0)          //se a senha de aux for a mesma que a passada como parâmetro
            {
                *complse += 1;
                x = 1;                                        //retorna 1
                return x;
            }
            else
            {
                *complse += 1;
                x = -1;                                       //senão, a senha não é a mesma e retorna -1
                return x;
            }
        }
        *complse += 1;
        aux = aux->prox;                            //aux passa para o próximo elemento
    }
    *complse += 1;
    return x;                                    //caso não encontre o usuário, retorna 0
}
