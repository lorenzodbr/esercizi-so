# Salvare in variabili distinte tutti i valori letti dai sensori (temperatura, umidità, luminosità, CO2).

temp=`cat Occupancy.csv | tail +2 | cut -d',' -f2`
hum=`cat Occupancy.csv | tail +2 | cut -d',' -f3`
light=`cat Occupancy.csv | tail +2 | cut -d',' -f4`
co2=`cat Occupancy.csv | tail +2 | cut -d',' -f5`

# a) Contare quanti valori sono presenti per ogni sensore

echo "Totale valori: "
echo $temp | wc -w
echo ""

# b) Stampare il minimo e il massimo valore letto da ogni sensore

echo "Min temp: "
echo "$temp" | sort -n | head -n 2 | tail -n 1
echo ""

echo "Min hum: "
echo "$hum" | sort -n | head -n 2 | tail -n 1
echo ""

echo "Min light: "
echo "$light" | sort -n | head -n 2 | tail -n 1
echo ""

echo "Min CO2: "
echo "$co2" | sort -n | head -n 2 | tail -n 1
echo ""

echo "Max temp: "
echo "$temp" | sort -n | tail -n 1
echo ""

echo "Max hum: "
echo "$hum" | sort -n | tail -n 1
echo ""

echo "Max light: "
echo "$light" | sort -n | tail -n 1
echo ""

echo "Max CO2: "
echo "$co2" | sort -n | tail -n 1
echo ""

# Salvare in due variabili distinte tutti i record contenuti nel file CSV suddividendoli in base alla
# presenza o meno di una persona nella stanza (valore binario espresso da Occupancy).

occupied=`cat Occupancy.csv | tr " " "," | grep ",1$"`
not_occupied=`cat Occupancy.csv | tr " " "," | grep ",0$"`

# a. Quanti record sono presenti nel dataset se considerati distintamente in base all’occupazione della stanza o meno.

occupied_length=`echo $occupied | wc -w`
not_occupied_length=`echo $not_occupied | wc -w`

echo "Totale occupied=1: $occupied_length"
echo "Totale occupied=0: $not_occupied_length"
echo ""

# b. Confrontare i valori dei sensori discriminando in base alla presenza o meno di una persona nella stanza, 
# in quale dei due casi la temperatura è più alta?

mean_occupied=0
mean_not_occupied=0
value=0

occupied_temp=`echo "$occupied" | cut -d',' -f3`
not_occupied_temp=`echo "$not_occupied" | cut -d',' -f3`

for row in $occupied_temp;
do
    mean_occupied=$((mean_occupied+$row))
done

mean_occupied=$((mean_occupied/occupied_length))

for row in $not_occupied_temp;
do
    mean_not_occupied=$((mean_not_occupied+$row))
done

mean_not_occupied=$((mean_not_occupied/not_occupied_length))

echo "La media della temperatura con persone è $mean_occupied°C"
echo "La media della temperatura senza persone è $mean_not_occupied°C"

if [ $mean_occupied -ge $mean_not_occupied ];
then
    echo "La temperatura è più alta, in media, quando ci sono persone";
else
    echo "La temperatura è più alta, in media, quando non ci sono persone";
fi

# ----------------------------------
# Questa parte cerca solo il massimo

# max_occ=`echo $occupied | tr " " "\n" | sort -t',' -k3 | tail -n 1 | cut -d',' -f3`
# max_not_occ=`echo $not_occupied | tr " " "\n" | sort -t',' -k3 | tail -n 1 | cut -d',' -f3`

# echo "La massima temperatura con persone è $max_occ°C"
# echo "La massima temperatura senza persone è $max_not_occ°C"