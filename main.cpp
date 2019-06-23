#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <iostream>

void incluir();
void alterar();
void remover();
void listar();
void deadlock();
int transacao();
void commit();
void rollback();
void copia();
void limpar();

void clrscr();

struct copy
{
    int comparac, codc, bloqueioc;
    char prodc[30];
} cp;

struct ferramenta
{
    int compara, cod, bloqueio;
    char prod[30]; //confirmar O COMPARA NUMERA FIXO UM PRODUDO
} reg;

FILE *log;     //somente log
FILE *arq;     //somenete incluir
FILE *arquivo; //somente bdproduto
FILE *mem;     //somente copiabanco
FILE *altrem;  //somente alterar e remover

char resposta;
int numprod, teste, tr = 0, controle;

int main()
{
    char opcao;

    do
    {
        clrscr();
        tr++;
        setlocale(LC_ALL, "Portuguese");

        printf("---BANCO DE DADOS---\n");

        printf("1 - NOVA TRANSACAO\n");
        printf("2 - CHECKPOINT\n");
        printf("3 - EXISTE DEADLOCK?\n");
        printf("4 - LISTAR BANCO\n");
        printf("\n0 - Sair!\n");
        printf("Opcao: ");
        opcao = getche();

        switch (opcao)
        {

        case '1':
            transacao();
            break;
        case '2':
            clrscr();
            printf("Em implementacao");
            getch();
            ;
            break;
        case '3':
            deadlock();
            break;
        case '4':
            listar();
            break;

        case '0':
            clrscr();
            printf("Programa encerrado\n");
            log = fopen("log.txt", "ab");
            fprintf(log, "Programa encerrado!\n");
            fclose(log);
            printf("Tecle para Sair");
            getch();
            break;
        default:
            printf("Opcao Invalida.\n");
            getch();
        }
    } while (opcao != '0');
}

void clrscr(void)
{
    system("@cls||clear");
}

int transacao()
{
    char opcao;
    copia();
    log = fopen("log.txt", "ab");
    fprintf(log, "Iniciou T"
                 "%i \n",
            tr);
    fclose(log);
    arq = fopen("incluir.txt", "w");
    fclose(arq);
    do
    {
        clrscr();

        /*gotoxy(19,3);*/ printf("---TRANSACAO %i---controle = %i\n", tr, controle);
        /*gotoxy(10,9);*/ printf("1 - INCLUIR\n");
        /*gotoxy(10,10);*/ printf("2 - ALTERAR\n");
        /*gotoxy(10,11);*/ printf("3 - REMOVER\n");
        /*gotoxy(10,12);*/ printf("4 - COMMIT\n");
        /*gotoxy(10, 13);*/ printf("5 - ROLLBACK\n");
        /*gotoxy(10,14);*/ printf("\n0 - VOLTAR MENU PRINCIPAL\n");
        /*gotoxy(10,16);*/ printf("Opcao: ");
        /*gotoxy(17,16);*/ opcao = getche();
        switch (opcao)
        {
        case '1':
            incluir();
            break;
        case '2':
            alterar();
            break;
        case '3':
            remover();
            break;
        case '4':
            commit();
            break;
        case '5':
            rollback();
            break;
        case '0':
            clrscr();
            log = fopen("log.txt", "ab");
            fprintf(log, " T%i saiu \n", tr);
            fclose(log);
            break;
        default: /*gotoxy(17,16)*/;
            printf("Opcao Invalida.\n");
            getch();
        }
    } while (opcao != '0');
    return tr;
}

void incluir()
{
    char confirmar;
    resposta = 'S';
    log = fopen("log.txt", "ab");
    arq = fopen("incluir.txt", "ab");
    if (arq)
    {
        while (resposta == 'S' || resposta == 's')
        {

            clrscr();
            /*gotoxy(10,3);*/ printf("---Incluir ferramenta---\n");
            /*gotoxy(10,7);*/ printf("Codigo da ferramenta......: ");
            reg.compara = controle;
            scanf("%i", &reg.cod);
            /*gotoxy(10,8);*/ printf("Descricao da ferramenta...: ");
            /*fflush(stdin);
            fgets(reg.prod, 30, stdin);*/
            scanf("%s", &reg.prod);
            reg.bloqueio = 0;
            teste = fwrite(&reg, sizeof(struct ferramenta), 1, arq);

            fprintf(log, "T%i incluiu Cod %i Produto %s \n", tr, reg.cod, reg.prod);
            /*gotoxy(10,14);*/
            printf("Registro gravado com sucesso!\n");

            /*gotoxy(10,20);*/ printf("Pressione algo para continuar?");
            resposta = getche();
            controle++;
        }
    }
    fclose(log);
    fclose(arq);
}

void alterar()
{
    char nomepesq[30];
    int achei, apontador, codant;
    resposta = 's';
    arq = fopen("bdproduto.txt", "rb+");
    mem = fopen("copiabanco.txt", "rb+");
    log = fopen("log.txt", "ab");
    if (arq)
    {
        while (resposta == 's' || resposta == 'S')
        {
            achei = 0;
            clrscr();
            /* gotoxy(10,3);*/
            printf("---Editar produto---\n");
            /*gotoxy(10,5);*/ printf("============================\n");
            /*gotoxy(10,7);*/
            printf("Produto a ser pesquisado: ");
            /*fflush(stdin);
            fgets(nomepesq, 30, stdin);*/
            scanf("%s", &nomepesq);
            rewind(mem);
            rewind(arq);
            apontador = 0;
            while (!feof(arq) && achei == 0)
            {
                teste = fread(&reg, sizeof(struct ferramenta), 1, arq);
                apontador++;
                if (teste)
                {

                    if (strcmp(reg.prod, nomepesq) == 0 && reg.bloqueio == 0)
                    {
                        apontador--;
                        reg.bloqueio = 1;
                        fseek(arq, apontador * sizeof(struct ferramenta), SEEK_SET);
                        teste = fwrite(&reg, sizeof(struct ferramenta), 1, arq);
                        fclose(arq);
                        apontador++;

                        /* gotoxy(10,13);*/ printf("Codigo..: %i\n", reg.cod);
                        /* gotoxy(10,14);*/ printf("Nome....: %s\n", reg.prod);
                        achei = 1;
                        codant = reg.cod;

                        /*gotoxy(40,10);*/ printf("Editando o registro...\n");
                        /*gotoxy(40,13);*/ printf("Codigo: ");
                        scanf("%i", &cp.codc);
                        /*gotoxy(40,14);*/ printf("Produto: ");
                        fflush(stdin);
                        fgets(cp.prodc, 20, stdin);

                        /*gotoxy(40,20);*/ printf("\nConfirma a edicao? S/N  ");
                        /*gotoxy(63,20);*/ resposta = getche();

                        if (resposta == 's' || resposta == 'S')
                        {
                            log = fopen("log.txt", "ab");
                            fprintf(log, " T%i alterou cod %i  produto %s para cod %i produto %s \n", tr, codant, nomepesq, cp.codc, cp.prodc);
                            fclose(log);
                            apontador--;
                            arq = fopen("bdproduto.txt", "rb+");
                            reg.bloqueio = 0;
                            fseek(mem, apontador * sizeof(struct copy), SEEK_SET);
                            teste = fwrite(&cp, sizeof(struct copy), 1, mem);
                            if (teste)
                            {
                                /*gotoxy(40, 20);*/
                                printf("Registro editado com sucesso\n");
                            }
                        }
                        else
                        {
                            arq = fopen("bdproduto.txt", "rb+");
                            apontador--;
                            reg.bloqueio = 0;
                            fseek(arq, apontador * sizeof(struct ferramenta), SEEK_SET);
                            teste = fwrite(&reg, sizeof(struct ferramenta), 1, arq);
                        }
                    }
                }
            }
            if (achei == 0)
            {
                /*gotoxy(10,12);*/
                printf("Registro nao encontrado.\n");
            }
            /*gotoxy(10,22);*/
            printf("Deseja continuar? S/N");
            resposta = getche();
        }
        fclose(arq);
    }
    else
    {
        clrscr();
        /*gotoxy(10,8);*/ printf("Arquivo Vazio.\n");
        /*gotoxy(10,10);*/ printf("Tecle algo para voltar\n");
        getch();
    }
}

/*void alterar(){
	char nomepesq[30];
	int achei, apontador, codant;
						
	resposta='s';
	
	mem=fopen("bdproduto.txt", "ab");
	arq=fopen("copiabanco.txt", "ab+"); 
	log=fopen("log.txt", "ab");
	
	if(arq){
		while (resposta=='s' || resposta=='S'){
			achei=0;
			clrscr();
			gotoxy(10,3);	printf("---Editar produto---");
			gotoxy(10,5);	printf("============================");
			gotoxy(10,7);
			printf("Produto a ser pesquisado: ");
			fflush(stdin);
			fgets(nomepesq,30,stdin);
			
			rewind(arq);
			apontador=0;
			while(!feof(arq) && achei ==0){
				teste=fread(&reg,sizeof(struct ferramenta), 1,arq);
				//apontador++;
				if(teste){
						
					if(strcmp(reg.prod,nomepesq)==0 && reg.bloqueio==0){
						apontador--;rewind(mem);
						reg.bloqueio=1;
							fseek(mem,apontador * sizeof(struct ferramenta), SEEK_SET);
							teste=fwrite(&reg,sizeof(struct ferramenta), 1, mem);fclose(mem);
							apontador++;
						
						gotoxy(10,13);	printf("C�digo..: %i", reg.cod);
						gotoxy(10,14);	printf("Nome....: %s", reg.prod);	
						achei=1;
						codant=reg.cod;
						
						
						gotoxy(40,10);	printf("Editando o registro...");
						
						gotoxy(40,13);	printf("C�digo: " );	scanf("%i",&cp.codc);
						gotoxy(40,14);	printf("Produto: ");		fflush(stdin);
																fgets(cp.prodc,20,stdin);
						
						
						
						gotoxy(40,20);	printf("Confirma a edi��o? S/N  ");
						gotoxy(63,20);  resposta=getche();
						
						
						if(resposta=='s' || resposta=='S'){
							log=fopen("log.txt", "ab");fprintf(log," T%i alterou cod %i  produto %s para cod %i produto %s \n",tr,codant, nomepesq, cp.codc, cp.prodc);fclose(log);
							apontador--;
							reg.bloqueio=0;
							fseek(arq,apontador * sizeof(struct copy), SEEK_SET);
							teste=fwrite(&cp,sizeof(struct copy), 1, arq);
							if(teste){
								gotoxy(40,20);
								printf("Registro editado com sucesso");
							}					
						}
						else{
							apontador--;
							reg.bloqueio=0;
							fseek(arq,apontador * sizeof(struct ferramenta), SEEK_SET);
							teste=fwrite(&reg,sizeof(struct ferramenta), 1, arq);
						}
						
					}			
				}
			}
			if(achei==0){
				gotoxy(10,12);	printf("Registro n�o encontrado.");			
			}
			gotoxy(10,22);		printf("Deseja continuar? S/N");			
			resposta=getche();
		}
		fclose(arq);
	}
	else{
		clrscr();
		gotoxy(10,8);	printf("Arquivo Vazio.");
		gotoxy(10,10);	printf("Tecle algo para voltar");
		getch();
	}
}*/

void remover()
{

    char nomepesq[30], remprod[30];
    int achei, apontador, remcod = 0;
    resposta = 'S';
    arq = fopen("copiabanco.txt", "rb+");
    mem = fopen("bdproduto.txt", "rb+");
    if (arq)
    {
        while (resposta == 'S' || resposta == 's')
        {
            achei = 0;
            clrscr();
            /*gotoxy(10, 3);*/
            printf("---Exclui Produto---");
            /*gotoxy(10, 7);*/
            printf("Qual o nome do produto: ");
            fflush(stdin);
            fgets(nomepesq, 30, stdin);
            rewind(arq);
            apontador = 0;
            while (!feof(arq) && achei == 0)
            {
                teste = fread(&reg, sizeof(struct ferramenta), 1, arq);
                apontador++;
                if (teste && reg.bloqueio != 1)
                {
                    if (strcmp(reg.prod, nomepesq) == 0 && reg.bloqueio == 0)
                    {
                        apontador--;
                        reg.bloqueio = 1;
                        fseek(mem, apontador * sizeof(struct ferramenta), SEEK_SET);
                        teste = fwrite(&reg, sizeof(struct ferramenta), 1, mem);
                        fclose(mem);
                        apontador++;

                        printf("Pesquisa realizada com sucesso");

                        printf("Codigo..: %i", reg.cod);
                        remcod = reg.cod;

                        printf("Descricao....: %s", reg.prod);
                        achei = 1;

                        printf("Confirma a exclusao? S/N");

                        resposta = getche();
                        if (resposta == 's' || resposta == 'S')
                        {
                            apontador--;
                            reg.cod = 0;
                            strcpy(reg.prod, " ");
                            reg.bloqueio = 3;
                            fseek(arq, apontador * sizeof(struct ferramenta), SEEK_SET);
                            teste = fwrite(&reg, sizeof(struct ferramenta), 1, arq);

                            if (teste)
                            {
                                /*gotoxy(10, 19);*/
                                printf("Registro excuido com sucesso.");
                                log = fopen("log.txt", "ab");
                                fprintf(log, "T%i removeu %i   %s \n", tr, remcod, nomepesq);
                                fclose(log);
                            }
                        }
                        else
                        {
                            apontador--;
                            reg.bloqueio = 0;
                            fseek(arq, apontador * sizeof(struct ferramenta), SEEK_SET);
                            teste = fwrite(&reg, sizeof(struct ferramenta), 1, arq);
                        }
                    }
                }
                else
                {
                    clrscr();
                    /*gotoxy(10, 10);*/
                    printf("BLOQUEADO PARA EXCLUS�O");
                }
            }
            if (achei == 0)
            {
                /*gotoxy(10,12);*/
                printf("Registro n�o encontrado.");
                arquivo = fopen("log.txt", "ab");
                fprintf(arquivo, "ERRO - PRODUTO N�O ENCONTRADO! \n");
                fclose(arquivo);
            }
            /*gotoxy(10,22);*/
            printf("Deseja continuar? S/N");
            resposta = getche();
        }
        fclose(arq);
    }
    else
    {
        clrscr();
        /*gotoxy(10,8);*/ printf("Arquivo vazio.");
        /*gotoxy(10,10);*/ printf("Tecle algo para voltar ao menu..");
        getch();
    }
}

void listar()
{
    int linha = 5;
    tr--;
    arq = fopen("bdproduto.txt", "rb");
    clrscr();
    if (arq)
    {
        /*gotoxy(19,3);*/ printf("---BANCO DE DADOS---\n");
        /*gotoxy(10,4);*/ printf("Codigo  Produto  Bloqueio\n");
        while (!feof(arq))
        {
            teste = fread(&reg, sizeof(struct ferramenta), 1, arq);
            if (teste && strcmp(reg.prod, ""))
            {
                /*
                printf("%04i", reg.cod);
                printf(" | ");
                printf("%s", reg.prod);
                printf(" | ");
                printf("%i", reg.bloqueio);
                printf(" | ");
                printf("%i", reg.compara);
                printf("\n");
                */

                printf("%04i | %s | %i - %i\n", reg.cod, reg.prod, reg.bloqueio, reg.compara);
                if (linha < 17)
                    linha++;
                else
                {

                    printf("Tecle ENTER para continuar exibindo os registros.");
                    getch();
                }
            }
        }
        fclose(arq);
        printf("Tecle algo para voltar...");
    }
    else
    {
        printf("Arquivo vazio.");
        printf("Tecle algo para voltar ao menu...");
        getch();
    }
    getch();
}

void commit()
{
    clrscr();
    arq = fopen("incluir.txt", "rb");
    arquivo = fopen("bdproduto.txt", "ab");

    log = fopen("log.txt", "ab");
    //	altrem=fopen("copiabanco","rb");
    if (arq)
    {
        while (!feof(arq))
        {
            rewind(arquivo);
            fread(&reg, sizeof(struct ferramenta), 1, arq);
            fseek(arquivo, sizeof(struct ferramenta), SEEK_END);
            fwrite(&reg, sizeof(struct ferramenta), 1, arquivo);
            fprintf(log, "T%i incluiu no Banco -Cod: %i Produto: %s \n", tr, reg.cod, reg.prod); //n�o mudar
        }
    }
    else
    {
        printf("Erro na copia... arquivo origem nao existe...\n");
        getche();
    } //nao mudar
    printf("Dados commitado! Tecle algo para voltar ao menu...\n");
    getch();
    listar();
    fclose(arq);
    fclose(log);
    limpar();
}

void rollback()
{
    clrscr();
    arq = fopen("incluir.txt", "w");
    altrem = fopen("copiabanco", "w");
    fclose(arq);
    fclose(altrem);
    copia();
    printf("Rolback dos arquivos!\n");
    printf("Tecle algo para voltar ao menu...");
    getch();
}

void copia()
{
    clrscr();
    controle = 0;
    arquivo = fopen("bdproduto.txt", "rb");
    arq = fopen("copiabanco.txt", "wb");
    if (arquivo)
    {
        while (!feof(arquivo))
        {
            controle++;
            fread(&reg, sizeof(struct ferramenta), 1, arquivo);
            fseek(arq, sizeof(struct ferramenta), SEEK_END);
            fwrite(&reg, sizeof(struct ferramenta), 1, arq);
        }
    }
    else
    {
        printf("Erro na copia... arquivo origem nao existe...");
        getche();
    }
    fclose(mem);
    fclose(arquivo);
}

void deadlock()
{
    int linha = 5, bl = 0;
    arquivo = fopen("bdproduto.txt", "rb");
    clrscr();
    if (arq)
    {
        while (!feof(arq))
        {
            teste = fread(&reg, sizeof(struct ferramenta), 1, arquivo);
            if (teste && strcmp(reg.prod, ""))
            {
                if (reg.bloqueio == 1)
                {
                    bl = 1;
                }
            }
            if (bl == 1)
            {
                clrscr();
                /*gotoxy(10,10);*/
                printf("Existe dead lock!");
            }
            else
            {
                clrscr();
                /*gotoxy(10,10);*/
                printf("NAO Existe dead lock!");
            }
        }
        fclose(arq);
    }
    else
    {
        /*gotoxy(10,19);*/ printf("Arquivo vazio.");
        /*gotoxy(10,10);*/ printf("Tecle algo para voltar ao menu...");
        getch();
    }
    getch();
}

void limpar()
{
    fclose(fopen("incluir.txt", "w"));
}
