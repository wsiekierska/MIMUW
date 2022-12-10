#include <stdio.h>
#include <mm_malloc.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

int scanfing;

bool iszero(double x){
    return fabs(x) < 1e-10;
}

///--------------------------------------------------struktura opisujaca punkt na plaszczyznie
typedef struct point {
    double x;
    double y;
} point;


///--------------------------------------------------struktura opisujaca pojedyncza kartke
typedef struct sheet {
    char type;
    point first;
    point second;
    double radius;
    int k;
} sheet;


///--------------------------------------------------struktura opisujaca poszczegolne zapytanie
typedef struct query {
    int k;
    point point;
} query;


///--------------------------------------------------funkcja okreslajaca polozenie punkt wzgledem prostej
double orientation(point a, point b, point c){
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
}


///--------------------------------------------------wczytanie danych do tablicy kartek oraz zapytan
void loading(sheet *tab, query *queries, int n, int q) {
    for (int i = 0; i < n; i++) {
        point Point_1 = {0, 0};
        sheet Sheet_1 = {0, Point_1, Point_1, 0, 0};
        scanfing = scanf(" %c", &Sheet_1.type);
        if (Sheet_1.type == 'P') {
            scanfing = scanf("%lf %lf", &Point_1.x, &Point_1.y);
            Sheet_1.first = Point_1;
            scanfing=scanf("%lf %lf", &Point_1.x, &Point_1.y);
            Sheet_1.second = Point_1;
        } else if (Sheet_1.type == 'K') {
            scanfing = scanf("%lf %lf", &Point_1.x, &Point_1.y);
            Sheet_1.first = Point_1;
            scanfing = scanf("%lf", &Sheet_1.radius);
        } else {
            scanfing=scanf("%d", &Sheet_1.k);
            scanfing=scanf("%lf %lf", &Point_1.x, &Point_1.y);
            Sheet_1.first = Point_1;
            scanfing=scanf("%lf %lf", &Point_1.x, &Point_1.y);
            Sheet_1.second = Point_1;
        }
        tab[i] = Sheet_1;
    }

    for (int i = 0; i < q; i++) {
        point pom2 = {0, 0};
        query pom = {0, pom2};
        scanfing=scanf("%d", &pom.k);
        scanfing=scanf("%lf %lf", &pom2.x, &pom2.y);
        pom.point = pom2;
        queries[i] = pom;
    }
}


///-------------------------------------------------- fukcja pomocnicza wyznaczajaca prostą przechodzącą przez dwa punkty
point line_determ(point begin, point end) {
    point line;
    line.x = (begin.y - end.y) / (begin.x - end.x);
    line.y = begin.y - (line.x * begin.x);
    return line;
}

///-------------------------------------------------- funkcja wyznaczająca symetrię osiową punktu względem prostej
///--------------------------------------------------przechodzącej przez dwa punkty
point symmetry(point x, point begin, point end) {
    point result;
    if (iszero(begin.x-end.x)) { // dla prostej pionowej
        result.y = x.y;
        result.x = x.x + 2 * (begin.x - x.x);
        return result;
    }
    if (iszero(begin.y-end.y)) { // dla prostej poziomej
        result.x = x.x;
        result.y = x.y + 2 * (begin.y - x.y);
        return result;
    } // ogólny przypadek
    point line = line_determ(begin, end); // wyznaczenie r-nia prostej
    point line_perp = {-1 / line.x, x.y - (line_perp.x * x.x)};  // r-nie prostej prostopadlej
    point middle = {(line_perp.y - line.y) / (line.x - line_perp.x), middle.x * line.x + line.y}; // punkt przeciecia prostych
    result.x = 2 * middle.x - x.x;
    result.y = 2 * middle.y - x.y;
    return result;
}


///--------------------------------------------------funkcja sprawdzająca czy punkt nalezy do danej kartki o postaci koła lub prostokąta
bool point_in(sheet paper, point x) {
    if (paper.type == 'P') { // dla kartki prostokątnej
        if (x.x > (paper.second).x || x.x < (paper.first).x) return false;
        if (x.y > (paper.second).y || x.y < (paper.first).y) return false;
        return true;
    }
    else { // dla kartki będącej kołem
        if (((paper.first).y - x.y) * ((paper.first).y - x.y) + ((paper.first).x - x.x) * ((paper.first).x - x.x) <=
            (paper.radius) * (paper.radius)){
            return true;
        }
        return false;
    }
}


///--------------------------------------------------funkcja obliczająca warstwy w danym punkcie
int layers(int k, point point1, sheet *tab) {
    if ((tab[k - 1]).type == 'K' || (tab[k - 1]).type == 'P') {    // sprawdzam czy punkt nalezy do kartki
        return point_in(tab[k - 1], point1);
    }
    int new_k = (tab[k - 1]).k; // numer kartki ktora jest skladana
    point P1=(tab[k - 1]).first, P2=(tab[k - 1]).second;  // punkty przez ktore skladamy
    point new_point = symmetry(point1, P1, P2);  // punkt symetryczny do sprawdzanego punktu wzgledem prostej, po ktorej skladamy kartkę
    if (iszero(orientation(P1, P2, point1))) { // jeśli punkt leży na zgięciu
        return layers((tab[new_k]).k, point1, tab);
    }
    if(orientation(P1, P2, point1)>0){ // jesli punkt lezy po lewej stronie wektora prostej
        return layers((tab[new_k]).k, point1, tab) + layers((tab[new_k]).k, new_point, tab); // suma warstw po obu stronach przed zlozeniem
    }
    return 0; // dla punktu leżącaego po prawej stronie wektora prostej
}


static void origami(sheet *tab, query *queries, int q) {
    int k;
    int result;
    point point1;
    for (int i = 0; i < (int)q; i++) {
        k = (queries[i]).k;
        point1 = (queries[i]).point;
        result = layers(k, point1, tab);
        printf("%d\n", result);
    }
}


int main() {
    int n, q;
    scanfing=scanf("%d %d", &n, &q);
    assert(scanfing);


    sheet *tab = (sheet*)malloc((size_t) n* sizeof(sheet));

    query *queries = (query*)malloc((size_t) q * sizeof(query));

    loading(tab, queries, n, q);
    origami(tab, queries, q);

    free(tab);
    free(queries);

    return 0;
}
