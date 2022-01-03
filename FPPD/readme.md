# Fundamentos de Programação Paralela e Distribuída

# lista1.c: 

Faça um código simples que crie 5 threads e execute todas elas imprimindo seu próprio ID de forma a visualizar a desordem das impressões fazendo com que o código principal espere até que todas as 5 threads terminem.

# lista2.c: 

Faça um código simples que crie 10 threads e um vetor de CEM MIL posições, popule o mesmo vetor na thread principal, crie cada uma das 10 threads fazendo com que as mesmas concorram entre si para somar as posições do vetor sem repetições das posições do mesmo. Cada thread só poderá somar uma posição por vez, USE MUTEX.
Seu código deve, ALÉM de expor o resultado final correto, imprimir quantas posições de soma cada thread foi responsável. 

# trab1.c -

Existem 6 processos nesse problema: 3 processos infectados e 3 processos laboratoriais.

Cada um dos infectados vai fazer uma vacina e usá-la [o vírus é altamente mutável então a vacina tem que ser reaplicada a todo momento que for possível].
Para fazer uma vacina é necessário vírus morto, injeção e insumo secreto.

Cada infectado tem 1 dos 3 produtos.
Exemplo: 1 processo infectado tem um suprimento infinito de vírus morto, outro tem de injeção e o último tem de insumo secreto.

Cada laboratório tem um diferente suprimento infinito de 2 dos 3 produtos.
* Exemplo: laboratório 1 tem injeção, vírus morto, laboratório 2 tem injeção e insumo secreto e laboratório 3 tem insumo secreto e vírus morto.
Cada laboratório coloca seus 2 produtos próprios numa mesa em comum a todos infectados à disposição de quem quiser e todos os infectados correm para pegar os dois que eles precisam para fazer a vacina e somente renovam quando os SEUS PRÓPRIOS distribuídos forem consumidos. [o lab2 não renova se consumir a injeção do lab1 e o insumo secreto do lab3, por exemplo]

A entrada do código será um número que vai ser o número de vezes MÍNIMO que cada thread realizou seu objetivo primordial. [um infectado que atinge o mínimo NÃO para de concorrer com os outros, ele continua se vacinando]

A saída deverá ser o número de vezes que cada thread realizou seu objetivo primordial:
no caso, os infectados irão contabilizar o número de vezes que cada um se injetou com a vacina e os laboratórios o número de vezes que cada um renovou seus estoques:
Exemplo de de execução:

./covid-19 90  [irá executar até todas as threads terem executado seu objetivo PELO MENOS 90 vezes]
infectado 1: 120
infectado 2: 100
infectado 3: 90
laboratorio 1: 120
laboratorio 2: 100
laboratorio 3: 90


