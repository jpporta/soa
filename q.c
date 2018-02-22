/*******************************************************************************
* Este programa esta baseado no segundo experimento do curso sobre tempo real
* do Laboratorio Embry-Riddle
*
* Seguem os comentarios originais:
*
* Experiment #2: Multi-Tasking, Measuring Drift
*
*    Programmer: Eric Sorton
*          Date: 1/27/97
*           For: MSE599, Special Topics Class
*
*       Purpose: When a basic sleep call is used to determine the precise time
*                when an action will occur the problem of drift occurs.
*                The measurement of time is imprecise.  Similarly, the time in
*                which the sleep call returns is imprecise.  Over time, this
*                will cause the ocurrence of time to drift.  Just as if a clock
*                loses 1 second every day, over one day, it is significant, but
*                over a year, it loses 365 seconds, which is over 6 minutes.
*                This is an example of drift.
*
*       Proposito: Quando uma chamada básica sleep e usada para determinar o
*                instante exato em que alguma acao vai ocorrer, ocorre o problema
*                do desvio. A medicao de tempo e imprecisa. Similarmente, o tempo
*                que demora o retorno da chamada sleep tambem e impreciso. Ao
*                longo do tempo, isto ocasionara um desvio de tempo. Algo como se
*                um relogio perdesse um segundo a cada dia. Ao longo de um dia,
*                essa diferenca e insignificante, mas, ao longo de um ano, sao
*                perdidos 365 segundos, o que e superior a 6 minutos. Este e um
*                exemplo de desvio.
*
*******************************************************************************/

/*
 * Includes Necessarios, verifique se as bibliotecas no diretorio sys/ estao
 * lah. Caso nao estejam, verifique onde estao e altere o include
 */

#include <sys/time.h>		/* for gettimeofday() */
#include <unistd.h>		/* for gettimeofday() and fork() */
#include <stdio.h>		/* for printf() */
#include <sys/types.h>		/* for wait() */
#include <sys/wait.h>		/* for wait() */
#include <stdlib.h> 		/* for exit() */
/*
 * Pergunta 1: o que o compilador gcc faz com o arquivo .h, cujo nome aparece após o include?
    R: ele adiciona os cabeçalhos do arquivo no programa através de um pré-processamento, gerando um arquivo intermediario com a extensão .i
 */

/*
 * Pergunta 2: apresentar (parcialmente) e explicar o que há em <stdio.h>
    R: "standard input-output header", ou seja, responsavel pela entrada e saída de dados,
    possui definições de entrada/saída como leitura de entradas do teclado e exibição de informações na tela. Além de
    definições de constantes, variaveis e tipos.
 */

/*
 * Pergunta 3: qual eh a funcao da diretiva include (linha que começa com #), com relação ao compilador?
    R: adiciona os cabeçalhos, indica ao compilador que os cabeçalhos do arquivo a seguir devem ser adicionados.
 */

/*
 * NO_OF_ITERATIONS e o numero de vezes que vai se repetir o loop existente
 * em cada futuro processo filho.
 */

#define NO_OF_ITERATIONS	1000

/*
 * NO_OF_CHILDREN eh o numero de filhos a serem criados, cada qual responsavel
 * pela medida do desvio.
 */

#define NO_OF_CHILDREN	10

/*
 * SLEEP_TIME corresponde a quantidade de tempo para ficar bloqueado.
 */

#define SLEEP_TIME 10000

/*
 * MICRO_PER_SECOND define o numero de microsegundos em um segundo
 */

#define MICRO_PER_SECOND	1000000

/*
 * NO_OF_TESTS numero de teste a ser feito em uma só execucao
 */
#define NO_OF_TESTS 5


/*
 * Programa Principal. Contem tanto o codigo para o processo pai como
 * o codigo dos futuros filhos
 */

int main( int argc, char *argv[] )
{
/*
 * Pergunta 4: o que são e para que servem argc e argv? Não esqueca de
 * considerar o * antes de argv.
    R: argc e argv são parametros para o programa, indicados na linha de comando na chamada do programa.
    Argc é um valor inteiro que indica a quantidade de argumentos que foram passados ao chamar o programa.
    Argv é um vetor de char que contém os argumentos, um para cada string passada na linha de comando.
 */
	/*
	 * start_time e stop_time conterao o valor de tempo antes e depois
	 * que as trocas de contexto comecem
         */
	struct timeval start_time;
	struct timeval stop_time;

      /*
       * Outras variaveis importantes
       */

			// variaveis para argumentos do terminal
			int sleepTime = SLEEP_TIME;
			int noInteractions = NO_OF_ITERATIONS;
			int noChildren = NO_OF_CHILDREN;

      float drift;
      int count;
      int child_no;
      pid_t rtn;

	//printf("ID Filho, Desvio Absoluto, Desvio Medio\n");

			for(int x = 1; x < argc; x++){
				if(argv[x][1] == 's'){ // Sleep Time
				}
				else if(argv[x][1] == 'i'){ // noInteractions
					noInteractions = atoi(&argv[x][2]);
				}
				else if(argv[x][1] == 'c'){ // NO_OF_CHILDREN
					noChildren = atoi(&argv[x][2]);
				}
			}

	/*
	 * Criacao dos processos filhos
	 */

	for(int i = 0; i < NO_OF_TESTS; i++){
		rtn = (pid_t)1;
		for( count = 0; count < noChildren; count++ ) {
			if( rtn != 0 ) {
				rtn = fork();
			} else {
				break;
			}
		}


		/*
		 * Verifica-se rtn para determinar se o processo eh pai ou filho
		 */

		if( rtn == 0 ) {

		/*
	         * Portanto, sou filho. Faco coisas de filho.
	         */

			child_no = count;

			/*
			 * Primeiro, obtenho o tempo inicial.
			 */
			gettimeofday( &start_time, NULL );


			/*
			 * Este loop ocasiona a minha dormencia, de acordo com
			 * SLEEP_TIME, tantas vezes quanto NO_OF_ITERATIONS
			 */
			for( count = 0; count < noInteractions; count++ ) {
				usleep(sleepTime);
			}

			/*
			 * Paraobter o tempo final
			 */
			gettimeofday( &stop_time, NULL );

			/*
			 * Calcula-se o desvio
			 */
			drift = (float)(stop_time.tv_sec  - start_time.tv_sec);
			drift += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;

			/*
			 * Exibe os resultados
			 */
			printf("%d,%.3f,%.10f\n",
				child_no, drift - noInteractions*sleepTime/MICRO_PER_SECOND,
				(drift - noInteractions*sleepTime/MICRO_PER_SECOND)/noInteractions);

			/*
			 * Pergunta 5: Qual a relação: entre SLEEP_TIME e o desvio, nenhuma, direta
			 * ou indiretamente proporcional?
	            R: indiretamente.
			 */
	        exit(0);
		}

			/*
			 * Sou pai, aguardo o termino dos filhos
			 */
				wait(NULL);
		}
	exit(0);
}
