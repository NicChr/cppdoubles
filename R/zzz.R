.onAttach <- function(...) {
  options(cppdoubles.tolerance = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps)))
}
