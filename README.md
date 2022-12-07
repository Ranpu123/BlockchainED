# Projeto de Blockchain ED2

Projeto realizado para a disciplina de Etrutuda de Dados 2, buscando simular uma Blockchain e permitir consultas solicitadas pelo Professor.

### 🔧 Instalação

É necessária a instalação da biblioteca ssl-dev, escrevendo o seguinte comando no terminal Linux:

```
sudo apt-get install libssl-dev
```

## ⚙️ Executando a compilação

Para a execução da compilação do código é necessário acessar a pasta onde se encontra este repositório e o arquivo "main.c" no terminal do Linux e executar o seguinte comando:

```
gcc main.c hashUtils.c mtwister.c -lcrypto -o bin/main
```

### ⌨️ Executando o programa para realizar as consultas

Para a execução do código é necessário acessar a pasta onde se encontraeste repositório e o arquivo "main.c" no terminal do Linux e executar o seguinte comando:

```
bin/main
```

Após isso aguarde o "Menu" aparecer para inserir inputs.

## 🖇️ Código Externo

Foram utilizados os seguintes códigos fonte fornecidos pelo professor para a execução deste projeto:

* [Mtwister](https://github.com/ESultanik/mtwister) - Código fonte fornecido, utilizado para executar as transações dado o seed "1234567".

## ✒️ Autores

O projeto foi desenvolvido pelos seguintes alunos da Disciplina de Estrutura de Dados 2 da UTFPR Ponta Grossa:

* **Guilherme Portela Meller** - *Implantação das consultas solicitadas* - [Guilherme Portela](https://github.com/guilhermemeller)
* **Fulano De Tal** - *Implantação do codigo de Huffman para a compressão de dados* - [fulanodetal](https://github.com/linkParaPerfil)

Você também pode ver a lista de todos os [colaboradores](https://github.com/Ranpu123/BlockchainED/contributors) que participaram deste projeto.
