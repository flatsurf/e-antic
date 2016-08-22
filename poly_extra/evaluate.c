/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "fmpz_poly.h"
#include "fmpq_poly.h"
#include "arb.h"
#include "arf.h"

#define NUMBER arb
#include "poly_evaluate_template.h"
#undef NUMBER

#define NUMBER arf
#define ADD(z,x,y,prec) arf_mul(z,x,y,prec,ARF_RND_NEAR)
#define MUL(z,x,y,prec) arf_mul(z,x,y,prec,ARF_RND_NEAR)
#define ADD_FMPZ(z,x,y,prec) arf_add_fmpz(z,x,y,prec,ARF_RND_NEAR)
#define DIV_FMPZ(z,x,y,prec) arf_div_fmpz(z,x,y,prec,ARF_RND_NEAR)
#include "poly_evaluate_template.h"
#undef NUMBER
