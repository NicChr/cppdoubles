#' Absolute and relative difference
#'
#' @description
#' Calculate absolute differences with `abs_diff()` and
#'  relative differences with `rel_diff()`
#'
#' @param x A [double] vector.
#' @param y A [double] vector.
#' @param scale A [double] vector.
#' When `NA`, the scale is calculated as `max(abs(x), abs(y))`.
#'
#' @details
#'
#' ## Relative difference
#'
#' The relative difference in this package is calculated as
#' `abs_diff(x / scale, y / scale)` except in the case that both
#' `x` and `y` are approximately 0 which results in 1.
#'
#' The scale is calculated as `max(abs(x), abs(y))` by default when
#' scale is `NA`.
#' This has the nice property of making `rel_diff()` a commutative function
#' in which the order of the arguments doesn't matter. You can of course
#' supply your own scale.
#'
#' ## Comparison with `all.equal()`
#'
#' As mentioned above, unlike `base::all.equal()`, `rel_diff()` is commutative.
#' To match the relative difference calculation used by `all.equal()`,
#' simply set `scale = x`.
#'
#' Therefore, to make a vectorised binary version of `all.equal()`,
#' we can write for example the following:
#'
#' \preformatted{
#'   all.equal2 <- \(x, y, tol = get_tolerance()) rel_diff(x, y, scale = x) < tol
#' }
#' @returns
#' A numeric vector.
#'
#' @rdname diff
#' @export
rel_diff <- function(x, y, scale = NA_real_){
  .Call(`_cppdoubles_cpp_double_rel_diff`, as.double(x), as.double(y), as.double(scale))
}
#' @rdname diff
#' @export
abs_diff <- function(x, y){
  abs(x - y)
}
