//
// Created by 霍伟元 on 2022/10/2.
//

#ifndef CPPTEC_ZYDEF_H
#define CPPTEC_ZYDEF_H

#ifdef __i386
#define ByteSize (4)
#else
#define ByteSize (8)
#endif

namespace zy
{

using Size =  unsigned long;
using uint = unsigned int;
using ulong = unsigned long;
using ushort = unsigned short;
}

#endif //CPPTEC_ZYDEF_H
