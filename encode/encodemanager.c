#include <types.h>
#include <encode.h>
#include <string.h>

PT_Encodeopr gp_encodeoprhead=NULL;

int EncodeRgister(PT_Encodeopr p_encodeopr)
{
    PT_Encodeopr current;
    PT_Encodeopr prev;
    if(gp_encodeoprhead == NULL)
    {
        gp_encodeoprhead = p_encodeopr;
        return 0;
    }
    current = gp_encodeoprhead;
    while(current)
    {
       prev = current;
       current = current->next;
    }
    prev->next = p_encodeopr;
    return 0;
    
}

PT_Encodeopr GetEncodeOpr(char * name)
{
    PT_Encodeopr current;
    current = gp_encodeoprhead;
    while(current)
    {
        if(!strcmp(name ,current->name ))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int EncodeInit()
{
    Utf8Register(); 
    return 0;
}
