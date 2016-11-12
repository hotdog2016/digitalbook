#ifndef  _FONT_H
#define  _FONT_H

typedef struct  FontBitmap{
    int cur_pos_x;
    int cur_pos_y;
    int xMax;
    int yMax;
    int left;
    int pitch;
    int top;
    int next_pos_x;
    int next_pos_y;
    unsigned char *p_bitmap_buffer;
}T_FontBitmap,*P_FontBitmap;

typedef struct FONTopr{
    char * name;
    int (*FontTypeInit)(void);
    int (*GetFontBitmap)(unsigned int p_onefontencode , P_FontBitmap p_fontbitmap);
    struct FONTopr * next;
}T_FontOprType,*P_FontOprType;



int FontInit();
int FontOprRegister(P_FontOprType p_fontoprtype);
P_FontOprType GetFontOprType(char *name);
int FreetypeRegister();
int ExitFT();


#endif
