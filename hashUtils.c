#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <math.h>

#include "hashUtils.h"

void cpyhash(unsigned char * dst, unsigned char * org){
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        dst[i]=org[i];
    }
}

void printhash(unsigned char * hash){
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        printf("%02x", hash[i]);
    }
    printf("\n");
}

//Carrega o vetor com os valores de cada carteira da blockchain
void loadWallet(int * wallet){
    FILE * pFile = fopen("data/blockchain.dat","rb");
    fseek(pFile, -1024L, SEEK_END);
    fread(wallet, sizeof(int), 256, pFile);
    fclose(pFile);
}

//Carrega o bloco com o indice informado
BlocoMinerado searchBlock(int index){
    //Retorna o bloco com os dados da caretira.
    //Se a busca não encontrar nenhum valor, retorna um bloco com valor -1 armazenado.

    BlocoMinerado aux;
    aux.bloco.numero = -1;

    FILE * pFile = fopen("data/blockchain.dat","rb");
    fseek(pFile, -1024L, SEEK_END);
    int sz = ftell(pFile)/sizeof(BlocoMinerado);
    if(index > 0 && index <=sz){
        fseek(pFile, (index-1)*sizeof(BlocoMinerado), SEEK_SET);
        fread(&aux, sizeof(BlocoMinerado), 1, pFile);  
    }else{
        printf("\nERRO: Indice inválido: Valor deve ser maior que 0, e menor ou igual a %d \n",sz);
    }
    fclose(pFile);
    return aux;
}

//MergeSort para o vetor de struct wallet
void mergeSort(Wallet * A, int p, int r){
    if (p<r){
        int q = floor((p+r)/2);
        mergeSort(A, p, q);
        mergeSort(A, q+1, r);
        merge(A, p, q, r);
    }
}

//Função para juntar ordenando o vetores.
void merge(Wallet * A, int p, int q, int r){
    int mergeSize = r - p + 1;
    int end1 = 0, end2 = 0;
    int leftPos = p;
    int rightPos = q+1;
    Wallet * temp = (Wallet *)malloc(mergeSize*sizeof(Wallet));

    if(temp != NULL){
        for(int i = 0; i<mergeSize; i++){
            if(!end1 && !end2){
                temp[i] = (A[leftPos].valor < A[rightPos].valor)? A[leftPos++]:A[rightPos++];
                if(leftPos>q){
                    end1=1;
                }
                if(rightPos>r){
                    end2=1;
                }
            }else{
                temp[i]= (!end1)? A[leftPos++]:A[rightPos++];
            }
        }
        int j = 0,k = 0;
        for(j = 0, k = p; j < mergeSize; j++, k++){
            A[k]=temp[j];
        }
    }

    free(temp);

    return;

}

