# Tests for the Python & SageMath Wrappers

The easiest way to run these tests is to run `make check` here. If you only
want to run a speceific test, use the `TESTS` variaible, e.g., `make check
TESTS=python-doctest.sh`. In either case, the test results are written to
`test-suite.log`; see the [automake
documentation](https://www.gnu.org/software/automake/manual/html_node/Scripts_002dbased-Testsuites.html#Scripts_002dbased-Testsuites)
for more details.

The tests are regular [pytest](https://docs.pytest.org/en/latest/) unit tests
for the most part. The two scripts, `python-doctest.sh` and `sage-doctest.sh`,
run doctests for the Python & SageMath docstrings however.

To manually run a test or go through the steps in an interactive Python
session, some environment variables need to be set so that we pick up the
libraries from here and not the ones potentially already installed in your
system. The easiest way to set these variables is to `source test-env.sh` in
your shell.
