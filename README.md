# Organizator pentru facultate

Aplicatie de consola in C pentru gestionarea sarcinilor de facultate.

## Functionalitati

- Adaugarea si afisarea sarcinilor.
- Marcarea unei sarcini ca terminata sau redeschiderea ei.
- Stergerea unei sarcini dupa numarul din lista.
- Afisarea progresului: cate sarcini sunt terminate.
- Salvare automata in `sarcini.txt`, dupa fiecare modificare.
- Validarea optiunilor, a descrierilor si a fisierului incarcat.

## Rulare in OnlineGDB

1. Deschide https://www.onlinegdb.com/ si selecteaza limbajul **C**.
2. Copiaza continutul din `main.c` in editor.
3. Selecteaza **Interactive Console** pentru introducerea datelor, apoi apasa **Run**.
4. Introdu optiunile si descrierile in consola. Apasa Enter dupa fiecare raspuns.

Programul creeaza `sarcini.txt` in mediul in care ruleaza. Pastrarea fisierului intre sesiuni depinde de mediul OnlineGDB; descarca separat fisierul daca vrei sa pastrezi sarcinile. Nu publica sarcini personale pe GitHub.

## Rulare locala

Ai nevoie de un compilator C, de exemplu GCC:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o organizator
./organizator
```

## Exemplu

```text
1. [ ] Termin exercitiile la programare
2. [X] Recapitulez cursul de retele
Progres: 1 din 2 terminate.
```

## Structura si limite

`Sarcina` grupeaza descrierea si starea unei activitati. Programul foloseste un tablou cu maximum 100 de sarcini, fiecare cu o descriere de maximum 159 de octeti. Numerele din lista se schimba dupa stergere. Diacriticele pot ocupa mai multi octeti.

Fisierul contine cate doua randuri pentru fiecare sarcina: starea (`0` sau `1`), apoi descrierea. La salvare se foloseste un fisier temporar, inlocuind lista veche numai dupa scriere. Programul se opreste daca fisierul existent nu poate fi citit sau are format invalid.

## Context

Proiect de inceput, pregatit cu asistenta AI pentru studiu si dezvoltare treptata. Codul initial foloseste structuri, tablouri, functii si fisiere; urmatorul pas este intelegerea si modificarea fiecarei parti.

## Idei pentru versiuni viitoare

- Materie si termen-limita pentru fiecare sarcina.
- Filtrarea sarcinilor neterminate.
- Cautare dupa cuvinte.
