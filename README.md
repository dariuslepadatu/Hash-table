Inainte de main:

Am declarat patru structuri diferite: TLG, TAux, TLGEL si TH. Prima reprezinta lista
care se construieste pe baza cuvintelor din fisier si a numarului de aparitii
al acestora. TAux reprezinta o sublista din lista TLGEL, lista de liste ce va fi
inserata ulterior in tabela Hash TH.

Functia CodHash returneaza indicele pe care un cuvant l-ar ocupa in vectorul
TLGEL din TH. 
AlocCelulaG, AlocCelulaAux, AlocCelulaEL si AlocCelulaEL2 aloca
dinamic celule ce vor fi inserate ulterior in anumite liste. 
In functia InserareTLG se parcurge o lista si se adauga un cuvant 
nou atunci cand acesta nu e gasit, iar daca acesta este gasit se incrementeaza 
numarul de aparitii.
InserareAlfabeticaAux inseareaza ordonat, din punct de vedere lexicografic,
cuvintele din lista TLG intr-o sublista de tip TAux.
Algoritmul din Bubblesort ordoneaza dupa numarul de aparitii o lista trimisa ca 
parametru, folosindu-se de functia Swap.
Functia de ConstruireEL parcurge fiecare celula din TLG L si formeaza lista de liste TLGEL EL.
EL contine in fiecare celula lungimea cuvintelor pe care le are sublista 
TAux, si, inclusiv pe aceasta din urma.
Functia cmp_lung returneaza diferinta lungimilor a doua siruri, iar cmp_sir
intoarce rezultatul functiei strcmp.

Exista trei functii care se ocupa de generarea tabelei Hash: GenerareHash, 
InsTH si IniTH. Acestea initializeaza TH si introduc celulele din lista TLGEL in
vectorul de acelasi tip, in functie de lungimea cuvintelor si a indecelui acestora. 
In continuare sunt mai multe functii de afisare a tabelei Hash care sunt similare intre ele,
diferind doar modul de parcurgere al vectorului de liste. AfiTH afiseaza intregul TH,
AfiTH2 cauta cuvintele care incep cu o litera data si au o anumita lungime, iar AfiTH3
afiseaza cuvintele care apar de un anumit numar de ori.
DistrugeLG, DistrugeEL si DistrugeTH elibereaza memoria continuta de aceste 
structuri de date.

In main:

Se deschide fisierul trimis ca argument si se citeste linie cu linie din acesta,
fiecare cuvant fiind parcurs cu strtok. Cand se apeleaza "insert" atunci se
introduce cuvantul in lista TLG prin functia InserareTLG, iar atunci cand se 
apeleaza "print" se construieste lista de liste TLGEL si tabelul TH pentru a putea
fi printat ulterior.

La final se elibereaza toata memoria alocata si se inchide fisierul.