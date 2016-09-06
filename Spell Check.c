/*

Kened Wanderson Cruz Oliveira
11321918
Engenharia da Computação

Estrutura de Dados


COMPILAR COM "gcc vamos.c -o prog -lm"
-lm diz para o gcc incluir a biblioteca libm, para o funcionamento correto da math.h


*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define A 0.6180339887

const int numtabela=61561; 																			//tamalho da tabela dicionario=(1/5)307855=61561;

struct lista_no {																								//estruruta de cada celula da minha tabela
	char	word[100];
  unsigned int   chave;
  struct lista_no *anter,*prox;

};
typedef struct lista_no LISTA;

void imprimir(LISTA *p[]);                                       //analiza o total de colisões e gera uma saida.csv
unsigned int hash(char palavra[]);                               //calcula o hash ja com compressão
void zera(LISTA* tabelaH[]);                                     //faz com que cada bucker aponte para NULL
LISTA* inserir(char palavra[],unsigned int hash, LISTA* tabelaH);//inseri os elementos na tabela e trata colições
void check(LISTA* tabelaH[],char texto[]);											 //spell checker
void imprimir_erro(int Npalavras,char *erros[],int Ntext,double tempo);	 //gera o log de erros


int main(int nada,char *arg[])
{
FILE* wordfile;
LISTA *tabela[numtabela];
unsigned int x;
char *buf = malloc (200);
arg[1]="dicionario.txt";
arg[2]="dicionario.txt";
	zera(tabela);                                                   	//inicializa a tabela como NULL

	if((wordfile=fopen(arg[2],"r")) == NULL)
	{
		printf("ERRO 404\n");																						//arquivo nao encontrado
		exit(-1);
	}
	while(fscanf(wordfile,"%s",buf) != EOF)		                      	//captura cada palavra, uma por uma
	{
		x=hash(buf);																										//calculo hash
		tabela[x]=inserir(buf,x,tabela[x]);															//inserindo as palavras na tabela hash e tratando colisões
	}
fclose(wordfile);
//imprimir(tabela);																									//imprimindo grafico colisões
	check(tabela,arg[1]);                                             //spellchecker
return 0;
}


unsigned int hash(char palavra[]) 																//faz o meu calculo
{
	int asc,i,ex;
	unsigned int valor=0;

	for (i=0;palavra[i]!= '\0';i++)
	{
			asc=palavra[i];																							//captura o asc de cada letra
		if((asc != 10) && (asc != 32))																//condição,faz o calculo somente com letras
		{
			ex=pow(10,i);
            valor+=asc*ex;
		}
	}
	double temp=((A*valor)-(unsigned int)(A*valor))*numtabela;			 //calculo da inversa da proporsão de aurea
	unsigned int	hash=temp; 																				 //compressão do hash

return hash;
}

void zera(LISTA *tabelaH[])
{
int i;
	for (i=0;i<numtabela;i++)																						//cada celula da tabela iniciar com valor NULL
	{
	tabelaH[i]=NULL;
	}

}

LISTA* inserir(char palavra[],unsigned int hash, LISTA* tabelaH)
{
	LISTA *p=tabelaH;
	LISTA *q=tabelaH;
	LISTA *t=tabelaH;

	if(p==NULL)																														//se o bucker estiver vazio
	{
		p=(LISTA*)malloc(sizeof(LISTA));																		//criando o nó
		strcpy(p->word,palavra);
		p->chave=hash;
		p->prox=NULL;
	return p;
	}
	else
	{

		while(t!=NULL)																											//tratamento para palavras iquais
		{
			if(!strcmp(t->word,palavra))
				return p;
			t=t->prox;
		}
		p=(LISTA*)malloc(sizeof(LISTA));                                     //criando o nó
		strcpy(p->word,palavra);
		p->chave=hash;
		p->prox=q;
		q->anter=p;
	return p;
	}
}

void imprimir(LISTA* p[])
{
	FILE *arq;
	LISTA *aux;
	int i=0;
	int cont=0;
unsigned int soma=0;

	arq = fopen("saida.csv", "w");
  while(i!=numtabela)
	{
    aux = p[i];
    while ( aux != NULL)
		{
        cont++;
				aux = aux->prox;
    }
    i++;
		if(cont==0)
 	   fprintf(arq, "%d;%d\n", i, cont);
		else
 	   fprintf(arq, "%d;%d\n", i, cont-1);

		soma+=cont-1;
    cont=0;
	}
fclose(arq);
printf("\nmedia--%f--\n",(float)soma/numtabela);
}


void check(LISTA *tabelaH[],char texto[])
{
	LISTA *p;
	FILE *arq;
	int Ptext,index=0,coluna=0,palavras=0,linha=1;
	unsigned int x=0;
	char *buf = malloc (200);
  char ** erros = malloc(numtabela * sizeof(*erros));
	clock_t start_time;
double timing=0;
	if((arq=fopen(texto,"r")) == NULL)
	{
		printf("ERRO 404\n");																						//arquivo nao encontrado
		exit(-1);
	}

	int temp1=numtabela;
	while(fscanf(arq,"%s",buf) != EOF)
	{
		palavras++;
		coluna++;																													//contador de colunas
		x=hash(buf);																											//calculo hash
		p=tabelaH[x];

	start_time = clock();																							//inicia o contador do tempo

		if(p==NULL)																												//se o lucal a ser conferido esta vazio
		{
				erros[index]= malloc(254 * sizeof(char));
				sprintf(erros[index], "%d.%s : %d, %d",index, buf, linha,coluna);
				index++;
				if(index==temp1)
				{
					temp1=numtabela+index;
			    erros = realloc(erros, (temp1) * sizeof(*erros));
				}
		}
		else
			while(strcmp(p->word,buf))																			//verifica todas as palavras daquele bucker
			{
				p=p->prox;
				if(p==NULL)
				{
					erros[index]= (char*)malloc (200);
					sprintf(erros[index], "%d.%s : %d, %d",index, buf, linha,coluna);
					index++;
					if(index==temp1)
					{
						temp1=numtabela+index;
				    erros = realloc(erros, (temp1) * sizeof(*erros));
					}
				break;
				}
			}
timing =timing+ ((clock() - start_time) / ((double)CLOCKS_PER_SEC*0.001));			//termina o contador do tempo e faz o calculo

		char temp=fgetc(arq);																								//captura o proximo caracter
		if(temp=='\n')																											//verifica se é uma quebra de linha
		{
			coluna=0;
			linha++;
		}
		if(temp==' ')																								  			//verifica se é um espaço
			coluna+=strlen(buf);																							//contador de colunas

	}

	fclose(arq);
	if(index!=0)
		Ptext=index-1;																												//pega o indece e transforma no numero de palavras erradas

	Ptext=index;																												//pega o indece e transforma no numero de palavras erradas
	imprimir_erro(palavras,erros,Ptext,timing);
free(erros);
}




void imprimir_erro(int Npalavras,char *erros[],int Ntext,double tempo)
{
FILE* arq;
int index;

	arq = fopen("log.txt", "w");

	fprintf(arq,"Número total de palavras do texto: %d\n",Npalavras);
	fprintf(arq,"Tempo total da verificação: %.2fms \n",tempo);
	fprintf(arq,"Número de palavras que falharam no spell check: %d\n",Ntext);
	fputs("Lista de palavras que falharam no spell check:\n\n",arq);
	fputs("Num. Ocorrencia ­ Palavra : Linha, Coluna\n----------------------------------------\n",arq);
	for(index=0;index<Ntext;index++)
	{
		fprintf(arq,"%s\n",erros[index]);
	}

	fclose(arq);
}
