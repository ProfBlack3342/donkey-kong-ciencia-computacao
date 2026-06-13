#  Donkey Kong - INF (DKINF)
Uma versão simplificada do jogo conhecido como Donkey Kong, codificado e apresentado para o curso de
Algoritmos e Programação - 2026/1

### Membros do Grupo
- Ariel Nathan Scheffer de Souza
- Eduardo Pereira Moreira

### Professores e Monitores
- Prof. Dr. Eder John Scheid
- Profa. Dra. Luciana Bencke
- Augusto Sturm
- Melchior Neto

## Como compilar (VSCode)
<details>
<summary>

### Windows 
</summary>

#### Instalação do Compilador:
1. Baixar e extrair a ultima versão de https://github.com/skeeto/w64devkit/releases em algum local de fácil acesso
- (Uma dica é colocar em C:\Tools)
2. Apertar as teclas "Win + R", digitar "sysdm.cpl" e apertar "Enter"
3. Aba "Avançado", Opcão "Variáveis de Ambiente"
4. Na caixa Variáveis do sistema, apertar na variável "Path" e na opção "Editar..."
5. Selecionar a opção "Novo", e digitar o caminho para a pasta bin do arquivo que você baixou
- (Se você extraiu em "C:\Tools", o caminho será "C:\Tools\w64devkit\bin")
6. Apertar em "Ok" em todas as janelas até todas encerrarem
- Se quiser testar a instalação, abra um terminal e digite "gcc --version"

#### Configuração do Visual Studio Code:
1. Abrir VSCode
2. Instalar a extensão "C/C++ Extension Pack" da Microsoft
3. Aperte as teclas "Ctrl + Shift + P", ache a opção "C/C++: Edit Configurations (UI)" e a selecione
4. Na janela que for aberta, ache a seção "Compiler Path" e coloque o caminho para o compilador GCC do arquivo anterior
- (Se ele foi extraido em "C:\Tools", o caminho será "C:\Tools\w64devkit\bin\gcc.exe")
5. Ache a seção "IntelliSense mode" e altere para "windows-gcc-x64"

#### Configuração do Raylib-Quickstart
1. Certifique-se que a pasta do projeto está em um caminho com pastas que não tenham espaço no nome
2. Abra a pasta do projeto no VSCode
3. Navegue na pasta .vscode e abra o arquivo tasks.json
4. Modifique todos os campos com o formato "windows": { "command": "..." } para que tenham o caminho para o executável "mingw32-make.exe"
- (Se você instalou o "w64devkit" em "C:\Tools", o caminho será "C:\\Tools\\w64devkit\\bin\\mingw32-make.exe")
5. Salve e feche o arquivo
6. Reinicie o VSCode
7. Abra o arquivo main.c
8. Aperte F5 e espere o processo concluir
9. Feche o jogo e copie/mova os arquivos dos mapas e scores para a pasta bin\Debug DENTRO DO PROJETO
10. Aperte F5 novamente e aproveite o jogo!
</details>

<details>
<summary>

### Linux
</summary>

1. Abrir a pasta do jogo no VSCode
2. Abrir o arquivo main.c
3. Aperte F5 e espere o processo concluir
4. Feche o jogo e copie/mova os arquivos dos mapas e scores para a pasta bin\Debug DENTRO DO PROJETO
5. Aperte F5 novamente e aproveite o jogo!
</details>

# Descrição do Programa
O programa é dividido em 3 loops principais, que são controlados por uma máquina de estado presente no main:

## Loop do Menu
Um loop que exibe o menu principal. Possui 3 opções:
- Iniciar Jogo - Encerra o Loop do Menu e muda o estado do main para o Loop do Jogo
- Scores - Encerra o Loop do Menu e muda o estado do main para o Loop dos Scores
- Sair - Encerra o Loop do Menu e sinaliza ao programa que ele pode encerrar a janela e encerrar

## Loop dos Scores
Um loop que carrega e exibe os scores salvos em um arquivo binário. Possui somente um botão, que encerra o Loop dos Scores e retorna ao Loop do Menu. Os scores estão organizados pela lógica:
1. Maior Fase Alcançada
2. Menor Tempo

## Loop do Jogo
De longe a parte mais complexa do programa, possui uma máquina de estado interna que o divide em 3 grandes áreas:

### Carregamento do Mapa
Carrega o mapa a partir do arquivo de texto correspondente e atribui a posição para cada struct. Depois, muda o estado interno para a execução do jogo.

### Execução do Jogo
Leitura de inputs do Player, simulação de física e interações do Player e Inimigos, desenho do jogo e do menu de pause quando receber input para tal. Pode encerrar o loop do jogo pelo menu de pausa, ou carregar o proximo mapa

### Encerramento do Jogo
Exibição da tela de vitória ou derrota, verificação do score do jogador e salvamento do mesmo se for melhor que o último dos scores salvos. Pode encerrar o loop do jogo ou retornar ao carregamento do mapa

# 🤖 Assistência de IA
Partes da lógica de movimentação, física, colisões e estrutura do jogo foram desenvolvidas com o auxílio do **DeepSeek**, um assistente de IA, e posteriormente revisadas, corrigidas e adaptadas por um desenvolvedor humano para atender às necessidades específicas do projeto.

Isso inclui as funções:
- `AtualizarPlayer` – Física, pulo, colisões com plataformas/inimigos/escadas/portal
- `AtualizarInimigos` – Patrulha, inversão de direção, colisões
- `LoopJogo` – Desenho da tela e entrada do score
- `TrocarScore`, `ParticionarPlacar`, `QuickSortPlacar` e `OrdenarPlacar` - Algoritmo de ordenação para o placar

# Licenças
Raylib-Quickstart por Jeffery Myers está marcada como CC0 1.0. Para ver uma cópia, visite https://creativecommons.org/publicdomain/zero/1.0/