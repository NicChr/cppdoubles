
<!-- badges: start -->

[![R-CMD-check](https://github.com/NicChr/cppdoubles/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/NicChr/cppdoubles/actions/workflows/R-CMD-check.yaml)
[![CRAN
status](https://www.r-pkg.org/badges/version/cppdoubles)](https://CRAN.R-project.org/package=cppdoubles)
[![Codecov test
coverage](https://codecov.io/gh/NicChr/cppdoubles/graph/badge.svg)](https://app.codecov.io/gh/NicChr/cppdoubles)
<!-- badges: end -->

# cppdoubles

# **Fast Relative Comparisons of Floating Point Numbers in C++**

You can install `cppdoubles` using the below code.

``` r
remotes::install_github("NicChr/cppdoubles")
```

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
  double_equal(x, round(x), tol = tol)
}
x <- seq(-5, 5, by = 0.2)
whole_nums <- x[whole_number(x)]
whole_nums
#>  [1] -5 -4 -3 -2 -1  0  1  2  3  4  5
```

`all_equal` as an alternative to base R `all.equal.numeric`

``` r
x <- seq(0, 10, 2)
y <- sqrt(x)^2

all_equal(x, y)
#> [1] TRUE
all_equal(x, 1)
#> [1] FALSE
all_equal(x, NA)
#> [1] NA
isTRUE(all_equal(x, NA))
#> [1] FALSE
```

Benchmark against `all.equal.numeric`

``` r
library(bench)
x <- abs(rnorm(10^7))
y <- sqrt(x)^2
z <- x^2

# 2 approximately equal vectors
mean(rel_diff(x, y))
#> [1] 7.760502e-17
mark(base = isTRUE(all.equal(x, y)),
     cppdoubles = all_equal(x, y))
#> # A tibble: 2 × 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 base          367ms    380ms      2.63     437MB     9.22
#> 2 cppdoubles    156ms    157ms      6.38        0B     0

# 2 significantly different vectors
mean(rel_diff(x, z))
#> [1] 0.4627624
mark(base = isTRUE(all.equal(x, z)),
     cppdoubles = all_equal(x, z))
#> # A tibble: 2 × 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 base        261.7ms  273.2ms      3.66     343MB     7.32
#> 2 cppdoubles    2.3µs    2.5µs 361935.          0B     0
```

Benchmark against using absolute differences

``` r
mark(double_equal(x, y),
     abs_diff(x, y) < sqrt(.Machine$double.eps))
#> # A tibble: 2 × 6
#>   expression                            min  median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                        <bch:t> <bch:t>     <dbl> <bch:byt>    <dbl>
#> 1 double_equal(x, y)                170.7ms 174.1ms      5.76    38.1MB      0  
#> 2 abs_diff(x, y) < sqrt(.Machine$d…  68.2ms  68.9ms     14.5    114.4MB     29.0
```
