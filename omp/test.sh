#rimuovere tutti correct,tutti i result, match,e le table,se presenti.
rm *.txt;
rm *.csv;
gcc main.c -fopenmp;
./a.out  $1;
bash analizza_correttezza.sh result_parallelo_1.txt result_parallelo_2.txt
bash analizza_correttezza.sh result_parallelo_1.txt result_parallelo_4.txt
bash analizza_correttezza.sh result_parallelo_1.txt result_parallelo_8.txt
bash analizza_correttezza.sh result_parallelo_1.txt result_parallelo_16.txt
bash analizza_correttezza.sh result_parallelo_1.txt result_parallelo_32.txt
