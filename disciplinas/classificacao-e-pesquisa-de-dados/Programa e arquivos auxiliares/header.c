#include "header.h"

FILME filme_aux;
int offset_title_pointer = (long long int)&filme_aux.title_pointer - (long long int)&filme_aux;
int offset_flags_and_counts = (long long int)&filme_aux.flags_and_counts - (long long int)&filme_aux;
int offset_release_date = (long long int)&filme_aux.release_date - (long long int)&filme_aux;
int offset_popularity = (long long int)&filme_aux.popularity - (long long int)&filme_aux;
int offset_vote_average = (long long int)&filme_aux.vote_average - (long long int)&filme_aux;
int offset_revenue = (long long int)&filme_aux.revenue - (long long int)&filme_aux;
int offset_budget = (long long int)&filme_aux.budget - (long long int)&filme_aux;
int offset_vote_count= (long long int)&filme_aux.vote_count - (long long int)&filme_aux;
int offset_runtime = (long long int)&filme_aux.runtime - (long long int)&filme_aux;