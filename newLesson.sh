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
    echo "#include <stdlib.h>" >> "$srcpath"
    echo "" >> "$srcpath"
    echo "int main(int argc, char *argv[]){" >> "$srcpath"
    echo "    " >> "$srcpath"
    echo "    " >> "$srcpath"
    echo "    return 0;" >> "$srcpath"
    echo "}" >> "$srcpath"
    
    # crea il makefile nella cartella principale
    makefile="$es_dir"/Makefile
    touch "$makefile"
    echo "CC = gcc" >> "$makefile"
    echo "CFLAGS = -Wall -pedantic -std=c99" >> "$makefile"
    echo "SRC = src/es$i.c" >> "$makefile"
    echo "OBJ = obj/es$i.o" >> "$makefile"
    echo "BIN = bin/es$i" >> "$makefile"
    echo "" >> "$makefile"
    echo "all: \$(BIN)" >> "$makefile"
    echo "" >> "$makefile"
    echo "\$(BIN): \$(OBJ)" >> "$makefile"
    echo "\t@echo \"Linking \$@...\"" >> "$makefile"
    echo "\t@\$(CC) \$(CFLAGS) -o \$@ \$^" >> "$makefile"
    echo "\t@echo \"Done.\"" >> "$makefile"
    echo "" >> "$makefile"
    echo "\$(OBJ): \$(SRC)" >> "$makefile"
    echo "\t@echo \"Compiling \$@...\"" >> "$makefile"
    echo "\t@\$(CC) \$(CFLAGS) -c -o \$@ \$^" >> "$makefile"
    echo "" >> "$makefile"
    echo "PHONY: clean" >> "$makefile"
    echo "" >> "$makefile"
    echo "clean:" >> "$makefile"
    echo "\t@echo \"Cleaning...\"" >> "$makefile"
    echo "\t@rm -f \$(OBJ) \$(BIN)" >> "$makefile"
    echo "\t@echo \"Done.\"" >> "$makefile"
  done
fi
