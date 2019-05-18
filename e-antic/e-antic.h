#ifndef E_ANTIC_H
#define E_ANTIC_H

#include <gmp.h>
#include <flint/flint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* e-antic version number */

#define __E_ANTIC_VERSION 1
#define __E_ANTIC_VERSION_MINOR 0
#define __E_ANTIC_VERSION_PATCHLEVEL 0
#define E_ANTIC_VERSION "1.0"
#define __E_ANTIC_RELEASE (__E_ANTIC_VERSION * 10000 + \
                           __E_ANTIC_VERSION_MINOR * 100 + \
                           __E_ANTIC_VERSION_PATCHLEVEL)

/* check flint version */

#if __FLINT_RELEASE != 20502 && __FLINT_RELEASE != 20503
#error FLINT 2.5.2 or 2.5.3 required
#endif

#ifdef __cplusplus
}
#endif

#endif
