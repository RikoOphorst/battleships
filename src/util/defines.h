#pragma once

#include <cmath>
#include <iostream>
#include <comdef.h>

// Define the ASSERT and DEBUG macros
#ifdef _WIN32
	#define ASSERT _ASSERT
	#ifdef _DEBUG 
		#define DEBUG
	#endif
#endif

// Note: BB stands for blowbox, a personal project of mine. This code is from that project.
// Source can be found on https://github.com/RikoOphorst/blowbox

#define BB_REPORT(cstr) OutputDebugString(cstr); OutputDebugString("\n"); printf(cstr); printf("\n");

#define BB_SAFE_DELETE(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }
#define BB_SAFE_RELEASE(ptr) if (ptr != nullptr) { ptr->Release(); ptr = nullptr; }

#ifdef _DEBUG
#define BB_CHECK(hr) { HRESULT _hr = hr; if (_hr != S_OK) { _com_error error(_hr); BB_REPORT(error.ErrorMessage()); DebugBreak(); } }
#define BB_GUARANTEE_DELETE(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; } else { BB_REPORT("Trying to delete a pointer that already is null."); DebugBreak(); }
#define BB_GUARANTEE_RELEASE(ptr) if (ptr != nullptr) { ptr->Release(); ptr = nullptr; } else { BB_REPORT("Trying to release a pointer that already is null."); DebugBreak(); }
#define BB_ASSERT_NOTNULL(expr, msg) if (!(expr != NULL && expr != nullptr)) { BB_REPORT(msg); DebugBreak(); }
#define BB_ASSERT(expr, msg) if (!(expr)) { BB_REPORT(msg); DebugBreak(); }
#else
#define BB_CHECK(hr) hr
#define BB_GUARANTEE_DELETE(ptr) BB_SAFE_DELETE(ptr)
#define BB_GUARANTEE_RELEASE(ptr) BB_SAFE_RELEASE(ptr)
#define BB_ASSERT_NOTNULL(expr, msg)
#define BB_ASSERT(expr, msg)
#endif

namespace igad
{

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;

const float Pi = 3.1415926535897932384626433832795f;
const float TwoPi = Pi * 2;
const float HalfPi = Pi / 2;
const float QuarterPi = Pi / 4;

inline float DegToRad(float deg) { return deg * Pi / 180.0f; }

/// Returns a random float between zero and 1
inline float RandFloat() { return static_cast<float>((rand()) / (RAND_MAX + 1.0)); }

/// Returns a random float between x and y
inline float RandInRange(float x, float y) { return x + RandFloat()*(y - x); }

/// Returns a random int between from and to
inline int RandInRange(int from, int to) { return from + rand() % (to - from); }

template <class T>
inline T Modulo(T x, T m) { return (x % m + m) % m; }

template <class T>
T Lerp(T a, T b, float t)
{
	if (t < 0)
		t = 0;
	else if (t > 1)
		t = 1;
	//
	return a * (1.0f - t) + b * t;
}


#ifdef DEBUG

#define LOG(macro_format, ...)										\
{																	\
	printf(macro_format, ##__VA_ARGS__);							\
    const size_t macro_len = strlen(macro_format);					\
    if(macro_len > 0 && macro_format[macro_len - 1] != '\n')		\
	{																\
        printf("\n");												\
    }																\
}

#else
#define LOG(...) { }
#endif

}