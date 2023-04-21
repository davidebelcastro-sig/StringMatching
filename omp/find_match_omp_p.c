#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include<omp.h>


char  RESULT[40];
char *match = "match.txt";
int num_thread;
int dist;
int dist_last;
int tot_mat = 0;

void computeLPSArray(char *pat, int M, int *lps);


/*
Questa funzione si occupa di dividere il lavoro tra i vari thread, prende come argomento la dimensione del problema(len) 
i thread(comm_sz) e tre puntatori a zone di memoria: in dist andrà salvato la quantità di lavoro di tutti i thread meno l'ultimo
l'ultimo invece in dist_last,num thread invece rappresenta il numero di effettivi thread che userò: può essere <= a comme sz
Se len = x e comm_sz a y con y > x allora userò x thread e ognuno avrà un solo elemento(gli altri thread non mi serviranno
*/
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
            *(dist_last) = *(dist) + (len - (*(dist) *  comm_sz));
        }
        else
            *(dist) = len / comm_sz;
    }
    if (*(dist_last) == 0)
        *(dist_last) = *(dist); 
}



//scrive su file
void openFileResult(char *name,char *txt)
{
    FILE *fp;
    fp = fopen(name, "a");
    fprintf(fp, "%s\n", txt);
    fclose(fp);
}

//algortimo KMP
int KMPSearch_S(char *pat, char *txt,int index_riga,int write) {

        int M = strlen(pat);
        int mat = 0;
        int N = strlen(txt);
        char riga[50];
        int *lps = (int *) malloc(sizeof(int) * M);
        int j = 0;
        computeLPSArray(pat, M, lps);         
        int i = 0; 
        while (i < N) 
        { 
            if (pat[j] == txt[i]) 
            { 
                j++;
                i++;
            }

            if (j == M) 
            {  

                if(write == 1)
                {
                    sprintf(riga,"Match trovato ad index %d della riga %d",i - j,index_riga);
                    openFileResult(match,riga);
                    strcpy(riga,"");
                }
                mat++;
                j = lps[j - 1];

            }
            else if (i < N && pat[j] != txt[i]) 
            {
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







/*
prende in input una stringa pattern,il file di testo,il numero di thread ed esegue in parallelo la ricerca
Ogni thread legge il file e quando una riga è nel range del suo lavoro allora applica l'algortimo di string matching
ritorna il numero di occorrenze in una variabile privata che alla fine andrà a sommare a una globale tramite direttiva atomic
ritorna il numero di match trovati
*/
int analizzaStringa(char *stringa,char *file_test,int thread_count,int len,int write)
{
    settaRange(len,thread_count,&dist,&dist_last,&num_thread);
    # pragma omp parallel num_threads(num_thread)   
    {
        int id = omp_get_thread_num();
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
                mat += KMPSearch_S(stringa,riga,index_riga,write);
            index_riga++;
        }
        fclose(pt);
        # pragma omp atomic
        tot_mat += mat;
    }
    int var_appo = tot_mat;
    tot_mat = 0;
    return var_appo;
}

//ogni thread_count ha il suo file di risultato
void changeResult(int thread_count)
{
     strcpy(RESULT,"");
    if(thread_count == 1)
       strcpy(RESULT,"result_parallelo_1.txt");
    else if(thread_count == 2)
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

/*prendo in input il file di testo,pattern,numero di thread e dimensioni dei file pattern e testo
Controllo che i file non siano vuoti
scrivo su file match il file che sto analizzando
apro il file pattern e,per ogni riga, scrivo sul file di match il pattern 
chiamo la funzione di analizza stringa per quel pattern
scrivo sul file match il numero di occorrenze
*/
    void run_p(char *file_testo, char *file_pattern,int thread_count,int num_rows_pattern,int num_rows_file,int write) 
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
        char riga_testo[255];
        FILE *pp;
        pp = fopen(file_pattern, "r");
        char stringa[35];
        char *result;
        //leggo il file pattern riga per riga
        while(1)
         {
            result = fgets(stringa, 35, pp);
            if( result == NULL )
                break;
            stringa[strlen(stringa)-1] = '\0';
            sprintf(riga,"Pattern: %s",stringa);
            openFileResult(RESULT,riga);
            strcpy(riga,"");
            //ritorno il numero di match della riga j-esima
            int occ = analizzaStringa(stringa,file_testo,thread_count,num_rows_file,write);  
            //stampo
            sprintf(riga_testo,"La stringa %s occorre %d volte",stringa,occ);
            openFileResult(RESULT,riga_testo);
            strcpy(riga_testo,"");
         }
       fclose(pp);


    }
