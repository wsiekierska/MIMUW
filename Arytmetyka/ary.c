#include "ary.h"
#include <math.h>

///------------------------------------------------------------ funkcje pomocnicze

static double const pos_inf = HUGE_VAL;
static double const neg_inf = -HUGE_VAL;
static const double myEps = 1e-10;

static bool myEqual(double x, double y) {
    return fabs(x - y) < myEps;
}

static double min(double x, double y) {
    if (x <= y)
        return x;
    return y;
}

static double max(double x, double y) {
    if (x >= y)
        return x;
    return y;
}

///------------------------------------------------------------ konstruktory

wartosc wartosc_dokladnosc(double x, double p) {
    wartosc w;
    double procent = p / 100;

    w.a = min((x - (procent * x)), (x + (procent * x)));
    w.b = max((x - (procent * x)), (x + (procent * x)));
    w.dopelnienie = false;

    return w;
}

wartosc wartosc_od_do(double x, double y) {
    wartosc w;

    if (myEqual(x, neg_inf)) {
        w.a = neg_inf;
    } else {
        w.a = (x + y) / 2 - (y - x) / 2;
    }
    if (myEqual(y, pos_inf)) {
        w.b = pos_inf;
    } else {
        w.b = (x + y) / 2 + (y - x) / 2;
    }
    w.dopelnienie = false;

    return w;
}

wartosc wartosc_dokladna(double x) {
    wartosc w;
    w.a = x;
    w.b = x;
    w.dopelnienie = false;

    return w;
}

///------------------------------------------------------------ modyfikatory

bool in_wartosc(wartosc x, double y) {
    if (isnan(x.a) && isnan(x.b)) {
        return false;
    }
    if (x.dopelnienie) {
        if (y < x.a || myEqual(y, x.a)) {
            return true;
        }
        if (y > x.b || myEqual(y, x.b)) {
            return true;
        }
        return false;
    }
    if ((x.a < y || myEqual(x.a, y))) {
        if (x.b > y || myEqual(x.b, y)) {
            return true;
        }
    }

    return false;
}

double min_wartosc(wartosc x) {
    if (!x.dopelnienie) {
        if (isnan(x.a) && isnan(x.b)) {
            return NAN;
        }
        return x.a;
    }

    return neg_inf;
}

double max_wartosc(wartosc x) {
    if (!x.dopelnienie) {
        if (isnan(x.a) && isnan(x.b)) {
            return NAN;
        }
        return x.b;
    }

    return pos_inf;
}

double sr_wartosc(wartosc x) {
    if (isnan(min_wartosc(x)) || isnan(max_wartosc(x))) {
        return NAN;
    }
    if (myEqual(min_wartosc(x), neg_inf) && myEqual(max_wartosc(x), pos_inf)) {
        return NAN;
    }
    if (myEqual(min_wartosc(x), neg_inf)) {
        return neg_inf;
    }
    if (myEqual(max_wartosc(x), pos_inf)) {
        return pos_inf;
    }

    return (min_wartosc(x) + max_wartosc(x)) / 2;
}

////------------------------------------------------------------ selektory

wartosc plus(wartosc a, wartosc b) {
    if ((isnan(a.a) && isnan(a.b)) || (isnan(b.a) && isnan(b.b))) {
        wartosc w = {.a=NAN, .b=NAN, .dopelnienie=0};
        return w;
    }
    if (!a.dopelnienie && !b.dopelnienie) {
        wartosc w;
        if (myEqual(max_wartosc(a), pos_inf) || myEqual(max_wartosc(b), pos_inf)) {
            w.b = pos_inf;
        } else {
            w.b = a.b + b.b;
        }
        if (myEqual(min_wartosc(a), neg_inf) || myEqual(min_wartosc(b), neg_inf)) {
            w.a = neg_inf;
        } else {
            w.a = a.a + b.a;
        }
        w.dopelnienie = false;
        return w;
    }
    if (a.dopelnienie && !b.dopelnienie) {
        wartosc w;
        w.a = a.a + b.b;
        w.b = a.b + b.a;

        if (w.a >= w.b) {
            w.a = neg_inf;
            w.b = pos_inf;
            w.dopelnienie = false;
        } else {
            w.dopelnienie = true;
        }
        return w;
    }
    if (b.dopelnienie && !a.dopelnienie) {
        wartosc w;
        w.a = a.b + b.a;
        w.b = a.a + b.b;

        if (w.a >= w.b) {
            w.a = neg_inf;
            w.b = pos_inf;
            w.dopelnienie = false;
        } else {
            w.dopelnienie = true;
        }
        return w;
    }

    wartosc w;
    w.a = neg_inf;
    w.b = pos_inf;
    w.dopelnienie = false;

    return w;
}

wartosc razy_bez_dopelnienia(wartosc a, wartosc b) {
    wartosc w;
    w.dopelnienie = false;

    if (a.a >= 0 && b.a >= 0) {
        if (myEqual(a.a, 0) || myEqual(b.a, 0)) {
            w.a = 0;
        }
        if (myEqual(a.b, 0) || myEqual(b.b, 0)) {
            w.b = 0;
        } else {
            w.a = a.a * b.a;
            w.b = a.b * b.b;
        }
        return w;
    }
    if (min_wartosc(a) >= 0 && min_wartosc(b) < 0) {
        if (myEqual(a.b, 0)) {
            w.a = 0;
            w.b = 0;
            return w;
        } else {
            w.a = a.b * b.a;
        }
        if (max_wartosc(b) >= 0) {
            w.b = max(a.b * b.b, a.a * b.b);
        } else {
            w.b = a.a * b.b;
        }
        return w;
    }
    if (min_wartosc(a) < 0 && min_wartosc(b) >= 0) {
        w.a = b.b * a.a;
        w.b = b.a * a.b;

        if (max_wartosc(a) >= 0) {
            w.b = b.b * a.b;
        }
        return w;
    }
    if (max_wartosc(a) < 0 && max_wartosc(b) < 0) {
        w.b = a.a * b.a;
        w.a = a.b * b.b;
        return w;
    }
    if (max_wartosc(a) >= 0 && max_wartosc(b) < 0) {
        w.a = a.b * b.a;
        w.b = a.a * b.a;
        return w;
    }
    if (max_wartosc(a) < 0 && max_wartosc(b) >= 0) {
        w.a = b.b * a.a;
        w.b = b.a * a.a;
        return w;
    }

    w.a = min(a.b * b.a, a.a * b.b);
    w.b = max(a.a * b.a, a.b * b.b);

    return w;
}

wartosc razy_z_dopelnieniem(wartosc a, wartosc b) {
    wartosc w;

    if (a.dopelnienie && !b.dopelnienie) {
        w.a = max(a.a * b.b, a.a * b.a);
        w.b = min(a.b * b.a, a.b * b.b);
        w.dopelnienie = true;

        if (w.a > w.b) {
            double pom = w.a;
            w.a = w.b;
            w.b = pom;
        }
    } else if (b.dopelnienie == true && !a.dopelnienie) {
        w.a = max(b.a * a.b, b.a * a.a);
        w.b = min(b.b * a.a, b.b * a.b);
        w.dopelnienie = true;

        if (w.a >= w.b) {
            double pom = w.a;
            w.a = w.b;
            w.b = pom;
        }
    } else {
        w.a = max(a.a * b.b, a.b * b.a);
        w.b = min(a.a * b.a, a.b * b.b);
        w.dopelnienie = true;
    }

    return w;
}

wartosc razy(wartosc a, wartosc b) {
    wartosc w;

    if (isnan(b.a) || isnan(a.a)) {
        w.a = NAN;
        w.b = NAN;
        w.dopelnienie = false;
        return w;
    }
    if ((myEqual(a.a, 0) && myEqual(a.b, 0)) ||
        (myEqual(b.a, 0) && myEqual(b.b, 0))) {
        w.a = 0;
        w.b = 0;
        w.dopelnienie = false;
        return w;
    }
    if (!a.dopelnienie && !b.dopelnienie) {
        return razy_bez_dopelnienia(a, b);
    }

    return razy_z_dopelnieniem(a, b);
}

///--------------------------------------------- zwraca przedzial zawierajacy liczby przeciwne do liczb z X

wartosc przeciwnosc(wartosc x) {
    return razy(x, wartosc_dokladna(-1));
}

wartosc minus(wartosc a, wartosc b) {
    return plus(a, przeciwnosc(b));
}

///--------------------------------------------- zwraca przedzial zawierajacy liczby odwrotne do liczb z X

wartosc odwrotnosc(wartosc x) {
    wartosc w;

    if (isnan(x.a)) {
        w.a = NAN;
        w.b = NAN;
        w.dopelnienie = false;
    } else if (!x.dopelnienie) {
        if (myEqual(x.a, x.b)) {
            if (myEqual(x.a, 0)) {
                w.a = NAN;
                w.b = NAN;
            } else {
                w.a = 1.0 / x.a;
                w.b = w.a;
            }
            w.dopelnienie = false;
        } else if (myEqual(max_wartosc(x), pos_inf)) {
            w.dopelnienie = false;

            if (myEqual(min_wartosc(x), neg_inf)) {
                w.a = neg_inf;
                w.b = pos_inf;
                return w;
            } else if (myEqual(x.a, 0)) {
                w.a = 0;
                w.b = pos_inf;
            } else if (x.a > 0) {
                w.a = 0;
                w.b = 1.0 / x.a;
            } else {
                w.a = neg_inf;
                w.b = pos_inf;
            }
        } else if (myEqual(min_wartosc(x), neg_inf)) {
            w.dopelnienie = false;

            if (myEqual(x.b, 0)) {
                w.a = neg_inf;
                w.b = 0;
            } else if (x.b < 0) {
                w.a = 1.0 / x.b;
                w.b = 0;
            } else {
                w.a = neg_inf;
                w.b = pos_inf;
            }
        } else if (myEqual(x.a, 0)) {
            w.dopelnienie = false;
            w.b = pos_inf;
            w.a = 1.0 / x.b;
        } else if (myEqual(x.b, 0)) {
            w.b = 1.0 / x.a;
            w.a = neg_inf;
            w.dopelnienie = false;
        } else if (x.a < 0 && x.b > 0) {
            w.a = 1.0 / x.a;
            w.b = 1.0 / x.b;
            w.dopelnienie = true;
        } else {
            w.a = min(1.0 / x.a, 1.0 / x.b);
            w.b = max(1.0 / x.a, 1.0 / x.b);
            w.dopelnienie = false;
        }
    } else {
        if (myEqual(x.a, 0)) {
            w.a = neg_inf;
        } else {
            w.a = 1.0 / x.a;
        }
        if (myEqual(x.b, 0)) {
            w.b = pos_inf;
        } else {
            w.b = 1.0 / x.b;
        }
        w.dopelnienie = false;
    }

    return w;
}

wartosc podzielic(wartosc a, wartosc b) {
    return razy(a, odwrotnosc(b));
}