#' Absolute and relative difference
#'
#' @description
#' Calculate absolute differences with `abs_diff()` and
#'  relative differences with `rel_diff()`
#'
#' @param x A [double] vector.
#' @param y A [double] vector.
#'
#' @returns
#' A numeric vector.
#'
#' @rdname diff
#' @export
rel_diff <- function(x, y){
  .Call(`_cppdoubles_cpp_double_rel_diff_vectorised`, as.double(x), as.double(y))
}
#' @rdname diff
#' @export
abs_diff <- function(x, y){
  abs(x - y)
}
