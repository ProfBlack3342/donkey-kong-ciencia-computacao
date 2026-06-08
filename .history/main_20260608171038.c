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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Constantes
//----------------------------------------------------------------------------------
#define MAPA_X 30
#define MAPA_Y 30

#define MAX_NOME_PLACAR 20

#define MAX_INIMIGOS 8
#define MAX_ITENS 5

#define MAX_PLATAFORMAS MAPA_X * MAPA_Y
#define MAX_ESCADAS_BAIXO MAPA_X * MAPA_Y
#define MAX_ESCADAS_MEIO MAPA_Y
#define MAX_ESCADAS_CIMA MAPA_X * MAPA_Y

#define TELA_ALTURA 900
#define TELA_LARGURA 900

#define COR_PLAYER RED
#define COR_INIMIGO BLUE
#define COR_PLATAFORMA GRAY
#define COR_ESCADA_BAIXO YELLOW
#define COR_ESCADA_MEIO ORANGE
#define COR_ESCADA_CIMA GREEN
#define COR_PORTAL PURPLE



#define GRAVIDADE 400
#define PLAYER_JUMP_SPD 225.0f
#define PLAYER_HOR_SPD 150.0f
#define ENEMY_HOR_SPD 100.0f

#define FPS 60

//----------------------------------------------------------------------------------
// Enums, Structs e Typedefs
//----------------------------------------------------------------------------------

// Enum para os estados possíveis do jogo
typedef enum enum_estado
{
    MENU,
    JOGO,
    END
} ESTADO;

// Struct obrigatória para o placar
typedef struct struct_placar
{
    char nome[MAX_NOME_PLACAR];
    int tempo;
    int score;
} PLACAR;

// Struct para posição de um ojeto ou plataforma na matriz do mapa
typedef struct struct_posicao
{
    int linha;
    int coluna;
} POSICAO;

// Struct para o jogador, definindo posição, velocidade e se está em pulo
typedef struct struct_player
{
    POSICAO posicao;
    float velocidade;
    int podePular;
    int vivo;
    Color cor;
} PLAYER;

// Struct para os inimigos, definindo posição, velocidade e direção de movimento
typedef struct struct_inimigo {
    POSICAO posicao;
    float velocidade;
    int direcaoMovimento; // 1 para direita, -1 para esquerda, 0 para parado
    Color cor;
} INIMIGO;

// Struct para item
typedef struct struct_item {
    POSICAO posicao;
    int altura;
    int comprimento;
    Color cor;
} ITEM;

// Struct para plataforma do mapa
typedef struct struct_plataforma {
    POSICAO posicao;
    int altura;
    int comprimento;
    Color cor;
} PLATAFORMA;

// Structs para escada
typedef struct struct_escada_baixo {
    POSICAO posicao;
    Color cor;
    ESCADA_CIMA *escadaCima; // Ponteiro para a escada de cima correspondente
} ESCADA_BAIXO;

typedef struct struct_escada_meio {
    POSICAO posicao;
    Color cor;
} ESCADA_MEIO;

typedef struct struct_escada_cima {
    POSICAO posicao;
    Color cor;
    ESCADA_BAIXO *escadaBaixo; // Ponteiro para a escada de baixo correspondente
} ESCADA_CIMA;

// Struct para o portal de saída
typedef struct struct_portal {
    POSICAO posicao;
    Color cor;
} PORTAL;

// Struct para o mapa do jogo
typedef struct struct_mapa {
    PLAYER player;
    INIMIGO inimigos[MAX_INIMIGOS];
    PLATAFORMA plataformas[MAX_PLATAFORMAS];
    ITEM itens[MAX_ITENS];
    ESCADA_BAIXO escadasBaixo[MAX_ESCADAS_BAIXO];
    ESCADA_MEIO escadasMeio[MAX_ESCADAS_MEIO];
    ESCADA_CIMA escadasCima[MAX_ESCADAS_CIMA];
    PORTAL portal;
} MAPA;

//----------------------------------------------------------------------------------
// Funções
//----------------------------------------------------------------------------------

ESTADO LoopMenu(void);

MAPA CarregarMapa(int numeroFase);
ESTADO LoopJogo(void);

//----------------------------------------------------------------------------------
// Função Main
//----------------------------------------------------------------------------------
int main(void)
{
    // Inicialização de variáveis
    ESTADO estadoJogo = MENU;

    while (estadoJogo != END)
    {
        switch (estadoJogo)
        {
        case MENU:
            // Loop do menu principal, onde o jogador pode escolher iniciar um novo jogo, ver o ranking ou sair
            // Tudo o que involver o menu principal fica aqui
            estadoJogo = LoopMenu();
            break;
        case JOGO:
            // Loop do jogo, onde a lógica principal do jogo é executada
            // Tudo o que envolver o jogo em si fica aqui
            estadoJogo = LoopJogo();
            break;
        default:
            break;
        }
    }

    // Encerramento do jogo

    return 0;
}

MAPA CarregarMapa(int numeroFase)
{
    // Variáveis para iteração em loops
    int i, j;

    // Matriz para armazenar o mapa lido do arquivo
    char matrizMapa[MAPA_X][MAPA_Y];

    // Variável para armazenar o mapa após processamento da matriz
    MAPA mapa;

    // Ponteiro para o arquivo do mapa a ser lido
    FILE *arquivoMapa;

    // Definindo nome do arquivo a ser carregado, seguindo o formato "mapaX.txt", onde X é o número da fase
    char nomeInicio = "mapa";
    char nomeFim = ".txt";
    char nomeArquivo[254];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s%d%s", nomeInicio, numeroFase, nomeFim);

    // Abrindo o arquivo do mapa para leitura
    arquivoMapa = fopen(nomeArquivo, "r");

    for(i = 0; i < MAPA_X; i++) {
        fgets(matrizMapa[i], sizeof(matrizMapa[i]), arquivoMapa);
    }

    // Fechando o arquivo após a leitura
    fclose(arquivoMapa);

    // Retorno do mapa carregado
    return mapa;
}

ESTADO LoopJogo(void)
{

}