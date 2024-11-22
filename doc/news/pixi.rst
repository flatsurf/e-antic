**Changed:**

* Changed the recommended setup for developers to use pixi instead of conda/mamba. See the README for details.
* Changed ``configure`` to not complain when test-requirements such as byexample, benchmark, realalg, or SageMath are missing. (The tests are included if the dependencies are detected but just disabled otherwise without the need for ``--without-byexample --without-benchmark --without-sage --without-realalg``.)

**Fixed:**

* Fixed dependency tracking when ``configure`` is rerun with different compiler flags by tracking config.h in all source files.

**Removed:**

* Removed binder demo from our README (we do not think that anybody has been using it never really worked great.)
