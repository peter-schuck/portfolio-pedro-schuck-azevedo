#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAP_HEIGHT 300
#define MAP_WIDTH 10
#define NASCIMENTO MAP_HEIGHT - 7 //local de nascimento definido desse jeito pois a posição do mapa muda de acordo com a largura dele
#define PULO 16.0f      //altura do pulo
#define GRAVIDADE 45.0 //Esse número só tá aqui porque foi o mais adequado que eu consegui chegar sem fazer ele parecer que tá na lua
#define VELOCIDADE_HORZ 0.1f


// Dimensões de tela
const int LarguraTela = 1200;
const int AlturaTela = 600;

float roll = 0.0f;  //variável para rotacionar o cubo

//Estrutura da pontuação dos jogadores
typedef struct pontuacao
{
    char nome[20]; //nome do jogador
    int pontos; //pontos que o jogador obteve
} pontuacao;


// Estrutura do Jogador
typedef struct jogador
{
    Vector3 posicao;
    Vector3 tamanho;
    Color color;
    float velocidade; //armazena a velocidade do jogador
    bool isGrounded; //verifica se o jogador está em contado com o chão
    bool isAlive; //verifica se o jogador tá vivo
    int contm; //contador de mortes
    float roll; //float para administrar rotação
    bool corrida; //Booleano para correr constantemente
    Matrix transform; // Transformação de matrizes
    pontuacao p1; //armazena as informações relacionadas a pontuação do jogador
} jogador;

//Estrutura do obstáculo
typedef struct obstaculo
{
    Vector3 posicao;
    Vector3 tamanho;
    Color color;
    bool collision;
} obstaculo;

//estrutura dos cubos para explosão
typedef struct ExplosionCube {
    Vector3 posicao;
    Vector3 velocidade;
    Vector3 tamanho;
    bool ativo;
} ExplosionCube;

ExplosionCube explosionCubes[4];
bool isExploding = false;


//Estrutura do Mouse
typedef struct Mouse
{
    Vector2 onde; //posicao do mouse
    int c; //inteiro que verifica se o mouse foi clicado
} Mouse;

int map[MAP_WIDTH][MAP_HEIGHT];

void initMap(int terreno[MAP_WIDTH][MAP_HEIGHT])
{
    // Inicializa o mapa com zeros (sem obstáculos)
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            map[i][j] = 0;
        }
    }

    int exampleMap[MAP_WIDTH][MAP_HEIGHT] =   //Mapa de exemplo que pode ser alterado a qualquer momento
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    //Passa por cada elemento do mapa exemplo
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            map[i][j] = terreno[i][j];  //mapa do jogo recebe o terreno lido do arquivo
        }
    }
}
//Função para desenhar o mapa usando da mesma lógica para ler ele: Passar por cada elemento e aplicar a função DrawCube
void DrawMap()
{
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            if (map[i][j] == 1)   //Desenha obstáculos do jogo
            {
                Vector3 position = { j, MAP_HEIGHT - 1 - i, 0.0f }; //Inverte para desenhar eles na perspectiva correta, talvez corrigir mais tarde (se tirar, Coluna vira Linha e vice-versa)
                Vector3 size = { 1.0f, 1.0f, 1.0f };
                Color color = (Color){101, 87, 148, 255};
                DrawCube(position, size.x, size.y, size.z, color); //Função para desenhar o cubo em relação a sua posição na matriz
            }
            if (map[i][j] == 2)   //Desenha obstáculo tipo 2 (espinhos)
            {
                Vector3 position = { j, MAP_HEIGHT - 1 - i, 0.0f };
                Vector3 size = { 1.0f, 1.0f, 1.0f };
                Color color = RED;
                DrawCube(position, size.x, size.y, size.z, color);
            }
        }
    }
}

// Função para inicializar o jogador
jogador Initjogador()
{
    jogador jogador;
    jogador.posicao = (Vector3){1.5f, NASCIMENTO, 0.0f};  // Posição inicial
    jogador.tamanho = (Vector3){1.0f, 1.0f, 1.0f};      // Tamanho do Jogador
    jogador.color = (Color){248, 220, 158, 255};       // Cor do Jogador
    jogador.velocidade = 0.0f; // Velocidade do jogador
    jogador.isGrounded = false; //Booleano para poder pular
    jogador.isAlive = true;
    jogador.contm = 0; // Contador de mortes//
    jogador.roll = 0.0f;  // Inicializa a variável de rotação
    jogador.transform = MatrixIdentity(); // Inicializa a transformação de matrizes
    jogador.corrida = true; //inicializa a variável de corrida para true (correr constantemente)
    return jogador;
}

// Função para inicializar obstáculo
obstaculo InitObstaculo()
{
    obstaculo obstaculo;
    obstaculo.posicao = (Vector3){0.0f, 5.0f, 0.0f};  // Posição inicial do obstáculo
    obstaculo.tamanho = (Vector3){1.0f, 1.0f, 1.0f};      // Tamanho do obstáculo
    obstaculo.color = (Color){24, 28, 27, 255};       // Cor do obstáculo
    return obstaculo;
}

// Função auxiliar para a colisão utilizando SAT (Separating Axis Theorem)
//sim, eu tive que estudar essa merda. Sim, tem coisa do ChatGPT e do Google aqui que eu não entendo perfeitamente
typedef struct
{
    Vector2 vertices[4]; //Vetor para armazenar os vértices do quadrado (por isso 4)
} Box2D;

//Função que calcula os vértices de uma caixa de colisão
Box2D GetBox2D(Vector3 position, Vector3 size)
{
    //com base na posição e tamanho fornecidos
    Box2D box;
    box.vertices[0] = (Vector2){position.x - size.x / 2, position.y - size.y / 2}; //Calcula a posição dos vértices da caixa de colisão para cada vértice
    box.vertices[1] = (Vector2){position.x + size.x / 2, position.y - size.y / 2};
    box.vertices[2] = (Vector2){position.x + size.x / 2, position.y + size.y / 2};
    box.vertices[3] = (Vector2){position.x - size.x / 2, position.y + size.y / 2};
    //retorna a caixa atualizada
    return box;
}

//Função que calcula a borda (aresta) da caixa de colisão
Vector2 GetEdge(Box2D box, int i)
{
    return (Vector2)
    {
        box.vertices[(i + 1) % 4].x - box.vertices[i].x, //Garante que o índice seja circular, voltando ao início após o último vértice
        box.vertices[(i + 1) % 4].y - box.vertices[i].y
    };
}

//Função para pegar a normal (Objeto que é perpendicular a um dado objeto), ou seja, pegar a aresta vizinha
//Função que calcula a normal de uma aresta, que é perpendicular à aresta
Vector2 GetNormal(Vector2 edge)
{
    return (Vector2)
    {
        -edge.y, edge.x //Troca as coordenadas e inverte o sinal de uma delas para obter a normal
        };
}

//Esse eu não tive aula com o afonso o suficiente para entender perfeitamente, mas faz a soma dos produtos escalares dos vetores (porque disso, não sei exatamente)
//Função que calcula o produto escalar de dois vetores
float DotProduct(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y; //Soma dos produtos das componentes correspondentes dos vetores
}

//Função que projeta os vértices da caixa de colisão em um eixo e encontra os valores mínimo e máximo da projeção
void Project(Box2D box, Vector2 axis, float *min, float *max)
{
    //Calcula a projeção do primeiro vértice
    *min = DotProduct(axis, box.vertices[0]);
    *max = *min;

    for (int i = 1; i < 4; i++)
    {
        float projection = DotProduct(axis, box.vertices[i]);
        if (projection < *min) *min = projection; //Atualiza o valor mínimo se a projeção atual for menor
        if (projection > *max) *max = projection; //Atualiza o valor máximo se a projeção atual for maior
    }
}

//Booleano para verificar se existe overlap entre dois objetos
//Função que verifica se há sobreposição entre dois intervalos
bool Overlaps(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA); // Retorna verdadeiro se os intervalos se sobrepõem
}

//Verifica se há colisão entre duas caixas de colisão usando o Teorema do Eixo Separador (Separating Axis Theorem, SAT)
bool CheckCollisionSAT(Box2D box1, Box2D box2)
{
    for (int i = 0; i < 4; i++) //Loop para verificar todas as arestas de box1 e box2
    {
        Vector2 edge = GetEdge(box1, i); //Obtém a aresta atual de box1
        Vector2 axis = GetNormal(edge); //Calcula a normal da aresta

        float minA, maxA, minB, maxB;
        Project(box1, axis, &minA, &maxA);//Projeta box1 no eixo
        Project(box2, axis, &minB, &maxB); //Projeta box2 no eixo

        if (!Overlaps(minA, maxA, minB, maxB)) return false; //Retorna falso se não há sobreposição nas projeções
    }

    //Mesma coisa de antes, só que para a segunda caixa de colisão (box2)
    for (int i = 0; i < 4; i++)
    {
        Vector2 edge = GetEdge(box2, i);
        Vector2 axis = GetNormal(edge);

        float minA, maxA, minB, maxB;
        Project(box1, axis, &minA, &maxA);
        Project(box2, axis, &minB, &maxB);

        if (!Overlaps(minA, maxA, minB, maxB)) return false;
    }

    return true; //Retorna verdadeiro se há sobreposição
}


// Função para atualizar a posição do jogador
void Updatejogador(jogador *jogador)
{
    Vector3 NovoJogadorPosicao = jogador->posicao; //Seta a nova posição como a posição do jogador

    if (jogador->corrida == true)
    {
        NovoJogadorPosicao.x += VELOCIDADE_HORZ; //Atualiza a posição do jogador constantemente para a direita
    }
    //if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) NovoJogadorPosicao.x -= 0.1f;/ //Usado para ele se mover para a esquerda (inutilizado, mas caso a gente queira testar mais casos, ta aí

    jogador->velocidade -= GRAVIDADE * GetFrameTime(); //Aplicar gravidade
    if (!jogador->isGrounded)
    {
        NovoJogadorPosicao.y += jogador->velocidade * GetFrameTime(); //Atualiza a posição do jogador verticalmente se o jogador não estiver no solo
    }

    Box2D JogadorBox = GetBox2D(NovoJogadorPosicao, jogador->tamanho); //Pega a caixa de colisão do jogador
    jogador->isGrounded = false;  // Reseta a condição de colidindo com o chão

    //Contador para aplicar colisão em todos os objetos definidos pela matriz do mapa (meio desnecessário, mas já é tarde demais para mudar...)
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            if (map[i][j] == 1 || map[i][j] == 2)
            {
                Vector3 obstaculoPosicao = { j, MAP_HEIGHT - 1 - i, 0.0f };
                Vector3 obstaculoTamanho = { 1.0f, 1.0f, 1.0f };
                Box2D ObstaculoBox = GetBox2D(obstaculoPosicao, obstaculoTamanho); //Pega a caixa de colisão do Obstáculo

                if (CheckCollisionSAT(JogadorBox, ObstaculoBox))
                {
                    // Calcula o overlap em relação ao eixo x e y
                    Vector3 displacement = { 0.0f, 0.0f, 0.0f };

                    float overlapX = (jogador->tamanho.x / 2.0f + obstaculoTamanho.x / 2.0f) - fabs(NovoJogadorPosicao.x - obstaculoPosicao.x);
                    //Verifica matematicamente se o jogador está prestes a entrar em outro objeto em relação ao eixo-x
                    float overlapY = (jogador->tamanho.y / 2.0f + obstaculoTamanho.y / 2.0f) - fabs(NovoJogadorPosicao.y - obstaculoPosicao.y);
                    //Verifica matematicamente se o jogador está prestes a entrar em outro objeto em relação ao eixo-y
                    jogador->isGrounded = true; //Quando ele registra qualquer colisão, ele pode pular
                    float margem = 0.5f;  //constante para deixar a colisão um pouco menos exata

                    if (overlapX < overlapY - margem)
                    {
                        //jogador->isAlive = false;
                        //StartExplosion(jogador->posicao);

                        jogador->posicao = (Vector3){2.0f, NASCIMENTO, 0.0f}; //Se colidir com uma parede
                        NovoJogadorPosicao = jogador->posicao; // renasce
                        jogador->velocidade = 0.0f; //seta a velocidade para zero (para caso ele morra enquanto com gravidade aplicada, renascer sem a velocidade)

                        if (jogador->posicao.y == NASCIMENTO && jogador->posicao.x == 2.0f) //verifica se ele renasceu mesmo
                        {
                            jogador->contm++; //contador de mortes aumenta quando ele renasce
                            return; //Garante que ele só registre isso uma vez (se tirar, registra umas 10-100 mortes ao colidir com parede)
                        }
                        // Resolve a colisão com eixo-x
                        if (NovoJogadorPosicao.x < obstaculoPosicao.x)
                        {
                            displacement.x = -overlapX;
                        }
                        else
                        {
                            displacement.x = overlapX;
                        }
                    }
                    else
                    {
                        // Resolve colisão com eixo-y
                        if (NovoJogadorPosicao.y < obstaculoPosicao.y)
                        {
                            displacement.y = -overlapY;
                            jogador->velocidade = 0; //Seta a velocidade do jogador para zero
                            jogador->isGrounded = true;  // Jogador está no chão
                        }
                        else
                        {
                            displacement.y = overlapY;
                            jogador->velocidade = 0;
                        }
                    }

                    NovoJogadorPosicao.x += displacement.x; //Atualiza a posição do jogador para ele sempre se manter acima do chão
                    NovoJogadorPosicao.y += displacement.y; //Atualiza a posição do jogador para ele sempre se manter fora das paredes

                    // Caso especial de objeto tipo espinho (reseta jogador)
                    if (map[i][j] == 2)
                    {
                        jogador->posicao = (Vector3){2.0f, NASCIMENTO, 0.0f}; //Faz o jogador renascer
                        NovoJogadorPosicao = jogador->posicao;

                        //StartExplosion(jogador->posicao);

                        jogador->velocidade = 0.0f;
                        if (jogador->posicao.y == NASCIMENTO) //Novamente, registrar a morte
                        {
                            jogador->contm++; //Contador de morte atualiza
                        }
                    }
                }
            }
        }
    }

    /*if (jogador->isAlive == false && !isExploding)
    {
        StartExplosion(jogador->posicao);
    */

    jogador->posicao.x = NovoJogadorPosicao.x; //Não se move se colidir com chão
    jogador->posicao.y = NovoJogadorPosicao.y; //Não se move se colidir com parede (meio inutilizado por conta da condição de morte, mas caso ela de errado, da pra voltar)

    // Lógica do Pulo
    if (IsKeyPressed(KEY_SPACE) && jogador->isGrounded)
    {
        jogador->velocidade = PULO;  //altura do pulo do jogador
        jogador->isGrounded = false; //Impede ele de pular novamente
    }

    // Controles de rotação, inutilizados no momento
    if (IsKeyDown(KEY_J)) jogador->roll += 2.5f * GetFrameTime();
    if (IsKeyDown(KEY_L)) jogador->roll -= 2.5f * GetFrameTime();

    // Rotaciona enquanto no ar (rotaciona em relação ao eixo-Z)
    if (!jogador->isGrounded)
    {
        jogador->roll -= 0.08f; // Ajuste a velocidade de rotação como queira
    }
    else
    {
        jogador->roll = 0.0f; //Caso ele não esteja no ar, para de rotacionar
    }

    // Atualizar matriz de transformação
    jogador->transform = MatrixIdentity();
    jogador->transform = MatrixMultiply(jogador->transform, MatrixRotateZ(jogador->roll));
    jogador->transform = MatrixMultiply(jogador->transform, MatrixTranslate(jogador->posicao.x, jogador->posicao.y, jogador->posicao.z));

}

//----------------------------------------------------//
//Função para escrever o nome
int Escreve(char letra[], int i, int *x) //Função para poder digitar
{
    char aa;
    aa = GetCharPressed(); //Pega o caracter digitado
    while (aa > 0) //Se for positivo,
    {
        if (i < 20)
        {
            letra[i] = aa; //Se o index for menor que 45, vetor pega o char e passa para a próxima posição
            i++;
        }
        aa = GetCharPressed(); //Pega novo char
    }
    if (IsKeyPressed(KEY_BACKSPACE) && (i >= 1)) //Se apertar backspace e o index for maior que 1
    {
        i--;
        letra[i] = '\0'; //Apaga o ultimo char digitado e volta uma poisção no vetor
    }
    if (IsKeyPressed(KEY_ENTER))   //Se apertar enter
    {
        letra[i] = '\0'; //Fecha o vetor
        *x = 0; //Volta pra tela inicial
    }

    return i;
}

void le_txt(pontuacao top5[])
{
    FILE *arq;
    int i=0;
    arq = fopen("top_scores.txt", "r");
    for (i=0; i<5; i++)
    {
        if (fgets(top5[i].nome, 21, arq) != NULL)
        {
            top5[i].nome[strlen(top5[i].nome)-1] = '\0';
            if (fscanf(arq, "\n%d\n", &top5[i].pontos) == -1)
            {
                printf("erro ao ler o arquivo");
            }
        }
    }
}


void atualiza_pontos(jogador *p1)   //Atualiza a pontuação do jogador
{
    p1->p1.pontos = 10000 - (p1->contm*500); //10.000 pontos menos 500 por cada morte
}
int verifica_se_entra_no_leaderboard(jogador *p1, int *pos, pontuacao top5[])    //Verifica se a pontuação do jogador é maior que a de alguém do leaderboard
{
    int i=0, cont=0, flag=0, x=0;
    pontuacao jog[6], jog2[6];
    for (i=0; i<5; i++)
    {
        jog2[i] = top5[i]; //Faz uma cópia do vetor de estruturas do arquivo
    }
    for (i=0; i<5; i++)
    {
        if (top5[i].pontos < p1->p1.pontos && flag==0)    //Se a pontuação do jogador for maior e a flag não estiver ativada,
        {
            *pos = i;  //Guarda a posição que o jogador deve ficar, passada por referência com um ponteiro
            flag=1;   //Ativa a flag
            strcpy(p1->p1.nome, "Nome temp");  //Põem um nome temporário para o jogador
            x=4;  //Atualiza x para 4, para ele entrar na tela de ler o nome do jogador
        }
        if (flag)   //Se a flag estiver ativa,
        {
            top5[i+1] = jog2[i];  //Coloca cada nome do leaderboard uma posição abaixo da original
        }
    }
    return x;  //Retorna x para mudança de tela
}

void troca_info_array(pontuacao top5[], pontuacao novoj, int posicao)    //Muda alguém do vetor pela pontuação do jogador
{
    top5[posicao].pontos = novoj.pontos;  //Pontuação do jogador substitui a pontuação de alguém do vetor
    strcpy(top5[posicao].nome, novoj.nome);  //Nome do jogador substitui o nome de alguém do vetor
}


void salva_txt(pontuacao top5[])
{
    FILE *arq;
    int i=0;
    arq = fopen("top_scores.txt", "w");
    for (i=0; i<5; i++)
    {
        if (fprintf(arq, "%s\n%d\n", top5[i].nome, top5[i].pontos) < 0)
        {
            printf("erro ao ler o arquivo");
        }
    }
}

void le_terreno(int mat[MAP_WIDTH][MAP_HEIGHT]) {   //Função para ler arquivo texto do mapa
FILE *arq;
int y=0, i=0, cont=0;
char c;
if ((arq = fopen("terrain.txt", "r")) == NULL)     //Abre o arquivo texto
    printf("Erro ao abrir o arquivo\n");        //Mensasem de erro
c = getc(arq);                                //Pega um caracter do arquivo
while (c != EOF) {                            //Se este não for o End Of File,
    switch(c) {                              //Transforma o caracter em 1, 2 ou 0 para representar diferentes obstáculos
    case 'G': y=1;
    break;
    case 'S': y=2;
    break;
    default: y=0;
    break;
    }
if (cont < MAP_HEIGHT) {                 //Armazena os valores transformados em uma matriz
    mat[i][cont] = y;
    cont++;
}
if (cont == MAP_HEIGHT) {
    cont=0;
    i++;
}
c = getc(arq);                         //Pega novo caracter
}
for (i=0;i<MAP_WIDTH;i++) {                //Se alguma coisa diferente estiver na matriz, troca este valor para zero
    for (cont=0;cont<MAP_HEIGHT;cont++) {
            if (mat[i][cont] != 0 && mat[i][cont] != 1 && mat[i][cont] != 2) {
                   mat[i][cont] = 0;
            }
    }
}

fclose(arq);           //Fecha o arquivo
}
//Função para fazer o personagem explodir, em fase de testes ainda
/*void StartExplosion(Vector3 posicao) {
    for (int i = 0; i < 4; i++) {
        explosionCubes[i].posicao = posicao;
        explosionCubes[i].tamanho = (Vector3){0.5f, 0.5f, 0.5f};
        explosionCubes[i].ativo = true;
    }

    // Assign velocities to make cubes fly in different directions
    explosionCubes[0].velocidade = (Vector3){-2, 2, -2}; // Top-left
    explosionCubes[1].velocidade = (Vector3){2, 2, -2};  // Top-right
    explosionCubes[2].velocidade = (Vector3){-2, -2, 2}; // Bottom-left
    explosionCubes[3].velocidade = (Vector3){2, -2, 2};  // Bottom-right

    isExploding = true;
}*/


// Função para atualizar os cubos de explosão (inutilizada no momento)
/*void UpdateExplosionCubes(float deltaTime) {
    if (!isExploding) return;

    for (int i = 0; i < 4; i++) {
        if (explosionCubes[i].ativo) {
            // Atualiza os cubos baseado na velocidade
            explosionCubes[i].posicao.x += explosionCubes[i].velocidade.x * deltaTime;
            explosionCubes[i].posicao.y += explosionCubes[i].velocidade.y * deltaTime;
            explosionCubes[i].posicao.z += explosionCubes[i].velocidade.z * deltaTime;

            // Applicar gravidade aos cubos (opcional)
            explosionCubes[i].velocidade.y -= GRAVIDADE * deltaTime;

            // Velocidade sobre o tempo retardada (para dar efeito)
            explosionCubes[i].velocidade.x *= 0.98f;
            explosionCubes[i].velocidade.y *= 0.98f;
            explosionCubes[i].velocidade.z *= 0.98f;

            // Desativa o cubo se estiver fora da tela (ou depois de um tempo)
            if (explosionCubes[i].posicao.y < 0.0f) {
                explosionCubes[i].ativo = false;
            }
        }
    }

    // Checa para ver se todos os cubos estão inativos
    bool allInativo = true;
    for (int i = 0; i < 4; i++) {
        if (explosionCubes[i].ativo) {
            allInativo = false;
            break;
        }
    }
    if (allInativo) {
        isExploding = false;
    }
}*/

// Função para desenhar a explosão dos cubos (por enquanto está falha e inutilizada)
/*void DrawExplosionCubes() {
    if (!isExploding) return;

    for (int i = 0; i < 4; i++) {
        if (explosionCubes[i].ativo) {
            DrawCube(explosionCubes[i].posicao, explosionCubes[i].tamanho.x, explosionCubes[i].tamanho.y, explosionCubes[i].tamanho.z, RED);
        }
    }
}*/

// Constantes para definir parallax
float scrollingBack = 0.0f;
float scrollingMid = 0.0f;
float scrollingFore = 0.0f;


//Função para desenhar textura de fundo
Texture2D skyTexture;
Texture2D backgroundTexture;
Texture2D midgroundTexture;
Texture2D foregroundTexture;

//Função para desenhar textura dos botões
Texture2D startTexture;
Texture2D quitTexture;
Texture2D scoreboardTexture;
Texture2D logoTexture;
Texture2D duplaTexture;

// Função para carregar a textura do Background
void LoadBackgroundTexture()
{
    skyTexture = LoadTexture("Imagens/background.png");
    backgroundTexture = LoadTexture("Imagens/prediosFundo.png"); // Ensure the path to your texture is correct
    midgroundTexture = LoadTexture("Imagens/prediosMeio.png");
    foregroundTexture = LoadTexture("Imagens/predios.png");

    //Textura dos botões

    logoTexture = LoadTexture ("Imagens/inf dash logo.png");
    startTexture = LoadTexture ("Imagens/start game.png");
    scoreboardTexture = LoadTexture ("Imagens/scoreboard.png");
    quitTexture = LoadTexture ("Imagens/quit game.png");
    duplaTexture = LoadTexture ("Imagens/cris e greg.png");
}

// Função para descarregar a textura do Background
void UnloadBackgroundTexture()
{
    UnloadTexture(skyTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(midgroundTexture);
    UnloadTexture(foregroundTexture);
}

// Função para desenhar o Background
void DrawBackground()
{
    DrawTexture(skyTexture, 0, 0, WHITE);
    DrawTexture(backgroundTexture, 0, 0, WHITE); // Desenha a textura começando pelo canto superior esquerdo
    DrawTexture(midgroundTexture, 0, 0, WHITE);
    DrawTexture(foregroundTexture, 0, 0, WHITE);
}

//-------------------//Função principal//------------------------//

int main(void)
{
    int i = 0, x = 0, sai_agora = 1, pos, terreno2 = 1, terreno[MAP_WIDTH][MAP_HEIGHT], ja_leu=0; //index para o vetor  e variável para o menu

    le_terreno(terreno);
    char letra[21]; //vetor para pegar o nome do jogador
    pontuacao top5[5];  //vetor com estruturas do tipo pontuacao
    /*strcpy(top5[0].nome, "Ednaldo Pereira");
    strcpy(top5[4].nome, "Manoel Gomes");
    strcpy(top5[1].nome, "Arthur Morgan");
    strcpy(top5[3].nome, "Trevor Philips");
    strcpy(top5[2].nome, "Monica de Sousa");
    top5[0].pontos = 100000;
    top5[1].pontos = 100000;
    top5[2].pontos = 100000;
    top5[3].pontos = 100000;
    top5[4].pontos = 100000;
    salva_arquivo(top5);*/
      for (int j=0; j<5;j++) {
      printf("antes de abrir o arquivo: nome %s pontos %d\n", top5[j].nome, top5[j].pontos);

    }
  //  le_arquivo(top5);  //preenche o vetor com as estruturas do arquivo "top_scores.bin"
    for (int cont=  0; cont < 21; cont++) //limpa o vetor do lixo
    {
        letra[cont] = '\0';
    }
    Color cor, cor2, cor3; //variáveis de cor
    Mouse m1; //mouse
    m1.c = 0; //começa como zero, porque não foi clicado ainda
    void Drawjogador(jogador *jogador)
    {
        // Desenha o jogador com a rotação aplicada
        rlPushMatrix();
        rlMultMatrixf(MatrixToFloat(jogador->transform));
        DrawCube(Vector3Zero(), jogador->tamanho.x, jogador->tamanho.y, jogador->tamanho.z, jogador->color);
        rlPopMatrix();
    }

    // Iniciar Janela
    InitWindow(LarguraTela, AlturaTela, "Geometry Dash 3D");

    LoadBackgroundTexture(); // Carrega a textura do Background
    // Definindo a camera para observar o mundo 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){2.0f, 2.0f, 10.0f};  // Posição da Camera, ligeiramente à direita
    camera.target = (Vector3){0.0f, 1.0f, 0.0f};    // Perspectiva da Camera
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Vetor de Camera (rotação em relação a perspectiva)
    camera.fovy = 48.0f;                              // Campo de visão Z
    obstaculo obstaculo = InitObstaculo();             //Inicialização do obstáculo
    jogador jogador = Initjogador();                //Inicialização do jogador
    initMap(terreno);

    SetTargetFPS(60);  // Setar para rodar a 60 frames p/ segundo

    // Loop principal do jogo
    while (!WindowShouldClose() && sai_agora){

        //-------Menu------//

        if (x == 0) //Como x começa como zero, essa é a primeira tela
        {
            BeginDrawing();
            jogador = Initjogador();
            ClearBackground((Color){121, 116, 137, 255});
            ShowCursor();

            m1.onde = GetMousePosition(); //pega posição do mouse
            if (m1.onde.y >= 200 && m1.onde.y <= 272 && m1.onde.x >= 468 && m1.onde.x <= 732) //Verifica se o mouse está em cima do botão de iniciar jogo para mudar a textura deste //Verifica se o mouse está em cima do botão de iniciar jogo para mudar a textura deste
            {
                cor = BLUE;
            }
            else
            {
                cor = VIOLET;
            }
            if (m1.onde.y >= 297 && m1.onde.y <= 369 && m1.onde.x >= 468 && m1.onde.x <= 732) //Verifica se o mouse está em cima do botão de pontuação para mudar a textura deste
            {
                cor2 = MAGENTA;
            }
            else
            {
                cor2 = BEIGE;
            }
            if (m1.onde.y >= 394 && m1.onde.y <= 466 && m1.onde.x >= 468 && m1.onde.x <= 732) //Verifica se o mouse está em cima do botão de sair para mudar a textura deste
            {
                cor3 = BROWN;
            }
            else
            {
                cor3 = DARKBLUE;
            }

            //Botões (suas texturas e localizações)
            DrawTexture(duplaTexture, 225, 225, WHITE);
            DrawTexture(logoTexture, 375, 50, WHITE);
            DrawTexture(startTexture, 468, 200, WHITE); //114 por 70   604 245
            DrawTexture(scoreboardTexture, 468, 297, WHITE); // 614 350
            DrawTexture(quitTexture, 468, 394, WHITE); //114 por 70   614 445
            EndDrawing();


        }
        if (x == 4)    //Tela intermediária para coletar o nome do jogador
        {
            strcpy(jogador.p1.nome, letra);  //Copia o que for escrito para o local onde é armazenado o nome do jogador
            i = Escreve(letra, i, &x); //Atualiza o i dentro da função e o retorna para a mesma
            BeginDrawing();

            ClearBackground(WHITE);
            DrawRectangle(310, 160, 800, 100, BLACK);
            DrawRectangle(300, 150, 800, 100, LIGHTGRAY);

            DrawText(letra, 320, 180, 50, RED); //Desenha o que está sendo escrito pelo vetor de char "letra"


            DrawText("Digite um nome de ate 20 caracteres: ", 300, 70, 20, BLACK);
            DrawText(TextFormat("Caracteres: %d/20", i), 350, 300, 20, BLACK); //Exibe quantos caracteres já foram digitados
            EndDrawing();
            troca_info_array(top5, jogador.p1, pos);  //Troca alguém do vetor pelas informações do jogador
            salva_txt(top5); //Salva as modificações feitas no arquivo
        }
        if ((m1.onde.y >= 375 && m1.onde.y <= 445 && m1.onde.x >= 500 && m1.onde.x <= 614 && IsMouseButtonPressed(m1.c)) || x == 2)  //Se o mouse estiver em cima do botão e for clicado,
        {
            x=2; //Deixa o x como 2, impossibilitando o retorno para a tela inicial
            sai_agora=0; //Fecha a janela do jogo
        }

        if (((m1.onde.y >= 280) && (m1.onde.y <= 350) && (m1.onde.x >= 500) && (m1.onde.x <= 614) && IsMouseButtonPressed(m1.c)) || x == 1) //Se o mouse estiver em cima do botão e for clicado,
        {
            x = 1; //Deixa o x como 1, impossibilitando o retorno para a tela inicial
            le_txt(top5); //Abre o arquivo binário/texto e pega suas informações, colocando-as em um vetor de estruturas
            if(IsKeyPressed(KEY_R)) {
            x = 0;
            }
            BeginDrawing();

            ClearBackground(WHITE);
            DrawRectangle(310, 20, 500, 550, BLACK);
            DrawRectangle(300, 10, 500, 550, LIGHTGRAY);
            DrawText(TextFormat("%s ---------- %d", top5[0].nome, top5[0].pontos), 450, 100, 20, BLACK); //exibe as informações do primeiro jogador do vetor
            DrawText(TextFormat("%s ---------- %d", top5[1].nome, top5[1].pontos), 450, 200, 20, BLACK); //exibe as informações do segundo jogador do vetor
            DrawText(TextFormat("%s ---------- %d", top5[2].nome, top5[2].pontos), 450, 300, 20, BLACK); //exibe as informações do terceiro jogador do vetor
            DrawText(TextFormat("%s ---------- %d", top5[3].nome, top5[3].pontos), 450, 400, 20, BLACK); //exibe as informações do quarto jogador do vetor
            DrawText(TextFormat("%s ---------- %d", top5[4].nome, top5[4].pontos), 450, 500, 20, BLACK); //exibe as informações do quinto jogador do vetor
            DrawText("Pressione R para voltar ao menu", 450, 530, 20, BLACK);
            DrawText("Leaderboard", 450, 50, 30, BLACK);
            EndDrawing();
        }

        if ((m1.onde.y >= 175 && m1.onde.y <= 245 && m1.onde.x >= 490 && m1.onde.x <= 604 && IsMouseButtonPressed(m1.c)) || x == 3) //Se o mouse estiver em cima do botão e for clicado,
        {
            x = 3; //Deixa o x como 3, impossibilitando o retorno para a tela inicial

            //Constante para efeitos parallax
            scrollingBack -= 0.1f;
            scrollingMid -= 0.25f;
            scrollingFore -= 0.5f;

            if (scrollingBack <= -backgroundTexture.width*2) scrollingBack = 0;
            if (scrollingMid <= -midgroundTexture.width*2) scrollingMid = 0;
            if (scrollingFore <= -foregroundTexture.width*2) scrollingFore = 0;


            // Atualiza Posição do Jogador
            Updatejogador(&jogador);

            // Update explosion cubes
            //AtualizaExplosaoCubos(deltaTime);

            // Atualiza camera para seguir o jogador
            camera.target = (Vector3){jogador.posicao.x + 2.5f, jogador.posicao.y + 2.0f, jogador.posicao.z};  // Offset da camera para a direita

            camera.position = (Vector3){jogador.posicao.x + 2.5f, jogador.posicao.y + 7.5f, jogador.posicao.z + 12.0f};  // Posiciona câmera ligeiramente à direita e acima

            // Começar a desenhar
            BeginDrawing();
            ClearBackground((Color){34, 36, 61, 255});

            //lógica do parallax
            DrawTextureEx (skyTexture, (Vector2){0, 20}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(backgroundTexture, (Vector2){scrollingBack, 20}, 0.0f, 1.0f, WHITE);
            DrawTextureEx(backgroundTexture, (Vector2){backgroundTexture.width + scrollingBack, 20}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(midgroundTexture, (Vector2){scrollingMid, 20}, 0.0f, 1.0f, WHITE);
            DrawTextureEx(midgroundTexture, (Vector2){midgroundTexture.width + scrollingMid, 20}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(foregroundTexture, (Vector2){scrollingFore, 20}, 0.0f, 1.0f, WHITE);
            DrawTextureEx(foregroundTexture, (Vector2){foregroundTexture.width + scrollingFore, 20}, 0.0f, 1.0f, WHITE);



            // Desenhar a cena 3D
            BeginMode3D(camera);

            // Desenha chão
            DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){50.0f, 50.0f}, (Color){24, 19, 42, 255});

            // Desenha obstáculos
            DrawMap();

            // Desenha Jogador
            //if(jogador.isAlive == true){
            Drawjogador(&jogador);
            //} else if (jogador.isAlive == false){
            //DrawExplosionCubes(deltaTime);
            //}
            //DrawCubeWires(jogador.posicao, jogador.tamanho.x, jogador.tamanho.y, jogador.tamanho.z, BLACK);

            EndMode3D();

            DrawText("3D INF-Dash", 10, 20, 20, DARKGRAY); // Desenha o título do jogo
            DrawText(TextFormat("Tentativas: %d", jogador.contm), 10, 40, 20, WHITE); // Desenha o texto do contador de morte
            if (jogador.contm > 10)   //Se o jogador tiver morrido mais do que 10 vezes,
            {
                DrawText("Continue tentando! Voce consegue! :)", 150, 30, 20, GREEN); //Exibe uma mensagem motivacional :D
            }
            EndDrawing();
            if (jogador.posicao.x > 300)   //Se o jogador cruzar certa posição (fim da fase)
            {
                atualiza_pontos(&jogador); //Atualiza os pontos do jogador
                x = verifica_se_entra_no_leaderboard(&jogador, &pos, top5);  //Checa se ele deve entrar no leaderboard

            }
        }
    }
    UnloadTexture (duplaTexture);
    UnloadTexture(logoTexture);       //Descarrega as texturas
    UnloadTexture(scoreboardTexture);
    UnloadTexture(quitTexture);
    UnloadTexture(startTexture);
    // Desiniciar
    CloseWindow();

    return 0;
}
