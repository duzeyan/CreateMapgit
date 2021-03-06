////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C), 2015, 南京理工大学计算机科学与工程学院, 智能科学与技术系
//  FileName:  NJUST_GLOBAL_DEF.h
//  Date:   2015.7.2
//  Description: 全局定义
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NJUST_GLOBAL_DEF_H_
#define _NJUST_GLOBAL_DEF_H_
////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  对齐
//
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)    //按1字节对齐,必须放在第一句
////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  OS选择
//
////////////////////////////////////////////////////////////////////////////////////////////////
#define _NJUST_OS_USE_WINDOWS_
//#define _NJUST_OS_USE_LINUX_
//#define _NJUST_OS_USE_ARM_
////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  Windows
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NJUST_OS_USE_WINDOWS_
   //#define _CRT_SECURE_NO_WARNINGS
   #include <windows.h>
   #include <stdio.h>
#else
    #include <string.h>
#ifndef __cplusplus
    //#define min(x,y) ((x)>(y)?(y):(x))
    //#define max(x,y) ((x)>(y)?(x):(y))
#endif
   typedef unsigned char BYTE;
#endif

#endif