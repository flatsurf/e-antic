/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

#include <stdexcept>
#include <string>

using namespace eantic;

static void check_string(const renf_elem_class& a,
        const std::string& s_ALG_D,
        const std::string& s_ALG,
        const std::string& s_D)
{
    {
        std::string t = a.get_str(EANTIC_STR_ALG | EANTIC_STR_D);
        if (t != s_ALG_D)
            throw std::runtime_error("error with ALG | D, expected " + s_ALG_D + " got " + t);
    }
    {
        std::string t = a.get_str(EANTIC_STR_ALG);
        if (t != s_ALG)
            throw std::runtime_error("error with ALG, expected " + s_ALG + " got " + t);
    }
    {
        std::string t = a.get_str(EANTIC_STR_D);
        if (t != s_D)
            throw std::runtime_error("error with D, expected " + s_D + " got " + t);
    }
}

int main(void)
{
    {
        // fmpq elements (nf reference is NULL)
        {
            renf_elem_class a(mpq_class("1/2"));
            check_string(a, "(1/2 ~ 0.500000)", "1/2", "0.500000");
        }
    }

    {
        // quadratic example
        auto K = renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");

        {
            renf_elem_class a(K, "0");
            check_string(a, "(0 ~ 0)", "0", "0");
        }

        {
            std::string alg = "2/3";
            std::string d = "2/3";
            renf_elem_class a(K, alg);
            check_string(a, "(2/3 ~ 0.66666667)", "2/3", "0.66666667");
        }

        {
            std::string alg = "x";
            std::string d = "1.4142136";
            renf_elem_class a(K, alg);
            check_string(a, "(" + alg + " ~ " + d + ")", alg, d);
        }
    }

    {
        // cubic example
        auto K = renf_class::make("x^3 - 2/3", "x", "0.87 +/- 0.1");

        {
            renf_elem_class a(K, "0");
            check_string(a, "(0 ~ 0)", "0", "0");
        }

        {
            renf_elem_class a(K, "2/3");
            check_string(a, "(2/3 ~ 0.66666667)", "2/3", "0.66666667");
        }

        {
            std::string alg = "x";
            std::string d = "0.87358046";
            renf_elem_class a(K, alg);
            check_string(a, "(" + alg + " ~ " + d + ")", alg, d);
        }

        {
            std::string alg = "3/7*x-2/11";
            std::string d = "0.19257345";
            renf_elem_class a(K, alg);
            check_string(a, "(" + alg + " ~ " + d + ")", alg, d);
        }
    }
}
