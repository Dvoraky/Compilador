# Compilador
Fases de um compilador para uma linguagem simplificada de pascal que estou desenvolvendo durante minha disciplina de compiladores.

Estou utilizando os arquivos exemplo1.sp e exemplo2.sp para testar o código.

Separei em bibliotecas então para a compilação do código é necessário que compile todos os arquivos .cpp juntos.

Compilação: g++ -O3 Compilador.cpp Lexer.cpp Sintático.cpp

E os arquivos de teste estão sendo passados como argumentos da execução então precisa executar deste jeito:
Execução: 
  - Linux: ./a.out exemplo1.sp
  - Windows: ./a.exe exemplo2.sp
