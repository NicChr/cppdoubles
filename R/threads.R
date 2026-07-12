#' Get and set threads for parallel computation
#'
#' @param n `[integer(1)]` - Number of threads to use package wide.
#'
#' @returns
#' Gets or set the number of specified threads.
#'
#' @examples
#'
#' library(cppdoubles)
#'
#' get_threads() # The default = max_threads / 4
#'
#' set_threads(1)
#' get_threads() # Should now be 1
#'
#' @rdname threads
#' @export
get_threads <- function(){
  get_cppdoubles_threads()
}
#' @rdname threads
#' @export
set_threads <- function(n){
  set_cppdoubles_threads(n)
}
