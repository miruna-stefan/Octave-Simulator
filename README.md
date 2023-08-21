# Octave-Simulator
Knowing that the Octave programming language and the whole software package are written in C and C++, I implemented my own Octave Simulator in C.

Copyright 2022 STEFAN MIRUNA ANDREEA 314CA
Simulator de octave - Descriere

In functia main, declararea si initializarea variabilelor se va face in vederea
implementarii urmatoarei scheme logice:
Vom retine toate matricele citite intr-o colectie (un vector de matrice).
In paralel, vom retine (in aceasta ordine) numarul de linii, numarul de coloane
si suma elementelor fiecarei matrice intr-un vector, pe care il adaugam la 
colectia de detalii ale matricelor (aceasta va fi un vector de vectori). 
Se va realoca memorie (atat la colectia de matrice, cat si la colectia de
detalii) de fiecare data cand vrem sa adaugam o noua matrice la colectie.
Detaliile ne vor fi necesare pentru efectuarea diferitelor operatii pe matrice.
Corespondenta dintre colectia de matrice si cea de detalii se realizeaza 
prin index, adica unei matrice de pe o anumita pozitie din colectia de matrice
ii corespund dimensiunile si suma elementelor aflate pe aceeasi pozitie in
colectia de detalii.
Vom citi de la tastatura caractere pana la intalnirea caracterului 'Q', care 
indica sfarsitul programului. Fiecarei comenzi ii corespunde o anumita litera.
Asadar, dupa ce recunoastem caracterul introdus, citim input-ul aferent
comenzii (daca este cazul) si apelam functiile specifice.
Acestea sunt urmatoarele:

- Functia free_all_resources, prin intermediul careia se realizeaza eliberarea 
tuturor resurselor. Aceasta va fi apelata la finalul programului, ca urmare a 
introducerii de la tastatura a caracterului 'Q' sau pe parcursul altor functii 
in care alocam memorie dinamic: daca alocarea a esuat, inainte de a incheia 
executia programului, va trebui sa dealocam toate resursele (programare 
defensiva).

- Functia new_matrix intoarce o matrice pe care o citeste de la tastatura, nu
inainte de a aloca dinamic memorie pentru aceasta.

- Functia sum_matrix_elements intoarce suma tuturor elementelor matricei. 
Ulterior, vom face sortarea matricelor in functie de suma aceasta.

- Functia resize adauga in colectia de matrice o noua matrice, nu inainte de 
a realoca memorie pentru colectie (ii face loc in colectie). In paralel, se 
adauga si vectorul ce contine dimensiunile si suma elementelor noii matrice 
in colectia detaliilor, nu inainte de a realoca memorie si pentru aceasta.

- Functia print_dimensions afiseaza numarul de linii, urmat de numarul de 
coloane al matricei de pe o anumita pozitie. Dimensiunile acesteia sunt stocate
in vectorul de pe pozitia aferenta din colectia detaliilor.

- Functia print_matrix afiseaza matricea de pe o anumita pozitie din colectia 
de matrice.

- Functia multiply realizeaza inmultirea a doua matrice, aloca memorie pentru 
inca o matrice in colectie si stocheaza matricea produs pe ultima pozitie din 
colectie. De asemenea, aloca memorie si pentru inca un vector in colectia de 
detalii si retine dimensiunile si suma elementelor matricei produs pe ultima 
pozitie din colectia de detalii.

- Functia redimension realizeaza redimensionarea unei matrice. In functie se 
citesc: numarul de linii, respectiv de coloane dupa care se face 
redimensionarea, urmate de liniile dupa care se face operatia (care se vor 
stoca intr-un vector), respectiv de coloanele dupa care se face operatia de 
redimensionare, care vor fi, de asemenea, stocate intr-un vector. Desi operatia
de redimensionare are loc in-place, se va dealoca memoria pentru matricea aflata 
initial la index-ul respectiv si se va aloca din nou memorie exact pe 
dimensiunile matricei redimensionate (pentru ca acestea vor fi clar mai mici 
sau cel mult egale cu dimensiunile matricei initiale). Apoi, pe index-ul 
respectiv se va aseza in colectie noua matrice si se va actualiza si in colectia
de detalii vectorul ce contine dimensiunile si suma elementelor noii matrice.
 
- Functia sort sorteaza simultan colectia de matrice si colectia de detalii 
dupa modelul algoritmului bubble sort, in functie de suma elementelor fiecarei
matrice, a carei valoare se gaseste pe ultima pozitie din vectorul cu detalii 
al fiecarei matrice. Acest vector se gaseste in colectia de detalii, la 
index-ul aferent matricei.
 
- Functia transpose_matrix realizeaza transpusa unei matrice, nu inainte de a
aloca memorie pentru aceasta. Chiar daca operatia are loc in-place, se dealoca
memoria matricei vechi inainte de a o pune pe cea noua in loc, pentru ca cele
doua pot avea dimensiuni diferite. De asemenea, se actualizeaza si vectorul din
colectia de detalii corespunzator matricei respective prin interschimbarea
numarului liniilor cu numarul coloanelor, pentru ca matricea noua este chiar
transpusa celei vechi.

- Functia multiply_same_dimension_matrixes ne va fi utila la ridicarea la 
putere, pentru ca realizeaza inmultirea a doua matrice patratice cu aceleasi
dimensiuni. Matricea-produs va fi pusa in locul primei matrice, care va fi
transmisa prin referinta.

- Functia raise face ridicarea la putere a unei matrice in timp logaritmic.
Se inlocuieste in colectie vechea matrice cu matricea rezultata in urma
operatiei. De data aceasta, nu va fi nevoie sa dealocam memoria pentru matricea
initiala, pentru ca aceasta si matricea-rezultat vor avea aceleasi dimensiuni.
Din acelasi motiv, in vectorul din colectia de detalii se actualizeaza doar
suma elementelor matricei, nu si dimensiunile.

- Functia remove_matrix elimina o singura matrice din memorie. Mai intai se
dealoca memoria matricei de pe index-ul dat, apoi toate matricele care urmeaza
dupa aceasta se muta cu o pozitie la stanga si se micsoreaza memoria alocata
colectiei de matrice. Se procedeaza analog si pentru colectia de detalii.
