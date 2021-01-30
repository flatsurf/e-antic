# Overview of the C++ interface

We reproduce below the C++ equivalent of the demo C program from [the C overview](overview_c.md).

```cpp
#include <e-antic/renfxx.h>
#include <iostream>

using namespace eantic;

int main() {
  const auto K = renf_class::make("a^3 - 3*a + 1", "a", "0.34 +/- 0.01", 64);

  const auto a = K->gen();
  const auto b = a * a - 2;
  std::cout << b << '\n';

  return 0;
}
```
