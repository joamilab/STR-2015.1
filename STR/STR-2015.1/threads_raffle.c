#include <stdlib.h>
#include <process.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void send_mes_sync(int *buf, int can);
void receive_mes(int *buf, int can);
void thread_a();
void thread_b();
void thread_c();

int chan[6] = {-1, -1, -1, -1, -1, -1};
int sincB = 0;
int controle = 0;

void start_thread()
{
    int thread_id;
    
   	#if     defined(__WIN32__)
    	if ((thread_id = _beginthread(thread_a, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(thread_a, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 1.\n");
        	return;
    	}
    	
    #if     defined(__WIN32__)
    	if ((thread_id = _beginthread(thread_b, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(thread_b, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 2.\n");
        	return;
    	}
    	
    #if     defined(__WIN32__)
    	if ((thread_id = _beginthread(thread_c, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(thread_c, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 3.\n");
        	return;
    	}

}

void send_mes_sync(int *buf, int can)
{
    chan[can] = *buf;
    while(chan[can] != -1){}
    return;    
}

void receive_mes(int *buf, int can)
{
    while(chan[can] == -1){}
    *buf = chan[can];
    chan[can] = -1;
    return;    
}


void thread_a()
{
	int num_sorteio;
	int i, j;
	int acertador[5] = {0, 0, 0, 0, 0};
	int send_acertador;
	
//	//Recebe números da aposta
	for(i=0;i<5;i++){
		for(j=0;j<6;j++){
			scanf("%d", &aposta[i][j]);
		}
	}
					
	//Ativa thread_b para começar o sorteio
	sincB = 1;

	while(controle < 6){
		//Recebe o número sorteado
		receive_mes(&num_sorteio, 0);
		
		send_acertador = 0;
		
		printf("\nRodada numero %d.\n", controle+1);
		printf("Numero sorteado %d\n", num_sorteio);
	
		//Encontra acertadores do número
		for(i=0;i<5;i++){
			for(j=0;j<6;j++){
				if(aposta[i][j] == num_sorteio)
					acertador[i] = 1;
			}
		}
						
		//Prepara número de acertadores para ser enviado à thread_c
		for(i=0; i<5; i++){
			if(acertador[i] == 1)
				send_acertador += pow(2,i);
			acertador[i] = 0;
		}
				
		//Envia acertadores para a thread_c
		send_mes_sync(&send_acertador, 2);	
	}
	   
}

void thread_b()
{
	int sorteio;
	srand(time(NULL));
	
	while(controle < 6){
		while(sincB == 0){}	
		
		//Faz o sorteio e envia o número para a thread_c
		sorteio = rand() % 100;
    	send_mes_sync(&sorteio, 2);
	
		sincB = 0;
	}    
}

void thread_c()
{
   	int buffer;
    int acertador;
    int i = 0;
    int g_rodada[5] = {0, 0, 0, 0, 0};
    int ganhador;
    
    while(controle < 6){
	    //Recebe o número sorteado e envia para a thread_a
		receive_mes(&buffer, 2);
	    send_mes_sync(&buffer, 0);
	    
	    //Recebe acertadores
		receive_mes(&acertador, 2);
		printf("acertadores: %d\n", acertador);
	    
	    //Extrai número dos apostadores que acertaram e imprime na tela
		while(acertador > 0){
			g_rodada[i] = acertador%2;
			acertador = acertador/2;
			i++;
		}
		
		for(i=0; i<5; i++){
			if(g_rodada[i] == 1)
				printf("O apostador %d acertou a rodada.\n", i+1);
			g_rodada[i] = 0;
		}
				
		controle++;
		i = 0;
		sincB = 1;
	}
    	
}


int main(int argc, char *argv[])
{
   	start_thread();
	 
    system("pause");
    return 0;
}
