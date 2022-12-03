#include <stdio.h>
#include <stdlib.h>


static int max(int x, int y) {
    if (x >= y) {
        return x;
    }
    return y;
}

static int min(int x, int y) {
    if (x <= y) {
        return x;
    }
    return y;
}


///---------------------------------------------------------- funkcja znajduje trzy pierwsze i ostatnie wystąpienia różnych sieci hoteli

static void skrajne(int **tab, int n, int indeksy[]) {
    int i = 0;
    indeksy[0] = 0;
    while (tab[i][0] == tab[0][0] && i < n - 1) i++;
    indeksy[1] = i;

    while ((tab[0][0] == tab[i][0] || tab[indeksy[1]][0] == tab[i][0]) && i < n - 1) i++;
    indeksy[2] = i;

    i = n - 1;
    indeksy[5] = n - 1;

    while ((tab[i][0] == tab[n - 1][0]) && i > 0) i--;
    indeksy[4] = i;

    while ((tab[n - 1][0] == tab[i][0] || tab[indeksy[4]][0] == tab[i][0]) && i > 0) i--;
    indeksy[3] = i;

}


// srodek - indeks hotelu znajdujacego sie miedzy pierwszym a ostatnim hotelem
///---------------------------------------------------------- funkcja zwraca minimum z odleglosci od skrajnych hoteli dla znalezionego srodka

static int szukaj_srodek(int **tab, int p, int k) {
    int maks = 0, d1, d2;
    for (int i = p + 1; i < k; i++) {
        if (tab[i][0] != tab[p][0] && tab[i][0] != tab[k][0]) {
            d1 = tab[i][1] - tab[p][1];
            d2 = tab[k][1] - tab[i][1];
            if (min(d1, d2) > maks) {
                maks = min(d1, d2);
            }
        }
    }
    return maks;
}

///---------------------------------------------------------- funkcja zwraca minimum z odleglosci pomiedzy hotelami dla najdalszej trojki

static int najdalej(int **tab, int n) {
    int indeksy[6] = {0};
    skrajne(tab, n, indeksy);
    int wynik = 0, maks;
    for (int i = 0; i < 3; i++) {
        for (int j = 5; j > 2; j--) {
            if (indeksy[i] < indeksy[j] && tab[indeksy[i]][0] != tab[indeksy[j]][0]) {
                maks = szukaj_srodek(tab, indeksy[i], indeksy[j]);
                if (maks > wynik) wynik = maks;
            }
        }

    }
    return wynik;
}

///---------------------------------------------------------- funkcja zwraca maksimum z odleglosci od skrajnych hoteli dla znalezionego srodka

static int szukaj_min(int **tab, int p, int k) {
    int maks = tab[k][1] - tab[p][1];
    int d1, d2;
    for (int i = p + 1; i < k; i++) {
        d1 = tab[i][1] - tab[p][1];
        d2 = tab[k][1] - tab[i][1];
        if (max(d1, d2) < maks)
            maks = max(d1, d2);
    }
    return maks;
}


///---------------------------------------------------------- funkcja zwraca maksimum z odleglosci pomiedzy hotelami dla najblizszej trojki

static int najblizej(int **tab, int n) {
    int p = 0, k = 2;
    int maks = tab[n - 1][1] - tab[0][1] + 1;
    while (k < n) {
        if (tab[k - 1][0] != tab[k][0]) {
            if (tab[k - 1][0] != tab[p][0] && tab[k][0] != tab[p][0]) {
                if (szukaj_min(tab, p, k) < maks)
                    maks = szukaj_min(tab, p, k);
            }
            p = k - 1;
        }
        k++;
    }
    if (maks == tab[n - 1][1] - tab[0][1] + 1) {
        return 0;
    }
    return maks;
}



int main() {
    int n;
    scanf("%d", &n);

    int **tab;
    tab = malloc((long unsigned int) n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        tab[i] = malloc(2 * sizeof(int));
        scanf("%d %d", &tab[i][0], &tab[i][1]);
    }

    printf("%d ", najblizej(tab, n));
    printf("%d \n", najdalej(tab, n));

    free(tab);

    return 0;
}
