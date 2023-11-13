#!/bin/bash

# verifica che sia stato specificato almeno un parametro
if [ $# -ne 2 ]; then
  echo "Usage: $0 <nome_cartella> <num_esercizi>"
  exit 1
fi

# nome della cartella principale
dir_name=$1
month=`echo $dir_name | cut -c 5-6`

if (((month >= 3 && month < 10))); then
    # crea la cartella principale
    mkdir "Secondo semestre/$dir_name"
    cd "Secondo semestre/$dir_name"
else
    mkdir "Primo semestre/$dir_name"
    cd "Primo semestre/$dir_name"
fi

# crea le sotto-cartelle Materiale ed Esercizi
mkdir Materiale Esercizi

# se è stato specificato un numero di esercizi, crea le relative cartelle
if [ $# -gt 1 ]; then
  num_esercizi=$2
  
  # ciclo per creare le cartelle esN
  for ((i=1; i<=num_esercizi; i++)); do
    es_dir="Esercizi/es$i"
    mkdir -p "$es_dir"/{bin,src,obj}
    srcpath="$es_dir"/src/"es$i.c"
    echo "#include <stdio.h>" >> "$srcpath"
    echo "" >> "$srcpath"
    echo "int main(int argc, char* argv[]){" >> "$srcpath"
    echo "    " >> "$srcpath"
    echo "    " >> "$srcpath"
    echo "    return 0;" >> "$srcpath"
    echo "}" >> "$srcpath"
    
    # crea il makefile nella cartella principale
    makefile="$es_dir"/Makefile
    touch "$makefile"
    # echo "NAME=es$i" >> "$makefile"
    # echo "CFLAGS=-Wall -Werror" >> "$makefile"
    # echo "" >> "$makefile"
    # echo "all: obj/es$i.o" >> "$makefile"
    # echo "" >> "$makefile"
    # echo "obj/es$i.o: src/es$i.c" >> "$makefile"
    # echo -e "\t\$(CC) \$(CFLAGS) -c src/es$i.c -o obj/es$i.o" >> "$makefile"
    # echo "" >> "$makefile"
    # echo ".PHONY: clean" >> "$makefile"
    # echo "clean:" >> "$makefile"
    # echo -e "\trm -f obj/es$i.o bin/es$i" >> "$makefile"
  done
fi
