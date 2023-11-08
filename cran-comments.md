
Resubmitted due to to issues with the description title and license.
Have changed the following:
  C++ in Description title has been changed to 'C++'
  License was referencing the wrong package name. 
  and has been changed from rdoubles to cppdoubles.

## Test environments
- R-hub windows-x86_64-devel (r-devel)
- R-hub ubuntu-gcc-release (r-release)
- R-hub fedora-clang-devel (r-devel)
- R-hub linux-x86_64-rocker-gcc-san (r-devel)

## R CMD check results
❯ On windows-x86_64-devel (r-devel)
  checking CRAN incoming feasibility ... NOTE
  Maintainer: 'Nick Christofides <nick.christofides.r@gmail.com>'
  
  New submission

❯ On windows-x86_64-devel (r-devel)
  checking for non-standard things in the check directory ... NOTE
  Found the following files/directories:
    ''NULL''

❯ On windows-x86_64-devel (r-devel)
  checking for detritus in the temp directory ... NOTE
  Found the following files/directories:
    'lastMiKTeXException'

❯ On ubuntu-gcc-release (r-release)
  checking CRAN incoming feasibility ... [6s/14s] NOTE
  Maintainer: ‘Nick Christofides <nick.christofides.r@gmail.com>’
  
  New submission

❯ On ubuntu-gcc-release (r-release), fedora-clang-devel (r-devel)
  checking HTML version of manual ... NOTE
  Skipping checking HTML validation: no command 'tidy' found

❯ On fedora-clang-devel (r-devel)
  checking CRAN incoming feasibility ... [6s/15s] NOTE
  Maintainer: ‘Nick Christofides <nick.christofides.r@gmail.com>’
  
  New submission

0 errors ✔ | 0 warnings ✔ | 6 notes ✖
