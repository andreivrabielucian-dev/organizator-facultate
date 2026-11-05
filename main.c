#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_SARCINI 100
#define MAX_TEXT 160
#define FISIER "sarcini.txt"

typedef struct {
    char text[MAX_TEXT];
    int terminata;
} Sarcina;

/* Citeste un rand complet. Respinge textul prea lung, fara a-l trunchia. */
int citeste_rand(FILE *sursa, char text[], int capacitate)
{
    int c;
    size_t lungime;

    if (fgets(text, capacitate, sursa) == NULL)
        return 0;

    lungime = strlen(text);
    if (lungime > 0 && text[lungime - 1] == '\n') {
        text[--lungime] = '\0';
    } else {
        c = fgetc(sursa);
        if (c != '\n' && c != EOF) {
            while ((c = fgetc(sursa)) != '\n' && c != EOF) { }
            return -1;
        }
    }
    if (lungime > 0 && text[lungime - 1] == '\r')
        text[lungime - 1] = '\0';
    return 1;
}

int citeste_numar(const char mesaj[], int minim, int maxim, int *rezultat)
{
    char rand[80], *sfarsit;
    long valoare;
    int stare;

    while (1) {
        printf("%s", mesaj);
        fflush(stdout);
        stare = citeste_rand(stdin, rand, sizeof(rand));
        if (stare == 0)
            return 0;
        if (stare == 1) {
            errno = 0;
            valoare = strtol(rand, &sfarsit, 10);
            if (sfarsit != rand) {
                while (isspace((unsigned char)*sfarsit))
                    sfarsit++;
                if (errno == 0 && *sfarsit == '\0' &&
                    valoare >= minim && valoare <= maxim) {
                    *rezultat = (int)valoare;
                    return 1;
                }
            }
        }
        printf("Introdu un numar intre %d si %d.\n", minim, maxim);
    }
}

int incarca(Sarcina sarcini[], int *numar)
{
    FILE *f;
    char stare[8];
    int rezultat;

    errno = 0;
    f = fopen(FISIER, "r");
    if (f == NULL)
        return errno == ENOENT;

    *numar = 0;
    while ((rezultat = citeste_rand(f, stare, sizeof(stare))) != 0) {
        if (rezultat != 1 || *numar == MAX_SARCINI ||
            (strcmp(stare, "0") != 0 && strcmp(stare, "1") != 0) ||
            citeste_rand(f, sarcini[*numar].text, MAX_TEXT) != 1 ||
            sarcini[*numar].text[0] == '\0') {
            fclose(f);
            return 0;
        }
        sarcini[*numar].terminata = stare[0] - '0';
        (*numar)++;
    }
    rezultat = !ferror(f);
    fclose(f);
    return rezultat;
}

int salveaza(const Sarcina sarcini[], int numar)
{
    int i, corect = 1;
    FILE *f = fopen(FISIER ".tmp", "w");
    if (f == NULL)
        return 0;
    for (i = 0; i < numar; i++) {
        if (fprintf(f, "%d\n%s\n", sarcini[i].terminata,
                    sarcini[i].text) < 0)
            corect = 0;
    }
    if (fclose(f) != 0)
        corect = 0;
    if (!corect || rename(FISIER ".tmp", FISIER) != 0) {
        remove(FISIER ".tmp");
        return 0;
    }
    return 1;
}

void afiseaza(const Sarcina sarcini[], int numar)
{
    int i, terminate = 0;
    if (numar == 0) {
        printf("Nu ai sarcini in lista.\n");
        return;
    }
    printf("\n--- Sarcinile mele ---\n");
    for (i = 0; i < numar; i++) {
        printf("%d. [%c] %s\n", i + 1,
               sarcini[i].terminata ? 'X' : ' ', sarcini[i].text);
        terminate += sarcini[i].terminata;
    }
    printf("Progres: %d din %d terminate.\n", terminate, numar);
}

int main(void)
{
    Sarcina sarcini[MAX_SARCINI];
    char text[MAX_TEXT];
    int numar = 0, optiune, pozitie, i, stare, are_text;
    int nesalvat = 0;

    if (!incarca(sarcini, &numar)) {
        printf("Nu pot citi %s sau formatul sau este invalid.\n", FISIER);
        printf("Verifica fisierul inainte de a reporni programul.\n");
        return 1;
    }

    printf("=== Organizator pentru facultate ===\n");
    while (1) {
        printf("\n1. Adauga o sarcina\n2. Afiseaza sarcinile\n");
        printf("3. Schimba starea unei sarcini\n4. Sterge o sarcina\n0. Iesire\n");
        if (!citeste_numar("Alege o optiune: ", 0, 4, &optiune) || optiune == 0)
            break;

        if (optiune == 2) {
            afiseaza(sarcini, numar);
            continue;
        }

        if (optiune == 1) {
            if (numar == MAX_SARCINI) {
                printf("Lista este plina (maximum %d sarcini).\n", MAX_SARCINI);
                continue;
            }
            printf("Descriere (maximum %d octeti): ", MAX_TEXT - 1);
            fflush(stdout);
            stare = citeste_rand(stdin, text, sizeof(text));
            if (stare == 0)
                break;
            are_text = 0;
            for (i = 0; text[i] != '\0'; i++)
                if (!isspace((unsigned char)text[i]))
                    are_text = 1;
            if (stare != 1 || !are_text) {
                printf("Descrierea este goala sau prea lunga.\n");
                continue;
            }
            strcpy(sarcini[numar].text, text);
            sarcini[numar].terminata = 0;
            numar++;
            printf("Sarcina adaugata.\n");
        } else {
            afiseaza(sarcini, numar);
            if (numar == 0)
                continue;
            if (!citeste_numar("Numarul sarcinii (0 = anulare): ", 0, numar, &pozitie))
                break;
            if (pozitie == 0)
                continue;
            pozitie--;
            if (optiune == 3) {
                sarcini[pozitie].terminata = !sarcini[pozitie].terminata;
                printf("Stare actualizata.\n");
            } else {
                for (i = pozitie; i < numar - 1; i++)
                    sarcini[i] = sarcini[i + 1];
                numar--;
                printf("Sarcina stearsa.\n");
            }
        }
        nesalvat = !salveaza(sarcini, numar);
        if (nesalvat)
            printf("Atentie: modificarile sunt doar in memorie; salvarea a esuat.\n");
    }
    if (nesalvat && !salveaza(sarcini, numar)) {
        printf("Nu am putut salva modificarile.\n");
        return 1;
    }
    printf("La revedere!\n");
    return 0;
}
