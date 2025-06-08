#' Are all values of x nearly equal (within a tolerance) to all values of y?
#'
#' @description
#' A memory-efficient alternative to `all.equal.numeric()`.
#'
#' @details
#' `all_equal` compares each pair of
#' double-precision floating point numbers
#' in the same way as `double_equal`.
#' If any numbers differ, the algorithm breaks immediately,
#' which can offer significant speed when there are differences at
#' the start of a vector.
#' All arguments are recycled except `na.rm`.
#'
#' @returns
#' A logical vector of length 1.
#'
#' The result should match `all(double_equal(x, y))`, including the way
#' `NA` values are handled.
#'
#' @param x A [double] vector.
#' @param y A [double] vector.
#' @param tol A [double] vector of tolerances.
#' @param na.rm Should `NA` values be ignored? Default is `FALSE`.
#'
#' @examples
#' library(cppdoubles)
#' library(bench)
#' x <- seq(0, 1, 0.2)
#' y <- sqrt(x)^2
#'
#' all_equal(x, y)
#'
#' # Comparison to all.equal
#' z <- runif(10^4, 1, 100)
#' ones <- rep(1, length(z))
#' mark(base = isTRUE(all.equal(z, z)),
#'             cppdoubles = all_equal(z, z),
#'             iterations = 100)
#' mark(base = isTRUE(all.equal(z, ones)),
#'             cppdoubles = all_equal(z, ones),
#'             iterations = 100)
#'
#' @export
all_equal <- function(x, y, tol = get_tolerance(), na.rm = FALSE){
  .Call(`_cppdoubles_cpp_double_all_equal`, as.double(x), as.double(y), as.double(tol), as.logical(na.rm))
}
