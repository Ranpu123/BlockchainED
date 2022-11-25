#include <stdio.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>

#include "mtwister.h"
#include "hashUtils.h"

typedef struct BlocoNaoMinerado{
  unsigned int numero;//4
  unsigned int nonce;//4
  unsigned char data[184];//nao alterar. Deve ser inicializado com zeros.
  unsigned char hashAnterior[SHA256_DIGEST_LENGTH]; //32
} BlocoNaoMinerado;

typedef struct BlocoMinerado{
    BlocoNaoMinerado bloco;
    unsigned char hash[SHA256_DIGEST_LENGTH];
}BlocoMinerado;

MTRand randNumber;

int wallet[256] = {0};

void searchBlock(int index, BlocoMinerado * bloco){
    FILE * pFile = fopen("blockchain.dat","rb");
    fseek(pFile, (index-1)*sizeof(BlocoMinerado), SEEK_SET);
    fread(bloco, sizeof(BlocoMinerado), 1, pFile);
}

void generateBlockData(unsigned char * data){
    unsigned long qtdTransacoes = (1 + (genRandLong(&randNumber) % 61)) * 3;
    for (int i = 3; i<=qtdTransacoes; i+=3){
        unsigned char endOrigem = (unsigned char) genRandLong(&randNumber) % 256;
        unsigned char qtdBitcoin = (unsigned char) (1 + (genRandLong(&randNumber) % 50));
        unsigned char endDst = (unsigned char) genRandLong(&randNumber) % 256;

        data[i-3] = endOrigem;
        data[i-2] = qtdBitcoin;
        data[i-1] = endDst;

        if((unsigned long) qtdBitcoin >= wallet[(unsigned long)endOrigem]){
            wallet[(unsigned long)endOrigem] = 0;
        }else{
            wallet[(unsigned long)endOrigem] -= (unsigned long)qtdBitcoin; 
        }
        wallet[(unsigned long)endDst] += (unsigned long)qtdBitcoin;

    }
    /*for (int i = qtdTransacoes; i<184; i++){
        data[i] = 0;
    }*/
}

void mineBlock(BlocoNaoMinerado * blocoAMinerar, unsigned char * h){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int flag = 1;
    while(flag){
        
        //Gera o hash do bloco.
        SHA256((unsigned char *)&(*blocoAMinerar), sizeof((*blocoAMinerar)),hash);
        
        //Verifica se os 3 primeiras posições do vetor são 0.
        for(int i=0; i < 3 ;i++){
            if((unsigned long)hash[i] == 0){
                flag = 0;
            }else{
                flag = 1;
                break;
            }
        }

        if(flag){
            blocoAMinerar->nonce+=1;
        }
    }
    cpyhash(h,hash);
}

void createBlock(BlocoNaoMinerado * blocoAMinerar, int i, unsigned char * hash){
    blocoAMinerar->numero = i;
    blocoAMinerar->nonce = 0;
    cpyhash(blocoAMinerar->hashAnterior,hash);
    generateBlockData(blocoAMinerar->data);
    mineBlock(blocoAMinerar,hash);
}

int main(){
    
    randNumber = seedRand(1234567);
    
    //Ultima Hash aceita gerada. No genesis o hash anterior é 0000...
    unsigned char hash [SHA256_DIGEST_LENGTH] = {0};

    BlocoMinerado buffer[16];
    int cont = 0;
    
    //Arquivo Binário com os dados da blockchain
    FILE * pFile = fopen("blockchain.dat","ab");
    

    for(int i = 1; i<=16; i++){

        BlocoNaoMinerado blocoAMinerar;
        //Gera os dados e minera o bloco.
        createBlock(&blocoAMinerar,i,hash);
        
        //Carrega o buffer 4096||512 bytes com o bloco.
        buffer[cont].bloco = blocoAMinerar;
        cpyhash(buffer[cont].hash, hash);
        
        printhash(blocoAMinerar.hashAnterior);
        printhash(hash);

        //Se a quantidade de blocos for 16||2, então grava no arquivo.
        if (15 == cont){
            fwrite(buffer,sizeof(BlocoMinerado),16,pFile);
            cont = 0;
        }else{
            cont ++;
        }
    }

    for (int i = 0; i<256; i++){
        printf("|%d|", wallet[i]);
    }

    fclose(pFile);
    return 0;
}