
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <time.h>

int ser_hand;
unsigned short ser_buffcnt;
unsigned short ser_maincnt;
unsigned char ser_buffer[(0xFFF+1)<<1];

//-----------------------------------------------------------------------------
unsigned char ser_open ( char *portname )
{
  struct termios options;
  int status;

  ser_hand=open(portname,O_RDWR|O_NOCTTY|O_NDELAY);
  if(ser_hand==-1)
  {
    fprintf(stderr,"open: error - %s\n",strerror(errno));
    return(1);
  }
  fcntl(ser_hand,F_SETFL,FNDELAY);
  bzero(&options,sizeof(options));
  options.c_cflag=B38400|CS8|CLOCAL|CREAD;
  options.c_iflag=IGNPAR;
  tcflush(ser_hand,TCIFLUSH);
  tcsetattr(ser_hand,TCSANOW,&options);
  ser_maincnt=ser_buffcnt=0;


  return(0);
}
//----------------------------------------------------------------------------
void ser_close ( void )
{
  close(ser_hand);
}
//-----------------------------------------------------------------------------
int ser_senddata ( unsigned char *s, unsigned int len )
{
    int ret;

    while(1)
    {

        ret=(int)(write(ser_hand,s,len));
        if(ret<0)
        {
//            printf("errno %d\n",errno);
            if(errno==11)
            {
            }
            else
            {
            printf("errno %d\n",errno);
            break;
            }
        }
        else
        {
            break;
        }
    }
//  tcdrain(ser_hand);
}
//-----------------------------------------------------------------------------
void ser_drain ( void )
{
  tcdrain(ser_hand);
}
//-----------------------------------------------------------------------------
void ser_sendstring ( char *s )
{
  write(ser_hand,s,strlen(s));
  tcdrain(ser_hand);
}
//-----------------------------------------------------------------------------
void ser_update ( void )
{
  int r;

  r=read(ser_hand,&ser_buffer[ser_maincnt],4000);
  if(r>0)
  {
    ser_maincnt+=r;
    if(ser_maincnt>0xFFF)
    {
      ser_maincnt&=0xFFF;
      memcpy(ser_buffer,&ser_buffer[0xFFF+1],ser_maincnt);
    }
  }
}
//-----------------------------------------------------------------------------
unsigned short ser_copystring ( unsigned char *d )
{
    unsigned short r;
    unsigned short buffcnt;
    unsigned short maincnt;

    ser_update();
    buffcnt=ser_buffcnt;
    maincnt=ser_maincnt;
    for(r=0;buffcnt!=maincnt;buffcnt=(buffcnt+1)&0xFFF,r++) *d++=ser_buffer[buffcnt];
    return(r);
}
//-----------------------------------------------------------------------------
unsigned short ser_dump ( unsigned short x )
{
    unsigned short r;

    for(r=0;r<x;r++)
    {
        if(ser_buffcnt==ser_maincnt) break;
        ser_buffcnt=(ser_buffcnt+1)&0xFFF;
    }
    return(r);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000
//-----------------------------------------------------------------------------

