

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fpin;
FILE *fpout;

#define MAX_DATA 0x20000
//yes , >>1, make this too big and limit check if data read overflows
//the limit
unsigned int data[MAX_DATA>>1];

int main ( int argc, char *argv[] )
{
    unsigned int flen;
    unsigned int ra;

    if(argc<2)
    {
        printf(".bin file not specified\n");
        return(1);
    }


    fpin=fopen(argv[1],"rb");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    sprintf((char *)data,"%s.c",argv[1]);
    fpout=fopen((char *)data,"wb");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",(char *)data);
        fclose(fpin);
        return(1);
    }
    flen=fread(data,1,sizeof(data),fpin);
    fclose(fpin);
    if(flen>MAX_DATA)
    {
        printf("File to big to be programmed this way on this part\n");
        fclose(fpout);
        fclose(fpin);
        return(1);
    }
    flen+=3;
    flen>>=2;

    fprintf(fpout,"\n");
    fprintf(fpout,"//software generated file\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"const unsigned int rom_size=0x%08X;\n",flen);
    fprintf(fpout,"const unsigned int rom_data[%u]=\n",flen);
    fprintf(fpout,"{\n");
    for(ra=0;ra<flen;ra++)
    {
        fprintf(fpout,"  0x%08X, //[0x%08X]\n",data[ra],0x08000000+(ra<<2));
    }
    //if(flen==0) fprintf(fpout,"0\n"); //keep the compiler happy.
    fprintf(fpout,"};\n");
    fprintf(fpout,"\n");
    fclose(fpout);

    return(0);
}

