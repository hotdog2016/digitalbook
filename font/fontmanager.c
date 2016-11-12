#include <mydebug.h>
#include <font.h>
#include <stdio.h>
#include <string.h>
P_FontOprType p_fontoprtypehead = NULL;

int FontOprRegister(P_FontOprType p_fontoprtype)
{
    P_FontOprType current;
    current = p_fontoprtypehead;
    if(current == NULL) 
    {
        p_fontoprtypehead = p_fontoprtype; 
        return 0;
    }
    while(current)
    {
        if(NULL == current->next)
        {
            current = p_fontoprtype;
            current->next=NULL;
            return 0;
        }
        current=current->next;
    }
    DebugPrint("can't add!\n");
    return -1;
}

P_FontOprType GetFontOprType(char *name)
{
    P_FontOprType current;
    current=p_fontoprtypehead;
    while(current)    
    {
        if(!strcmp(name,current->name))
        {
           return current; 
        }
        current = current->next;
    }
    DebugPrint("can't get!\n");
    return NULL;
}
int FontInit()
{
    return (FreetypeRegister());
}
