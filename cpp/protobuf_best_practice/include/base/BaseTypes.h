#ifndef INCL_BaseTypes_20131017061824_H_
#define INCL_BaseTypes_20131017061824_H_

typedef unsigned char  BYTE;
typedef char CHAR;

typedef unsigned short WORD;
typedef unsigned short WORD16;
typedef signed short   SWORD16;

typedef unsigned long  DWORD;
typedef DWORD          WORD32;
typedef signed long    SWORD32;

typedef double         DOUBLE;

#ifdef WIN32
    typedef unsigned __int64 WORD64;
#else
    typedef unsigned long long WORD64;
#endif

#endif
