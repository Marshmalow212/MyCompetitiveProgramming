/*************************************************************************
 *                                                                       *
 *                     XV  Olimpiada Informatyczna                       *
 *                                                                       *
 *   Zadanie:  Robinson (ROB)                                            *
 *   Plik:     robs8.c                                                   *
 *   Autor:    Jakub Radoszewski                                         *
 *   Opis:     Brutalny BFS o zlozonosci O(n^4) z wykorzystaniem STL-a   *
 *             polaczony z heurystyka z programu robb1.cpp.              *
 *             Przepisanie robs3.cpp na C.                               *
 *                                                                       *
 *************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 2000

char lodz[MAX_N][MAX_N + 1];
int n;
int odc[MAX_N]; /* odcinki skladajace sie na lodke */
int odcsize;

int minx, miny, maxx, maxy;
int sr; /* srodek lodki */
int polsz;

int min(int a, int b)
{
  if (a < b)
    return a;
  else
    return b;
}

int max(int a, int b)
{
  if (a < b)
    return b;
  else
    return a;
}

/* Sprawdzanie poprawnosci ulozenia lodki */
int lodka()
{
  int i, j;
  minx = n; miny = n; maxx = -1; maxy=-1;
  odcsize = 0;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      if (lodz[i][j] == 'r')
      {
        minx = min(minx, i);
        maxx = max(maxx, i);
        miny = min(miny, j);
        maxy = max(maxy, j);
      }
  sr = (maxx + minx) / 2;
  polsz = (maxx - minx) / 2;
  for (j = miny; j <= maxy; j++)
  {
    int i1 = 0, i2 = 0; /* ile kawalkow pod osia symetrii, a ile nad */
    for (i = minx; i <= maxx; i++)
    {
      if (lodz[i][j] == 'r')
      {
        if (i < sr)
          i1++;
        if (i > sr)
          i2++;
      }
    }
    if (i1 != i2)
      /* Lodka nie jest symetryczna */
      return 0;
    odc[odcsize++] = i1;
  }
  return 1;
}

struct pair
{
  short int first, second;
};

struct pair make_pair(int a, int b)
{
  struct pair p;
  p.first = (short int)a; 
  p.second = (short int)b;
  return p;
}

inline int ins(int a, int m)
{
  return a >= 0 && a < m;
}

/* Czy lodka moze miec poczatek w punkcie (a,b). */
inline int mozliwa(int a, int b)
{
  int i, j;
  for (j = 0; j <= maxy - miny; j++)
  {
    if (!ins(b + j - n, n))
      continue;
    for (i = 0; i <= odc[j]; i++)
    {
      if (ins(a + i - n, n) && lodz[a + i - n][b + j - n] == 'X')
        return 0;
      if (ins(a - i - n, n) && lodz[a - i - n][b + j - n] == 'X')
        return 0;
    }
  }
  return 1;
}


int sumy[MAX_N][MAX_N];

/* Liczy liczby pol 'X' w prostokatach o jednym z wierzcholkow
 * rownym (0,0). */
void dynamik()
{
  int i, j;
  for (i = 0; i < n; i++)
  {
    int akt = 0;
    for (j = 0; j < n; j++)
    {
      if (lodz[i][j] == 'X')
        akt++;
      sumy[i][j] = akt + (i ? sumy[i - 1][j] : 0);
    }
  }
}

/* Zwraca sume X-ow z odpowiedniego prostokata. */
inline int policz(int a, int b)
{
  a -= n; b -= n;
  a = min(a, n - 1); b = min(b, n - 1);
  if (a < 0 || b < 0)
    return 0;
  return sumy[a][b];
}

/* Czy lodka moze miec poczatek w punkcie (a,b) - zakladamy, ze jest
 * idealnym prostokatem. */
inline int mozliwa1(int a, int b)
{
  return (policz(a + polsz, b + maxy - miny) - policz(a + polsz, b - 1)
    - policz(a - polsz - 1, b + maxy - miny) + policz(a - polsz - 1, b - 1)) == 0;
}


struct pair kol[6 * MAX_N * MAX_N];
int head, tail;

int odl[3 * MAX_N][2 * MAX_N];

int x[] = {1, -1, 0, 0};
int y[] = {0, 0, 1, -1};

/* Czy lodka juz wyplynela? */
inline int wyplynela(int a, int b)
{
  int i, j;
  for (j = 0; j <= maxy - miny; j++)
  {
    if (!ins(b + j - n, n))
      continue;
    for (i = 0; i <= odc[j]; i++)
    {
      if (ins(a + i - n, n))
        return 0;
      if (ins(a - i - n, n))
        return 0;
    }
  }
  return 1;
}

/* BFS zaczynajacy od punktu (x0,y0). */
int bfs(int x0, int y0)
{
  int i, j, a, b;
  for (i = 0; i < 3 * n; i++)
    for (j = 0; j < 2 * n; j++)
      odl[i][j] = -1;
  odl[x0][y0] = 0;
  kol[0] = make_pair(x0, y0);
  head = 0; tail = 1;

  while (head != tail)
  {
    struct pair p = kol[head];
    head++;
    for (i = 0; i < 4; i++)
    {
      a = p.first + x[i]; b = p.second + y[i];
      /* Wyszlismy poza plansze - sukces */
      if (wyplynela(a, b))
        return odl[p.first][p.second] + 1;
      if (odl[a][b] == -1 && (mozliwa1(a, b) || mozliwa(a, b)))
      {
        odl[a][b] = odl[p.first][p.second] + 1;
        kol[tail++] = make_pair(a, b);
      }
    }
  }
  return -1;
}

int main()
{
  int i, j, wynik;
  scanf("%d", &n);
  for (i = 0; i < n; i++)
    scanf("%s", lodz[i]);
  if (!lodka()) /* czy lodka zorienowana poziomo */
  {
    /* Odbij wszystko wzgledem osi y=x */
    for (i = 0; i < n; i++)
      for (j = 0; j < i; j++)
      {
        char c = lodz[i][j];
        lodz[i][j] = lodz[j][i];
        lodz[j][i] = c;
      }
    lodka(); /* no to lodka zorientowana pionowo */
  }
  dynamik();
  wynik = bfs(n + sr, n + miny);
  if (wynik < 0)
    puts("NIE");
  else
    printf("%d\n", wynik);
  return 0;
}
