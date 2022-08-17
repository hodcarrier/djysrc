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
        KEEP(*libos_Iboot.a:initcpuc.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:pre_loader.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:cpu_peri_gpio.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:IAP_pLoad.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:set-cache.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:cpu_peri_hal.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_hal.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_hal_tim.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_hal_rcc.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_hal_pwr_ex.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_hal_cortex.o (.text .text.* .rodata .rodata.*))
        KEEP(*libos_Iboot.a:stm32h7xx_ll_fmc.o (.text .text.* .rodata .rodata.*))
    }>InnerFlash

    text_preload_rIbootRom_lIbootRom :
    {
       . = ALIGN(0x08);
        g_pAppCodeStartRamAddr = .;
        *libos_Iboot.a:loader.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:int_hard.o (.text .text.* .rodata .rodata.*)
         KEEP(*libos_Iboot.a:hard-exps.o (.text .text.* .rodata .rodata.*))
        *libos_Iboot.a:hard-exp.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:int.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:ints.o (.text .text.* .rodata .rodata.*)
        *src/iboot/critical/*.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:heap-static.o (.text .text.* .rodata .rodata.*)
        *libos_Iboot.a:list.o (.text .text.* .rodata .rodata.*)
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
        *libos_Iboot.a:loader.o (.data .data.*)
        *libos_Iboot.a:int_hard.o (.data .data.*)
        KEEP(*libos_Iboot.a:hard-exps.o (.data .data.*))
        *libos_Iboot.a:hard-exp.o (.data .data.*)
        *libos_Iboot.a:int.o (.data .data.*)
        *libos_Iboot.a:ints.o (.data .data.*)
        *src/iboot/critical/*.o (.data .data.*)
        *libos_Iboot.a:heap-static.o (.data .data.*)
        *libos_Iboot.a:list.o (.data .data.*)
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
        *libos_Iboot.a:loader.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int_hard.o (.bss .bss.* COMMON)
        KEEP(*libos_Iboot.a:hard-exps.o (.bss .bss.* COMMON))
        *libos_Iboot.a:hard-exp.o (.bss .bss.* COMMON)
        *libos_Iboot.a:int.o (.bss .bss.* COMMON)
        *libos_Iboot.a:ints.o (.bss .bss.* COMMON)
        *src/iboot/critical/*.o (.bss .bss.* COMMON)
        *libos_Iboot.a:heap-static.o (.bss .bss.* COMMON)
        *libos_Iboot.a:list.o (.bss .bss.* COMMON)
        *libos_Iboot.a:cpu_peri_hal.o (.bss .bss.* COMMON)
    }>RAM1

    rw_sysload_rRAM1_lIbootRom :
    {
       . = ALIGN(0x08);
        * (.data .data.*)
    }>RAM1 AT>InnerFlash
    zi_sysload_rextram_lextram (NOLOAD) :
    {
        . = ALIGN(0x08);
        *libos_Iboot.a:gk_draw.o (.bss .bss.* COMMON)
        *libos_Iboot.a:gk_clip.o (.bss .bss.* COMMON)
        *libos_Iboot.a:gk_win.o (.bss .bss.* COMMON)
    }>extram1

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
        sys_heap1_bottom = .;
    }>RAM1
    sys_heap1_top = InnerRam1Start + InnerRam1Size;

    heap_rRAM2_lRAM2 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap2_bottom = .;
    }>RAM2
    sys_heap2_top = InnerRam2Start + InnerRam2Size;

    heap_rRAM3_lRAM3 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap3_bottom = .;
    }>RAM3
    sys_heap3_top = InnerRam3Start + InnerRam3Size;

    heap_rRAM4_lRAM4 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap4_bottom = .;
    }>RAM4
    sys_heap4_top = InnerRam4Start + InnerRam4Size;

    heap_rRAM5_lRAM5 (NOLOAD) :
    {
       . = ALIGN(0x08);
        sys_heap5_bottom = .;
    }>RAM5
    sys_heap5_top = InnerRam5Start + InnerRam5Size;

     heap_rextram1_lextram (NOLOAD) :
      {
          . = ALIGN(0x08);
          extram1_heap_bottom = .;
      }>extram1
      extram1_heap_top = ExtRam1Start + ExtRam1Size;

     heap_rextram2_lextram (NOLOAD) :
      {
          . = ALIGN(0x08);
          extram2_heap_bottom = .;
      }>extram2
      extram2_heap_top = ExtRam2Start + ExtRam2Size;

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
        LONG(6)                     /*��ʾ�ö��������ڴ����*/
        LONG(0)                     /*�ö��Ϸ�����ڴ�Ķ���ߴ磬0��ʾʹ��ϵͳ����*/
        LONG(0)                     /*0=ͨ�ö�,1=ר�ö�,���ϵͳֻ��Ψһһ����,��ֻ����ͨ�ö�*/
        LONG(sys_heap1_bottom)       /*��һ�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap1_top)          /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��1��ҳ�ߴ�=32bytes*/
        LONG(sys_heap2_bottom)      /*��2�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap2_top)         /*��2�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��2��ҳ�ߴ�=32bytes*/
        LONG(sys_heap3_bottom)      /*��3�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap3_top)         /*��3�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��3��ҳ�ߴ�=32bytes*/
        LONG(sys_heap4_bottom)      /*��4�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap4_top)         /*��4�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��4��ҳ�ߴ�=32bytes*/
        LONG(sys_heap5_bottom)      /*��5�λ�ַ������϶���Ҫ��*/
        LONG(sys_heap5_top)         /*��5�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(32)                    /*��5��ҳ�ߴ�=32bytes*/
        LONG(msp_bottom)            /*��6�λ�ַ�����ճ�ʼ��ջ������϶���Ҫ��*/
        LONG(msp_top - 1024)         /*��6�ε�ַ���ޣ���������1024�������ж�ʹ�õģ�����϶���Ҫ��*/
        LONG(24)                    /*�ڶ���ҳ�ߴ�=24bytes*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0x79)                  /*'y'*/
        BYTE(0x73)                  /*'s'*/
        BYTE(0)                     /*��������"sys"�Ƕѵ�����*/

            . = ALIGN(0x04);
            LONG(2)                     /*��ʾ�ö���1���ڴ����*/
            LONG(0)                     /*�ö��Ϸ�����ڴ�Ķ���ߴ磬0��ʾʹ��ϵͳ����*/
            LONG(0)                     /*0=ͨ�ö�,1=ר�ö�,���ϵͳֻ��Ψһһ����,��ֻ����ͨ�ö�*/
            LONG(extram1_heap_bottom)    /*��һ�λ�ַ������϶���Ҫ��*/
            LONG(extram1_heap_top)       /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(128)                   /*��һ��ҳ�ߴ�=128bytes*/
            LONG(extram2_heap_bottom)    /*��һ�λ�ַ������϶���Ҫ��*/
            LONG(extram2_heap_top)       /*��һ�ε�ַ���ޣ�������������϶���Ҫ��*/
        LONG(128)                   /*��һ��ҳ�ߴ�=128bytes*/
            BYTE(0x65)                  /*'e'*/
            BYTE(0x78)                  /*'x'*/
            BYTE(0x74)                  /*'t'*/
            BYTE(0x72)                  /*'r'*/
            BYTE(0x61)                  /*'a'*/
            BYTE(0x6d)                  /*'m'*/
            BYTE(0)                     /*��������"extram"�Ƕѵ�����*/

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

        gc_ptIbootSize = .;
        LONG(IbootSize)

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
    }>InnerFlash

    App_Info :
    {
        . = ALIGN(0x08);
        gc_pAppOffset = .;          /* APP�ռ�ƫ�� */
        LONG(InnerFlashOffset + IbootSize)
        gc_pAppRange = .;           /* APP�ռ䷶Χ */
        LONG(InnerFlashRange - IbootSize)
    }>InnerFlash

   Iboot_AppInfo :
    {
       /* . = restore + IbootSize;*/
        gc_ptIbootCtrl = .;         /*�ṹ������c�п��ã����ڼ��APP��ȷ��*/
        LONG( restore + IbootSize)
        . += 256;
        gc_ptAppInfo = .;           /*ROM�� �洢APP����ʼ��ַ����С */
        LONG( restore + IbootSize + 256)
        . += 256;
        gc_AppAddr = .;             /*����ָ�������ָ��APP����ʼ��ַ*/
        LONG( restore + IbootSize + 256 + 256)
        __AppStart = .;           /*����ָ�������ָ��APP��Preload����*/
        LONG( restore + IbootSize + 256 + 256)
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