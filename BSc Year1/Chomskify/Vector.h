#ifndef Vector_h
#define Vector_h

#define NMAX 100

template <class Tip>
class Vector
{
    Tip elem[NMAX];
    int n;
    
public:
    
    Vector<Tip> (int N = 0, Tip* elemente = nullptr)
    {
        n = N;
        
        for(int i=0; i<n; i++)
            elem[i] = elemente[i];
        
    }
    /*Vector<Tip> (Tip x)                             // constructor dintr-un singur element
     {
     n = 1;
     elem[0] = x;
     }*/
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, Vector<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Vector<Tip2>);
    
    template <class Tip2> friend bool operator== (Vector<Tip2>, Vector<Tip2>);
    
    Vector<Tip> operator() (Tip* elemente, int N = -1)
    {
        int i;
        
        if(N == -1)                                     // mai mult pt stringuri asta
            for(i=0; elemente[i]; i++)
                elem[i] = elemente[i];
        else
            for(i=0; i<N; i++)
                elem[i] = elemente[i];
        
        n = i;
        
        return *this;
    }
    
    Tip& operator[] (int);
    inline int lungime() const { return n; }
    
    int adauga(Tip x, int poz = -1);
    int stergeElem(Tip);
    int stergePoz(int);
    
    int gasire(Tip);
    inline bool contine(Tip);
    Vector<Tip> sortare(bool descresc = 0);
    
    int reuniune(Tip x) {
        if(!contine(x))
            return adauga(x);
        else
            return -1;
    }
    
    template <class Tip2> friend Vector<Tip2> operator+ (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator- (Vector<Tip2>, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator* (Vector<Tip2>, Vector<Tip2>);   // reuniune
    template <class Tip2> friend Vector<Tip2> operator/ (Vector<Tip2>, Vector<Tip2>);   // intersectie
    
    template <class Tip2> friend Vector<Tip2> operator+= (Vector<Tip2>&, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator-= (Vector<Tip2>&, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator*= (Vector<Tip2>&, Vector<Tip2>);
    template <class Tip2> friend Vector<Tip2> operator/= (Vector<Tip2>&, Vector<Tip2>);
    
};

template <class Tip2> std::istream& operator>> (std::istream& is, Vector<Tip2>& v)
{
    is >> v.n;
    for(int i=0; i<v.n; i++)
        is >> v.elem[i];
    
    return is;
}

template <class Tip2> std::ostream& operator<< (std::ostream& os, Vector<Tip2> v)
{
    bool eString = typeid(v.elem[0]).name()[0] == 'c' ||           // daca e char
    typeid(v.elem[0]).name()[0] == 'h';             // sau unsigned char
    
    for(int i=0; i<v.n; i++)
        os << v.elem[i] << (eString ? "" : " ");
    
    return os;
}

template <class Tip2> bool operator== (Vector<Tip2> v, Vector <Tip2> w)
{
    if(v.n != w.n)
        return 0;
    
    for(int i=0; i<v.n; i++)
        if(v[i] != w[i])
            return 0;
    
    return 1;
}

template <class Tip> Tip& Vector<Tip>::operator[] (int i)
{
    if(i < 0 || i > n-1)                                        // elem indexate de la 0 la n-1
        throw 101;
    
    return elem[i];
}

template <class Tip> int Vector<Tip>::adauga(Tip x, int poz)
{
    n++;
    if(poz != -1 && (poz < 0 || poz > n-1))
        throw 102;
    
    if(poz == -1)
        elem[n-1] = x;
    else {
        for(int i=n; i>poz; i--)
            elem[i] = elem[i-1];
        
        elem[poz] = x;
    }
    
    return n;
}

template <class Tip> int Vector<Tip>::stergePoz(int poz)
{
    if(poz < 0 || poz > n-1)
        throw 103;
    
    n--;
    for(int i=poz; i<n; i++)
        elem[i] = elem[i+1];
    elem[n] = NULL;
    
    return n;
}

template <class Tip> int Vector<Tip>::stergeElem(Tip x)
{
    if(!contine(x))
        throw 104;
    
    return stergePoz(gasire(x));
}

template <class Tip> int Vector<Tip>::gasire(Tip x)
{
    for(int i=0; i<n; i++)
        if(x == elem[i])
            return i;
    return -1;
}

template <class Tip> bool Vector<Tip>::contine(Tip x)
{
    return (gasire(x) != -1);
}

template <class Tip> Vector<Tip> Vector<Tip>::sortare(bool descresc)
{
    for(int i=1; i<n-1; i++)
        for(int j=i+1; j<n; j++)
            if( (descresc && elem[i] < elem[j]) ||
               (!descresc && elem[i] > elem[j]) )
                std::swap(elem[i], elem[j]);
    
    return *this;
}

template <class Tip2> Vector<Tip2> operator+ (Vector<Tip2> V, Vector<Tip2> W)
{
    for(int i=0; i<W.n; i++)
        V.adauga(W[i]);
    
    return V;
    
}
template <class Tip2> Vector<Tip2> operator- (Vector<Tip2> V, Vector<Tip2> W)
{
    for(int i=0; i<W.n; i++)
        V.stergeElem(W[i]);
    
    return V;
    
}
template <class Tip2> Vector<Tip2> operator* (Vector<Tip2> V, Vector<Tip2> W)
{
    for(int i=0; i<W.n; i++)
        V.reuniune(W[i]);
    
    return V;
    
}
template <class Tip2> Vector<Tip2> operator/ (Vector<Tip2> V, Vector<Tip2> W)
{
    for(int i=0; i<V.n; i++)
        if(!W.contine(V[i])) {
            V.stergePoz(i);
            i--;
        }
    
    return V;
}

template <class Tip2> Vector<Tip2> operator+= (Vector<Tip2>& V, Vector<Tip2> W)
{
    return (V = V + W);
}
template <class Tip2> Vector<Tip2> operator-= (Vector<Tip2>& V, Vector<Tip2> W)
{
    return (V = V - W);
}
template <class Tip2> Vector<Tip2> operator*= (Vector<Tip2>& V, Vector<Tip2> W)
{
    return (V = V * W);
}
template <class Tip2> Vector<Tip2> operator/= (Vector<Tip2>& V, Vector<Tip2> W)
{
    return (V = V / W);
}

#endif