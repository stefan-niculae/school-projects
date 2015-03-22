#ifndef criptografie_expressionTree_h
#define criptografie_expressionTree_h

/*
 *   partial credits: http://cplusplus.kurttest.com/notes/stack.html#binary-expression-tree
 */

#include <stack>
#include <exception>
#include <sstream>

template <class Tip>
class NodExpresie
{                                                           // o expresie poate fi un numar sau un operator
public:
    virtual Tip val() = 0;
};

template <class Tip>
class NodNumar : public NodExpresie<Tip>
{                                                      // spunem numar, dar poate fi clasa de echiv, fractie etc
    Tip nr;
public:
    NodNumar(Tip x = NULL) : nr(x) { }
    virtual Tip val() { return nr; }
};

template <class Tip>
class NodOperatie : public NodExpresie<Tip>
{
    char op;
    
    NodExpresie<Tip>* st;
    NodExpresie<Tip>* dr;
    
public:
    NodOperatie(char o = NULL, NodExpresie<Tip>* s = NULL, NodExpresie<Tip>* d = NULL) :
    op(o), st(s), dr(d) { };
    
    virtual Tip val() {
        switch(op) {
            case '+' : return st->val() + dr->val();
            case '-' : return st->val() - dr->val();
            case '*' : return st->val() * dr->val();
            case '/' : return st->val() / dr->val();
        }
        
        return NULL;
    }
};

template <class Tip>
class ConstructorBET
{
    std::stack <char>               stivaOperatori;          // in ea avem ( + - * /
    std::stack <NodExpresie<Tip>* > stivaOperanzi;           // o expresie este un numar sau un operator
    
    void procesareOperator(char);
    void procesareParantDr();
    void calcOperatia(char);
    int  prioritateOp(char);
    
public:
    class FormulaGresita : public std::exception {
        public: virtual const char* ce() const throw() {
            return "Formula este gresita!";
        }
    };
    
    NodOperatie<Tip>* parsare(std::string& s, Tip x, Tip y, int nVect = -1) throw(FormulaGresita);
};

template <class Tip> int ConstructorBET<Tip>::prioritateOp(char op)
{
    enum { mica, mijlocie, mare };
    
    switch(op) {
        case '+':
        case '-':
            return mijlocie;
            
        case '*':
        case '/':
            return mare;
        
        default :
            return mica;
    }
}

template <class Tip> NodOperatie<Tip>* ConstructorBET<Tip>::parsare(std::string& s, Tip x, Tip y, int nVect) throw(FormulaGresita)
{
    std::istringstream is(s);
    char c;
    
    while(is >> c) {
        switch(c) {
            case '+':
            case '-':
            case '*':
            case '/':
                procesareOperator(c);
                break;
                
            case ')':
                procesareParantDr();
                break;
                
            case '(':
                stivaOperatori.push(c);
                break;
                
            default:            // dc am citit x sau y sau un coef
                Tip cePunem;    
                
                     if(c == 'x') cePunem = x;
                else if(c == 'y') cePunem = y;
                else {              // dc e coef
                    is.putback(c);  // il punem inapoi, poate se intinde pe mai multe caractere
                    
                    int coef;
                    is >> coef;     // il citim ca int
                    
                    if(nVect != -1)         // daca este vector mai intai setam dimensiunea
                        cePunem = nVect;    // apoi mai jos vom pune pe fiecare pozitie coeficientul
                    
                    cePunem = coef; // il convertim (are grija fiecare clasa sa faca conversia asta)

                }
                
                //std::cout << "am adaugat " << cePunem << " in stiva de operanzi\n";
                
                NodNumar<Tip>* nodNou = new NodNumar<Tip>(cePunem);
                stivaOperanzi.push(nodNou);
                continue;
        }
    }
    
    while(!stivaOperatori.empty()) {
        calcOperatia(stivaOperatori.top());
        stivaOperatori.pop();
    }
    
    if(stivaOperanzi.size() != 1)
        throw FormulaGresita();
        
    NodExpresie<Tip>* tos = stivaOperanzi.top();
    return static_cast<NodOperatie<Tip>*> (tos);
}

template <class Tip> void ConstructorBET<Tip>::procesareOperator(char op)
{
    while(!stivaOperatori.empty() && (prioritateOp(op) <= prioritateOp(stivaOperatori.top()))) {
        calcOperatia(stivaOperatori.top());                 // scoadem op cu cea mai mare prioritate, le construim nodul
        stivaOperatori.pop();
    }
    
    stivaOperatori.push(op);
}

template <class Tip> void ConstructorBET<Tip>::procesareParantDr()
{
    while(!stivaOperatori.empty() && stivaOperatori.top() != '(') {
        calcOperatia(stivaOperatori.top());
        stivaOperatori.pop();
    }
    
    stivaOperatori.pop();                           // scoatem '('
}

template <class Tip> void ConstructorBET<Tip>::calcOperatia(char op)
{
    NodExpresie<Tip>* dr = stivaOperanzi.top();             // scoatem cei mai de sus doi operanzi
    stivaOperanzi.pop();
    
    NodExpresie<Tip>* st = stivaOperanzi.top();
    stivaOperanzi.pop();
    
    NodOperatie<Tip>* x = new NodOperatie<Tip> (stivaOperatori.top(), st, dr);
    stivaOperanzi.push(x);                                 // ii inlocuim cu unul singur
}

template <class Tip>
class ArbBinExpr
{
    ConstructorBET<Tip> CBET;
    NodOperatie<Tip>* rad;
    std::string formula;
    
    std::string tipToString(Tip x) {
        std::ostringstream xStream;
        xStream << x;       // mare atetie la cum scriem ex: "6+7i" il scriem ca "6 7"!!!!!!
        
        return xStream.str();
    }
    void inlocuireInForm(std::string& s, char c, Tip x) {
        for(int i=0; i<formula.length(); i++)
            if(formula[i] == c) {
                formula.erase(i,1);                 // stergem de pe poz i cate 1 litera
                formula.insert(i, tipToString(x));         // inseram la    poz i numarul y (vazut ca string)
            }
    }
    
public:
    ArbBinExpr<Tip> (std::string form, Tip x, Tip y, int nVect) : formula(form) {
    
        //inlocuireInForm(formula, 'x', x);
        //inlocuireInForm(formula, 'y', y);
        
        rad = CBET.parsare(formula, x, y, nVect);
    }
    
    Tip rezultat() { return rad->val(); }
};

#endif
