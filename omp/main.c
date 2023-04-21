#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "find_match_omp_p.c"

int different_thread = 6; //numero di thread che voglio testare(1,2,4,8,16,32)
int variazioni_dim = 6; //numero di dimensioni diversi di file e pattern che voglio testare
int num_cicli;

//ritorna numero righe
int getNumRows(char *file_testo) 
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

/*
crea la tabella con variazione del file di testo
Itero sul numero di file in input 
per ogni iterazione mi prendo il rispettivo file
all'interno itero sul numero di thread
dentro itero su num cicli: eseguo la funzione di controllo match num cicli volte per ogni esecuzione con un numero fisso di thread
salvo nella tabella i risultati di speedup ed efficency
Ogni file viene eseguito con diverse dimensioni di thread(1,2,4,8,16,32) 
ogni esecuzione con x thread viene fata num cicli volte(per evidenziare la differenza tra diversi thread)
*/
void creaTabella_var_txt()
{

    char file_name[50];
    char pattern_name[50];
    strcpy(pattern_name,"../file/file_var_txt/pattern.txt");
    int num_pat = getNumRows(pattern_name); //numero di righe del pattern
    FILE *tsped_txt;
    tsped_txt = fopen("file_speedup_var_testo.csv","a");
    fprintf(tsped_txt,"Tabella speedup variazione dimensione file di testo\n" );
    fprintf(tsped_txt,"size/num_thread;1;2;4;8;16;32\n");
    FILE *teff_txt;
    teff_txt = fopen("file_efficiency_var_testo.csv","a");
    fprintf(teff_txt,"Tabella efficiency variazione dimensione file di testo\n" );
    fprintf(teff_txt,"size/num_thread;1;2;4;8;16;32\n");
    //mi salvo il nome del file che devo analizzare
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
    int num_txt = getNumRows(file_name); //numero di righe del file
    int num_thread = 1;
    int write = 0;
    fprintf(tsped_txt,"%d;",num_txt*num_cicli);
    fprintf(teff_txt,"%d;",num_txt*num_cicli);
    int time_esecution_seq;
    //itero variando il numero di thread con cui voglio eseguirlo
    for(int j=1;j<=different_thread;j++)
    {
        double m;
        double now = time(NULL);
        if(num_thread == 1) 
            write = 1;
        for(int k=0;k<num_cicli;k++)
            run_p(file_name,pattern_name,num_thread,num_pat,num_txt,write);
        m = difftime(time(NULL), now);
        if (m == 0.0)
            m = 0.1;
        if(num_thread == 1)
            time_esecution_seq = m;
        //calcolo speedup ed efficency
        double tm = (double)time_esecution_seq/m;
        double tm2 = (double)tm/num_thread;
        fprintf(tsped_txt,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
        fprintf(teff_txt,"%f;",tm2);
        printf("num_thread = %d e tempo = %f e file %d\n",num_thread,m,i);
        num_thread = num_thread * 2;
    }
    strcpy(file_name,"");
    fprintf(tsped_txt,"\n");
    fprintf(teff_txt,"\n");
    }
    fclose(tsped_txt);
    fclose(teff_txt);
}

//qui varia il pattern,fisso il testo e ciclo su pattern
//uguale a quella sopra
void creaTabella_var_pattern()
{
   char file_name[50];
   char pattern_name[50];
   strcpy(file_name,"../file/file_var_pt/testo.txt");
   int num_txt = getNumRows(file_name); //numero di righe del testo
   FILE *tsped_pat;
    tsped_pat = fopen("file_speedup_var_pat.csv","a");
    fprintf(tsped_pat,"Tabella speedup variazione dimensione pattern\n" );
    fprintf(tsped_pat,"size/num_thread;1;2;4;8;16;32\n");
    FILE *teff_pat;
    teff_pat = fopen("file_efficiency_var_pat.csv","a");
    fprintf(teff_pat,"Tabella efficiency variazione dimensione pattern\n" );
    fprintf(teff_pat,"size/num_thread;1;2;4;8;16;32\n");
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

    int num_pat = getNumRows(pattern_name); //numero di righe del file
    int num_thread = 1;
    int write = 0;
    fprintf(tsped_pat,"%d;",num_pat*num_cicli);
    fprintf(teff_pat,"%d;",num_pat*num_cicli);
    int time_esecution_seq;
    for(int j=1;j<=different_thread;j++)
    {
        double m;
        double now = time(NULL);
        if(num_thread == 1) 
            write = 1;
        for(int k=0;k<num_cicli;k++)
            run_p(file_name,pattern_name,num_thread,num_pat,num_txt,write);
        m = difftime(time(NULL), now);
         if (m == 0.0)
            m = 0.1;
        if(num_thread == 1)
            time_esecution_seq = m;
        double tm = (double)time_esecution_seq/m;
        double tm2 = (double)tm/num_thread;
        fprintf(tsped_pat,"%f;",tm);   //lo speedup di dim n e num thread p = Tempo sequenziale con inp n / Tempo parallelo con inp n e thread p
        fprintf(teff_pat,"%f;",tm2);
         printf("num_thread = %d e tempo = %f e file %d\n",num_thread,m,i);
        num_thread = num_thread * 2;
    }
    strcpy(pattern_name,"");
    fprintf(tsped_pat,"\n");
    fprintf(teff_pat,"\n");
    }
     fclose(tsped_pat);
    fclose(teff_pat);
}



//funzione main che crea le due tabelle
int main(int argc, char* argv[])
{
    num_cicli = atoi(argv[1]);
    creaTabella_var_txt();
    creaTabella_var_pattern();
    return 0;
}
