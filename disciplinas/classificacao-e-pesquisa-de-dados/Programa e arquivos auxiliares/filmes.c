#include "filmes.h"
#include <string.h>
#include "header.h"
#include <ctype.h>

void decod_gen(int gen)
{
    int cont=0;
    for(int i=0; i<19; i++)                  //verifica os 19 bits do gênero
    {
        if (gen & (1 << i))                 //testa se o bit está em 1
        {
            if(cont > 0)
            {
                printf(", ");              //separa os gêneros com ,
                if (cont % 5 == 0)         //se passar de 5 gêneros, quebra a linha
                {
                    printf("\n");
                }
            }
            switch(i)                    //exibe o gênero equivalente a cada bit
            {
            case 0:
                printf("Acao");
                break;
            case 1:
                printf("Aventura");
                break;
            case 2:
                printf("Animacao");
                break;
            case 3:
                printf("Comedia");
                break;
            case 4:
                printf("Crime");
                break;
            case 5:
                printf("Documentario");
                break;
            case 6:
                printf("Drama");
                break;
            case 7:
                printf("Familia");
                break;
            case 8:
                printf("Fantasia");
                break;
            case 9:
                printf("Historia");
                break;
            case 10:
                printf("Horror");
                break;
            case 11:
                printf("Musica");
                break;
            case 12:
                printf("Misterio");
                break;
            case 13:
                printf("Romance");
                break;
            case 14:
                printf("Ficcao cientifica");
                break;
            case 15:
                printf("Suspense");
                break;
            case 16:
                printf("TV Movie");
                break;
            case 17:
                printf("Guerra");
                break;
            case 18:
                printf("Faroeste");
                break;
            default:
                break;
            }
            cont++;                            //contabiliza quantidade de gêneros
        }
    }
}


//Funções da árvore B de char:
// Function to create a new node
BTreeNode *createNode(bool is_leaf)
{
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < M; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChild(BTreeNode *parent, int index)
{
    BTreeNode *child = parent->children[index];
    BTreeNode *newNode = createNode(child->is_leaf);

    newNode->num_keys = M/2 - 1;

    // Move keys and children to the new node
    for (int i = 0; i < M/2 - 1; i++)
    {
        newNode->keys[i] = child->keys[i + M/2];
    }

    if (!child->is_leaf)
    {
        for (int i = 0; i < M/2; i++)
        {
            newNode->children[i] = child->children[i + M/2];
        }
    }

    child->num_keys = M/2 - 1;

    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    parent->children[index + 1] = newNode;

    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = child->keys[M/2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFull(BTreeNode *node, chave key)
{
    int i = node->num_keys - 1;

    if (node->is_leaf)
    {
        // Insert key into the sorted order
        while (i >= 0 && strcmp(node->keys[i].titulo, key.titulo) > 0)      //compara os nomes dos filmes para encontrar a posicao correta na inserção
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find the child to insert the key
        while (i >= 0 && strcmp(node->keys[i].titulo, key.titulo) > 0)      //compara os nomes dos filmes para encontrar o filho correto para ir
        {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == M - 1)
        {
            // Split child if it's full
            splitChild(node, i);

            // Determine which of the two children is the new one
            if (strcmp(node->keys[i].titulo, key.titulo) <= 0)             //compara os nomes dos filmes para identificar qual filho é o que receberá a inserção
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insert(BTreeNode **root, chave key)
{
    BTreeNode *node = *root;

    if (node == NULL)
    {
        // Create a new root node
        *root = createNode(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    }
    else
    {
        if (node->num_keys == M - 1)
        {
            // Split the root if it's full
            BTreeNode *new_root = createNode(false);
            new_root->children[0] = node;
            splitChild(new_root, 0);
            *root = new_root;
        }
        insertNonFull(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
void traverse(BTreeNode *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            if(root->children[i])
            traverse(root->children[i]);
            /*for(int j = 0; j < strlen(root->keys[i].titulo); j++) //imprime todos os titulos na ordem alfabetica
            {
                fprintf("%c", root->keys[i].titulo[j]);
            }   
            printf("\n");*/
            fprintf(stdout, "%s\n", root->keys[i].titulo);
        }
        if(root->children[i])
        traverse(root->children[i]);
    }
}

//localiza determinado titulo dentro da arvore B
long busca_titulo(BTreeNode *root, char titulo[256], long x)
{
    if (root != NULL)                                                              //se a raiz não é nula,
    {
        int i=0;
        while(i < root->num_keys && strcmp(root->keys[i].titulo, titulo) < 0)      //verifica a posição i onde o título do nodo é menor ou igual ao buscado
        {
            i++;
        }
        if (strcmp(root->keys[i].titulo, titulo) == 0)                            //se for igual, salva o offset do arquivo original binário em x
        {
            x = root->keys[i].pos;
            return x;
        }
        else
        {
            return busca_titulo(root->children[i], titulo, x);                     //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
    }
    else
    {
        return x;                                                                  //se o ponteiro é nulo, retorna x, que por padrão é -1
    }
}

//localiza título dentro da árvore B, sem usar recursão
int busca_titulo2(BTreeNode *root, char titulo[256])
{
    int x = -1;
    BTreeNode* aux = root;
    while (aux != NULL)                                                              //se a raiz não é nula,
    {
        int i=0;
        while(i < aux->num_keys && strcmp(aux->keys[i].titulo, titulo) < 0)      //verifica a posição i onde o título do nodo é menor ou igual ao buscado
        {
            i++;
        }
        if (strcmp(aux->keys[i].titulo, titulo) == 0)                            //se for igual, salva o offset do arquivo original binário em x
        {
            x = aux->keys[i].pos;
            aux = NULL;
        }
        else
        {
            aux = aux->children[i];                     //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
    }
    free(aux);
    return x;                                                                  //se o ponteiro é nulo, retorna x, que por padrão é -1
}

//localiza título e possíveis outros iguais a ele dentro da árvore B, sem usar recursão
int busca_sucessor(BTreeNode *root, char titulo[256], int vet[10])
{
    int x = -1, y, ind=0;
    BTreeNode* aux = root, *aux2 = NULL;
    while (aux != NULL)                                                              //se a raiz não é nula,
    {
        int i=0;
        while(i < aux->num_keys && strcmp(aux->keys[i].titulo, titulo) < 0)      //verifica a posição i onde o título do nodo é menor ou igual ao buscado
        {
            i++;
        }
        if (i < aux->num_keys && strcmp(aux->keys[i].titulo, titulo) == 0)                            //se for igual, salva o offset do arquivo original binário em x
        {
            x = aux->keys[i].pos;
            aux2 = aux;                                                           //salav as informações de aux em aux2 e i em y
            y = i;
            aux = NULL;
        }
        else if (i <= aux->num_keys)
        {
            aux = aux->children[i];                     //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
        else
        {
            aux = NULL;
        }
    }
    while(ind < 10 && aux2 != NULL)                                                                //testa se os sucessores de aux possuem o mesmo titulo que ele
    {
        if (aux2->children[y] != NULL && strcmp(titulo, aux2->children[y]->keys[y].titulo) == 0)      //testa nas chaves do nodo filho
        {
            vet[ind] = aux2->children[y]->keys[y].pos;                                              //armazena os offsets em um vetor
            ind++;
            aux2 = aux2->children[y];                                                            //se for, caminha até a chave do nodo filho com titulo igual
        }
        else if (y+1 < aux2->num_keys && (strcmp(titulo, aux2->keys[y+1].titulo) == 0))             //testa nas chaves do mesmo nodo
        {
            vet[ind] = aux2->keys[y+1].pos;                                                   //armazena os offsets em um vetor
            ind++;
            y++;                                                                             //se for, passa a analisar a chave a direita da atual
        }
        else
        {
            aux2 = NULL;                                                                           //se não, atribui nulo a aux2 e saí do laço
        }
    }
    return x;
}



int busca_sucessor2(BTreeNode *root, char titulo[256], int vet[10])
{
    int x = -1, y, ind = 0;
    BTreeNode* aux = root, *aux2 = NULL;

    while (aux != NULL)                                                              //se a raiz não é nula,
    {
        int i = 0;
        while (i < aux->num_keys && strcmp(aux->keys[i].titulo, titulo) < 0)      //verifica a posição i onde o título do nodo é menor ou igual ao buscado
        {
            i++;
        }

        if (i < aux->num_keys && strcmp(aux->keys[i].titulo, titulo) == 0)       //se for igual, salva o offset do arquivo original binário em x
        {
            x = aux->keys[i].pos;
            aux2 = aux;                                                        //salva as informações de aux em aux2 e i em y
            y = i;
            break;
        }
        else if (i <= aux->num_keys && aux->children[i] != NULL)
        {
            aux = aux->children[i];                                //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
        else
        {
            aux = NULL;
        }
    }

    while (ind < 10 && aux2 != NULL)                             //testa se os sucessores de aux possuem o mesmo titulo que ele
    {
        if (y < aux2->num_keys && aux2->children[y] != NULL)      //testa nas chaves do nodo filho
        {
            if (aux2->children[y]->num_keys > y) {
            if (strcmp(titulo, aux2->children[y]->keys[y].titulo) == 0)
            {
                vet[ind] = aux2->children[y]->keys[y].pos;           //armazena os offsets em um vetor
                ind++;
                aux2 = aux2->children[y];                          //se for, caminha até a chave do nodo filho com titulo igual
                continue;
            }
            }
        }
        if (y + 1 < aux2->num_keys)
        {
            if (strcmp(titulo, aux2->keys[y + 1].titulo) == 0)         //testa nas chaves do mesmo nodo
            {
                vet[ind] = aux2->keys[y + 1].pos;                     //armazena os offsets em um vetor
                ind++;
                y++;                                                  //se for, passa a analisar a chave a direita da atual
                continue;
            }
        }
        aux2 = NULL;                                               //se não, atribui nulo a aux2 e saí do laço
    }

    return x;
}



void traverse_backwards(BTreeNode *root)
{
    if (root != NULL)
    {
        int i;
        traverse_backwards(root->children[root->num_keys]);
        for (i = root->num_keys - 1; i >= 0; i--)
        {
            printf("%s \n", root->keys[i].titulo);     //imprime todos os titulos na ordem reversa a alfabetica
            traverse_backwards(root->children[i]);
        }
    }
}

//libera os ponteiros da árvore B
void delete_Btree(BTreeNode *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            delete_Btree(root->children[i]);
        }
        free(root);
    }
}

void traverse_file(BTreeNode *root, FILE* arq)
{
     if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse_file(root->children[i], arq);
            fwrite(&root->keys[i], sizeof(chave), 1, arq);
        }
        traverse_file(root->children[i], arq);
    }
}


//Funções da árvore B de int:
// Function to create a new node
BTreeNum *createNodeN(bool is_leaf)
{
    BTreeNum *newNode = (BTreeNum *)malloc(sizeof(BTreeNum));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < M; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChildN(BTreeNum *parent, int index)
{
    BTreeNum *child = parent->children[index];
    BTreeNum *newNode = createNodeN(child->is_leaf);

    newNode->num_keys = M/2 - 1;

    // Move keys and children to the new node
    for (int i = 0; i < M/2 - 1; i++)
    {
        newNode->keys[i] = child->keys[i + M/2];
    }

    if (!child->is_leaf)
    {
        for (int i = 0; i < M/2; i++)
        {
            newNode->children[i] = child->children[i + M/2];
        }
    }

    child->num_keys = M/2 - 1;

    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    parent->children[index + 1] = newNode;

    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = child->keys[M/2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFullN(BTreeNum *node, chave_num key)
{
    int i = node->num_keys - 1;

    if (node->is_leaf)
    {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i].number > key.number)      //compara os nomes dos filmes para encontrar a posicao correta na inserção
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find the child to insert the key
        while (i >= 0 && node->keys[i].number > key.number)      //compara os nomes dos filmes para encontrar o filho correto para ir
        {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == M - 1)
        {
            // Split child if it's full
            splitChildN(node, i);

            // Determine which of the two children is the new one
            if (node->keys[i].number < key.number)             //compara os nomes dos filmes para identificar qual filho é o que receberá a inserção
            {
                i++;
            }
        }
        insertNonFullN(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insertN(BTreeNum **root, chave_num key)
{
    BTreeNum *node = *root;

    if (node == NULL)
    {
        // Create a new root node
        *root = createNodeN(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    }
    else
    {
        if (node->num_keys == M - 1)
        {
            // Split the root if it's full
            BTreeNum *new_root = createNodeN(false);
            new_root->children[0] = node;
            splitChildN(new_root, 0);
            *root = new_root;
        }
        insertNonFullN(*root, key);
    }
}

int busca_id(BTreeNum *root, int id)
{
    int x = -1;
    BTreeNum* aux = root;
    while (aux != NULL)                                                              //se a raiz não é nula,
    {
        int i=0;
        while(i < aux->num_keys && aux->keys[i].number < id)      //verifica a posição i onde o título do nodo é menor ou igual ao buscado
        {
            i++;
        }
        if (aux->keys[i].number == id)                            //se for igual, salva o offset do arquivo original binário em x
        {
            x = aux->keys[i].pos;
            aux = NULL;
        }
        else
        {
            aux = aux->children[i];                     //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
    }
    free(aux);
    return x;                                                                  //se o ponteiro é nulo, retorna x, que por padrão é -1
}

// Function to traverse and print the B-tree in-order
void traverseN(BTreeNum *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverseN(root->children[i]);
            printf("%d \n", root->keys[i].number);     //imprime todos os titulos na ordem alfabetica
        }
        traverseN(root->children[i]);
    }
}

void traverse_backwardsN(BTreeNum *root)
{
    if (root != NULL)
    {
        int i;
        traverse_backwardsN(root->children[root->num_keys]);
        for (i = root->num_keys - 1; i >= 0; i--)
        {
            printf("%d \n", root->keys[i].number);     //imprime todos os titulos na ordem reversa a alfabetica
            traverse_backwardsN(root->children[i]);
        }
    }
}

//libera os ponteiros da árvore B
void delete_BtreeN(BTreeNum *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            delete_BtreeN(root->children[i]);
        }
        free(root);
    }
}

void traverse_fileN(BTreeNum *root, FILE* arq4)
{
     if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse_fileN(root->children[i], arq4);
            fwrite(&root->keys[i], sizeof(chave_num), 1, arq4);
        }
        traverse_fileN(root->children[i], arq4);
    }
}

void traverse_titleN(BTreeNum *root, FILE* filmes, FILE* titulos)
{
    FILME m;
    int j=0;
    char title[256];
     if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse_titleN(root->children[i], filmes, titulos);
            fseek(filmes, root->keys[i].pos, SEEK_SET);
            fread(&m, sizeof(FILME), 1, filmes);
            fseek(titulos, m.title_pointer, SEEK_SET);
            j = 0;
                    do                                                                            //laço para obter o titulo do filme
                    {
                        (fread(title + (j * 32), sizeof(char), 32, titulos));
                        j++;
                    }
                    while(title[(j * 32) - 1] != '\0');
                    printf("%s - %d/%d/%d\n", title, root->keys[i].number % 100, (root->keys[i].number % 10000) / 100, root->keys[i].number / 10000);
        }
        traverse_titleN(root->children[i], filmes, titulos);
    }
}

void traverse_title_backwardsN(BTreeNum *root, FILE* filmes, FILE* titulos)
{
    FILME m;
    int j=0;
    char title[256];
     if (root != NULL)
    {
        int i;
        traverse_title_backwardsN(root->children[root->num_keys], filmes, titulos);
        for (i = root->num_keys - 1; i >= 0; i--)
        {
            fseek(filmes, root->keys[i].pos, SEEK_SET);
            fread(&m, sizeof(FILME), 1, filmes);
            fseek(titulos, m.title_pointer, SEEK_SET);
            j = 0;
                    do                                                                            //laço para obter o titulo do filme
                    {
                        (fread(title + (j * 32), sizeof(char), 32, titulos));
                        j++;
                    }
                    while(title[(j * 32) - 1] != '\0');
                    printf("%s - %d/%d/%d\n", title, root->keys[i].number % 100, (root->keys[i].number % 10000) / 100, root->keys[i].number / 10000);
                    traverse_title_backwardsN(root->children[i], filmes, titulos);
        }
    }
}


//Funções da árvore B de float:
// Function to create a new node
BTreeNumf *createNodeF(bool is_leaf)
{
    BTreeNumf *newNode = (BTreeNumf *)malloc(sizeof(BTreeNumf));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < M; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChildF(BTreeNumf *parent, int index)
{
    BTreeNumf *child = parent->children[index];
    BTreeNumf *newNode = createNodeF(child->is_leaf);

    newNode->num_keys = M/2 - 1;

    // Move keys and children to the new node
    for (int i = 0; i < M/2 - 1; i++)
    {
        newNode->keys[i] = child->keys[i + M/2];
    }

    if (!child->is_leaf)
    {
        for (int i = 0; i < M/2; i++)
        {
            newNode->children[i] = child->children[i + M/2];
        }
    }

    child->num_keys = M/2 - 1;

    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    parent->children[index + 1] = newNode;

    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = child->keys[M/2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFullF(BTreeNumf *node, chave_numf key)
{
    int i = node->num_keys - 1;

    if (node->is_leaf)
    {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i].number > key.number)      //compara os nomes dos filmes para encontrar a posicao correta na inserção
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find the child to insert the key
        while (i >= 0 && node->keys[i].number > key.number)      //compara os nomes dos filmes para encontrar o filho correto para ir
        {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == M - 1)
        {
            // Split child if it's full
            splitChildF(node, i);

            // Determine which of the two children is the new one
            if (node->keys[i].number < key.number)             //compara os nomes dos filmes para identificar qual filho é o que receberá a inserção
            {
                i++;
            }
        }
        insertNonFullF(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insertF(BTreeNumf **root, chave_numf key)
{
    BTreeNumf *node = *root;

    if (node == NULL)
    {
        // Create a new root node
        *root = createNodeF(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    }
    else
    {
        if (node->num_keys == M - 1)
        {
            // Split the root if it's full
            BTreeNumf *new_root = createNodeF(false);
            new_root->children[0] = node;
            splitChildF(new_root, 0);
            *root = new_root;
        }
        insertNonFullF(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
void traverseF(BTreeNumf *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverseF(root->children[i]);
            printf("%.2f \n", root->keys[i].number);     //imprime todos os titulos na ordem alfabetica
        }
        traverseF(root->children[i]);
    }
}

void traverse_backwardsF(BTreeNumf *root)
{
    if (root != NULL)
    {
        int i;
        traverse_backwardsF(root->children[root->num_keys]);
        for (i = root->num_keys - 1; i >= 0; i--)
        {
            printf("%.2f \n", root->keys[i].number);     //imprime todos os titulos na ordem reversa a alfabetica
            traverse_backwardsF(root->children[i]);
        }
    }
}

//libera os ponteiros da árvore B
void delete_BtreeF(BTreeNumf *root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            delete_BtreeF(root->children[i]);
        }
        free(root);
    }
}

void traverse_fileF(BTreeNumf *root, FILE* arq)
{
     if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse_fileF(root->children[i], arq);
            fwrite(&root->keys[i], sizeof(chave_numf), 1, arq);
        }
        traverse_fileF(root->children[i], arq);
    }
}

void traverse_titleF(BTreeNumf *root, FILE* filmes, FILE* titulos)
{
    FILME m;
    int j=0;
    char title[256];
     if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse_titleF(root->children[i], filmes, titulos);
            fseek(filmes, root->keys[i].pos, SEEK_SET);
            fread(&m, sizeof(FILME), 1, filmes);
            fseek(titulos, m.title_pointer, SEEK_SET);
            j = 0;
                    do                                                                            //laço para obter o titulo do filme
                    {
                        (fread(title + (j * 32), sizeof(char), 32, titulos));
                        j++;
                    }
                    while(title[(j * 32) - 1] != '\0');
                    printf("%s - %f\n", title, root->keys[i].number);
        }
        traverse_titleF(root->children[i], filmes, titulos);
    }
}

void traverse_title_backwardsF(BTreeNumf *root, FILE* filmes, FILE* titulos)
{
    FILME m;
    int j=0;
    char title[256];
     if (root != NULL)
    {
        int i;
        traverse_title_backwardsF(root->children[root->num_keys], filmes, titulos);
        for (i = root->num_keys - 1; i >= 0; i--)
        {
            fseek(filmes, root->keys[i].pos, SEEK_SET);
            fread(&m, sizeof(FILME), 1, filmes);
            fseek(titulos, m.title_pointer, SEEK_SET);
            j = 0;
                    do                                                                            //laço para obter o titulo do filme
                    {
                        (fread(title + (j * 32), sizeof(char), 32, titulos));
                        j++;
                    }
                    while(title[(j * 32) - 1] != '\0');
                    printf("%s - %f\n", title, root->keys[i].number);
                    traverse_title_backwardsF(root->children[i], filmes, titulos);
        }
    }
}

/*BTreeNum cria_e_grava_arvore_B_int(FILE* arq_estrutura, FILE* arq_arvore, int tam_of_struct, int offset_of_field)
{
    rewind(arq_estrutura);
    BTreeNum* root = NULL;
    while (!feof(arq_estrutura))
    {
        //Lê filmes e guarda offset
        int element_pointer = ftell(arq_estrutura);
        char* elemento = (char*)malloc(tam_of_struct);
        fread(elemento,tam_of_struct, 1 ,arq_estrutura);

        //Pega o campo desejado
        int* campo = (int*)get_field_array(elemento, 1, tam_of_struct, offset_of_field, sizeof(int));

        //Insere na árvore B
        chave_num key;
        key.number = *campo;
        key.pos = element_pointer;
        insertN(&root, key); 
    }
    //Depois de pronta grava a árvore B no arquivo;
    traverse_fileN(root, arq_arvore);
}*/

/*BTreeNumf cria_e_grava_arvore_B_float(FILE* arq_estrutura, FILE* arq_arvore, int tam_of_struct, int offset_of_field)
{
    rewind(arq_estrutura);
    BTreeNumf* root = NULL;
    while (!feof(arq_estrutura))
    {
        //Lê filmes e guarda offset
        int element_pointer = ftell(arq_estrutura);
        char* elemento = (char*)malloc(tam_of_struct);
        fread(elemento,tam_of_struct, 1 ,arq_estrutura);

        //Pega o campo desejado
        int* campo = (int*)get_field_array(elemento, 1, tam_of_struct, offset_of_field, sizeof(int));

        //Insere na árvore B
        chave_numf key;
        key.number = *campo;
        key.pos = element_pointer;
        insertF(&root, key); 
    }
    //Depois de pronta grava a árvore B no arquivo;
    traverse_fileF(root, arq_arvore);
}*/

void inicializa_arvores(BTreeNode* root, BTreeNum* raiz, BTreeNumf* rootf ,FILE* filmes, FILE* titulos, FILE* arq, FILE* arq4, FILE* arq5)
{
    char title[256];
    FILME novo_filme;
    chave key;
    chave_num key_n;
    chave_numf key_f;
    int i, m;
    if(root == NULL)
    {
        for(int j = 0; j < MAX_FILMES; j++)
        {
            int filme_offset = ftell(filmes);                                                 //guarda offset do filme que sera lido
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
            //fread(&key, sizeof(chave), 1, arq);        //linha para ler somente do arquivo da árvore B de char
            insert(&root, key);                                                           //insere chave na arvore B
            //fwrite(&key, sizeof(chave), 1, arq);
            key_n.number = novo_filme.release_date;
            if(key_n.number > 0)
            {
                key_n.pos = filme_offset;
                insertN(&raiz, key_n);
            }

            key_f.number = novo_filme.vote_average;
            key_f.pos = filme_offset;
            insertF(&rootf, key_f);
        }
        traverse_file(root, arq);
        traverse_fileN(raiz, arq4);
        traverse_fileF(rootf, arq5);
    }
}

