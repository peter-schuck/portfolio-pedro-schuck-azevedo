#include "medias.h"
#include "general_functions.h"
#include "header.h"

double medias_i(FILE* arq_source, int offset_of_field)
{
    FILME* filmes = (FILME*)create_array_of_structs(arq_source, sizeof(FILME),0, MAX_FILMES);
    int* values = (int*)get_field_array(filmes, MAX_FILMES, sizeof(FILME), offset_of_field, sizeof(int));
    long long int acumulador = 0;
    for(int i = 0; i < MAX_FILMES; i++)
    {
        acumulador += values[i];
    }
    double media = (double)acumulador/(double)MAX_FILMES;
    return media;
}

double medias_f(FILE* arq_source, int offset_of_field)
{
    FILME* filmes = (FILME*)create_array_of_structs(arq_source, sizeof(FILME),0, MAX_FILMES);
    float* values = (float*)get_field_array(filmes, MAX_FILMES, sizeof(FILME), offset_of_field, sizeof(int));
    long double acumulador = 0;
    for(int i = 0; i < MAX_FILMES; i++)
    {
        acumulador += values[i];
    }
    double media = (double)acumulador/(double)MAX_FILMES;
    return media;
}