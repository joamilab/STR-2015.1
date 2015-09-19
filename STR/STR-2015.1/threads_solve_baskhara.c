#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void entraValores();
void calculaRaizes();
void imprimeResultado();

int syncA = 1;
int syncB = 0;
int syncC = 0;

double coefA=0;
double coefB=0;
double coefC=0;
double delta;
double raiz1;
double raiz2;


void start_thread()
{
    int thread_id;

	#if     defined(__WIN32__)
    	if ((thread_id = _beginthread(entraValores, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(entraValores, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 1.\n");
        	return;
    	}
   

    #if     defined(__WIN32__)
    	if ((thread_id = _beginthread(calculaRaizes, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(calculaRaizes, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 2.\n");
        	return;
    	}
    
    #if     defined(__WIN32__)
    	if ((thread_id = _beginthread(imprimeResultado, 4096, NULL)) == (unsigned long)-1)
	#else
    	if ((thread_id = _beginthread(imprimeResultado, 4096, NULL)) == -1)
	#endif
    	{
        	printf("Unable to create thread 3.\n");
        	return;
    	}
}

void entraValores(){
	while(syncA == 0){}

	/*coefA = 1;
	coefB = -2;
	coefC = -8;*/
	scanf("%lf %lf %lf", &coefA, &coefB, &coefC);
		 	
	syncA = 0;
    syncB = 1;
}

void calculaRaizes(){
	while(syncB == 0){}
	
	delta = (coefB*coefB) - 4*coefA*coefC;
	raiz1 = ((-coefB) + sqrt(delta)) / 2*coefA;
	raiz2 = ((-coefB) - sqrt(delta)) / 2*coefA;
	
	syncB = 0;
	syncC = 1;
}

void imprimeResultado(){
	while(syncC == 0){}
	
	printf("raiz 1 = %lf\nraiz 2 = %lf\n", raiz1, raiz2);
	 
	syncC = 0;
}


int main(int argc, char *argv[]) {
	start_thread();
	
	system("pause");
	return 0;
}
