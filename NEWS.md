# cppdoubles 0.5.0

- cppdoubles has migrated its C++ dependency from cpp11 to cppally.

- Major speed improvements to most functions through the use of 
parallelisation and compiler optimisations.

- New functions `get_threads` and `set_threads` for getting and setting 
threads for parallel computation.

# cppdoubles 0.4.0

### New features

- `get_tolerance` and `set_tolerance` as helpers to get/set the package-wide
tolerance.

- New argument `scale` added to `rel_diff` to allow users to optionally
specify a scale for each relative difference.

### Changes

- The default argument for `tol` now uses `get_tolerance()`.

### Internal updates

- Inlined most of the simple C++ functions which provides some moderate
speed improvements.

- Some C++ functions were returning double when they should return bool. This
has now been fixed.

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
