#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "trabfinal.h"

int main(void)
{
    float start1, start2, end1, end2;     //variáveis para pegar o clock no meio do código
    float total, total2;                  //variáveis para armazenar o tempo gasto em segundos de certos trechos de código
    pNodoA *a = NULL;                     //raiz da ABP
    PtNo *l = NULL;                       //primeiro elemento da lse
    int user, senha_corretabp=0, senha_erradabp=0, nao_existebp=0, senha_corretalse=0, senha_erradalse=0, nao_existelse=0;
    int *complse, *compabp, consulta_senhabp, consulta_senhalse, cmplse, cmpabp, escolha;
    char senha[20];
    complse = &cmplse;
    compabp = &cmpabp;
    *complse = 0;                         //ponteiros para contabilizar o número de comparações
    *compabp = 0;
    FILE *arq, *arq2;
    Gera_dados();
    printf("\nQual estrutura? \n1-ABP\n2-LSE\n");         //opção de escolha entre as estruturas
    scanf("%d", &escolha);
    if (escolha == 1)                                                //se for 1, trata da abp
    {
        if ((arq = fopen("dadosmil.txt", "r")) == NULL)                  //abre o arquivo de texto com a base de dados para leitura
        {
            printf("erro na abertura do arquivo");
        }
        start1 = clock();                                                //pega o clock antes de inserir na árvore
        while(!feof(arq))                                                //enquanto o arquivo não chegar ao fim
        {
            fscanf(arq, "%d %s\n", &user, senha);                            //lê o usuário e senha de cada linha
          //printf("\nuser: %d, senha: %s", user, senha);
            a = InsereArvore2(a, user, senha);                               //insere essas informações no novo nodo da abp
        }
        end1 = clock();                                                  //pega o clock depois da operação de inserir
        fclose(arq);                                                     //fecha o arquivo

        if ((arq2 = fopen("teste500metade.txt", "r")) == NULL)           //abre o arquivo de texto com a base de teste para leitura
        {
            printf("erro na abertura do arquivo2");
        }
        start2 = clock();                                                //pega o clock antes de consultar os testes na árvore
        while(!feof(arq2))                                               //enquanto o arquivo não chegar ao fim
        {
            fscanf(arq2, "%d %s\n", &user, senha);                           //lê o usuário e senha de cada linha
          //printf("\nuser: %d, senha: %s", user, senha);
            consulta_senhabp = Verifica_senhabp(a, user, senha, compabp);    //verifica se existe usuário com essa senha na abp
            if (consulta_senhabp == 1)
            {
                senha_corretabp++;                                           //contabiliza se a senha estiver correta
            }
            else if (consulta_senhabp == 0)
            {
                nao_existebp++;                                              //contabiliza se o usuário não existir na abp
            }
            else if (consulta_senhabp == -1)
            {
                senha_erradabp++;                                            //contabiliza se a senha estiver errada
            }
        }
        end2 = clock();                                                  //pega o clock depois da operação de consulta
        fclose(arq2);                                                    //fecha o arquivo
        total = (float)(end1 - start1) / CLOCKS_PER_SEC;                 //subtrai o tempo final do tempo inicial da operação de inserção e depois divide por clocks_per_sec para obter o tempo total
        total2 = (float)(end2 - start2) / CLOCKS_PER_SEC;                //subtrai o tempo final do tempo inicial da operação de consulta e depois divide por clocks_per_sec para obter o tempo total
        printf("\nnumero de comparacoes feita na abp: %d", cmpabp);      //exibe o número de comparações feitas
        printf("\nSenhas corretas abp: %d", senha_corretabp);            //exibe a quantidade de senhas corretas
        printf("\nSenhas erradas abp: %d", senha_erradabp);              //exibe a quantidade de senhas erradas
        printf("\nUsuarios inexistentes abp: %d", nao_existebp);         //exibe a quantidade de usuários inexistentes
        printf("\nTempo gasto inserindo nas estruturas: %f", total);     //exibe o tempo gasto inserindo
        printf("\nTempo gasto consultando nas estruturas: %f", total2);  //exibe o tempo gasto consultando

        Destroiabp(a);                                                   //destrói a abp

    }
    else if (escolha == 2)                                           //se for 2, trata da lse
    {
        if ((arq = fopen("dadoscemmilord.txt", "r")) == NULL)                  //abre o arquivo de texto com a base de dados para leitura
        {
            printf("erro na abertura do arquivo");
        }
        start1 = clock();                                                //pega o clock antes de inserir na lista
        while(!feof(arq))                                                //enquanto o arquivo não chegar ao fim
        {
            fscanf(arq, "%d %s\n", &user, senha);                            //lê o usuário e senha de cada linha
          //printf("\nuser: %d, senha: %s", user, senha);
            l = insere_ord(l, user, senha);                                  //insere essas informações no novo nodo da lse
        }
        end1 = clock();                                                  //pega o clock depois da operação de inserir
        fclose(arq);                                                     //fecha o arquivo

        if ((arq2 = fopen("teste10000metade.txt", "r")) == NULL)           //abre o arquivo de texto com a base de teste para leitura
        {
            printf("erro na abertura do arquivo2");
        }
        start2 = clock();                                                //pega o clock antes de consultas os testes na lista
        while(!feof(arq2))                                               //enquanto o arquivo não chegar ao fim
        {
            fscanf(arq2, "%d %s\n", &user, senha);                           //lê o usuário e senha de cada linha
          //printf("\nuser: %d, senha: %s", user, senha);
            consulta_senhalse = Verifica_senhalse(l, user, senha, complse);  //verifica se existe usuário com essa senha na lse
            if (consulta_senhalse == 1)
            {
                senha_corretalse++;                                          //contabiliza se a senha estiver correta
            }
            else if (consulta_senhalse == 0)
            {
                nao_existelse++;                                              //contabiliza se o usuário não existir na lse
            }
            else if (consulta_senhalse == -1)
            {
                senha_erradalse++;                                           //contabiliza se a senha estiver errada
            }
        }
        end2 = clock();                                                  //pega o clock depois da operação de consulta
        fclose(arq2);                                                    //fecha o arquivo
        total = (float)(end1 - start1) / CLOCKS_PER_SEC;                 //subtrai o tempo final do tempo inicial da operação de inserção e depois divide por clocks_per_sec para obter o tempo total
        total2 = (float)(end2 - start2) / CLOCKS_PER_SEC;                //subtrai o tempo final do tempo inicial da operação de consulta e depois divide por clocks_per_sec para obter o tempo total
        printf("\nnumero de comparacoes feita na lse: %d", cmplse);      //exibe o número de comparações feitas
        printf("\nSenhas corretas lse: %d", senha_corretalse);           //exibe a quantidade de senhas corretas
        printf("\nSenhas erradas lse: %d", senha_erradalse);             //exibe a quantidade de senhas erradas
        printf("\nUsuarios inexistentes lse: %d", nao_existelse);        //exibe a quantidade de usuários inexistentes
        printf("\nTempo gasto inserindo nas estruturas: %f", total);     //exibe o tempo gasto inserindo
        printf("\nTempo gasto consultando nas estruturas: %f", total2);  //exibe o tempo gasto consultando
        l = destroi(l);                                                  //destrói a lse
    }
    return 0;
}
