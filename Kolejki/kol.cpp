#include <cstdlib>
#include "kol.h"


using namespace std;

namespace {

    typedef struct lista {
        interesant *head, *tail;
    } lista;

    lista create_list() {
        lista res;
        res.head = (interesant *) malloc(sizeof(interesant));
        res.tail = (interesant *) malloc(sizeof(interesant));
        res.head->l1 = NULL;
        res.head->l2 = res.tail;
        res.tail->l1 = NULL;
        res.tail->l2 = res.head;
        return res;
    }

    bool isempty(const lista l) {
        return l.head->l2 == l.tail;
    }

    interesant *front(const lista l) {
        return l.head->l2;
    }

    interesant *back(const lista l) {
        return l.tail->l2;
    }

    void link(interesant *el, interesant *old, interesant *nw) {
        if (el) {
            if (el->l1 == old) {
                el->l1 = nw;
            } else {
                el->l2 = nw;
            }
        }
    }


    void push_back(lista &l, interesant *i) {
        // laczy "i" z (poprzednim) ostatnim elementem oraz tailem
        i->l2 = l.tail;
        i->l1 = l.tail->l2;
        // laczy tail z "i" oraz przedostatni element z "i"
        link(l.tail->l2, l.tail, i);
        l.tail->l2 = i;
    }

    void remove_interesant(interesant *el) {
        link(el->l1, el, el->l2);
        link(el->l2, el, el->l1);
    }

    void pop_front(lista &l) {
        if (!isempty(l)) {
            remove_interesant(l.head->l2);
        }
    }

    void reverse(lista &l) {
        interesant *tmp = l.head;
        l.head = l.tail;
        l.tail = tmp;
    }

    void append(lista &l1, lista &l2) {
        interesant *b = l1.tail;
        interesant *f = l2.head;
        link(b->l2, b, f->l2);
        link(f->l2, f, b->l2);
        b->l2 = f;
        f->l2 = b;
        l1.tail = l2.tail;
        l2.head = f;
        l2.tail = b;
    }

    vector<lista> kolejki;

    int akt_numerek;
}

void otwarcie_urzedu(int m) {
    kolejki.resize(m);
    for (int i = 0; i < m; ++i)
        kolejki[i] = create_list();
    akt_numerek = 0;
}

interesant* nowy_interesant(int k) {
    interesant *i = (interesant*)malloc(sizeof(interesant));
    i->val = akt_numerek++;
    push_back(kolejki[k], i);
    return back(kolejki[k]);
}

int numerek(interesant* i) {
    return i->val;
}

interesant* obsluz(int k) {
    if (isempty(kolejki[k]))
        return NULL;
    interesant* i = front(kolejki[k]);
    pop_front(kolejki[k]);
    return i;
}

void zmiana_okienka(interesant* i, int k) {
    remove_interesant(i);
    push_back(kolejki[k], i);
}

void zamkniecie_okienka(int k1, int k2) {
    if (k1 == k2) return;
    append(kolejki[k2], kolejki[k1]);
}

vector<interesant*> fast_track(interesant* i1, interesant* i2) {
    vector<interesant*> interesants1, interesants2;
    interesant* k1 = i1, * k2 = i1;
    bool dir1 = 1, dir2 = 0; // przez l1 w "lewo", przez l2 "w lewo"
    interesants1.push_back(i1); //przechodze przez l1 "w prawo"
    interesants2.push_back(i1); //przechodze przez l2 "w lewo"
    while (k1 != i2 && k2 != i2) {
        if (dir1 == 1) { //przechodzę po liście w "prawo"
            if (k1->l1 != NULL){
                interesants1.push_back(k1->l1);
                if (k1->l1->l1 == k1) dir1 = 0; //sprawdzam czy l1 czy l2 jest następnikiem, jak l2 to zmieniam kierunek
                k1 = k1->l1;
            }
        }
        else { //przechodze w "lewo"
            interesants1.push_back(k1->l2);
            if (k1->l2->l2 == k1) dir1 = 1; //sprawdzam czy l1 czy l2 jest poprzednikiem, jak l1 to zmieniam kierunek
            k1 = k1->l2;
        }
        if (dir2 == 1) { //przechodzę po liście w "prawo", analogicznie
            if (k2->l1 != NULL){
                interesants2.push_back(k2->l1);
                if (k2->l1->l1 == k2) dir2 = 0;
                k2 = k2->l1;
            }
        }
        else { //przechodze w "lewo", analogicznie
            interesants2.push_back(k2->l2);
            if (k2->l2->l2 == k2) dir2 = 1;
            k2 = k2->l2;
        }
    }
    if (k1 == i2) { // jeżeli z którejś strony dochodzimy do l2
        if (dir1 == 1) { // na czym skonczylismy
            link(i1->l2, i1, i2->l1); //łączenie poprzednika i1 z nastepnikiem i2 (poprzednikiem jest l2)
            link(i2->l1, i2, i1->l2); //łączenie nastepnika i2 z poprzednikiem i1
        }
        else {
            link(i1->l2, i1, i2->l2); //łączenie poprzednika i1 z nastepnikiem i2 (poprzednikiem jest l1)
            link(i2->l2, i2, i1->l2); //łączenie nastepnika i2 z poprzednikiem i1
        }
        return interesants1;
    }
    else {
        if (dir2 == 1) {
            link(i1->l1, i1, i2->l1); //łączenie poprzednika i1 z nastepnikiem i2 (poprzednikiem jest l2)
            link(i2->l1, i2, i1->l1); //łączenie nastepnika i2 z poprzednikiem i1
        }
        else {
            link(i1->l1, i1, i2->l2); //łączenie poprzednika i1 z nastepnikiem i2 (poprzednikiem jest l1)
            link(i2->l2, i2, i1->l1); //łączenie nastepnika i2 z poprzednikiem i1
        }
        return interesants2;
    }
}

void naczelnik(int k) {
    reverse(kolejki[k]);
}

vector<interesant*> zamkniecie_urzedu() {
    vector<interesant*> wszyscy;
    for (int i = 0; i < (int)kolejki.size(); i++){
        while (!isempty(kolejki[i])) {
            wszyscy.push_back(front(kolejki[i]));
            pop_front(kolejki[i]);
        }
        free(kolejki[i].head);
        free(kolejki[i].tail);
    }
    return wszyscy;
}
