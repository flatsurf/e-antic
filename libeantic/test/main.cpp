#include <flint/flint.h>

#define CATCH_CONFIG_RUNNER
#include "external/catch2/single_include/catch2/catch.hpp"

int main( int argc, char* argv[] ) {
  int result = Catch::Session().run( argc, argv );

  flint_cleanup();

  return result;
}
