#' Get and set package-wide tolerance
#'
#' @param x `[double(1)]` - Tolerance to be used across all
#' cppdoubles functions.
#' @name tolerance
#'
#' @returns
#' Either sets or gets the tolerance to be used package-wide.
#'
#' @rdname tolerance
#' @export
get_tolerance <- function(){
  getOption("cppdoubles.tolerance", .default_tol)
}
#' @rdname tolerance
#' @export
set_tolerance <- function(x){
  if (!is.null(x) && !(is.double(x) && length(x) == 1)){
    stop("`x` must be a double vector of length 1 representing the tolerance")
  }
  options(cppdoubles.tolerance = x)
}

.default_tol <- sqrt(.Machine$double.eps)
