/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//此处填写手动配置，DIDE生成配置文件时，不会被修改
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_IBOOT           //由IBOOT加载的APP
#define    CN_RUNMODE_BOOTSELF             1                       //无须IBOOT，自启动模式APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu drive inner flash  ******************************************//
#define CFG_EFLASH_PART_FORMAT     false      //分区选项,是否需要擦除该芯片。
#define CFG_EFLASH_PAGE_SIZE                 512      //片内flash的页大小，单位字节。
#define CFG_EFLASH_SMALL_SECT_PAGE_NUM       256         //片内flash的小扇区中，有多少页。
#define CFG_EFLASH_LARGE_SECT_PAGE_NUM       0         //片内flash的大扇区中，有多少页。
#define CFG_EFLASH_NORMAL_SECT_PAGE_NUM      0         //片内flash的标准扇区中，有多少页。
#define CFG_EFLASH_PLANE_SMALL_SECT_NUM      16         //片内flash的主存储块中，有多少小扇区。
#define CFG_EFLASH_PLANE_LARGE_SECT_NUM      0         //片内flash的主存储块中，有多少大扇区。
#define CFG_EFLASH_PLANE_NORMAL_SECT_NUM     0       //片内flash的主存储块中，有多少标准扇区。
#define CFG_EFLASH_PLANE_NUM                 1         //片内flash的主存储块个数。
#define CFG_EFLASH_MAPPED_START_ADDR         0x8000000 //片内flash的映射起始地址。
#define    CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                  //
#define CFG_UART_RECVBUF_LEN            32                  //
#define CFG_UART1_ENABLE                false                //
#define CFG_UART2_ENABLE                true               //
#define CFG_UART3_ENABLE                true               //
#define CFG_UART4_ENABLE                false               //
#define CFG_UART5_ENABLE                false               //
#define CFG_UART6_ENABLE                false               //
#define CFG_UART7_ENABLE                false               //
#define CFG_UART8_ENABLE                false               //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure emflash insatall xip  ******************************************//
#define CFG_EFLASH_XIP_PART_START      6          //分区起始，填写块号，块号从0开始计算
#define CFG_EFLASH_XIP_PART_END        -1         //分区结束，-1表示最后一块，如果结束块是6，起始块是0，则该分区使用的块为0，1，2，3，4，5块
#define CFG_EFLASH_XIP_PART_FORMAT     false      //分区选项,是否需要格式化该分区。
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"    //需安装的文件系统的mount的名字，NULL表示该flash不挂载文件系统
#define    CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP忽略
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define CFG_FORCED_UPDATE_PATH     "/SD/djyapp.bin"           //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART2"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART2"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART2"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Core Clock  ******************************************//
#define    CFG_CORE_MCLK                   (216.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              "NUCLEO-H743ZI"   //板件类型
#define    PRODUCT_CPU_TYPE                "stm32h743xx"     //CPU类型


#endif
