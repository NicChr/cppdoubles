
<!-- badges: start -->

[![R-CMD-check](https://github.com/NicChr/cppdoubles/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/NicChr/cppdoubles/actions/workflows/R-CMD-check.yaml)
[![Codecov test
coverage](https://codecov.io/gh/NicChr/cppdoubles/branch/master/graph/badge.svg)](https://app.codecov.io/gh/NicChr/cppdoubles?branch=master)
<!-- badges: end -->

# cppdoubles

# **Fast Relative Comparisons of Floating Point Numbers in C++**

Comparing equality of 2 double vectors

``` r
library(cppdoubles)

### Basic usage ###

# Standard equality operator
sqrt(2)^2 == 2
#> [1] FALSE

# approximate equality operator
sqrt(2)^2 %~==% 2
#> [1] TRUE
```

Other approximate equality operators

``` r
sqrt(2)^2 %~>=% 2
#> [1] TRUE
sqrt(2)^2 %~<=% 2
#> [1] TRUE
sqrt(2)^2 %~>% 2
#> [1] FALSE
sqrt(2)^2 %~<% 2
#> [1] FALSE

# Alternatively
double_equal(2, sqrt(2)^2)
#> [1] TRUE
double_gte(2, sqrt(2)^2)
#> [1] TRUE
double_lte(2, sqrt(2)^2)
#> [1] TRUE
double_gt(2, sqrt(2)^2)
#> [1] FALSE
double_lt(2, sqrt(2)^2)
#> [1] FALSE
```

All comparisons are vectorised and recycled

``` r
double_equal(sqrt(1:10),
             sqrt(1:5),
             tol = c(-Inf, 1e-10, Inf))
#>  [1] FALSE  TRUE  TRUE FALSE  TRUE  TRUE FALSE FALSE  TRUE FALSE
```

One can check if a double is a whole number like so

``` r
# One can check for whole numbers like so
whole_number <- function(x, tol = getOption("cppdoubles.tolerance", sqrt(.Machine$double.eps))){
  double_equal(x, round(x))
}
x <- seq(-5, 5, by = 0.2)
whole_nums <- x[whole_number(x)]
whole_nums
#>  [1] -5 -4 -3 -2 -1  0  1  2  3  4  5
```

# Benchmark against using syntactically simpler absolute differences

``` r
library(bench)
x <- abs(rnorm(10^7))
y <- sqrt(x)^2

mark(double_equal(x, y),
     abs_diff(x, y) < sqrt(.Machine$double.eps))
#> # A tibble: 2 × 6
#>   expression                            min  median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                        <bch:t> <bch:t>     <dbl> <bch:byt>    <dbl>
#> 1 double_equal(x, y)                390.1ms 390.1ms      2.56    38.1MB     2.56
#> 2 abs_diff(x, y) < sqrt(.Machine$d…  94.7ms  95.9ms     10.4    114.4MB    15.6
```
