#include "raylib.h"
#include "raymath.h"

#define TELA_ALTURA 900
#define TELA_LARGURA 900

#define COR_PLAYER RED
#define COR_INIMIGO BLUE
#define COR_PLATAFORMA GRAY
#define COR_ESCADA_BAIXO YELLOW
#define COR_ESCADA_MEIO ORANGE
#define COR_ESCADA_CIMA GREEN
#define COR_PORTAL PURPLE

#define MAPA_X 30
#define MAPA_Y 30

#define G 400
#define PLAYER_JUMP_SPD 225.0f
#define PLAYER_HOR_SPD 150.0f
#define ENEMY_MAX_NUMBER 10
#define ENEMY_HOR_SPD 100.0f

#define FPS 60

//----------------------------------------------------------------------------------
// Enums
//----------------------------------------------------------------------------------
typedef enum enum_estado_main
{
    MENU,
    SCORE,
    JOGO,
    FIM
} EstadoMain;

typedef enum enum_estado_jogo
{
    INICIALIZANDO,
    RODANDO,
    PAUSADO,
    VITORIA,
    DERROTA
} EstadoJogo;
//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------

// Struct obrigatória, conforme especificada na documentação do trabalho
// Guarda o score do jogador
typedef struct struct_score
{
    char nome[20];
    int tempo;
} Score;

typedef struct Player
{
    Vector2 position;
    int tempoVivo;
    int isVivo;
    float speed;
    bool canJump;
} Player;

typedef struct Enemy
{
    Vector2 position;
    float speed;
    int direction; // 1 para direita, -1 para esquerda
} Enemy;

typedef struct Enemies
{
    Enemy enemy[ENEMY_MAX_NUMBER];
    int quant;
} Enemies;

typedef struct EnvItem
{
    Vector2 position;
    int blocking;
    Color color;
} EnvItem;

typedef struct EnvItems
{
    EnvItem item[MAPA_X * MAPA_Y];
    int quant;
} EnvItems;

typedef struct StairLow
{
    Vector2 position;
    Color color;
} StairLow;

typedef struct StairMiddle
{
    Vector2 position;
    Color color;
} StairMiddle;

typedef struct StairHigh
{
    Vector2 position;
    Color color;
} StairHigh;

typedef struct Stairs {
    StairLow stairLow[MAPA_X * MAPA_Y];
    StairMiddle stairMiddle[MAPA_X * MAPA_Y];
    StairHigh stairHigh[MAPA_X * MAPA_Y];
    int quantLow;
    int quantMiddle;
    int quantHigh;
} Stairs;

typedef struct Portal {
    Vector2 position;
    Color color;
} Portal;

//----------------------------------------------------------------------------------
// Declarações implícitas de funções
//----------------------------------------------------------------------------------
void LoadFromMap(Player *player, Enemies *enemies, EnvItems *envitems, Stairs *stairs, Portal *portal);
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateEnemy(Enemy *enemy, EnvItem *envItems, int envItemsLength, float delta);

EstadoJogo LoopMenu(void);
EstadoJogo LoopScore(void);
EstadoJogo LoopJogo(void);

//------------------------------------------------------------------------------------
// Função main
//------------------------------------------------------------------------------------
int main(void)
{
    // Inicializando o estado do jogo
    EstadoMain estadoMain = MENU;

    // Flags para o uso de vsync e dpi alto em telas de alta resolução
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Criando a janela
    InitWindow(TELA_ALTURA, TELA_LARGURA, "Donkey Kong em C com Raylib");

    // Setando o FPS
    SetTargetFPS(FPS);

    // Loop da janela do Raylib, só termina ao encerrar o jogo
    while (!WindowShouldClose())
    {
        // Máquina de estado do main
        switch(estadoMain)
        {
            case MENU:
                // Jogo no menu, pode ir para score, jogo ou fim
                estadoMain = LoopMenu();
                break;
            case SCORE:
                // Jogo no score, pode voltar ao menu
                estadoMain = LoopScore();
                break;
            case JOGO:
                // Jogo rodando, pode voltar ao menu ou ir para o fim
                estadoMain = LoopJogo();
                break;
            case FIM:
                // Fim do jogo, encerrar loop e janela
                CloseWindow();
                break;
        }
    }

    return 0;
}

void LoadFromMap(Player *player, Enemies *enemies, EnvItems *envitems, Stairs *stairs, Portal *portal)
{
    // Carregar mapa
    char mapa[MAPA_X][MAPA_Y] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'F', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E'},
        {'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'},
    };

    for (int i = 0; i < MAPA_X; i++)
    {
        for (int j = 0; j < MAPA_Y; j++)
        {
            char c = mapa[i][j];

            switch (c)
            {
                case 'P':
                    // Posição inicial do player
                    player->position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    player->speed = PLAYER_HOR_SPD;
                    player->canJump = false;
                    break;
                case 'E':
                    // Posição inicial de um inimigo
                    enemies->enemy[enemies->quant].position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    enemies->enemy[enemies->quant].speed = ENEMY_HOR_SPD;
                    enemies->enemy[enemies->quant].direction = 1;
                    enemies->quant++;
                    break;
                case 'Z':
                    // Plataforma
                    envitems->item[envitems->quant].position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    envitems->item[envitems->quant].blocking = true;
                    envitems->item[envitems->quant].color = COR_PLATAFORMA;
                    envitems->quant++;
                    break;
                case 'S':
                    // Escada (baixo para cima)
                    stairs->stairLow[stairs->quantLow].position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    stairs->stairLow[stairs->quantLow].color = COR_ESCADA_BAIXO;
                    stairs->quantLow++;
                    break;
                case 'D':
                    stairs->stairHigh[stairs->quantHigh].position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    stairs->stairHigh[stairs->quantHigh].color = COR_ESCADA_CIMA;
                    stairs->quantHigh++;
                    break;
                case 'F':
                    // Portal para o próximo nível
                    portal->position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    portal->color = COR_PORTAL;
                    break;
                case 'H':
                    // Meio da escada (somente visual)
                    stairs->stairMiddle[stairs->quantMiddle].position = (Vector2){ j * 30 + 10, i * 30  + 10};
                    stairs->stairMiddle[stairs->quantMiddle].color = COR_ESCADA_MEIO;
                    stairs->quantMiddle++;
                    break;
                default:
                    // Espaço vazio, não precisa fazer nada

                    break;
            }
        }
        
    }
    
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    if (IsKeyDown(KEY_LEFT))
    {
        player->position.x -= PLAYER_HOR_SPD*delta;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player->position.x += PLAYER_HOR_SPD*delta;
    }
    if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    bool hitObstacle = false;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->position.x <= p->x &&
            ei->position.x + 40 >= p->x &&
            ei->position.y >= p->y&&
            ei->position.y <= p->y + player->speed*delta)
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->position.y;
            break;
        }
    }

    if (!hitObstacle)
    {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
}

void UpdateEnemy(Enemy *enemy, EnvItem *envItems, int envItemsLength, float delta)
{
    // Verificando se o inimigo bateu em uma parede e invertendo a direção se necessário
        if (enemy->direction == 1)
        {
            if (enemy->position.x >= TELA_LARGURA - 20)
            {
                enemy->direction = -1;
            }
        }
        else if (enemy->direction == -1)
        {
            if (enemy->position.x <= 20)
            {
                enemy->direction = 1;
            }
        }

    if (enemy->direction == 1)
    {
        enemy->position.x += ENEMY_HOR_SPD*delta;
    }
    else if (enemy->direction == -1)
    {
        enemy->position.x -= ENEMY_HOR_SPD*delta;
    }

    bool hitObstacle = false;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *e = &(enemy->position);
        if (ei->blocking &&
            ei->position.x <= e->x &&
            ei->position.x + 40 >= e->x &&
            ei->position.y >= e->y &&
            ei->position.y <= e->y + enemy->speed*delta)
        {
            hitObstacle = true;
            enemy->speed = 0.0f;
            e->y = ei->position.y;
            break;
        }
    }

    if (!hitObstacle)
    {
        enemy->position.y += enemy->speed*delta;
        enemy->speed += G*delta;
    }
}

EstadoJogo LoopMenu(void)
{
    EstadoMain estadoMain = MENU;
    int opcaoSelecionada = 0;   // 0=Iniciar, 1=Scores, 2=Fim
    int loopMenu = 1;

    while(loopMenu)
    {
        // Navegação por teclas
        if (IsKeyPressed(KEY_DOWN))
            opcaoSelecionada = (opcaoSelecionada + 1) % 3;
        if (IsKeyPressed(KEY_UP))
            opcaoSelecionada = (opcaoSelecionada == 0) ? 2 : opcaoSelecionada - 1;

        // Navegação pelo mouse
        Vector2 mouse = GetMousePosition();
        Rectangle retIniciar = { largura/2 - 100, 200, 200, 40 };
        Rectangle retScores  = { largura/2 - 100, 260, 200, 40 };
        Rectangle retSair    = { largura/2 - 100, 320, 200, 40 };

        // Seleção pelo mouse
        if (CheckCollisionPointRec(mouse, retIniciar))
        {
            opcaoSelecionada = 0;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estadoMain = JOGO;   //Muda para o jogo
                loopMenu = 0;
            }
        } 
        else if (CheckCollisionPointRec(mouse, retScores))
        {
            opcaoSelecionada = 1;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estadoMain = SCORE; //Muda para scores
                loopMenu = 0;
            }
        }
        else if (CheckCollisionPointRec(mouse, retSair))
        {
            opcaoSelecionada = 2;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estadoMain = FIM;   // Sai do programa
                loopMenu = 0;
            }
        }

        // Seleção com enter
        if (IsKeyPressed(KEY_ENTER)) {
            if (opcaoSelecionada == 0)
            {
                estadoMain = JOGO;
                loopMenu = 0;
            }
            else if (opcaoSelecionada == 1)
            {
                estadoMain = SCORE;
                loopMenu = 0;
            }
            else if (opcaoSelecionada == 2)
            {
                estadoMain = FIM;
                loopMenu = 0;
            }
        }

        // ----- Desenho do menu -----
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("MENU PRINCIPAL", largura/2 - MeasureText("MENU PRINCIPAL", 30)/2, 80, 30, DARKGRAY);

        Color corIniciar = (opcaoSelecionada == 0) ? RED : DARKGRAY;
        Color corScores  = (opcaoSelecionada == 1) ? RED : DARKGRAY;
        Color corSair    = (opcaoSelecionada == 2) ? RED : DARKGRAY;

        DrawText("Iniciar Jogo", retIniciar.x + 10, retIniciar.y + 10, 20, corIniciar);
        DrawText("Scores",       retScores.x + 50,  retScores.y + 10,  20, corScores);
        DrawText("Sair",         retSair.x + 70,    retSair.y + 10,    20, corSair);

        DrawRectangleLinesEx(retIniciar, 1, DARKGRAY);
        DrawRectangleLinesEx(retScores, 1, DARKGRAY);
        DrawRectangleLinesEx(retSair, 1, DARKGRAY);
        EndDrawing();
    }

    return estadoMain;
}

EstadoJogo LoopScore(void)
{

}

EstadoJogo LoopJogo(void)
{
    // Estado do jogo
    EstadoJogo estadoJogo = INICIALIZANDO;

    // Estado do main, será enviado como resposta no fim do loop do jogo
    EstadoMain estadoMain = JOGO;

    // Variável para o player no nível
    Player player = {0};

    // Variável para os inimigos no nível
    Enemies enemies = {0};

    // Variável para as plataformas do nível
    EnvItems envItems = {0};

    // Variáveis para as escadas do nível
    Stairs stairs = {0};

    // Variável para os portais do nível
    Portal portal = {0};

    // Função para carregar as posições do player, inimigos, itens do ambiente e escadas a partir do mapa
    LoadFromMap(&player, &enemies, &envItems, &stairs, &portal);

    // Máquina de estado do jogo
    switch(estadoJogo)
    {
        case INICIALIZANDO:
            // Inicializando o jogo, carrega dados do mapa para variáveis
            // Muda de estado para RODANDO
            // Não altera o estado do main

            // Inicializar variáveis do player, inimigos, itens, escadas e portal
            // Não esquecer de salvar o tempo de vida do jogador entre inicializações

            estadoJogo = RODANDO;
            break;
        case RODANDO:
            // Jogo rodando normalmente, processar input, atualizar e desenhar a tela
            // Muda de estado para PAUSADO se receber input para tal
            // Não altera o estado do main

            float deltaTime = GetFrameTime();
            UpdatePlayer(&player, envItems.item, envItems.quant, deltaTime);

            // Atualizando os inimigos
            for (int i = 0; i < enemies.quant; i++)
            {
                Enemy *e = enemies.enemy + i;
                
                // Atualizando a posição do inimigo
                UpdateEnemy(e, envItems.item, envItems.quant, deltaTime);
            }

            // Resetando a posição do player se R for pressionado
            // Para testes, remover depois
            if (IsKeyPressed(KEY_R))
            {
                player.position = (Vector2){ 450, 280 };
            }

            if (IsKeyPressed(KEY_P))
            {
                estadoJogo = PAUSADO;
                break;
            }

            // Desenhando o frame
            //----------------------------------------------------------------------------------
            BeginDrawing();

            // Limpando o fundo e setando para a cor LIGHTGRAY
            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);

                // Desenhando as escadas
                for (int i = 0; i < stairs.quantLow; i++)
                {
                    DrawRectangleRec((Rectangle){ stairs.stairLow[i].position.x - 10, stairs.stairLow[i].position.y - 10, 40.0f, 40.0f }, 
                                     stairs.stairLow[i].color);
                }
                for (int i = 0; i < stairs.quantMiddle; i++)
                {
                    DrawRectangleRec((Rectangle){ stairs.stairMiddle[i].position.x - 10, stairs.stairMiddle[i].position.y - 10, 40.0f, 40.0f }, 
                                     stairs.stairMiddle[i].color);
                }
                for (int i = 0; i < stairs.quantHigh; i++)
                {
                    DrawRectangleRec((Rectangle){ stairs.stairHigh[i].position.x - 10, stairs.stairHigh[i].position.y - 10, 40.0f, 40.0f }, 
                                     stairs.stairHigh[i].color);
                }
                
                // Desenhando o portal
                DrawRectangleRec((Rectangle){ portal.position.x - 10, portal.position.y - 10, 40.0f, 40.0f }, 
                                     portal.color);


                // Desenhando as plataformas do ambiente
                for (int i = 0; i < envItems.quant; i++)
                {
                    DrawRectangleRec((Rectangle){ envItems.item[i].position.x - 10, envItems.item[i].position.y, 40.0f, 40.0f }, 
                                     envItems.item[i].color);
                }

                // Desenhando o player
                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40.0f, 40.0f };
                DrawRectangleRec(playerRect, COR_PLAYER);
                DrawCircleV(player.position, 5.0f, GOLD);

                // Desenhando os inimigos
                for (int i = 0; i < enemies.quant; i++)
                {
                    Rectangle enemyRect = { enemies.enemy[i].position.x - 20, enemies.enemy[i].position.y - 40, 40.0f, 40.0f };
                    DrawRectangleRec(enemyRect, COR_INIMIGO);
                    DrawCircleV(enemies.enemy[i].position, 5.0f, DARKBLUE);
                }

            EndMode2D();

            DrawText("Controls:", 20, 20, 10, BLACK);
            DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
            DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
            DrawText("- R to reset position", 40, 100, 10, DARKGRAY);

            EndDrawing();
            break;
        case PAUSADO:
            // Jogo pausado, processar input para voltar a rodar, ir para o menu principal ou encerrar o jogo
            // Muda de estado para RODANDO ou FINALIZANDO, dependendo do input recebido
            // Pode alterar o estado do main para MENU ou FIM, dependendo do input recebido

            break;
        case VITORIA:
            // Jogo com vitória, mostrar mensagem e guardar o score
            // Muda de estado para FINALIZANDO
            // Altera o estado do main para MENU

            break;
        case DERROTA:
            // Jogo com derrota, mostrar mensagem e guardar o score
            // Muda de estado para FINALIZANDO
            // Altera o estado do main para MENU

            break;
    }

    return estadoMain;

}
