#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>


char  RESULT[40] = "result_parallelo_1.txt";
char *match = "match.txt";



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


void computeLPSArray(char *pat, int M, int *lps);


void openFileResult(char *name,char *txt)
{
    FILE *fp;
    fp = fopen(name, "a");
    fprintf(fp, "%s\n", txt);
    fclose(fp);
}


int KMPSearch_S(char *pat, char *txt,int index_riga) {

        int M = strlen(pat);
        int mat = 0;
        int N = strlen(txt);
        char riga[50];
    
        // create lps[] that will hold the longest prefix suffix values for pattern

        int *lps = (int *) malloc(sizeof(int) * M);

        int j = 0; // index for pat[]
        // Preprocess the pattern (calculate lps[] array)
        computeLPSArray(pat, M, lps);         
        int i = 0; // index for txt[]

        while (i < N) { //finche ho caratteri da analizzare nel testo



            if (pat[j] == txt[i]) { //se il carattere del pattern è uguale al carattere del testo
                j++;
                i++;

            }

            if (j == M) {  //se ho analizzato tutto il pattern, ho trovato una corrispondenza


                    sprintf(riga,"Match trovato ad index %d della riga %d",i - j,index_riga);
                    openFileResult(match,riga);
                    strcpy(riga,"");
                mat++;
                j = lps[j - 1];

            }


            // mismatch after j matches

            else if (i < N && pat[j] != txt[i]) {

                // Do not match lps[0..lps[j-1]] characters,

                // they will match anyway

                if (j != 0)

                    j = lps[j - 1];

                else

                    i = i + 1;

            }

        }

    free(lps); // to avoid memory leak
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








int analizzaStringa(char *stringa,char *file_test)
{

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
            mat += KMPSearch_S(stringa,riga,index_riga);
            index_riga++;
        }
    fclose(pt);
    return mat;
}




    void run_s(char *file_testo, char *file_pattern,int num_rows_pattern,int num_rows_file) 
    {
        FILE *pt;
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
            int occ = analizzaStringa(stringa,file_testo);  
            //stampo
            sprintf(riga_testo,"La stringa %s occorre %d volte",stringa,occ);
            openFileResult(RESULT,riga_testo);
            strcpy(riga_testo,"");
         }
       //vedere se salvo tutte le parole del pattern in modo corretto
       fclose(pp);


    }
