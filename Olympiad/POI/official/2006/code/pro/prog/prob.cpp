/*************************************************************************
 *                                                                       *
 *                   XIII Olimpiada Informatyczna                        *
 *                                                                       *
 *   Rozwi�zanie zadania: PRO (Profesor Szu)                             *
 *   Plik:                prob.cpp                                       *
 *   Autor:               Micha� Adamaszek                               *
 *   Opis:                Wykorzystuje DFS, ale nie usuwa najpierw       *
 *                        wierzcho�k�w z kt�rych nie da si� doj��]       *
 *                        do gmachu.                                     *
 *                                                                       *
 *************************************************************************/

#include <stdio.h>

#define N 1000001                /* Liczba domkow */
#define M 1000000                /* Liczba sciezek */
#define INF 36501

/* Reprezentacja grafu: tablica list sasiedztwa
   zaszyta w tablicy edge. Tablica firstEdge wskazuje
   na poczatek listy dla danego wierzcholka.

   Gmachowi przypisujemy numer 0 zamiast n+1
 */
typedef struct TEdge
{
    int v;        //wierzcholek dodelowy
    int next;        //indeks nastepnego elementu listy lub -1
};

TEdge edge[M];                                                                //8MB
int firstEdge[N];                                                        //4MB
int n,m;                //liczba wierzcholkow, krawedzi
char reachable[N];        //czy da sie stad dojsc do 0                        //1MB
int result[N];                //wynik                                                //4MB
char visited[N];        //czy odwiedzony w czasie DFS                        //1MB

#define NOTVIS 1        //nieodwiedzony
#define INVIS 2                //odwiedzany
#define VIS 3                //odwiedzony

/**
 Wczytanie danych.
 */
void input()
{
    int i, v, w;

    scanf("%d %d", &n, &m);

    //inicjalizacja struktur danych
    for(i = 0; i <= n; i++)
        firstEdge[i] = -1,
        reachable[i] = 1,
        visited[i] = NOTVIS,
        result[i] = 0;

    reachable[0] = 1;

    for(i = 0; i < m; i++)
    {
        scanf("%d %d", &v, &w);
        if (v == n+1) v = 0;
        if (w == n+1) w = 0;

        //zapamietujemy krawedz
        edge[i].v = w;
        edge[i].next = firstEdge[v];
        firstEdge[v] = i;
    }
}

/**
 Dodaje do wyniki w wierzcholku v wynik wierzcholka w
 */
void addResult(int v, int w)
{
    if (result[w] == INF)
        result[v] = INF;
    else
    {
        result[v] = result[v] + result[w];
        if (result[v] >= INF)
            result[v] = INF;
    }
}

/**
 Procedura odwiedzajaca woierzcholek w algorytmie DFS
 */
void DFS(int k)
{
    int v, pos;

    visited[k] = INVIS;

    pos = firstEdge[k];
    while (pos != -1)
    {
        v = edge[pos].v;
        if (reachable[v])
        {
            if (visited[v] == VIS)
            {
                addResult(k, v);
            }
            if (visited[v] == INVIS)
            {
                result[k] = INF;
            }
            if (visited[v] == NOTVIS)
            {
                DFS(v);
                addResult(k, v);
            }
        }
        pos = edge[pos].next;
    }

    visited[k] = VIS;
}

/**
 Znajdz maksimum (rozpatrujemy tylko wierzcholki osiagalne)
 i wypisz wynik
 */
void print()
{
    int i, maxFound, maxCount;

    maxFound = 0;
    maxCount = 0;

    //znajdz max
    for(i = 1; i <= n; i++)
        if (reachable[i])
            if (result[i] > maxFound)
                maxFound = result[i];

    //znajdz liczbe maksow
    for(i = 1; i <= n; i++)
        if (reachable[i] && result[i] == maxFound)
            maxCount++;

    //wypisz
    if (maxFound == INF)
        printf("zawsze\n");
    else printf("%d\n", maxFound);

    printf("%d\n", maxCount);

    for(i = 1; i <= n; i++)
        if (reachable[i] && result[i] == maxFound)
            printf("%d ", i);
    printf("\n");
}

/**
 Program glowny
 */
int main()
{
    input();

    result[0] = 1;
    for(int i = 1; i <= n; i++)
        if (reachable[i] == 1 && visited[i] == NOTVIS)
            DFS(i);

    print();

    return 0;
}
