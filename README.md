# StringMatching
Project for the Exam "PROGRAMMING OF EMBEDDED AND MULTICORE SYSTEMS" at University of Rome La Sapienza


INFORMAZIONI -----------------------
	La relazione contiene una descrizione generale dell'algortimo e dell'approcio usato,nonchè una descrizione più dettagliata delle funzioni presenti nel codice,delle difficoltà ottenute e un esempio di esecuzione commentando gli output.
	I codici c sono commentati e spiegano in generale cosa fa una determinata funzione,alcune funzioni sono le stesse negli stessi file,per questo motivo alcune funzioni presenti in mpi non sono commentate mentre vengono commentate in openmp.
	In ogni cartella (omp e mpi) sono presenti i file .txt e .csv calcolati all'esecuzione precedente, quando si esegue il programma questi vengono automaticamente eliminati e, alla fine, ricreati.
	

PER ESEGUIRE -------------------------
	solo omp : eseguire lo script test.sh dentro la cartella omp passando come argomento un numero >=1, nella relazione è scritto bene il motivo di questo numero
	solo mpi : eseguire lo script test.sh dentro la cartella mpi passando come argomento un numero >=1, nella relazione è scritto bene il motivo di questo numero
	tutte e due: eseguire lo script test.sh dentro questa cartella passando come argomento un numero >=1, nella relazione è scritto bene il motivo di questo numero
	l'argomento rappresenta il numero di volte che effettuo lo stesso controllo match su uno stesso file, i file di input sono 6 e hanno dimensioni diverse, passare come argomento un numero uguale a 2
	significa raddoppiare la dimensione di un file, l'ho fatto per far vedere maggior differenza tra i thread(per evitare di fare file enormi ho fatto file più leggeri dell'ordine di 16 MB il più grande e 5 MB il più piccolo che però vengono calcolati n volte se n è il numero passato in input)
	Nella relazione sono presenti screen di output passando in input 5, le differenze in tempi di esecuzione sono evidenti e il tempo totale di esecuzione è di 15 minuti.
	Passando invece come input 2 le differenze sono lievi e il tempo totale è di 4 minuti.
	NB: tempi di esecuzione calcolati sul mio portatile intel i7. 


INPUT ---------------------------------
	Nella cartella "file" sono presenti tutti gli input che verrano utilizzati
	Git non mi permette di inserire file maggiori di 100MB di conseguenza ho lasciato i file di testo vuoti,inserire un testo a piacere di dimensioni notevoli e differenti nei diversi file.


OUTPUT ---------------------------------
	Nelle cartelle omp e mpi si genereranno i file .csv che rappresentano le tabelle di speedup e efficency
	su schermo vengono stampati i tempi di esecuzione di ogni esecuzione con x thread per l'i-esimo file
