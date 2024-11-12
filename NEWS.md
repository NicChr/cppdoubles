# cppdoubles 0.3.0

### Breaking changes

-   The 'C++' functions have been renamed for readability and consistency though the equivalent R function names remain unchanged.

### Internal improvements

-   Vectorised loops have been internally sped-up.
-   Code has been simplified for readability.

### Bug fixes

-   `rel_diff(0, 0)` now correctly returns `0`

# cppdoubles 0.2.0

-   New function `all_equal` as an efficient alternative to `all.equal` for numeric vectors.

-   `NA` values are now returned when `tol` is `NA`.

# cppdoubles 0.1.1

-   CRAN resubmission.

# cppdoubles 0.1.0

-   Initial CRAN submission.
