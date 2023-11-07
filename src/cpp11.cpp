// Generated by cpp11: do not edit by hand
// clang-format off


#include "cpp11/declarations.hpp"
#include <R_ext/Visibility.h>

// cpp_doubles.cpp
SEXP cpp_double_equal_vectorised(SEXP x, SEXP y, SEXP tolerance);
extern "C" SEXP _cppdoubles_cpp_double_equal_vectorised(SEXP x, SEXP y, SEXP tolerance) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_double_equal_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y), cpp11::as_cpp<cpp11::decay_t<SEXP>>(tolerance)));
  END_CPP11
}
// cpp_doubles.cpp
SEXP cpp_double_gt_vectorised(SEXP x, SEXP y, SEXP tolerance);
extern "C" SEXP _cppdoubles_cpp_double_gt_vectorised(SEXP x, SEXP y, SEXP tolerance) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_double_gt_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y), cpp11::as_cpp<cpp11::decay_t<SEXP>>(tolerance)));
  END_CPP11
}
// cpp_doubles.cpp
SEXP cpp_double_gte_vectorised(SEXP x, SEXP y, SEXP tolerance);
extern "C" SEXP _cppdoubles_cpp_double_gte_vectorised(SEXP x, SEXP y, SEXP tolerance) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_double_gte_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y), cpp11::as_cpp<cpp11::decay_t<SEXP>>(tolerance)));
  END_CPP11
}
// cpp_doubles.cpp
SEXP cpp_double_lt_vectorised(SEXP x, SEXP y, SEXP tolerance);
extern "C" SEXP _cppdoubles_cpp_double_lt_vectorised(SEXP x, SEXP y, SEXP tolerance) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_double_lt_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y), cpp11::as_cpp<cpp11::decay_t<SEXP>>(tolerance)));
  END_CPP11
}
// cpp_doubles.cpp
SEXP cpp_double_lte_vectorised(SEXP x, SEXP y, SEXP tolerance);
extern "C" SEXP _cppdoubles_cpp_double_lte_vectorised(SEXP x, SEXP y, SEXP tolerance) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_double_lte_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y), cpp11::as_cpp<cpp11::decay_t<SEXP>>(tolerance)));
  END_CPP11
}
// cpp_doubles.cpp
SEXP cpp_rel_diff_vectorised(SEXP x, SEXP y);
extern "C" SEXP _cppdoubles_cpp_rel_diff_vectorised(SEXP x, SEXP y) {
  BEGIN_CPP11
    return cpp11::as_sexp(cpp_rel_diff_vectorised(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(y)));
  END_CPP11
}

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    {"_cppdoubles_cpp_double_equal_vectorised", (DL_FUNC) &_cppdoubles_cpp_double_equal_vectorised, 3},
    {"_cppdoubles_cpp_double_gt_vectorised",    (DL_FUNC) &_cppdoubles_cpp_double_gt_vectorised,    3},
    {"_cppdoubles_cpp_double_gte_vectorised",   (DL_FUNC) &_cppdoubles_cpp_double_gte_vectorised,   3},
    {"_cppdoubles_cpp_double_lt_vectorised",    (DL_FUNC) &_cppdoubles_cpp_double_lt_vectorised,    3},
    {"_cppdoubles_cpp_double_lte_vectorised",   (DL_FUNC) &_cppdoubles_cpp_double_lte_vectorised,   3},
    {"_cppdoubles_cpp_rel_diff_vectorised",     (DL_FUNC) &_cppdoubles_cpp_rel_diff_vectorised,     2},
    {NULL, NULL, 0}
};
}

extern "C" attribute_visible void R_init_cppdoubles(DllInfo* dll){
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
