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
    int (*Exit)(void);
    int (*Init)(void);
    struct InputOpr * next;
}* PT_InputOpr , T_InputOpr;

int RegisterTouchScreen();
int RegisterStdin();
int RegisterInputOpr (PT_InputOpr p_inputopr);
PT_InputOpr GetInputOpr(char * name);
int InitInputOpr();
int GetEvent();
#endif
