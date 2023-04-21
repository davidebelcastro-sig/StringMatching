#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>


#include "string_match_s.c"

int variazioni_dim = 6;
int num_cicli;
//programma con un singolo thread.




//inserisce una linea nella tabella di variazioni di testo.
void insertLinea_var_testo()
{
    char file_name[50];
    char pattern_name[50];
    FILE *tsped_txt;
    tsped_txt = fopen("file_speedup_var_testo.csv","a");
    FILE *teff_txt;
    teff_txt = fopen("file_efficiency_var_testo.csv","a");
    strcpy(pattern_name,"../file/file_var_txt/pattern.txt");
    fprintf(tsped_txt,"1;");
    fprintf(teff_txt,"1;");
    int num_pat = getNumRows(13);     //numero di righe del pattern
   for(int i=1;i<=variazioni_dim;i++)
   {
        if(i == 1)
            strcpy(file_name,"../file/file_var_txt/file1.txt");
        else if (i == 2)
            strcpy(file_name,"../file/file_var_txt/file2.txt");
        else if (i == 3)
            strcpy(file_name,"../file/file_var_txt/file3.txt");
        else if (i == 4)
            strcpy(file_name,"../file/file_var_txt/file4.txt");
        else if (i == 5)
            strcpy(file_name,"../file/file_var_txt/file5.txt");
        else if (i == 6)
            strcpy(file_name,"../file/file_var_txt/file6.txt");
    int num_txt = getNumRows(i-1); //numero di righe del file
    FILE *time_es_seq;
    time_es_seq = fopen("time_esecution_seq.txt","a");
    double m;
    double now = time(NULL);

    for(int k=0;k<num_cicli;k++)
        run_s(file_name,pattern_name,num_pat,num_txt);
    m = difftime(time(NULL), now);
    if (m == 0.0)
        m = 0.1;
    fprintf(time_es_seq,"%f\n",m);
    fclose(time_es_seq);
    double tm = (double)m/m;
    double tm2 = (double)tm/1;
    fprintf(tsped_txt,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
    fprintf(teff_txt,"%f;",tm2);
    printf("num_thread = %d e tempo = %f e file %d\n",1,m,i);
    strcpy(file_name,"");
    }
    fprintf(tsped_txt,"\n");
    fprintf(teff_txt,"\n");
    fclose(tsped_txt);
    fclose(teff_txt);
}



void insertLinea_var_pattern()
{
    char file_name[50];
   char pattern_name[50];
   strcpy(file_name,"../file/file_var_pt/testo.txt");
   int num_txt = getNumRows(12);             //numero di righe del file
   FILE *tsped_pat;
    tsped_pat = fopen("file_speedup_var_pattern.csv","a");
    FILE *teff_pat;
    teff_pat = fopen("file_efficiency_var_pattern.csv","a");
    fprintf(tsped_pat,"1;");
    fprintf(teff_pat,"1;");
   for(int i=1;i<=variazioni_dim;i++) 
   {
        if(i == 1)
            strcpy(pattern_name,"../file/file_var_pt/pattern1.txt");
        else if (i == 2)
            strcpy(pattern_name,"../file/file_var_pt/pattern2.txt");
        else if (i == 3)
            strcpy(pattern_name,"../file/file_var_pt/pattern3.txt");
        else if (i == 4)
            strcpy(pattern_name,"../file/file_var_pt/pattern4.txt");
        else if (i == 5)
            strcpy(pattern_name,"../file/file_var_pt/pattern5.txt");
        else if (i == 6)
            strcpy(pattern_name,"../file/file_var_pt/pattern6.txt");
    //lancio questo file e il pattern prima con un thread,poi 2 poi 4 poi 8 poi 16 poi 32
    int num_pat = getNumRows(5+i); //numero di righe del file
    int num_thread = 1;
    int write = 0;
    FILE *time_es_seq;
    time_es_seq = fopen("time_esecution_seq.txt","a"); //line1 time var txt, line2 time var pat
    double m;
    double now = time(NULL);

    for(int k=0;k<num_cicli;k++)
        run_s(file_name,pattern_name,num_pat,num_txt);
    m = difftime(time(NULL), now);
    if (m == 0.0)
        m = 0.1;
    fprintf(time_es_seq,"%f\n",m);
    fclose(time_es_seq);
    double tm = (double)m/m;
    double tm2 = (double)tm/1;
    fprintf(tsped_pat,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
    fprintf(teff_pat,"%f;",tm2);
    printf("num_thread = %d e tempo = %f e file %d\n",1,m,i);
    strcpy(pattern_name,"");
    }
    fprintf(tsped_pat,"\n");
    fprintf(teff_pat,"\n");
     fclose(tsped_pat);
    fclose(teff_pat);
}



int main(int argc, char* argv[])
{
    num_cicli = atoi(argv[1]);
    MPI_Init(NULL, NULL);
    int thread_count;
    MPI_Comm_size(MPI_COMM_WORLD, &thread_count);
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    insertLinea_var_testo();
    insertLinea_var_pattern();
    MPI_Finalize();
    return 0;
}
