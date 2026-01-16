#define FAT_ELEMENT_TAM 4
#define CLUSTER_TAM 256
#define FAT_TAM 262144
#define FIM_DO_ENCADEAMENTO 0xFFFF
#define ELEMENTS_PER_CLUSTER 64
#define CATEGORY_INFO_TAM 16
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct 
{
    int root_offset;
    int root_final_cluster; //Bloco final do encadeamento do diretório root
    int bytes_escritos_in_root_final_cluster;
    int FAT_offset;
    int FAT_tam;
    int FAT_elements_tam;
    int first_unused_cluster;

}BOOT_INFO;

typedef struct 
{
    int id_da_categoria;
    int bloco_inicial;
    int bloco_final;
    int bytes_escrito_no_bloco_final;
    

}ARQ_INFO;

extern int cluster0_pointer;
extern int FAT_pointer;

void insere_arq_inv_caso1(FILE* arq_invertido, FILE* source, int bytes_tam_of_struct, int offset_movie_pointer, int offset_category_id, int inicio, int quant);
void insere_arq_inv_caso_generos(FILE* arq_invertido, FILE* source, int bytes_tam_of_struct, int offset_category_id, int quant);
void insere_arq_invertido(FILE* arq_invertido, int movie_pointer, int id_da_categoria);
int insere_nova_categoria(FILE* arq_invertido, ARQ_INFO* info);
int busca_id_categoria(FILE* arq_invertido, int id);
int* extract_genres(int generos, int* tam_of_array);
int calcula_endereco_de_bloco(int bloco);
int aloca_e_atualiza_FAT(FILE* arq_invertido);
int offset_no_bloco(int pos);
int expande_encadeamento_FAT(FILE* arq_invertido, int bloco);
int encadeamento_continua(FILE* arq_invertido, int bloco, int pos_atual);
int bloco_atual_dado_endereco(int pos);
int element_pointer_in_FAT(int bloco);
int cluster_number_of_element_FAT(int pos);
void cria_e_inicializa_arq_invertido(FILE* arq_invertido);
int retorna_filme_indice(FILE* arq_invertido, int id_da_categoria, int indice);
void imprime_arquivo_invertido(FILE* arq_invertido, FILE* destino, FILE* filmes, FILE* titulos);
void printa_arq_invertido_paginado(FILE* arq_invertido, FILE* arq_estrtutra, FILE* arq_name, int id_da_categoria, int tam_of_struct);
void filtra_categoria(FILE* arq_invertido, FILE* arq_estrutura, FILE* arq_name);
//void imprime_arquivo_invertido(FILE* arq_invertido, FILE* filmes, FILE* titulos);
