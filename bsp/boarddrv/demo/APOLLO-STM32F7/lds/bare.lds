ENTRY(Init_Cpu)

INCLUDE  ../src/lds/memory.lds

/*��ͬ��make�����ļ�·����ʾ��ʽ���ǲ�ͬ�ģ���������ļ���д��Ҫ��Ӧ�޸�*/
/*��ȷ��д�����ɲο�console�������������*/
SECTIONS
{
    Rom_Start :
    {
        restore = .;
    }>InnerFlash

    /*RevRam��Ram�����������ڴ洢preloadǰʹ�û򱣴����������,ֻ��IAP_pload.c*/
    /*��pg_IapVar�����Ӹö��ڷ��䣬�Ҹö���iboot.lds��debug/release.lds����һ��*/
    Iboot_RevRam (NOLOAD):
    {
        . = ALIGN(0x08);
        KEEP(*(.IbootAppInfo .IbootAppInfo.*))
    }>RAM1

    stack_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x08);
        msp_bottom = .;
        KEEP(* (.data.InitStack))
        msp_top = ALIGN(0x08);
    }>RAM1

    text_startup_rIbootRom_lIbootRom :
    {
       . = ALIGN(0x08);
        KEEP(*(.StartupExpTbl .StartupExpTbl.*))
        KEEP(*libos_App.a:initcpuc.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:sysinit.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:pre_loader.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:cpu_peri_gpio.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:IAP_pLoad.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:set-cache.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:cpu_peri_hal.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_hal.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_hal_tim.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_hal_rcc.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_hal_pwr_ex.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_hal_cortex.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_App.a:stm32f7xx_ll_fmc.o (.text .text.* .rodata .rodata.*))
        KEEP(*src/app/OS_prjcfg/Iboot_config.o (.text .text.* .rodata .rodata.*))
    }>InnerFlash

    text_preload_rIbootRom_lIbootRom :
    {
       . = ALIGN(0x08);
        g_pAppCodeStartRamAddr = .;
        *libos_App.a:loader.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:int_hard.o (.text .text.* .rodata .rodata.*)
         KEEP(*libos_App.a:hard-exps.o (.text .text.* .rodata .rodata.*))
        *libos_App.a:hard-exp.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:int.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:ints.o (.text .text.* .rodata .rodata.*)
        *src/app/critical/*.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:heap-static.o (.text .text.* .rodata .rodata.*)
        *libos_App.a:list.o (.text .text.* .rodata .rodata.*)
    }>InnerFlash

    text_sysload_rIbootRom_lIbootRom :
    {
       . = ALIGN(0x08);
        * (.text .text.* .rodata .rodata.*)
        g_pAppCodeEndRamAddr = .;
    }>InnerFlash /* }>RAM1 AT>InnerFlash */

    rw_preload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x08);
        *libos_App.a:loader.o (.data .data.*)
        *libos_App.a:int_hard.o (.data .data.*)
        KEEP(*libos_App.a:hard-exps.o (.data .data.*))
        *libos_App.a:hard-exp.o (.data .data.*)
        *libos_App.a:int.o (.data .data.*)
        *libos_App.a:ints.o (.data .data.*)
        *src/app/critical/*.o (.data .data.*)
        *libos_App.a:heap-static.o (.data .data.*)
        *libos_App.a:list.o (.data .data.*)
    }>RAM1 AT>InnerFlash

    rw_nocache_rRAM1_lRAM1 (NOLOAD) :
    {
        . = ALIGN(0x20);
        KEEP(* (.nocacheram .nocacheram.*))
    }>RAM1NOCACHE

    /*Cortex-M ϵ�е����������ض�λ������ʼ��ַ��Ҫ���ǣ�����N�������������쳣����*/
    /*������2���������ݣ���ΪM����Addr = ALIGN(4 * M) */
    /*��������cpu��64���жϣ���N = 80, M = 128, Addr = ALIGN(0x200)*/
    zi_preload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x200);
        KEEP(* (.table.exceptions .table.exceptions.*))
        KEEP(* (.table.vectors .table.vectors.*))
        *libos_App.a:loader.o (.bss .bss.* COMMON)
        *libos_App.a:int_hard.o (.bss .bss.* COMMON)
        KEEP(*libos_App.a:hard-exps.o (.bss .bss.* COMMON))
        *libos_App.a:hard-exp.o (.bss .bss.* COMMON)
        *libos_App.a:int.o (.bss .bss.* COMMON)
        *libos_App.a:ints.o (.bss .bss.* COMMON)
        *board/APOLLO-STM32F7/src/critical/*.o (.bss .bss.* COMMON)
        *libos_App.a:heap-static.o (.bss .bss.* COMMON)
        *libos_App.a:list.o (.bss .bss.* COMMON)
        *libos_App.a:cpu_peri_hal.o (.bss .bss.* COMMON)
    }>RAM1

    rw_sysload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x08);
        * (.data .data.*)
    }>RAM1 AT>InnerFlash
    zi_sysload_rextram_lextram (NOLOAD) :
    {
        . = ALIGN(0x08);
        *libos_App.a:gk_draw.o (.bss .bss.* COMMON)
        *libos_App.a:gk_clip.o (.bss .bss.* COMMON)
        *libos_App.a:gk_win.o (.bss .bss.* COMMON)
    }>extram

    zi_sysload_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x08);
        * (.bss .bss.* COMMON)
    }>RAM1


    routine_shell_data_section :
        {
                 KEEP(* (.ro_shell_data))
         }>RAM1 AT>InnerFlash

    expand_shell_data_section :
        {
             KEEP(* (.ex_shell_data))
         }>RAM1 AT>InnerFlash

    preload_cp_table :
    {
       . = ALIGN(0x08);
        preload_copy_table = .;
        LONG(16)
        LONG(3)

        LONG(LOADADDR(text_preload_rIbootRom_lIbootRom))
        LONG(ADDR(text_preload_rIbootRom_lIbootRom))
        LONG(SIZEOF(text_preload_rIbootRom_lIbootRom))
        LONG(1)

        LONG(LOADADDR(rw_preload_rRAM1_lIbootRom))
        LONG(ADDR(rw_preload_rRAM1_lIbootRom))
        LONG(SIZEOF(rw_preload_rRAM1_lIbootRom))
        LONG(1)

        LONG(LOADADDR(zi_preload_rRAM1_lRAM1))
        LONG(ADDR(zi_preload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_preload_rRAM1_lRAM1))
        LONG(0)
    }>InnerFlash

    sysload_cp_table :
    {
       . = ALIGN(0x08);
        sysload_copy_table = .;
        LONG(16)
        LONG(6)

        LONG(LOADADDR(text_sysload_rIbootRom_lIbootRom))
        LONG(ADDR(text_sysload_rIbootRom_lIbootRom))
        LONG(SIZEOF(text_sysload_rIbootRom_lIbootRom))
        LONG(1)

        LONG(LOADADDR(rw_sysload_rRAM1_lIbootRom))
        LONG(ADDR(rw_sysload_rRAM1_lIbootRom))
        LONG(SIZEOF(rw_sysload_rRAM1_lIbootRom))
        LONG(1)

        LONG(LOADADDR(zi_sysload_rextram_lextram))
        LONG(ADDR(zi_sysload_rextram_lextram))
        LONG(SIZEOF(zi_sysload_rextram_lextram))
        LONG(0)

        LONG(LOADADDR(zi_sysload_rRAM1_lRAM1))
        LONG(ADDR(zi_sysload_rRAM1_lRAM1))
        LONG(SIZEOF(zi_sysload_rRAM1_lRAM1))
        LONG(0)

        LONG(LOADADDR(routine_shell_data_section))
        LONG(ADDR(routine_shell_data_section))
        LONG(SIZEOF(routine_shell_data_section))
        LONG(1)

        LONG(LOADADDR(expand_shell_data_section))
        LONG(ADDR(expand_shell_data_section))
        LONG(SIZEOF(expand_shell_data_section))
        LONG(1)


    }>InnerFlash

    heap_rRAM1_lRAM1 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap_bottom = .;
    }>RAM1
    sys_heap_top = InnerRamStart + InnerRamSize;

      heap_rextram_lextram (NOLOAD) :
       {
           . = ALIGN(0x08);
           extram_heap_bottom = .;
       }>extram
       extram_heap_top = ExtRamStart + ExtRamSize;

     heap_rnocache_lextram (NOLOAD) :
      {
         . = ALIGN(0x08);
         NoCacheRam_heap_bottom = .;
      }>RAM1NOCACHE
     NoCacheRam_heap_top = NoCacheRamStart + NoCacheRamSize;

    heap_cp_table :
    {
        . = ALIGN(0x08);
        pHeapList = .;

/* Ĭ�϶ѣ���������λ�����ֱ����ǡ�sys�� */
        LONG(2)                     /*��ʾ�ö��������ڴ����*/
        LONG(0)                     /*�ö��Ϸ�����ڴ�Ķ���ߴ磬0��ʾʹ��ϵͳ����*/
        LONG(0)                     /*0=ͨ�ö�,1=ר�ö�,���ϵͳֻ��Ψһһ����,��ֻ����ͨ�ö�*/
        LONG(sys_heap_bottom)       /*��һ�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap_top)          /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��һ��ҳ�ߴ�=32bytes*/
        LONG(msp_bottom)            /*�ڶ��λ�ַ�����ճ�ʼ��ջ������϶���Ҫ��*/
        LONG(msp_top - 2048)         /*�ڶ��ε�ַ���ޣ���������1024�������ж�ʹ�õģ�����϶���Ҫ��*/
        LONG(24)                    /*�ڶ���ҳ�ߴ�=24bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*��������"sys"�Ƕѵ�����*/

            . = ALIGN(0x04);
            LONG(1)                      /*��ʾ�ö���1���ڴ����*/
            LONG(0)                     /*�ö��Ϸ�����ڴ�Ķ���ߴ磬0��ʾʹ��ϵͳ����*/
            LONG(0)                    /*0=ͨ�ö�,1=ר�ö�,���ϵͳֻ��Ψһһ����,��ֻ����ͨ�ö�*/
            LONG(extram_heap_bottom)      /*��һ�λ�ַ������϶���Ҫ��*/
            LONG(extram_heap_top)         /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
         LONG(128)                    /*��һ��ҳ�ߴ�=128bytes*/
            BYTE(0x65)                    /*'e'*/
            BYTE(0x78)                    /*'x'*/
            BYTE(0x74)                   /*'t'*/
            BYTE(0x72)                   /*'r'*/
            BYTE(0x61)                   /*'a'*/
            BYTE(0x6d)                   /*'m'*/
            BYTE(0)                      /*��������"extram"�Ƕѵ�����*/

        . = ALIGN(0x04);
        LONG(1)                     /*��ʾ�ö���1���ڴ����*/
        LONG(0)                     /*�ö��Ϸ�����ڴ�Ķ���ߴ磬0��ʾʹ��ϵͳ����*/
        LONG(0)                     /*0=ͨ�ö�,1=ר�ö�,���ϵͳֻ��Ψһһ����,��ֻ����ͨ�ö�*/
        LONG(NoCacheRam_heap_bottom)    /*��һ�λ�ַ������϶���Ҫ��*/
        LONG(NoCacheRam_heap_top)       /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(64)                    /*��һ��ҳ�ߴ�=128bytes*/
        BYTE(0x6e)                  /*'n'*/
        BYTE(0x6f)                  /*'o'*/
        BYTE(0x63)                  /*'c'*/
        BYTE(0x61)                  /*'a'*/
        BYTE(0x63)                  /*'c'*/
        BYTE(0x68)                  /*'h'*/
        BYTE(0x65)                  /*'e'*/
        BYTE(0)                     /*��������"nocache"�Ƕѵ�����*/

        . = ALIGN(0x04);
        LONG(0)                     /*������־*/
    }>InnerFlash

    Iboot_Info :
    {
        gc_ptFlashOffset = .;
        LONG(InnerFlashOffset)

        gc_ptFlashRange = .;
        LONG(InnerFlashRange)

        gc_ptRevSectors = .;
        LONG(RevSectors)

        gc_ptCacheRam1Size = .;
        LONG(CacheRam1Size)

        gc_ptCacheRam2Size = .;
        LONG(CacheRam2Size)

        gc_ptCacheRam3Size = .;
        LONG(CacheRam3Size)

        gc_ptCacheRam4Size = .;
        LONG(CacheRam4Size)

        gc_ptNoCacheRamSize = .;
        LONG(NoCacheRamSize)


                /*��ʱ������һЩ���� */
        gc_pAppOffset = .;
        gc_pAppRange = .;
        gc_ptIbootCtrl = .;
        gc_ptAppInfo = .;
        gc_AppAddr = .;
        __AppStart = .;
    }>InnerFlash




 shell_config_section :
        {
                KEEP(*(.shellconfig))
        }>InnerFlash

    routine_shell_section :
        {
                 KEEP(* (.ro_shell_cmd))
                 KEEP(* (.ro_shell_data))
        }>InnerFlash

    expand_shell_section :
        {
             KEEP(* (.ex_shell_cmd))
             KEEP(* (.ex_shell_data))
              . = ALIGN(0x08);
        }>InnerFlash

        Shell_Info :
    {
        p_shell_info = .;
        LONG(restore) /* ��bin�ļ�����ʼ��ַ*/
        LONG(IbootSize) /* ��bin�ļ�������С*/
    }>InnerFlash



}
/**** The End ****/