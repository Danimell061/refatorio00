#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) return NULL;

    Estrada *estrada = malloc(sizeof(Estrada));
    if (!estrada) return NULL;

    fscanf(file, "%d", &estrada->T);
    fscanf(file, "%d", &estrada->N);

    estrada->C = malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        free(estrada);
        fclose(file);
        return NULL;
    }

    //restrições de tamanho
    if (estrada->T < 3 || estrada->T > 1000000 || estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(file);
        return NULL;
    }

    int *posicoes = malloc(estrada->N * sizeof(int));
    if (!posicoes) {
        free(estrada->C);
        free(estrada);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        fscanf(file, "%d", &estrada->C[i].Posicao);

        //restrições se esta correto a sua posição
        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }

        //restrição se repete
        for (int j = 0; j < i; j++) {
            if (posicoes[j] == estrada->C[i].Posicao) {
                free(posicoes);
                free(estrada->C);
                free(estrada);
                fclose(file);
                return NULL;
            }
        }

        posicoes[i] = estrada->C[i].Posicao;

        fgetc(file);  // Consome o caractere de nova linha
        fgets(estrada->C[i].Nome, sizeof(estrada->C[i].Nome), file);
        estrada->C[i].Nome[strcspn(estrada->C[i].Nome, "\n")] = '\0';
        fflush(stdin);
    }
    
    //ordenando
    for (int o = 0; o < estrada->N; o++) {
        for (int p = 0; p < estrada->N - 1; p++) {
            if (estrada->C[p].Posicao > estrada->C[p + 1].Posicao) {
                Cidade temp = estrada->C[p];
                estrada->C[p] = estrada->C[p + 1];
                estrada->C[p + 1] = temp;
            }
        }
    }
    free(posicoes);
    fclose(file);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1;

    double menorVizinhanca = estrada->T;  // Inicializa com o valor máximo possível

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca;

        //PRIMEIRA
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao + estrada->C[i].Posicao) / 2.0;
        }

        else if (i == estrada->N - 1) {
            vizinhanca = (estrada->T - (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0);
        }

        else {
            double distEsq = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            double distDir = (estrada->C[i + 1].Posicao + estrada->C[i].Posicao) / 2.0;
            vizinhanca = distDir - distEsq;
        }
        
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }
    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    double menorVizinhanca = estrada->T;
    int cidadeIndex = -1;

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca;

        //PRIMEIRA
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao + estrada->C[i].Posicao) / 2.0;
        }

        else if (i == estrada->N - 1) {
            vizinhanca = (estrada->T - (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0);
        }

        else {
            double distEsq = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            double distDir = (estrada->C[i + 1].Posicao + estrada->C[i].Posicao) / 2.0;
            vizinhanca = distDir - distEsq;
        }
        
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            cidadeIndex = i;
        }
    }

    char *cidadeComMenorVizinhanca = malloc(strlen(estrada->C[cidadeIndex].Nome) + 1);
    strcpy(cidadeComMenorVizinhanca, estrada->C[cidadeIndex].Nome);

    free(estrada->C);
    free(estrada);
    return cidadeComMenorVizinhanca;
}