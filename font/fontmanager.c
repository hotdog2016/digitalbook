#include <types.h>
#include <mydebug.h>
#include <font.h>
#include <stdio.h>
#include <string.h>
PT_FontOprType p_fontoprtypehead = NULL;

int FontOprRegister(PT_FontOprType p_fontoprtype)
{
    PT_FontOprType current;
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
			p_fontoprtype->next = NULL;
            current->next = p_fontoprtype;
            return 0;
        }
        current=current->next;
    }
    DebugPrint("can't add!\n");
    return -1;
}

PT_FontOprType GetFontOprType(char *name)
{
    PT_FontOprType current;
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
