/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NF_EMB_ELEMXX_H
#define NF_EMB_ELEMXX_H

#include "renf.h"
#include "renf_elem.h"

class renf_elem_class
{
private:
    renf * nf;
    renf_elem_t a;
public:
    renf_elem_class(renf_t nf);

    ~renf_elem_class();

    renf_elem_class& operator=(slong n); 
    renf_elem_class& operator=(fmpz_t z);
    renf_elem_class& operator=(fmpq_t q);
    renf_elem_class& operator=(fmpq_poly_t p);
    renf_elem_class& operator=(renf_elem_class a);

    renf_elem_class operator+(const renf_elem_class &);
    renf_elem_class operator-(const renf_elem_class &);
    renf_elem_class operator*(const renf_elem_class &);
    renf_elem_class operator/(const renf_elem_class &);

    // unary operators
    renf_elem_class operator-();
    renf_elem_class operator+();

// also implement integer/rational comparison

    bool operator>(renf_elem_class &);
    bool operator>=(renf_elem_class &);
    bool operator==(renf_elem_class &);
    bool operator<=(renf_elem_class &);
    bool operator<(renf_elem_class &);
    bool operator!=(renf_elem_class &);

    bool is_zero() { return renf_elem_is_zero(this->a, this->nf); };
    bool is_one() { return renf_elem_is_one(this->a, this->nf); };

// input, output operators <<, >> (for streams)
    friend std::ostream& operator << (std::ostream &, const renf_elem_class&);
    void print();
};

renf_elem_class::renf_elem_class(renf_t nf)
{
    this->nf = nf;
    renf_elem_init(this->a, nf);
    renf_elem_zero(this->a, nf);
}

#define __renf_elem_set(TYP, FUN) \
    renf_elem_class& renf_elem_class::operator=(TYP n) { FUN(this->a, n, this->nf); }
__renf_elem_set(slong, renf_elem_set_si);
__renf_elem_set(fmpz_t, renf_elem_set_fmpz);
__renf_elem_set(fmpq_t, renf_elem_set_fmpq);
__renf_elem_set(fmpq_poly_t, renf_elem_set_fmpq_poly);
#undef __renf_elem_set
renf_elem_class& renf_elem_class::operator=(renf_elem_class a)
{
    renf_elem_set(this->a, a.a, this->nf);
}

renf_elem_class::~renf_elem_class(void)
{
    renf_elem_clear(this->a, this->nf);
}

std::ostream& operator << (std::ostream & os, const renf_elem_class& a)
{
    char * res = renf_elem_get_str_pretty(a.a, "x", a.nf, 5);
    os << res;
    flint_free(res);
    return os;
}

renf_elem_class renf_elem_class::operator+(const renf_elem_class & other)
{
    renf_elem_class ans(this->nf);
    renf_elem_add(ans.a, this->a, other.a, this->nf);
    return ans;
}

renf_elem_class renf_elem_class::operator-(const renf_elem_class &other)
{
    renf_elem_class ans(this->nf);
    renf_elem_sub(ans.a, this->a, other.a, this->nf);
    return ans;
}

renf_elem_class renf_elem_class::operator*(const renf_elem_class & other)
{
    renf_elem_class ans(this->nf);
    renf_elem_mul(ans.a, this->a, other.a, this->nf);
    return ans;
}

renf_elem_class renf_elem_class::operator/(const renf_elem_class & other)
{
    renf_elem_class ans(this->nf);
    renf_elem_div(ans.a, this->a, other.a, this->nf);
    return ans;
}

renf_elem_class renf_elem_class::operator-()
{
    renf_elem_class ans(this->nf);
    renf_elem_neg(ans.a, this->a, ans.nf);
    return ans;
}

renf_elem_class renf_elem_class::operator+()
{
    renf_elem_class ans(this->nf);
    renf_elem_set(ans.a, this->a, this->nf);
    return ans;
}

bool renf_elem_class::operator==(renf_elem_class & other)
{
    return renf_elem_equal(this->a, other.a, this->nf);
}

bool renf_elem_class::operator!=(renf_elem_class & other)
{
    return not renf_elem_equal(this->a, other.a, this->nf);
}

bool renf_elem_class::operator>(renf_elem_class & other)
{
    if (this->nf != other.nf)
    {
        fprintf(stderr, "DIFFERENT FIELDS\n");
        flint_abort();
    }
    return renf_elem_cmp(this->a, other.a, this->nf) > 0;
}
bool renf_elem_class::operator>=(renf_elem_class & other)
{
    return renf_elem_cmp(this->a, other.a, this->nf) >= 0;
}
bool renf_elem_class::operator<=(renf_elem_class & other)
{
    return renf_elem_cmp(this->a, other.a, this->nf) <= 0;
}
bool renf_elem_class::operator<(renf_elem_class & other)
{
    return renf_elem_cmp(this->a, other.a, this->nf) < 0;
}

#endif

