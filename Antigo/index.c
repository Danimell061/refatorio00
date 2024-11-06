#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char numero[255];
    char data_ajuizamento[255];
    int id_classe[8];
    int id_assunto[8];
    int ano_eleicao;
} Processo;

typedef struct{
    int ano;
    int mes;
    int dia;
} Data;

Processo *criarProcesso(int numLinhas);
int contLinhas(FILE *fp);
void lerCsv(FILE *fp, Processo *p, int numLinhas);
void ordenarIdCsv(Processo *p, int n);
void ordenarDataCsv(Processo *p, int n);
int contClasse(Processo *p, int n, int id);
int idAssuntos(Processo *p, int n);
int diasProcesso(Processo *p, int n, int id);
void salvarCsv(Processo *p, int n, char *nome_arquivo);

int main(){
    FILE *fp;
    
    system("cls");
    
    fp = fopen("processo_043_202409032338.csv", "r");
    if(fp == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    }

    int numLinhas = contLinhas(fp) - 1;
    Processo *p = criarProcesso(numLinhas);

    // ------------------------------------------------------------------


    lerCsv(fp, p, numLinhas);

    printf("Quantidade de processos com a classe %d: %d\n", 11533, contClasse(p,numLinhas,11533));
    printf("Quantidade de assuntos diferentes: %d\n", idAssuntos(p,numLinhas));

    printf("Dias do processo em tramitação até 27/09/2024: %d \n",diasProcesso(p,numLinhas,405280783));

    ordenarIdCsv(p, numLinhas);
    salvarCsv(p, numLinhas, "ordenandoID.csv");


    ordenarDataCsv(p, numLinhas);
    salvarCsv(p, numLinhas, "ordenandoDATA.csv");

    free(p);
    fclose(fp);
}


Processo *criarProcesso(int numLinhas) {
    Processo *p = (Processo*) malloc(numLinhas * sizeof(Processo));
    if(p == NULL){
        printf("Erro na alocação de memória (malloc)");
        exit(1);
    }

    
    memset(p->numero, 0, sizeof(p->numero));
    memset(p->data_ajuizamento, 0, sizeof(p->data_ajuizamento));
    p->id = 0;
    p->ano_eleicao = 0;
    return p;
}

int contLinhas(FILE *fp){
    char linha[255];
    int i;
    for(i = 0; fgets(linha, sizeof(linha), fp) != NULL; i++);
    rewind(fp);
    return i;
}

void lerCsv(FILE *fp, Processo *p, int numLinhas){
    int i, j, cont, virg[2] = {0, 0}, fChave = 0;
    char linha[255];
    char *rem;

    // Faz o L
    fgets(linha, sizeof(linha), fp);

    for(i = 0; i < numLinhas; i++){
        if(fgets(linha, sizeof(linha), fp) == NULL){
            break;
        };
        
        cont = 0;
        virg[0] = 0;
        virg[1] = 0;
        fChave = 0;

        // O CODIGO
        for(j = 0; j < strlen(linha); j++){
            if(linha[j] == '{'){
                int contagem = 0;
                for(int k = j + 1; linha[k] != '}'; k++){
                    if (linha[k] == ','){
                        contagem++;
                    }
                }
                if (fChave == 0) {
                    virg[0] = contagem;
                    fChave = 1;
                } else {
                    virg[1] = contagem;
                    break;
                }
            }
        }
        // O CODIGO

        rem = strtok(linha, ",\"{}");
        while(rem != NULL && cont < 6){
            switch(cont){
                case 0:
                    p[i].id = atoi(rem);
                    break;
                case 1:
                    strcpy(p[i].numero, rem);
                    break;
                case 2:
                    strcpy(p[i].data_ajuizamento, rem);
                    break;
                case 3:
                    for(j = 0; j <= virg[0] && j < 8; j++){
                        p[i].id_classe[j] = atoi(rem);
                        rem = strtok(NULL, ",\"{}");
                    }
                    cont++;
                    continue;
                case 4:
                    for(j = 0; j <= virg[1] && j < 8; j++){
                        p[i].id_assunto[j] = atoi(rem);
                        rem = strtok(NULL, ",\"{}");
                    }
                    cont++;
                    continue;
                case 5:
                    p[i].ano_eleicao = atoi(rem);
                    break;
            }
            rem = strtok(NULL, ",\"{}");
            cont++;
        }
    }
}

void ordenarIdCsv(Processo *p, int n){
        int i, j;
        Processo temp;
        //Ira ler termo por termo
        for (i = 0; i < n; i++) {
            //Ira comparar termo por termo
            for (j = 0; j < n-1; j++) {
                // Troca os elementos
                if (p[j].id > p[j + 1].id){
                    temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                }
            }
        }
}

void ordenarDataCsv(Processo *p, int n){
        int i, j;
        Processo temp;
        //Ira ler termo por termo
        for (i = 0; i < n; i++) {
            //Ira comparar termo por termo
            for (j = 0; j < n-1; j++) {
                // Troca os elementos
                if (strcmp(p[j].data_ajuizamento, p[j + 1].data_ajuizamento) < 0){
                    temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                }
            }
        }
}

int contClasse(Processo *p, int n, int id){
    int cont;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 8; j++){
            if(p[i].id_classe[j] == id){
                cont++;
            }
        }
    }
    return cont;
}

int idAssuntos(Processo *p, int n) {
    int *assuntos_unicos = malloc((4 * (n * sizeof(int))));

    int count = 0;

    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < 8; j++) { 
            int assunto = p[i].id_assunto[j];
            if (assuntos_unicos[assunto] != 0 || p[i].id_assunto[j] == 0) {
                continue;
            }
            assuntos_unicos[assunto] = 1;
            count++;
        }
    }

    free(assuntos_unicos);
    return count;
}

int diasProcesso(Processo *p, int n, int id){
    int proc;
    int finalD1 = 0, finalD2 = 0;
    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    Data d1;
    Data d2 = {2024, 9, 27};

    
    for(int i = 0; i<n; i++){
        if(id == p[i].id){
            proc = i;
            break;
        }
    }
    char data[sizeof(p[proc].data_ajuizamento)];
    strcpy(data, p[proc].data_ajuizamento);
    sscanf(data, "%d-%d-%d", &d1.ano, &d1.mes, &d1.dia);



    for (int i = 0; i < d1.ano; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
            finalD1 += 366;
        } else {
            finalD1 += 365;
        }
    }

    for (int i = 0; i < d2.ano; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
            finalD2 += 366;
        } else {
            finalD2 += 365;
        }
    }


    for (int i = 1; i < d1.mes; i++) {
        if (i == 2 && ((d1.ano % 4 == 0 && d1.ano % 100 != 0) || (d1.ano % 400 == 0))) {
            finalD1 += 29;
        } else {
            finalD1 += dias_mes[i - 1];
        }
    }


    for (int i = 1; i < d2.mes; i++) {
        if (i == 2 && ((d2.ano % 4 == 0 && d2.ano % 100 != 0) || (d2.ano % 400 == 0))) {
            finalD2 += 29;
        } else {
            finalD2 += dias_mes[i - 1]; 
        }
    }

  
    finalD1 += d1.dia;
    finalD2 += d2.dia;
    

    return finalD2 - finalD1;
}
void salvarCsv(Processo *p, int n, char *nome_arquivo){
    FILE *fp = fopen(nome_arquivo, "w");
    int j, x;

    if (fp == NULL){
        printf("Error ao criar o arquivo %s\n", nome_arquivo);
        return;
    }

    fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < n; i++){
        fprintf(fp, "%d,\"%s\",%s", 
               p[i].id, p[i].numero, p[i].data_ajuizamento);

        for(j = 0; j < 8 && p[i].id_classe[j] != 0; j++);

        if(j > 1){
            fprintf(fp, ",\"{");
        }else{
            fprintf(fp, ",{");
        }
        for(int k = 0; k < 8 && p[i].id_classe[k] != 0; k++){
            if(p[i].id_classe[k + 1] == 0){
                fprintf(fp,"%d", p[i].id_classe[k]);
            }else{
                fprintf(fp,"%d,", p[i].id_classe[k]);
            }
        }
        if(j > 1){
            fprintf(fp, "}\",");
        }else{
            fprintf(fp, "},");
        }
        //-------------------------------------------------------------
        for(x = 0; x < 8 && p[i].id_assunto[x] != 0; x++);

        if(x > 1){
            fprintf(fp, "\"{");
        }else{
            fprintf(fp, "{");
        }
        for(int k = 0; k < 8 && p[i].id_assunto[k] != 0; k++){
            if(p[i].id_assunto[k + 1] == 0){
                fprintf(fp,"%d", p[i].id_assunto[k]);
            }else{
                fprintf(fp,"%d,", p[i].id_assunto[k]);
            }
        }
        if(x > 1){
            fprintf(fp, "}\",");
        }else{
            fprintf(fp, "},");
        }

        fprintf(fp, "%d\n", p[i].ano_eleicao);
    }

    rewind(fp);
    fclose(fp);
}