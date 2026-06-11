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
#define MAPA_MAX 4

// Score
#define PLACAR_NOME_MAX 20
#define PLACAR_MAX 10

// Player
#define PLAYER_VELOCIDADE_HORIZONTAL 150.0f
#define PLAYER_VELOCIDADE_PULO 225.0f
#define PLAYER_ALTURA 20.0f
#define PLAYER_COMPRIMENTO 20.0f
#define PLAYER_COR RED

// Inimigos
#define INIMIGO_MAX 8
#define INIMIGO_VELOCIDADE_HORIZONTAL 100.0f
#define INIMIGO_ALTURA 20.0f
#define INIMIGO_COMPRIMENTO 20.0f
#define INIMIGO_COR BLUE

// Plataformas
#define PLATAFORMA_MAX (MAPA_X * MAPA_Y)
#define PLATAFORMA_ALTURA 30.0f
#define PLATAFORMA_COMPRIMENTO 30.0f
#define PLATAFORMA_COR GRAY

// Escadas
#define ESCADA_BAIXO_MAX (MAPA_X * 2)
#define ESCADA_BAIXO_COR YELLOW
#define ESCADA_BAIXO_ALTURA 30.0f
#define ESCADA_BAIXO_COMPRIMENTO 30.0f

#define ESCADA_MEIO_MAX (ESCADA_BAIXO_MAX * MAPA_Y)
#define ESCADA_MEIO_COR ORANGE
#define ESCADA_MEIO_ALTURA 30.0f
#define ESCADA_MEIO_COMPRIMENTO 30.0f

#define ESCADA_CIMA_MAX ESCADA_BAIXO_MAX
#define ESCADA_CIMA_COR GREEN
#define ESCADA_CIMA_ALTURA 30.0f
#define ESCADA_CIMA_COMPRIMENTO 30.0f

// Portal
#define PORTAL_ALTURA 40.0f
#define PORTAL_COMPRIMENTO 30.0f
#define PORTAL_COR PURPLE

// Janela do Jogo
#define MULTIPLICADOR_TELA 30.0f
#define TELA_LARGURA (MAPA_X * MULTIPLICADOR_TELA)
#define TELA_ALTURA (MAPA_Y * MULTIPLICADOR_TELA)
#define FPS 60

// Simulação de Gravidade para Pulo/Queda do Player e Inimigos
#define GRAVIDADE 400.0f

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
    JOGANDO,
    ENCERRAMENTO
} EstadoJogoInterno;

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------
typedef struct struct_score {
    char nome[PLACAR_NOME_MAX];
    float tempoVivo;
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
    int concluiuFase;
    float altura;
    float comprimento;
    Color cor;
} Player;

typedef struct struct_inimigo {
    Posicao posicao;
    float velocidade;
    int direcao;
    float altura;
    float comprimento;
    Color cor;
} Inimigo;

typedef struct struct_inimigos {
    Inimigo inimigo[INIMIGO_MAX];
    int quantInimigos;
} Inimigos;

typedef struct struct_plataforma {
    Posicao posicao;
    float altura;
    float comprimento;
    Color cor;
} Plataforma;

typedef struct struct_plataformas {
    Plataforma plataforma[PLATAFORMA_MAX];
    int quantPlataformas;
} Plataformas;

// Declaração Antecipada por ser variável de EscadaBaixo
typedef struct struct_escada_cima EscadaCima;

typedef struct struct_escada_baixo {
    Posicao posicao;
    EscadaCima *escadaCima;
    float altura;
    float comprimento;
    Color cor;
} EscadaBaixo;

typedef struct struct_escada_meio {
    Posicao posicao;
    float altura;
    float comprimento;
    Color cor;
} EscadaMeio;

// Definição da struct declarada anteriormente
struct struct_escada_cima {
    Posicao posicao;
    EscadaBaixo *escadaBaixo;
    float altura;
    float comprimento;
    Color cor;
};

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
    float altura;
    float comprimento;
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

// Funções para carregar o mapa
// Arquivo de Texto -> Matriz -> Structs
void CarregarMapa(int numeroFase, char mapa[MAPA_X][MAPA_Y]);
void MatrizParaStructs(char mapa[MAPA_X][MAPA_Y], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal);

// Funções para lidar com a movimentação e colisão do Player e Inimigos
// Feitas com a ajuda do DeepSeek, pq meu deus eu odeio física, e a física me odeia
int PontoSobrePlataforma(float x, float y, Plataformas *plataformas);
void AtualizarPlayer(Player *player, Plataformas *plataformas, Inimigos *inimigos, Escadas *escadas, Portal *portal, float delta);
void AtualizarInimigos(Inimigos *inimigos, Plataformas *plataformas, float delta);

// A fazer: Função para carregar scores de um arquivo binário para um vetor de PLACAR_MAX scores
// A fazer: Função para comparar scores por lógica própria e devolver o maior (1º: Maior fase, 2º Menor tempo)
// A fazer: Função que recebe um vetor de scores e ordena ele de melhor a pior, seguindo a lógica anterior
// A fazer: Função que salva um vetor de PLACAR_MAX scores em um arquivo binário

// Funções para os diferentes loops de tela: Menu, Score e Jogo
// Quando encerram, retornam o próximo estado a ser desenhado
EstadoJogo LoopMenu();
EstadoJogo LoopScore();
EstadoJogo LoopJogo();

//----------------------------------------------------------------------------------
// Função Main
//----------------------------------------------------------------------------------
int main(void)
{
    // Inicializando o estado do jogo
    EstadoJogo estado = MENU;

    // Flags para o uso de vsync e dpi alto em telas de alta resolução
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Criando a janela
    InitWindow(TELA_LARGURA, TELA_ALTURA, "Donkey Kong em C com Raylib");

    // Setando o FPS
    SetTargetFPS(FPS);

    // Loop da janela do Raylib, só termina ao encerrar o jogo
    while (!WindowShouldClose())
    {
        switch(estado)
        {
            case MENU:
                // Jogo no menu, pode ir para score, jogo ou fim
                estado = LoopMenu();
                break;
            case SCORE:
                // Jogo no score, pode voltar ao menu
                estado = LoopScore();
                break;
            case JOGO:
                // Jogo rodando, pode voltar ao menu ou ir para o fim
                estado = LoopJogo();
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
        0.0f,
        0,
        0,
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
        0.0f,
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
        GetPosicaoPadrao(),
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

void CarregarMapa(int numeroFase, char mapa[MAPA_X][MAPA_Y])
{
    char nomeMapaInicio[] = "mapa";
    char nomeMapaFim[] = ".txt";
    char nomeMapa[10];                  // {'m', 'a', 'p', 'a', 'N', '.', 't', 'x', 't', '\0'}
    char basePath[256] = {0};

    snprintf(nomeMapa, sizeof(nomeMapa), "%s%d%s", nomeMapaInicio, numeroFase, nomeMapaFim);
    strcpy(basePath, GetApplicationDirectory());  // Raylib fornece o diretório do executável
    strcat(basePath, nomeMapa);

    // Buffer para uma linha do arquivo: MAPA_X caracteres + '\n' + '\0'
    char linha[MAPA_X + 2];
    int lin = 0;         // índice da linha atual no arquivo (0 a MAPA_Y-1)

    FILE *arquivo = fopen(basePath, "r");
    if (!arquivo) {
        // Em caso de erro, preenche toda a matriz com espaços
        for (int col = 0; col < MAPA_X; col++)
            for (int l = 0; l < MAPA_Y; l++)
                mapa[col][l] = ' ';
        return;
    }

    // Lê no máximo MAPA_Y linhas do arquivo
    while (lin < MAPA_Y && fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remove o caractere de nova linha (\n ou \r\n)
        linha[strcspn(linha, "\r\n")] = '\0';

        // Copia cada caractere da linha para a coluna correspondente, na linha 'lin'
        int col;
        for (col = 0; col < MAPA_X && linha[col] != '\0'; col++) {
            mapa[col][lin] = linha[col];   // transposição aqui: [coluna][linha]
        }
        // Preenche o restante das colunas com espaço (linha muito curta)
        while (col < MAPA_X) {
            mapa[col][lin] = ' ';
            col++;
        }
        lin++;
    }

    // Se o arquivo terminou antes de MAPA_Y linhas, preenche o restante
    while (lin < MAPA_Y) {
        for (int col = 0; col < MAPA_X; col++) {
            mapa[col][lin] = ' ';
        }
        lin++;
    }

    fclose(arquivo);
}
void MatrizParaStructs(char mapa[MAPA_X][MAPA_Y], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal)
{
    EscadaBaixo *escadaBaixo;
    EscadaCima *escadaCima;

    for(int i = 0; i < MAPA_X; i++)
    {
        escadaBaixo = NULL;
        escadaCima = NULL;
        
        for(int j = 0; j < MAPA_Y; j++)
        {
            switch(mapa[i][j])
            {
                case 'P':   // Posição do Player
                    player->posicao.x = (i * MULTIPLICADOR_TELA);
                    player->posicao.y = (j * MULTIPLICADOR_TELA);
                    break;
                case 'E':   // Posição de um Inimigo
                    if(inimigos->quantInimigos < INIMIGO_MAX)
                    {
                        inimigos->inimigo[inimigos->quantInimigos].posicao.x = (i * MULTIPLICADOR_TELA);
                        inimigos->inimigo[inimigos->quantInimigos].posicao.y = (j * MULTIPLICADOR_TELA);
                        inimigos->quantInimigos++;
                    }
                    break;
                case 'Z':   // Posição de uma Plataforma
                    if(plataformas->quantPlataformas < PLATAFORMA_MAX)
                    {
                        plataformas->plataforma[plataformas->quantPlataformas].posicao.x = (i * MULTIPLICADOR_TELA);
                        plataformas->plataforma[plataformas->quantPlataformas].posicao.y = (j * MULTIPLICADOR_TELA);
                        plataformas->quantPlataformas++;
                    }
                    break;
                case 'S':   // Posição de uma parte de baixo de Escada
                    if(escadas->quantEscadasBaixo < ESCADA_BAIXO_MAX)
                    {
                        escadaBaixo = &(escadas->escadaBaixo[escadas->quantEscadasBaixo]);
                        escadaBaixo->posicao.x = (i * MULTIPLICADOR_TELA);
                        escadaBaixo->posicao.y = (j * MULTIPLICADOR_TELA);

                        escadaBaixo->escadaCima = escadaCima;
                        escadaCima->escadaBaixo = escadaBaixo;

                        escadas->quantEscadasBaixo++;
                    }
                    break;
                case 'D':   // Posição de uma parte de cima de Escada
                    if(escadas->quantEscadasCima < ESCADA_CIMA_MAX)
                    {
                        escadaCima = &(escadas->escadaCima[escadas->quantEscadasCima]);
                        escadaCima->posicao.x = (i * MULTIPLICADOR_TELA);
                        escadaCima->posicao.y = (j * MULTIPLICADOR_TELA);
                        escadas->quantEscadasCima++;
                    }
                    break;
                case 'F':   // Posição do Portal de saída da fase
                    portal->posicao.x = (i * MULTIPLICADOR_TELA);
                    portal->posicao.y = (j * MULTIPLICADOR_TELA);
                    break;
                case 'H':   // Posição de uma parte do meio de Escada
                    if(escadas->quantEscadasMeio < ESCADA_MEIO_MAX)
                    {
                        escadas->escadaMeio[escadas->quantEscadasMeio].posicao.x = (i * MULTIPLICADOR_TELA);
                        escadas->escadaMeio[escadas->quantEscadasMeio].posicao.y = (j * MULTIPLICADOR_TELA);
                        escadas->quantEscadasMeio++;
                    }
                    break;
                default:    // Espaço vazio

            }
        }
    }
}

int PontoSobrePlataforma(float x, float y, Plataformas *plataformas)
{
    for (int i = 0; i < plataformas->quantPlataformas; i++)
    {
        Plataforma *p = &plataformas->plataforma[i];
        Rectangle r = { p->posicao.x, p->posicao.y, p->comprimento, p->altura };
        if (CheckCollisionPointRec((Vector2){x, y}, r))
            return 1;
    }
    return 0;
}
void AtualizarPlayer(Player *player, Plataformas *plataformas, Inimigos *inimigos, Escadas *escadas, Portal *portal, float delta)
{
    // ========== MOVIMENTAÇÃO HORIZONTAL COM COLISÃO ==========
    float novoX = player->posicao.x;
    if (IsKeyDown(KEY_LEFT))  novoX -= PLAYER_VELOCIDADE_HORIZONTAL * delta;
    if (IsKeyDown(KEY_RIGHT)) novoX += PLAYER_VELOCIDADE_HORIZONTAL * delta;

    Rectangle playerRectX = {
        novoX, player->posicao.y,
        player->comprimento, player->altura
    };

    int podeMoverX = 1;
    for (int i = 0; i < plataformas->quantPlataformas; i++)
    {
        Plataforma *plat = &plataformas->plataforma[i];
        Rectangle platRect = {
            plat->posicao.x, plat->posicao.y,
            plat->comprimento, plat->altura
        };
        if (CheckCollisionRecs(playerRectX, platRect))
        {
            podeMoverX = 0;
            break;
        }
    }
    if (podeMoverX) player->posicao.x = novoX;

    // ========== VERIFICAÇÃO DE CHÃO PARA PULO ==========
    // Pontos sob os pés (esquerdo e direito), 1 pixel abaixo da base
    float peEsqX = player->posicao.x + 2.0f;                      // margem para não confundir bordas
    float peDirX = player->posicao.x + player->comprimento - 2.0f;
    float peY    = player->posicao.y + player->altura + 1.0f;     // logo abaixo do pé

    int estaNoChao = PontoSobrePlataforma(peEsqX, peY, plataformas) ||
                     PontoSobrePlataforma(peDirX, peY, plataformas);

    if (IsKeyDown(KEY_SPACE) && estaNoChao)
    {
        player->velocidade = -PLAYER_VELOCIDADE_PULO;
    }

    // ========== FÍSICA VERTICAL (GRAVIDADE + COLISÃO) ==========
    player->velocidade += GRAVIDADE * delta;
    float novoY = player->posicao.y + player->velocidade * delta;

    Rectangle playerRectAtual = {
        player->posicao.x, player->posicao.y,
        player->comprimento, player->altura
    };
    Rectangle playerRectNovo = {
        player->posicao.x, novoY,
        player->comprimento, player->altura
    };

    int bateu = 0;
    for (int i = 0; i < plataformas->quantPlataformas; i++)
    {
        Plataforma *plat = &plataformas->plataforma[i];
        Rectangle platRect = {
            plat->posicao.x, plat->posicao.y,
            plat->comprimento, plat->altura
        };

        if (CheckCollisionRecs(playerRectNovo, platRect))
        {
            // Caindo (velocidade > 0) e o jogador estava acima da plataforma
            if (player->velocidade > 0 && (playerRectAtual.y + player->altura <= plat->posicao.y + 1.0f))
            {
                player->posicao.y = plat->posicao.y - player->altura;  // pouso perfeito
                player->velocidade = 0.0f;
                bateu = 1;
            }
            // Subindo (velocidade < 0) e estava abaixo – bate a cabeça
            else if (player->velocidade < 0 && (playerRectAtual.y >= plat->posicao.y + plat->altura - 1.0f))
            {
                player->posicao.y = plat->posicao.y + plat->altura;
                player->velocidade = 0.0f;
            }
            break;
        }
    }

    if (!bateu)
        player->posicao.y = novoY;

    // ========== ATUALIZAÇÃO FINAL DO ESTADO DE CHÃO ==========
    peEsqX = player->posicao.x + 2.0f;
    peDirX = player->posicao.x + player->comprimento - 2.0f;
    peY    = player->posicao.y + player->altura + 1.0f;
    player->podePular = PontoSobrePlataforma(peEsqX, peY, plataformas) ||
                        PontoSobrePlataforma(peDirX, peY, plataformas);

    // Retângulo final do jogador (usado por escadas, portal e inimigos)
    Rectangle playerRectFinal = {
        player->posicao.x, player->posicao.y,
        player->comprimento, player->altura
    };

    // ========== INTERAÇÃO COM ESCADAS (TELETRANSPORTE) ==========
    // Escadas de baixo → subir (KEY_UP)
    for (int i = 0; i < escadas->quantEscadasBaixo; i++)
    {
        EscadaBaixo *eb = &escadas->escadaBaixo[i];
        Rectangle escadaRect = {
            eb->posicao.x, eb->posicao.y,
            eb->comprimento, eb->altura
        };
        if (CheckCollisionRecs(playerRectFinal, escadaRect) && IsKeyPressed(KEY_UP))
        {
            if (eb->escadaCima != NULL)
            {
                player->posicao.x = eb->escadaCima->posicao.x;
                player->posicao.y = eb->escadaCima->posicao.y - player->altura;
                player->velocidade = 0.0f;
            }
            break;
        }
    }

    // Escadas de cima → descer (KEY_DOWN)
    for (int i = 0; i < escadas->quantEscadasCima; i++)
    {
        EscadaCima *ec = &escadas->escadaCima[i];
        Rectangle escadaRect = {
            ec->posicao.x, ec->posicao.y,
            ec->comprimento, ec->altura
        };
        if (CheckCollisionRecs(playerRectFinal, escadaRect) && IsKeyPressed(KEY_DOWN))
        {
            if (ec->escadaBaixo != NULL)
            {
                player->posicao.x = ec->escadaBaixo->posicao.x;
                player->posicao.y = ec->escadaBaixo->posicao.y - player->altura;
                player->velocidade = 0.0f;
            }
            break;
        }
    }

    // ========== INTERAÇÃO COM PORTAL ==========
    Rectangle portalRect = {
        portal->posicao.x, portal->posicao.y,
        portal->comprimento, portal->altura
    };
    if (CheckCollisionRecs(playerRectFinal, portalRect))
    {
        player->concluiuFase = 1;
    }

    // ========== COLISÃO COM INIMIGOS ==========
    for (int i = 0; i < inimigos->quantInimigos; i++)
    {
        Inimigo *inim = &inimigos->inimigo[i];
        Rectangle inimRect = {
            inim->posicao.x, inim->posicao.y,
            inim->comprimento, inim->altura
        };
        if (CheckCollisionRecs(playerRectFinal, inimRect))
        {
            player->isVivo = 0;
            return;
        }
    }
}
void AtualizarInimigos(Inimigos *inimigos, Plataformas *plataformas, float delta)
{
    for (int i = 0; i < inimigos->quantInimigos; i++)
    {
        Inimigo *inimigo = &inimigos->inimigo[i];

        // ---------- 1. DETECÇÃO DE BORDA DA PLATAFORMA (FIM DO CHÃO) ----------
        float pontoVerificacaoX;
        float pontoVerificacaoY = inimigo->posicao.y + inimigo->altura + 1.0f; // 1 pixel abaixo dos pés

        if (inimigo->direcao == 1) // Indo para a direita
        {
            pontoVerificacaoX = inimigo->posicao.x + inimigo->comprimento + 1.0f;
            if (!PontoSobrePlataforma(pontoVerificacaoX, pontoVerificacaoY, plataformas))
                inimigo->direcao = -1;   // Inverte para a esquerda
        }
        else if (inimigo->direcao == -1) // Indo para a esquerda
        {
            pontoVerificacaoX = inimigo->posicao.x - 1.0f;
            if (!PontoSobrePlataforma(pontoVerificacaoX, pontoVerificacaoY, plataformas))
                inimigo->direcao = 1;    // Inverte para a direita
        }

        // ---------- 2. MOVIMENTAÇÃO HORIZONTAL COM COLISÃO (PAREDES) ----------
        float novoX = inimigo->posicao.x;
        if (inimigo->direcao == 1)
            novoX += INIMIGO_VELOCIDADE_HORIZONTAL * delta;
        else if (inimigo->direcao == -1)
            novoX -= INIMIGO_VELOCIDADE_HORIZONTAL * delta;

        Rectangle inimRectX = {
            novoX, inimigo->posicao.y,
            inimigo->comprimento, inimigo->altura
        };

        int podeMoverX = 1;
        for (int j = 0; j < plataformas->quantPlataformas; j++)
        {
            Plataforma *plat = &plataformas->plataforma[j];
            Rectangle platRect = {
                plat->posicao.x, plat->posicao.y,
                plat->comprimento, plat->altura
            };

            if (CheckCollisionRecs(inimRectX, platRect))
            {
                podeMoverX = 0;
                // Inverte a direção ao bater na parede
                inimigo->direcao = (inimigo->direcao == 1) ? -1 : 1;
                break;
            }
        }

        if (podeMoverX)
            inimigo->posicao.x = novoX;

        // ---------- 3. FÍSICA VERTICAL (GRAVIDADE + COLISÃO) ----------
        inimigo->velocidade += GRAVIDADE * delta;
        float novoY = inimigo->posicao.y + inimigo->velocidade * delta;

        Rectangle inimRectAtual = {
            inimigo->posicao.x, inimigo->posicao.y,
            inimigo->comprimento, inimigo->altura
        };
        Rectangle inimRectNovo = {
            inimigo->posicao.x, novoY,
            inimigo->comprimento, inimigo->altura
        };

        int bateu = 0;
        for (int j = 0; j < plataformas->quantPlataformas; j++)
        {
            Plataforma *plat = &plataformas->plataforma[j];
            Rectangle platRect = {
                plat->posicao.x, plat->posicao.y,
                plat->comprimento, plat->altura
            };

            if (CheckCollisionRecs(inimRectNovo, platRect))
            {
                // Caindo (pouso)
                if (inimigo->velocidade > 0 && (inimRectAtual.y + inimigo->altura <= plat->posicao.y + 1.0f))
                {
                    inimigo->posicao.y = plat->posicao.y - inimigo->altura;
                    inimigo->velocidade = 0.0f;
                    bateu = 1;
                }
                // Subindo (bate a cabeça)
                else if (inimigo->velocidade < 0 && (inimRectAtual.y >= plat->posicao.y + plat->altura - 1.0f))
                {
                    inimigo->posicao.y = plat->posicao.y + plat->altura;
                    inimigo->velocidade = 0.0f;
                }
                break;
            }
        }

        if (!bateu)
            inimigo->posicao.y = novoY;
    }
}

EstadoJogo LoopMenu()
{
    EstadoJogo estado = MENU;
    int opcaoSelecionada = 0;
    int loopMenu = 1;

    while (loopMenu)
    {
        // Permite fechar a janela mesmo no menu
        if (WindowShouldClose())
        {
            estado = FIM;
            loopMenu = 0;
            continue;
        }

        // Navegação por teclas
        if (IsKeyPressed(KEY_DOWN))
            opcaoSelecionada = (opcaoSelecionada + 1) % 3;
        if (IsKeyPressed(KEY_UP))
            opcaoSelecionada = (opcaoSelecionada == 0) ? 2 : opcaoSelecionada - 1;

        // Navegação pelo mouse
        Vector2 mouse = GetMousePosition();
        Rectangle retIniciar = { TELA_LARGURA/2 - 100, 200, 200, 40 };
        Rectangle retScores  = { TELA_LARGURA/2 - 100, 260, 200, 40 };
        Rectangle retSair    = { TELA_LARGURA/2 - 100, 320, 200, 40 };

        // Seleção pelo mouse
        if (CheckCollisionPointRec(mouse, retIniciar))
        {
            opcaoSelecionada = 0;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estado = JOGO;
                loopMenu = 0;
            }
        }
        else if (CheckCollisionPointRec(mouse, retScores))
        {
            opcaoSelecionada = 1;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estado = SCORE;
                loopMenu = 0;
            }
        }
        else if (CheckCollisionPointRec(mouse, retSair))
        {
            opcaoSelecionada = 2;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                estado = FIM;
                loopMenu = 0;
            }
        }

        // Seleção com Enter
        if (IsKeyPressed(KEY_ENTER))
        {
            if (opcaoSelecionada == 0)
            {
                estado = JOGO;
                loopMenu = 0;
            }
            else if (opcaoSelecionada == 1)
            {
                estado = SCORE;
                loopMenu = 0;
            }
            else if (opcaoSelecionada == 2)
            {
                estado = FIM;
                loopMenu = 0;
            }
        }

        // Desenho do menu
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("MENU PRINCIPAL", TELA_LARGURA/2 - MeasureText("MENU PRINCIPAL", 30)/2, 80, 30, DARKGRAY);

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
EstadoJogo LoopScore()
{
    EstadoJogo estado = MENU;

    // Ainda será feito/implementado
    // FILE *arquivo;
    // fclose(arquivo);

    return estado;
}
EstadoJogo LoopJogo()
{
    EstadoJogo estado = MENU; // valor inicial seguro
    EstadoJogoInterno estadoInterno = CARREGAMENTO;

    Player player;
    Inimigos inimigos;
    Plataformas plataformas;
    Escadas escadas;
    Portal portal;

    int mapaAtual = 0;
    char mapa[MAPA_X][MAPA_Y];

    int loopJogo = 1;
    int isPausado = 0;
    int opcaoPause = 0;

    float timerNivel = 0.0f;

    int playerGanhou = 0;

    while (loopJogo)
    {
        if (WindowShouldClose())
        {
            estado = FIM;
            loopJogo = 0;
            break;
        }

        switch (estadoInterno)
        {
            case CARREGAMENTO:
            {
                player = GetPlayerPadrao();
                inimigos = GetInimigosPadrao();
                plataformas = GetPlataformasPadrao();
                escadas = GetEscadasPadrao();
                portal = GetPortalPadrao();

                CarregarMapa(mapaAtual, mapa);
                MatrizParaStructs(mapa, &player, &inimigos, &plataformas, &escadas, &portal);

                timerNivel = 0.0f;
                estadoInterno = JOGANDO;

                break;
            }
            case JOGANDO:
            {
                float deltaTime = GetFrameTime();

                // Controle de pausa
                if (IsKeyPressed(KEY_P))
                {
                    isPausado = !isPausado;
                    opcaoPause = 0;
                }

                // Definição dos retângulos do menu de pausa
                Rectangle retContinuar  = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 - 50, 240, 40 };
                Rectangle retMenu       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 10, 240, 40 };
                Rectangle retSair       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 70, 240, 40 };

                if (!isPausado)
                {
                    timerNivel += deltaTime;

                    AtualizarPlayer(&player, &plataformas, &inimigos, &escadas, &portal, deltaTime);
                    if (player.concluiuFase)
                    {
                        // Salvar tempo e fase no score
                        player.score.tempoVivo += timerNivel;   // acumula o tempo da fase
                        player.score.faseCompletada = mapaAtual;
                        
                        mapaAtual++;   // avança para a próxima fase
                        
                        if(mapaAtual > MAPA_MAX)
                        {
                            // Player ganhou, mostrar tela de vitória, score e voltar ao menu
                            playerGanhou = 1;
                            estadoInterno = ENCERRAMENTO;
                        }
                        else
                        {
                            // Reinicia a fase (volta ao estado CARREGAMENTO)
                            estadoInterno = CARREGAMENTO;
                            player.velocidade = 0.0f;
                            player.podePular = 0;
                            player.isVivo = 1;          // Só pra garantir
                            player.concluiuFase = 0;
                        }
                    }
                    else if(!(player.isVivo))
                    {
                        // Player morreu, mostrar tela de derrota, score e voltar ao menu
                        playerGanhou = 0;
                        estadoInterno = ENCERRAMENTO;
                    }

                    AtualizarInimigos(&inimigos, &plataformas, deltaTime);

                    // Reset de teste (tecla R)
                    if (IsKeyPressed(KEY_R))
                    {
                        player.posicao.x = 450;
                        player.posicao.y = 280;
                    }
                }
                else
                {
                    // Lógica de navegação do menu de pausa (teclado e mouse)
                    if (IsKeyPressed(KEY_DOWN))
                        opcaoPause = (opcaoPause + 1) % 3;
                    if (IsKeyPressed(KEY_UP))
                        opcaoPause = (opcaoPause == 0) ? 2 : opcaoPause - 1;

                    if (IsKeyPressed(KEY_ENTER))
                    {
                        if (opcaoPause == 0)            // Continuar
                        {
                            isPausado = 0;
                        }
                        else if (opcaoPause == 1)       // Menu Principal
                        {
                            estado = MENU;
                            loopJogo = 0;
                            isPausado = 0;
                        }
                        else if (opcaoPause == 2)       // Sair
                        {
                            estado = FIM;
                            loopJogo = 0;
                            isPausado = 0;
                        }
                    }

                    Vector2 mouse = GetMousePosition();

                    if (CheckCollisionPointRec(mouse, retContinuar))
                    {
                        opcaoPause = 0;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            isPausado = 0;
                    }
                    else if (CheckCollisionPointRec(mouse, retMenu))
                    {
                        opcaoPause = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            estado = MENU;
                            loopJogo = 0;
                            isPausado = 0;
                        }
                    }
                    else if (CheckCollisionPointRec(mouse, retSair))
                    {
                        opcaoPause = 2;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            estado = FIM;
                            loopJogo = 0;
                            isPausado = 0;
                        }
                    }
                }

                // Desenho da cena
                BeginDrawing();
                ClearBackground(LIGHTGRAY);

                // Desenhando as escadas
                for (int i = 0; i < escadas.quantEscadasBaixo; i++)
                {
                    DrawRectangleRec(
                        (Rectangle){
                            escadas.escadaBaixo[i].posicao.x,
                            escadas.escadaBaixo[i].posicao.y,
                            escadas.escadaBaixo[i].comprimento,
                            escadas.escadaBaixo[i].altura
                        }, 
                        escadas.escadaBaixo[i].cor);
                }
                for (int i = 0; i < escadas.quantEscadasMeio; i++)
                {
                    DrawRectangleRec(
                        (Rectangle){
                            escadas.escadaMeio[i].posicao.x,
                            escadas.escadaMeio[i].posicao.y,
                            escadas.escadaMeio[i].comprimento,
                            escadas.escadaMeio[i].altura
                        }, 
                        escadas.escadaMeio[i].cor);
                }
                for (int i = 0; i < escadas.quantEscadasCima; i++)
                {
                    DrawRectangleRec(
                        (Rectangle){
                            escadas.escadaCima[i].posicao.x,
                            escadas.escadaCima[i].posicao.y,
                            escadas.escadaCima[i].comprimento,
                            escadas.escadaCima[i].altura
                        }, 
                        escadas.escadaCima[i].cor);
                }

                // Desenhando o portal
                DrawRectangleRec(
                    (Rectangle){
                        portal.posicao.x,
                        portal.posicao.y,
                        portal.comprimento,
                        portal.altura
                    }, 
                    portal.cor);

                // Desenhando as plataformas
                for (int i = 0; i < plataformas.quantPlataformas; i++)
                {
                    DrawRectangleRec(
                        (Rectangle){
                            plataformas.plataforma[i].posicao.x,
                            plataformas.plataforma[i].posicao.y,
                            plataformas.plataforma[i].comprimento,
                            plataformas.plataforma[i].altura
                        }, 
                        plataformas.plataforma[i].cor);
                }

                // Desenhando os inimigos
                for (int i = 0; i < inimigos.quantInimigos; i++)
                {
                    DrawRectangleRec(
                        (Rectangle){
                            inimigos.inimigo[i].posicao.x,
                            inimigos.inimigo[i].posicao.y,
                            inimigos.inimigo[i].comprimento,
                            inimigos.inimigo[i].altura
                        }, 
                        inimigos.inimigo[i].cor);
                }

                // Desenhando o player
                DrawRectangleRec(
                    (Rectangle){
                        player.posicao.x,
                        player.posicao.y,
                        player.comprimento,
                        player.altura
                    }, 
                    player.cor);

                // Desenho do timer + fase
                int minutos = (int)(timerNivel / 60);
                int segundos = (int)(timerNivel) % 60;
                int centesimos = (int)((timerNivel - (int)timerNivel) * 100);
                DrawText(TextFormat("Fase: %d\nTempo: %02d:%02d:%02d", mapaAtual, minutos, segundos, centesimos),
                        10, 10, 30, WHITE);

                // Menu de pausa sobreposto
                if (isPausado)
                {
                    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, Fade(BLACK, 0.6f));
                    DrawText("PAUSA", TELA_LARGURA/2 - MeasureText("PAUSA", 50)/2, TELA_ALTURA/4, 50, WHITE);

                    Color corContinuar = (opcaoPause == 0) ? RED : LIGHTGRAY;
                    Color corMenu      = (opcaoPause == 1) ? RED : LIGHTGRAY;
                    Color corSair      = (opcaoPause == 2) ? RED : LIGHTGRAY;

                    DrawText("Continuar",       retContinuar.x + 30, retContinuar.y + 8, 25, corContinuar);
                    DrawText("Menu Principal",  retMenu.x + 10,      retMenu.y + 8,      25, corMenu);
                    DrawText("Sair",            retSair.x + 80,      retSair.y + 8,      25, corSair);

                    DrawRectangleLinesEx(retContinuar, 1, DARKGRAY);
                    DrawRectangleLinesEx(retMenu, 1, DARKGRAY);
                    DrawRectangleLinesEx(retSair, 1, DARKGRAY);
                }

                EndDrawing();
                break;
            }
            case ENCERRAMENTO:
            {
                int opcaoFim = 0;
                int escolhaFeita = 0;

                while (!WindowShouldClose() && !escolhaFeita)
                {
                    // ----- Navegação por teclado -----
                    if (IsKeyPressed(KEY_DOWN))
                        opcaoFim = (opcaoFim + 1) % 3;
                    if (IsKeyPressed(KEY_UP))
                        opcaoFim = (opcaoFim == 0) ? 2 : opcaoFim - 1;

                    // ----- Navegação por mouse -----
                    Vector2 mouse = GetMousePosition();
                    Rectangle retJogarNovamente = { TELA_LARGURA/2 - 150, TELA_ALTURA/2 - 40, 300, 45 };
                    Rectangle retMenuFim        = { TELA_LARGURA/2 - 150, TELA_ALTURA/2 + 20, 300, 45 };
                    Rectangle retSairFim        = { TELA_LARGURA/2 - 150, TELA_ALTURA/2 + 80, 300, 45 };

                    if (CheckCollisionPointRec(mouse, retJogarNovamente))
                    {
                        opcaoFim = 0;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            escolhaFeita = 1;
                    }
                    else if (CheckCollisionPointRec(mouse, retMenuFim))
                    {
                        opcaoFim = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            escolhaFeita = 1;
                    }
                    else if (CheckCollisionPointRec(mouse, retSairFim))
                    {
                        opcaoFim = 2;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            escolhaFeita = 1;
                    }

                    // ----- Confirmação por Enter -----
                    if (IsKeyPressed(KEY_ENTER))
                        escolhaFeita = 1;

                    // ----- Desenho da tela de fim -----
                    BeginDrawing();
                    ClearBackground(DARKGRAY);

                    if (playerGanhou)
                    {
                        DrawText("VOCE VENCEU!", TELA_LARGURA/2 - MeasureText("VOCE VENCEU!", 50)/2,
                                TELA_ALTURA/4 - 30, 50, GREEN);
                        DrawText(TextFormat("Tempo total: %.2f segundos", player.score.tempoVivo),
                                TELA_LARGURA/2 - 150, TELA_ALTURA/4 + 40, 20, LIGHTGRAY);
                        DrawText(TextFormat("Fase final: %d", player.score.faseCompletada + 1),
                                TELA_LARGURA/2 - 100, TELA_ALTURA/4 + 70, 20, LIGHTGRAY);
                    }
                    else
                    {
                        DrawText("VOCE MORREU!", TELA_LARGURA/2 - MeasureText("VOCE MORREU!", 50)/2,
                                TELA_ALTURA/4 - 30, 50, RED);
                        DrawText(TextFormat("Tempo sobrevivido: %.2f segundos", timerNivel),
                                TELA_LARGURA/2 - 170, TELA_ALTURA/4 + 40, 20, LIGHTGRAY);
                    }

                    // Checar os scores, se for maior ou igual ao último seguindo os critérios,
                    // Perguntar se o usuário deseja salvar seu nome.
                    // Se o usuário digitar algo e apertar "Salvar", salvar o score no binário,
                    // lembrando de fazer a organização em ordem decrescente

                    // Opções
                    Color corJogar = (opcaoFim == 0) ? YELLOW : LIGHTGRAY;
                    Color corMenu  = (opcaoFim == 1) ? YELLOW : LIGHTGRAY;
                    Color corSair  = (opcaoFim == 2) ? YELLOW : LIGHTGRAY;

                    DrawText("Jogar Novamente", retJogarNovamente.x + 30, retJogarNovamente.y + 10, 25, corJogar);
                    DrawText("Menu Principal",  retMenuFim.x + 30,        retMenuFim.y + 10,        25, corMenu);
                    DrawText("Sair",            retSairFim.x + 100,       retSairFim.y + 10,        25, corSair);

                    // Contornos para debug visual (opcional)
                    DrawRectangleLinesEx(retJogarNovamente, 1, LIGHTGRAY);
                    DrawRectangleLinesEx(retMenuFim, 1, LIGHTGRAY);
                    DrawRectangleLinesEx(retSairFim, 1, LIGHTGRAY);

                    EndDrawing();
                }

                // ----- Processa a escolha -----
                if (WindowShouldClose())
                {
                    estado = FIM;
                    loopJogo = 0;
                }
                else
                {
                    switch (opcaoFim)
                    {
                        case 0: // Jogar Novamente
                            // Reinicializa tudo
                            player = GetPlayerPadrao();
                            inimigos = GetInimigosPadrao();
                            plataformas = GetPlataformasPadrao();
                            escadas = GetEscadasPadrao();
                            portal = GetPortalPadrao();
                            mapaAtual = 0;
                            timerNivel = 0.0f;
                            playerGanhou = 0;
                            estadoInterno = CARREGAMENTO;
                            loopJogo = 1;          // continua o loop principal do jogo
                            estado = JOGO;         // permanece no estado JOGO
                            break;

                        case 1: // Menu Principal
                            estado = MENU;
                            loopJogo = 0;          // encerra este loop, main voltará ao menu
                            break;

                        case 2: // Sair
                            estado = FIM;
                            loopJogo = 0;
                            break;
                    }
                }
                break;
            }
        }
    }

    return estado;
}