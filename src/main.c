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

        * Tecla *               - * Acao *
        ← / → ou (’A’ / ’D’)    -   Movimento horizontal
        ↑ ou (’W’)              -   Subir escada
        ↓ ou (’S’)              -   Descer escada
        TAB                     -   Pausar o jogo

    • O jogo deve possuir sistema de pausa funcional, interrompendo completamente a execucao da logica do jogo
      e permitindo posterior volta a partir do mesmo ponto e mesmas condicoes.
      O menu de pausa deve possuir ao menos as seguintes opcoes:

        * Opcao *               - * Significado *
        Continuar               -   Retoma o jogo do ponto em que foi pausado
        Voltar ao menu          -   Retorna ao menu principal
        Sair                    -   Fecha o jogo

    • O jogador nao deve atravessar plataformas e nem caminhar fora de plataformas.
    • Os inimigos devem se movimentar sobre as plataformas sem atravessar obstaculos.
    • Ao colidir com inimigos, isto é, quando o jogador se encontrar na mesma posicao que o inimigo,
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
#include <ctype.h>


//----------------------------------------------------------------------------------
// Constantes
//----------------------------------------------------------------------------------

// Mapa
#define MAPA_X 30
#define MAPA_Y 30
#define MAPA_MAX 4

// Score
#define SCORE_NOME_MAX 21
#define SCORE_ARRAY_MAX 10
#define SCORE_NOME_ARQUIVO "placar.bin"

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

// Enumerador que define os estados da máquina de estado principal
typedef enum enum_estado_jogo {
    MENU,
    SCORE,
    JOGO,
    FIM
} EstadoJogo;

// Enumerador que define os estados da máquina de estado interna do jogo
typedef enum enum_estado_jogo_interno {
    CARREGAMENTO,
    JOGANDO,
    ENCERRAMENTO
} EstadoJogoInterno;


//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------

// Estrutura que armazena os dados relativos a um score
typedef struct struct_score {
    // String com o nome do jogador, que pode ter até SCORE_NOME_MAX - 1 caracteres
    char nome[SCORE_NOME_MAX];

    // O tempo total que o jogador permaneceu vivo no jogo
    float tempoVivo;

    // A maior fase que foi completada pelo jogador
    int faseCompletada;
} Score;

// Estrutura que armazena os dados relativos a posição de um elemento do mapa
typedef struct struct_posicao {
    // Posição X do elemento no mapa
    float x;

    // Posição Y do elemento no mapa
    float y;
} Posicao;

// Estrutura que armazena os dados relativos ao player no mapa
typedef struct struct_player {
    // Score do player
    Score score;
    
    // Posicao do player no mapa
    Posicao posicao;
    
    // Se o player está vivo
    int isVivo;
    
    // Velocidade do player
    float velocidade;
    
    // Se o player pode pular
    int podePular;
    
    // Se o player concluiu a fase
    int concluiuFase;
    
    // Altura do player
    float altura;
    
    // Comprimento do player
    float comprimento;
    
    // Cor do player
    Color cor;
} Player;

// Estrutura que armazena os dados relativos a um inimigo no mapa
typedef struct struct_inimigo {
    // Posição do inimigo no mapa
    Posicao posicao;

    // Velocidade do inimigo
    float velocidade;

    // Direcao do inimigo
    // -1 = Esquerda
    // 0 = Parado
    // 1 = Direita
    int direcao;
    
    // Altura do inimigo
    float altura;
    
    // Comprimento do inimigo
    float comprimento;
    
    // Cor do inimigo
    Color cor;
} Inimigo;

// Estrutura que armazena os dados relativos a todos os inimigos no mapa
typedef struct struct_inimigos {
    // Array com todas as structs "Inimigo" existentes
    Inimigo inimigo[INIMIGO_MAX];

    // Quantidade de structs salvas no array "struct_plataformas.inimigo"
    int quantInimigos;
} Inimigos;

// Estrutura que armazena os dados relativos a uma plataforma do mapa
typedef struct struct_plataforma {
    // Posição da plataforma no mapa
    Posicao posicao;

    // Altura da plataforma
    float altura;
    
    // Comprimento da plataforma
    float comprimento;
    
    // Cor da plataforma
    Color cor;
} Plataforma;

// Estrutura que armazena os dados relativos a todas as plataformas do mapa
typedef struct struct_plataformas {
    // Array com todas as structs "Plataforma" existentes
    Plataforma plataforma[PLATAFORMA_MAX];

    // Quantidade de structs salvas no array "struct_plataformas.plataforma"
    int quantPlataformas;
} Plataformas;

// Estrutura que armazena os dados relativos a uma parte de cima de escada
// Declaração Antecipada por ser variável de EscadaBaixo
typedef struct struct_escada_cima EscadaCima;

// Estrutura que armazena os dados relativos a uma parte de baixo de escada
typedef struct struct_escada_baixo {
    // Posição da parte de baixo da escada no mapa
    Posicao posicao;

    // Estrutura "EscadaCima" que está ligada a esta
    EscadaCima *escadaCima;
    
    // Altura da parte de baixo da escada
    float altura;
    
    // Comprimento da parte de baixo da escada
    float comprimento;
    
    // Cor da parte de baixo da escada
    Color cor;
} EscadaBaixo;

// Estrutura que armazena os dados relativos a uma parte do meio de escada
typedef struct struct_escada_meio {
    // Posição da parte do meio da escada no mapa
    Posicao posicao;

    // Altura da parte do meio da escada
    float altura;

    // Comprimento da parte do meio da escada
    float comprimento;

    // Cor da parte do meio da escada
    Color cor;
} EscadaMeio;

// Estrutura que armazena os dados relativos a uma parte de cima de escada
// Definição da struct declarada anteriormente
struct struct_escada_cima {
    // Posição da parte de cima da escada no mapa
    Posicao posicao;

    // Estrutura "EscadaBaixo" que está ligada a esta
    EscadaBaixo *escadaBaixo;

    // Altura da parte de cima da escada
    float altura;

    // Comprimento da parte de cima da escada
    float comprimento;

    // Cor da parte de cima da escada
    Color cor;
};

// Estrutura que armazena os dados relativos a todas as escadas do mapa
typedef struct struct_escadas {
    // Array com todas as structs "EscadaBaixo" existentes
    EscadaBaixo escadaBaixo[ESCADA_BAIXO_MAX];

    // Quantidade de structs salvas no array "struct_escadas.escadaBaixo"
    int quantEscadasBaixo;

    // Array com todas as structs "EscadaMeio" existentes
    EscadaMeio escadaMeio[ESCADA_MEIO_MAX];

    // Quantidade de structs salvas no array "struct_escadas.escadaMeio"
    int quantEscadasMeio;

    // Array com todas as structs "EscadaCima" existentes
    EscadaCima escadaCima[ESCADA_CIMA_MAX];

    // Quantidade de structs salvas no array "struct_escadas.escadaCima"
    int quantEscadasCima;
} Escadas;

// Estrutura que armazena os dados relativos ao portal no mapa
typedef struct struct_portal {
    // Posição do portal no mapa
    Posicao posicao;

    // Altura do portal
    float altura;

    // Comprimento do portal
    float comprimento;

    // Cor do portal
    Color cor;
} Portal;


//----------------------------------------------------------------------------------
// Funções
//----------------------------------------------------------------------------------

// ==================================================
// Funções que inicializam as estruturas
// ==================================================

// Função para criação padronizada da estrutura Score
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Score GetScorePadrao();

// Função para criação padronizada da estrutura Posicao
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Posicao GetPosicaoPadrao();

// Função para criação padronizada da estrutura Player
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Player GetPlayerPadrao();

// Função para criação padronizada da estrutura Inimigo
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Inimigo GetInimigoPadrao();

// Função para criação padronizada da estrutura Inimigos
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Inimigos GetInimigosPadrao();

// Função para criação padronizada da estrutura Plataforma
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Plataforma GetPlataformaPadrao();

// Função para criação padronizada da estrutura Plataformas
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Plataformas GetPlataformasPadrao();

// Função para criação padronizada da estrutura EscadaBaixo
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
EscadaBaixo GetEscadaBaixoPadrao();

// Função para criação padronizada da estrutura EscadaMeio
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
EscadaMeio GetEscadaMeioPadrao();

// Função para criação padronizada da estrutura EscadaCima
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
EscadaCima GetEscadaCimaPadrao();

// Função para criação padronizada da estrutura Escadas
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Escadas GetEscadasPadrao();

// Função para criação padronizada da estrutura Portal
// Struct deste tipo devem ser criadas SEMPRE com o uso dessas funções, para evitar erros
Portal GetPortalPadrao();


// ==================================================
// Função que lida com comparação de Scores
// ==================================================

// Função para comparar 2 scores e devolver qual é maior pela lógica:
// 1º - Maior Fase Alcancada > 2º - Menor Tempo Total
int DevolverMaiorScore(Score score1, Score score2);


// ==================================================
// Funções que lidam com a ordenação do placar (array de score com tamanho SCORE_ARRAY_MAX)
// Utiliza QuickSort
// Feitas com a ajuda do DeepSeek, pois apesar de saber implementar o algoritmo, não sabia exatamente uma forma eficiente de fazê-lo
// ==================================================

// Função que troca dois elementos de posição no array
void TrocarScore(Score *a, Score *b);

// Partição do QuickSort (usa DevolverMaiorScore para decidir a ordem)
int ParticionarPlacar(Score placar[], int baixo, int alto);

// QuickSort recursivo (ordem decrescente)
void QuickSortPlacar(Score placar[], int baixo, int alto);

// Função que ordena, em ordem decrescente, um placar de SCORE_ARRAY_MAX scores
void OrdenarPlacar(Score placar[]);


// ==================================================
// Funções que lidam com o carregamento e salvamento do placar (array de score com tamanho SCORE_ARRAY_MAX)
// ==================================================

// Função para carregar scores de um arquivo binário para um array de tipo Score com tamanho SCORE_ARRAY_MAX
int CarregarPlacar(Score placar[]);

// Função que salva um array de tipo Score com tamanho SCORE_ARRAY_MAX em um arquivo binário
int SalvarPlacar(Score placar[]);


// ==================================================
// Funções que lidam com o carregamento do mapa
// ==================================================

// Função que carrega um mapa de um arquivo de texto
void CarregarMapa(int numeroFase, char mapa[][MAPA_Y]);

// Função que percorre um array de duas dimensões que armazena um mapa,
// atribuindo a posição de cada elemento para sua estrutura correspondente
void MatrizParaStructs(char mapa[][MAPA_Y], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal);


// ==================================================
// Funções que lidam com a atualização do Player e Inimigos
// Feitas com a ajuda do DeepSeek, pq meu deus eu odeio física, e a física me odeia
// ==================================================

// Função que recebe um ponto, percorre todas as plataformas e
// tenta identificar se o ponto colide com alguma delas
int PontoSobrePlataforma(float x, float y, Plataformas *plataformas);

// Atualiza o Player, simulando:
// - Movimentação horizontal com colisão
// - Verificação de chão para pulo
// - Gravidade e colisão
// - Interação com Escadas e Portais
// - Interação com Inimigos
void AtualizarPlayer(Player *player, Plataformas *plataformas, Inimigos *inimigos, Escadas *escadas, Portal *portal, float delta);

// Atualiza o Inimigo, simulando:
// - Detecção de borda da plataforma
// - Movimentação horizontal com colisão
// - Gravidade e colisão
void AtualizarInimigos(Inimigos *inimigos, Plataformas *plataformas, float delta);

// ==================================================
// Funções para os diferentes loops de tela
// ==================================================

// Função que desenha o menu principal e recebe o input do usuário
EstadoJogo LoopMenu();

// Função que desenha o placar de scores mais altos
EstadoJogo LoopScore();

// Função que simula o jogo em si, funciona com uma máquina de estado interna
// Desenvolvida com auxílio do DeepSeek (IA) e revisada/corrigida manualmente
EstadoJogo LoopJogo();


//----------------------------------------------------------------------------------
// Função Main
//----------------------------------------------------------------------------------

// Inicializa variáveis do Raylib e a máquina de estado principal do jogo
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
                // Jogo rodando, pode voltar ao menu ou fim
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
        360.0f,
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

int DevolverMaiorScore(Score score1, Score score2)
{
    if(score1.faseCompletada > score2.faseCompletada)
        return 1;
    else if(score1.faseCompletada < score2.faseCompletada)
        return 2;
    else
    {
        if(score1.tempoVivo < score2.tempoVivo)
            return 1;
        else if(score1.tempoVivo > score2.tempoVivo)
            return 2;
        else    // Caso milagroso aonde tanto a fase quanto o tempo dos dois são iguais
            return 0;
    }
}

void TrocarScore(Score *a, Score *b)
{
    Score scoreTemp = *a;
    *a = *b;
    *b = scoreTemp;
}
int ParticionarPlacar(Score placar[], int baixo, int alto)
{
    // Pivô: último elemento do segmento
    Score pivo = placar[alto];
    int i = baixo - 1;   // índice do último elemento "melhor que o pivo"

    for (int j = baixo; j < alto; j++) {
        // Se placar[j] é melhor que o pivo, deve ir para a esquerda
        // DevolverMaiorScore(A, B) retorna 1 se A > B (melhor)
        if (DevolverMaiorScore(placar[j], pivo) == 1) {
            i++;
            TrocarScore(&placar[i], &placar[j]);
        }
    }
    // Posiciona o pivo no lugar correto
    TrocarScore(&placar[i + 1], &placar[alto]);
    return i + 1;
}
void QuickSortPlacar(Score placar[], int baixo, int alto)
{
    if (baixo < alto) {
        int indicePivo = ParticionarPlacar(placar, baixo, alto);
        QuickSortPlacar(placar, baixo, indicePivo - 1);
        QuickSortPlacar(placar, indicePivo + 1, alto);
    }
}
void OrdenarPlacar(Score placar[])
{
    QuickSortPlacar(placar, 0, (SCORE_ARRAY_MAX - 1));
}

int CarregarPlacar(Score placar[])
{
    char caminhoPlacar[256];
    strcpy(caminhoPlacar, GetApplicationDirectory());  // Raylib fornece o diretório do executável
    strcat(caminhoPlacar, SCORE_NOME_ARQUIVO);

    FILE *arquivo = fopen(caminhoPlacar, "rb");
    // Verificando se abriu o arquivo
    if(!arquivo)
        return 0;

    // Ler dados do arquivo e salva-los no array, checando se teve sucesso
    if( (fread(placar, sizeof(Score), SCORE_ARRAY_MAX, arquivo)) != SCORE_ARRAY_MAX )
        return 0;

    fclose(arquivo);
    return 1;
}
int SalvarPlacar(Score placar[])
{
    char caminhoPlacar[256];
    strcpy(caminhoPlacar, GetApplicationDirectory());  // Raylib fornece o diretório do executável
    strcat(caminhoPlacar, SCORE_NOME_ARQUIVO);

    FILE *arquivo = fopen(caminhoPlacar, "wb");
    // Verificando se abriu o arquivo
    if(!arquivo)
        return 0;

    // Salvar valores do array no arquivo, checando se teve sucesso
    if( (fwrite(placar, sizeof(Score), SCORE_ARRAY_MAX, arquivo)) != SCORE_ARRAY_MAX )
        return 0;

    fclose(arquivo);
    return 1;
}

void CarregarMapa(int numeroFase, char mapa[][MAPA_Y])
{
    char nomeMapaInicio[] = "mapa";
    char nomeMapaFim[] = ".txt";
    char nomeMapa[10];                  // {'m', 'a', 'p', 'a', 'N', '.', 't', 'x', 't', '\0'}
    char caminhoMapa[256] = {0};

    snprintf(nomeMapa, sizeof(nomeMapa), "%s%d%s", nomeMapaInicio, numeroFase, nomeMapaFim);
    strcpy(caminhoMapa, GetApplicationDirectory());  // Raylib fornece o diretório do executável
    strcat(caminhoMapa, nomeMapa);

    // Buffer para uma linha do arquivo: MAPA_X caracteres + '\n' + '\0'
    char linha[MAPA_X + 2];
    int lin = 0;         // índice da linha atual no arquivo (0 a MAPA_Y-1)

    FILE *arquivo = fopen(caminhoMapa, "r");
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
void MatrizParaStructs(char mapa[][MAPA_Y], Player *player, Inimigos *inimigos, Plataformas *plataformas, Escadas *escadas, Portal *portal)
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
    Score placar[SCORE_ARRAY_MAX];

    // Carrega o placar do arquivo binário; se falhar, inicializa com valores padrão
    if (!CarregarPlacar(placar))
    {
        for (int i = 0; i < SCORE_ARRAY_MAX; i++)
        {
            placar[i] = GetScorePadrao();
        }
    }

    while (!WindowShouldClose())
    {
        // Volta ao menu com Enter ou ESC
        if (IsKeyPressed(KEY_V))
        {
            estado = MENU;
            break;
        }

        BeginDrawing();

            ClearBackground(DARKPURPLE);

            // Título
            DrawText("HIGH SCORES", TELA_LARGURA/2 - MeasureText("HIGH SCORES", 40)/2, 30, 40, WHITE);

            // Cabeçalho da tabela
            int yBase = 100;
            DrawText("Pos", 80, yBase, 25, YELLOW);
            DrawText("Nome", 160, yBase, 25, YELLOW);
            DrawText("Fase", 400, yBase, 25, YELLOW);
            DrawText("Tempo (s)", 480, yBase, 25, YELLOW);

            // Linhas do placar
            for (int i = 0; i < SCORE_ARRAY_MAX; i++)
            {
                int y = yBase + 35 + i * 30;
                Color cor = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : DARKGRAY;

                // Posição
                DrawText(TextFormat("%d", i + 1), 80, y, 20, cor);

                // Nome (truncado se necessário)
                char nomeExibicao[SCORE_NOME_MAX + 1];
                strncpy(nomeExibicao, placar[i].nome, SCORE_NOME_MAX);
                nomeExibicao[SCORE_NOME_MAX] = '\0';
                DrawText(nomeExibicao, 160, y, 20, cor);

                // Fase completada
                DrawText(TextFormat("%d", placar[i].faseCompletada + 1), 400, y, 20, cor);

                // Tempo total (com duas casas decimais)
                DrawText(TextFormat("%.2f", placar[i].tempoVivo), 480, y, 20, cor);
            }

            // Instrução de retorno
            DrawText("Pressione V para voltar ao menu",
                    TELA_LARGURA/2 - MeasureText("Pressione V para voltar ao menu", 20)/2,
                    TELA_ALTURA - 50, 20, LIGHTGRAY);

        EndDrawing();
    }

    return estado;
}
EstadoJogo LoopJogo()
{
    // Variável que será enviada como resposta no final desta função, define qual o próximo estado a ser executado no main
    EstadoJogo estado = MENU;
    // Variável que define o estado da máquina de estados interna desta função
    EstadoJogoInterno estadoInterno = CARREGAMENTO;

    // Estrutura que representa o jogador no mapa
    Player player;
    // Estrutura que representa os inimigos no mapa
    Inimigos inimigos;
    // Estrutura que representa as plataformas do mapa
    Plataformas plataformas;
    // Estrutura que representa as escadas do mapa
    Escadas escadas;
    // Estrutura que representa o portal no mapa
    Portal portal;
    // Array de estruturas, com todos os scores que já foram registrados
    Score placar[SCORE_ARRAY_MAX];

    // O mapa que está atualmente sendo mostrado na tela
    int mapaAtual = 0;
    // O mapa em si, carregado do arquivo correspondente
    char mapa[MAPA_X][MAPA_Y];

    // Variável que controla se o loop do jogo ainda deve ocorrer
    int loopJogo = 1;
    // Variável que controla se o jogo está no menu de pausa
    int isPausado = 0;
    // Variável que define a opção sendo escolhida no menu de pausa DURANTE A NAVEGAÇÃO
    int opcaoPause = 0;
    // Variável que define se o jogador pode salvar o score ou não, após encerrar o jogo
    int podeSalvarScore = 0;
    // O timer do mapa atual
    float timerNivel = 0.0f;
    // O timer somado de todos os mapas jogados
    float timerTotal = 0.0f;
    // Variável que define se o jogador ganhou ou não o jogo
    int playerGanhou = 0;


    while (loopJogo)
    {
        // Checando para ver se o jogador fechou a janela, e encerrar o jogo se for o caso
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
                // Inicializar structs com valores padrão
                player = GetPlayerPadrao();
                inimigos = GetInimigosPadrao();
                plataformas = GetPlataformasPadrao();
                escadas = GetEscadasPadrao();
                portal = GetPortalPadrao();

                // Tentar carregar o placar do arquivo binário
                // Em caso de falha, preenche o array com valores padrão de Score
                if(!(CarregarPlacar(placar)))
                {
                    for(int i = 0; i < SCORE_ARRAY_MAX; i++)
                    {
                        placar[i] = GetScorePadrao();
                    }
                }

                // Carregar o mapa atual para o array
                CarregarMapa(mapaAtual, mapa);

                // Carregar as posições de cada elemento do mapa do array para suas structs correspondentes
                MatrizParaStructs(mapa, &player, &inimigos, &plataformas, &escadas, &portal);

                // Resetando o timer e mudando o estado
                timerNivel = 0.0f;
                estadoInterno = JOGANDO;

                break;
            }
            case JOGANDO:
            {
                // Tempo decorrido (em segundos) desde o ultimo frame
                float deltaTime = GetFrameTime();

                // Controle de pausa
                if (IsKeyPressed(KEY_P))
                {
                    isPausado = !isPausado;
                    opcaoPause = 0;
                }

                // Definição dos retângulos do menu de pausa

                // Botão de continuar
                Rectangle retContinuar  = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 - 50, 240, 40 };
                // Botão de voltar ao menu
                Rectangle retMenu       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 10, 240, 40 };
                // Botão de encerrar o jogo
                Rectangle retSair       = { TELA_LARGURA/2 - 120, TELA_ALTURA/2 + 70, 240, 40 };

                if (!isPausado)
                {
                    timerNivel += deltaTime;

                    AtualizarPlayer(&player, &plataformas, &inimigos, &escadas, &portal, deltaTime);
                    if (player.concluiuFase)
                    {
                        // Salvar tempo e fase no score
                        timerTotal += timerNivel;   // acumula o tempo da fase
                        timerNivel = 0.0f;
                        player.score.faseCompletada = mapaAtual;
                        
                        mapaAtual++;   // avança para a próxima fase
                        
                        if(mapaAtual > MAPA_MAX)
                        {
                            // Player ganhou
                            player.score.tempoVivo = timerTotal;
                            playerGanhou = 1;
                            estadoInterno = ENCERRAMENTO;
                        }
                        else
                        {
                            // Reinicia a fase
                            estadoInterno = CARREGAMENTO;
                            player.velocidade = 0.0f;
                            player.podePular = 0;
                            player.isVivo = 1;          // Só pra garantir
                            player.concluiuFase = 0;
                        }
                    }
                    else if(!(player.isVivo))
                    {
                        // Player morreu
                        timerTotal += timerNivel;
                        player.score.tempoVivo = timerTotal;
                        player.score.faseCompletada = mapaAtual - 1;
                        timerNivel = 0.0f;
                        playerGanhou = 0;
                        estadoInterno = ENCERRAMENTO;
                    }

                    AtualizarInimigos(&inimigos, &plataformas, deltaTime);
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

                    // Posição atual do mouse na tela
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

                    // Menu de pausa sobreposto, caso o jogador tenha pausado
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
                // Variável que armazena a opção no menu de vitória/derrota sendo escolhida DURANTE A NAVEGAÇÃO
                int opcaoFim = 0;
                // Variável que armazena a opção CONFIRMADA pelo jogador no menu de vitória/derrota
                int escolhaFeita = 0;

                // Variável para armazenar o teste de se o jogador pode salvar o score
                int comparacao = DevolverMaiorScore(player.score, placar[SCORE_ARRAY_MAX - 1]);
                if (comparacao == 1 || comparacao == 0)   // player.score >= último placar
                {
                    podeSalvarScore = 1;
                }
                else
                {
                    podeSalvarScore = 0;
                }

                // Variáveis para controle da entrada do nome

                // Variável buffer para o nome digitado na tela de salvamento de score
                char nomeBuffer[SCORE_NOME_MAX] = "";
                // Variável que armazena o tamanho da string em nomeBuffer, sem o '\0'
                int  nomeTamanho = 0;
                // Variável que define se a tela está no "modo" de salvar nome
                bool modoSalvar = false;
                // Variável que define se um nome foi confirmado para salvamento na tela de salvamento de score
                bool nomeConfirmado = false;
                // Variável que define se o jogador cancelou a tentativa de salvamento na tela de salvamento de score
                bool nomeCancelado = false;

                // Se o score merece ser salvo, ativa o modo de entrada de nome
                if (podeSalvarScore)
                {
                    modoSalvar = true;
                    nomeTamanho = 0;
                    nomeBuffer[0] = '\0';
                }

                while (!WindowShouldClose() && !escolhaFeita)
                {
                    // ----- Modo de entrada do nome (sobrepõe tudo) -----
                    if (modoSalvar)
                    {
                        // Variável que processa caracteres pressionados
                        int tecla = GetCharPressed();
                        while (tecla > 0)
                        {
                            // Aceita apenas caracteres imprimíveis e limita ao tamanho máximo - 1
                            if (isprint(tecla) && nomeTamanho < SCORE_NOME_MAX - 1)
                            {
                                nomeBuffer[nomeTamanho++] = (char)tecla;
                                nomeBuffer[nomeTamanho] = '\0';
                            }
                            tecla = GetCharPressed();
                        }

                        // Backspace
                        if (IsKeyPressed(KEY_BACKSPACE) && nomeTamanho > 0)
                        {
                            nomeTamanho--;
                            nomeBuffer[nomeTamanho] = '\0';
                        }

                        // Enter – confirma o nome
                        if (IsKeyPressed(KEY_ENTER))
                        {
                            // Se o nome ficou vazio, usa "Mario"
                            if (nomeTamanho == 0)
                            {
                                strncpy(nomeBuffer, "Mario", SCORE_NOME_MAX - 1);
                                nomeBuffer[SCORE_NOME_MAX - 1] = '\0';
                            }
                            // Copia o nome para o score do jogador
                            strncpy(player.score.nome, nomeBuffer, SCORE_NOME_MAX - 1);
                            player.score.nome[SCORE_NOME_MAX - 1] = '\0';

                            // Salva o score no array placar (substitui o último)
                            placar[SCORE_ARRAY_MAX - 1] = player.score;

                            // Ordena o array em ordem decrescente (melhor primeiro)
                            OrdenarPlacar(placar);

                            // Salva o array em arquivo binário
                            SalvarPlacar(placar);

                            nomeConfirmado = true;
                            modoSalvar = false;     // sai do modo de entrada
                            podeSalvarScore = 0;    // impede reexibição
                        }

                        // Escape – cancela o salvamento
                        if (IsKeyPressed(KEY_ESCAPE))
                        {
                            nomeCancelado = true;
                            modoSalvar = false;
                            podeSalvarScore = 0;
                        }

                        // ----- Desenho da tela de entrada do nome -----
                        BeginDrawing();

                            ClearBackground(DARKGRAY);

                            DrawText("DIGITE SEU NOME", TELA_LARGURA/2 - MeasureText("DIGITE SEU NOME", 30)/2,
                                    TELA_ALTURA/4 - 60, 30, WHITE);
                            DrawText("(Enter para salvar, ESC para pular)", TELA_LARGURA/2 - MeasureText("(Enter para salvar, ESC para pular)", 20)/2,
                                    TELA_ALTURA/4 - 30, 20, LIGHTGRAY);

                            // Retângulo do campo de texto
                            Rectangle campo = { TELA_LARGURA/2 - 150, TELA_ALTURA/2 - 15, 300, 40 };
                            DrawRectangleRec(campo, LIGHTGRAY);
                            DrawText(nomeBuffer, campo.x + 10, campo.y + 10, 20, BLACK);

                            // Cursor piscante
                            if (((int)(GetTime() * 2) % 2) == 0)
                            {
                                float cursorX = campo.x + 10 + MeasureText(nomeBuffer, 20);
                                DrawLine(cursorX, campo.y + 8, cursorX, campo.y + 32, BLACK);
                            }

                        EndDrawing();

                        // Enquanto estiver no modo de entrada, não processa mais nada
                        continue;
                    }

                    // ----- Navegação normal da tela de fim (após sair do modo salvar) -----
                    // Navegação por teclado
                    if (IsKeyPressed(KEY_DOWN))
                        opcaoFim = (opcaoFim + 1) % 3;
                    if (IsKeyPressed(KEY_UP))
                        opcaoFim = (opcaoFim == 0) ? 2 : opcaoFim - 1;

                    // Navegação por mouse
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

                    // Confirmação por Enter
                    if (IsKeyPressed(KEY_ENTER))
                        escolhaFeita = 1;

                    // ----- Desenho da tela de fim (vitória/derrota) -----
                    BeginDrawing();

                        ClearBackground(DARKGRAY);

                        if (playerGanhou)
                        {
                            DrawText("VOCE VENCEU!", TELA_LARGURA/2 - MeasureText("VOCE VENCEU!", 50)/2,
                                    TELA_ALTURA/4 - 30, 50, GREEN);
                        }
                        else
                        {
                            DrawText("VOCE MORREU!", TELA_LARGURA/2 - MeasureText("VOCE MORREU!", 50)/2,
                                    TELA_ALTURA/4 - 30, 50, RED);
                        }
                        DrawText(TextFormat("Tempo total: %.2f segundos", player.score.tempoVivo),
                                    TELA_LARGURA/2 - 150, TELA_ALTURA/4 + 40, 20, LIGHTGRAY);
                            DrawText(TextFormat("Ultima fase concluida: %d", player.score.faseCompletada),
                                    TELA_LARGURA/2 - 100, TELA_ALTURA/4 + 70, 20, LIGHTGRAY);

                        // Mensagem adicional caso o score não tenha sido qualificado para salvar
                        if (!podeSalvarScore && !modoSalvar && !nomeConfirmado && !nomeCancelado)
                        {
                            DrawText("Seu score nao foi alto o suficiente para entrar no placar.",
                                    TELA_LARGURA/2 - 200, TELA_ALTURA/2 - 120, 20, GRAY);
                        }

                        // Opções do menu

                        Color corJogar = (opcaoFim == 0) ? YELLOW : LIGHTGRAY;
                        Color corMenu  = (opcaoFim == 1) ? YELLOW : LIGHTGRAY;
                        Color corSair  = (opcaoFim == 2) ? YELLOW : LIGHTGRAY;

                        DrawText("Jogar Novamente", retJogarNovamente.x + 30, retJogarNovamente.y + 10, 25, corJogar);
                        DrawText("Menu Principal",  retMenuFim.x + 30,        retMenuFim.y + 10,        25, corMenu);
                        DrawText("Sair",            retSairFim.x + 100,       retSairFim.y + 10,        25, corSair);

                        // Contornos
                        DrawRectangleLinesEx(retJogarNovamente, 1, LIGHTGRAY);
                        DrawRectangleLinesEx(retMenuFim, 1, LIGHTGRAY);
                        DrawRectangleLinesEx(retSairFim, 1, LIGHTGRAY);

                    EndDrawing();
                }

                // ----- Processa a escolha final -----
                if (WindowShouldClose())
                {
                    estado = FIM;
                    loopJogo = 0;
                }
                else
                {
                    switch (opcaoFim)
                    {
                        case 0: // Jogar Novamente, resetar tudo só pra garantir
                            player = GetPlayerPadrao();
                            inimigos = GetInimigosPadrao();
                            plataformas = GetPlataformasPadrao();
                            escadas = GetEscadasPadrao();
                            portal = GetPortalPadrao();
                            mapaAtual = 0;
                            timerNivel = 0.0f;
                            timerTotal = 0.0f;
                            playerGanhou = 0;
                            estadoInterno = CARREGAMENTO;
                            loopJogo = 1;
                            estado = JOGO;
                            break;

                        case 1: // Menu Principal
                            estado = MENU;
                            loopJogo = 0;
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