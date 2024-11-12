#include <cpp11.hpp>

// Author: Nick Christofides
// Date: 12-Nov-2024
// License: MIT License

// Below are a complete set of C++ functions for comparing doubles
// mimicking the ==, <=, <, > and >= operators with a tolerance

// Relative differences are used except when either x or y is very close to zero
// in which case absolute differences are used

#define DOUBLE_IS_NA(x) ( (double) (x != x))

bool both_same_inf(double x, double y){
  return (x == R_PosInf && y == R_PosInf) || (x == R_NegInf && y == R_NegInf);
}
bool any_inf(double x, double y){
  return x == R_PosInf || y == R_PosInf || x == R_NegInf || y == R_NegInf;
}
double abs_diff(double x, double y){
  return std::fabs(x - y);
}
double rel_diff(double x, double y){
  double tol = std::sqrt(std::numeric_limits<double>::epsilon());
  double ax = std::fabs(x);
  double ay = std::fabs(y);

  if (ax <= tol && ay <= tol){
    return 0.0;
  } else {
    return abs_diff(x, y) / std::fmax(ax, ay);
  }
}
bool any_zero(double x, double y, double tol){
  return ( std::fabs(x) < tol ) || ( std::fabs(y) < tol );
}

// Testing equality

bool equal(double x, double y, double tol){
  double ax = std::fabs(x);
  double ay = std::fabs(y);
  bool any_zero = ( ax < tol ) || ( ay < tol );
  double adiff = std::fabs(x - y);
  double rdiff = adiff / std::fmax(ax, ay);
  double out;
  if (any_zero){
    out = (adiff < tol);
  } else {
    out = both_same_inf(x, y) || (rdiff < tol);
  }
  return out;
}

// Testing >, >=, < and <=
bool gt(double x, double y, double tol){
  double out;
  double diff = (x - y);
  bool any_zeros = any_zero(x, y, tol);
  if (any_zeros || any_inf(x, y)){
    out = diff > tol;
  } else {
    out = (diff / std::fmax(std::fabs(x), std::fabs(y))) > tol;
  }
  return out;
}
bool lt(double x, double y, double tol){
  double out;
  double diff = (x - y);
  bool any_zeros = any_zero(x, y, tol);
  if (any_zeros || any_inf(x, y)){
    out = diff < -tol;
  } else {
    out = (diff / std::fmax(std::fabs(x), std::fabs(y))) < -tol;
  }
  return out;
}
bool gte(double x, double y, double tol){
  return gt(x, y, tol) || equal(x, y, tol);
}
bool lte(double x, double y, double tol){
  return lt(x, y, tol) || equal(x, y, tol);
}

#define CPPDOUBLES_VECTORISED_COMPARISON(_func_)               \
R_xlen_t xn = Rf_xlength(x);                                   \
R_xlen_t yn = Rf_xlength(y);                                   \
R_xlen_t tn = Rf_xlength(tolerance);                           \
R_xlen_t n = std::max(std::max(xn, yn), tn);                   \
if (xn <= 0 || yn <= 0 || tn <= 0){                            \
  n = 0;                                                       \
}                                                              \
double *p_x = REAL(x);                                         \
double *p_y = REAL(y);                                         \
double *p_t = REAL(tolerance);                                 \
SEXP out = Rf_protect(Rf_allocVector(LGLSXP, n));              \
int *p_out = LOGICAL(out);                                     \
R_xlen_t i, xi, yi, ti;                                        \
for (i = xi = yi = ti = 0; i < n;                              \
xi = (++xi == xn) ? 0 : xi,                                    \
  yi = (++yi == yn) ? 0 : yi,                                  \
  ti = (++ti == tn) ? 0 : ti, ++i){                            \
  p_out[i] = _func_(p_x[xi], p_y[yi], p_t[ti]);                \
  if (DOUBLE_IS_NA(p_x[xi]) ||                             \
      DOUBLE_IS_NA(p_y[yi]) ||                             \
      DOUBLE_IS_NA(p_t[ti])){                              \
    p_out[i] = NA_LOGICAL;                                     \
  }                                                            \
}                                                              \
Rf_unprotect(1);                                               \
return out;                                                    \


[[cpp11::register]]
SEXP cpp_double_equal(SEXP x, SEXP y, SEXP tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(equal)
}

[[cpp11::register]]
SEXP cpp_double_gt(SEXP x, SEXP y, SEXP tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gt)
}

[[cpp11::register]]
SEXP cpp_double_gte(SEXP x, SEXP y, SEXP tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gte)
}

[[cpp11::register]]
SEXP cpp_double_lt(SEXP x, SEXP y, SEXP tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lt)
}

[[cpp11::register]]
SEXP cpp_double_lte(SEXP x, SEXP y, SEXP tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lte)
}

[[cpp11::register]]
SEXP cpp_double_rel_diff(SEXP x, SEXP y){
  R_xlen_t xn = Rf_xlength(x);
  R_xlen_t yn = Rf_xlength(y);
  R_xlen_t n = std::max(xn, yn);
  if (xn <= 0 || yn <= 0){
    // Avoid loop if any are length zero vectors
    n = 0;
  }
  double *p_x = REAL(x);
  double *p_y = REAL(y);
  SEXP out = Rf_protect(Rf_allocVector(REALSXP, n));
  double *p_out = REAL(out);
  R_xlen_t i, xi, yi;
  for (i = xi = yi = 0; i < n;
  xi = (++xi == xn) ? 0 : xi,
    yi = (++yi == yn) ? 0 : yi, ++i){
    p_out[i] = rel_diff(p_x[xi], p_y[yi]);
  }
  Rf_unprotect(1);
  return out;
}
[[cpp11::register]]
SEXP cpp_double_all_equal(SEXP x, SEXP y, SEXP tolerance, SEXP na_rm) {
  if (Rf_length(na_rm) != 1 || !Rf_isLogical(na_rm)){
    Rf_error("na.rm must be a logical vector of length 1");
  }
  bool skip_na = Rf_asLogical(na_rm);
  bool has_na;
  R_xlen_t xn = Rf_xlength(x);
  R_xlen_t yn = Rf_xlength(y);
  R_xlen_t tn = Rf_xlength(tolerance);
  R_xlen_t n = std::max(std::max(xn, yn), tn);
  if (xn <= 0 || yn <= 0 || tn <= 0){
    // Avoid loop if any are length zero vectors
    n = 0;
  }
  double *p_x = REAL(x);
  double *p_y = REAL(y);
  double *p_t = REAL(tolerance);
  SEXP out = Rf_protect(Rf_allocVector(LGLSXP, 1));
  LOGICAL(out)[0] = TRUE;
  R_xlen_t i, xi, yi, ti;
  for (i = xi = yi = ti = 0; i < n;
  xi = (++xi == xn) ? 0 : xi,
    yi = (++yi == yn) ? 0 : yi,
    ti = (++ti == tn) ? 0 : ti, ++i){
    has_na = DOUBLE_IS_NA(p_x[xi]) ||
      DOUBLE_IS_NA(p_y[yi]) ||
      DOUBLE_IS_NA(p_t[ti]);
    if (has_na){
      if (skip_na){
        continue;
      } else {
        LOGICAL(out)[0] = NA_LOGICAL;
        break;
      }
    }
    if ( !equal(p_x[xi], p_y[yi], p_t[ti]) ){
      LOGICAL(out)[0] = FALSE;
      break;
    }
  }
  Rf_unprotect(1);
  return out;
}
