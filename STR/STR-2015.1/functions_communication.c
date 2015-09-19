#include <process.h>

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

void T1 (void *threadno)
{
    int buffer = 10;
    send_mes_sync(&buffer, 0);
    return;    
}

void T2 (void *threadno)
{
    int buffer;
    receive_mes(&buffer, 0);
    printf("%d", buffer);
    return;    
}
