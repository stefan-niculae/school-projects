#ifndef criptografie_Zn_h
#define criptografie_Zn_h

/*
 pt minus[] din corp.h
 ar trebui initializate elem la n cu card cu care a fost decl corpul
 asta inseana sa apelam Zn::set_n(card) in constructor
 si apoi mai apare problema si la citire
 */

class Zn
{
    int n;
    int val;
    static int nInit;
    
public:
    Zn(int nCat = -1, int valCat = -1);
    Zn& operator= (int x) { n = nInit, val = x; return *this; }
    Zn operator- () const {
        Zn temp;
        temp.set_n(get_n());
        temp.set_val(get_n() - get_val());
        
        return temp;
    }
    bool operator! () const {
        return !val;
    }
    static void nInitSet(int x) { Zn::nInit = x; };
    
    inline void set_n  (int x) {   n = x; }
    inline void set_val(int x) { val = x; }
    inline int get_n  () const { return n;   }
    inline int get_val() const { return val; }

};
int Zn::nInit = 0;

Zn::Zn(int nCat, int valCat) {
    try {
        n = nCat;
        val = n != -1 ? valCat % n : -1;
        
        if(n < 2 && n != -1) throw 101;
    } catch(int eroare) {
        if(eroare == 101)
            std::cout << "Nu exista Z0 sau Z1!";
        exit(eroare);
    }
}

std::istream& operator>> (std::istream& is, Zn& a) {
    int x;
    
    if(a.get_n() != -1) is >> x, a.set_n(x);
    is >> x; a.set_val(x % a.get_n());

    return is;
}
std::ostream& operator<< (std::ostream& os, Zn a) {
    if(a.get_n() == -1 || a.get_val() == -1)
        return os << "neinitializat ";
    return os << a.get_val() << " (mod " << a.get_n() << ")";
}

void nDiferite(Zn a, Zn b) {
    if(a.get_n() != b.get_n()) {
        std::cout << "Nu putem face operatii/comparatii cu un elem din Z" << a.get_n() << " si unul din Z" << b.get_n() << "!";
        exit(13);
    }
}

Zn operator+ (Zn a, Zn b) {
    nDiferite(a,b);
    a.set_val( (a.get_val() + b.get_val()) % a.get_n() );
    return a;
}
Zn operator* (Zn a, Zn b) {
    nDiferite(a,b);
    a.set_val( (a.get_val() * b.get_val()) % a.get_n() );
    return a;
}

Zn operator- (Zn a, Zn b) {
    nDiferite(a,b);
    b.set_val( b.get_n() - b.get_val() );
    return a + b;
}
Zn ZnInvers (Zn a) {
    for(int i=1; i<a.get_n(); i++)
        if( (a.get_val() * i) % a.get_n() == 1 ) {
            a.set_val(i);
            return a;
        }
    
    std::cout << "Nu exista inversul lui " << a.get_val() << "in Z" << a.get_n() << "!";
    return -1;
}
Zn operator/ (Zn a, Zn b) {
    nDiferite(a,b);
    return a * ZnInvers(b);
}

Zn operator+ (int a, Zn b) {    // nu putem sa-l facem din constructor pt ca nu stim ce n
    Zn temp(b.get_n(), a % b.get_n());
    return temp + b;
}
Zn operator- (int a, Zn b) {
    Zn temp(b.get_n(), a % b.get_n());
    return temp - b;
}
Zn operator* (int a, Zn b) {
    Zn temp(b.get_n(), a % b.get_n());
    return temp * b;
}
Zn operator/ (int a, Zn b) {
    Zn temp(b.get_n(), a % b.get_n());
    return temp / b;
}

Zn operator+= (Zn& a, Zn b) {
    a = a + b;
    return a;
}
Zn operator-= (Zn& a, Zn b) {
    a = a - b;
    return a;
}
Zn operator*= (Zn& a, Zn b) {
    a = a * b;
    return a;
}
Zn operator/= (Zn& a, Zn b) {
    a = a / b;
    return a;
}

bool operator== (Zn a, Zn b) {
    nDiferite(a, b);
    return a.get_val() == b.get_val();
}
bool operator!= (Zn a, Zn b) {
    return !(a==b);
}
bool operator< (Zn a, Zn b) {
    nDiferite(a, b);
    return a.get_val() < b.get_val();
}
bool operator> (Zn a, Zn b) {
    return !(a < b) && !(a == b);
}
bool operator<=(Zn a, Zn b) {
    return a < b || a == b;
}
bool operator>=(Zn a, Zn b) {
    return a > b || a == b;
}

#endif
