
//derived from test_usb.c and other files from https://github.com/texane/stlink
//dwelch@dwelch.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stlink-common.h"

FILE *fp;
unsigned int pdata[0x20000>>2];

int main ( int argc, char *argv[] )
{
    stlink_t* sl;
    unsigned int ra;
    unsigned int rb;
    unsigned int flen;
    int ret;

    if(argc<2)
    {
        printf(".bin file not specified\n");
        return(1);
    }

    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    memset(pdata,0xFF,sizeof(pdata));
    flen=fread(pdata,1,sizeof(pdata),fp);
    flen+=3;
    flen>>=2;
    fclose(fp);

    sl = stlink_open_usb(10);
    if(sl==NULL)
    {
        printf("stlink_open_usb failed\n");
        return(1);
    }

    printf("-- version\n");
    stlink_version(sl);

    printf("mode before doing anything: %d\n", stlink_current_mode(sl));

    if (stlink_current_mode(sl) == STLINK_DEV_DFU_MODE) {
        printf("-- exit_dfu_mode\n");
        stlink_exit_dfu_mode(sl);
    }

    printf("-- enter_swd_mode\n");
    stlink_enter_swd_mode(sl);

    printf("-- mode after entering swd mode: %d\n", stlink_current_mode(sl));

    printf("-- chip id: %#x\n", sl->chip_id);
    printf("-- core_id: %#x\n", sl->core_id);

    cortex_m3_cpuid_t cpuid;
    stlink_cpu_id(sl, &cpuid);
    printf("cpuid:impl_id = %0#x, variant = %#x\n", cpuid.implementer_id, cpuid.variant);
    printf("cpuid:part = %#x, rev = %#x\n", cpuid.part, cpuid.revision);

  //  printf("-- status\n");
    //stlink_status(sl);

    printf("-- reset\n");
    stlink_reset(sl);
    stlink_force_debug(sl);

//    printf("-- status\n");
  //  stlink_status(sl);

#ifdef LOAD_RAM

    printf("-- load\n");

    for(ra=0;ra<flen;ra++)
    {
        write_uint32(sl->q_buf,pdata[ra]);
        stlink_write_mem32(sl, 0x20000000+(ra<<2), 4);
    }

    for(ra=0;ra<8;ra++)
    {
        stlink_read_mem32(sl, 0x20000000+(ra<<2), 4);
        rb=read_uint32(sl->q_buf,0);
        printf("[0x%08X] 0x%08X 0x%08X\n",ra,rb,pdata[ra]);
    }

    printf("-- run\n");

    stlink_write_reg(sl, 0x20020000, 13); /* pc register */
    stlink_write_reg(sl, 0x20000000, 15); /* pc register */

    stlink_run(sl);

    ret =0;

#endif //LOAD_RAM

#ifdef LOAD_FLASH

    ra=0;
    rb=0;
    ret=stlink_write_flash(sl,0x08000000,(unsigned char *)pdata,0x4000);
    if(ret)
    {
        printf("stlink_write_flasin error\n");
    }

#endif //LOAD_FLASH















    printf("-- exit_debug_mode\n");
    stlink_exit_debug_mode(sl);

    stlink_close(sl);

    return 0;
}
