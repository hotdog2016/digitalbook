#ifndef _ENCODE_H
#define _ENCODE_H
typedef struct Encodeopr {
    char *name;
    int (*GetEncode)(unsigned int * onefontencode,char *pcCurrentAddr);
    struct Encodeopr * next;
}T_EncodeOpr , *PT_Encodeopr;

int Utf8Register();
int EncodeRgister(PT_Encodeopr p_encodeopr);
int EncodeInit();
PT_Encodeopr GetEncodeOpr(char * name);
#endif
