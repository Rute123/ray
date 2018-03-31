#include "test_common.h"

#include <iostream>

#define NS ref
#include "../internal/simd/simd_vec.h"

void test_simd_ref() {
#include "test_simd.ipp"
}
#undef NS

#define NS sse
#define USE_SSE
#include "../internal/simd/simd_vec.h"

void test_simd_sse() {
#include "test_simd.ipp"
}
#undef USE_SSE
#undef NS

#define NS avx
#define USE_AVX
#include "../internal/simd/simd_vec.h"

void test_simd_avx() {
#include "test_simd.ipp"
}
#undef USE_AVX
#undef NS

void test_simd() {
    test_simd_ref();
    test_simd_sse();
    test_simd_avx();
}