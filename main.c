#include <stdio.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>

#include "mtwister.h"
#include "hashUtils.h"

typedef struct vetorordenado{
    int valor;
    int posicao;
}vetorordenado;

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
    do{
        blocoAMinerar->nonce += 1;

        //Gera o hash do bloco.
        SHA256((unsigned char *)&(*blocoAMinerar), sizeof((*blocoAMinerar)),hash);

    //Verifica se os 3 primeiras posições do vetor são 0.
    }while(((unsigned long)hash[0] + (unsigned long)hash[1] + (unsigned long)hash[2] != 0));
    cpyhash(h,hash);
}

void createBlock(BlocoNaoMinerado * blocoAMinerar, int i, unsigned char * hash){
    blocoAMinerar->numero = i;
    blocoAMinerar->nonce = -1;
    cpyhash(blocoAMinerar->hashAnterior,hash);
    generateBlockData(blocoAMinerar->data);
    mineBlock(blocoAMinerar,hash);
}

int carregavetor(const void *a, const void *b){
    vetorordenado *a1 = (vetorordenado *)a;
    vetorordenado *a2 = (vetorordenado *)b;
    if ((*a1).valor < (*a2).valor)
        return -1;
    else if ((*a1).valor > (*a2).valor)
        return 1;
    else
        return 0;
}

int main(){
    int z, aux;

    randNumber = seedRand(1234567);
    
    //Ultima Hash aceita gerada. No genesis o hash anterior é 0000...
    unsigned char hash [SHA256_DIGEST_LENGTH] = {0};

    BlocoMinerado buffer[16];
    int cont = 0;
    
    //Arquivo Binário com os dados da blockchain
    FILE * pFile = fopen("blockchain.dat","wb");
    
    printf("Aguarde o menu para utilizar o programa\n");
    for(int i = 1; i<=16; i++){

        BlocoNaoMinerado blocoAMinerar;
        //Gera os dados e minera o bloco.
        createBlock(&blocoAMinerar,i,hash);
        
        //Carrega o buffer 4096||512 bytes com o bloco.
        buffer[cont].bloco = blocoAMinerar;
        cpyhash(buffer[cont].hash, hash);
        
        //printhash(blocoAMinerar.hashAnterior);
        //printhash(hash);

        //Se a quantidade de blocos for 16||2, então grava no arquivo.
        if (15 == cont){
            fwrite(buffer,sizeof(BlocoMinerado),16,pFile);
            cont = 0;
        }else{
            cont ++;
        }
    }

    vetorordenado vet[256];
    for(int i = 0; i<256; i++){
        vet[i].valor = wallet[i];
        vet[i].posicao = i;
    }

    qsort(vet, 256, sizeof(vet[0]), carregavetor);

    BlocoMinerado blocoaux;

    do{

    printf("\n\t\t||Menu||");
    printf("\n1-Imprimir dados de um bloco.");
    printf("\n2-Mostrar a quantidade de Bitcoins de um endereço.");
    printf("\n3-Mostrar a carteira com mais Bitcoins.");
    printf("\n4-Listar endereços em ordem crescente (Relativa a quantidade de bitcoins).");
	printf("\n5-Sair\n");

    scanf("%d", &z);

    switch(z){
        case 1:
            do{
                printf("\nInsira o bloco desejado (de 1 até 100.000):\n");
                scanf("%d", &aux);
            }while (aux>100000 || aux<0);
            searchBlock(aux, &blocoaux);
            printf("\nHash: %p, Numero: %d, Nonce: %d, Dados: %p", blocoaux.hash, blocoaux.bloco.numero, blocoaux.bloco.nonce, blocoaux.bloco.data);
        break;

        case 2:
            do{
                printf("\nInsira o endereço desejado (de 0 até 255):\n");
                scanf("%d", &aux);
            }while (aux>255 || aux<1);
            printf("\nO valor em Bitcoins do endereço %d eh de BTC$:%d,00 \n", aux, wallet[aux]);
        break;
		
		case 3:
            printf("\nA carteira com mais bitcoins eh a carteira de numero %d, que contém: BTC:%d,00\n", vet[255].posicao, vet[255].valor);
		break;

		case 4:
            printf("\nQuantidades de BTC ordenadas de menor para maior: \n");
            for (int i = 0; i<256; i++){
                printf("|Wallet: %d = BTC$:%d,00|\n", vet[i].posicao, vet[i].valor);
            }
		break;
      
    }
    }while (z!=5);

    fclose(pFile);
    return 0;
}