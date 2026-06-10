/*
    ### Trabalho Final de Algoritmos e Programação - 2026/1 ###

    Grupo:
        Ariel Nathan Scheffer de Souza      -   00xxxxxx 
        Eduardo Pereira Moreira             -   00228404

    Objetivo Principal:
        Implementação de uma versao simplificada do jogo conhecido como Donkey Kong, utilizando
        a linguagem C e a biblioteca gráfica Raylib.


    ## Visao Geral do Jogo ##
    • O jogador possui as seguintes informacoes:
        – Localizacao: coordenadas da linha e coluna (linha, coluna)
        – Tempo de execuc¸ao ou pontuacao
        – Estado (ativo ou morto)

    • Os inimigos possuem as seguintes informacoes:
        – Localizacao: coordenadas de linha e coluna (linha, coluna)
        – Direcao de movimento (esquerda ou direita)

    • A tela do jogo, implementada atraves da biblioteca Raylib, deve conter:
        – Area de jogo com o mapa carregado
        – Informacoes de tempo ou pontuacao
        – Indicacao da fase atual 


    ## Requisitos Minimos ##
    Implementações do Trabalho Final devem respeitar os seguintes requisitos mínimos:

    • O jogo começa com a apresentação de um menu inicial com as opções:
        1. Novo jogo: inicia uma nova partida
        2. Ranking: apresenta os melhores resultados
        3. Sair: encerra o jogo

    • O jogo deve ser implementado em modo grafico utilizando a biblioteca Raylib.
      Podem ser utilizados sprites (imagens) ou apenas formas geométricas
      (por exemplo, um quadrado azul representando o jogador e triangulos brancos representando os inimigos).
      Sprites exemplo podem ser encontradas no Moodle. Grupos podem utilizar outras sprites ou criar as suas.

    • O jogo nao deve possuir atrasos artificiais nas ações do jogador.

    • O mapa do jogo devera ser carregado a partir de arquivos texto nomeados "mapaX.txt", onde X indica
      a fase correspondente.

    • O placar deve ser salvo e carregado a partir de um arquivo placar.bin.
      Esse contem ate 10 nomes de jogadores e suas respectivas pontuacoes anteriormente salvas.
      O arquivo placar.bin sera formado por um arranjo de dez structs, dados por:

        typedef struct tipo_placar
        {
        char nome[20];
        int time;
        } TIPO_PLACAR;

      Quando o jogo for encerrado, o programa deve verificar se a pontuacao do jogador atual é uma das dez melhores.
      Caso afirmativo, perguntar o nome do jogador e atualizar o arquivo placar.bin.
      O arquivo deve ser mantido sempre ordenado em ordem decrescente de pontuação.
      O sistema de ranking deve ser baseado em tempo (menor é melhor) ou pontuação (maior é melhor).
      A escolha deve ser consistente com o campo armazenado no placar.bin.

    • O programa deve ser capaz de interpretar corretamente a matriz do mapa com tamanho fixo de
      30 linhas por 30 colunas.

    • O jogo deve permitir multiplas fases, carregadas dinamicamente.
      O jogo deve ser entregue com pelo menos 2 fases diferentes dos exemplos.
      Mais um mapa sera disponibilizado no dia da apresentacao para testes.

    • A interacao do jogador com o jogo se da ao pressionar teclas especıficas. As acoes e teclas sao as seguintes:

        Tecla                   -   Acao

        ← / → ou (’A’ / ’D’)    -   Movimento horizontal
        ↑ ou (’W’)              -   Subir escada
        ↓ ou (’S’)              -   Descer escada
        TAB                     -   Pausar o jogo

    • O jogo deve possuir sistema de pausa funcional, interrompendo completamente a execucao da logica do jogo
      e permitindo posterior volta a partir do mesmo ponto e mesmas condicoes.
      O menu de pausa deve possuir ao menos as seguintes opcoes:

        Opcao                   -   Significado

        Continuar               -   Retoma o jogo do ponto em que foi pausado
        Voltar ao menu          -   Retorna ao menu principal
        Sair                    -   Fecha o jogo

    • O jogador nao deve atravessar plataformas e nem caminhar fora de plataformas.
    • Os inimigos devem se movimentar sobre as plataformas sem atravessar obstaculos.
    • Ao colidir com inimigos, isto ´e, quando o jogador se encontrar na mesma posic¸ao que o inimigo,
      o jogo deve reiniciar (ou consumir vida, caso implementado como extra).
    • O jogo deve possuir sistema de ranking baseado em tempo ou pontuacao.
    • O jogo deve funcionar corretamente mesmo com modificacoes externas nos arquivos de mapa (leitura dinamica).
*/

//----------------------------------------------------------------------------------
// Bibliotecas
//----------------------------------------------------------------------------------

#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Constantes
//----------------------------------------------------------------------------------

// Mapa
#define MAPA_X 30
#define MAPA_Y 30

// Score
#define NOME_PLACAR_MAX 20

// Player
#define PLAYER_VELOCIDADE_HORIZONTAL 150.0f
#define PLAYER_VELOCIDADE_PULO 225.0f
#define PLAYER_ALTURA 20
#define PLAYER_COMPRIMENTO 20
#define PLAYER_COR RED

// Inimigos
#define INIMIGO_MAX 8
#define INIMIGO_VELOCIDADE_HORIZONTAL 100.0f
#define INIMIGO_ALTURA 20
#define INIMIGO_COMPRIMENTO 20
#define INIMIGO_COR BLUE

// Plataformas
#define PLATAFORMA_MAX MAPA_X * MAPA_Y
#define PLATAFORMA_ALTURA 30
#define PLATAFORMA_COMPRIMENTO 30
#define PLATAFORMA_COR GRAY

// Escadas
#define ESCADA_BAIXO_MAX MAPA_X * 2
#define ESCADA_BAIXO_COR YELLOW
#define ESCADA_BAIXO_ALTURA 30
#define ESCADA_BAIXO_COMPRIMENTO 30

#define ESCADA_MEIO_MAX MAX_ESCADAS_BAIXO * MAPA_Y
#define ESCADA_MEIO_COR ORANGE
#define ESCADA_MEIO_ALTURA 30
#define ESCADA_MEIO_COMPRIMENTO 30

#define ESCADA_CIMA_MAX MAX_ESCADAS_BAIXO
#define ESCADA_CIMA_COR GREEN
#define ESCADA_CIMA_ALTURA 30
#define ESCADA_CIMA_COMPRIMENTO 30

// Portal
#define PORTAL_ALTURA 40
#define PORTAL_COMPRIMENTO 30
#define PORTAL_COR PURPLE

// Janela do Jogo
#define TELA_ALTURA MAPA_X * 30
#define TELA_LARGURA MAPA_Y * 30
#define FPS 60

// Simulação de Gravidade para Pulo/Queda do Player e Inimigos
#define GRAVIDADE 400

//----------------------------------------------------------------------------------
// Enums
//----------------------------------------------------------------------------------
typedef enum enum_estado_jogo {
    MENU,
    SCORE,
    JOGO,
    FIM
} EstadoJogo;

typedef enum enum_estado_jogo_interno {
    CARREGAMENTO,
    JOGO,
    ENCERRAMENTO
} EstadoJogoInterno;

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------
typedef struct struct_score {
    char nome[NOME_PLACAR_MAX];
    int tempoVivo;
    int faseCompletada;
} Score;

typedef struct struct_posicao {
    float x;
    float y;
} Posicao;

typedef struct struct_player {
    Score score;
    Posicao posicao;
    int isVivo;
    float velocidade;
    int podePular;
    int altura;
    int comprimento;
    Color cor;
} Player;

typedef struct struct_inimigo {
    Posicao posicao;
    float velocidade;
    int direcao;
    int altura;
    int comprimento;
    Color cor;
} Inimigo;

typedef struct struct_inimigos {
    Inimigo inimigo[INIMIGO_MAX];
    int quantInimigos;
} Inimigos;

typedef struct struct_plataforma {
    Posicao posicao;
    int altura;
    int comprimento;
    Color cor;
} Plataforma;

typedef struct struct_plataformas {
    Plataforma plataforma[PLATAFORMA_MAX];
    int quantPlataformas;
} Plataformas;

typedef struct struct_escada_baixo {
    Posicao posicao;
    EscadaCima *escadaCima;
    int altura;
    int comprimento;
    Color cor;
} EscadaBaixo;

typedef struct struct_escada_meio {
    Posicao posicao;
    int altura;
    int comprimento;
    Color cor;
} EscadaMeio;

typedef struct struct_escada_cima {
    Posicao posicao;
    EscadaBaixo *escadaBaixo;
    int altura;
    int comprimento;
    Color cor;
} EscadaCima;

typedef struct struct_escadas {
    EscadaBaixo escadaBaixo[ESCADA_BAIXO_MAX];
    int quantEscadasBaixo;
    EscadaMeio escadaMeio[ESCADA_MEIO_MAX];
    int quantEscadasMeio;
    EscadaCima escadaCima[ESCADA_CIMA_MAX];
    int quantEscadasCima;
} Escadas;

typedef struct struct_portal {
    Posicao posicao;
    int altura;
    int comprimento;
    Color cor;
} Portal;

//----------------------------------------------------------------------------------
// Funções
//----------------------------------------------------------------------------------

// Funções para criação padronizada de estruturas
// Struct devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Score GetScorePadrao();
Posicao GetPosicaoPadrao();
Player GetPlayerPadrao();
Inimigo GetInimigoPadrao();
Inimigos GetInimigosPadrao();
Plataforma GetPlataformaPadrao();
Plataformas GetPlataformasPadrao();
EscadaBaixo GetEscadaBaixoPadrao();
EscadaMeio GetEscadaMeioPadrao();
EscadaCima GetEscadaCimaPadrao();
Escadas GetEscadasPadrao();
Portal GetPortalPadrao();

// Funções de lógica do jogo
// Carregar o mapa, atualizar a posição do jogador, dos inimigos, etc.
void CarregarMapa(int numeroFase, char mapa[MAPA_X][]);
void MatrizParaStructs(char mapa[MAPA_X][], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal);
void AtualizarPlayer(Player *player, Plataformas *plataformas, float delta);
void AtualizarInimigos(Inimigos *inimigos, Plataformas *plataformas, float delta);

// Funções para os diferentes loops de tela: Menu, Score e Jogo
// Quando encerram, retornam o próximo estado a ser desenhado
EstadoMain LoopMenu();
EstadoMain LoopScore();
EstadoMain LoopJogo();

//----------------------------------------------------------------------------------
// Função Main
//----------------------------------------------------------------------------------
int main(void)
{
    // Inicializando o estado do jogo
    EstadoMain estado = MENU;

    // Flags para o uso de vsync e dpi alto em telas de alta resolução
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Criando a janela
    InitWindow(TELA_ALTURA, TELA_LARGURA, "Donkey Kong em C com Raylib");

    // Setando o FPS
    SetTargetFPS(FPS);

    // Loop da janela do Raylib, só termina ao encerrar o jogo
    while (!WindowShouldClose())
    {
        switch(estado)
        {
            case MENU:
                // Jogo no menu, pode ir para score, jogo ou fim
                estadoMain = LoopMenu();
            case SCORE:
                // Jogo no score, pode voltar ao menu
                estadoMain = LoopScore();
                break;
            case JOGO:
                // Jogo rodando, pode voltar ao menu ou ir para o fim
                estadoMain = LoopJogo();
                break;
            case FIM:
                // Fim do jogo, encerrar loop/janela
                CloseWindow();
                break;
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------
// Definições de Funções
//----------------------------------------------------------------------------------

Score GetScorePadrao()
{
    Score scorePadrao = {
        {"Mario"},
        0,
        -1
    };

    return scorePadrao;
}
Posicao GetPosicaoPadrao()
{
    Posicao posicaoPadrao = {
        0.0,
        0.0
    };

    return posicaoPadrao;
}
Player GetPlayerPadrao()
{
    Player playerPadrao = {
        GetScorePadrao(),
        GetPosicaoPadrao(),
        1,
        0,
        1,
        PLAYER_ALTURA,
        PLAYER_COMPRIMENTO,
        PLAYER_COR
    };

    return playerPadrao;
}
Inimigo GetInimigoPadrao()
{
    Inimigo inimigoPadrao = {
        GetPosicaoPadrao(),
        0,
        1,
        INIMIGO_ALTURA,
        INIMIGO_COMPRIMENTO,
        INIMIGO_COR
    };

    return inimigoPadrao;
}
Inimigos GetInimigosPadrao()
{
    Inimigos inimigosPadrao = {0};

    for(int i = 0; i < INIMIGO_MAX; i++)
    {
        inimigosPadrao.inimigo[i] = GetInimigoPadrao();
    }

    inimigosPadrao.quantInimigos = 0;

    return inimigosPadrao;
}
Plataforma GetPlataformaPadrao()
{
    Plataforma plataformaPadrao = {
        getPosicaoPadrao(),
        PLATAFORMA_ALTURA,
        PLATAFORMA_COMPRIMENTO,
        PLATAFORMA_COR
    };

    return plataformaPadrao;
}
Plataformas GetPlataformasPadrao()
{
    Plataformas plataformasPadrao = {0};

    for(int i = 0; i < PLATAFORMA_MAX; i++)
    {
        plataformasPadrao.plataforma[i] = GetPlataformaPadrao();
    }

    plataformasPadrao.quantPlataformas = 0;

    return plataformasPadrao;
}
EscadaBaixo GetEscadaBaixoPadrao()
{
    EscadaBaixo escadaBaixoPadrao = {
        GetPosicaoPadrao(),
        NULL,
        ESCADA_BAIXO_ALTURA,
        ESCADA_BAIXO_COMPRIMENTO,
        ESCADA_BAIXO_COR
    };

    return escadaBaixoPadrao;
}
EscadaMeio GetEscadaMeioPadrao()
{
    EscadaMeio escadaMeioPadrao = {
        GetPosicaoPadrao(),
        ESCADA_MEIO_ALTURA,
        ESCADA_MEIO_COMPRIMENTO,
        ESCADA_MEIO_COR
    };

    return escadaMeioPadrao;
}
EscadaCima GetEscadaCimaPadrao()
{
    EscadaCima escadaCimaPadrao = {
        GetPosicaoPadrao(),
        NULL,
        ESCADA_CIMA_ALTURA,
        ESCADA_CIMA_COMPRIMENTO,
        ESCADA_CIMA_COR
    };

    return escadaCimaPadrao;
}
Escadas GetEscadasPadrao()
{
    Escadas escadasPadrao = {0};

    for(int i = 0; i < ESCADA_BAIXO_MAX; i++)
    {
        escadasPadrao.escadaBaixo[i] = GetEscadaBaixoPadrao();
    }

    for(int i = 0; i < ESCADA_MEIO_MAX; i++)
    {
        escadasPadrao.escadaMeio[i] = GetEscadaMeioPadrao();
    }

    for(int i = 0; i < ESCADA_CIMA_MAX; i++)
    {
        escadasPadrao.escadaCima[i] = GetEscadaCimaPadrao();
    }

    escadasPadrao.quantEscadasBaixo = 0;
    escadasPadrao.quantEscadasMeio = 0;
    escadasPadrao.quantEscadasCima = 0;

    return escadasPadrao;
}
Portal GetPortalPadrao()
{
    Portal portalPadrao = {
        GetPosicaoPadrao(),
        PORTAL_ALTURA,
        PORTAL_COMPRIMENTO,
        PORTAL_COR
    };

    return portalPadrao;
}

void CarregarMapa(int numeroFase, char mapa[MAPA_X][])
{

}
void MatrizParaStructs(char mapa[MAPA_X][], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal)
{
    
}
void AtualizarPlayer(Player *player, Plataformas *plataformas, float delta)
{
    Plataforma *plataforma;

    int bateuEmAlgo;

    if (IsKeyDown(KEY_LEFT))
    {
        player->posicao.x -= (PLAYER_VELOCIDADE_HORIZONTAL * delta);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player->posicao.x += (PLAYER_VELOCIDADE_HORIZONTAL * delta);
    }
    if (IsKeyDown(KEY_SPACE) && player->podePular)
    {
        player->velocidade = -PLAYER_VELOCIDADE_PULO;
        player->podePular = 0;
    }

    bateuEmAlgo = 0;
    for (int i = 0; i < plataformas->quantPlataformas; i++)
    {
        plataforma = &(plataformas->plataforma[i]);

        if
        (
            plataforma->posicao.x <= player->posicao.x &&
            (plataforma->posicao.x + PLATAFORMA_COMPRIMENTO) >= player->posicao.x &&
            plataforma->posicao.y >= player->posicao.y &&
            plataforma->posicao.y <= (player->posicao.y + (player->velocidade * delta)))
        {
            bateuEmAlgo = 1;
            player->velocidade = 0.0f;
            player->posicao.y = plataforma->posicao.y;
            break;
        }
    }

    if (!bateuEmAlgo)
    {
        player->posicao.y += (player->velocidade * delta);
        player->velocidade += (GRAVIDADE * delta);
        player->podePular = 0;
    }
    else
    {
        player->podePular = 1;
    }
}
void AtualizarInimigos(Inimigos *inimigos, Plataformas *plataformas, float delta)
{
    Inimigo *inimigo;
    Plataforma *plataforma;

    int bateuEmAlgo;

    for(int i = 0; i < inimigos->quantInimigos; i++)
    {
        inimigo = &(inimigos->inimigo[i]);

        // Checando pra ver se chegou no canto da tela, e invertendo a direção se for o caso
        if(inimigo->direcao == 1)           // Indo para a direita
        {
            if(inimigo->posicao.x >= (TELA_LARGURA - INIMIGO_COMPRIMENTO))
            {
                inimigo->direcao = -1;
            }
        }
        else if(inimigo->direcao == -1)     // Indo para a esquerda
        {
            if(inimigo->posicao.x <= INIMIGO_COMPRIMENTO)
            {
                inimigo->direcao = 1;
            }
        }
        else                                // Parado
        {

        }

        // Aplicando a movimentação na direção do movimento
        if(inimigo->direcao == 1)           // Indo para a direita
        {
            inimigo.posicao.x += INIMIGO_VELOCIDADE_HORIZONTAL * delta;
        }
        else if(inimigo->direcao == -1)     // Indo para a esquerda
        {
            inimigo.posicao.x -= INIMIGO_VELOCIDADE_HORIZONTAL * delta;
        }
        else                                // Parado
        {

        }

        // Checando colisões com plataformas
        bateuEmAlgo = 0;
        for(int j = 0; j < plataformas->quantPlataformas; j++)
        {
            plataforma = &(plataformas->plataforma[i]);

            if(
                plataforma->posicao.x <= inimigo->posicao.x &&
                (plataforma->posicao.x + PLATAFORMA_COMPRIMENTO) >= inimigo->posicao.x &&
                plataforma->posicao.y >= inimigo->posicao.y &&
                plataforma->posicao.y <= (inimigo->posicao.y + (inimigo->velocidade * delta))
            )
            {
                bateuEmAlgo = 1;
                inimigo->velocidade = 0.0f;
                inimigo->posicao.y = plataforma->posicao.y;
                break;
            }
        }

        if(!bateuEmAlgo)
        {
            inimigo->posicao.y += (inimigo->velocidade * delta);
            inimigo->velocidade += (GRAVIDADE * delta);
        }
    }
}

EstadoMain LoopMenu()
{
    // Maquina de estado do jogo, define o que o programa vai fazer após este loop encerrar
    EstadoMain estado;

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

    return estado;
}
EstadoMain LoopScore()
{
    EstadoMain estado;



    return estado;
}
EstadoMain LoopJogo()
{
    // Maquina de estado do jogo, define o que o programa vai fazer após este loop encerrar
    EstadoMain estado;

    // Maquina de estado interna, para carregamento, execução e encerramento do jogo em momentos distintos
    EstadoJogoInterno estadoInterno = CARREGAMENTO;

    // Inicialização das structs
    Player player = GetPlayerPadrao();
    Inimigos inimigos = GetInimigosPadrao();
    Plataformas plataformas = GetPlataformasPadrao();
    Escadas escadas = GetEscadasPadrao();
    Portal portal = GetPortalPadrao();

    // Info do mapa atual
    int mapaAtual = 0;
    char mapa[MAPA_X][MAPA_Y];

    int loopJogo = 1;
    int isPausado = 0;
    int opcaoPause = 0;

    float timerNivel = 0.0;

    while(loopJogo)
    {
        switch(estadoInterno)
        {
            case CARREGAMENTO:
                // Carregar o mapa atual para a matriz
                CarregarMapa(mapaAtual, mapa);
                // Interpretar a matriz para as structs
                MatrizParaStructs(mapa, &player, &inimigos, &plataformas, &escadas, &portal);
                timerNivel = 0.0;
                estadoInterno = JOGO;
                break;
            case JOGO:
                // Simular o jogo com as structs

                // Tempo atual da fase
                float deltaTime = GetFrameTime();

                // Detecção de input
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    isPausado = !isPausado;
                    opcaoPause = 0; // reseta seleção ao abrir o menu
                }

                // Atualizando a posição do player e dos inimigos se o jogo não estiver pausado
                if(!isPausado)
                {
                    timerNivel += deltaTime;
                    AtualizarPlayer(&player, &plataformas, deltaTime);
                    AtualizarInimigos(&inimigos, &plataformas, deltaTime);
                }
                else
                {
                    // ----- Atualização do menu de pausa -----
                    // Navegação por teclado
                    if (IsKeyPressed(KEY_DOWN))
                    {
                        opcaoPause = (opcaoPause + 1) % 3;
                    }

                    if (IsKeyPressed(KEY_UP))
                    {
                        opcaoPause = (opcaoPause == 0) ? 2 : opcaoPause - 1;
                    }

                    // Confirmação com Enter
                    if (IsKeyPressed(KEY_ENTER))
                    {
                        if (opcaoPause == 0)            // Continuar
                        {          
                            isPausado = 0;
                        }
                        else if (opcaoPause == 1)       // Menu Principal
                        {   
                            estado = MENU;
                            isPausado = 0;
                            // Opcional: chame uma função para limpar dados do jogo
                        }
                        else if (opcaoPause == 2)       // Sair
                        {   
                            estado = FIM;
                            estadoInterno = ENCERRAMENTO;
                            isPausado = 0;
                        }
                    }

                    // Navegação por mouse
                    Vector2 mouse = GetMousePosition();
                    Rectangle retContinuar  = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 - 50, 240, 40 };
                    Rectangle retMenu       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 10, 240, 40 };
                    Rectangle retSair       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 70, 240, 40 };

                    if (CheckCollisionPointRec(mouse, retContinuar))
                    {
                        opcaoPause = 0;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            isPausado = 0;
                        }
                    }
                    else if (CheckCollisionPointRec(mouse, retMenu))
                    {
                        opcaoPause = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            estado = MENU;
                            estadoInterno = ENCERRAMENTO;
                            isPausado = 0;
                        }
                    }
                    else if (CheckCollisionPointRec(mouse, retSair))
                    {
                        opcaoPause = 2;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            estado = FIM;
                            estadoInterno = ENCERRAMENTO;
                            isPausado = 0;
                        }
                    }
                }

                // Desenhando o Frame
                BeginDrawing();
                    // Colorindo o Fundo
                    ClearBackground(LIGHTGRAY);

                    

                    // Desenhando as escadas

                    // Desenhando o portal

                    // Desenhando as Plataformas

                    // Desenhando os inimigos

                    // Desenhando o player

                    // Desenhando o timer
                    int minutos = (int)(timerNivel / 60);
                    int segundos = (int)(timerNivel) % 60;
                    int centesimos = (int)((timerNivel - (int)timerNivel) * 100);
                    DrawText(TextFormat("Tempo: %02d:%02d:%02d", minutos, segundos, centesimos),
                            10, 10, 30, WHITE);

                    // Desenhando o menu de pausa e a sua lógica, se isPausado == 1
                    if (jogoPausado) {
                        // Fundo semi-transparente escurecendo a tela
                        DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, Fade(BLACK, 0.6f));

                        // Título
                        DrawText("PAUSA", TELA_LARGURA/2 - MeasureText("PAUSA", 50)/2, TELA_ALTURA/4, 50, WHITE);

                        // Opções
                        Color corContinuar = (opcaoPause == 0) ? RED : LIGHTGRAY;
                        Color corMenu      = (opcaoPause == 1) ? RED : LIGHTGRAY;
                        Color corSair      = (opcaoPause == 2) ? RED : LIGHTGRAY;

                        DrawText("Continuar",       retContinuar.x + 30, retContinuar.y + 8, 25, corContinuar);
                        DrawText("Menu Principal",  retMenu.x + 10,      retMenu.y + 8,      25, corMenu);
                        DrawText("Sair",            retSair.x + 80,      retSair.y + 8,      25, corSair);

                        // (Opcional) contorno dos retângulos para debug visual
                        DrawRectangleLinesEx(retContinuar, 1, DARKGRAY);
                        DrawRectangleLinesEx(retMenu, 1, DARKGRAY);
                        DrawRectangleLinesEx(retSair, 1, DARKGRAY);
                    }

                EndDrawing();
                break;
            case ENCERRAMENTO:
                loopJogo = 0;
                break;
        }
    }


    return estado;
}