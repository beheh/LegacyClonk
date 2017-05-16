/* Copyright (C) 1998-2000  Matthes Bender  RedWolf Design */

/* All kinds of valuable helpers */

#ifndef INC_STANDARD
#define INC_STANDARD

// A standard product name for this project which is used in window registration etc.
#define STD_PRODUCT "Clonk Rage"

#ifdef HAVE_CONFIG_H
#include <config.h>
#elif defined(_WIN32)
#define HAVE_IO_H 1
#define HAVE_DIRECT_H 1
#define HAVE_SHARE_H 1
#define HAVE_FREETYPE
#endif // _WIN32, HAVE_CONFIG_H

// debug memory management
#if !defined(NODEBUGMEM) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

// Integer dataypes
#include <stdint.h>

#ifdef __GNUC__
	// Temporary-To-Reference-Fix
	#define ALLOW_TEMP_TO_REF(ClassName) operator ClassName & () { return *this; }
	// Allow checks for correct printf-usage
	#define GNUC_FORMAT_ATTRIBUTE __attribute__ ((format (printf, 1, 2)))
	#define GNUC_FORMAT_ATTRIBUTE_O __attribute__ ((format (printf, 2, 3)))
	#define ALWAYS_INLINE inline __attribute__ ((always_inline))
#else
	#define ALLOW_TEMP_TO_REF(ClassName)
	#define GNUC_FORMAT_ATTRIBUTE
	#define GNUC_FORMAT_ATTRIBUTE_O
	#define ALWAYS_INLINE __forceinline
#endif

#if defined(_DEBUG) && defined(_MSC_VER)
	// use inline assembler to invoke the "breakpoint exception"
	#define BREAKPOINT_HERE _asm int 3
#elif defined(_DEBUG) && defined(HAVE_SIGNAL_H)
	#include <signal.h>
	#if defined(SIGTRAP)
		#define BREAKPOINT_HERE raise(SIGTRAP);
	#else
		#define BREAKPOINT_HERE
	#endif
#else
	#define BREAKPOINT_HERE
#endif

#include <string.h>

#ifdef _WIN32
	#ifndef _INC_WINDOWS
		#define _WIN32_WINDOWS 0x0500
		#define _WIN32_WINNT 0x0501
		#define WINVER 0x0500
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
#else
// Boolean datatype
typedef int BOOL;
#define TRUE  true
#define FALSE false

// Windows integer types
typedef uint32_t DWORD;
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t UINT;

typedef struct
{
	long left; long top; long right; long bottom;
} RECT;

#define INFINITE 0xFFFFFFFF

unsigned long timeGetTime(void);

inline int stricmp(const char *s1, const char *s2)
{
	return strcasecmp(s1, s2);
}

#define GetRValue(rgb) ((unsigned char)(rgb))
#define GetGValue(rgb) ((unsigned char)(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb) ((unsigned char)((rgb) >> 16))
#define RGB(r, g, b) ((DWORD)((BYTE)(r) | ((BYTE)(g) << 8) | ((BYTE)(b) << 16)))
#define ZeroMemory(d, l) memset((d), 0, (l))
#endif // _WIN32

// These functions have to be provided by the application.
bool Log(const char *szMessage);
bool LogSilent(const char *szMessage);
BOOL LogF(const char *strMessage, ...) GNUC_FORMAT_ATTRIBUTE;
BOOL LogSilentF(const char *strMessage, ...) GNUC_FORMAT_ATTRIBUTE;

#include <memory.h>
#include <math.h>

// Color triplets
#define C4RGB(r, g, b) ((((DWORD)(r) & 0xff) << 16) | (((DWORD)(g) & 0xff) << 8) | ((b) & 0xff))

// Small helpers
template <class T> inline T Max(T val1, T val2) { return val1 > val2 ? val1 : val2; }
template <class T> inline T Min(T val1, T val2) { return val1 < val2 ? val1 : val2; }
template <class T> inline T Abs(T val) { return val > 0 ? val : -val; }
template <class T> inline bool Inside(T ival, T lbound, T rbound) { return ival >= lbound && ival <= rbound; }
template <class T> inline T BoundBy(T bval, T lbound, T rbound) { return bval < lbound ? lbound : bval > rbound ? rbound : bval; }
template <class T> inline int Sign(T val) { return val < 0 ? -1 : val > 0 ? 1 : 0; }
template <class T> inline void Swap(T &v1, T &v2) { T t = v1; v1 = v2; v2 = t; }
template <class T> inline void Toggle(T &v) { v = !v; }

const double pi = 3.14159265358979323846;

inline int DWordAligned(int val)
{
	if (val % 4) { val >>= 2; val <<= 2; val += 4; }
	return val;
}

inline int32_t ForceLimits(int32_t &rVal, int32_t iLow, int32_t iHi)
{
	if (rVal < iLow) { rVal = iLow; return -1; }
	if (rVal > iHi)  { rVal = iHi;  return +1; }
	return 0;
}

int32_t Distance(int32_t iX1, int32_t iY1, int32_t iX2, int32_t iY2);
int Angle(int iX1, int iY1, int iX2, int iY2);
int Pow(int base, int exponent);

inline void FillMem(void *lpMem, size_t dwSize, char bValue)
{
	memset(lpMem, bValue, dwSize);
}

inline void ZeroMem(void *lpMem, size_t dwSize)
{
	FillMem(lpMem, dwSize, 0);
}

inline bool MemEqual(const void *lpMem1, const void *lpMem2, size_t dwSize)
{
	return !memcmp(lpMem1, lpMem2, dwSize);
}

inline void MemCopy(const void *lpMem1, void *lpMem2, size_t dwSize)
{
	memmove(lpMem2, lpMem1, dwSize);
}

bool ForLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
	bool(*fnCallback)(int32_t, int32_t, int32_t), int32_t iPar = 0,
	int32_t *lastx = NULL, int32_t *lasty = NULL);

char CharCapital(char cChar);
bool IsIdentifier(char cChar);
bool IsWhiteSpace(char cChar);

int SLen(const char *sptr);

bool SEqual(const char *szStr1, const char *szStr2);
bool SEqual2(const char *szStr1, const char *szStr2);
bool SEqualNoCase(const char *szStr1, const char *szStr2, int iLen = -1);
bool SEqual2NoCase(const char *szStr1, const char *szStr2, int iLen = -1);

void SCopy(const char *szSource, char *sTarget, int iMaxL = -1);
void SCopyUntil(const char *szSource, char *sTarget, char cUntil, int iMaxL = -1, int iIndex = 0);
void SCopyUntil(const char *szSource, char *sTarget, const char *sUntil, size_t iMaxL);
void SCopyIdentifier(const char *szSource, char *sTarget, int iMaxL = 0);
bool SCopySegment(const char *fstr, int segn, char *tstr, char sepa = ';', int iMaxL = -1, bool fSkipWhitespace = false);
bool SCopySegmentEx(const char *fstr, int segn, char *tstr, char sepa1, char sepa2, int iMaxL = -1, bool fSkipWhitespace = false);
bool SCopyEnclosed(const char *szSource, char cOpen, char cClose, char *sTarget, int iSize);

void SAppend(const char *szSource, char *szTarget, int iMaxL = -1);
void SAppendChar(char cChar, char *szStr);

void SInsert(char *szString, const char *szInsert, int iPosition = 0, int iMaxLen = -1);
void SDelete(char *szString, int iLen, int iPosition = 0);

int SCharPos(char cTarget, const char *szInStr, int iIndex = 0);
int SCharLastPos(char cTarget, const char *szInStr);
int SCharCount(char cTarget, const char *szInStr, const char *cpUntil = NULL);
int SCharCountEx(const char *szString, const char *szCharList);

void SReplaceChar(char *str, char fc, char tc);

const char *SSearch(const char *szString, const char *szIndex);
const char *SSearchNoCase(const char *szString, const char *szIndex);

const char *SAdvanceSpace(const char *szSPos);
const char *SAdvancePast(const char *szSPos, char cPast);

bool SGetModule(const char *szList, int iIndex, char *sTarget, int iSize = -1);
bool SIsModule(const char *szList, const char *szString, int *ipIndex = NULL, bool fCaseSensitive = false);
bool SAddModule(char *szList, const char *szModule, bool fCaseSensitive = false);
bool SAddModules(char *szList, const char *szModules, bool fCaseSensitive = false);
bool SRemoveModule(char *szList, const char *szModule, bool fCaseSensitive = false);
bool SRemoveModules(char *szList, const char *szModules, bool fCaseSensitive = false);
int SModuleCount(const char *szList);

const char *SGetParameter(const char *strCommandLine, int iParameter, char *strTarget = NULL, int iSize = -1, bool *pWasQuoted = NULL);

void SNewSegment(char *szStr, const char *szSepa = ";");
void SCapitalize(char *szString);
void SWordWrap(char *szText, char cSpace, char cSepa, int iMaxLine);
int SClearFrontBack(char *szString, char cClear = ' ');

int SGetLine(const char *szText, const char *cpPosition);
int SLineGetCharacters(const char *szText, const char *cpPosition);

// case sensitive wildcard match with some extra functionality
// can match strings like  "*Cl?nk*vour" to "Clonk Endeavour"
BOOL SWildcardMatchEx(const char *szString, const char *szWildcard);

#define LineFeed "\x00D\x00A"
#define EndOfFile "\x020"

#ifdef _WIN32
#define DirSep "\\"
#else
#define DirSep "/"
#endif

void StdBlit(uint8_t *bypSource, int iSourcePitch, int iSrcBufHgt,
	int iSrcX, int iSrcY, int iSrcWdt, int iSrcHgt,
	uint8_t *bypTarget, int iTargetPitch, int iTrgBufHgt,
	int iTrgX, int iTrgY, int iTrgWdt, int iTrgHgt,
	int iBytesPerPixel = 1, bool fFlip = false);

// sprintf wrapper

#include <stdio.h>
#include <stdarg.h>

// old, insecure sprintf
inline int osprintf(char *str, const char *fmt, ...) GNUC_FORMAT_ATTRIBUTE_O;
inline int osprintf(char *str, const char *fmt, ...)
{
	va_list args; va_start(args, fmt);
	return vsprintf(str, fmt, args);
}

// wrapper to detect "char *"
template <typename T> struct NoPointer { static void noPointer() {} };
template <> struct NoPointer<char *> {};

// format string security
bool IsSafeFormatString(const char *szFmt);

// secure sprintf
#define sprintf ssprintf
template <typename T>
inline int ssprintf(T &str, const char *fmt, ...) GNUC_FORMAT_ATTRIBUTE_O;
template <typename T>
inline int ssprintf(T &str, const char *fmt, ...)
{
	// Check parameters
	NoPointer<T>::noPointer();
	if (!IsSafeFormatString(fmt))
	{
		BREAKPOINT_HERE
			fmt = "<UNSAFE FORMAT STRING>";
	}
	int n = sizeof(str);
	// Build string
	va_list args; va_start(args, fmt);
	int m = vsnprintf(str, n, fmt, args);
	if (m >= n) { m = n - 1; str[m] = 0; }
	return m;
}

// open a weblink in an external browser
bool OpenURL(const char *szURL);

#endif // INC_STANDARD