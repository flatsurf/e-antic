#ifndef EANTIC_PATCHES_NF_H
#define EANTIC_PATCHES_NF_H

#include <antic/nf.h>

#ifndef HAVE_NF_INIT_RANDTEST
FLINT_DLL void nf_init_randtest(nf_t nf, flint_rand_t state, slong len,  mp_bitcnt_t bits_in);
#endif

#endif
