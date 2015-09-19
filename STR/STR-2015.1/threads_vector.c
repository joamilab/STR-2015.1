#include <stdio.h>
#include <stdlib.h>
#include <process.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void criaA();
void criaB();
void criaC();

char alfabeto[30];
int syncA = 1;
int syncB = 0;
int syncC = 0;
int i = 0;
int j;

void start_thread()
{
    int thread_id;

#if     defined(__WIN32__)
    if ((thread_id = _beginthread(criaA, 4096, NULL)) == (unsigned long)-1)
#else
    if ((thread_id = _beginthread(criaA, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread 1.\n");
        return;
    }
   

    #if     defined(__WIN32__)
    if ((thread_id = _beginthread(criaB, 4096, NULL)) == (unsigned long)-1)
#else
    if ((thread_id = _beginthread(criaB, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread 2.\n");
        return;
    }
    
     #if     defined(__WIN32__)
    if ((thread_id = _beginthread(criaC, 4096, NULL)) == (unsigned long)-1)
#else
    if ((thread_id = _beginthread(criaC, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread 3.\n");
        return;
    }

}

void criaA(){
	 while(i<27){
	 	while(syncA == 0){};
	 	alfabeto[i] = 'A';
	    i++; 
	    syncA = 0;
	    syncB = 1;
	}
}


void criaB(){
	  while(i<28){
	 	while(syncB == 0){};
	 	alfabeto[i] = 'B';
	    i++; 
	    syncB = 0;
	    syncC = 1;
	}
}

void criaC(){
	 while(i<29){
	 	while(syncC == 0){};
	 	alfabeto[i] = 'C';
	    i++; 
	    syncC = 0;
	    syncA = 1;
	}
	
	printf("%s", alfabeto);
	printf("\n");
	  
}



int main(int argc, char *argv[]) {
	
	start_thread();
	
	
	system("pause");
	return 0;
	
	
}
