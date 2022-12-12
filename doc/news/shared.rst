**Removed:**

* Removed the static library from the conda package that we upload with every commit to the master branch to the flatsurf channel. To our knowledge nobody is using these builds. They can sometimes be useful for debugging because they are built with debug symbols (which is why they are very big.) Providing a static library that nobody should use anyway is eating up the limited storage on anaconda.org twice as fast so we disable this.
