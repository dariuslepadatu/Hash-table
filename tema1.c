/* LEPADATU Darius - 312 CB */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OSUTA 500

typedef struct celulag {
    int nr_aparitii;
    void *cuv;
    struct celulag *urm;
} TCelulaG, *TLG;

typedef struct celulaaux {
    int nr;
    void *cuv;
    struct celulaaux *urm;
} TAuxiliar, *TAux;

typedef struct celulael {
    unsigned int lung;
    int cod;
    void *el;
    struct celulael *urm;
} TElement, *TLGEL;

typedef int (*TFHash)(void*);

typedef struct {
    int M;
    TFHash fh;
    TLGEL *v;
} TH;

// Functia CodHash returneaza indicele pe care un cuvant l-ar ocupa in vectorul
// TLGEL din TH. 

int CodHash (void *x) {
    int i = 0;
    char *cod = (char*)x;
    if (cod[0] >= 65 && cod[0] <= 90) {
        i = cod[0] - 'A';
    } else if (cod[0] >= 97 && cod[0] <= 122) {
        i = cod[0] - 'a';
    }
    return i;
}

// AlocCelulaG, AlocCelulaAux, AlocCelulaEL si AlocCelulaEL2 aloca
// dinamic celule ce vor fi inserate ulterior in anumite liste. 

TLG AlocCelulaG(int nr, void *x) {
    TLG aux = (TLG)calloc(1, sizeof(TCelulaG));
    if (!aux) return NULL;
    aux->nr_aparitii = nr;
    aux->cuv = x;
    aux->urm = NULL;
    return aux;
}

TAux AlocCelulaAux(TLG L) {
    TAux aux = (TAux)calloc(1, sizeof(TAuxiliar));
    if (!L) return NULL;
    if (!aux) return NULL;
    aux->cuv = L->cuv;
    aux->nr = L->nr_aparitii;
    aux->urm = NULL;
    return aux;
}

TLGEL AlocCelulaEL(void *A) {
    TLGEL aux = NULL;
    TAux p = (TAux)A;
    if (!p) return NULL;
    aux = (TLGEL)calloc(1, sizeof(TElement));
    if (!aux) return NULL;
    aux->cod = CodHash(p->cuv);;
    aux->lung = strlen(p->cuv);
    aux->el = p;
    aux->urm = NULL;
    return aux;
}

TLGEL AlocCelulaEL2(TLGEL EL) {
    TLGEL aux = NULL;
    if (!EL) return NULL;
    aux = (TLGEL)calloc(1, sizeof(TElement));
    if (!aux) return NULL;
    aux->cod = EL->cod;
    aux->el = EL->el;
    aux->lung = EL->lung;
    aux->urm = NULL;
    return aux;
}

// In functia InserareTLG se parcurge o lista si se adauga un cuvant 
// nou atunci cand acesta nu e gasit, iar daca acesta este gasit se incrementeaza 
// numarul de aparitii.

int InserareTLG(TLG *aL, void *x,
int (*cmp_sir)(void *, void*)) {
    TLG p = NULL, aux = NULL, ant = NULL;
    for (p = *aL; p != NULL; p = p->urm) {
		if (cmp_sir(p->cuv, x) == 0) {
			p->nr_aparitii++;
            return 0;
		} else if(cmp_sir(p->cuv, x) > 0) {
            break;
        }
		ant = p;
    }
    aux = AlocCelulaG(1, x);
    if (!ant) {
        aux->urm = *aL;        
        *aL = aux;
    } else {
        ant->urm = aux;
        aux->urm = p;
    }
    return 1;
}

// InserareAlfabeticaAux inseareaza ordonat, din punct de vedere lexicografic,
// cuvintele din lista TLG intr-o sublista de tip TAux.

int InserareAlfabeticaAux(TAux *aL, TLG x,
int(*cmp_sir)(void *, void *)) {
    TAux p = NULL, aux = NULL, ant = NULL;
    for (p = *aL; p != NULL; p = p->urm) {
        if (cmp_sir(p->cuv, x->cuv) == 0) {
            return 0;
        }
		if (cmp_sir(p->cuv, x->cuv) > 0) {
			break;
		}
		ant = p;
    }
    aux = AlocCelulaAux(x);
    if (!ant) {
        aux->urm = *aL;
        *aL = aux;
    } else {
        ant->urm = aux;
        aux->urm = p;
    }
    return 1;
}

// Algoritmul din Bubblesort ordoneaza dupa numarul de aparitii o lista trimisa ca 
// parametru, folosindu-se de functia Swap.

void Swap(TAux a, TAux b) {
    char *s = NULL;
    int nr = 0;
    s = strdup((char*)a->cuv);
    nr = a->nr;
    a->cuv = b->cuv;
    a->nr = b->nr;
    b->cuv = s;
    b->nr = nr;
}

void BubbleSort(TAux L) {
    TAux p1 = NULL, p2 = NULL;
    int swap = 0;
    if (!L) return;
    do {
        swap = 0;
        for (p1 = L; p1->urm != p2; p1 = p1->urm) {
            if (p1->nr < p1->urm->nr) { 
                Swap(p1, p1->urm);
                swap = 1;
            }
        }
        p2 = p1;
    } while (swap);
}

// Functia de ConstruireEL parcurge fiecare celula din TLG L si formeaza lista de liste TLGEL EL.
// EL contine in fiecare celula lungimea cuvintelor pe care le are sublista 
// TAux, si, inclusiv pe aceasta din urma

TLGEL ConstruireEL(TLG L, int (*cmp_lung)(void *, void *),
int (*cmp_sir)(void *, void*)) {
    TLGEL EL = NULL, aux = NULL, q = NULL, ant = NULL;
    TAux A = NULL, pA = NULL;
    TLG p = NULL;
    int ok = 0;
    for (p = L; p != NULL; p = p->urm) {
        ok = 0;
        ant = NULL;
        for (q = EL; q != NULL && ok == 0; q = q->urm) {
            pA = (TAux)q->el;
            if (cmp_lung(p->cuv, pA->cuv) == 0 &&
            CodHash(p->cuv) == CodHash(pA->cuv)) {
                InserareAlfabeticaAux(&pA, p, cmp_sir);
                if (p->nr_aparitii != pA->nr) {
                    BubbleSort(pA);
                }
                ok = 1;
                break;
            }
            if (cmp_lung(p->cuv, pA->cuv) < 0) {
                break;
            }
            ant = q;
        }
        if (ok == 0) {
            A = AlocCelulaAux(p);
            aux = AlocCelulaEL(A);
            if (!ant) {
                aux->urm = EL;
                EL = aux;
            } else {
                aux->urm = q;
                ant->urm = aux;

            }           
        }
    }
    return EL;
}

// Functia cmp_lung returneaza diferinta lungimilor a doua siruri, iar cmp_sir
// intoarce rezultatul functiei strcmp.

int cmp_lung(void *x, void *y) {
    char *p1 = (char*)x, *p2 = (char*)y;
    return (strlen(p1) - strlen(p2));
}

int cmp_sir(void *x, void *y) {
    char *p = (char*)x;
    char *q = (char*)y;
    return strcmp(p, q);
}

// Exista trei functii care se ocupa de generarea tabelei Hash: GenerareHash, 
// InsTH si IniTH. Acestea initializeaza TH si introduc celulele din lista TLGEL in
// vectorul de acelasi tip, in functie de lungimea cuvintelor si a indecelui acestora. 

TH* InitTH(int M, TFHash fh) {
    TH *h = (TH*)calloc(1, sizeof(TH));
    if (!h) return NULL;
    h->v = (TLGEL*)calloc(M, sizeof(TLGEL));
    if(!h->v) {
        free(h);
        return NULL;
    }
    h->M = M;
    h->fh = fh;
    return h;
}

int InsTH(TH *a, void *EL) {
    TLGEL p = (TLGEL)EL, aux = NULL, v = NULL, ant = NULL, copie = NULL;
    int cod = p->cod;
    aux = AlocCelulaEL2(EL);
    if (!aux) return 0;
    if (!a->v[cod]) {
        aux->urm = (TLGEL)a->v[cod];
        a->v[cod] = aux;
    } else {
        for (v = a->v[cod]; v != NULL; v = v->urm) {
            if (!v) break;
            ant = v;
        }
        ant->urm = aux;
        aux->urm = v;
    }
    return 1;
}

TH *GenerareHash(TLGEL L) {
    TH *h = NULL;
	TLGEL p = NULL;
	int M = ('Z'-'A' + 1);
	h = (TH*)InitTH(M, CodHash);
	if(h == NULL) return NULL;
	for(p = L; p != NULL; p = p->urm) {
		InsTH(h, p);
	}
	return h;
}

// AfiTH2 cauta cuvintele care incep cu o litera data si au o anumita lungime, iar AfiTH3
// afiseaza cuvintele care apar de un anumit numar de ori.


void AfiTH(TH *h, void (*Afisare_EL)(TLGEL)) {
    TLGEL p = NULL;
    int i = 0;
    for(i = 0; i < h->M; i++) {
        p = h->v[i];
        if(p) {
            Afisare_EL(p);
        }
    }
}

void AfiTH2(TH *h, void (*Afisare_EL)(TLGEL, int), int nr1, int nr2) {
    TLGEL p = NULL;
        p = h->v[nr1];
        if(p) {
            Afisare_EL(p, nr2);
        }
}

void AfiTH3(TH *h, void (*Afisare_EL)(TLGEL, int, int *), int nr1) {
    TLGEL p = NULL;
    int i = 0, print = 0;
    for(i = 0; i < h->M; i++) {
        p = (TLGEL)h->v[i];
        if(p) {
            Afisare_EL(p, nr1, &print);
        }
        if (print == 1) {
            printf("\n");
            print = 0;
        }
    }
}

void Afisare_EL(TLGEL EL) {
    TLGEL q = NULL;
    TAux pA = NULL;
    printf("pos %d: ", EL->cod);
    for (q = EL; q != NULL; q = q->urm) {
        printf("(");
        pA = (TAux)q->el;
        printf("%d:", q->lung);
        for (; pA != NULL; pA = pA->urm) {
            printf("%s/%d", (char*)pA->cuv, pA->nr);
            if (pA->urm) {
                printf(", ");
            }
        }
            printf(")");        
    }
    printf("\n");
}

void Afisare_EL2(TLGEL EL, int nr2) {
    TLGEL q = NULL;
    TAux pA = NULL;
    for (q = EL; q != NULL; q = q->urm) {
        if(q->lung == (unsigned int)nr2) {
        printf("(");
        pA = (TAux)q->el;
        printf("%d:", q->lung);
        for (; pA != NULL; pA = pA->urm) {
            printf("%s/%d", (char*)pA->cuv, pA->nr);
            if (pA->urm) {
                printf(", ");
            }
        }
            printf(")");             
        }
    }
    printf("\n");
}

void Afisare_EL3(TLGEL EL, int nr1, int *print) {
    TLGEL q = NULL;
    TAux pA = NULL;
    int ok1 = 0, ok2 = 0, ok3 = 0;
    for (q = EL; q != NULL; q = q->urm) {
        pA = (TAux)q->el;
        ok2 = 0;
        ok3 = 0;
        for (; pA != NULL; pA = pA->urm) {
            if (pA->nr <= nr1) {
                if (ok3 == 1) {
                    printf(",");
                    ok3 = 0;                    
                }
                if (ok1 == 0) {
                    printf("pos%d: ", EL->cod);                    
                    ok1 = 1;
                }
                if(ok2 == 0) {
                    printf("(");
                    printf("%d:", q->lung);
                    ok2 = 1;
                }
                printf(" %s/%d", (char*)pA->cuv, pA->nr);
                if(pA->urm) {
                    ok3 = 1;
                }
            }
        }
        if(ok2 == 1) {
            printf(")");
            *print = 1;            
        }
    }

}

// DistrugeLG, DistrugeEL si DistrugeTH elibereaza memoria continuta de aceste 
// structuri de date.

void DistrugeLG(TLG* aL){
	while(*aL != NULL) {
        TLG aux = *aL;
        if (!aux) return;
        free(aux->cuv);
        *aL = aux->urm;
        if (!aux) return;
        free(aux);
    }
}

void DistrugeAux(TAux* aL){
	while(*aL != NULL) {
        TAux aux = *aL;
        if (!aux) return;
        free(aux->cuv);
        *aL = aux->urm;
        if (!aux) return;
        free(aux);
    }
}

void DistrugeEL(TLGEL* aL){
    TAux q = NULL;
	while(*aL != NULL) {
        TLGEL aux = *aL;
        q = aux->el;
        DistrugeAux(&q);
        if (!aux) return;
        *aL = aux->urm;
        free(aux);
    }
}

void DistrugeTH(TH **ah) {
    TLGEL *p;
    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) {
        if (p) {
            DistrugeEL(p);
        }
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
}

// Se deschide fisierul trimis ca argument si se citeste linie cu linie din acesta,
// fiecare cuvant fiind parcurs cu strtok. Cand se apeleaza "insert" atunci se
// introduce cuvantul in lista TLG prin functia InserareTLG, iar atunci cand se 
// apeleaza "print" se construieste lista de liste TLGEL si tabelul TH pentru a putea
// fi printat ulterior.

int main(int argc, char **argv) {
    FILE *input_file = NULL;
    TH *H = NULL;
    TLGEL EL = NULL;
    TLG L = NULL;;
    char *p = NULL, *linie = NULL, *token = NULL;
    int nr1 = 0, nr2 = 0;
    linie = (char*)calloc(OSUTA, sizeof(char));
    input_file = fopen(argv[1], "rt");
    InitTH(CodHash("Z"), CodHash);
    while (fgets(linie, OSUTA, input_file)) {
        if (linie[strlen(linie) - 1] == '\n') {
            linie[strlen(linie) - 1] = '\0';
        }
        if (linie[strlen(linie) - 1] == '.') {
            linie[strlen(linie) - 1] = ' ';
        }
        p = strtok(linie, " .,");
        if (!p) break;
        if (strcmp(p, "insert") == 0) {
            while (p) {
                p = strtok(NULL, " .,");
                if (!p) break;
                while (strlen(p) < 3 || (p[0] >=  48 && p[0] <= 57)) {
                    p = strtok(NULL, " .,");
                    if (!p) break;
                }
                if (p) {
                    token = strdup(p);
                    InserareTLG(&L, token, cmp_sir);
                }
            }
        } else if (strcmp(p, "print") == 0) {
            EL = ConstruireEL(L, cmp_lung, cmp_sir);                    
            H = GenerareHash(EL);
            p = strtok(NULL, " .,");
            if (!p) {
                AfiTH(H, Afisare_EL);
            }
            while (p) {
                if (!atoi(p)) {
                    nr1 = CodHash(p);
                } else {
                    nr1 = atoi(p);
                }
                p = strtok(NULL, " .,");                
                if (!p) {
                    AfiTH3(H, Afisare_EL3, nr1);
                    break;
                }                 
                nr2 = atoi(p);
                if (nr2 >= 3) {
                    AfiTH2(H, Afisare_EL2, nr1, nr2);
                } 
                p = NULL;
            }
        }
        p = strtok(NULL, " .,");
    }
    DistrugeTH(&H);
    // DistrugeLG(&L); 
    free(linie);
    fclose(input_file);
    return 0;
}