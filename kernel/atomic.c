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
//所属模块:功能函数库
//作者：网络
//版本：V1.0.0
//文件描述:原子变量操作部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "int.h"

#include "component_config_int.h"

//-----------------------------------------------------------------------------
//功能: 32位无符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_Set32(u32 *base, u32 value)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base = value;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 32位无符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_Set64(u64 *base, u64 value)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base = value;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 32位无符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_UADD32(u32 *base, u32 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 32位无符号数原子减法
//参数: base: 被操作数地址
//      sub: 减数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_USUBb2(u32 *base, u32 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 32位有符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_SADD32(s32 *base, s32 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 32位有符号数原子减法
//参数: base: 被操作数地址
//      sub: 减数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_SSUB32(s32 *base, s32 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 64位无符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_UADD64(u64 *base, u64 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 64位无符号数原子减法
//参数: base: 被操作数地址
//      sub: 减数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_USUB64(u64 *base, u64 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 64位有符号数原子加法
//参数: base: 被操作数地址
//      inc: 加数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_SADD64(s64 *base, s64 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//功能: 64位有符号数原子减法
//参数: base: 被操作数地址
//      sub: 减数
//返回: 无
//注，本函数只能防止线程或普通中断(异步信号)共享冲突，如果你在实时中断中访问base
//    导致共享冲突，神仙也救不了你
//-----------------------------------------------------------------------------
void Atom_SSUB64(s64 *base, s64 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}


