
//Strcuts
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

void cpyhash(unsigned char * dst, unsigned char * org);
void printhash(unsigned char * hash);
void loadWallet(int * wallet);
BlocoMinerado searchBlock(int index);