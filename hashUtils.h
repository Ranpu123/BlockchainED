
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

typedef struct Wallet{
  int valor;
  unsigned char endereco;
}Wallet;

void cpyhash(unsigned char * dst, unsigned char * org);
void printhash(unsigned char * hash);

void loadWallet(int * wallet);

void mergeSort(Wallet * A, int p, int r);
void merge(Wallet * A, int p, int q, int r);

BlocoMinerado searchBlock(int index);