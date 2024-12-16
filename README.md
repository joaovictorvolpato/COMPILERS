Autores: 
João Victor Volpato
João Paulo Bonomo
Rodrigo Santos de Carvalho
Rafael Vargas

Considereções:
O comando make compila o projeto e o arquivo executável alvo a ser rodado é o compiler
O projeto utiliza a biblioteca glib-2.0, então precisas ter ela localmente (é para o makefile cuidar disso)
Os 3 arquivos com 100 linhas são o input1.txt, input2.txt e input3.txt. Para trocar entre ele altere a linha 253 do arquivo compiler.c

Pontos:
O arquivo compiler.c tem a lógica da anásile léxica e comanda o pipeline da compilação
O arquivo ll1-parser.c faz a analise sintática
O arquivo symbol-table.c contém a lógica da tabela de simbolos
O arquivo type-checker.c faz a checkagem de tipos de expressões numéricas
O arquivo parse-numeric.c constroí a árvove de expressão
O arquivo loop-handler.c garante que não existe o comando break fora de um for
O arquivo handle-vardecl-types.c isere os tipos das variáveis na tabela de símbolos
O arquivo intermidiate-code-genertor gera o código intermediário

SDDs e SDT
As SDD's e SDT's desenvolvidas estão nos arquivos txt com os nomes
L-attribuited_SDD_NumExpre.txt
L-attribuited_SDD_Vardecl.txt
SDD-GCI.txt