#include "arq_invertido.h"
#include <time.h>
#include "general_functions.h"
#include "header.h"
#include "filmes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cluster0_pointer = sizeof(BOOT_INFO) + FAT_TAM;
int FAT_pointer = sizeof(BOOT_INFO);
/*
Opção1: 
Fazer uma função que serve para os três casos produtoras, paises e gêneros
Para os casos das produtoras e paises
    leia-se os respectivos arquivos possuido e feito. 
    Para cada elemento da tabela de relação 
        Usando a função get_field_array pega-se os campos de interesse, movie_pointer e id_da_categoria
        chama a função insere_arq_invertido
Para o caso dos gêneros
    leia-se o arquivo de filmes.bin. 
    Para cada elemento
        Guarde o movie_pointer
        Usa a função extract_genres
            Para cada gênero no array obtido atrvés da extract_genres
                Chama a função insere_arq_invertido
*/
/*Para os casos das produtoras e paises
    leia-se os respectivos arquivos possuido e feito. 
    Para cada elemento da tabela de relação 
        Usando a função get_field_array pega-se os campos de interesse, movie_pointer e id_da_categoria
        chama a função insere_arq_invertido*/

void insere_arq_inv_caso1(FILE* arq_invertido, FILE* source, int bytes_tam_of_struct, int offset_movie_pointer, int offset_category_id, int inicio, int quant)
{
    rewind(source);
    int count = 0;
    char* elemento = (char*)malloc(bytes_tam_of_struct); //Aloca receptor geral da estrutura 
    int *movie_pointer, *category_id;
    fseek(source, inicio * sizeof(POSSUIDO), SEEK_SET);
    while(fread(elemento, bytes_tam_of_struct, 1, source) == 1)
    {
        movie_pointer = (int*)get_field_array(elemento, 1, bytes_tam_of_struct, offset_movie_pointer, sizeof(int));
        category_id = (int*)get_field_array(elemento, 1, bytes_tam_of_struct, offset_category_id, sizeof(int));
        insere_arq_invertido(arq_invertido, *movie_pointer, *category_id);
        fflush(arq_invertido);
        count++;
        if(count >= quant)
            break;
        
        /*if(count % 1000 == 0)
        {
            printf("\nElementos analisados: %d", count); 
            
        }*/
    }
    

}
void insere_arq_inv_caso_generos(FILE* arq_invertido, FILE* source, int bytes_tam_of_struct, int offset_category_id, int quant)
{
    rewind(source);
    char* elemento = (char*)malloc(bytes_tam_of_struct);
    //FILME* filme_aux = NULL;
    int movie_pointer, *generos;
    int tam_of_array_genres;
    int  i = 0;
    while(true)
    {
        i++;
        movie_pointer = ftell(source);
        if(fread(elemento, bytes_tam_of_struct, 1, source) != 1)
        {
            break;
        }
        //filme_aux = (FILME*)elemento;
        //printf("%d\n", filme_aux->id);

        //Pega flags_and_counts
        generos = (int*)get_field_array(elemento, 1, bytes_tam_of_struct, offset_category_id, sizeof(int));
        //Usa a função que cria um array com os gêneros;
        generos = extract_genres(*generos, &tam_of_array_genres);
        //Para cada elemento do array chama a funçaõ insere
        for(int j = 0; j < tam_of_array_genres; j++)
        {
            //const char* genero = nome_genero(generos[j]);
            //printf("Genero: %s\n", genero);
            insere_arq_invertido(arq_invertido, movie_pointer, generos[j]);
            fflush(arq_invertido);
        }
        if(i >= quant)
        {
            break;
        }
        if(i % 1000 == 0)
        {
            printf("%d inseridos\n", i);
        }
        
    }
    
}
//Função básica que insere um único elemento no arquivo invertido
void insere_arq_invertido(FILE* arq_invertido, int movie_pointer, int id_da_categoria)
{
    //clock_t inicio, fim, inicio_2;
    //inicio = clock();
    //Inserir um novo ponteiro de filme:
        // Consulta-se o bloco do diretório raiz
        // Busca o id_da_categoria que corresponde ao id associado ao filme, se não houver inserir um nova categoria
        ARQ_INFO arq_categoria;
        //inicio_2 = clock();
        int endereco_arq_info = busca_id_categoria(arq_invertido, id_da_categoria);
        //fim = clock();
        //printf("\nBuscar: %.5f ", ((double)(fim - inicio_2)) / CLOCKS_PER_SEC);
        if (endereco_arq_info == 0)
        {
            //inicio_2 = clock();
            arq_categoria.id_da_categoria = id_da_categoria;
            endereco_arq_info = insere_nova_categoria(arq_invertido, &arq_categoria);

            //fim = clock();
            //printf("\nNova categoria: %.5f ", ((double)(fim - inicio_2)) / CLOCKS_PER_SEC);
        }else if(endereco_arq_info >= cluster0_pointer)
        {
            fseek(arq_invertido, endereco_arq_info, SEEK_SET); //Vai para a posição da categoria
            fread(&arq_categoria, sizeof(ARQ_INFO), 1, arq_invertido);
            //printf("Categoria encontrada é: %s\n", nome_genero(arq_categoria.id_da_categoria));
        }else
        {
            printf("\nErro no insere_arq_invertido: tentando usar um endereco que nao pertence a um bloco, %d  ", endereco_arq_info);
            exit(EXIT_FAILURE);
        }
        
        // Verifica se o bloco atual tem espaço para mais um elemento, atraves da equação (byte_final - size(boot) - size(FAT)) % 256),
        // Se o resultado da equação for 0 significa que o bloco encheu e que um novo bloco deve ser alocado
        if(arq_categoria.bytes_escrito_no_bloco_final >= 256)
        {
            //     Procura na tabela FAT qual o primeiro bloco livre, marca como final de um encadeamento
                //     Verifica o encadeamento na FAT a partir do bloco inicial informado no elemento do diretório 
                //     Atualiza o encadeamento.
            //inicio_2 = clock();
            arq_categoria.bloco_final = expande_encadeamento_FAT(arq_invertido, arq_categoria.bloco_inicial);
            arq_categoria.bytes_escrito_no_bloco_final = 0;
            //fim = clock();
            //printf("\nExpande categoria: %.5f ", ((double)(fim - inicio_2)) / CLOCKS_PER_SEC);

        }
        //Vai para onde é para colocar o novo movie_pointer
        int movie_pointer_position =  calcula_endereco_de_bloco(arq_categoria.bloco_final) + arq_categoria.bytes_escrito_no_bloco_final;
        fseek(arq_invertido, movie_pointer_position, SEEK_SET);
        /*if(ftell(arq_invertido) < cluster0_pointer)
        {
            printf("\nErro no inserir_arq_invertido: arq_categoria.final_byte nao esta em um bloco %li", ftell(arq_invertido));
            exit(EXIT_FAILURE);
        }*/
        fwrite(&movie_pointer, sizeof(int), 1, arq_invertido); //     Escreve o novo ponteiro

        //     Atualiza quantidade de bytes escritos no ultimo no bloco final do encadeamento da categoria
        arq_categoria.bytes_escrito_no_bloco_final += sizeof(movie_pointer);
        /*if(ftell(arq_invertido) < cluster0_pointer)
        {
            printf("\nErro no inserir_arq_invertido: arq_categoria.self_pointer nao esta em um bloco %li", ftell(arq_invertido));
            exit(EXIT_FAILURE);
        }*/
        fseek(arq_invertido, endereco_arq_info, SEEK_SET);
        //printf("Posicao onde vai ser escrita as infos da categoria: %d\n", ftell(arq_invertido));
        fwrite(&arq_categoria, sizeof(ARQ_INFO), 1, arq_invertido);


        
        //fim = clock();
        //double tempo_decorrido = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        //printf("Inserir: %.5f\n", tempo_decorrido);

        fflush(arq_invertido);
 
}

//Completa a nova categoria passa em info
//Retorna a posisção do arquivo em que essa nova categoria foi armazenada
int insere_nova_categoria(FILE* arq_invertido, ARQ_INFO* info)
{
    fflush(arq_invertido);
    //printf("insere_nova_categoria\n");
    // Inserir uma nova categoria:
    BOOT_INFO boot;
    rewind(arq_invertido);
    
    if(fread(&boot, sizeof(BOOT_INFO), 1, arq_invertido) != 1) //Le as informações de boot
    {
        printf("\nErro insere_nova_Categoria: erro ao ler informacoes de boot");
        exit(EXIT_FAILURE);
    }/*else if(boot.root_offset == 0)
    {
        printf("\nErro insere_nova_Categoria: informacoes de boot foram lidas mas a erro nos dados");
        exit(EXIT_FAILURE);    
    }*/
    // Verifica se ainda há espaço no bloco do diretório raiz  e testa o caso inicial em que se está no começo do cluster0
    //printf("first_unused_cluster: %d\n", boot.first_unused_cluster); 
    if(boot.bytes_escritos_in_root_final_cluster >= 256)
    {   
        //Expande o diretório raiz
        int novo_bloco_raiz = expande_encadeamento_FAT(arq_invertido, boot.root_final_cluster); //
        //Atualiza as informações de boot: atualiza bloco final do diretório root e root_final_byte
        rewind(arq_invertido);
        fread(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
        boot.root_final_cluster = novo_bloco_raiz;
        boot.bytes_escritos_in_root_final_cluster = 0; //Começa em 0 pois nenhum byte foi escrito
        rewind(arq_invertido);
        fwrite(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
        fflush(arq_invertido);

    }

    //Usa informações de boot para saber onde as informações da nova categoria deve ser escrita
    
    info->bloco_inicial = aloca_e_atualiza_FAT(arq_invertido);
    //printf("bloco alocado: %d\n", info->bloco_inicial);
    info->bloco_final = info->bloco_inicial;
    info->bytes_escrito_no_bloco_final = 0; 
    rewind(arq_invertido);  //Informações de boot foram alteradas com a chamada de aloca_e_atualiza_FAT
    fread(&boot, sizeof(BOOT_INFO), 1, arq_invertido); //Releitura de boot para manter as informações atualizadas
    //printf("boot.root_final_cluster: %d boot.bytes_escritos_in_root_final_cluster:%d\n", boot.root_final_cluster, boot.bytes_escritos_in_root_final_cluster);
    int pos_nova_categoria = calcula_endereco_de_bloco(boot.root_final_cluster) + boot.bytes_escritos_in_root_final_cluster;
    /*if(pos_nova_categoria < cluster0_pointer)
    {
        printf("\nErro ao inserir nova categoria: Self pointer nao esta em um bloco");
        exit(EXIT_FAILURE);
    }
    fseek(arq_invertido, pos_nova_categoria, SEEK_SET); //Vai para onde as informações do subarquivo devem ser escritas no diretório root
    if(fwrite(info, sizeof(ARQ_INFO), 1, arq_invertido) != 1) //Escreve nova entrada de subarquivo
    {
        printf("Erro ao escrever as informações da nova categoria\n");
        exit(EXIT_FAILURE);
    }*/
    
    //Atualiza informações de boot
    boot.bytes_escritos_in_root_final_cluster += sizeof(ARQ_INFO);
    if(boot.bytes_escritos_in_root_final_cluster > 256)
    {
        printf("\nErro ao inserir nova categoria: Estouro de bloco");
        exit(EXIT_FAILURE);
    }
    rewind(arq_invertido);
    fwrite(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
    fflush(arq_invertido);

    
    return pos_nova_categoria;
    
    // Verifica se ainda há espaço no bloco do diretório raiz
    // Se não 
    //     Expande o diretório raiz
    //     Aloca mais um bloco para o diretório raiz
    //     Atualiza encadeamento na FAT
    // Aloca um bloco para essa nova categoria, Atualiza FAT
    // Escreve as informações desta categoria no diretório raiz

}

//Percorre o diretório root e seus encadeamentos procurando o id de categoria dado,
//Retorna o endereço o endereço das informações do arquivo no diretório root
//Se não achar retorna 0
int busca_id_categoria(FILE* arq_invertido, int id)
{
    fflush(arq_invertido);   
    int bloco = 0;
    ARQ_INFO info_categoria;
    int quant = 0;
    int guarda_pos;
    //Enquanto houver encadeamentos
    do
    {
        fseek(arq_invertido, calcula_endereco_de_bloco(bloco), SEEK_SET); //Na primeira vez começa no primeiro byte do diretório raiz, depois segue o diretório raiz
        //Enquanto não encontrar o id ou chegar ao fim dos blocos
        quant = 0;
        do
        {
            guarda_pos = ftell(arq_invertido);
            if(fread(&info_categoria, sizeof(ARQ_INFO), 1, arq_invertido) != 1) //Le as informações do subarquivo de uma categoria
            {
                printf("Erro ao ler as informações de uma categoria\n");
                exit(EXIT_FAILURE);
            }
            quant += (int)CATEGORY_INFO_TAM;   //Guarda a proxima posição a ser lida para testar se não vai sair do bloco
        
        } while(info_categoria.id_da_categoria != id && quant < 256 && info_categoria.id_da_categoria != 0); 
        if(info_categoria.id_da_categoria == id) 
        {
            return guarda_pos;
        }
        else if(info_categoria.id_da_categoria == 0)
        {
            return 0;
        }      
        bloco = encadeamento_continua(arq_invertido, bloco, ftell(arq_invertido));
    } while (bloco);

    return 0;

}


//Função que cria um array com os ids dos gêneros de um filme
int* extract_genres(int generos, int* tam_of_array)
{
    int byte;
    int* generos_array = NULL;
    int size_generos = 0;
    //Dos bits 0-18
    for(int i = 0; i < 18; i++)
    {
        //Isola o bit com uma máscara
        byte = generos & (int)pow(2, i);
        //Se o bit for 1 
        if(byte != 0)
        {
            //Coloaca o bit*sua_posicao em um novo elemento do array
            generos_array = (int*)realloc(generos_array, (size_generos + 1) * sizeof(int));
            size_generos++;
            generos_array[size_generos - 1] = byte;
        }        
    }
    *tam_of_array = size_generos;
    return generos_array;
}

int atualiza_boot_infos(FILE* arq_invertido, BOOT_INFO boot)
{
    rewind(arq_invertido);
    fwrite(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
    return 0;
}

//Recebe o número de um bloco
//Retorna qual o endereço deste bloco
int calcula_endereco_de_bloco(int bloco)
{
    /*if(bloco < 0)
    {
        printf("\nErro: calcula_endereco_de_bloco: Bloco nao pode ser negativo");
        exit(EXIT_FAILURE);
    }*/
    int cluster_pointer = cluster0_pointer + (bloco * CLUSTER_TAM);
    /*if(cluster_pointer < cluster0_pointer)
    {
        printf("\nErro: calcula_endereco_de_bloco: Endereco do bloco deu um resultado inesperado");
        exit(EXIT_FAILURE);
    }*/
    return cluster_pointer;
}

//Aloca um novo bloco e atualiza tabela FAT, retorna número do bloco alocado
int aloca_e_atualiza_FAT(FILE* arq_invertido)
{
    int valor;
    BOOT_INFO boot;
    //Usar as informações de boot
    rewind(arq_invertido);
    fread(&boot, sizeof(BOOT_INFO), 1, arq_invertido); //Le as informações de boot, para usar boot.first_unused_cluster
    
    fseek(arq_invertido, element_pointer_in_FAT(boot.first_unused_cluster), SEEK_SET); //Vai para o bloco livre
    int bloco_alocado = boot.first_unused_cluster;
    valor = FIM_DO_ENCADEAMENTO;
    /*if(ftell(arq_invertido) < FAT_pointer || ftell(arq_invertido) > cluster0_pointer)
    {
        printf("\nErro no aloca_e_atualiza_FAT: tentando escrever fora da FAT %li", ftell(arq_invertido));
        exit(EXIT_FAILURE);
    }*/
    fwrite(&valor, sizeof(int), 1, arq_invertido); //Marca o bloco como sendo usado

    fseek(arq_invertido, calcula_endereco_de_bloco(bloco_alocado), SEEK_SET); //Vai até o bloco alocado
    char zera[256] = {};
    fwrite(zera, sizeof(char), 256, arq_invertido); //Zera o bloco

    //Atualiza informações de boot
    boot.first_unused_cluster++;
    rewind(arq_invertido);
    fwrite(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
    fflush(arq_invertido);
    return bloco_alocado; //Retorna qual o bloco na tabela FAT foi alocado
        
}

//Essa função calcula a quantidade de bytes dentro de um bloco através do offeset da posição dada dentro de um bloco
//Recebe endereço do bloco + offset_dentro_do_bloco
int offset_no_bloco(int pos)
{
    /*if(pos < cluster0_pointer)
    {
        printf("\nErro na offset_no_bloco: posicao dada nao pertence a um bloco, %d", pos);
        exit(EXIT_FAILURE);
    }*/
    int quant = (pos - sizeof(BOOT_INFO) - FAT_TAM) % 256;
    return quant;
}

//Retorna numero do bloco alocado
int expande_encadeamento_FAT(FILE* arq_invertido, int bloco)
{
    int clutser_pointer;
    int valor = bloco;
    //Percorre o encadeamento na FAT começando em bloco;
    do
    {
        bloco = valor;
        fseek(arq_invertido, element_pointer_in_FAT(bloco), SEEK_SET);
        clutser_pointer = ftell(arq_invertido);
        fread(&valor, sizeof(int), 1, arq_invertido);
        /*if(valor == 0)
        {
            printf("\nErro ao expandir encadeamento: bloco não alocado");
            exit(EXIT_FAILURE);
        }*/

    } while (valor != FIM_DO_ENCADEAMENTO);
    
    //Quando achar o bloco final 
        //Procura um bloco vazio e expande o encadeamento 
    int new_cluster = aloca_e_atualiza_FAT(arq_invertido);
    /*if(bloco == new_cluster)
    {
        printf("\nErro na expande_encadeamento: erro no novo bloco da cadeia, %d", bloco);
        exit(EXIT_FAILURE);
    }*/
    //Atualiza encadeamento
    fseek(arq_invertido, clutser_pointer, SEEK_SET);
    fwrite(&new_cluster, sizeof(int), 1, arq_invertido);
    fflush(arq_invertido);
    
    return new_cluster;

}

//Testa se o bloco informado tem ao menos um a sequencia 
//Mantem o leitor no mesmo lugar
//Retorna o número do bloco em que continua o encadeamento
int encadeamento_continua(FILE* arq_invertido, int bloco, int pos_atual)
{
    int valor;
    int cluster_pointer = element_pointer_in_FAT(bloco);
    fseek(arq_invertido, cluster_pointer, SEEK_SET);
    fread(&valor, sizeof(int), 1, arq_invertido);
    fseek(arq_invertido, pos_atual, SEEK_SET);
    if(valor == FIM_DO_ENCADEAMENTO)
    {
        return 0;

    }/*else if(valor == 0)
    {
        printf("\nErro na encadeamento_continua: Bloco nao alocado");
        exit(EXIT_FAILURE);
    }*/else
    {
        return valor;
    }
    
}

int bloco_atual_dado_endereco(int pos)
{
    /*if(pos < cluster0_pointer)
    {
        printf("\nErro bloco_atual_dado_endereco: Endereco nao eh de um bloco");
        exit(EXIT_FAILURE);
    }*/
    int bloco = (pos - sizeof(BOOT_INFO) - FAT_TAM)/256;
    return bloco;
}

//Recebe um número de bloco 
//Retorna qual o endereço da posição associada ao bloco na tabela FAT
int element_pointer_in_FAT(int bloco)
{
    /*if(bloco < 0)
    {
        printf("\nErro na element_pointer_in_FAT: bloco nao pode ser negativo, %d", bloco);
        exit(EXIT_FAILURE);
    }*/
    int pointer_cluster_in_FAT = FAT_pointer + (bloco * FAT_ELEMENT_TAM);
    /*if(pointer_cluster_in_FAT < FAT_pointer || pointer_cluster_in_FAT >= cluster0_pointer)
    {
        printf("\nErro na element_pointer_in_FAT: resultado inesperado, %d", pointer_cluster_in_FAT);
        exit(EXIT_FAILURE);
    }*/
    return pointer_cluster_in_FAT;
}

int cluster_number_of_element_FAT(int pos)
{
    /*if(pos < FAT_pointer || pos > cluster0_pointer)
    {
        printf("\nErro cluster_number_of_element_FAT: posicao dada nao pertence a um elemento da tabela FAT");
        exit(EXIT_FAILURE);
    }*/
    int number = (pos - FAT_pointer)/FAT_ELEMENT_TAM;
    return number;
}

void cria_e_inicializa_arq_invertido(FILE* arq_invertido)
{
    BOOT_INFO boot;
    boot.root_offset = sizeof(BOOT_INFO) + FAT_TAM;
    boot.bytes_escritos_in_root_final_cluster = 0; //Inicia em 0 pois nenhuma informação foi escrita ainda
    boot.FAT_offset = sizeof(BOOT_INFO);
    boot.FAT_tam = FAT_TAM;
    boot.FAT_elements_tam = FAT_ELEMENT_TAM;
    boot.first_unused_cluster = 1;
    boot.root_final_cluster = bloco_atual_dado_endereco(boot.root_offset);
    /*if(*/fwrite(&boot, sizeof(BOOT_INFO), 1, arq_invertido); //!= 1);
    /*{
        printf("\nErro ao gravar informacoes de boot");
        exit(EXIT_FAILURE);
    }*/ 
    fseek(arq_invertido, FAT_pointer, SEEK_SET); //Vai para a tabela FAT
    int sinal = FIM_DO_ENCADEAMENTO;
    fwrite(&sinal, sizeof(int), 1, arq_invertido); //Marca o primeiro cluster como utilizado, está senod utilizado pelo diretório root

    fseek(arq_invertido, cluster0_pointer, SEEK_SET); //Garante a existencia do primeiro bloco
    char zera[256] = {};
    fwrite(zera, sizeof(char), 256, arq_invertido);
    fflush(arq_invertido);
    rewind(arq_invertido);
}

//Escreve um arquivo invertido no FILE destino, se quiser printar no terminal use stdout
void imprime_arquivo_invertido(FILE* arq_invertido, FILE* destino, FILE* filmes, FILE* titulos)
{
    //Percorre cada arquivo no diretório root
    BOOT_INFO boot;
    ARQ_INFO arq_categoria;
    rewind(arq_invertido);
    int movie_pointer;
    FILME filme;
    char title[256];
    int pos_categoria;
    int pos_root = 0;
    int pos_info_categoria;
    int final_position_of_category;
    fread(&boot, sizeof(BOOT_INFO), 1, arq_invertido);
    do //Enquanto houver encadeamento no diretório root
    {
        int j = 0;
        fseek(arq_invertido, calcula_endereco_de_bloco(pos_root), SEEK_SET);
        do//Enquanto houver categorias
        {            
            fread(&arq_categoria, sizeof(ARQ_INFO), 1, arq_invertido);
            if(arq_categoria.bloco_inicial == 0)
            {
                return; //Encerra Todas as categorias foram percorridas
            }
            pos_info_categoria = ftell(arq_invertido);
            pos_categoria = arq_categoria.bloco_inicial;
            final_position_of_category = calcula_endereco_de_bloco(arq_categoria.bloco_final) + arq_categoria.bytes_escrito_no_bloco_final;
            fprintf(destino, "\n******************************\n");
            fprintf(destino, "%d: ", arq_categoria.id_da_categoria);
            do //Enquanto a categoria tiver encadeamento
            {
                fseek(arq_invertido, calcula_endereco_de_bloco(pos_categoria), SEEK_SET);
                int  i = 0;
                do//Enquanto houver ponteiros
                {
                    fread(&movie_pointer, sizeof(int), 1, arq_invertido);
                    i++;
                    fseek(filmes, movie_pointer, SEEK_SET);
                    fread(&filme, sizeof(FILME), 1, filmes);
                    fseek(titulos, filme.title_pointer, SEEK_SET);
                    int a = 0;
                    do            //laço para obter o titulo do filme
                    {
                        (fread(title + (a * 32), sizeof(char), 32, titulos));
                        a++;
                    }
                    while(title[(a * 32) - 1] != '\0');
                    fprintf(destino, "%s, ", title);
                    if(i >= ELEMENTS_PER_CLUSTER)
                    {
                        break;
                    }
                    if(ftell(arq_invertido) == final_position_of_category)
                    {
                        break;
                    }
                    

                } while (true);
                if(movie_pointer != 0) //Se terminou o bloco e não haviam acabado os ponteiros
                {
                    //Verifica se o encadeamento continua
                    pos_categoria = encadeamento_continua(arq_invertido, pos_categoria, ftell(arq_invertido));
                }else
                {
                    pos_categoria = 0; //Encerra a leitura da categoria pois já foi terminada
                }
                
            } while (pos_categoria);
            fseek(arq_invertido, pos_info_categoria, SEEK_SET);
            j++;

        } while (j < (int)CLUSTER_TAM/(int)sizeof(ARQ_INFO));
        pos_root = encadeamento_continua(arq_invertido, bloco_atual_dado_endereco(ftell(arq_invertido)) - 1, ftell(arq_invertido));
    } while (pos_root);
    
        //Percorre cada os blocos de cada subarquivo
}

//Recebe o id de uma categoria e retorna o n-ésimo filme da categoria, n é dado por indice
int retorna_filme_indice(FILE* arq_invertido, int id_da_categoria, int indice)
{
    //Esta função recebe uma categoria e o indice do filme desejado
    //Ou seja se o indice for 10 será retornado o 10° filme da categoria dada

    //Usa a função busca_id_categoria para encontrar o endereço das informações do subarquivo da categoria 
    int endereco_do_arquivo = busca_id_categoria(arq_invertido, id_da_categoria); 

    //Lê as informações do subarquivo da categoria
    fseek(arq_invertido, endereco_do_arquivo, SEEK_SET);
    ARQ_INFO categoria;
    fread(&categoria, sizeof(ARQ_INFO), 1, arq_invertido);

    //Se o indice for maior do que cabe em um bloco percorre os blocos até encontrar o bloco que contém o indice desejado
    int bloco = categoria.bloco_inicial;
    int blocos_percorridos = 0;
    while((blocos_percorridos + 1)*64 < indice)
    {
        bloco = encadeamento_continua(arq_invertido, bloco, ftell(arq_invertido));
        if(!bloco)
        {
            printf("Fim dos filmes desta categoria!\n");
            return 0;
        }
        blocos_percorridos++;
    }
    indice -= blocos_percorridos*64; //Ajusta o indice para inidice dentro do bloco

    //Pega o ponteiro do filme
    fseek(arq_invertido, calcula_endereco_de_bloco(bloco) + indice * sizeof(int), SEEK_SET);
    int movie_pointer;
    fread(&movie_pointer, sizeof(int), 1, arq_invertido);

    return movie_pointer;
    
}

int retorna_id_genero()
{
    system("cls");
    printf("======= Opcoes de Genero =======\n");
    printf(" 1 - Action\n");
    printf(" 2 - Adventure\n");
    printf(" 3 - Animation\n");
    printf(" 4 - Comedy\n");
    printf(" 5 - Crime\n");
    printf(" 6 - Documentary\n");
    printf(" 7 - Drama\n");
    printf(" 8 - Family\n");
    printf(" 9 - Fantasy\n");
    printf("10 - History\n");
    printf("11 - Horror\n");
    printf("12 - Music\n");
    printf("13 - Mystery\n");
    printf("14 - Romance\n");
    printf("15 - Science Fiction\n");
    printf("16 - Thriller\n");
    printf("17 - TV Movie\n");
    printf("18 - War\n");
    printf("19 - Western\n");
    printf("0 - Sair\n");
    printf("================================\n");

    int genero;
    scanf("%d", &genero);
    if(genero == 0)
    return genero;

    int genres_number = 1 << (genero - 1);
    
    return genres_number;
}

void filtra_categoria(FILE* arq_invertido, FILE* arq_estrutura, FILE* arq_name)
{
    int id_da_categoria;
    do
    {
        id_da_categoria = retorna_id_genero();
        if(id_da_categoria == 0)
        {
            break;
        }
        printa_arq_invertido_paginado(arq_invertido, arq_estrutura, arq_name, id_da_categoria, sizeof(FILME));


    } while (true);   


}


void printa_arq_invertido_paginado(FILE* arq_invertido, FILE* arq_estrutura, FILE* arq_name, int id_da_categoria, int tam_of_struct)
{
    
    BTreeNode* root = NULL;
    chave key;
    int pag;
    int acabou = 0;
        int contpag = 0;
        do {
                system("cls");
                fprintf(stdout, "======= Pagina %d ========\n", contpag+1);
                
                for(int j = 0; j < 20; j++)
                {
                    key.pos = retorna_filme_indice(arq_invertido, id_da_categoria, j + (contpag * 20));  
                    if(!key.pos)
                    {
                        acabou = 1;
                        break;
                    } 
                    recebe_estrutura_retorna_nome(arq_name, arq_estrutura, key.pos, tam_of_struct, offset_title_pointer, key.titulo);
                    insert(&root, key);                                                       
                }
                traverse(root);
                if(acabou)
                {
                    printf("Nao ha mais filmes desta categoria");
                }
                fprintf(stdout, "\n1 - Voltar | 2 - Avancar | 3 - Sair: \n");
                scanf("%d", &pag);
                if(pag == 1 && contpag > 0) {
                    acabou = 0;
                    contpag--;
                } else if (pag == 2 && contpag < (MAX_FILMES / 20) - 1 && !acabou) {
                    contpag++;
                }
                delete_Btree(root);
                root = NULL;
            } while(pag != 3);
}





/*
Sistema FAT, Cada arquivo invertido vai ter:

Boot: 
    Bloco que contém o diretório raiz
    Onde começa a tabela FAT
    Tamanho dos blocos
    Tamanho da tabela FAT
    Tamanho dos elementos da tabela FAT

Tabela FAT:
    Elementos: Cada elemento da tabela será um par {bloco, valor}, onde valor pode ser:
        0 - significa que o bloco está livre
        0 < x < FFFF - significa o próximo bloco do encadeamento, usado nos casos em que é necessário mais de um bloco
            para guardar as informações da categoria
        FFFF - significa fim do encadeamento 
        Aproveitando o fato de que a distância entre os blocos é constante usarei o offset para referenciar os blocos
        logo se a tabela FAT começa no byte 64 do arquivo, o segundo elemento estará no byte 68. Dessa forma o endereço 
        do elemento da tabela que referencia o bloco x será dado por (offset_no_arquivo_da_FAT + x * tamanho_de_cada_elemento) 
    Tamanho: 
        2^18 bytes, pois isso possibilita uma tabela de 2^16 elementos capaz de suportar 256*(2^16) ou 16MB bytes de dados 
        
    
Blocos: 
    Tamanho: 256 bytes cada
    Elementos: 
        Caso diretório:
            Cada elemento será uma 4-upla {id_da_categoria, bloco_inicial, bloco_final, offset_dentro_do_bloco}
            Para saber em qual endereço a informação deve ser escrita calcula-se 
            (endereço_do_primeiro_bloco + (bloco_final * tamanho_do_bloco) + offset_dentro_do_bloco)
            Exemplo: {24, 5, 6, 64}, ou seja se for necessário escrever um novo ponteiro de um filmes que pertence a produtora
            de id 24 será escrito no byte (endereço_do_primeiro_bloco + (6 * 256) + 64). 
            A informação do bloco_inicial será para quando for necessário consultar a FAT 
            Dado que cada elemento terá 16 bytes cada bloco pode listar 16 categorias
        Caso arquivo:
            Ponteiros de 4 bytes para os filmes pertencentes aquela categoria.

Funcionamento:
    Inserir um novo ponteiro de filme:
        Consulta-se o bloco do diretório raiz
        Busca o id_da_categoria que corresponde ao id associado ao filme, se não houver inserir um nova categoria
        Verifica se o bloco atual tem espaço para mais um elemento, atraves da equação (byte_final - size(boot) - size(FAT)) % 256), 
        Se o resultado da equação for 0 significa que o bloco encheu e que um novo bloco deve ser alocado
            Procura na tabela FAT qual o primeiro bloco livre, marca como final de um encadeamento
            Verifica o encadeamento na FAT a partir do bloco inicial informado no elemento do diretório 
            Atualiza o encadeamento.
            Escreve no novo bloco 
            Atualiza byte_final
        Senão o bloco ainda não encheu
            Escreve o novo ponteiro
            Atualiza byte_final
    
    Inserir uma nova categoria:
        Verifica se ainda há espaço no bloco do diretório raiz
        Se não 
            Expande o diretório raiz
            Aloca mais um bloco para o diretório raiz
            Atualiza encadeamento na FAT
        Aloca um bloco para essa nova categoria, Atualiza FAT
        Escreve as informações desta categoria no diretório raiz

*/
