#include "header.h"
#include "general_functions.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


int get_cmd_nrows(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
 }
 int get_cmd_ncols(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return(csbi.srWindow.Right - csbi.srWindow.Left + 1);
 }
 void clear1() { //método com espaços
    int i, r, c;
    r = get_cmd_nrows();
    c = get_cmd_ncols();
    for(i = 0;i<r*c;i++) {
        printf(" ");
    }
 }
 void clear2() {
    int i, r; //Método usando quebras de linha
    r = get_cmd_nrows();
    for(i = 0;i<r ;i++) {
        printf("\n");
    }
 }

int char_to_int(char* source)
{
    int number;
    char* aux;
    aux = (char*)&number;
    for(int i = 0; i < 4; i++)
    {     
        *aux = source[i];
        aux += 1;  
    }
    return number;
}



void* get_field_array(void* array_souce, int tam_of_array, int tam_of_struct, int field_offset_bytes, int tam_of_field)
{
    //O objetico dessa função é a partir de uma array do tipo de uma estrutura qualquer,
    //copiar um determinado campo de cada elemento da estrutura tipo do array;

    //Ponteiro void do array se´ra convertido para um ponteiro char para possibilitar a aritimética de ponteiros
    char* aux = (char*)array_souce;
    char* retorno = NULL;
    int tam_retorno = 0;

    //Percorre o array usando tam_of_struct
    //Usando o offset do campo dentro da estrutura será copiado tantos bytes quanto o tamanho do campo(tam_of_field)
    for(int i = 0; i < tam_of_array; i++)
    {
        for(int j = 0; j < tam_of_field; j++)
        {
            retorno = (char*)realloc(retorno, (tam_retorno + 1) * sizeof(char));
            retorno[tam_retorno] = aux[(i * tam_of_struct) + field_offset_bytes + j];
            tam_retorno++;
        }
    }
    void* general_return = (void*)retorno;
    return general_return;    
}

//Recebe lê n elementos com estrututra de tamanho tam_of_struct, retorna um array com esses elementos
void* create_array_of_structs(FILE* arq_source, int tam_of_struct, int inicio, int quant)
{
    char* aux = NULL;
    fseek(arq_source, inicio * tam_of_struct, SEEK_SET);
    for(int i = 0; i < quant; i++)
    {
        aux = (char*)realloc(aux, (i + 1)*tam_of_struct);
        if(fread(aux + (i * tam_of_struct), tam_of_struct, 1, arq_source) != 1)
        {
            printf("\nErro na create_array_of_structs: erro na leitura");
            exit(EXIT_FAILURE);
        }

    }
    return aux;
}

void print_produtora()
{
    FILE* arq_possuido = fopen("possuido.bin", "rb");
    FILE* arq_filmes = fopen("filmes.bin", "rb");
    FILE* arq_produtora = fopen("produtora.bin", "rb");
    FILE* arq_name_companies = fopen("company_name.bin", "rb");
    FILE* arq_titles = fopen("titulos.bin", "rb");

    char title[256];
    char name_company[256];
    int i = 0;
    int j = 0;
    POSSUIDO aux;
    FILME filme_aux;
    PRODUTORA produtora_aux;
    while(fread(&aux, sizeof(POSSUIDO), 1, arq_possuido) == 1)
    {
        printf("\n%d %d %d %d", aux.movie_id, aux.movie_pointer, aux.companie_id, aux.company_pointer);
        i = 0;
        fseek(arq_filmes, aux.movie_pointer, SEEK_SET);
        fread(&filme_aux, sizeof(FILME), 1, arq_filmes);
        fseek(arq_titles, filme_aux.title_pointer, SEEK_SET);
        do                                                                            //laço para obter o titulo do filme
        {
            (fread(title + (i * 32), sizeof(char), 32, arq_titles));
            i++;
        }
        while(title[(i * 32) - 1] != '\0');
        fseek(arq_produtora, aux.company_pointer, SEEK_SET);
        fread(&produtora_aux, sizeof(PRODUTORA), 1, arq_produtora);
        fseek(arq_name_companies, produtora_aux.name_pointer, SEEK_SET);
        i = 0;
        do                                                                            //laço para obter o titulo do filme
        {
            (fread(name_company + (i * 32), sizeof(char), 32, arq_name_companies));
            i++;
        }
        while(name_company[(i * 32) - 1] != '\0');
        printf("\n%s, %s", title, name_company); 
        j++;
        if(j > 100)
        {
            break;
        }
        
    }    
}


//Decodifica genero
const char* nome_genero(int numero_genero) 
{
    switch (numero_genero) 
    {
        case 1:     return "Action";
        case 2:     return "Adventure";
        case 4:     return "Animation";
        case 8:     return "Comedy";
        case 16:    return "Crime";
        case 32:    return "Documentary";
        case 64:    return "Drama";
        case 128:   return "Family";
        case 256:   return "Fantasy";
        case 512:   return "History";
        case 1024:  return "Horror";
        case 2048:  return "Music";
        case 4096:  return "Mystery";
        case 8192:  return "Romance";
        case 16384: return "Science Fiction";
        case 32768: return "Thriller";
        case 65536: return "TV Movie";
        case 131072:return "War";
        case 262144:return "Western";
        default:    return "Unknown";
    }
}

int extrai_status_number(int flags_and_counts)
{
    int mascara  = (int)pow(2, 21) + (int)pow(2,20) + (int)pow(2, 19);
    int status = flags_and_counts & mascara; 
    status = status >> 19;
    return status;

}

//Recebe um número de status e retorna o nome do status
const char* nome_status(int numero_status) 
{
    switch (numero_status) 
    {
        case 0: return "Rumored";
        case 1: return "Planned";
        case 2: return "In Production";
        case 3: return "Post Production";
        case 4: return "Released";
        case 5: return "Canceled";
        default: return "Unknown";
    }
}

//Esta função recebe o offset de um FILME ou uma PRODUTORA e retorna o seu nome em buffer
void recebe_estrutura_retorna_nome(FILE* arq_name, FILE* arq_estrutura, int offset_of_struct, int tam_of_struct, int field_offset, char* buffer)
{
    fseek(arq_estrutura, offset_of_struct, SEEK_SET);
    char* aux = (char*)malloc(tam_of_struct);
    fread(aux, tam_of_struct, 1, arq_estrutura);
    int* field = get_field_array(aux, 1, tam_of_struct, field_offset, sizeof(int));
    retorna_nome(arq_name, *field, buffer);
}

//Recebe um arquivo no qual deve ser acessado um offset de uma string e poem em buffer
void retorna_nome(FILE* arq, int offset, char* buffer)
{
    fseek(arq, offset, SEEK_SET);
    char* aux = NULL;
    int i = 0;
    do
    {
        aux = (char*)realloc(aux, (i + 1) * 32);
        fread(aux + (i * 32), sizeof(char), 32, arq);
        i++;

    } while (aux[(i * 32) - 1] != '\0');
    strcpy(buffer, aux);
    
}

//Copia quant elementos de tamanho tam_of_struct a partir da variável inicio para o arquivo dest
void copia_arq_n(FILE* source, int tam_of_struct, FILE* dest, int inicio, int quant)
{
    rewind(source);
    char* aux = (char*)malloc(tam_of_struct);
    fseek(source, inicio*tam_of_struct, SEEK_SET);
    for(int i = 0; i < quant; i++)
    {
        fread(aux, tam_of_struct, 1, source);
        fwrite(aux, tam_of_struct, 1, dest);
    }
    fflush(dest);
}

void copia_arq_blocos(FILE* source, FILE* dest, int inicio, int quant, int tam_of_cluster)
{
    rewind(source);
    char aux[256];
    for(int i = 0; i < inicio; i++)
    {
        int j = 0;
        do
        {
            fread(aux + (j * tam_of_cluster), tam_of_cluster, 1, source);
            j++;

        } while (aux[(j * tam_of_cluster) - 1] != '\0');
        
    }
    for(int i = 0; i < quant; i++)
    {
        int j = 0;
        do
        {
            fread(aux + (j * tam_of_cluster), tam_of_cluster, 1, source);
            j++;

        } while (aux[(j * tam_of_cluster) - 1] != '\0');
        //printf("\n%d - %s", i, aux);
        
        
        fwrite(aux, j * tam_of_cluster, 1, dest);
    }
    fflush(dest);
}

int conta_elementos(FILE* arq, int tam_of_struct)
{
    rewind(arq);
    char* aux = malloc(tam_of_struct);
    int count = 0;
    while(!feof(arq))
    {
        if(fread(aux, tam_of_struct, 1 , arq) == 1)
        {
            count++;
            //filme_aux = aux;
            //printf("\n%d - %d", count, filme_aux->title_pointer);
        }
        

    }
    return count;
}

int conta_clusters(FILE* arq, int tam_of_cluster)
{
    rewind(arq);
    char aux[256];
    int count = 0;
    while(!feof(arq))
    {
        int j = 0;
        do
        {
            if(fread(aux + (j * tam_of_cluster), tam_of_cluster, 1, arq) != 1)
            {
                return count;
            }
            j++;

        } while (aux[(j * tam_of_cluster) - 1] != '\0');
        count++;
        //printf("\n%d - %s", count, aux);

    }
    return count;
}
