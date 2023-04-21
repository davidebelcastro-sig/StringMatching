#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int num_cicli;

int getNumRows(char *file_testo) //ritorna numero righe
{
    FILE *pt;
    pt = fopen(file_testo, "r");
    char stringa[255];
    char *result;
    int s=0;
    while(1)
    {
        result = fgets(stringa, 255, pt);
        if( result == NULL )
            break;
        s++;
    }
    fclose(pt);
    return s;
}


//inizializza le tabelle,salva le dimensioni di tutti i file in modo da non contarle in seguito, compila e lancia i file.
int main(int argc, char *argv[])
{
    //creo file csv per le tabelle
    num_cicli = atoi(argv[1]);
    FILE *tsped_txt;
    int dim1,dim2,dim3,dim4,dim5,dim6,dim7,dim8,dim9,dim10,dim11,dim12;
    dim1 = getNumRows("../file/file_var_txt/file1.txt");
    dim2 = getNumRows("../file/file_var_txt/file2.txt");
    dim3 = getNumRows("../file/file_var_txt/file3.txt");
    dim4 = getNumRows("../file/file_var_txt/file4.txt");
    dim5 = getNumRows("../file/file_var_txt/file5.txt");
    dim6 = getNumRows("../file/file_var_txt/file6.txt");
    dim7 = getNumRows("../file/file_var_pt/pattern1.txt");
    dim8 = getNumRows("../file/file_var_pt/pattern2.txt");
    dim9 = getNumRows("../file/file_var_pt/pattern3.txt");
    dim10 = getNumRows("../file/file_var_pt/pattern4.txt");
    dim11 = getNumRows("../file/file_var_pt/pattern5.txt");
    dim12 = getNumRows("../file/file_var_pt/pattern6.txt");
    int oth_dim = getNumRows("../file/file_var_pt/testo.txt");
    int oth_dim2 = getNumRows("../file/file_var_txt/pattern.txt");
    FILE *num_righe;
    num_righe = fopen("num_righe.txt","w");
    fprintf(num_righe,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",dim1,dim2,dim3,dim4,dim5,dim6,dim7,dim8,dim9,dim10,dim11,dim12,oth_dim,oth_dim2);
    fclose(num_righe);
    tsped_txt = fopen("file_speedup_var_testo.csv","w");
    fprintf(tsped_txt,"Tabella speedup variazione dimensione file di testo\n" );
    fprintf(tsped_txt,"num_thread/size;%d;%d;%d;%d;%d;%d\n",dim1*num_cicli,dim2*num_cicli,dim3*num_cicli,dim4*num_cicli,dim5*num_cicli,dim6*num_cicli);
    fclose(tsped_txt);
    FILE *teff_txt;
    teff_txt = fopen("file_efficiency_var_testo.csv","w");
    fprintf(teff_txt,"Tabella efficiency variazione dimensione file di testo\n" );
    fprintf(teff_txt,"num_thread/size;%d;%d;%d;%d;%d;%d\n",dim1*num_cicli,dim2*num_cicli,dim3*num_cicli,dim4*num_cicli,dim5*num_cicli,dim6*num_cicli);
    fclose(teff_txt);
    FILE *tsped_pat;
    tsped_pat = fopen("file_speedup_var_pattern.csv","w");
    fprintf(tsped_pat,"Tabella speedup variazione dimensione pattern\n" );
    fprintf(tsped_pat,"num_thread/size;%d;%d;%d;%d;%d;%d\n",dim7*num_cicli,dim8*num_cicli,dim9*num_cicli,dim10*num_cicli,dim11*num_cicli,dim12*num_cicli);
    fclose(tsped_pat);
    FILE *teff_pat;
    teff_pat = fopen("file_efficiency_var_pattern.csv","w");
    fprintf(teff_pat,"Tabella efficiency variazione dimensione pattern\n" );
    fprintf(teff_pat,"num_thread/size;%d;%d;%d;%d;%d;%d\n",dim7*num_cicli,dim8*num_cicli,dim9*num_cicli,dim10*num_cicli,dim11*num_cicli,dim12*num_cicli);
    fclose(teff_pat);
    //ora lancio i file e ogni run con x thread farà la sua riga su tutti e 4 i file
    system("mpicc run_mpi_s.c -o run_mpi");
    char command[50];
    sprintf(command,"mpirun -n 1 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    //return 0;
    system("mpicc run_mpi_p.c -o run_mpi");
    sprintf(command,"mpirun -n 2 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    sprintf(command,"mpirun -n 4 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    sprintf(command,"mpirun -n 8 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    sprintf(command,"mpirun -n 16 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    sprintf(command,"mpirun -n 32 --oversubscribe ./run_mpi %d",num_cicli);
    system(command);
    return 0;
}
