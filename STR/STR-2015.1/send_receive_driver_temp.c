#include <stdlib.h>
#include <process.h>
#include <stdio.h>
#include <time.h>
 

void delay_s(int s);
int send_sync(int *buffer, int c, int t);
int receive_t(int *buffer, int c, int t);
int alt_wait(int c, int *chan);
void processA(void *thread);
void processB(void *thread);
void processC(void *thread);
void driver(void *thread);

int chan[3] = {-1, -1, -1};

void start_thread()
{
    int thread_id;
     
    #if     defined(__WIN32__)
        if ((thread_id = _beginthread(processA, 4096, NULL)) == (unsigned long)-1)
    #else
        if ((thread_id = _beginthread(processA, 4096, NULL)) == -1)
    #endif
        {
            printf("Unable to create thread 1.\n");
            return;
        }
         
    #if     defined(__WIN32__)
        if ((thread_id = _beginthread(processB, 4096, NULL)) == (unsigned long)-1)
    #else
        if ((thread_id = _beginthread(processB, 4096, NULL)) == -1)
    #endif
        {
            printf("Unable to create thread 2.\n");
            return;
        }
         
    #if     defined(__WIN32__)
        if ((thread_id = _beginthread(processC, 4096, NULL)) == (unsigned long)-1)
    #else
        if ((thread_id = _beginthread(processC, 4096, NULL)) == -1)
    #endif
        {
            printf("Unable to create thread 3.\n");
            return;
        }
        
        #if     defined(__WIN32__)
        if ((thread_id = _beginthread(driver, 4096, NULL)) == (unsigned long)-1)
    #else
        if ((thread_id = _beginthread(driver, 4096, NULL)) == -1)
    #endif
        {
            printf("Unable to create thread 4.\n");
            return;
        }
 
}

void processA(void *thread){
    int buffer = 10;
    int x =1 ;
    delay_s(5);
    do{
        x = send_sync_t(&buffer, 0, 5);
    }while(x==1);
    return;
}

void processB(void *thread){
    int buffer = 10;
    int x =1 ;
    delay_s(7);
    do{
        x = send_sync_t(&buffer, 0, 5);
    }while(x==1);
    return;
}

void processC(void *thread){
    int buffer = 10;
    int x =1 ;
    delay_s(9);
    do{
        x = send_sync_t(&buffer, 0, 5);
    }while(x==1);
    return;
}

void driver(void *thread){
    int vetor[3];
    int x, i;
    
    for(i=0; i<=2; i++){
        x = alt_wait(3, chan);
        receive_t(&vetor[i], x, 2);
    }
    
    for(i=0; i<=2; i++){
        printf("valor: %d\n", vetor[i]);
    }
    return;
}

void delay_s(int t){
    time_t start, end;
    
    start = time(NULL);
    do{
        end = time(NULL);
    }while(difftime(start, end) <= t);
    return;
}

int send_sync_t(int *buffer, int c, int t){
    time_t start, end;
    
    start = time(NULL);
    chan[c] = *buffer;
    
    do{
        end = time(NULL);
    }while(difftime(start, end) <= t && chan[c] != -1);
    
    if(chan[c] == -1) return 0;
    else return 1;
}

int receive_t(int *buffer, int c, int t){
    time_t start, end;
    
    start = time(NULL);
    
    do{
        end = time(NULL);
    } while(difftime(start, end) <= t && chan[c] != -1);
    
    if(chan[c] != -1){
        *buffer = chan[c];
        chan[c] = -1;
        return 0;
    }
    else return 1; 
} 

int alt_wait(int c, int *chan){
    int i;
    while(1){
        for(i = 0; i < c; i++){
            if (chan[c] != -1) return i;
        }
    }
}

int main(int argc, char *argv[])
{
    start_thread();
     
    system("pause");
    return 0;
}
