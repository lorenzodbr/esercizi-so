#!/bin/bash

#a) Elenco indirizzi IP (indicati una sola volta) da cui sono arrivate le richieste delle pagine

cat wordperfect.net-error_log | cut -d' ' -f10 | cut -d: -f1 | sort -un

#a. Indicare numero degli accessi per ogni indirizzo IP

cat wordperfect.net-error_log | cut -d' ' -f10 | cut -d: -f1 | sort -n | uniq -c

#b. Elenco indirizzi IP riportati una sola volta con numero di accessi, ordinati per numero di accessi

cat wordperfect.net-error_log | cut -d' ' -f10 | cut -d: -f1 | sort -n | uniq -c | sort -n

#b) Elenco dei soli errori presenti nel file

cat wordperfect.net-error_log | grep ":error"

#c) Numero di righe che presentano warning

cat wordperfect.net-error_log | grep ":warn" -c

#d) Dato il percorso 'home/web/mysite/html'

#a) Elenco di tutti i percorsi ricercati (unici)

cat wordperfect.net-error_log | grep "'/home/web/mysite/html/" | cut -d"'" -f2 | cut -d"[" -f2 | sort | uniq

#b) Elenco dei percorsi con quante volte appaiono

cat wordperfect.net-error_log | grep "'/home/web/mysite/html/" | cut -d"'" -f2 | cut -d"[" -f1 | sort | uniq -c

#c) Elenco dei percorsi con quante volte appaiono, ordinati per numero di accessi

cat wordperfect.net-error_log | grep "'/home/web/mysite/html/" | cut -d"'" -f2 | cut -d"[" -f1 | sort | uniq -c | sort -n
