#include <stdio.h>

int my_min(int x, int y) {
    if (x <= y) {
        return x;
    }
    return y;
}

int my_max(int x, int y) {
    if (x >= y) {
        return x;
    }
    return y;
}


int my_najdalej(int tab[][2], int n) {
    int d1, d2;
    int maks = 0;
    for (int p = 0; p < n - 2; p++) {
        for (int s = p + 1; s < n - 1; s++) {
            if (tab[p][0] != tab[s][0]) {
                d1 = tab[s][1] - tab[p][1];
                for (int k = s + 1; k < n; k++) {
                    if (tab[k][0] != tab[s][0] && tab[k][0] != tab[p][0]) {
                        d2 = tab[k][1] - tab[s][1];
                        if (my_min(d1, d2) > maks) {
                            maks = my_min(d1, d2);
                        }
                    }
                }
            }
        }
    }
    return maks;
}




static int najblizej(int tab[][2], int n) {
    int d1, d2;
    int mini = tab[n - 1][1] - tab[0][1] +1;
    for (int p = 0; p < n - 2; p++) {
        for (int s = p + 1; s < n - 1; s++) {
            if (tab[p][0] != tab[s][0]) {
                d1 = tab[s][1] - tab[p][1];
                for (int k = s + 1; k < n; k++) {
                    if (tab[k][0] != tab[s][0] && tab[k][0] != tab[p][0]) {
                        d2 = tab[k][1] - tab[s][1];
                        if (my_max(d1, d2) < mini) {
                            mini = my_max(d1, d2);
                        }
                    }
                }
            }
        }
    }
    if(mini == tab[n - 1][1] - tab[0][1] +1) return 0;
    return mini;
}



int main() {
    int n;
    scanf("%d", &n);
    int tab[n][2];
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &tab[i][0], &tab[i][1]);
    }

    int mini = najblizej(tab, n);
    printf("%d ", mini);

    int maks = my_najdalej(tab, n);
    printf("%d \n", maks);


    return 0;
}