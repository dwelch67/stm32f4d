
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

unsigned char rdata[5000];

int main ( int argc, char *argv[] )
{
    unsigned int ra,rb;
    unsigned int command;
    unsigned int nontime;
    unsigned int lasttime;
    unsigned int nowtime;
    unsigned int lastfe;


    if(argc<2)
    {
        printf("file not specified\n");
        return(1);
    }

    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file\n");
        return(1);
    }
    rb=fread(rdata,1,sizeof(rdata),fp);
    fclose(fp);
    if(rb==0) return(1);


    lasttime=rdata[1]; lasttime<<=8; lasttime|=rdata[2];
    lastfe=lasttime;
    for(ra=0;ra<rb;ra+=3)
    {
        if(rdata[ra]&0x80) printf("\n");



        if(rdata[ra]==0xFE)
        {
            nowtime=rdata[ra+1]; nowtime<<=8; nowtime|=rdata[ra+2];
            nontime+=(nowtime-lasttime)&0xFFFF;
            printf("[%u] ",(nowtime-lastfe)&0xFFFF);
            lasttime=nowtime;
            lastfe=nowtime;
        }
        if((rdata[ra]&0xF0)==0x90)
        {
            nowtime=rdata[ra+1]; nowtime<<=8; nowtime|=rdata[ra+2];
            nontime+=(nowtime-lasttime)&0xFFFF;
            printf("note [%u] ",nontime);
            nontime=0;
            lasttime=nowtime;
        }

        printf("0x%02X ",rdata[ra]);

    }



    //for(ra=0;ra<rb;ra++)
    //{
        //if(rdata[ra]==0xFE) break;
    //}
    //for(;ra<rb;ra++)
    //{
        //if(rdata[ra]&0x80) command=rdata[ra];
        //switch(rdata[ra]&0xF0)
        //{
            //default:
            //{
                //switch(command&0xF0)
                //{
                    //default:
                    //{
                        //printf("dump 0x%02X\n",rdata[ra]);
                        //break;
                    //}
                    //case 0xB0:
                    //{
                        //printf("0x%02X 0x%02X 0x%02X\n",command,rdata[ra],rdata[ra+1]);
                        //ra+=1;
                        //break;
                    //}
                //}
                //break;
            //}
            //case 0x80: //note off
            //{
                //printf("note off %2u 0x%02X 0x%02X\n",rdata[ra]&0xF,rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0x90: //note on
            //{
                //printf("note on  %2u 0x%02X 0x%02X\n",rdata[ra]&0xF,rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0xA0:
            //{
                //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0xB0:
            //{
                //switch(rdata[ra+1])
                //{
                    //default:
                    //{
                        //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                        //ra+=2;
                    //}
                    //case 0x04:
                    //{
                        //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                        //ra+=2;
                    //}
                    //break;
                //}
                //break;
            //}
            //case 0xC0:
            //{
                //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0xD0:
            //{
                //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0xE0:
            //{
                //printf("0x%02X 0x%02X 0x%02X\n",rdata[ra],rdata[ra+1],rdata[ra+2]);
                //ra+=2;
                //break;
            //}
            //case 0xF0:
            //{
                //switch(rdata[ra]&0xF)
                //{
                    //case 0x1:
                    //{
                        //printf("0x%02X 0x%02X\n",rdata[ra],rdata[ra+1]);
                        //ra+=1;
                        //break;
                    //}
                    //default:
                    //{
                        //printf("0x%02X\n",rdata[ra]);
                        //break;
                    //}
                //}
                //break;
            //}
        //}
    //}
    return(0);

}

//0x26 snare
//0x30 tom1
//0x2D tom2
//0x2B floor tom

