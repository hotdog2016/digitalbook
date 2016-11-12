#include <mydebug.h>
#include <font.h>
#include <draw.h>
#include <stdio.h>
#include <string.h>
#include <input.h>


int main(int argc , char ** argv)
{
	DebugInit();

	DebugPrint("debug init ok\n");

    LoadText();

    SetDisDev();

    InitInputOpr();

    SetFont();

    SetEncode();
    
    ShowText();

    ExitFT();

    return 0;
}
