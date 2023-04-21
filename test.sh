#$1 è il numero di cicli,controllo che sia maggiore di 0
if [ $1 -gt 0 ]
then
	cd omp;
	bash test.sh $1
	cd ../mpi;
	bash test.sh $1
	cd ..;
	echo "confronto fra sequenziale omp e mpi,evenutuali errori,qui sotto:";
	diff ./omp/result_parallelo_1.txt ./mpi/result_parallelo_1.txt
else
	echo "inserire numero maggiore di 0!";

fi
