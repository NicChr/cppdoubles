#' Relative comparison of double-precision floating point numbers
#'
#' @description Fast and efficient methods for comparing floating point numbers
#' using relative differences.
#'
#' @param x A [double] vector.
#' @param y A [double] vector.
#' @param tol A [double] vector of tolerances.
#'
#' @returns
#' A logical vector.
#'
#' @details
#' When either `x[i]` or `y[i]` contain a number very close to zero,
#' absolute differences are used, otherwise relative differences are used.
#'
#' The output of `double_equal()` is commutative,
#' which means the order of arguments don't matter
#' whereas this is not the case for `all.equal.numeric()`.
#'
#' The calculation is done in C++ and is quite efficient.
#' Recycling follows the usual R rules and is done without allocating
#' additional memory.
#' @examples
#' library(cppdoubles)
#'
#' ### Basic usage ###
#'
#' # Standard equality operator
#' sqrt(2)^2 == 2
#'
#' # approximate equality operator
#' sqrt(2)^2 %~==% 2
#'
#' sqrt(2)^2 %~>=% 2
#' sqrt(2)^2 %~<=% 2
#' sqrt(2)^2 %~>% 2
#' sqrt(2)^2 %~<% 2
#'
#' # Alternatively
#' double_equal(2, sqrt(2)^2)
#' double_gte(2, sqrt(2)^2)
#' double_lte(2, sqrt(2)^2)
#' double_gt(2, sqrt(2)^2)
#' double_lt(2, sqrt(2)^2)
#'
#' rel_diff(1, 1 + 2e-10)
#' double_equal(1, 1 + 2e-10, tol = sqrt(.Machine$double.eps))
#' double_equal(1, 1 + 2e-10, tol = 1e-10)
#'
#' # Optionally set a threshold for all comparison
#' options(cppdoubles.tolerance = 1e-10)
#' double_equal(1, 1 + 2e-10)
#'
#' # Floating point errors magnified example
#'
#' x1 <- 1.1 * 100 * 10^200
#' x2 <- 110 * 10^200
#'
#' abs_diff(x1, x2) # Large absolute difference
#' rel_diff(x1, x2) # Very small relative difference as expected
#'
#' double_equal(x1, x2)
#'
#' # all.equal is not commutative but double_equal is
#' all.equal(10^-8, 2 * 10^-8)
#' all.equal(2 * 10^-8, 10^-8)
#'
#' double_equal(10^-8, 2 * 10^-8)
#' double_equal(2 * 10^-8, 10^-8)
#'
#' # All comparisons are vectorised and recycled
#'
#' double_equal(sqrt(1:10),
#'              sqrt(1:5),
#'              tol = c(-Inf, 1e-10, Inf))
#'
#' # One can check for whole numbers like so
#' whole_number <- function(x, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
#'   double_equal(x, round(x))
#' }
#' whole_number(seq(-5, 5, 0.25))
#' @rdname doubles
#' @export
`%~==%` <- function(x, y){
  if (is.integer(x) && is.integer(y)){
    x == y
  } else {
    cpp_double_equal_vectorised(as.double(x), as.double(y),
                                as.double(getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))))
  }
}
#' @rdname doubles
#' @export
`%~>=%` <- function(x, y){
  if (is.integer(x) && is.integer(y)){
    x >= y
  } else {
    cpp_double_gte_vectorised(as.double(x), as.double(y),
                              as.double(getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))))
  }
}
#' @rdname doubles
#' @export
`%~>%` <- function(x, y){
  if (is.integer(x) && is.integer(y)){
    x > y
  } else {
    cpp_double_gt_vectorised(as.double(x), as.double(y),
                             as.double(getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))))
  }
}
#' @rdname doubles
#' @export
`%~<=%` <- function(x, y){
  if (is.integer(x) && is.integer(y)){
    x <= y
  } else {
    cpp_double_lte_vectorised(as.double(x), as.double(y),
                              as.double(getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))))
  }
}
#' @rdname doubles
#' @export
`%~<%` <- function(x, y){
  if (is.integer(x) && is.integer(y)){
    x < y
  } else {
    cpp_double_lt_vectorised(as.double(x), as.double(y),
                             as.double(getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))))
  }
}
#' @rdname doubles
#' @export
double_equal <- function(x, y, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  if (is.integer(x) && is.integer(y)){
    x == y
  } else {
    cpp_double_equal_vectorised(as.double(x), as.double(y), as.double(tol))
  }
}
#' @rdname doubles
#' @export
double_gte <- function(x, y, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  if (is.integer(x) && is.integer(y)){
    x >= y
  } else {
    cpp_double_gte_vectorised(as.double(x), as.double(y), as.double(tol))
  }
}
#' @rdname doubles
#' @export
double_gt <- function(x, y, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  if (is.integer(x) && is.integer(y)){
    x > y
  } else {
    cpp_double_gt_vectorised(as.double(x), as.double(y), as.double(tol))
  }
}
#' @rdname doubles
#' @export
double_lte <- function(x, y, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  if (is.integer(x) && is.integer(y)){
    x <= y
  } else {
    cpp_double_lte_vectorised(as.double(x), as.double(y), as.double(tol))
  }
}
#' @rdname doubles
#' @export
double_lt <- function(x, y, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  if (is.integer(x) && is.integer(y)){
    x < y
  } else {
    cpp_double_lt_vectorised(as.double(x), as.double(y), as.double(tol))
  }
}

