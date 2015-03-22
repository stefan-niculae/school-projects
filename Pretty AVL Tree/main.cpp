#include <iostream>
#include <fstream>

using namespace std;

ifstream f ("avltree.txt");

struct nod
{
    int val, h;
    nod *st, *dr;
} *rad;

void inaltimare(nod *t)
{
    if(t->st && t->dr)
        t->h = t->st->h > t->dr->h ? t->st->h +1 : t->dr->h +1;
    else if(t->st)
        t->h = t->st->h +1;
    else if(t->dr)
        t->h = t->dr->h +1;
    else
        t->h = 1;
}

nod* creare(nod *t, int x)
{
    if(!t)
    {
        t = new nod;
        t->val = x;
        t->st = t->dr = NULL;

        t->h = 1;                         // initializam inaltimea cu zero (pt frunze)
    }
    else if(x < t->val)
        t->st = creare(t->st,x);
    else
        t->dr = creare(t->dr,x);

    inaltimare(t);

    /*
    if(caz == 2 && rheavy(t))
        if(x < t->st->val)
            LL(t);
        else
            LR(t);
    else if(caz == 3 && lheavy(t))
        if(x > t->dr->val)
            RR(t);
        else
            RL(t);
    */

    return t;
}

void afisare(nod *rad)
{
   if(rad)
   {
       afisare(rad->st);
       cout << rad->val << ' ';
       afisare(rad->dr);
   }
}

void citire()
{
   int n, x;

   f >> n;
   for(int i=1; i<=n; i++)
   {
       f >> x;
       rad = creare(rad,x);
   }
}

void LL(nod *a)                 // left rotation
{
    /*
       a                            b
         \                        /   \
           b        --LL-->     a       c
             \
               c
    */
    nod *b = a->dr;
    nod *c = b->dr;
    rad = b;                    // b este noua radacina
    a->dr = b->st;              // a adopta in dreapta copilul stang al lui b
    b->st = a;                  // b il adopta pe a in stanga

    inaltimare(a);
    inaltimare(b);
    inaltimare(c);
}

void RR(nod *c)                 // right rotation
{
    /*
             c                      b
            /                     /   \
           b        --RR-->     a       c
         /
       a
    */

    nod *b = c->st;
    nod *a = b->st;
    rad = b;                    // c este noua radacina
    c->st = b->dr;              // c il adopta pe copilul drept al lui b in staga
    b->dr = c;                  // b il adopta pe c in dreatpta

    inaltimare(a);
    inaltimare(b);
    inaltimare(c);
}

void LR(nod *a)                 // double left
{
    /*
       a                     a                           b
        \                     \                        /   \
         c      --RR(c)-->     b        --LL(a)-->    a     c
        /                       \
       b                         c
    */

    nod *c = a->dr;
    nod *b = c->st;

    RR(c);                      // rotiim spre stanga dupa c
    a->dr = rad;                // avem grija sa adoptam in dreapta lui a pe b (noua radacina dupa rotatie)

    LL(a);

    inaltimare(a);
    inaltimare(b);
    inaltimare(c);
}

void RL(nod *c)                 // double right
{
    /*
          c                      c                       b
         /                      /                      /   \
       a        --LL(a)-->     b        --RR(c)-->    a     c
         \                    /
          b                  a
    */

    nod *a = c->st;
    nod *b = a->dr;

    LL(a);                      // rotim spre stanga dupa a
    c->st = rad;                // si avem grija sa adoptam pe b (noua radacina dupa rotire) in stanga lui c

    RR(c);                      // rotim spre dreapta dupa c

    inaltimare(a);
    inaltimare(b);
    inaltimare(c);
}

void afisval()
{
    /*
        4 2 6 1 3 5 7
    */

    cout << endl << "valorile: " << endl;

    cout << "     " << rad->val << endl;
    if(rad->st) cout << "   /"; else cout << "   ";
    if(rad->dr) cout << "   \\ ";
    cout << endl;
    if(rad->st) cout << "  " <<  rad->st->val; else cout << "  ";
    if(rad->dr) cout << "     " << rad->dr->val;

    cout << endl;
    if(rad->st || rad->dr)
    {
        if(rad->st && rad->st->st) cout << " /"; else cout << "  ";
        if(rad->st && rad->st->dr) cout << " \\";else cout << "  ";

        if(rad->dr && rad->dr->st) cout << "   /"; else cout << "    ";
        if(rad->dr && rad->dr->dr) cout << " \\";

        cout << endl;

        if(rad->st && rad->st->st) cout << "" <<  rad->st->st->val; else cout << " ";
        if(rad->st && rad->st->dr) cout << "   " << rad->st->dr->val; else cout << "    ";

        if(rad->dr && rad->dr->st) cout << " " << rad->dr->st->val; else cout << "  ";
        if(rad->dr && rad->dr->dr) cout << "   " << rad->dr->dr->val;
    }

    cout << endl;
}

void afish()
{
    cout << endl << "inaltimile: " << endl;

    cout << "     " << rad->h << endl;
    if(rad->st) cout << "   /"; else cout << "   ";
    if(rad->dr) cout << "   \\ ";
    cout << endl;
    if(rad->st) cout << "  " <<  rad->st->h; else cout << "  ";
    if(rad->dr) cout << "     " << rad->dr->h;

    cout << endl;
    if(rad->st || rad->dr)
    {
        if(rad->st && rad->st->st) cout << " /"; else cout << "  ";
        if(rad->st && rad->st->dr) cout << " \\";else cout << "  ";

        if(rad->dr && rad->dr->st) cout << "   /"; else cout << "    ";
        if(rad->dr && rad->dr->dr) cout << " \\";

        cout << endl;

        if(rad->st && rad->st->st) cout << "" <<  rad->st->st->h; else cout << " ";
        if(rad->st && rad->st->dr) cout << "   " << rad->st->dr->h; else cout << "    ";

        if(rad->dr && rad->dr->st) cout << " " << rad->dr->st->h; else cout << "  ";
        if(rad->dr && rad->dr->dr) cout << "   " << rad->dr->dr->h;
    }

    cout << endl;
}

bool rheavy(nod *t)
{
    if(t->dr && t->st)
        return (t->dr->h - t->st->h > 1);
    if(t->dr)
        return (t->dr->h > 1);
    return 0;
}

bool lheavy(nod *t)
{
    if(t->dr && t->st)
        return (t->st->h - t->dr->h > 1);
    if(t->st)
        return (t->st->h > 1);
    return 0;
}

void balance(nod *t)
{
    if(rheavy(t) && rheavy(t->dr)) LL(t);
    if(rheavy(t) && lheavy(t->dr)) LR(t);

    if(lheavy(t) && lheavy(t->st)) RR(t);
    if(lheavy(t) && rheavy(t->st)) RL(t);

    if(rheavy(t)) LL(t);
    if(lheavy(t)) RR(t);
}

void traversare(nod *t)
{
    if(!t) return;
    if(t->st) traversare(t->st);
    if(t->dr) traversare(t->dr);
    balance(t);
}

int main()
{
    citire();
    afisval(); afish();

    traversare(rad);
    afisval(); afish();

    return 0;
}
