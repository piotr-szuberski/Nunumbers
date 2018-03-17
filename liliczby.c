/* Piotr Szuberski, p.szuberski@student.uw.edu.pl
   Program zalcizeniowy nr 3 - Liliczby
   Program wczytuje z wejścia dwie liliczby zapisane
   znakami Y i Z oraz oblicza ich iloczyn. */

#include <stdio.h>
#include <stdlib.h>

#define Y 1
#define Z 0
#define MNIEJSZA (-1)
#define WIEKSZA 1
#define ROWNE 0

struct liliczba {
    int wartosc;
    struct liliczba *wykladnik;
    struct liliczba *bok;
};
typedef struct liliczba Tliliczba;

/*Funkcja nr 1: wczytuje jedną liliczbę */
Tliliczba *wczytanie() {
    int x;
    Tliliczba *l = malloc(sizeof *l);
    x = getchar();
    if (x == 'Z') {
        l->wartosc = Z;
        l->bok = l->wykladnik = NULL;
    } else {
        l->wartosc = Y;
        l->wykladnik = wczytanie();
        l->bok = wczytanie();
    }
    return l;
}

/* Funkcja nr 2: wypisuje liliczbę na wyjście */
void pisz(Tliliczba *l) {
    if (l->wartosc == Z)
        printf("Z");
    else {
        printf("Y");
        pisz(l->wykladnik);
        pisz(l->bok);
    }
}

/* Funkcja nr 3: kasuje liliczbę, uwalniając zaalokowane miejsce */
void usun_liczbe(Tliliczba *x) {
    if (x->wartosc == Z) {
        free(x);
    } else {
        usun_liczbe(x->wykladnik);
        usun_liczbe(x->bok);
        free(x);
    }
}

/* Funkcja nr 4: kasuje segment liliczby */
void usun_segment(Tliliczba *x) {
    if (x->wartosc == Z)
        free(x);
    else {
        usun_liczbe(x->wykladnik);
        free(x);
    }
}

/* Funkcja nr 4: porównuje ze sobą dwie liliczby */
int porownaj_liczbe(Tliliczba *a, Tliliczba *b) {
    int porownanie;

    if (a->wartosc == Z && b->wartosc == Z) {
        porownanie = ROWNE;
    } else if (a->wartosc == Z) {
        porownanie = MNIEJSZA;
    } else if (b->wartosc == Z) {
        porownanie = WIEKSZA;
    } else {
        porownanie = porownaj_liczbe(a->wykladnik, b->wykladnik);
        if (porownanie == ROWNE) {
            porownanie = porownaj_liczbe(a->bok, b->bok);
        } else if (porownanie == MNIEJSZA) {
            porownanie = porownaj_liczbe(a->bok, b);
            if (porownanie == ROWNE)
                porownanie = WIEKSZA;
        } else if (porownanie == WIEKSZA) {
            porownanie = porownaj_liczbe(a, b->bok);
            if (porownanie == ROWNE)
                porownanie = MNIEJSZA;
        }
    }
    return porownanie;
}

/* Funkcja nr 5: porównująca segmenty liliczby.
   Wartość wyniku dotyczy liliczby "a".
*/
int porownaj_segment(Tliliczba *a, Tliliczba *b) {
    int porownanie;

    if (a->wartosc == Z && b->wartosc == Z)
        porownanie = ROWNE;
    else if (a->wartosc == Z)
        porownanie = MNIEJSZA;
    else if (b->wartosc == Z)
        porownanie = WIEKSZA;
    else
        porownanie = porownaj_liczbe(a->wykladnik, b->wykladnik);

    return porownanie;
}

/* Funkcja nr 6: dodaje wartość 1 do liliczby */
void dodaj_jeden(Tliliczba *x) {
    Tliliczba *nowy1, *nowy2;

    while (x->wartosc != Z)
        x = x->bok;

    nowy1 = malloc(sizeof *nowy1);
    nowy2 = malloc(sizeof *nowy2);
    nowy2->wykladnik = nowy2->bok = nowy1->wykladnik = nowy1->bok = NULL;
    nowy2->wartosc = nowy1->wartosc = Z;
    x->wartosc = Y;
    x->wykladnik = nowy1;
    x->bok = nowy2;
}

/* Funkcja nr 7: zamienia wykładniki liliczb */
void zamien(Tliliczba *a, Tliliczba *b) {
    Tliliczba *tmp;
    tmp = a->wykladnik;
    a->wykladnik = b->wykladnik;
    b->wykladnik = tmp;
}

/* Funkcja nr 8: normalizująca liliczby.
   Jeśli liliczba równa się 0, to nic się nie dzieje - bazowy przypadek.
   Jeśli segment liliczby po prawej stronie jest większy - nic
   się nie dzieje.
   Jeśli segmenty są równe - segment po prawej stronie jest kasowany,
   a wykładnik segmentu po lewej zwiększa się o 1 i jest normalizowany.
   Jeśli segment po lewej stronie jest większy - jest zamieniany miejscami
   z prawym do momentu, w którym segment po prawej będzie większy. */
void *normalizacja(Tliliczba *a) {
    int x;
    Tliliczba *tmp;

    if (a->wartosc == Z)
        return a;

    normalizacja(a->wykladnik);
    do {
        normalizacja(a->bok);

        if (a->bok->wartosc == Z)
            x = MNIEJSZA;
        else
            x = porownaj_segment(a, a->bok);

        if (x == ROWNE) {
            tmp = a->bok;
            a->bok = tmp->bok;
            tmp->bok = NULL;
            usun_segment(tmp);
            dodaj_jeden(a->wykladnik);
            normalizacja(a);
        } else if (x == WIEKSZA) {
            zamien(a, a->bok);
        }
    } while (x == WIEKSZA);
    return a;
}

/* Funkcja nr 9: dodająca do siebie liliczby - łączy boki
   liliczb i kasuje Z jednej z nich, następnie je normalizuje */
Tliliczba *dodawanie(Tliliczba *a, Tliliczba *b) {
    Tliliczba *suma, *koniec, *poprz;
    if (a->wartosc == Z) {
        free(a);
        return b;
    } else if (b->wartosc == Z) {

        free(b);
        return a;
    } else {
        poprz = a;
        koniec = a->bok;
        while (koniec->wartosc != Z) {
            koniec = koniec->bok;
            poprz = poprz->bok;
        }
        free(koniec);
        suma = a;
        poprz->bok = b;
        normalizacja(suma);
        return suma;
    }

}

/* Funkcja nr 10: kopiuje liliczbę, alokując nowe miejsca w pamięci */
Tliliczba *kopiuj(Tliliczba *x) {
    Tliliczba *kopia;
    kopia = (Tliliczba *) malloc(sizeof *kopia);
    if (x->wartosc == Z) {
        kopia->wartosc = Z;
        kopia->wykladnik = kopia->bok = NULL;
    } else {
        kopia->wartosc = Y;
        kopia->wykladnik = kopiuj(x->wykladnik);
        kopia->bok = kopiuj(x->bok);
    }
    return kopia;
}

/* Funkcja nr 11: mnoży dwie liliczby.
   Jeśli minimum jedna z liliczb wynosi 0 - wynik jest 0.
   Jeśli żadna nie jest zerem, to wymnażane są kolejno
   cyfry liliczby poprzez skopiowanie wykładników, następnie
   dodaniu ich do siebie i utworzeniu pamięci na pierwsze Y
   i połączeniu ich w jedną liliczbę, następnie znormalizowaniu
   jej i dodaniu do wykladnika Y. */
Tliliczba *mnozenie(Tliliczba *a, Tliliczba *b) {
    Tliliczba *tmpb, *tmpa, *poma, *pomb;
    Tliliczba *iloczyn, *poczatek, *nowy1, *nowy2;
    iloczyn = malloc(sizeof *iloczyn);
    poczatek = iloczyn;
    if (a->wartosc == Z || b->wartosc == Z) {
        iloczyn->wartosc = Z;
        iloczyn->bok = iloczyn->wykladnik = NULL;
    } else {
        iloczyn->wartosc = Y;
        poma = a;
        while (poma->wartosc != Z) {
            pomb = b;
            while (pomb->wartosc != Z) {
                nowy2 = malloc(sizeof *nowy2);
                tmpa = kopiuj(poma->wykladnik);
                tmpb = kopiuj(pomb->wykladnik);
                nowy1 = dodawanie(tmpa, tmpb);
                nowy2->wartosc = Y;
                iloczyn->wykladnik = nowy1;
                iloczyn->bok = nowy2;
                iloczyn = iloczyn->bok;
                pomb = pomb->bok;
            }
            poma = poma->bok;
        }
        iloczyn->wartosc = Z;
        iloczyn->bok = iloczyn->wykladnik = NULL;
    }
    iloczyn = poczatek;
    normalizacja(iloczyn);
    return iloczyn;
}

int main(void) {
    Tliliczba *x, *y, *iloczyn;
    x = wczytanie();
    getchar();
    y = wczytanie();
    normalizacja(x);
    normalizacja(y);
    iloczyn = mnozenie(x, y);
    pisz(iloczyn);
    printf("\n");
    usun_liczbe(x);
    usun_liczbe(y);
    usun_liczbe(iloczyn);
    return 0;
}