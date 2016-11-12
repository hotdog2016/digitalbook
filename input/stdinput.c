#include <mydebug.h>
#include <input.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static T_InputOpr gt_stdinput;

static int ExitInput(void)
{
    struct termios t_new_tty_conf;

    gt_stdinput.fd = STDIN_FILENO;

    tcgetattr(0,&t_new_tty_conf);
    t_new_tty_conf.c_lflag |= (ICANON);
    t_new_tty_conf.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&t_new_tty_conf);
    return 0;
}

static int InitStdInput()
{
    struct termios t_new_tty_conf;

    gt_stdinput.fd = STDIN_FILENO;

    tcgetattr(0,&t_new_tty_conf);
    t_new_tty_conf.c_lflag &= ~(ICANON);
    t_new_tty_conf.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&t_new_tty_conf);
    return 0;
}

static int StdInputGetEvent()
{
    char c;
    c=getchar();

    switch(c)
    {
        case 'n': gt_stdinput.event_type = NEXTPAGE;
            return NEXTPAGE;
        case 'b': gt_stdinput.event_type = PREVPAGE;
            return PREVPAGE;
        case 'q': 
            ExitInput();
            return QUIT;
    }
    return 0;
   
}

static T_InputOpr gt_stdinput = {
    .name     = "stdinput",
    .GetEvent = StdInputGetEvent,
    .Exit     = ExitInput,
    .Init     = InitStdInput,
};

int RegisterStdin()
{
    return RegisterInputOpr(&gt_stdinput);
}
