#include <stdio.h>
#include <mpi.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>


char  RESULT[40];
int variazioni_dim = 6;
int num_cicli;


//programma parallelo con n thread.


//ritorna il tempo di esecuzione sequenziale all'indice index
double getTime_s(int index)
{
    FILE *fp;
    fp = fopen("time_esecution_seq.txt","r");
    char stringa[30];
    int i=0;
    char *result;   
    while(i<=index)
    {
        result = fgets(stringa,30,fp);
        if(result == NULL)
            break;
        i++;
    }
    stringa[strlen(stringa)-1] = '\0';
    double num = atof(stringa);
    fclose(fp);
    return num;
}




void changeResult(int thread_count)
{
     strcpy(RESULT,"");
    if(thread_count == 2)
        strcpy(RESULT,"result_parallelo_2.txt");
    else if(thread_count == 4)
        strcpy(RESULT,"result_parallelo_4.txt");
    else if(thread_count == 8)
        strcpy(RESULT,"result_parallelo_8.txt");
    else if(thread_count == 16)
        strcpy(RESULT,"result_parallelo_16.txt");
    else if(thread_count == 32)
        strcpy(RESULT,"result_parallelo_32.txt");
}

int getNumRows(int index)
{
    FILE *fp;
    fp = fopen("num_righe.txt","r");
    char stringa[10];
    int i=0;
    char *result;
    while(i<=index)
    {
        result = fgets(stringa,10,fp);
        if(result == NULL)
            break;
        i++;
    }
    stringa[strlen(stringa)-1] = '\0';
    int num = atoi(stringa);
    fclose(fp);
    return num;
}






//divide equamente il lavoro
void settaRange(int len,int comm_sz,int *dist,int *dist_last,int *num_thread)
{
    *(dist_last) = 0;
    if(len <= comm_sz)
        {
            *num_thread = len;
            *(dist) = 1;
        }
    else
    {
        *num_thread = comm_sz;
        if(len % comm_sz != 0)
        {
            *(dist) = len / comm_sz;
            /*  tutti i thread a parte l'ultimo prendono dist elementi,l'ultimo invece..*/
            *(dist_last) = *(dist) + (len - (*(dist) *  comm_sz));
        }
        else
            *(dist) = len / comm_sz;
    }
    if (*(dist_last) == 0)
        *(dist_last) = *(dist); 
}



void computeLPSArray(char *pat, int M, int *lps);


void openFileResult(char *name,char *txt)
{
    FILE *fp;
    fp = fopen(name, "a");
    fprintf(fp, "%s\n", txt);
    fclose(fp);
}


int KMPSearch_S(char *pat, char *txt) {

        int M = strlen(pat);
        int mat = 0;
        int N = strlen(txt);
        char riga[50];
        int *lps = (int *) malloc(sizeof(int) * M);
        int j = 0; 
        computeLPSArray(pat, M, lps);         
        int i = 0; 
        while (i < N) { 



            if (pat[j] == txt[i]) { 
                j++;
                i++;

            }

            if (j == M) {  
                mat++;
                j = lps[j - 1];

            }
            else if (i < N && pat[j] != txt[i]) {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i = i + 1;
            }

        }

    free(lps); 
    return mat;  
    }

    void computeLPSArray(char *pat, int M, int *lps) 
    {
        int len = 0; 
        int i;
        lps[0] = 0; 
        i = 1;
        while (i < M) 
        {
            if (pat[i] == pat[len]) 
            {
                len++;
                lps[i] = len;
                i++;
            } 
            else 
            {
                if (len != 0) 
                {
                    len = lps[len - 1];
                }
                else // if (len == 0)
                {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }





int analizzaStringa(int id,char *stringa,char *file_test,int num_thread,int dist,int dist_last)
{
        int start = id * dist;
        int end = ((id + 1) * dist) - 1;
        if(id == num_thread - 1)
            end = dist_last + start -1;
        int mat = 0;
        int index_riga = 0;
        FILE *pt;
        pt = fopen(file_test, "r");
        char riga[255];
        char *result;
        while(1)
        {
            result = fgets(riga, 255, pt);
            if( result == NULL )
                break;
            riga[strlen(riga)-1] = '\0';
            if(index_riga >= start && index_riga <= end)
                mat += KMPSearch_S(stringa,riga);
            index_riga++;
        }
        fclose(pt);
    return mat;

}




    void run_s(char *file_testo, char *file_pattern,int num_rows_pattern,int num_rows_file,int thread_count) 
    {
        FILE *pt;
        changeResult(thread_count);
        char riga[50];
        if(num_rows_pattern == 0) //pattern vuoto
        {
            sprintf(riga,"Errore: con input: %s e pattern %s",file_testo,file_pattern);
            openFileResult(RESULT,riga);
            strcpy(riga,"");
            return;
        }
        if(num_rows_file == 0) //testo vuoto
        {
            sprintf(riga,"Errore: con input: %s e pattern %s",file_testo,file_pattern);
            openFileResult(RESULT,riga);
            strcpy(riga,"");
            return;
        }
        sprintf(riga,"Input file: %s",file_testo);
        openFileResult(RESULT,riga);
        strcpy(riga,"");
        //lista di occorrezne
        char riga_testo[255];
        FILE *pp;
        pp = fopen(file_pattern, "r");
        char stringa[35];
        char *result;
        //invio lunghezza file pattern,nome file
        for(int q=1;q<thread_count;q++)
        {
            MPI_Send(&num_rows_pattern,1, MPI_INT,q, 0, MPI_COMM_WORLD);
            MPI_Send(file_testo, 255, MPI_CHAR,q, 0, MPI_COMM_WORLD);
        }
        while(1)
         {
            result = fgets(stringa, 35, pp);
            if( result == NULL )
                break;
            stringa[strlen(stringa)-1] = '\0';
            sprintf(riga,"Pattern: %s",stringa);
            openFileResult(RESULT,riga);
            strcpy(riga,"");
            //faccio analizzare la stringa su tutto il testo
            //conto quante righe ha il testo
            int num_thread;
            int dist;
            int dist_last;
            settaRange(num_rows_file,thread_count,&dist,&dist_last,&num_thread);
            for(int q = 1; q < thread_count; q++)
            {
                MPI_Send(&num_thread, 1, MPI_INT,q, 0, MPI_COMM_WORLD);
                MPI_Send(&dist, 1, MPI_INT,q, 0, MPI_COMM_WORLD);
                MPI_Send(&dist_last, 1, MPI_INT,q, 0, MPI_COMM_WORLD);
            }
            //invio stringa
            for(int q=1;q<num_thread;q++)
                MPI_Send(stringa, 35, MPI_CHAR,q, 0, MPI_COMM_WORLD);
            int occ = analizzaStringa(0,stringa,file_testo,num_thread,dist,dist_last);  //analizzo la mia porzione di testo
            for(int f = 1; f < num_thread; f++)
            {
                int ris;
                MPI_Recv(&ris, 1, MPI_INT, f, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                occ += ris;
            }   
            //stampo
            sprintf(riga_testo,"La stringa %s occorre %d volte",stringa,occ);
            openFileResult(RESULT,riga_testo);
            strcpy(riga_testo,"");
         }
       //vedere se salvo tutte le parole del pattern in modo corretto
       fclose(pp);


    }

//identica a quella sequenziale del file run_mpi_s.c
void insertLinea_var_testo(int thread_count)
{
    //vado a prendere la cartella di var txt.
    //analizzo tutti i file con lo stesso pattern
    char file_name[50];
    char pattern_name[50];
    FILE *tsped_txt;
    tsped_txt = fopen("file_speedup_var_testo.csv","a");
    FILE *teff_txt;
    teff_txt = fopen("file_efficiency_var_testo.csv","a");
    strcpy(pattern_name,"../file/file_var_txt/pattern.txt");
    fprintf(tsped_txt,"%d;",thread_count);
    fprintf(teff_txt,"%d;",thread_count);
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
        //lancio questo file e il pattern prima con un thread,poi 2 poi 4 poi 8 poi 16 poi 32
        int num_txt = getNumRows(i-1); //numero di righe del file
        double m;
        double now = time(NULL);
        double time_es_s = getTime_s(i-1);
        for(int k=0;k<num_cicli;k++)
            run_s(file_name,pattern_name,num_pat,num_txt,thread_count);
        m = difftime(time(NULL), now);
        if (m == 0.0)
            m = 0.1;

        double tm = (double)time_es_s/m;
        double tm2 = (double)tm/thread_count;
        fprintf(tsped_txt,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
        fprintf(teff_txt,"%f;",tm2);
        printf("num_thread = %d e tempo = %f e file %d\n",thread_count,m,i);
        strcpy(file_name,"");
    }
    fprintf(tsped_txt,"\n");
    fprintf(teff_txt,"\n");
    fclose(tsped_txt);
    fclose(teff_txt);
}


//identica a quella sequenziale del file run_mpi_s.c
void insertLinea_var_pattern(int thread_count)
{
    char file_name[50];
   char pattern_name[50];
   strcpy(file_name,"../file/file_var_pt/testo.txt");
   int num_txt = getNumRows(12);             //numero di righe del pattern
   FILE *tsped_pat;
    tsped_pat = fopen("file_speedup_var_pattern.csv","a");
    FILE *teff_pat;
    teff_pat = fopen("file_efficiency_var_pattern.csv","a");
    fprintf(tsped_pat,"%d;",thread_count);
    fprintf(teff_pat,"%d;",thread_count);
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
    double m;
    double now = time(NULL);
    double time_es_s = getTime_s(i+5);
    for(int k=0;k<num_cicli;k++)
        run_s(file_name,pattern_name,num_pat,num_txt,thread_count);
    m = difftime(time(NULL), now);
    if (m == 0.0)
        m = 0.1;
    //printf("time_s: %f\n",time_es_s);
    double tm = (double)time_es_s/m;
    double tm2 = (double)tm/thread_count;
    fprintf(tsped_pat,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
    fprintf(teff_pat,"%f;",tm2);
    printf("num_thread = %d e tempo = %f e file %d\n",thread_count,m,i);
    strcpy(pattern_name,"");
    }
    fprintf(tsped_pat,"\n");
    fprintf(teff_pat,"\n");
     fclose(tsped_pat);
    fclose(teff_pat);
}



int main(int argc, char*argv[])
{
    num_cicli = atoi(argv[1]);
    MPI_Init(NULL, NULL);
    int thread_count;
    MPI_Comm_size(MPI_COMM_WORLD, &thread_count);
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    if(id == 0) //avvio il programma
    {
        insertLinea_var_testo(thread_count);
        insertLinea_var_pattern(thread_count);
    }
    else   //ricevo i dati dal thread 0
    {
        for(int j=0;j<2;j++)
        {
            for(int k = 0;k<variazioni_dim;k++)
            {
                for(int h=0;h<num_cicli;h++)
                {

                    int len;
                    MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    char testo[255];
                    MPI_Recv(testo, 255, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    char pattern[35];
                    
                    for(int i=0;i<len;i++)
                    {
                        int num_thread;
                        int dist;
                        int dist_last;
                        MPI_Recv(&num_thread, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        MPI_Recv(&dist, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        MPI_Recv(&dist_last, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        if( id < num_thread)
                        {
                            MPI_Recv(pattern, 35, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            int occ = analizzaStringa(id,pattern,testo,num_thread,dist,dist_last);
                            MPI_Send(&occ, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                        }
                            
                    }
                }
            }
        }
        
    }

    MPI_Finalize();
    return 0;
}
