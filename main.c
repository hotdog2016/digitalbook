#include <bitmapinfo.h>
#include <types.h>
#include <mydebug.h>
#include <encode.h>
#include <font.h>
#include <disdev.h>
#include <draw.h>
#include <stdio.h>
#include <string.h>
#include <input.h>
#include <network.h>

int main(int argc , char ** argv)
{

	char * TextName = "hanzi.txt";
//	NetworkInit();

	DebugInit();
    DisplayInit();
 //   FontInit();
  //  EncodeInit();

	GetBMPFileInfo();
//	ShowPicDetail();


   // InitInputOpr();

  // ShowText(TextName);
   // ExitFT();
    return 0;
}
