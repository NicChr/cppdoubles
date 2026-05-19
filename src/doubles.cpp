#include <cppally.hpp>
using namespace cppally;

// Author: Nick Christofides
// Date: 19-May-2026
// License: MIT License

// Below are a complete set of C++ functions for comparing doubles
// mimicking the ==, <=, <, > and >= operators with a tolerance

// Relative differences are used except when either x or y is very close to zero
// in which case absolute differences are used

constexpr r_dbl default_tol(){
  return r_limits<r_dbl>::tolerance();
}

r_lgl is_inf(r_dbl x){
  return abs(x) == pos_inf;
}

r_lgl both_same_inf(r_dbl x, r_dbl y){
  return (x == pos_inf && y == pos_inf) || (x == neg_inf && y == neg_inf);
}
r_lgl any_inf(r_dbl x, r_dbl y){
  return is_inf(x) || is_inf(y);
}

r_lgl close_to_zero(r_dbl x, r_dbl tol){
  return abs(x) <= tol;
}

r_dbl rel_diff(r_dbl x, r_dbl y, r_dbl scale){
  r_dbl ax = abs(x);
  r_dbl ay = abs(y);
  scale = is_na(scale) ? max(ax, ay) : scale;

  if ( (close_to_zero(ax, default_tol()) && close_to_zero(ay, default_tol())).is_true() ) {
    return r_dbl(0.0);
  } else {
    return abs_diff(x / scale, y / scale);
  }
}

// Testing equality

r_lgl equal(r_dbl x, r_dbl y, r_dbl tol){
  r_dbl ax = abs(x);
  r_dbl ay = abs(y);
  r_dbl adiff = abs_diff(x, y);

  // If any are close to zero use absolute diff, otherwise relative diff
  if ( ((ax <= tol) || (ay <= tol) || any_inf(x, y)).is_true() ) {
    if (both_same_inf(x, y).is_true()){
      return r_true;
   } else {
    return adiff <= tol;
   }
  } else {
    return (adiff / max(ax, ay)) <= tol;
  }
}

// Testing >, >=, < and <=
r_lgl gt(r_dbl x, r_dbl y, r_dbl tol){
  r_dbl diff = (x - y);
  r_lgl any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if ( (any_zeros || any_inf(x, y)).is_true() ){
    if (both_same_inf(x, y).is_true()) return r_false;
    return diff > tol;
  } else {
    return (diff / max(abs(x), abs(y))) > tol;
  }
}
r_lgl lt(r_dbl x, r_dbl y, r_dbl tol){
  r_dbl diff = (x - y);
  r_lgl any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if ( (any_zeros || any_inf(x, y)).is_true() ){
    if (both_same_inf(x, y).is_true()) return r_false;
    return diff < -tol;
  } else {
    return (diff / max(abs(x), abs(y))) < -tol;
  }
}
r_lgl gte(r_dbl x, r_dbl y, r_dbl tol){
  return gt(x, y, tol) || equal(x, y, tol);
}
r_lgl lte(r_dbl x, r_dbl y, r_dbl tol){
  return lt(x, y, tol) || equal(x, y, tol);
}

#define CPPDOUBLES_VECTORISED_COMPARISON(FN)                                                \
r_size_t xn = x.length();                                                                     \
r_size_t yn = y.length();                                                                     \
r_size_t tn = tolerance.length();                                                             \
r_size_t n = std::max(std::max(xn, yn), tn);                                                \
if (xn <= 0 || yn <= 0 || tn <= 0){                                                         \
  n = 0;                                                                                    \
}                                                                                           \
r_vec<r_lgl> out(n);                                                                        \
r_size_t i, xi, yi, ti;                                                                     \
for (i = xi = yi = ti = 0; i < n;                                                           \
  recycle_index(xi, xn),                                                                    \
  recycle_index(yi, yn),                                                                    \
  recycle_index(ti, tn),                                                                    \
  ++i){                                                                                     \
  out.set(i, FN(x.get(xi), y.get(yi), tolerance.get(ti)));                                  \
}                                                                                           \
return out;


[[cppally::register]]
r_vec<r_lgl> cpp_double_equal(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(equal)
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_gt(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gt)
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_gte(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(gte)
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_lt(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lt)
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_lte(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance) {
  CPPDOUBLES_VECTORISED_COMPARISON(lte)
}

[[cppally::register]]
r_vec<r_dbl> cpp_double_rel_diff(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> scale){
  r_size_t xn = x.length();
  r_size_t yn = y.length();
  r_size_t sn = scale.length();
  r_size_t n = std::max(std::max(xn, yn), sn);
  if (xn <= 0 || yn <= 0 || sn <= 0){
    n = 0;
  }

  r_vec<r_dbl> out(n);
  r_size_t i, xi, yi, si;
  for (i = xi = yi = si = 0; i < n;
    recycle_index(xi, xn),
    recycle_index(yi, yn),
    recycle_index(si, sn),
    ++i){
    out.set(i, r_dbl(rel_diff(x.get(xi), y.get(yi), scale.get(si))));
  }
  return out;
}

[[cppally::register]]
r_lgl cpp_double_all_equal(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> tolerance, r_lgl na_rm) {
  bool skip_na = na_rm.is_true();
  bool has_na;
  r_size_t xn = x.length();
  r_size_t yn = y.length();
  r_size_t tn = tolerance.length();
  r_size_t n = std::max(std::max(xn, yn), tn);
  if (xn <= 0 || yn <= 0 || tn <= 0){
    // Avoid loop if any are length zero vectors
    n = 0;
  }

  r_lgl out = r_true;

  r_size_t i, xi, yi, ti;
  for (i = xi = yi = ti = 0; i < n;
  recycle_index(xi, xn),
  recycle_index(yi, yn),
  recycle_index(ti, tn),
  ++i){
    has_na = is_na(x.get(xi)) ||
      is_na(y.get(yi)) ||
      is_na(tolerance.get(ti));
    if (has_na){
      if (skip_na){
        continue;
      } else {
        out = na<r_lgl>();
        break;
      }
    }
    if ( equal(x.get(xi), y.get(yi), tolerance.get(ti)).is_false() ){
      out = r_false;
      break;
    }
  }
  return out;
}
