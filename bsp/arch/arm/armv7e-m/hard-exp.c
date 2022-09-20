//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:中断模块
//作者：lst
//版本：V1.1.0
//文件描述: cm4异常处理模块
//其他说明:
//修订历史:
//3. 日期: 2018-05-31
//   作者: chj
//   新版本号: V1.1.1
//   修改说明: 把__Exp_TableSet函数里面的
//  g_u32ExpTable[CN_SYSVECT_RESET]= (uint32_t)(*(u32*)0x00000004);改成了(u32)Init_Cpu;
//  0x00000004在某些CPU上可能会出错
//2. 日期: 2010-01-20
//   作者: lst
//   新版本号: V1.1.0
//   修改说明: 修改 exp_esr_tick函数，否则在tick中断中调用异步信号的使能和禁止
//      函数时，由于en_asyn_signal_counter等参数未正确设置，会出问题。
//1. 日期: 2009-08-30
//   作者: lst
//   新版本号: V1.0.0
//   修改说明:
//------------------------------------------------------
#include "stdint.h"
#include "string.h"
#include "hard-exp.h"
#include "blackbox.h"
#include "stdio.h"
#include "endian.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include "cpu.h"
#include "dbug.h"
#include "board-config.h"
#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#endif
extern struct IntMasterCtrl  tg_int_global;          //定义并初始化总中断控制结构
extern void __DJY_ScheduleAsynSignal(void);

//异常向量表的外部声名
extern void HardExp_SystickHandler(void);
extern void HardExp_SvcHandler(void);
extern void HardExp_MemfaultHandler(void);
extern void HardExp_UsagefaultHandler(void);
extern void HardExp_BusfaultHandler(void);

extern   uint32_t   msp_top[ ];
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
bool_t  HardExp_Analysis(struct BlackBoxThrowPara *parahead, u32 endian);
struct SystickReg volatile * const pg_systick_reg
                        = (struct SystickReg *)0xE000E010;
struct ScbReg volatile * const pg_scb_reg
                        = (struct ScbReg *)0xe000ed00;


//系统tick函数的声名
void (*user_systick)(u32 inc_ticks);

u32 g_u32ExpTable[16] __attribute__((section(".table.exceptions")));

// =============================================================================
// 功能:异常向量表初始化，将向量表存放于指定的地址段内,并使能各种异常
// 参数:无
// 返回:无
// =============================================================================
static void __Exp_TableSet(void)
{
    g_u32ExpTable[0]                            = (uint32_t)msp_top;
    g_u32ExpTable[CN_SYSVECT_RESET]             = (uint32_t)Init_Cpu;
    g_u32ExpTable[CN_SYSVECT_NMI]               = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_HARD_FAULT]        = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_MEMMANAGE_FAULT]   = (uint32_t) HardExp_MemfaultHandler;
    g_u32ExpTable[CN_SYSVECT_BUS_FAULT]         = (uint32_t) HardExp_BusfaultHandler;
    g_u32ExpTable[CN_SYSVECT_USAGE_FAULT]       = (uint32_t) HardExp_UsagefaultHandler;
    g_u32ExpTable[7]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[8]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[9]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[10]                           = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SVC]               = (u32)HardExp_SvcHandler;
    g_u32ExpTable[CN_SYSVECT_DEBUG]             = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[13]                           = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_PENDSV]            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SYSTICK]           = (u32)HardExp_SystickHandler;

    pg_scb_reg->VTOR = (u32)g_u32ExpTable;
    pg_scb_reg->systick_pri15 = 0xff;   //最低优先级,systick应该能随异步信号关断
    pg_scb_reg->svc_pri11 = 0x0;       //svc的优先级和异步信号相同。
    pg_scb_reg->SHCSR |=0x70000;     //允许 总线、用法、存储器 fault

    pg_scb_reg->CCR |= 0x610;       //强制SP自动对齐，除0
}

// =============================================================================
// 函数功能：__SwapExpCpuInfoByEndian
//          转换硬件CPU异常信息字节序
// 输入参数：cpuinfo, CPU异常通用信息
// 输出参数：cpuinfo, CPU异常通用信息（转换后）
// 返回值  ：无
// 说明    ：内部调用
// =============================================================================
void __SwapExpCpuInfoByEndian(struct SysExceptionInfo *cpuinfo)
{
    u32 *temp;
    u32 i,len;
    temp =(u32 *)cpuinfo;
    len = (sizeof(struct SysExceptionInfo))/4;
    for(i = 0; i<len; i++)
    {
        *temp = swapl(*temp);
        temp++;
    }
}

// =============================================================================
// 功能:系统内存管理异常服务函数，主要处理与memory manage 异常相关的异常
// 参数:core_info，异常核心现场
// 返回: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 fpu_used,u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    u32 ExpAddr;
    enum EN_BlackBoxAction Action;
    bool_t   shedule_bak,MMA_Flag=false;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));

    if(fpu_used)
        ExpRecord.ExpRegInfo = *((struct SysExpRegInfo *)core_info);
    else
        ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    if(pg_scb_reg->MFSR & CN_EXP_MFSR_MMARVALID)
    {
        ExpAddr = pg_scb_reg->MMFAR;
        MMA_Flag = true;
    }
    //handler the exp
    if(pg_scb_reg->MFSR & CN_EXP_MFSR_IACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: 0x%08x 处指令访问错误\n\r",ExpAddr);
        }
        else
        {
            printk("MEM Fault: 指令访问错, 无法分析出错地址\n\r");
        }
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_DACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: 0x%08x 处数据访问错误\n\r",ExpAddr);
        }
        else
        {
            printk("MEM Fault: 数据访问错, 无法分析出错地址\n\r");
        }
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MUNSTKERR)
    {
        printk("MEM Fault: 出栈错误\n\r");
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MSTKERR)
    {
        printk("MEM Fault: 入栈错误\n\r");
    }
    else
    {
        printk("MEM Fault: %s\n\r","未知存储器错误");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_MEMMANAGE_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);

   //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// 功能:系统硬件错误，可能会是由于其他的fault上仿的结果，如果是，则应该进一步查询原因
// 参数:core_info，异常核心现场
// 返回: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_HardFaultHandler(u32 fpu_used,u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    enum EN_BlackBoxAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));

    if(fpu_used)
        ExpRecord.ExpRegInfo = *((struct SysExpRegInfo *)core_info);
    else
        ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->HFSR & CN_EXP_HFSR_VECTBL)
    {
        printk("Hard Fault: 取向量表时发生错误\n\r");
    }
    else if(pg_scb_reg->UFSR & CN_EXP_HFSR_FORCED)
    {
        printk("Hard Fault: 总线fault升级\n\r");
    }
    else if(pg_scb_reg->HFSR & CN_EXP_HFSR_DEBUGEVT)
    {
        printk("Hard Fault: 因调试产生的fault\n\r");
    }
    else
    {
        printk("Hard Fault: %s\n\r","未知hardfault");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_HARD_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// 功能:系统由于调试引起的异常，不作分析，只报告错误
// 参数:core_info，异常核心现场
// 返回: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 fpu_used,u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));

    if(fpu_used)
        ExpRecord.ExpRegInfo = *((struct SysExpRegInfo *)core_info);
    else
        ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    printk("Debug Fault: %s\n\r","This is a debug fault");

    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;

    return EN_BLACKBOX_DEAL_IGNORE;
}

// =============================================================================
// 功能:系统内存管理异常服务函数，主要处理与memory manage 异常相关的异常
// 参数:core_info，异常核心现场
// 返回: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_BusFaultHandler(u32 fpu_used,u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_BlackBoxAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));

    if(fpu_used)
        ExpRecord.ExpRegInfo = *((struct SysExpRegInfo *)core_info);
    else
        ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);
    CoreRegister = &ExpRecord.ExpRegInfo.CoreInfo;

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->BFSR & CN_EXP_BFSR_IBUSERR)
    {
        printk("Bus Fault: 0x%08x处取指错误\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_PRECISERR)
    {
        //查找异常时的地址
        if(pg_scb_reg->BFSR & CN_EXP_BFSR_BFARVALID)
        {
            printk("Bus Fault: 0x%08x处数据访问错误, PC= 0x%08x\n\r",
                                pg_scb_reg->BFAR,CoreRegister->spr_pc);
        }
        else
        {
            printk("Bus Fault: 未知地址数据访问错误, PC= 0x%08x\n\r",
                                CoreRegister->spr_pc);
        }
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_IMPRECISERR)
    {
        printk("Bus Fault: 未知地址数据访问错误, PC= 0x%08x\n\r",
                            CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_UNSTKERR)
    {
        printk("Bus Fault: 出栈异常,SP = 0x%08x,PC = 0x%08x\n\r",
                            CoreRegister->spr_sp,CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_STKERR)
    {
        printk("Bus Fault: 入栈异常,SP = 0x%08x,PC = 0x%08x\n\r",
                            CoreRegister->spr_sp,CoreRegister->spr_pc);
    }
    else
    {
        printk("Bus Fault: %s\n\r","Unknown Fault,可能是中断向量表地址异常");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;

#if (DEBUG == 1)
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_BUS_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);
    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// 功能：此部分主要用来处理处理器的Usage异常信息
// 参数：core_info,异常核心现场
// 返回 ： EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 fpu_used,u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_BlackBoxAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));

    if(fpu_used)
        ExpRecord.ExpRegInfo = *((struct SysExpRegInfo *)core_info);
    else
        ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);
    CoreRegister = &ExpRecord.ExpRegInfo.CoreInfo;

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNDEFINSTR)
    {
        printk("Usage Fault: 未定义指令,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVSTATE)
    {
        printk("Usage Fault: 试图切入ARM状态,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVPC)
    {
        printk("Usage Fault: 设置PC时使用了非法值,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_NOCP)
    {
        printk("Usage Fault: 协处理器指令错误, PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNALIGNED)
    {
        printk("Usage Fault: 非对齐地址访问, PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_DIVBYZERO)
    {
        printk("Usage Fault: 除0错误,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else
    {
        printk("Usage Fault: %s\n\r","Unknown Fault");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_USAGE_FAULT;

   Action = BlackBox_ThrowExp(&head);
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// 函数功能：异常信息解析，打印异常时的分析结果和寄存器值
// 输入参数：layer,抛出层次
//           parahead，抛出时的参数
//           endian, 信息的存储格式
// 输出参数：无
// 返回值  : true成功, false失败
// =============================================================================
bool_t  HardExp_Analysis(struct BlackBoxThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //非本层信息，无需解析
        printk("CPU核心硬件没有异常\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//看看包是否完整
        {
            //基本算是完整包，慢慢解析吧
            mycpuinfo = (struct SysExceptionInfo *)(parahead->BlackBoxInfo);
            //存储端转换
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//当前版本，可以解析
            {
                printk("异常类型: ");
                switch(parahead->BlackBoxAction)
                {
                    case CN_EXP_TYPE_HARD_FAULT: printk("hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: printk("存储器管理异常\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: printk("总线异常\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: printk("用法异常\r\n");break;
                    default: break;
                }
                printk("异常最终动作:");
                switch(parahead->BlackBoxAction)
                {
                    case EN_BLACKBOX_DEAL_RECORD: printk("仅记录\r\n");break;
                    case EN_BLACKBOX_DEAL_RESET: printk("硬件复位\r\n");break;
                    case EN_BLACKBOX_DEAL_REBOOT: printk("跳转到启动地址,重新初始化内存和时钟\r\n");break;
                    case EN_BLACKBOX_DEAL_RESTART: printk("跳转到预加载之后的运行地址\r\n");break;
                    case EN_BLACKBOX_DEAL_WAIT: printk("死循环等待\r\n");break;
                    default: break;
                }
                //EXP FAULT 信息解析
                printk("异常时CPU寄存器值:\n\r");
                printk("R0 :0X%08x R1 :0x%08x R2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r0,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r1,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r2);
                printk("R3 :0X%08x R4 :0x%08x R5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r3,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r4,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r5);
                printk("R6 :0X%08x R7 :0x%08x R8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r6,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r7,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r8);
                printk("R9 :0X%08x R10:0x%08x R11:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r9,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r10,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r11);
                printk("R12:0X%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r12);
                printk("SP :0X%08x LR :0x%08x PC :0x%08x  XPSR:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.spr_sp,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_lr,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_pc,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_xpsr);
                printk("NVIC Exception Registers:\n\r");
                printk("BFAR:0x%08x MMAR:0x%08x MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfar,\
                                                                 mycpuinfo->NvicInfo.mmar,\
                                                                 mycpuinfo->NvicInfo.mfsr);
                printk("HFSR:0x%08x DFSR:0x%08x BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.hfsr,\
                                                                 mycpuinfo->NvicInfo.dfsr,\
                                                                 mycpuinfo->NvicInfo.bfsr);
                printk("UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);

#if(_D_FPU_USED == 1)
                printk("Cortex M4 FPU Register:\r\n");
                printk("S0 :0X%08x S1 :0x%08x S2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s0,\
                                                              mycpuinfo->ExpRegInfo.fpu_s1,\
                                                              mycpuinfo->ExpRegInfo.fpu_s2);
                printk("S3 :0X%08x S4 :0x%08x S5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s3,\
                                                              mycpuinfo->ExpRegInfo.fpu_s4,\
                                                              mycpuinfo->ExpRegInfo.fpu_s5);
                printk("S6 :0X%08x S7 :0x%08x S8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s6,\
                                                              mycpuinfo->ExpRegInfo.fpu_s7,\
                                                              mycpuinfo->ExpRegInfo.fpu_s8);
                printk("S9 :0X%08x S10:0x%08x S11:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s9,\
                                                              mycpuinfo->ExpRegInfo.fpu_s10,\
                                                              mycpuinfo->ExpRegInfo.fpu_s11);
                printk("S12:0X%08x S13:0x%08x S14:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s12,\
                                                              mycpuinfo->ExpRegInfo.fpu_s13,\
                                                              mycpuinfo->ExpRegInfo.fpu_s14);
                printk("S15:0X%08x S16:0x%08x S17:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s15,\
                                                              mycpuinfo->ExpRegInfo.fpu_s16,\
                                                              mycpuinfo->ExpRegInfo.fpu_s17);
                printk("S18:0X%08x S19:0x%08x S20:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s18,\
                                                              mycpuinfo->ExpRegInfo.fpu_s19,\
                                                              mycpuinfo->ExpRegInfo.fpu_s20);
                printk("S21:0X%08x S22:0x%08x S23:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s21,\
                                                              mycpuinfo->ExpRegInfo.fpu_s22,\
                                                              mycpuinfo->ExpRegInfo.fpu_s23);
                printk("S24:0X%08x S25:0x%08x S26:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s24,\
                                                              mycpuinfo->ExpRegInfo.fpu_s25,\
                                                              mycpuinfo->ExpRegInfo.fpu_s26);
                printk("S27:0X%08x S28:0x%08x S29:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s27,\
                                                              mycpuinfo->ExpRegInfo.fpu_s28,\
                                                              mycpuinfo->ExpRegInfo.fpu_s29);
                printk("S30:0X%08x S31:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s30,\
                                                              mycpuinfo->ExpRegInfo.fpu_s31);
                printk("FPSCR:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_fpscr);

 #endif
                result = true;
            }
            else
            {
                printk("CPU Info Decode:ivalid info flag!\n\r");
                result = false; //可能不是一样的CPU或者版本，无法解析
            }
        }
        else
        {
            printk("CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }
    return result;
}
// =============================================================================
// 函数功能：异常信息解析，解析异常信息
// 输入参数：layer,抛出层次
//           parahead，抛出时的参数
//           endian, 信息的存储格式
// 输出参数：无
// 返回值  : true成功, false失败
// =============================================================================
bool_t  HardExp_Decoder(struct BlackBoxThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //非本层信息，无需解析
        debug_printf("cortex_EXP","CPU核心硬件没有异常\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//看看包是否完整
        {
            //基本算是完整包，慢慢解析吧
            mycpuinfo = (struct SysExceptionInfo *)(parahead->BlackBoxInfo);
            //存储端转换
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//当前版本，可以解析
            {
                debug_printf("cortex_EXP","异常类型: ");
                switch(parahead->BlackBoxType)
                {
                    case CN_EXP_TYPE_HARD_FAULT: debug_printf("cortex_EXP","hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: debug_printf("cortex_EXP","存储器管理异常\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: debug_printf("cortex_EXP","总线异常\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: debug_printf("cortex_EXP","用法异常\r\n");break;
                    default: break;
                }
                debug_printf("cortex_EXP","异常最终动作:");
                switch(parahead->BlackBoxAction)
                {
                    case EN_BLACKBOX_DEAL_RECORD: debug_printf("cortex_EXP","仅记录\r\n");break;
                    case EN_BLACKBOX_DEAL_RESET: debug_printf("cortex_EXP","硬件复位\r\n");break;
                    case EN_BLACKBOX_DEAL_REBOOT: debug_printf("cortex_EXP","跳转到启动地址,重新初始化内存和时钟\r\n");break;
                    case EN_BLACKBOX_DEAL_RESTART: debug_printf("cortex_EXP","跳转到预加载之后的运行地址\r\n");break;
                    case EN_BLACKBOX_DEAL_WAIT: debug_printf("cortex_EXP","死循环等待\r\n");break;
                    default: break;
                }
                //EXP FAULT 信息解析
                debug_printf("cortex_EXP","异常时CPU寄存器值:\n\r");
                debug_printf("cortex_EXP","R0 :0X%08x R1 :0x%08x R2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r0,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r1,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r2);
                debug_printf("cortex_EXP","R3 :0X%08x R4 :0x%08x R5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r3,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r4,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r5);
                debug_printf("cortex_EXP","R6 :0X%08x R7 :0x%08x R8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r6,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r7,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r8);
                debug_printf("cortex_EXP","R9 :0X%08x R10:0x%08x R11:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r9,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r10,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r11);
                debug_printf("cortex_EXP","R12:0X%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r12);
                debug_printf("cortex_EXP","SP :0X%08x LR :0x%08x PC :0x%08x  XPSR:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.spr_sp,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_lr,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_pc,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_xpsr);
                debug_printf("cortex_EXP","NVIC Exception Registers:\n\r");
                debug_printf("cortex_EXP","BFAR:0x%08x MMAR:0x%08x MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfar,\
                                                                 mycpuinfo->NvicInfo.mmar,\
                                                                 mycpuinfo->NvicInfo.mfsr);
                debug_printf("cortex_EXP","HFSR:0x%08x DFSR:0x%08x BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.hfsr,\
                                                                 mycpuinfo->NvicInfo.dfsr,\
                                                                 mycpuinfo->NvicInfo.bfsr);
                debug_printf("cortex_EXP","UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);

     #if(_D_FPU_USED == 1)
                debug_printf("cortex_EXP","Cortex M7 FPU Register:\r\n");
                debug_printf("cortex_EXP","S0 :0X%08x S1 :0x%08x S2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s0,\
                                                              mycpuinfo->ExpRegInfo.fpu_s1,\
                                                              mycpuinfo->ExpRegInfo.fpu_s2);
                debug_printf("cortex_EXP","S3 :0X%08x S4 :0x%08x S5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s3,\
                                                              mycpuinfo->ExpRegInfo.fpu_s4,\
                                                              mycpuinfo->ExpRegInfo.fpu_s5);
                debug_printf("cortex_EXP","S6 :0X%08x S7 :0x%08x S8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s6,\
                                                              mycpuinfo->ExpRegInfo.fpu_s7,\
                                                              mycpuinfo->ExpRegInfo.fpu_s8);
                debug_printf("cortex_EXP","S9 :0X%08x S10:0x%08x S11:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s9,\
                                                              mycpuinfo->ExpRegInfo.fpu_s10,\
                                                              mycpuinfo->ExpRegInfo.fpu_s11);
                debug_printf("cortex_EXP","S12:0X%08x S13:0x%08x S14:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s12,\
                                                              mycpuinfo->ExpRegInfo.fpu_s13,\
                                                              mycpuinfo->ExpRegInfo.fpu_s14);
                debug_printf("cortex_EXP","S15:0X%08x S16:0x%08x S17:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s15,\
                                                              mycpuinfo->ExpRegInfo.fpu_s16,\
                                                              mycpuinfo->ExpRegInfo.fpu_s17);
                debug_printf("cortex_EXP","S18:0X%08x S19:0x%08x S20:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s18,\
                                                              mycpuinfo->ExpRegInfo.fpu_s19,\
                                                              mycpuinfo->ExpRegInfo.fpu_s20);
                debug_printf("cortex_EXP","S21:0X%08x S22:0x%08x S23:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s21,\
                                                              mycpuinfo->ExpRegInfo.fpu_s22,\
                                                              mycpuinfo->ExpRegInfo.fpu_s23);
                debug_printf("cortex_EXP","S24:0X%08x S25:0x%08x S26:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s24,\
                                                              mycpuinfo->ExpRegInfo.fpu_s25,\
                                                              mycpuinfo->ExpRegInfo.fpu_s26);
                debug_printf("cortex_EXP","S27:0X%08x S28:0x%08x S29:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s27,\
                                                              mycpuinfo->ExpRegInfo.fpu_s28,\
                                                              mycpuinfo->ExpRegInfo.fpu_s29);
                debug_printf("cortex_EXP","S30:0X%08x S31:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_s30,\
                                                              mycpuinfo->ExpRegInfo.fpu_s31);
                debug_printf("cortex_EXP","FPSCR:0x%08x\n\r",mycpuinfo->ExpRegInfo.fpu_fpscr);

                result = true;
    #endif
            }
            else
            {
                debug_printf("cortex_EXP","CPU Info Decode:ivalid info flag!\n\r");
                result = false; //可能不是一样的CPU或者版本，无法解析
            }
        }
        else
        {
            debug_printf("cortex_EXP","CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }
    return result;
}
// =============================================================================
// 函数功能: 异常数据解析函数,异常硬件解码函数的注册,初始化异常模块后调用
// 输入参数: 无
// 输出参数: 无
// 返回值     :true成功, false失败
// =============================================================================
bool_t HardExp_InfoDecoderInit(void)
{
    static struct BlackBoxInfoDecoder HardExpDecoder;
    bool_t result;
    HardExpDecoder.MyDecoder = HardExp_Decoder;
    HardExpDecoder.DecoderName = CN_HARDEXP_DECODERNAME;
    result = BlackBox_RegisterThrowInfoDecoder(&HardExpDecoder);

    return result;
}

// =============================================================================
// 函数功能：异常初始化,包括异常向量表的初始化
// 输入参数：暂时无（后续待定）
// 输出参数：无
// 返回值     :true成功, false失败
// =============================================================================
void HardExp_Init(void)
{
    __Exp_TableSet();
}
