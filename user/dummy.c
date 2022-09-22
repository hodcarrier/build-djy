/*
 * work in progress routines.
 */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wimplicit-int"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-weak-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-weak-variable-attribute
 */
#define __weak                          __attribute__((__weak__))

__weak printf(){}
__weak printk(){}
__weak fopen(){}
__weak fclose(){}
__weak fread(){}
__weak fwrite(){}
__weak File_Format(){}
__weak Wdt_Clean(){}
__weak BlackBox_ThrowExp(){}
__weak debuglevel(){}
__weak gc_pAppOffset(){}
__weak sprintf(){}
__weak Wdt_Create(){}
__weak Board_Init(){}
__weak pHeapList(){}
__weak fd2stdio(){}
__weak __Multiplex_Set(){}
__weak djy_flash_write_ori(){}
__weak Cache_config(){}
__weak ModuleInstall_Shell(){}
__weak ModuleInstall_BlackBox(){}
__weak ModuleInstall_dev(){}
__weak ModuleInstall_Multiplex(){}
__weak ModuleInstall_EmbededFlash(){}
__weak ModuleInstall_EmFlashInstallXIP(){}
__weak ModuleInstall_UART(){}
__weak ModuleInstall_STDIO(){}
__weak BlackBox_RegisterThrowInfoDecoder(){}
__weak gc_ProductSn(){}
__weak Init_Cpu(){}
__weak msp_top(){}
__weak preload_copy_table(){}
__weak sscanf(){}
__weak Stdio_KnlInOutInit(){}
__weak sysload_copy_table(){}


/* user/keepshell.c */
void *djysh_startmsg;
void *djysh_linemem;
void *djysh_blackboxrtest;
void *djysh_blackboxi;
void *djysh_heap;
void *djysh_evtt;
void *djysh_iapmode;
void *djysh_resetshell;
void *djysh_help;
void *djysh_time;
