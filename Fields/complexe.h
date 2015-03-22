#ifndef criptografie_complexe_h
#define criptografie_complexe_h

#include <math.h>

class Complex
{
    long double re, im;
    
public:
    Complex(double a=0, double b=0) : re(a), im(b) { };
    Complex(const Complex& sursa) { re = sursa.re, im = sursa.im; }
    Complex& operator= (Complex);
    
    double modul();
    Complex operator-() const {
        Complex temp;
        temp.re = -re;
        temp.im = -im;
        return temp;
    }
    bool operator!() const {
        return !re && !im;
    }
    
    friend std::istream& operator>> (std::istream&, Complex&);
    friend std::ostream& operator<< (std::ostream&, Complex);
    
    friend Complex operator+ (Complex, Complex);
    friend Complex operator- (Complex, Complex);
    friend Complex operator* (Complex, Complex);
    friend Complex operator/ (Complex, Complex);
    
    friend Complex operator+= (Complex&, Complex);
    friend Complex operator-= (Complex&, Complex);
    friend Complex operator*= (Complex&, Complex);
    friend Complex operator/= (Complex&, Complex);
    
    friend bool operator== (Complex, Complex);
    friend bool operator!= (Complex, Complex);
    friend bool operator<  (Complex, Complex);
    friend bool operator>  (Complex, Complex);
    friend bool operator<= (Complex, Complex);
    friend bool operator>= (Complex, Complex);
    
    int get_n() { return -1; }
};

std::istream& operator>> (std::istream& is, Complex& z)
{
    is >> z.re;
    is >> z.im;
    return is;
}
std::ostream& operator<< (std::ostream& os, Complex z)
{
    if(z.re) os << z.re;
    if(z.im < 0) { os << '-'; if(z.im != -1 && z.im) os << -z.im; }
    else         { if(z.im) os << '+'; if(z.im !=  1 && z.im) os <<  z.im; }
    
    if(z.im)
        os << 'i';
    
    return os;
}
Complex operator+ (Complex st, Complex dr) {
    Complex temp;
    temp.re = st.re + dr.re;
    temp.im = st.im + dr.im;
    return temp;
}
Complex operator- (Complex st, Complex dr) {
    Complex temp;
    temp.re = st.re - dr.re;
    temp.im = st.im - dr.im;
    return temp;
}
Complex operator* (Complex st, Complex dr) {
    Complex temp;
    temp.re = st.re*dr.re - st.im*dr.im;
    temp.im = st.re*dr.im + st.im*dr.re;
    return temp;
}
Complex operator/ (Complex st, Complex dr) {
    Complex temp; double conj = dr.re*dr.re + dr.im*dr.im;
    temp.re = (st.re*dr.re + st.im*dr.im) / conj;
    temp.im = (st.im*dr.re - st.re*dr.im) / conj;
    return temp;
}

Complex& Complex::operator= (Complex dr) {
    re = dr.re;
    im = dr.im;
    return *this;
}

Complex operator+= (Complex &st, Complex dr) {
    st = st + dr;
    return st;
}
Complex operator-= (Complex &st, Complex dr) {
    st = st - dr;
    return st;
}
Complex operator*= (Complex &st, Complex dr) {
    st = st * dr;
    return st;
}
Complex operator/= (Complex &st, Complex dr) {
    st = st / dr;
    return st;
}

bool operator== (Complex st, Complex dr) {
    return st.re == dr.re && st.im == dr.im;
}
bool operator!= (Complex st, Complex dr) {
    return !(st==dr);
}
bool operator<  (Complex st, Complex dr) {
    return (st.re < dr.re && (st.im < dr.im || st.im == dr.im)) ||
    (st.im < dr.im && st.re == dr.re);
}
bool operator>  (Complex st, Complex dr) {
    return (st.re > dr.re && (st.im > dr.im || st.im == dr.im)) ||
    (st.im > dr.im && st.re == dr.re);
}
bool operator<= (Complex st, Complex dr) {
    return (st<dr) || (st==dr);
}
bool operator>= (Complex st, Complex dr) {
    return (st>dr) || (st==dr);
}

double Complex::modul() {
    return sqrt(re*re + im*im);
}

#endif