#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



int char_to_int(char* source);
void* get_field_array(void* array_souce, int tam_of_array, int tam_of_struct, int field_offset_bytes, int tam_of_field);
void* create_array_of_structs(FILE* arq_source, int tam_of_struct, int inicio, int quant);
void print_produtora();
const char* nome_genero(int numero_genero);
const char* nome_status(int numero_status);
int extrai_status_number(int flags_and_counts);
void retorna_nome(FILE* arq, int offset, char* buffer);
void recebe_estrutura_retorna_nome(FILE* arq_name, FILE* arq_estrutura, int offset_of_struct, int tam_of_struct, int field_offset, char* buffer);
void clear2();
void copia_arq_n(FILE* source, int tam_of_struct, FILE* dest, int inicio, int quant);
void copia_arq_blocos(FILE* source, FILE* dest, int inicio, int quant, int tam_of_cluster);
int conta_elementos(FILE* arq, int tam_of_struct);
int conta_clusters(FILE* arq, int tam_of_cluster);
