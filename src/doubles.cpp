#include <cpp11.hpp>
using namespace cpp11;

// Author: Nick Christofides
// Date: 06-June-2025
// License: MIT License

// Below are a complete set of C++ functions for comparing doubles
// mimicking the ==, <=, <, > and >= operators with a tolerance

// Relative differences are used except when either x or y is very close to zero
// in which case absolute differences are used

#ifndef DOUBLE_EPS
#define DOUBLE_EPS std::numeric_limits<double>::epsilon()
#endif

#ifndef DEFAULT_TOL
#define DEFAULT_TOL std::sqrt(std::numeric_limits<double>::epsilon())
#endif

inline bool is_na(double x){
  return x != x;
}

inline bool is_inf(double x){
  return std::fabs(x) == R_PosInf;
}

inline bool both_same_inf(double x, double y){
  return (x == R_PosInf && y == R_PosInf) || (x == R_NegInf && y == R_NegInf);
}
inline bool any_inf(double x, double y){
  return is_inf(x) || is_inf(y);
}
inline double abs_diff(double x, double y){
  return std::fabs(x - y);
}

inline bool close_to_zero(double x, double tol){
  return std::fabs(x) < tol;
}

double rel_diff(double x, double y, double scale){
  double ax = std::fabs(x);
  double ay = std::fabs(y);
  scale = is_na(scale) ? std::fmax(ax, ay) : scale;

  if (close_to_zero(ax, DEFAULT_TOL) && close_to_zero(ay, DEFAULT_TOL)){
    return 0.0;
  } else {
    return abs_diff(x / scale, y / scale);
  }
}

// Testing equality

bool equal(double x, double y, double tol){
  double ax = std::fabs(x);
  double ay = std::fabs(y);
  double adiff = abs_diff(x, y);

  // If any are close to zero use absolute diff, otherwise relative diff

  return ( ax < tol ) || ( ay < tol ) ?
  adiff < tol :
    both_same_inf(x, y) ||
      ( (adiff / std::fmax(ax, ay)) < tol); // Relative diff
}

// Testing >, >=, < and <=
bool gt(double x, double y, double tol){
  double diff = (x - y);
  bool any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if (any_zeros || any_inf(x, y)){
    return diff > tol;
  } else {
    return (diff / std::fmax(std::fabs(x), std::fabs(y))) > tol;
  }
}
bool lt(double x, double y, double tol){
  double diff = (x - y);
  bool any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if (any_zeros || any_inf(x, y)){
    return diff < -tol;
  } else {
    return (diff / std::fmax(std::fabs(x), std::fabs(y))) < -tol;
  }
}
bool gte(double x, double y, double tol){
  return gt(x, y, tol) || equal(x, y, tol);
}
bool lte(double x, double y, double tol){
  return lt(x, y, tol) || equal(x, y, tol);
}

#define CPPDOUBLES_VECTORISED_COMPARISON(FN)                                      \
R_xlen_t xn = x.size();                                                           \
R_xlen_t yn = y.size();                                                           \
R_xlen_t tn = tolerance.size();                                                   \
R_xlen_t n = std::max(std::max(xn, yn), tn);                                      \
if (xn <= 0 || yn <= 0 || tn <= 0){                                               \
  n = 0;                                                                          \
}                                                                                 \
const double *p_x = REAL_RO(x);                                                   \
const double *p_y = REAL_RO(y);                                                   \
const double *p_t = REAL_RO(tolerance);                                           \
writable::logicals out(n);                                                        \
int* __restrict__ p_out = LOGICAL(out);                                           \
R_xlen_t i, xi, yi, ti;                                                           \
for (i = xi = yi = ti = 0; i < n;                                                 \
xi = (++xi == xn) ? 0 : xi,                                                       \
  yi = (++yi == yn) ? 0 : yi,                                                     \
  ti = (++ti == tn) ? 0 : ti, ++i){                                               \
  p_out[i] = FN(p_x[xi], p_y[yi], p_t[ti]);                                       \
  if (is_na(p_x[xi]) ||                                                           \
      is_na(p_y[yi]) ||                                                           \
      is_na(p_t[ti])){                                                            \
    p_out[i] = NA_LOGICAL;                                                        \
  }                                                                               \
}                                                                                 \
return out;                                                                       \


[[cpp11::register]]
logicals cpp_double_equal(doubles x, doubles y, doubles tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(equal)
}

[[cpp11::register]]
logicals cpp_double_gt(doubles x, doubles y, doubles tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gt)
}

[[cpp11::register]]
logicals cpp_double_gte(doubles x, doubles y, doubles tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gte)
}

[[cpp11::register]]
logicals cpp_double_lt(doubles x, doubles y, doubles tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lt)
}

[[cpp11::register]]
logicals cpp_double_lte(doubles x, doubles y, doubles tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lte)
}

[[cpp11::register]]
doubles cpp_double_rel_diff(doubles x, doubles y, doubles scale){
  R_xlen_t xn = x.size();
  R_xlen_t yn = y.size();
  R_xlen_t sn = scale.size();
  R_xlen_t n = std::max(std::max(xn, yn), sn);
  if (xn <= 0 || yn <= 0 || sn <= 0){
    n = 0;
  }
  const double *p_x = REAL_RO(x);
  const double *p_y = REAL_RO(y);
  const double *p_s = REAL_RO(scale);

  writable::doubles out(n);

  double* __restrict__ p_out = REAL(out);

  R_xlen_t i, xi, yi, si;
  for (i = xi = yi = si = 0; i < n;
  xi = (++xi == xn) ? 0 : xi,
    yi = (++yi == yn) ? 0 : yi,
    si = (++si == sn) ? 0 : si, ++i){
    p_out[i] = rel_diff(p_x[xi], p_y[yi], p_s[si]);
  }
  return out;
}

[[cpp11::register]]
logicals cpp_double_all_equal(doubles x, doubles y, doubles tolerance, logicals na_rm) {
  if (na_rm.size() != 1){
    stop("`na.rm` must be a logical vector of length 1");
  }
  bool skip_na = na_rm[0];
  bool has_na;
  R_xlen_t xn = x.size();
  R_xlen_t yn = y.size();
  R_xlen_t tn = tolerance.size();
  R_xlen_t n = std::max(std::max(xn, yn), tn);
  if (xn <= 0 || yn <= 0 || tn <= 0){
    // Avoid loop if any are length zero vectors
    n = 0;
  }
  const double *p_x = REAL_RO(x);
  const double *p_y = REAL_RO(y);
  const double *p_t = REAL_RO(tolerance);

  writable::logicals out(1);
  out[0] = true;

  R_xlen_t i, xi, yi, ti;
  for (i = xi = yi = ti = 0; i < n;
  xi = (++xi == xn) ? 0 : xi,
    yi = (++yi == yn) ? 0 : yi,
    ti = (++ti == tn) ? 0 : ti, ++i){
    has_na = is_na(p_x[xi]) ||
      is_na(p_y[yi]) ||
      is_na(p_t[ti]);
    if (has_na){
      if (skip_na){
        continue;
      } else {
        out[0] = NA_LOGICAL;
        break;
      }
    }
    if ( !equal(p_x[xi], p_y[yi], p_t[ti]) ){
      out[0] = false;
      break;
    }
  }
  return out;
}
