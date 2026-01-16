#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define M 16 // Maximum degree of the B-tree
#define MAX_FILMES 10000

extern int offset_title_pointer;
extern int offset_flags_and_counts;
extern int offset_release_date;
extern int offset_popularity;
extern int offset_vote_average;
extern int offset_revenue;
extern int offset_budget;
extern int offset_vote_count;
extern int offset_runtime;

typedef struct
{
    int id;
    int title_pointer;
    int flags_and_counts;
    int budget;
    int revenue;
    int release_date;
    int runtime;
    float popularity;
    float vote_average;
    int vote_count;
    int companies_pointer;
    int countries_pointer;

}FILME;

typedef struct
{
    int id;
    int name_pointer;
    char country_id[2];

}PRODUTORA;

typedef struct
{
    int movie_id;
    int movie_pointer; 
    int companie_id;
    int company_pointer;

}POSSUIDO;

typedef struct 
{
    int movie_id;
    int movie_pointer;
    char country_id[2];

}FEITO;