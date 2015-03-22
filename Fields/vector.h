#ifndef criptografie_vector_h
#define criptografie_vector_h

#define NMAX 100

template <class Tip>
class Vector
{
    Tip comp[NMAX];
    int n;
    
public:
    
    Vector (int N = -1, int* = NULL);
    Vector& operator= (int x)
    {
        if(n == -1)
            n = x;
        else
            for(int i=0; i<n; i++)
                comp[i] = x;
        
        return *this;
    }
    Vector operator- () {
        Vector temp;
        
        temp.n = n;
        for(int i=0; i<n; i++)
            temp.comp[i] = -comp[i];
        
        return temp;
    }
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, Vector<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Vector<Tip2> );
    
    template <class Tip2> friend Vector<Tip2> operator+ (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator- (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator* (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator/ (Vector<Tip2>, Vector<Tip2>);
    
    template <class Tip2> friend Vector<Tip2> operator+ (int, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator- (int, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator* (int, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator/ (int, Vector<Tip2>);
    
    template <class Tip2> friend bool operator== (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend bool operator!= (Vector<Tip2>, Vector<Tip2>);
    
    void set_n (int x) {
        n = x;
    }
    int get_n() {
        return n;
    }
    void set_elem(int indice, Tip val) {
        comp[indice] = val;
    }
    
};

template <class Tip> Vector<Tip>::Vector(int N, int* v)
{
    n = N;
    for(int i=0; i<n; i++)
        comp[i] = v[i];
}

template <class Tip2>  std::istream& operator>> (std::istream& is, Vector<Tip2>& V)
{
    if(V.n == -1) is >> V.n;
    
    for(int i=0; i<V.n; i++)
        is >> V.comp[i];
    
    return is;
}
template <class Tip2>  std::ostream& operator<< (std::ostream& os, Vector<Tip2> V)
{
    if(V.n == -1) {
        std::cout << "Mai intai defineste n pt a afisa un vector!";
        exit(40);
    }
    
    os << "(";
    for(int i=0; i<V.n-1; i++)
        os << V.comp[i] << ", ";
    os << V.comp[V.n-1] << ")";
    
    return os;
}

template <class Tip2> Vector<Tip2> operator+ (Vector<Tip2> V, Vector<Tip2> W)
{
    if(V.n != W.n)
        throw 65;
    for(int i=0; i<V.n; i++)
        V.comp[i] += W.comp[i];
    return V;
}
template <class Tip2> Vector<Tip2> operator- (Vector<Tip2> V, Vector<Tip2> W)
{
    if(V.n != W.n)
        throw 65;
    for(int i=0; i<V.n; i++)
        V.comp[i] -= W.comp[i];
    return V;
}
template <class Tip2> Vector<Tip2> operator* (Vector<Tip2> V, Vector<Tip2> W)
{
    if(V.n != W.n)
        throw 65;
    for(int i=0; i<V.n; i++)
        V.comp[i] *= W.comp[i];
    return V;
}
template <class Tip2> Vector<Tip2> operator+ (int a, Vector<Tip2> V)
{
    for(Tip2 x : V.comp)
        x += a;
    return V;
}
template <class Tip2> Vector<Tip2> operator- (int a, Vector<Tip2> V)
{
    return -a + V;
}
template <class Tip2> Vector<Tip2> operator* (int a, Vector<Tip2> V)
{   // nu-l putem face din constructor cu un singur int pt ca nu stim pe cate comp este (n-ul adica)
    for(Tip2 x : V.comp)
        x *= a;
    return V;
}
template <class Tip2> Vector<Tip2> operator/ (Vector<Tip2> V, Vector<Tip2> W)
{
    if(V.n != W.n)
        throw 65;
    for(int i=0; i<V.n; i++)
        if(!W.comp[i]) throw 999;   // protectie pt x/0 (am avut grija sa supraincarcam op! ())
        else V.comp[i] /= W.comp[i];
    return V;
}

template <class Tip2> Vector<Tip2> operator/ (int a, Vector<Tip2> V)
{
    if(!a) throw 999;               // protectie pt x/0
    return 1/a * V;
}

template <class Tip2> bool operator== (Vector<Tip2> V, Vector<Tip2> W)
{
    for(int i=0; i<V.n; i++)
        if(V.comp[i] != W.comp[i])
            return 0;
    return 1;
}
template <class Tip2> bool operator!= (Vector<Tip2> V, Vector<Tip2> W)
{
    return !(V == W);
}

#endif
