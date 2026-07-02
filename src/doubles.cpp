#include <cppally.hpp>
using namespace cppally;

// Author: Nick Christofides
// Date: 19-May-2026
// License: MIT License

// Below are a complete set of C++ functions for comparing doubles
// mimicking the ==, <=, <, > and >= operators with a tolerance

// Relative differences are used except when either x or y is very close to zero
// in which case absolute differences are used

// Set threads for use via R - needed for unit testing as CRAN only allows a maximum of 2 threads
[[cppally::register]]
void set_cppdoubles_threads(int n){
  int n_threads = std::max(1, std::min(cppally::max_threads(), n));
  cppally::set_threads(n_threads);
}

// Start off with a quarter of the available threads
[[cppally::init]]
void init_threads(DllInfo* dll){
  int n_threads = std::max(1, static_cast<int>(cppally::max_threads() / 4));
  set_cppdoubles_threads(n_threads);
}

constexpr r_dbl default_tol(){
  return r_limits<r_dbl>::tolerance();
}

r_lgl is_inf(r_dbl x) noexcept {
  return abs(x) == pos_inf;
}

r_lgl both_same_inf(r_dbl x, r_dbl y) noexcept {
  return (x == pos_inf && y == pos_inf) || (x == neg_inf && y == neg_inf);
}
r_lgl any_inf(r_dbl x, r_dbl y) noexcept {
  return is_inf(x) || is_inf(y);
}

r_lgl close_to_zero(r_dbl x, r_dbl tol) noexcept {
  return abs(x) <= tol;
}

r_dbl rel_diff(r_dbl x, r_dbl y, r_dbl scale) noexcept {

  r_dbl ax = abs(x);
  r_dbl ay = abs(y);

  if (is_na(scale)){
    scale = max(ax, ay);
  }

  if ( (ax < default_tol() && ay < default_tol()).is_true()){
    return r_dbl(0.0);
  }

  if (identical(scale, r_dbl(0.0))){
    return r_dbl(1.0);
  }

  return abs((x / scale) - (y / scale));
}

// Testing equality

r_lgl equal(r_dbl x, r_dbl y, r_dbl tol) noexcept {

  // Check exact equality first
  r_lgl eq = x == y;
  if (eq.is_true()){
    return eq;
  }

  r_dbl ax = abs(x);
  r_dbl ay = abs(y);
  r_dbl adiff = abs(x - y);

  // If any are close to zero use absolute diff, otherwise relative diff
  r_lgl use_abs_diff = ax <= tol || ay <= tol || ax == pos_inf || ay == pos_inf;
  if (use_abs_diff.is_true()){
      return adiff <= tol;
    } else {
      return (adiff / max(ax, ay)) <= tol;
    }
}

// Testing >, >=, < and <=
r_lgl gt(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  r_dbl diff = x - y;
  r_lgl any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if ( (any_zeros || any_inf(x, y)).is_true() ){
    if (both_same_inf(x, y).is_true()){
      return r_false;
    } else {
      return diff > tol;
    }
  } else {
    return (diff / max(abs(x), abs(y))) > tol;
  }
}
r_lgl lt(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  r_dbl diff = x - y;
  r_lgl any_zeros = close_to_zero(x, tol) || close_to_zero(y, tol);
  if ( (any_zeros || any_inf(x, y)).is_true() ){
    if (both_same_inf(x, y).is_true()){
      return r_false;
    } else {
      return diff < -tol;
    }
  } else {
    return (diff / max(abs(x), abs(y))) < -tol;
  }
}
r_lgl gte(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  return equal(x, y, tol) || gt(x, y, tol);
}
r_lgl lte(r_dbl x, r_dbl y, r_dbl tol) noexcept {
  return equal(x, y, tol) || lt(x, y, tol);
}

template <auto FN>
auto do_vectorised(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& z) {
  if (x.length() == 1){
    r_dbl x_ = x.get(0);
    return pmap_parallel_simd([x_](auto b, auto c) noexcept {
      return FN(x_, b, c);
    }, y, z);
  } else if (y.length() == 1){
    r_dbl y_ = y.get(0);
    return pmap_parallel_simd([y_](auto a, auto c) noexcept {
      return FN(a, y_, c);
    }, x, z);
  } else if (z.length() == 1){
    r_dbl z_ = z.get(0);
    return pmap_parallel_simd([z_](auto a, auto b) noexcept {
      return FN(a, b, z_);
    }, x, y);
  } else {
    return pmap_parallel_simd([](auto a, auto b, auto c) noexcept {
      return FN(a, b, c);
    }, x, y, z);
  }
}


[[cppally::register]]
r_vec<r_lgl> cpp_double_equal(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance) {
  return do_vectorised<equal>(x, y, tolerance);
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_gt(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance) {
  return do_vectorised<gt>(x, y, tolerance);
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_gte(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance) {
  return do_vectorised<gte>(x, y, tolerance);
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_lt(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance) {
  return do_vectorised<lt>(x, y, tolerance);
}

[[cppally::register]]
r_vec<r_lgl> cpp_double_lte(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance) {
  return do_vectorised<lte>(x, y, tolerance);
}

[[cppally::register]]
r_vec<r_dbl> cpp_double_rel_diff(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& scale) {
  return do_vectorised<rel_diff>(x, y, scale);
}

[[cppally::register]]
r_vec<r_dbl> cpp_double_abs_diff(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y) {
  if (x.length() == 1){
    r_dbl x_ = x.get(0);
    return pmap_parallel_simd([x_](auto b) noexcept {
      return abs(x_ - b);
    }, y);
  } else if (y.length() == 1){
    r_dbl y_ = y.get(0);
    return pmap_parallel_simd([y_](auto a) noexcept {
      return abs(a - y_);
    }, x);
  } else {
    return pmap_parallel_simd([](auto a, auto b) noexcept {
      return abs(a - b);
    }, x, y);
  }
}

[[cppally::register]]
r_lgl cpp_double_all_equal(const r_vec<r_dbl>& x, const r_vec<r_dbl>& y, const r_vec<r_dbl>& tolerance, r_lgl na_rm) {
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
