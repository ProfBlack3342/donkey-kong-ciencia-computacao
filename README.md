#  Donkey Kong - INF (DKINF)
Uma versao simplificada do jogo conhecido como Donkey Kong, codificado e apresentado para o curso de
Algoritmos e Programação - 2026/1

# Membros do Grupo
- Ariel Nathan Scheffer de Souza
- Eduardo Pereira Moreira

# Professores e Monitores
- Prof. Dr. Eder John Scheid
- Profa. Dra. Luciana Bencke
- Augusto Sturm
- Melchior Neto

# Como compilar (VSCode)
*Nota* Precisa ter um compilador de C instalado e configurado no VSCode

1. Abrir a pasta do jogo no VSCode
2. Mandar compilar ( CTRL+SHIFT+B ou F5 )
3. Pronto!

# Descrição do Programa
O programa é dividido em 3 loops principais, que são controlados por uma máquina de estado presente no main:

## Loop do Menu
Um loop que exibe o menu principal. Possui 3 opções:
- Iniciar Jogo - Encerra o Loop do Menu e muda o estado do main para o Loop do Jogo
- Scores - Encerra o Loop do Menu e muda o estado do main para o Loop dos Scores
- Sair - Encerra o Loop do Menu e sinaliza ao programa que ele pode encerrar a janela e encerrar

## Loop dos Score
Um loop que carrega e exibe os scores salvos em um arquivo binário. Possui somente um botão, que encerra o Loop dos Scores e retorna ao Loop do Menu. Os scores estão organizados pela lógica:
1. Maior Fase Alcançada
2. Menor Tempo

## Loop do Jogo
De longe a parte mais complexa do programa, possui uma máquina de estado interna que o divide em 3 grandes áreas:

### CARREGAMENTO

### JOGANDO

### ENCERRAMENTO

# Licenças
Raylib-Quickstart por Jeffery Myers está marcada como CC0 1.0. Para ver uma cópia, visite https://creativecommons.org/publicdomain/zero/1.0/

