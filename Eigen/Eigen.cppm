module;

#include <assert.h>
#include <limits.h>
#include <stddef.h>

#define EIGEN_USE_MODULE
#include "src/Core/util/Macros.h"

#if EIGEN_COMP_MSVC && (EIGEN_ARCH_i386_OR_x86_64 || EIGEN_ARCH_ARM64) && !EIGEN_OS_WINCE
#include <intrin.h>
#endif

// The following (except #include <malloc.h> and _M_IX86_FP ??) can likely be
// removed as gcc 4.1 and msvc 2008 are not supported anyways.
#if EIGEN_COMP_MSVC
#include <malloc.h>  // for _aligned_malloc -- need it regardless of whether vectorization is enabled
// a user reported that in 64-bit mode, MSVC doesn't care to define _M_IX86_FP.
#endif

// This extern "C" works around a MINGW-w64 compilation issue
// https://sourceforge.net/tracker/index.php?func=detail&aid=3018394&group_id=202880&atid=983354
// In essence, intrin.h is included by windows.h and also declares intrinsics (just as emmintrin.h etc. below do).
// However, intrin.h uses an extern "C" declaration, and g++ thus complains of duplicate declarations
// with conflicting linkage.  The linkage for intrinsics doesn't matter, but at that stage the compiler doesn't know;
// so, to avoid compile errors when windows.h is included after Eigen/Core, ensure intrinsics are extern "C" here too.
// notice that since these are C headers, the extern "C" is theoretically needed anyways.
extern "C" {
    // In theory we should only include immintrin.h and not the other *mmintrin.h header files directly.
    // Doing so triggers some issues with ICC. However old gcc versions seems to not have this file, thus:
    #if EIGEN_COMP_ICC >= 1110 || EIGEN_COMP_EMSCRIPTEN
    #include <immintrin.h>
    #else
    #include <mmintrin.h>
    #include <emmintrin.h>
    #include <xmmintrin.h>
    #ifdef EIGEN_VECTORIZE_SSE3
    #include <pmmintrin.h>
    #endif
    #ifdef EIGEN_VECTORIZE_SSSE3
    #include <tmmintrin.h>
    #endif
    #ifdef EIGEN_VECTORIZE_SSE4_1
    #include <smmintrin.h>
    #endif
    #ifdef EIGEN_VECTORIZE_SSE4_2
    #include <nmmintrin.h>
    #endif
    #if defined(EIGEN_VECTORIZE_AVX) || defined(EIGEN_VECTORIZE_AVX512)
    #include <immintrin.h>
    #endif
    #endif
}  // end extern "C"

export module Eigen;

import std;
import std.compat;

export extern "C++" {
    #include "Eigen"
}
