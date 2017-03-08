/*
	Proiect Atestat Profesional Informatica
	CNCV, 12B
	2013
	Stefan Niculae, Dan Zabava

	SNAKE
*/


/*
	TO DO:
	- modify your high score
	- intoarcere directie (cap = coada)
	- play again (inca un while + golire variabile)
*/

#include <fstream.h>
#include <conio.h>  // clrscr()
#include <stdlib.h> // getch(), kbhit()
#include <time.h>   // time()
#include <string.h> // strcpy()



int n, l_snake, a[100][100],
    lung_ec, lat_ec,
    x, y,
    scor, m_s,
    scoruri[11], timpuri[11],
    juctop, newhighscore;

long timp;
char last_dir, nume[11][15];

time_t start, curent, ultimul;
clock_t t;

ifstream fis_in("hs.txt");

//ofstream fis_out("hs.txt");


struct nod
{
    int x, y;
    nod* adr;
};

nod *p, *u, *p2, *u2;


// adaugarea nodurilor in lista
void adaug(nod*& p, nod*& u, int x, int y)
{
    nod* c = new nod;

    c->x = x;
    c->y = y;

    c->adr = NULL;

    // daca nu exista primul nod, il cream acum
    if (!p)
        p = u = c;

    // altfel, lipim in continuarea ultimului nod
    else
    {
        u->adr = c;
        u = c;
    }
}

// failed attempt at "heads = tails"
void inv_dir(nod*& c)
{
    if (c != NULL)
        inv_dir(c->adr);
    adaug(p2, u2, c->x, c->y);
}

void directie(char dir)
{
    // daca nu te intorci de unde ai venit
    /*if(last_dir == 'a' && dir == 'd' ||
		last_dir == 'd' && dir == 'a' ||
		last_dir == 'w' && dir == 's' ||
		last_dir == 's' && dir == 'w')
	{
		inv_dir(p);
		nod *c1 = new nod;
		nod *c2 = new nod;
		for(c1 = p, c2 = p2; c1!=NULL, c2!=NULL; c1=c1->adr, c2=c2->adr)
		{
			c2->x = c1->x;
			c2->y = c1->y;
			c2->adr = c1->adr;
		}

	}*/

    // deplaseaza cursorul sus/jos/dreapta/stanga
    if (dir == 'a')
        y--;
    if (dir == 'w')
        x--;
    if (dir == 's')
        x++;
    if (dir == 'd')
        y++;
}

// pt a sterge coada sarpelui
void stergere_primul(nod*& p)
{
    nod* temp;
    temp = p->adr;

    p->adr = NULL;
    p = temp;
}

// functie auxiliara, folosita la debugging
void afisare_lista(nod* p)
{
    nod* c;

    for (c = p; c != NULL; c = c->adr)
        cout << c->x << ',' << c->y << ' ';
    cout << endl;
}

void constr_ecran(nod* p)
{
    // lungim capul sarpelui cu o bucatica
    a[x][y] = 1;
    // stergem o bucatica din coada sarpelui
    a[p->x][p->y] = 0;
}

void afisare_ecran()
{
    clrscr(); //curatare ecran

    for (int i = 0; i <= lung_ec + 1; i++)
    {
        for (int j = 0; j <= lat_ec + 1; j++)
            if (i == 3 && j == lat_ec + 1)
                cout << ".    SCOR: " << scor;
            else if (i == 4 && j == lat_ec + 1)
                cout << ".    TIMP: " << (curent - start);
            // la limitele superioare si inferioare ale coordonatelor, afisam bordura
            else if (i == 0 || i == lung_ec + 1 || j == 0 || j == lat_ec + 1) 
                cout << '.';
            else if (a[i][j] == 1)
            {
                // daca are o bucata de mancare aproape, sarpele deschide gura in directia ei
                if (i == u->x && j == u->y)
                { 
                    if (a[i - 1][j] == 2 || a[i - 1][j] == 3)
                        cout << 'v';
                    else if (a[i + 1][j] == 2 || a[i + 1][j] == 3)
                        cout << '^';
                    else if (a[i][j - 1] == 2 || a[i][j - 1] == 3)
                        cout << '>';
                    else if (a[i][j + 1] == 2 || a[i][j + 1] == 3)
                        cout << '<';
                    else
                        cout << '@'; // capul este repr de @
                }
                else
                    cout << '*'; // corpul este repr de *
            }
            else if (a[i][j] == 2) // mancarea este reprezentata cu o
                cout << 'o';
            else if (a[i][j] == 3) // mancarea speciala este reprezentata cu x
                cout << 'x';
            else // blank in restul spatiului
                cout << ' ';
        cout << endl;
    }
    //cout << "processor clicks: " << (clock() - t);
}

int game_over()
{
    if (x == lung_ec + 1 || x == 0 || y == lat_ec + 1 || y == 0)
        return 1; // daca ai lovit peretii
    return (a[x][y] == 1); // daca te-ai muscat de coada
}

void newhighsc() // failed attempt at new high score
{
    for (int i = juctop; i > newhighscore; i--)
    {
        strcpy(nume[i], nume[i - 1]);
        scoruri[i] = scoruri[i - 1];
        timpuri[i] = timpuri[i - 1];
    }

    /*char numele_tau[15];
	for(i=1; i<=lat_ec/2-12; i++)
		cout << "GZ! New high score!" << endl;
	for(i=1; i<=lat_ec/2-8; i++)
		cout << "Your name? ";
	cin >> numele_tau;

	strcpy(nume[newhighscore],numele_tau);
	scoruri[newhighscore] = scor;
	timpuri[newhighscore] = timp;*/
}

void highsc()
{
    int i;

    for (i = 1; i <= lat_ec / 2 - 6; i++)
        cout << ' ';
    cout << "HIGH SCORES: " << endl;

    // afisam centrat lista cu high scores
    for (int j = 1; j < juctop; j++) 
    {
        for (i = 1; i <= lat_ec / 2 - 9; i++)
            cout << ' ';
        cout << j << ". " << nume[j] << " - " << scoruri[j] << " (" << timpuri[j] << " sec)" << endl;
    }
}

// ecranul de game over
void game_over_screen() 
{
    // citim din fisier numele, scorul si timpul
    while (!fis_in.eof()) 
    {
        juctop++;
        fis_in >> nume[juctop];
        fis_in >> scoruri[juctop];
        fis_in >> timpuri[juctop];

        if (scor > scoruri[juctop]) // verificam daca scorul jucatorului este mai mare decat vreun high score
            newhighscore = juctop;
    }

    /*if(newhighscore) // daca da, inseamna ca jucatorul a facut un nou high score                  
		newhighsc(); */


    clrscr();
    int i;
    //la mijlocul ecranului scriem GAME OVER
    for (i = 1; i <= lung_ec / 2 - 5; i++)
        cout << endl;
    for (i = 1; i <= lat_ec / 2 - 4; i++)
        cout << ' ';
    cout << "GAME OVER!" << endl;

    // incadram la jumatatea ecranului
    for (i = 1; i <= lat_ec / 2 - 3; i++) 
        cout << ' ';
    cout << "scor: " << scor; // afisam scorul
    if (newhighscore)
        cout << " *NEW HIGH SCORE*";
    cout << endl;

    for (i = 1; i <= lat_ec / 2 - 5; i++)
        cout << ' ';
    timp = (curent - start);
    cout << "timp: " << timp << " sec" << endl << endl << endl << endl;

    highsc(); // apelam functia de high score


    // failed attempt at "play again?"
    /*	for(i=1; i<=lat_ec/2-7; i++)       
		cout << ' ';
	cout << "Mai joci? (D/N)";

	char rasp; cin >> rasp;
	return (int)rasp;   */
}

void mancare(int x)
{
    int xr, yr;

    do // generam valori aleatori
    {   
        randomize();
        xr = rand() % lung_ec + 1;
        yr = rand() % lat_ec + 1;
    }
    while (a[xr][yr]); // avem grija sa nu plasam mancarea intr-un spatiu liber

    a[xr][yr] = x;
}


void creare(nod*& p, nod*& u) // crearea sarpelui initial
{
    for (int i = y; i <= y + l_snake; i++)
        adaug(p, u, x, i);
    for (nod* c = p; c != NULL; c = c->adr)
        a[c->x][c->y] = 1;
    
    y += l_snake; // actualizam poz actuala a capului sarpelui
    a[x][y - l_snake] = 0; // stergem o bucatica reziduala de sarpe

    last_dir = 'd'; // sarpele original este orientat spre dreapta
    n = (int)last_dir;

    mancare(2); // generam prima bucatica de mancare
}

void main()
{ 
    //t = clock();

    lung_ec = 12; //setare lungime + latime ecran
    lat_ec = 32;

    l_snake = 6; //setare pct initial si lungime sarpe
    x = lung_ec / 2;
    y = lat_ec / 2 - l_snake - 1;

    creare(p, u); // umplerea initiala a listei
    afisare_ecran(); // afisarea initiala

    time(&start);
    ultimul = curent = start;

    while (1)
    {
        time(&curent);
        // verificam sa fi trecut o secunda si acceptam numai tastele directie si esc
        if ((curent > ultimul) && (n == 27 || n == 97 || n == 115 || n == 100 || n == 119))
        { 

            directie((char)n); //codificam directiile in litere
            adaug(p, u, x, y); //adaugam ultima bucatica de sarpe

            if (a[x][y] != 2) // daca nu am mancat o bucata de mancare, stergem coada sarpelui
                stergere_primul(p);

            if (a[x][y] == 2 || a[x][y] == 3) // crestem scorul in functie de tipul de mancare
            {
                scor += a[x][y] - 1;
                if (a[x][y] == 2) // daca a mancat ultima bucata de mancare normala
                    mancare(2); // generam alta locatie pt mancare normala

                if (a[x][y] == 3)
                    m_s = 0; // retinem ca am mancat mancare speciala, deci nu mai exista pe ecran

                if (scor && scor % 4 == 0 && !m_s) // odata la fiecare 3 bucati de mancare normala, daca nu exista deja o mancare speciala
                {
                    mancare(3); // generam o bucata de mancare speciala
                    m_s = 1; // retinem ca exista o mancare speciala pe ecran
                }
            }


            if (game_over()) // la lovirea unui perete sau muscarea propriei cozi, se termina jocul
                break;

            else
            { // construim si afisam pe ecran
                constr_ecran(p);
                afisare_ecran();
            }

            last_dir = (char)n; // actualizam ultima directie
            ultimul = curent; // tinem minte cand a fost ultima secunda la care s-a facut afisarea
        }

        if (kbhit()) // daca a fost apasata o tasta
        {
            n = getch(); // retinem ce tasta a fost apasata in n

            if (n == 27) // esc = game over
                break;
        }
    }

    game_over_screen(); // cand am iesit din bucla jocului, afisam game over
    fis_in.close(); // inchidem fisierul de citire
    //fis_out.close();
}
