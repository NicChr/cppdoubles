options("cppdoubles.tolerance" = sqrt(.Machine$double.eps))

test_that("Integers", {
  set.seed(1000)
  x <- sample((-10^2):(10^2), size = 10^3, replace = TRUE)
  y <- sample((-10^2):(10^2), size = 10^3, replace = TRUE)
  expect_equal(double_equal(x, y),
                         x == y)
  expect_equal(double_gte(x, y),
                         x >= y)
  expect_equal(double_lte(x, y),
                         x <= y)
  expect_equal(double_gt(x, y),
                         x > y)
  expect_equal(double_lt(x, y),
                         x < y)

  expect_equal(x %~==% y,
                         x == y)
  expect_equal(x %~>=% y,
                         x >= y)
  expect_equal(x %~<=% y,
                         x <= y)
  expect_equal(x %~>% y,
                         x > y)
  expect_equal(x %~<% y,
                         x < y)
})

test_that("Differences", {

  expect_equal(rel_diff(0, 0), 0)

  expect_equal(rel_diff(10^-8, 2 * 10^-8),
                         rel_diff(2 * 10^-8, 10^-8))

  expect_equal(abs_diff(10^-8, 2 * 10^-8),
                         abs_diff(2 * 10^-8, 10^-8))
})

test_that("Double floating point precision", {
  set.seed(1849127)

  x <- abs(rnorm(5 * (10^6)))
  y <- sqrt(x)^2
  z <- sample(x)

  expect_equal(all(double_gt(x, y) == FALSE),
                         TRUE)
  expect_equal(all(double_gte(x, y) == TRUE),
                         TRUE)
  expect_equal(all(double_lte(x, y) == TRUE),
                         TRUE)
  expect_equal(all(double_lt(x, y) == FALSE),
                         TRUE)
  expect_equal(all(double_equal(x, y) == TRUE),
                         TRUE)

  expect_equal(all.equal(x >= z, double_gte(x, z)), TRUE)
  expect_equal(all.equal(x > z, double_gt(x, z)), TRUE)
  expect_equal(all.equal(x <= z, double_lte(x, z)), TRUE)
  expect_equal(all.equal(x < z, double_lt(x, z)), TRUE)
  expect_equal(all.equal(x == z, double_equal(x, z)), TRUE)

  x <- seq(-10, 10, 0.2)
  expect_true(all(double_gte(x + 0.2, x - 0.2 + sqrt(0.2)^2 + 0.2)))
  expect_true(all(double_gt(x + 0.2, x - 0.2 + sqrt(0.2)^2 + 0.2)) == FALSE)
  expect_true(all(double_lt(x + 0.2, x - 0.2 + sqrt(0.2)^2 + 0.2)) == FALSE)
  expect_true(all(double_lte(x + 0.2, x - 0.2 + sqrt(0.2)^2 + 0.2)))

  expect_true(all(double_equal(diff(x) - 0.2, 0)))
})

test_that("more tests", {
  expect_true(double_equal(10^-8, 2 * 10^-8))
  expect_true(double_equal(2 * 10^-8, 10^-8))
  expect_false(double_equal(10^-8, 2 * 10^-8, tol = sqrt(.Machine$double.eps)/100))
  expect_false(double_equal(2 * 10^-8, 10^-8, tol = sqrt(.Machine$double.eps)/100))
  expect_true(double_equal(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(double_equal(110 * 10^200, 1.1 * 100 * 10^200))
  expect_false(double_lt(1.1 * 100 * 10^200, 110 * 10^200))
  expect_false(double_gt(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(double_lte(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(double_gte(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(double_equal(0, 0))
  expect_false(double_equal(0, sqrt(.Machine$double.eps)))
  expect_true(double_equal(0, sqrt(.Machine$double.eps)^2))
  expect_identical(
    double_equal(c(NaN, NA_real_, NaN, NaN, Inf, Inf, -Inf, -Inf, 0, 0, -3, -3, 2, 2),
                 c(1, 2, NaN, 0, Inf, -Inf, Inf, -Inf, Inf, -Inf, Inf, -Inf, Inf, -Inf)),
    c(NA, NA, NA, NA, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE)
  )
  x <- c(NaN, NA_real_, 0, 0.01, -0.001, sqrt(2)^2, -sqrt(2)^2, Inf, -Inf, 10^7, -10^7)
  combs <- expand.grid(x1 = x, x2 = x)
  expect_identical(double_equal(combs$x1, combs$x2),
                             c(NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                               NA, NA, NA, NA, NA, NA, NA, NA, NA, TRUE, FALSE, FALSE, FALSE,
                               FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE, TRUE, FALSE,
                               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE, FALSE,
                               TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE,
                               FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA,
                               FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE,
                               NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
                               FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,
                               FALSE, FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                               FALSE, TRUE, FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE,
                               FALSE, FALSE, FALSE, TRUE))
  x <- c(NaN, NA_real_, 0, 0.01, -0.001, Inf, -Inf, 10^7, -10^7)
  combs <- expand.grid(x1 = x, x2 = x)
  expect_identical(double_gt(combs$x1, combs$x2),
                             combs$x1 > combs$x2)
  expect_identical(double_gte(combs$x1, combs$x2),
                             combs$x1 >= combs$x2)
  expect_identical(double_lt(combs$x1, combs$x2),
                             combs$x1 < combs$x2)
  expect_identical(double_lte(combs$x1, combs$x2),
                             combs$x1 <= combs$x2)

  # Abs diff would work here but rel diff doesn't
  expect_true(double_equal(10^9, 10^9 + 0.002))
  expect_false(double_equal(0, -0.00001))
  expect_false(double_equal(0, 0.00001))
  expect_false(double_equal(0, 10^20))
  expect_false(double_equal(0, -10^20))
  expect_false(double_equal(0, -10^20))
  expect_true(double_equal(10^-9, 2 * 10^-9)) # Default tolerance isnt low enough
  expect_false(double_equal(10^-9, 2 * 10^-9, tol = sqrt(.Machine$double.eps)/10^4))
})

test_that("even more tests", {
  expect_true(`%~==%`(10^-8, 2 * 10^-8))
  expect_true(`%~==%`(2 * 10^-8, 10^-8))
  options(cppdoubles.tolerance = sqrt(.Machine$double.eps)/100)
  expect_false(`%~==%`(10^-8, 2 * 10^-8))
  options(cppdoubles.tolerance = sqrt(.Machine$double.eps)/100)
  expect_false(`%~==%`(2 * 10^-8, 10^-8))
  options(cppdoubles.tolerance = sqrt(.Machine$double.eps))
  expect_true(`%~==%`(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(`%~==%`(110 * 10^200, 1.1 * 100 * 10^200))
  expect_false(`%~<%`(1.1 * 100 * 10^200, 110 * 10^200))
  expect_false(`%~>%`(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(`%~<=%`(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(`%~>=%`(1.1 * 100 * 10^200, 110 * 10^200))
  expect_true(`%~==%`(0, 0))
  expect_false(`%~==%`(0, sqrt(.Machine$double.eps)))
  expect_true(`%~==%`(0, sqrt(.Machine$double.eps)^2))
  expect_identical(
    `%~==%`(c(NaN, NA_real_, NaN, NaN, Inf, Inf, -Inf, -Inf, 0, 0, -3, -3, 2, 2),
                 c(1, 2, NaN, 0, Inf, -Inf, Inf, -Inf, Inf, -Inf, Inf, -Inf, Inf, -Inf)),
    c(NA, NA, NA, NA, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE)
  )
  x <- c(NaN, NA_real_, 0, 0.01, -0.001, sqrt(2)^2, -sqrt(2)^2, Inf, -Inf, 10^7, -10^7)
  combs <- expand.grid(x1 = x, x2 = x)
  expect_identical(`%~==%`(combs$x1, combs$x2),
                             c(NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                               NA, NA, NA, NA, NA, NA, NA, NA, NA, TRUE, FALSE, FALSE, FALSE,
                               FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE, TRUE, FALSE,
                               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE, FALSE,
                               TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA, FALSE,
                               FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, NA, NA,
                               FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE,
                               NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
                               FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,
                               FALSE, FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                               FALSE, TRUE, FALSE, NA, NA, FALSE, FALSE, FALSE, FALSE, FALSE,
                               FALSE, FALSE, FALSE, TRUE))
  x <- c(NaN, NA_real_, 0, 0.01, -0.001, Inf, -Inf, 10^7, -10^7)
  combs <- expand.grid(x1 = x, x2 = x)
  expect_identical(`%~>%`(combs$x1, combs$x2),
                             combs$x1 > combs$x2)
  expect_identical(`%~>=%`(combs$x1, combs$x2),
                             combs$x1 >= combs$x2)
  expect_identical(`%~<%`(combs$x1, combs$x2),
                             combs$x1 < combs$x2)
  expect_identical(`%~<=%`(combs$x1, combs$x2),
                             combs$x1 <= combs$x2)

  # Abs diff would work here but rel diff doesn't
  expect_true(`%~==%`(10^9, 10^9 + 0.002))
  expect_false(`%~==%`(0, -0.00001))
  expect_false(`%~==%`(0, 0.00001))
  expect_false(`%~==%`(0, 10^20))
  expect_false(`%~==%`(0, -10^20))
  expect_false(`%~==%`(0, -10^20))
  expect_true(`%~==%`(10^-9, 2 * 10^-9)) # Default tolerance isnt low enough
  options(cppdoubles.tolerance = sqrt(.Machine$double.eps)/10^4)
  expect_false(`%~==%`(10^-9, 2 * 10^-9))
  options(cppdoubles.tolerance = sqrt(.Machine$double.eps))
})

test_that("vectorisation", {
  set.seed(42)
  x <- as.double(sample(c(Inf, -Inf, NA, -10:10), size = 111, replace = TRUE))
  y <- as.double(sample(0:5, size = 10^4, replace = TRUE))
  tol <- as.double(sample(c(0, 0.000001), size = 77, replace = TRUE,
                          prob = c(0.2, 0.8)))

  res_eq <- double_equal(x, y, tol)
  res_gte <- double_gte(x, y, tol)
  res_gt <- double_gt(x, y, tol)
  res_lte <- double_lte(x, y, tol)
  res_lt <- double_lt(x, y, tol)

  zero <- numeric(max(length(x), length(y), length(tol)))
  df <- suppressWarnings(data.frame(x + zero, y + zero, tol + zero))

  df$eq <- abs(df$x - df$y) < df$tol
  df$gt <- (df$x - df$y) > df$tol
  df$gte <- (df$x - df$y) > -df$tol
  df$lte <- (df$x - df$y) < df$tol
  df$lt <- (df$x - df$y) < -df$tol

  df$res_eq <- res_eq
  df$res_gte <- res_gte
  df$res_gt <- res_gt
  df$res_lte <- res_lte
  df$res_lt<- res_lt

  expect_equal(df$eq, df$res_eq)
  expect_equal(df$gte, df$res_gte)
  expect_equal(df$gt, df$res_gt)
  expect_equal(df$lte, df$res_lte)
  expect_equal(df$lt, df$res_lt)
})
