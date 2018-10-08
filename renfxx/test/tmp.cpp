/* relevant cpp files */
/*   Qsource/Qinput.cpp and Qsource/Qnormaliz.cpp */
/* structure for allocation */

/* istream: input stream
(canonical example is std::cin)
The internal extensible array is a general-purpose array of objects of
type long (if accessed with member iword) or void* (if accessed with
member pword). The xalloc provides a new valid index.

iword
pword
xalloc
*/


/* Qinput.cpp line 623-632 */
template<>
void read_number_field<renf_elem_class, renf_class>(istream &in, renf_class &renf)
{
    in >> renf;
    if (in.fail()) {
        throw BadInputException("Could not read number field!");
    }
    in >> set_renf(renf);
}

/* Qinput.cpp line 211-220 */
void string2coeff(renf_elem_class& coeff, istream& in, const string& s ){
            stringstream  for_coeff;

            for_coeff >> set_renf(get_renf(in));

            for_coeff << s;
            for_coeff >> coeff;
}

/* ios_base::xalloc

Returns a unique (program-wide) index value that can be used to access
one long and one void* elements in the private storage of
std::ios_base by calling iword() and pword(). The call to xalloc does
not allocate memory.
*/

/* the stream >> on renf_elem_class silently uses the previously */
/* stored value for the number field... */

/* renf manipulator */
struct set_renf
{
    renf_srcptr _nf;  // Does not belong to us.
    set_renf(renf_srcptr nf) { _nf = nf; }
    set_renf(const renf_class &NF) { _nf = NF.get_renf(); }
    static int xalloc();
};

inline int set_renf::xalloc()
{
    static int xa = std::ios_base::xalloc();
    return xa;
}

inline std::istream& operator>>(std::istream & is, const set_renf &sr)
{
    is.pword(set_renf::xalloc()) = (void *) sr._nf;
    return is;
}

inline renf* get_renf(std::istream& is)
{
    return (renf *) is.pword(set_renf::xalloc());  // transfer number field
}


