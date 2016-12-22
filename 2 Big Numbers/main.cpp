#include <iostream>
#include "numarIntreg.h"
#include "numarReal.h"

using namespace std;

int main()
{
    NrIntreg a("1000000000000000000000000000000000000000000000000000000000000");
    NrIntreg b(                                                    "123456789");
    cout << a << " + " << b << " = " << endl
         << a + b << endl << endl;

    NrIntreg x("111111111");
    NrIntreg y("111111111");
    cout << x << " * " << y << " = " << endl
         << x * y << endl << endl;

    return 0;
}
