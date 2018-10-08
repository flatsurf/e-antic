/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>

#include "e-antic/renfxx.h"

void check_invalid_renf(const std::string& s)
{
    std::istringstream is(s);
    renf_class f;

    try
    {
        is >> f;
    }
    catch (std::exception &e)
    {
        return;
    }

    throw 10;
}

void check_reconstruction(renf_class& k)
{
    renf_class k2;
    std::ostringstream os;

    os << k;
    std::istringstream is(os.str());
    is >> k2;

    if (!renf_equal(k.get_renf(), k2.get_renf()))
        throw 10;
}

void check_invalid_renf_elem(renf_class& k, const std::string& s)
{
    std::istringstream is(s);
    renf_elem_class a(k);

    try
    {
        is >> a;
    }
    catch (std::exception &e)
    {
        return;
    }

    throw 10;
}

int main(void)
{
    {
        renf_elem_class a;

        std::istringstream is("42");
        is >> a;
        if (a != 42)
        {
            std::cerr << "FAIL: C++ read rational" << std::endl;
            throw 10;
        }
    }

    {
        renf_class k1, k2;

        // read number field
        std::istringstream is1("minpoly 3  -2 0 1 embedding [1.41 +/- 0.3]");
        is1 >> k1;
        std::istringstream is2("minpoly 3  -3 0 1 embedding [1.73 +/- 0.3]");
        is2 >> k2;

        check_reconstruction(k1);
        check_reconstruction(k2);

        // read number field element
        renf_elem_class a(k1);
        std::istringstream is3("32 5/7");
        is3 >> a;

        if (a.parent() != k1 || a != (5 * k1.gen()) / 7 + 32)
        {
            std::cerr << "FAIL: C++ read nf element" << std::endl;
            throw 10;
        }

        // read number field element passing by the number field in the stream
        std::istringstream is4("1 1");
        is4 >> set_renf(k2) >> a;

        if (a.parent() != k2 || a != (k2.gen() + 1))
        {
            std::cerr << "FAIL: C++ read nf element" << std::endl;
            throw 10;
        }

    }

    return 0;
}
