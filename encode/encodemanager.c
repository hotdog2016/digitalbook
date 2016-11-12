#include <encode.h>
#include <string.h>

P_Encodeopr gp_encodeoprhead=NULL;

int EncodeRgister(P_Encodeopr p_encodeopr)
{
    P_Encodeopr current;
    P_Encodeopr prev;
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
P_Encodeopr GetEncodeOpr(char * name)
{
    P_Encodeopr current;
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
