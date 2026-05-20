#include <cppally.hpp>
using namespace cppally;

// Author: Nick Christofides
// Date: 19-May-2026
// License: MIT License

// Below are a complete set of C++ functions for comparing doubles
// mimicking the ==, <=, <, > and >= operators with a tolerance

// Relative differences are used except when either x or y is very close to zero
// in which case absolute differences are used

// Start off with half the available threads
static int cppdoubles_threads = std::max(1, static_cast<int>(cppally::max_threads() / 2));

// Set threads for use via R - needed for unit testing as CRAN only allows a maximum of 2 threads
[[cppally::register]]
void set_cppdoubles_threads(int n){
  cppdoubles_threads = std::max(1, std::min(cppally::max_threads(), n));
}

int get_cppdoubles_threads(){
  return cppdoubles_threads;
} 

constexpr double default_tol(){
  return unwrap(r_limits<r_dbl>::tolerance());
}

bool is_inf(double x) noexcept {
  return std::abs(x) == unwrap(pos_inf);
}

bool both_same_inf(double x, double y) noexcept {
  return (x == unwrap(pos_inf) && y == unwrap(pos_inf)) || (x == unwrap(neg_inf) && y == unwrap(neg_inf));
}
bool any_inf(double x, double y) noexcept {
  return is_inf(x) || is_inf(y);
}

bool close_to_zero(double x, double tol) noexcept {
  return std::abs(x) <= tol;
}

r_dbl rel_diff(r_dbl x, r_dbl y, r_dbl scale) noexcept {

  if (is_na(x) || is_na(y)){
    return na<r_dbl>();
  }

  double x_ = x;
  double y_ = y;
  double tol = default_tol();
  double a = std::abs(x_);
  double b = std::abs(y_);

  if (is_na(scale)){
    scale = r_dbl(std::max(a, b));
  }

  double c = scale;

  if (a < tol && b < tol){
    return r_dbl(0.0);
  }

  if ( c == 0.0 ){
    return r_dbl(1.0);
  }

  return r_dbl(std::abs((x_ / c) - (y_ / c)));

}

// Testing equality

r_lgl equal(r_dbl x, r_dbl y, r_dbl tol) noexcept {

  // Check exact equality first
  r_lgl eq = x == y;
  if (eq.is_true()){
    return eq;
  }

  if (is_na(eq) || is_na(tol)){
    return r_na;
  }

  double x_ = x;
  double y_ = y;
  double tol_ = tol;

  double ax = std::abs(x_);
  double ay = std::abs(y_);
  double adiff = std::abs(x_ - y_);

  // If any are close to zero use absolute diff, otherwise relative diff
  if ((ax <= tol_) || (ay <= tol_) ||
    ax == unwrap(pos_inf) ||
    ay == unwrap(pos_inf)){
      return r_lgl(adiff <= tol_);
    } else {
      return r_lgl((adiff / std::max(ax, ay)) <= tol_);
    }
}

// Testing >, >=, < and <=
r_lgl gt(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  if (is_na(x) || is_na(y) || is_na(tol)){
    return r_na;
  }
  double x_ = x;
  double y_ = y;
  double tol_ = tol;
  double diff = x_ - y_;
  bool any_zeros = close_to_zero(x, tol_) || close_to_zero(y, tol_);
  if ( any_zeros || any_inf(x_, y_) ){
    if (both_same_inf(x_, y_)){
      return r_false;
    } else {
      return r_lgl(diff > tol_);
    }
  } else {
    return r_lgl((diff / std::max(std::abs(x_), std::abs(y_))) > tol_);
  }
}
r_lgl lt(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  if (is_na(x) || is_na(y) || is_na(tol)){
    return r_na;
  }
  double x_ = x;
  double y_ = y;
  double tol_ = tol;
  double diff = x_ - y_;
  bool any_zeros = close_to_zero(x, tol_) || close_to_zero(y, tol_);
  if ( any_zeros || any_inf(x_, y_) ){
    if (both_same_inf(x_, y_)){
      return r_false;
    } else {
      return r_lgl(diff < -tol_);
    }
  } else {
    return r_lgl((diff / std::max(std::abs(x_), std::abs(y_))) < -tol_);
  }
}
r_lgl gte(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  return equal(x, y, tol) || gt(x, y, tol);
}
r_lgl lte(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  return equal(x, y, tol) || lt(x, y, tol);
}

#define CPPDOUBLES_VECTORISED_COMPARISON(FN)                                                                              \
r_size_t xn = x.length();                                                                                                 \
r_size_t yn = y.length();                                                                                                 \
r_size_t tn = tolerance.length();                                                                                         \
r_size_t n = std::max(std::max(xn, yn), tn);                                                                              \
if (xn <= 0 || yn <= 0 || tn <= 0){                                                                                       \
  n = 0;                                                                                                                  \
}                                                                                                                         \
r_vec<r_lgl> out(n);                                                                                                      \
if (n >= 100000 && xn == yn && tn == 1){                                                                                  \
  r_dbl tol = tolerance.get(0);                                                                                           \
  OMP_PARALLEL_FOR_SIMD(get_cppdoubles_threads())                                                                         \
    for (r_size_t i = 0; i < n; ++i){                                                                                     \
      out.set(i, FN(x.get(i), y.get(i), tol));                                                                            \
    }                                                                                                                     \
} else {                                                                                                                  \
  r_size_t i, xi, yi, ti;                                                                                                 \
  for (i = xi = yi = ti = 0; i < n;                                                                                       \
  recycle_index(xi, xn),                                                                                                  \
  recycle_index(yi, yn),                                                                                                  \
  recycle_index(ti, tn),                                                                                                  \
  ++i){                                                                                                                   \
    out.set(i, FN(x.get(xi), y.get(yi), tolerance.get(ti)));                                                              \
  }                                                                                                                       \
}                                                                                                                         \
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
r_vec<r_dbl> cpp_double_rel_diff(r_vec<r_dbl> x, r_vec<r_dbl> y, r_vec<r_dbl> scale) {
  r_size_t xn = x.length();
  r_size_t yn = y.length();
  r_size_t sn = scale.length();
  r_size_t n = std::max(std::max(xn, yn), sn);
  if (xn <= 0 || yn <= 0 || sn <= 0){
    n = 0;
  }

  r_vec<r_dbl> out(n);

  if (n >= 100000 && xn == yn && sn == 1){
    r_dbl sc = scale.get(0);
    OMP_PARALLEL_FOR_SIMD(get_cppdoubles_threads())
    for (r_size_t i = 0; i < n; ++i){
      out.set(i, rel_diff(x.get(i), y.get(i), sc));
    }
  } else {
    r_size_t i, xi, yi, si;
    for (i = xi = yi = si = 0; i < n;
      recycle_index(xi, xn),
      recycle_index(yi, yn),
      recycle_index(si, sn),
      ++i){
      out.set(i, rel_diff(x.get(xi), y.get(yi), scale.get(si)));
    }
  }
  return out;
}

[[cppally::register]]
r_vec<r_dbl> cpp_double_abs_diff(r_vec<r_dbl> x, r_vec<r_dbl> y) {
  r_size_t xn = x.length();
  r_size_t yn = y.length();
  r_size_t n = std::max(xn, yn);
  if (xn <= 0 || yn <= 0){
    n = 0;
  }

  r_vec<r_dbl> out(n);

  if (n >= 100000 && xn == yn){
    OMP_PARALLEL_FOR_SIMD(get_cppdoubles_threads())
    for (r_size_t i = 0; i < n; ++i){
      out.set(i, abs(x.get(i) - y.get(i)));
    }
  } else {
    for (r_size_t i = 0, xi = 0, yi = 0; i < n;
      recycle_index(xi, xn),
      recycle_index(yi, yn),
      ++i){
      out.set(i, abs(x.get(xi) - y.get(yi)));
    }
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
