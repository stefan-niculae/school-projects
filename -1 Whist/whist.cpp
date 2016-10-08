/*
	WHIST - Stefan Niculae
	10.02.2012 - 11.02.2012
*/


#include <fstream.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

int i, j, n, m, r, o, u, runda, start=1, streak[7], facut[7], lista[33], scor[7][33], zis[2][7][33];
char juc[7][9];
ofstream f ("C:\\whist.txt");

struct score
{
	int s;
	char n[10];
} sf[7];

void cit();
void afis(int prima = 0);
void creare_lista(int opturi = 4, int unuri = 4);
void prerunda();
void postrunda();
void final();
void backup(int x=1);

void main ()
{
	//backup();

   cout << "Cati jucatori? ";
	cin >> n;

	creare_lista(n,n);
	cit();
	afis(1);


	for(r=start; r<=m; r++)
	{
		runda = lista[r];

		prerunda();	  		afis();

		postrunda();	 	afis();
	}

	final();
}

void backup(int x)
{
	if(x==1)
	{
		ifstream g ("C:\\backup.txt");
		g >> n >> o >> u >> r; start = r;
		for(i=1; i<=7; i++)
		{
				g >> streak[i] >> facut[i] >> lista[i];
				for(j=1; j<=33; j++)
					g >> scor[i][j] >> zis[1][i][j];
		}
	}
	else
	{
		ofstream h("C:\\backup.txt");
		h.open("C:\\backup.txt", ios::trunc);

		h << n <<' ' << o <<' '<< u<<' ' << r<< ' ';
		for(i=1; i<=7; i++)
		{
				h << streak[i] << ' ' << facut[i] << ' ' << lista[i] << ' ';
				for(j=1; j<=33; j++)
					h << scor[i][j] << ' ' << zis[1][i][j] <<' ';
		}
	}
}

void final()
{
	for(i=1; i<=n; i++)
	{
		sf[i].s = scor[i][m];
		strcpy(sf[i].n,juc[i]);
	}

	int t;
	char temp[10];
	for (i=1; i<n; i++)
		for(j=i+1; j<=n; j++)
			if(sf[i].s < sf[j].s)
			{
				t = sf[i].s; strcpy(temp,sf[i].n);
				sf[i].s = sf[j].s; strcpy(sf[i].n,sf[j].n);
				sf[j].s = t; strcpy(sf[j].n,temp);
			}
	for(i=1; i<=n; i++)
	{
		if(i==1)
			cout << "Castigatorul: ";
		else if(i==n)
			cout << "Ultimul loc: ";
		else
			cout << "Locul " << i << ": ";
		cout << sf[i].n << " (" << sf[i].s << ")" << endl;
	}
}

void prerunda()
{
	int x=1, zise = 0;
	for(i=1; i<=n; i++)
	{
		cout << "Cate zice " << juc[i];
		if(i==n)
		{
			if(x < 4 && x > 0)
				cout << " (fara " << x << ")";
			else
				cout << " (oricate)";
		}
		cout << "? ";

		cin >> zis[1][i][r];
		zise += zis[1][i][r];
		if(i==n)
			while(zis[1][i][r] == x)
			{
				cout << "Fara " << x <<": ";
				cin >> zis[1][i][r];
			}

		if(i==n-1)
			x = runda-zise;
	}
}

void afis(int prima)
{
	clrscr();
	f.open("C:\\whist.txt", ios::trunc);

	int ok=1;

	for(i=1; i<=m+2; i++)
	{
		if(r <= m/2)
			if(i > m/2 +5)
				ok=0;
		if(r > m/2)
			if(i < m/2)
				ok=0;

		if(i==1)
		{
			cout << "   |"; f << "   |";
			for(j=1; j<=n; j++)
			{
				for(int l=1; l <= (8-strlen(juc[j])); l++)
				{
					cout << ' '; f << ' ';

					if(l == (8-strlen(juc[j]))/2)
					{ cout << juc[j]; f << juc[j];  }
				}
				cout << '|'; f << '|';
			}
		}

		else if(i==2)
			for(j=1; j <= 4 + n*9; j++)
				{ cout << '-'; f << '-'; }

		else
		{
			if(ok)cout << ' '<< lista[i-2] << " |";
			f << ' '<< lista[i-2] << " |";

			for(j=1; j<=n; j++)
			{
				if(prima || zis[1][j][i-2] == -300)
					{ if(ok)cout << ' '; f << ' '; }
				else
					{ if(ok)cout << zis[1][j][i-2]; f << zis[1][j][i-2]; }

				if(zis[1][j][i-2]==9)
				{ if(ok)cout << '*'; f << '*';}
				else
					{ if(ok)cout << ' '; f << ' '; }

				if(ok)cout << " "; f << " ";
				if((r==0 || (r==1 && scor[j][i-2] == -300)) && i==3)
					{ if(ok)cout << " 0"; f << " 0"; }
				else if(scor[j][i-2] == -300)
					{ if(ok)cout << "  "; f << "  "; }
				else
				{
					if(scor[j][i-2] > 0)
						{ if(ok)cout << ' '; f << ' '; }
					if(ok)cout << scor[j][i-2]; f << scor[j][i-2];
				}
				if(scor[j][i-2]<9){ if(ok)cout<<' '; f<<' '; }
				if(scor[j][i-2]<99){ if(ok)cout<<' '; f<<' '; }
				if(ok)cout << " |"; f << " |";
			}
		}
		if(ok || i==1 || i==2)cout << endl; f << endl;
		ok=1;
	}
	if(ok || i==1 || i==2)cout << endl; f << endl;
	f.close();
	backup(2);
}

void postrunda()
{
	for(i=1; i<=n; i++)
	{
		cout << "Cate a facut " << juc[i] << "? ";
		cin >> facut[i];

		if(facut[i] == zis[1][i][r])
		{
			if(r==1)
				scor[i][r] = 0 + 5 + facut[i];
			else
				scor[i][r] = scor[i][r-1] + 5 + facut[i];
			streak[i] ++;

			if(streak[i] == 5)
			{
				scor[i][r] = scor[i][r-1] + 15 + facut[i];
				streak[i] = 0;
				zis[1][i][r] = 9;
			}
		}
		else
		{
			if(r==1)
				scor[i][r] = 0 - abs(zis[1][i][r] - facut[i]);
			else
				scor[i][r] = scor[i][r-1] - abs(zis[1][i][r] - facut[i]);
			streak[i] = 0;
		}
	}
}

void creare_lista(int opturi, int unuri)
{
	o=opturi; u=unuri;
	m = 2*opturi + unuri + 12;
	j = 7;

	for(i=1; i <= m; i++)
	{
		if(i<=opturi)
			lista[i] = lista[m-i+1] = 8;
		else if(i<=opturi+6)
		{
			lista[i] = lista[m-i+1] = j;
			j--;
		}
	}

	for(i=1; i <= m; i++)
		if(!lista[i]) lista[i] = 1;

	for(i=1; i<=7; i++)
		for(j=1; j<=33; j++)
			zis[1][i][j] = scor[i][j] = -300;
}

void cit()
{
	for(i=1; i<=n; i++)
	{
		cout << "Jucatorul nr " << i << ": ";
		cin >> juc[i];
		juc[i][0] = toupper(juc[i][0]);
	}
}


