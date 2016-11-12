#ifndef _ENCODE_H
#define _ENCODE_H
typedef struct Encodeopr {
    char *name;
    int (*GetEncode)(unsigned int * onefontencode,unsigned char *);
    struct Encodeopr * next;
}T_EncodeOpr , *P_Encodeopr;

int Utf8Register();
int EncodeRgister(P_Encodeopr p_encodeopr);
int EncodeInit();
P_Encodeopr GetEncodeOpr(char * name);
#endif
