#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"

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
// Types e Structs
//----------------------------------------------------------------------------------
typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
} Player;

typedef struct Enemy {
    Vector2 position;
    float speed;
    int direction; // 1 para direita, -1 para esquerda
} Enemy;

typedef struct Enemies {
    Enemy enemy[ENEMY_MAX_NUMBER];
    int quant;
} Enemies;

typedef struct EnvItem {
    Vector2 position;
    int blocking;
    Color color;
} EnvItem;

typedef struct EnvItems {
    EnvItem item[MAPA_X * MAPA_Y];
    int quant;
} EnvItems;

typedef struct StairLow {
    Vector2 position;
    Color color;
} StairLow;

typedef struct StairMiddle {
    Vector2 position;
    Color color;
} StairMiddle;

typedef struct StairHigh {
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

//------------------------------------------------------------------------------------
// Função main
//------------------------------------------------------------------------------------
int main(void)
{
    // Variáveis de objetos do jogo
    //--------------------------------------------------------------------------------------

    // Variável para o player no nível
    // Valores para teste, implementar carregamento a partir do mapa depois
    Player player = {0};

    // Variável para os inimigos no nível
    Enemies enemies = {0};

    // Variável para as plataformas do nível
    // Valores para teste, implementar carregamento a partir do mapa depois
    EnvItems envItems = {0};

    // Variáveis para as escadas do nível
    // Valores para teste, implementar carregamento a partir do mapa depois
    Stairs stairs = {0};

    // Variável para os portais do nível
    Portal portal = {0};

    // Função para carregar as posições do player, inimigos, itens do ambiente e escadas a partir do mapa
    LoadFromMap(&player, &enemies, &envItems, &stairs, &portal);

    //--------------------------------------------------------------------------------------

    // Inicialização
    //--------------------------------------------------------------------------------------

	// Flags para o uso de vsync e dpi alto em telas de alta resolução
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Carregando e setando o diretório de recursos
	SearchAndSetResourceDir("resources");

	// Carregando a textura do coelho
	Texture wabbit = LoadTexture("wabbit_alpha.png");

    // Criando a janela
    InitWindow(TELA_ALTURA, TELA_LARGURA, "Donkey Kong em C com Raylib");

    // Criando a câmera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ TELA_ALTURA / 2.0f, TELA_LARGURA / 2.0f };
    camera.offset = (Vector2){ TELA_LARGURA/2.0f, TELA_ALTURA/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Setando o FPS
    SetTargetFPS(FPS);

    //--------------------------------------------------------------------------------------

    // Loop principal
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        // Atualizando o player
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
        //----------------------------------------------------------------------------------
    }

    // Encerramento da janela
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

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