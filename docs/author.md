# Introduction

Sudoku is a classic number puzzle game.

# Author(s)

Pedro Lucas Medeiros do Nascimento [pedrolucas.jsrn@gmail.com]

# Problems found or limitations

Lista aqui os problemas encontrados (se houve algum) e as
possíveis limitações do programa.

Considerando as exigências não existem limitações encontradas ou previstas. Mesmo assim, se destaca algumas coisas que poderiam ser implementadas a mais:
- Aviso caso o jogador tente remover de uma posição que não possui números (fácil).
- Otimizar o desenho da tabuleiro para trabalhar com outros tamanhos (16x16) (fácil).
- Possibilidade de estabelecer uma quantidade máxima de erros que faria o jogador perder a partida (fácil).
- Geração automática de tabuleiros novos (fácil).
- Realizar um salvamento rápido de partidas que já foram salvas anteriormente (fácil).
- Contagem de tempo para a realização total do jogo (fácil).

# Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.


Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Trata corretamente os argumentos de linha de comando | 5 pts | 5
Lê e armazena os vários puzzles contidos em um arquivo de entrada |5 pts| 5
Exibe corretamente a tela principal com o menu de ações |5 pts| 5
Permite a escolha dos vários puzzles lidos do arquivo  |5 pts| 5
Executa corretamente a ação de posicionamento de dígitos no tabuleiro (comando posicionar) identificação jogadas inválidas |11 pts| 11
Executa corretamente a remoção de dígitos posicionados pelo jogador (comando remover) |10 pts| 10
Executa corretamente o comando de desfazer ação |12 pts| 12
Executa corretamente o comando de verificação do tabuleiro |12 pts| 12
Salva corretamente, em arquivo externo, uma partida em andamento | 7 pts | 7
Carrega corretamente, de um arquivo externo, uma partida em andamento previamente salva e consegue retormar a partida | 8 pts | ??
Exibe as regras do jogo quando solicitado |5 pts| 5
Indica corretamente se uma partida finalizada foi vencida ou perdida |5 pts| 5
Apresenta uma interface com usuário igual ou similar a que foi especificada nesse documento |5 pts| 5
Apresenta uma validação robusta dos comandos textuais de ação fornecidos pelo usuário |5 pts| 5
Exibe corretamente a lista de dígitos disponíveis, conforme o progresso do jogo |5 pts| 5
Apresenta as indicações com `setas` da coordenada da célula que foi alvo da última ação |5 pts| 5
Apresenta um fluxo correto entre telas, com opção de retomar uma partida suspensa e solicitação de confirmação de finalização de partida, caso exista uma partida em andamento |5 pts| 5

# Compiling and Runnig

To compile the project, follow these steps:

1. Create a build directory:
   ```shell
   cmake -S . -B build
   ```

2. Build the project:
   ```shell
   cmake --build build
   ```

3. Run the project:
   ```shell
   ./build/sudoku
   ```
