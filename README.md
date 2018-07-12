# Progetto di Prova Finale di Algoritmi e Strutture Dati, AA 2017-18
Si progetti ed implementi in linguaggio C standard, senza usufruire di librerie esterne, un interprete di Macchine di Turing non-deterministiche, nella variante a **nastro singolo** e solo **accettori**.

La struttura del file di ingresso è la seguente: prima viene fornita la funzione di transizione, quindi gli stati di accettazione e un limite massimo sul numero di passi da effettuare per una singola computazione (per evitare in maniera banale il problema delle macchine che non terminano), infine una serie di stringhe da far leggere alla macchina.

In uscita ci si attende un file contenente 0 per le stringhe non accettate e 1 per quelle accettate; essendoci anche un limite sul numero di passi, il risultato può anche essere U se non si è arrivati ad accettazione.

**Convenzioni:** Per semplicità i simboli di nastro sono dei `char`, mentre gli stati sono `int`. Il carattere `_` indica il simbolo *blank*. La macchina parte sempre dallo stato 0 e dal primo carattere della stringa in ingresso. I caratteri *L*, *R*, *S* vengono usati, come al solito, per il movimento della testina.

Il file di ingresso viene fornito tramite lo *standard input*, mentre quello in uscita è sullo *standard output*.

Il file di ingresso è diviso in 4 parti:

+ Una prima parte, che inizia con ***tr***, contiene le transizioni, una per linea — ogni carattere può essere separato dagli altri da spazi. Per esempio, `0 a c R 1` significa che con la transizione si va dallo stato 0 allo stato 1, leggendo *a* e scrivendo *c*. La testina viene spostata a destra (*R*).
+ La parte successiva, che inizia con ***acc***, elenca gli stati di accettazione, uno per linea.
+ Per evitare problemi di computazioni infinite, nella sezione successiva, che inizia con ***max***, viene indicato il numero di mosse massimo che si possono fare per accettare una stringa.
+ La parte finale, che inizia con ***run***, è un elenco di stringhe da fornire alla macchina, una per linea.

### Esempio: una MT per ww

Ecco un esempio di file di ingresso:
```
tr
0 a C R 1
0 b C R 2
1 a a R 1
1 b b R 1
2 a a R 2
2 b b R 2
1 a C L 3
2 b C L 3
3 a a L 3
3 b b L 3
3 C c R 4
4 a C R 5
4 b C R 6
5 a a R 5
5 b b R 5
5 c c R 5
5 C c R 7
6 a a R 6
6 b b R 6
6 c c R 6
6 C c R 8
7 a C L 9
8 b C L 9
9 a a L 9
9 b b L 9
9 C c R 4
4 c c R 10
4 C c R 10
10 c c R 10
10 _ _ S 11
acc
11
max
180
run
aabaab
bbabbb
ababa
babaaababaaa
```
Il conseguente file di uscita in questo caso è il seguente:
```
1
0
0
1
```
