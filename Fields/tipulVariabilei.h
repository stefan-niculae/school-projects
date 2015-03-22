#ifndef criptografie_tipulVariabilei_h
#define criptografie_tipulVariabilei_h

#include <cxxabi.h>
#include <string>

#define Rational double
#define Real long double

template <class Tip> class Multime;
template <class Tip2> std::string tipul(const Multime<Tip2> A)
{
    std::unique_ptr<char, void(*)(void*)>
    name{abi::__cxa_demangle(typeid(A.elem[0]).name(), 0, 0, nullptr), std::free};
    
    std::string s = name.get();

    return s;
}


#endif
