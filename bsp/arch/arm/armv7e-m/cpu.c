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
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//3. 日期: 2018-05-31
//   作者: chj
//   新版本号: V1.0.2
//   修改说明: 把reboot函数里面的
//  InitCpu_Addr=*(u32*)0x00000004;改成了InitCpu_Addr = (u32)Init_Cpu;
//  0x00000004在某些CPU上可能会出错
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "dbug.h"
#include <board-config.h>
#include "Iboot_info.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_MSP(uint32_t topOfProcStack);
extern void Init_Cpu(void);
// void __start_systick(void);

///////////////////////////////////////////////djy-api end//////////////////////////////////

extern void Iboot_LoadPreload(void);
#include <blackbox.h>
#include <Iboot_info.h>

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG            (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)
typedef struct
{
    union
    {
        u32 value;
        struct
        {
            u32 procrst:1;
            u32 res3:2;
            u32 extrst:1;
            u32 res2:4;
            u32 res1:16;
            u32 key:8;
        }bits;
    }cr;
    union
    {
        u32 value;
        struct
        {
            u32 ursts:1;
            u32 res3:7;
            u32 rsttype:3;
            u32 res2:5;
            u32 nrstl:1;
            u32 srcmp:1;
            u32 res1:14;
        }bits;
    }sr;
    union
    {
        u32 value;
        struct
        {
            u32 ursten:1;
            u32 res3:3;
            u32 urstien:1;
            u32 res2:3;
            u32 erstl:4;
            u32 res1:12;
            u32 key:8;
        }bits;
    }mr;
}tagRstc;
// =============================================================================
// 功能：运行到选择系统运行方式前，对于M3/M4的CPU，即PC跳转到Init_CPU()
// 参数：无
// 返回：无
// =============================================================================
void CPU_Reboot(void)
{
    u32 InitCpu_Addr;
    Iboot_SetRebootFlag();
    Iboot_SetPreviouResetFlag();
    InitCpu_Addr = (u32)Init_Cpu;
    ((void (*)(void))(InitCpu_Addr))();

    return;
}
// =============================================================================
// 功能：Reset硬件CPU，相当于上电重新启动，硬件软件都得到复位
// 参数：无
// 返回：无
// =============================================================================
void CPU_Reset(void)
{
    Iboot_SetSoftResetFlag();
    Iboot_SetPreviouResetFlag();
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
    return;
}
// =============================================================================
// 功能：运行到CPU加载代码前，即pre_load()前
// 参数：无
// 返回：无
// =============================================================================
void CPU_RestartSystem(void)
{
    DJY_DelayUs(10);
    __set_PSP((uint32_t)msp_top);
    __set_MSP((uint32_t)msp_top);
    Iboot_LoadPreload();
    return;
}

enStartMode GetBootMethodSoft(void)
{
    enStartMode result = 0;
    return result;
}
enum
{
    EN_RESET_GENERALRST = 0,
    EN_RESET_BACKUPRST,
    EN_RESET_WDTRST,
    EN_RESET_SOFTRST,
    EN_RESET_USERRST,
    EN_RESET_RES,
}enResetType;
enStartMode GetBootMethodHard(void)
{
    enStartMode result;
    tagRstc rstc;
    rstc.cr.value = *(vu32 *)(0x400e1800);
    rstc.sr.value = *(vu32 *)(0x400e1804);
    rstc.mr.value = *(vu32 *)(0x400e1808);
    switch(rstc.sr.bits.rsttype)
    {
        case EN_RESET_GENERALRST:
            printk("General RESET\n\r");
            result =EN_Start_POWERDOWN;
            break;
        case EN_RESET_BACKUPRST:
            printk("BackUp RESET\n\r");
            result =EN_Start_UNKNOWN;
            break;
        case EN_RESET_WDTRST:
            printk("WDT RESET\n\r");
            result =EN_Start_UNKNOWN;
            break;
        case EN_RESET_SOFTRST:
            printk("SOFT RESET\n\r");
            result =EN_Start_SRST;
            break;
        case EN_RESET_USERRST:
            printk("USER RESET\n\r");
            result =EN_Start_HRST;
            break;
        default:
            printk("UNKOWN MODE\n\r");
            result =EN_Start_UNKNOWN;
            break;
    }
    return result;
}
