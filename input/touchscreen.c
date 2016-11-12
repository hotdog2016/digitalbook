#include <mydebug.h>
#include <disdev.h>
#include <input.h>
#include <stdio.h>
#include <tslib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>

static struct ts_sample samp;
static struct tsdev *ts;
static struct timeval pretime;
static char *tsdevice=NULL;
static T_InputOpr t_touchscreenopr; 

static int Isoutof500ms(struct timeval *pre , struct timeval *samptime)
{
    unsigned long pretime;
    unsigned long nowtime;
    pretime = pre->tv_sec * 1000 + pre->tv_usec /1000;
    nowtime = samptime->tv_sec * 1000 + samptime->tv_usec /1000;
    if(nowtime > 500+pretime)
    {
        return 1;
    }else{
        return 0;
    }

}

static int TouchScreenGetEvent()
{
    int ret;
    P_DisDevINFO pt_fbinfo; 
    pt_fbinfo = GetDevInfo("fb");

    ret = ts_read(ts, &samp, 1);
        if (ret < 0) {
            exit(1);
        }
    
       if(Isoutof500ms(&pretime,&samp.tv))
       {
            pretime = samp.tv;
            if((samp.x < (pt_fbinfo->xres/3)) && (samp.x > 0))
            {
                t_touchscreenopr.event_type = PREVPAGE;
                return PREVPAGE;
            }
            else if(samp.x > (pt_fbinfo->xres/3*2))
            {
                t_touchscreenopr.event_type = NEXTPAGE;
                return NEXTPAGE;
            }
       }
   return 0;
}

static int TouchScreenInit()
{

	if ((tsdevice = getenv("TSLIB_TSDEVICE")) == NULL) {
        DebugPrint("can't find device \n");
        return -1;
       }

    	ts = ts_open (tsdevice, 1);
    	if (ts_config(ts)) {
    		perror("ts_config");
    		exit(1);
    	}
        t_touchscreenopr.fd = ts_fd(ts);
		/* Hide it */
        
        
        return 0;
}

static T_InputOpr t_touchscreenopr ={
    .name = "touchscreen",
    .GetEvent = TouchScreenGetEvent,
    .Init     = TouchScreenInit,
};
int RegisterTouchScreen()
{
   return RegisterInputOpr(&t_touchscreenopr);
}

