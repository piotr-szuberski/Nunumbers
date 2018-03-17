/* Piotr Szuberski, p.szuberski@student.uw.edu.pl
   Program zalcizeniowy nr 3 - Liliczby
   Program wczytuje z wejścia dwie liliczby zapisane
   znakami Y i Z oraz oblicza ich iloczyn. */

#include <stdio.h>
#include <stdlib.h>

#define Y 1
#define Z 0
#define MNIEJSZA -1
#define WIEKSZA 1
#define ROWNE 0

struct liliczba {
    int wart;
    struct liliczba *wykl;
    struct liliczba *bok;
};
typedef struct liliczba Tliliczba;

/*Funkcja nr 1: wczytuje jedną liliczbę */
Tliliczba *wczytanie(void) {
    int x;
    Tliliczba *l = malloc(sizeof *l);
    x = getchar();
    if(x == 'Z') {
        l->wart = Z;
        l->bok = l->wykl = NULL;
    } else {
        l->wart = Y;
        l->wykl = wczytanie();
        l->bok = wczytanie();
    }
    return l;
}

/* Funkcja nr 2: wypisuje liliczbę na wyjście */
void pisz(Tliliczba *l) {
    if(l->wart == Z)
        printf("Z");
    else {
        printf("Y");
        pisz(l->wykl);
        pisz(l->bok);
    }
}

/* Funkcja nr 3: kasuje liliczbę, uwalniając zaalokowane miejsce */
void usun_liczbe(Tliliczba *x) {
    if(x->wart == Z) {
        free(x);
    } else {
        usun_liczbe(x->wykl);
        usun_liczbe(x->bok);
        free(x);
    }
}

/* Funkcja nr 4: kasuje segment liliczby */
void usun_segment(Tliliczba *x) {
    if(x->wart == Z)
        free(x);
    else {
        usun_liczbe(x->wykl);
        free(x);
    }
}

/* Funkcja nr 4: porównuje ze sobą dwie liliczby */
int porownaj_liczbe(Tliliczba *a, Tliliczba *b) {
    int porownanie;
    if(a->wart == Z && b->wart == Z) {
        porownanie = ROWNE;
    } else { if(a->wart == Z) {
        porownanie = MNIEJSZA;
    } else { if(b->wart == Z) {
        porownanie = WIEKSZA;
    } else {
        porownanie = porownaj_liczbe(a->wykl, b->wykl);
        if(porownanie == ROWNE) {
            porownanie = porownaj_liczbe(a->bok, b->bok);
        } else { if(porownanie == MNIEJSZA) {
            porownanie = porownaj_liczbe(a->bok, b);
            if(porownanie == ROWNE)
                porownanie = WIEKSZA;
        } else { if(porownanie == WIEKSZA) {
            porownanie = porownaj_liczbe(a, b->bok);
            if(porownanie == ROWNE)
                porownanie = MNIEJSZA;
        }}}
    }}}
    return porownanie;
}

/* Funkcja nr 5: porównująca segmenty liliczby.
   Wartość wyniku dotyczy liliczby "a".
*/
int porownaj_segment(Tliliczba *a, Tliliczba *b) {
    int porownanie;
    if(a->wart == Z && b->wart == Z) {
        porownanie = ROWNE;
    } else { if(a->wart == Z) {
        porownanie = MNIEJSZA;
    } else { if(b->wart == Z) {
        porownanie = WIEKSZA;
    } else {
        porownanie = porownaj_liczbe(a->wykl, b->wykl);
    }}}
    return porownanie;
}

/* Funkcja nr 6: dodaje wartość 1 do liliczby */
void dodaj_jeden(Tliliczba *x) {
    Tliliczba *nowy1, *nowy2;
    while(x->wart != Z) {
        x = x->bok;
    }
    nowy1 = malloc(sizeof*nowy1);
    nowy2 = malloc(sizeof*nowy2);
    nowy2->wykl = nowy2->bok = nowy1->wykl = nowy1->bok = NULL;
    nowy2->wart = nowy1->wart = Z;
    x->wart = Y;
    x->wykl = nowy1;
    x->bok = nowy2;
}

/* Funkcja nr 7: zamienia wykładniki liliczb */
void zamien(Tliliczba *a, Tliliczba *b) {
    Tliliczba *tmp;
    tmp = a->wykl;
    a->wykl = b->wykl;
    b->wykl = tmp;
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
    if(a->wart == Z) {
        return a;
    }
    normalizacja(a->wykl);
    do {
        normalizacja(a->bok);
        if (a->bok->wart == Z)
            x = MNIEJSZA;
        else {
            x = porownaj_segment(a, a->bok);
        }
        if(x == ROWNE) {
            tmp = a->bok;
            a->bok = tmp->bok;
            tmp->bok = NULL;
            usun_segment(tmp);
            dodaj_jeden(a->wykl);
            normalizacja(a);
        } else if(x == WIEKSZA) {
            zamien(a, a->bok);
        }
    } while (x == WIEKSZA);
    return a;
}

/* Funkcja nr 9: dodająca do siebie liliczby - łączy boki
   liliczb i kasuje Z jednej z nich, następnie je normalizuje */
Tliliczba *dodawanie(Tliliczba *a, Tliliczba *b) {
    Tliliczba *suma, *koniec, *poprz;
    if(a->wart == Z) {
        free(a);
        return b;
    } else { if(b->wart == Z) {
        free(b);
        return a;
    } else {
        poprz = a;
        koniec = a->bok;
        while(koniec->wart != Z) {
            koniec = koniec->bok;
            poprz = poprz->bok;
        }
        free(koniec);
        suma = a;
        poprz->bok = b;
        normalizacja(suma);
        return suma;
    }}
}

/* Funkcja nr 10: kopiuje liliczbę, alokując nowe miejsca w pamięci */
Tliliczba *kopiuj(Tliliczba *x) {
    Tliliczba *kopia;
    kopia = (Tliliczba*)malloc(sizeof*kopia);
    if(x->wart == Z) {
        kopia->wart = Z;
        kopia->wykl = kopia->bok = NULL;
    } else {
        kopia->wart = Y;
        kopia->wykl = kopiuj(x->wykl);
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
    iloczyn = malloc(sizeof*iloczyn);
    poczatek = iloczyn;
    if(a->wart == Z || b->wart == Z) {
        iloczyn->wart = Z;
        iloczyn->bok = iloczyn->wykl = NULL;
    } else {
        iloczyn->wart = Y;
        poma = a;
        while(poma->wart != Z) {
            pomb = b;
            while(pomb->wart != Z) {
                nowy2 = malloc(sizeof*nowy2);
                tmpa = kopiuj(poma->wykl);
                tmpb = kopiuj(pomb->wykl);
                nowy1 = dodawanie(tmpa, tmpb);
                nowy2->wart = Y;
                iloczyn->wykl = nowy1;
                iloczyn->bok = nowy2;
                iloczyn = iloczyn->bok;
                pomb = pomb->bok;
            }
            poma = poma->bok;
        }
        iloczyn->wart = Z;
        iloczyn->bok = iloczyn->wykl = NULL;
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