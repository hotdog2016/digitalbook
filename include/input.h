#ifndef _INPUT_H
#define _INPUT_H

#define NEXTPAGE                      (int)1
#define PREVPAGE                      (int)2
#define QUIT                          (int)3

#include <pthread.h>

typedef struct InputOpr {
    char * name;
    int (*GetEvent)();
    int fd;
    pthread_t tid;
    int event_type;
    struct InputOpr * next;
    int (*Exit)(void);
    int (*Init)(void);
}* P_InputOpr , T_InputOpr;

int RegisterTouchScreen();
int RegisterStdin();
int RegisterInputOpr (P_InputOpr p_inputopr);
P_InputOpr GetInputOpr(char * name);
int InitInputOpr();
int GetEvent();
#endif
