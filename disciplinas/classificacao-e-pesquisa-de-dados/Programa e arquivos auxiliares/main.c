#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filmes.h"
#include "header.h"
#include "general_functions.h"
#include "arq_invertido.h"
#include "medias.h"
#include <windows.h>

int main(void)
{
    BTreeNode *root = NULL;    //raiz da árvore B char
    BTreeNum *raiz = NULL;
    BTreeNumf *rootf = NULL;
    FILE* filmes_source = fopen("Binarios\\filmes.bin", "ab+");
    FILE* titulos_source = fopen("Binarios\\titulos.bin", "ab+");
    FILE* paises = fopen("Binarios\\feito.bin", "ab+");
    FILE* produtoras = fopen("Binarios\\produtora.bin", "ab+");
    FILE* nome_produtoras = fopen("Binarios\\company_name.bin", "ab+");
    FILE* arq_possuido = fopen("Binarios\\possuido.bin", "rb");
    FILE* arq = fopen("btree_char.bin", "ab+");
    FILE* arq2 = fopen("mais_filmes.bin", "ab+");
    FILE* arq3 = fopen("mais_titulos.bin", "ab+");
    FILE* arq4 = fopen("btree_int.bin", "ab+");
    FILE* arq5 = fopen("btree_float.bin", "ab+");
    FILE* filmes = fopen("filmes2.bin", "ab+");
    FILE* titulos = fopen("titulos2.bin", "ab+"); 
    FILE* filmes3 = fopen("filmes3.bin", "ab+");
    FILE* titulos3 = fopen("titulos3.bin", "ab+"); 



    FILE* arq_invertido_categorias = fopen("arq_invertido_generos.bin", "wb+");
    cria_e_inicializa_arq_invertido(arq_invertido_categorias);
    insere_arq_inv_caso_generos(arq_invertido_categorias, filmes, sizeof(FILME), offset_flags_and_counts, MAX_FILMES);

    FILE* more_movies;
    FILE* more_titles;
    FILME novo_filme;
    FEITO pais;
    chave key;
    chave_num key_n;
    chave_numf key_f;
    char title[256], d, novo_titulo[32];
    long arq_offset;
    int i=0, filme_offset, flagt, flagm, op, op2, op3, pag, contpag, m=0, menu, v, vetor[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // copia_arq_n(filmes_source, sizeof(FILME), filmes, 0, MAX_FILMES);
    // copia_arq_blocos(titulos_source, titulos, 0, MAX_FILMES, 32);
    // rewind(filmes_source);
    // rewind(titulos_source);
    // copia_arq_n(filmes_source, sizeof(FILME), filmes3, MAX_FILMES, 10);
    // copia_arq_blocos(titulos_source, titulos3, MAX_FILMES, 10, 32);
    rewind(filmes);
    rewind(titulos);
    int num_of_movies = conta_elementos(filmes, sizeof(FILME));
    printf("Num of movies: %d\n", num_of_movies);
    int num_of_titles = conta_clusters(titulos, 32);
    printf("Num of title: %d\n", num_of_titles);

    rewind(filmes);
    rewind(titulos);
    puts("Catalago de filmes - Absolute Cinema");
    printf("Bem-vindo ao catalogo! ");
    do                                                           //laço principal do menu do catálogo
    {
        
        system("cls");
        puts("Selecione uma das seguintes opcoes para continuar: ");
        puts("1 - Pesquisar por um titulo de filme\n2 - Filtrar os filmes de acordo com uma categoria\n3 - Ordenar todos os filmes\n4 - Inserir um arquivo com novos filmes");
        puts("5 - Consultar alguma estatistica sobre os filmes\n6 - Sair\n");
        scanf("%d", &menu);
        
        system("cls");
        switch(menu)
        {
        case 1:
            if(root == NULL)
            {

                for(int j = 0; j < num_of_movies; j++)
                {
                    filme_offset = ftell(filmes);                                                 //guarda offset do filme que sera lido
                    fread(&novo_filme, sizeof(FILME),1, filmes);                                  //le os dados do filme
                    i = 0;
                    m = 0;
                    do                                                                            //laço para obter o titulo do filme
                    {
                        (fread(title + (i * 32), sizeof(char), 32, titulos));
                        i++;
                    }
                    while(title[(i * 32) - 1] != '\0');
                    key.pos = filme_offset;                                                       //armazena offset do filme na chave
                    do                                                                            //laço para padronizar o titulo para minusculas apenas
                    {
                        title[m] = tolower(title[m]);
                        m++;
                    }
                    while(title[m] != '\0');
                    //printf("\n%s", title);
                    strcpy(key.titulo, title);                                                  //armazena titulo do filme na chave
                    //fread(&key, sizeof(chave), 1, arq);        //linha para ler somente do arquivo da árvore B de char
                    insert(&root, key);                                                                 //insere chave na arvore B
                    //fwrite(&key, sizeof(chave), 1, arq);
                    key_n.pos = filme_offset;
                    key_n.number = novo_filme.release_date;
                    insertN(&raiz, key_n);
                    key_f.number = novo_filme.popularity;
                    key_f.pos = filme_offset;
                    insertF(&rootf, key_f);
                }
            }
            rewind(titulos);
            rewind(filmes);
            do
            {printf("Escolha uma opcao:\n1 - Pesquisar o titulo de um filme\n2 - Exibir todos os filmes em ordem alfabetica\n3 - Exibir todos os filmes na ordem reversa a alfabetica\n");
                printf("4 - Voltar para o menu principal\n");
                scanf("%d", &op2);
                
                
                system("cls");
                switch(op2)
                {
                case 1:
                    while ((d = getchar()) != '\n' && d != EOF);                               //limpa buffer do teclado para ler somente o titulo digitado
                    do                                                                              //laço para ler titulos do usuario e busca-los na árvore
                    {
                        printf("Digite o titulo em ingles do filme que deseja buscar (apenas minusculas): \n");
                        if (fgets(title, 256, stdin) != NULL)                                   //adquire titulo atraves do usuario
                        {
                            title[strlen(title)-1] = '\0';
                        }
                        for(int l=0; l<10; l++)                                                    //laço de limpeza para o vetor
                        {
                            vetor[l] = 0;
                        }
                        filme_offset = busca_sucessor2(root, title, vetor);                           //busca titulo na arvore B
                        if(filme_offset > -1)                                                   //se o titulo for encontrado, exibe os dados do filme
                        {
                            fseek(filmes, filme_offset, SEEK_SET);
                            fread(&novo_filme, sizeof(FILME),1, filmes);
                            fseek(paises, novo_filme.countries_pointer, SEEK_SET);
                            fread(&pais, sizeof(FEITO), 1, paises);
                            fseek(arq_possuido, novo_filme.companies_pointer, SEEK_SET);
                            POSSUIDO possuido_aux;
                            fread(&possuido_aux, sizeof(POSSUIDO), 1, arq_possuido);
                            fseek(produtoras, possuido_aux.company_pointer, SEEK_SET);
                            PRODUTORA company;
                            fread(&company, sizeof(PRODUTORA), 1, produtoras);
                            i=0;
                            fseek(nome_produtoras, company.name_pointer, SEEK_SET);
                            do                                                                            //laço para obter o titulo da produtora
                            {
                                (fread(title + (i * 32), sizeof(char), 32, nome_produtoras));
                                i++;
                            }
                            while(title[(i * 32) - 1] != '\0');
                            printf("\nEste titulo esta na posicao %d do arquivo binario\n", filme_offset);
                            printf("\nInformacoes do filme:\nGeneros: ");
                            decod_gen(novo_filme.flags_and_counts);
                            printf("\nOrcamento: $%d\nPopularidade: %f\n", novo_filme.budget, novo_filme.popularity);
                            printf("Pais de origem: %c%c\n", pais.country_id[0], pais.country_id[1]);
                            printf("Data de lancamento: %d/%d/%d\nReceita: $%d\nDuracao: %d min\n", novo_filme.release_date % 100, (novo_filme.release_date % 10000) / 100, novo_filme.release_date / 10000, novo_filme.revenue, novo_filme.runtime);
                            printf("Media de votos: %f\nTotal de votos: %d\n", novo_filme.vote_average, novo_filme.vote_count);
                            printf("Nome da produtora: %s\n", title);
                            //printf("Pais da produtora: %c%c\n", company.country_id[0], company.country_id[1]);
                            v=0;
                            if (vetor[0] != 0)                           //se o vetor foi preenchido, exibe a mensagem
                            {
                                printf("\nExiste mais de um filme com este titulo!\nExibindo lista dos filmes\n");
                            }
                            while(v < 10 && vetor[v] != 0)                  //laço para exibir cada um dos offsets dos filmes presentes no vetor; obs: só aguenta 11 títulos iguais
                            {
                                fseek(filmes, vetor[v], SEEK_SET);
                                fread(&novo_filme, sizeof(FILME),1, filmes);
                                fseek(paises, novo_filme.countries_pointer, SEEK_SET);
                                fread(&pais, sizeof(FEITO), 1, paises);
                                fseek(arq_possuido, novo_filme.companies_pointer, SEEK_SET);
                                fread(&possuido_aux, sizeof(POSSUIDO), 1, arq_possuido);
                                fseek(produtoras, possuido_aux.company_pointer, SEEK_SET);
                                fread(&company, sizeof(PRODUTORA), 1, produtoras);
                                i=0;
                                fseek(nome_produtoras, company.name_pointer, SEEK_SET);
                                do                                                                            //laço para obter o titulo da produtora
                                {
                                    (fread(title + (i * 32), sizeof(char), 32, nome_produtoras));
                                    i++;
                                }
                                while(title[(i * 32) - 1] != '\0');
                                printf("\nEste titulo esta na posicao %d do arquivo binario\n", vetor[v]);
                                printf("\nInformacoes do filme:\nGeneros: ");
                                decod_gen(novo_filme.flags_and_counts);
                                printf("\nOrcamento: $%d\nPopularidade: %f\n", novo_filme.budget, novo_filme.popularity);
                                printf("Pais de origem: %c%c\n", pais.country_id[0], pais.country_id[1]);
                                printf("Data de lancamento: %d/%d/%d\nReceita: $%d\nDuracao: %d min\n", novo_filme.release_date % 100, (novo_filme.release_date % 10000) / 100, novo_filme.release_date / 10000, novo_filme.revenue, novo_filme.runtime);
                                printf("Media de votos: %f\nTotal de votos: %d\n", novo_filme.vote_average, novo_filme.vote_count);
                                printf("Nome da produtora: %s\n", title);
                                v++;
                            }
                        }
                        else
                        {
                            printf("\nTitulo nao encontrado\n");
                        }
                        printf("\nGostaria de pesquisar outro titulo? 0-Nao e 1-Sim\n");
                        scanf("%d", &op);
                        
                        system("cls");
                        while ((d = getchar()) != '\n' && d != EOF);                               //limpa buffer do teclado para ler somente o titulo digitado
                    }
                    while(op);
                    break;
                case 2:
                    printf("\n\nIn-order traversal of the B-tree: \n");                        //títulos imprimidos em ordem, usando uma travessia pela árvore
                    traverse(root);
                    //traverseN(raiz);
                    //traverse_titleN(raiz, filmes, titulos);
                    //traverseF(rootf);
                    //traverse_titleF(rootf, filmes, titulos);
                    printf("\n");
                    break;
                case 3:
                    traverse_backwards(root);
                    //traverse_backwardsN(raiz);
                    //traverse_backwardsF(rootf);
                    break;
                case 4:
                    break;
                default:
                    break;
                }
            }
            while(op2 != 4);
            // traverse_file(root, arq);        //linha para gerar arquivo da árvore B de char
            // traverse_fileN(raiz, arq4);
            // traverse_fileF(rootf, arq5);
            delete_Btree(root);
            root = NULL;
            delete_BtreeN(raiz);
            raiz = NULL;
            delete_BtreeF(rootf);
            rootf = NULL;
            break;
        case 2:
            filtra_categoria(arq_invertido_categorias, filmes, titulos);
            break;
        case 3: do {
        printf("Escolha um tipo de ordenamento: \n1 - Ordem alfabetica\n2 - Ordem alfabetica reversa\n3 - Mais antigos\n4 - Mais novos\n5 - Menos populares\n");
        printf("6 - Mais populares\n7 - Sair\n");
        scanf("%d", &op3);
        
        system("cls");
        switch(op3) {
            case 1:
        rewind(arq);
        contpag = 0;
            do {
                   printf("======= Pagina %d ========\n", contpag+1);
                   arq_offset = contpag * sizeof(chave) * 20;
                   fseek(arq, arq_offset, SEEK_SET);
                for(int j = 0; j < 20; j++)
                {
                    fread(&key, sizeof(chave), 1, arq);        //linha para ler somente do arquivo da árvore B de char
                    insert(&root, key);                                                           //insere chave na arvore B
                }
                traverse(root);
                printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                scanf("%d", &pag);
                if(pag == 1 && contpag > 0) {
                    contpag--;
                } else if (pag == 2 && contpag < (MAX_FILMES / 20) - 1) {
                    contpag++;
                }
                
                system("cls");
                delete_Btree(root);
                root = NULL;
            } while(pag != 3);
            break;
            case 2:
                    rewind(arq);
                contpag = (MAX_FILMES / 20) - 1;
                do 
                {
                   printf("======= Pagina %d ========\n", (MAX_FILMES / 20) - contpag);
                   arq_offset = contpag * sizeof(chave) * 20;
                   fseek(arq, arq_offset, SEEK_SET);
                   
                    for(int j = 0; j < 20; j++)
                    {
                        fread(&key, sizeof(chave), 1, arq);        //linha para ler somente do arquivo da árvore B de char
                        insert(&root, key);                                                           //insere chave na arvore B
                    }
                    traverse_backwards(root);
                    printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                    scanf("%d", &pag);
                    if(pag == 2 && contpag > 0) {
                        contpag--;
                    } else if (pag == 1 && contpag < (MAX_FILMES / 20) - 1) {
                        contpag++;
                    }
                    
                    system("cls");
                    delete_Btree(root);
                    root = NULL;
                } while(pag != 3);
                break;
            case 3: rewind(arq4);
                contpag=0;
                do {
                    printf("======= Pagina %d ========\n", contpag+1);
                    arq_offset = contpag * 20 * sizeof(chave_num);
                    fseek(arq4, arq_offset, SEEK_SET);
                    for(int l=0;l<20;l++) {
                        fread(&key_n, sizeof(chave_num), 1, arq4);
                        insertN(&raiz, key_n);
                    }
                    traverse_titleN(raiz, filmes, titulos);
                    printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                    scanf("%d", &pag);
                    if(pag == 1 && contpag > 0) {
                        contpag--;
                    } else if(pag == 2 && contpag < (MAX_FILMES / 20) - 1) {
                    contpag++;
                    }
                    
                    system("cls");
                    delete_BtreeN(raiz);
                    raiz = NULL;
                } while(pag != 3);
                break;
            case 4: rewind(arq4);
                contpag= (MAX_FILMES / 20) - 1;
                do {
                    printf("======= Pagina %d ========\n", (MAX_FILMES / 20) - contpag);
                    arq_offset = contpag * 20 * sizeof(chave_num);
                    fseek(arq4, arq_offset, SEEK_SET);
                    for(int l=0;l<20;l++) {
                        fread(&key_n, sizeof(chave_num), 1, arq4);
                        insertN(&raiz, key_n);
                    }
                    traverse_title_backwardsN(raiz, filmes, titulos);
                    printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                    scanf("%d", &pag);
                    if(pag == 2 && contpag > 0) {
                        contpag--;
                    } else if(pag == 1 && contpag < (MAX_FILMES / 20) - 1) {
                    contpag++;
                    }
                    
                    system("cls");
                    delete_BtreeN(raiz);
                    raiz = NULL;
                } while(pag != 3);
                break;
                case 5: rewind(arq5);
                contpag=0;
                do {
                    printf("======= Pagina %d ========\n", contpag+1);
                    arq_offset = contpag * 20 * sizeof(chave_numf);
                    fseek(arq5, arq_offset, SEEK_SET);
                    for(int n=0;n<20;n++) {
                        fread(&key_f, sizeof(chave_numf), 1, arq5);
                        insertF(&rootf, key_f);
                    }
                    traverse_titleF(rootf, filmes, titulos);
                    printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                    scanf("%d", &pag);
                    if(pag == 1 && contpag > 0) {
                        contpag--;
                    } else if(pag == 2 && contpag < (MAX_FILMES / 20) - 1) {
                        contpag++;
                    }
                    
                    system("cls");
                    delete_BtreeF(rootf);
                    rootf = NULL;
                } while(pag != 3);
                    break;
                case 6: rewind(arq5);
                contpag= (MAX_FILMES / 20) - 1;
                do {
                    printf("======= Pagina %d ========\n", (MAX_FILMES / 20) - contpag);
                    arq_offset = contpag * 20 * sizeof(chave_numf);
                    fseek(arq5, arq_offset, SEEK_SET);
                    for(int n=0;n<20;n++) {
                        fread(&key_f, sizeof(chave_numf), 1, arq5);
                        insertF(&rootf, key_f);
                    }
                    traverse_title_backwardsF(rootf, filmes, titulos);
                    printf("\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                    scanf("%d", &pag);
                    if(pag == 2 && contpag > 0) {
                        contpag--;
                    } else if(pag == 1 && contpag < (MAX_FILMES / 20) - 1) {
                        contpag++;
                    }
                    
                    system("cls");
                    delete_BtreeF(rootf);
                    rootf = NULL;
                } while(pag != 3);
                    break;
                case 7: break;
                default: break;
        }
        } while (op3 != 7);
            break;
        case 4: delete_Btree(root);                                                        //deleta quaisquer árvores existentes
        root = NULL;
        delete_BtreeF(rootf);
        rootf = NULL;
        delete_BtreeN(raiz);
        raiz = NULL;
        flagm=0;
        flagt=0;
        while ((d = getchar()) != '\n' && d != EOF);                                            //limpa buffer do teclado
        printf("Digite o nome do arquivo de filmes que deseja inserir na base de dados original: \n");
                        if (fgets(title, 256, stdin) != NULL)                                   //adquire titulo atraves do usuario
                        {
                            title[strlen(title)-1] = '\0';
                        }
                        more_movies = fopen(title, "rb");                                        //abre arquivo do usuário
                        if(more_movies == NULL) {
                            printf("\nErro ao abrir o arquivo de filmes!\n");                    //avisa se houver erro na abertura
                        } else {
                            while(fread(&novo_filme, sizeof(FILME), 1, more_movies) == 1) 
                            {      //transfere todo conteúdo do arquivo do usuário para o arquivo base
                                fwrite(&novo_filme, sizeof(FILME), 1, filmes);
                                num_of_movies++;
                            }
                            printf("\nArquivo de filmes adicionado com sucesso!\n");             //exibe mensagem de sucesso
                            flagm=1;
                        }
        printf("\nDigite o nome do arquivo de titulos que deseja inserir: \n");
                        if (fgets(title, 256, stdin) != NULL)                                   //adquire titulo atraves do usuario
                        {
                            title[strlen(title)-1] = '\0';
                        }
                        more_titles = fopen(title, "rb");                                        //abre arquivo do usuário
                        if(more_titles == NULL) {
                            printf("\nErro ao abrir o arquivo de titulos!\n");                    //avisa se houver erro na abertura
                        } else {
                            while(fread(&novo_titulo, sizeof(char), 32, more_titles) == 32) 
                            {     //ou > 0     transfere todo conteúdo do arquivo do usuário para o arquivo base
                                fwrite(&novo_titulo, sizeof(char), 32, titulos);
                            }
                            printf("\nArquivo de titulos adicionado com sucesso!\n");             //exibe mensagem de sucesso
                            flagt=1;
                        }
                        if(flagm && flagt) 
                        {
                            rewind(titulos);
                            rewind(filmes);
                            fclose(arq);
                            fclose(arq4);
                            fclose(arq5);
                            for(int k=0;k<MAX_FILMES;k++) {
                                filme_offset = ftell(filmes);                                                 //guarda offset do filme que sera lido
                                fread(&novo_filme, sizeof(FILME),1, filmes);                                  //le os dados do filme
                                i = 0;
                                m = 0;
                                do                                                                            //laço para obter o titulo do filme
                                {
                                    (fread(title + (i * 32), sizeof(char), 32, titulos));
                                    i++;
                                }
                                while(title[(i * 32) - 1] != '\0');
                                key.pos = filme_offset;                                                       //armazena offset do filme na chave
                                do                                                                            //laço para padronizar o titulo para minusculas apenas
                                {
                                    title[m] = tolower(title[m]);
                                    m++;
                                }
                                while(title[m] != '\0');
                                strcpy(key.titulo, title);                                                    //armazena titulo do filme na chave
                                insert(&root, key);                                                           //insere chave na arvore B
                                key_n.pos = filme_offset;
                                key_n.number = novo_filme.release_date;
                                insertN(&raiz, key_n);
                                key_f.number = novo_filme.popularity;
                                key_f.pos = filme_offset;
                                insertF(&rootf, key_f);
                            }
                            arq = fopen("btree_char.bin", "ab+");
                            arq4 = fopen("btree_int.bin", "ab+");
                            arq5 = fopen("btree_float.bin", "ab+");
                            traverse_file(root, arq);
                            traverse_fileN(raiz, arq4);
                            traverse_fileF(rootf, arq5);
                            delete_Btree(root);
                            root = NULL;
                            delete_BtreeN(raiz);
                            raiz = NULL;
                            delete_BtreeF(rootf);
                            rootf = NULL;
                        }

            break;
        case 5:
            do
            {
                printf("Media de contagem de votos: %lf\n", medias_i(filmes, offset_vote_count));
                printf("Media de votos: %lf\n", medias_f(filmes, offset_vote_average));
                printf("Media de tempo de um filme: %lf\n", medias_i(filmes, offset_runtime));
                printf("Media de orcamento: %lf\n", medias_i(filmes, offset_budget));
                printf("Media de receita: %lf\n", medias_i(filmes, offset_revenue));
                printf("Media de popularidade: %lf\n", medias_f(filmes, offset_popularity));
                printf("\nDigite 0 para sair\n");
                int op;
                scanf("%d", &op);
            } while (op != 0);
            break;
        case 6:
            puts("Obrigado por utilizar nosso catalogo! Ate a proxima :)");
            break;
        default:
            break;
        }
    }
    while(menu != 6);
    /*delete_Btree(root);
    root = NULL;
        delete_BtreeF(rootf);
        rootf = NULL;
        delete_BtreeN(raiz);
        raiz = NULL;*/
    fclose(filmes);
    fclose(titulos);
    fclose(paises);
    //fclose(produtoras);
    //fclose(nome_produtoras);
    fclose(arq);
    fclose(arq2);
    fclose(arq3);
    //fclose(more_movies);
    //fclose(more_titles);

    return 0;
}