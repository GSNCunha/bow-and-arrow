/*
Gabriel Cunha e Joao Caetano
o programa é um jogo de um arqueiro
que estoura baloes e mata monstros
inspirado no jogo bow's arrow
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>      // ver enunciado para uso com LINUX
#include <time.h>
#include <stdbool.h>
#define BALOES 15        // quantidade de “balões” a exibir
#define LINHA_MAXIMO 30   // coordenada de linha máxima
#define COLUNA_MAXIMO 90  // coordenada de coluna  máxima
#define ALTURA_MAX 35
#define LARGURA_MAX 50
#define FLECHAMAX 15
#define FLECHAMAX2 30
#define MONSTRO 30
#define COLUNAS 8
#define MENU 0
#define JOGO 1
#define HIGHSCORE 2
typedef struct tipo_jogador_st
{
    char nome[40];
    int score;
} TIPO_JOGADOR;

typedef struct ponto_st
{
    int x;
    int y;
} PONTO;
int GAMEOVER();
int move_arqueiro(char comand, int alt, char arqueiro[][8]);// printa o arqueiro de acordo com a localizacao anterior e a tecla precionada
void printa_baloes(int u, int estourado[], char balao[][4]);//subrotina que recebe uma localizacao e informacoes de baloes e imprime baloes de acordo com isso
int le_arquivo(char nome_arquivo[], TIPO_JOGADOR arranjo[]);// le o nome do arquivo
void le_arqueiro(FILE *arq, char arqueiro[][COLUNAS]);// le um arquivo que contem um arqueiro
void escreve_arqueiro(char nome_arq[], char arqueiro[][COLUNAS]);// escreve o deseho do arqueiro
int adiciona__monstro(int r, int x, int monstrol[], int monstroc[], char matrizmonstro[][6], int monstro,int *numeromonstro, int *last_monster);// adiciona um monstro no jogo
void imprime_scores(TIPO_JOGADOR arranjo[], int num_elementos);// imprime a pontuacao dos melhores jogadores
int move__monstro(int monstro,int monstrol[], int monstroc[], char matrizmonstro[][6],int *numeromonstro, int last_monster, int *fim, int flecha2, int *pontos, int alt); // a cada rodada bota todos os monstros um pra frente
int imprime_tela(int qual);// imprime a tela de acordo com a opcao que o usuario preferir
void imprime_monstro(char matrizmonstro[][6], int monstrol[], int monstroc[], int monstro);//subrotina que imprime uma matriz que representa um monstro
void escreve_tela(int qual);//subrotina que escreve em um arquivo o cenario
void imprime_arqueiro(int alt,char arqueiro[4][8], char comand);//subrotina que imprime um matriz de char que representa um arqueiro
void apaga();// limpa uma area printando varios espacos
char insere_score(TIPO_JOGADOR nova_entrada, TIPO_JOGADOR arranjo[]);//bota o score do jogador nos top 5 caso ele mereca
void escreve_arquivo(char nome_arquivo[], TIPO_JOGADOR arranjo[], int num);// escreve informacoes em um arquivo
void gotoxy(int x, int y); // x = coluna, y = linha
void le_balao(FILE *arq, char[][4]);//le o arquivo do balao
void le_flecha(FILE *arq, char flechav[]);// le o arquivo da flecha
int printa_flechas(int z, int flechal[], int flechac[], int u, int estourado[], int apagaumavez[], int flecha, int nivel, int monstrol[], int monstroc[], int monstro, int *pontos, int *numeromonstro, int *fim, char flechav[], int flecha2);//a cada rodada bota todas as flechas um pouco mais pra frente
int main()
{
    int x, y, i, j, z, u, r, t = 65, flecha = 0, apagaumavez[FLECHAMAX] = {0},soma = 0, pontos = 0,fim = 0;
    int score_antigo, nivel = 1, flecha2 = 0, monstro = 0;
    int numeromonstro = 0;
    char arqueiro[4][8], hscheck = 'n';
    char flechav[4];
    int posicao[LINHA_MAXIMO][COLUNA_MAXIMO] = {0};
    int flechal[FLECHAMAX+FLECHAMAX2];
    int last_monster = -1;
    int flechac[FLECHAMAX+FLECHAMAX2];
    int monstrol[MONSTRO-1] = {0};
    int monstroc[MONSTRO-1];
    char balao[5][4];
    char matrizmonstro[7][6] = {"   "};
    int estourado[BALOES-1] = {0};
    char com = 'f';
    srand(time(0));
    int pos_curs = 0;
    char comand = 'p';
    int alt = 7;
    TIPO_JOGADOR jogadores[5], player_sc;
    FILE *arq;
    bool fecha_jogo = false;

    for(x = 0; x< FLECHAMAX+FLECHAMAX2; x++)// ja que a flecha vai sair da casa x13, a variavel ja comeca assim
        flechac[x] = 13;

    for(x = 0; x< MONSTRO-1; x++)// ja que a monstro vai sair da casa x60, a variavel ja comeca assim
        monstroc[x] = 60;

    if(arq = fopen("arqueiro.txt", "r"))
    {
        le_arqueiro(arq, arqueiro);
        fclose(arq);
    }
    else
    {
        printf("Erro na leitura do arqueiro!");//caso tenha erro ao ler o arquivo
        return 1;
    }
    if(arq = fopen("balao.txt", "r"))
    {
        le_balao(arq, balao);
        fclose(arq);
    }
    else
    {
        printf("Erro na leitura do balao!");//caso tenha erro ao ler o arquivo
        return 1;
    }
    if(arq = fopen("monstro.txt", "r"))
    {
        le_arqueiro(arq, matrizmonstro);
        fclose(arq);
    }
    else
    {
        printf("Erro na leitura do monstro!");//caso tenha erro ao ler o arquivo
        return 1;
    }
    if(arq = fopen("flecha.txt", "r"))
    {
        le_flecha(arq,flechav);
        fclose(arq);
    }
    else
    {
        printf("Erro na leitura do monstro!");//caso tenha erro ao ler o arquivo
        return 1;
    }
    escreve_tela(MENU);
    escreve_tela(JOGO);
    while(!fecha_jogo)// while de repeticao infinito para poder jogar repetidas vezes
    {
        le_arquivo("highscores.bin", jogadores);
        imprime_tela(MENU); // imprime a primeira tela de todas, que é o menu
        for(x=0; x < 20; x++) // limpa qualquer resto que poderia ter no campo
            printf("                                                                                  \n");
        com = 'f';//para n dar erro, mudamos o caracter para que nao pule nenhuma etapa
        gotoxy(32, 9);
        printf("-->");
        pos_curs = 0;
        while(com != ' ')// escolhe que opcao quer ver
        {
            com = getch();
            gotoxy(32, 9+pos_curs);
            printf("   ");
            if(com == 'w' && pos_curs > 0)
                pos_curs--;
            if(com == 's' && pos_curs < 2)
                pos_curs++;
            gotoxy(32, 9+pos_curs);
            printf("-->");
        }

        t = 65;                  //da um valor  todas as variaveis que precisam ser valorizadas para efetuar loops do programa
        flecha = 0;
        for(x = 0; x<=FLECHAMAX; x++)
            apagaumavez[x] = 0;
        soma = 0;
        pontos = 0;
        fim = 0;
        nivel = 1;
        flecha2 = 0;
        monstro = 0;
        numeromonstro = 0;
        hscheck = 'n';
        last_monster = -1;

        for(x = 0; x<=MONSTRO-1; x++)
            monstrol[x] = 0;
        for(x = 0; x<=BALOES-1; x++)
            estourado[x] = 0;
        alt = 7;
        comand = 'p';
        com = 'f';
        for(x = 0; x< FLECHAMAX+FLECHAMAX2; x++)
            flechac[x] = 13;

        for(x = 0; x< MONSTRO-1; x++)
            monstroc[x] = 60;
        for(x = 0; x<= FLECHAMAX+FLECHAMAX2; x++)
            flechal[x] = 0;


        z = 0;
        u = 0;
        if(pos_curs == 0)// se a escolha foi novo jogo
        {
            imprime_tela(JOGO);// imprime tela do novo jogo
            gotoxy(15,3);
            printf("%d    ", jogadores[0].score);// bota o high score
            gotoxy(3,7);
            imprime_arqueiro(alt, arqueiro, comand);//printa o arqueiro
            for (x = 0; x < BALOES; x++)// printa os baloes
            {
                gotoxy(15+(x*3),25);
                for ( y=0; y<4; y++ )
                {
                    printf("%s\n", balao[y]);
                    gotoxy(15+(x*3),25+y+1);
                }
            }

            while(comand != 'q')// while de cada jogo completo, só sai quando o jogo acabar
            {
                comand = 'p';
                if(z%7 == 0)// caso o programa tenha exectado 7 loops, cada um com 10 milésimos, move a flecha pro lado(representa a velocidade)
                {
                    monstro = printa_flechas(z,flechal, flechac, u, estourado, apagaumavez, flecha, nivel, monstrol, monstroc,monstro,&pontos,&numeromonstro, &fim,flechav, flecha2);
                    if(pontos >= jogadores[0].score)
                    {
                        gotoxy(15,3);
                        printf("%d     ", pontos);
                    }
                }

                if(u%40 == 0 && nivel == 1)// caso o programa tenha exectado 40 loops, cada um com 10 milésimos, move o balao pra cima, mas so e usado no nivel um
                {
                    printa_baloes(u, estourado,balao);
                }
                if(z%10 == 0 && nivel == 2)// caso o programa tenha exectado 10 loops, cada um com 10 milésimos,executa a acao dos monstros, mas so eh usado no nivel 2
                {
                    monstro = move__monstro(monstro,monstrol,monstroc, matrizmonstro,&numeromonstro, last_monster, &fim, flecha2, &pontos, alt);
                    if(z%200 == 0)// caso o programa tenha exectado 40 loops, cada um com 10 milésimos, move o balao pra cima
                    {
                        r = rand();
                        monstro =adiciona__monstro(r,monstro, monstrol,monstroc, matrizmonstro, monstro,&numeromonstro, &last_monster);
                    }
                }
                if((u/40) == 20 && nivel == 1)// se o balao tiver chegado no topo da pagina, a posicao dele muda para o inicio
                    u = 0;
                if(kbhit())//apenas quando alguma tecla for preccionada, ler qual foi
                    comand = getch();

                if(comand == ' ')// caso tenha apertado espaco, cria uma flecha
                {
                    flechal[flecha]= alt+1;//reserva a linha onde foi criada a flecha
                    flecha++;//contador do nivel 1
                    flecha2++;//contador do nivel 2
                    if(nivel == 1)//apaga as flechas usadas do contador do nivel um
                    {
                        gotoxy(t,3);
                        printf(" ");
                        t++;
                    }
                    if(nivel == 2)//apaga as flechas usadas do contador do nivel dois
                    {
                        gotoxy(t,3);
                        printf(" ");
                        t++;
                    }
                    if(flecha2 == 31 && nivel == 2)//se no segundo nivel o personagem usou mais flechas do que o permitido, o jogo acaba
                    {
                        break;
                    }
                }
                score_antigo = soma;
                soma = 0;
                for(x = 0; x<BALOES; x++)// ja que um balao x estourado vai ter uma variavel que reserva seu estado esoutrado(1), podemos contar quandos estourados tem
                    soma += estourado[x];
                if(soma == 15 && nivel == 1)// se 15 baloes estouraram, acabou a fase!
                {
                    gotoxy(10,2);//calcula pontos de flechas n usadas
                    pontos = soma*100;
                    printf("%d    ", pontos);
                    apaga();
                    gotoxy(25,20);
                    printf("passou de fase! parabens");
                    gotoxy(25,21);
                    printf("bonus de %d flechas, + %d", 15-flecha,(15-flecha)*50);
                    Sleep(1000);
                    pontos = pontos +(15-flecha)*50;
                    gotoxy(10,2);
                    printf("%d    ", pontos);
                    nivel = 2;
                    flecha2 = 0;
                    gotoxy(46,2);
                    printf("2");
                    apaga();
                    t = 50;
                    gotoxy(50,3);
                    printf("::::::::::::::::::::::::::::::");// bota novo contdor de flechas
                }
                if(monstro == -2)// caso algum programa tenha identificado o fim do jogo, vai passar por referencia o numeor -2 para acabar o programa
                {
                    break;
                }
                if( score_antigo != soma && nivel ==1 )//apenas eh necessario gastar processamento se o score mudou
                {
                    gotoxy(10,2);
                    pontos = soma*100;//printa pontos em tempo real
                    printf("%d    ", pontos);
                    gotoxy(15,3);
                    if(pontos > jogadores[0].score)
                    {
                        gotoxy(15,3);
                        printf("%d    ", pontos);
                    }
                }
                alt += move_arqueiro(comand, alt, arqueiro);//altera a variavel de linha de acordo com os movimentos pra cima e pra baixo
                Sleep(10);//garantia de tempo
                z++;//duas variaveis como contadores de loop
                u++;
                if(fim == 1)// alguma parte do programa identificou o fim do jogo e botou um na variavel fim para acabar o programa
                {
                    break;
                }
            }
            player_sc.score = pontos;
            hscheck = insere_score(player_sc, jogadores);//grava nos high scores se necessario
            if(hscheck == 's')//muda para a tela do high scores para vermos a pontuacao atualizada
            {
                pos_curs = 1;
            }
        }
        if(pos_curs == 1)// caso tenham pedido o HIGHSCORES, monstrmos a tela com o pontos reservados em um arquivo
        {
            com = 'f';
            gotoxy(1,1);
            for(x=0; x<35; x++)
            {
                for(y=0; y<81; y++)//limpa tela
                    printf(" ");
                printf("\n");
            }
            gotoxy(1,1);
            if(hscheck == 's')
            {
                for(i=0; i<5; i++)
                    if(jogadores[i].score == pontos)
                        break;
                printf("Parabens, voce entrou no highscore!\nEntre seu nome:\n-->");
                fflush(stdin);
                fgets(jogadores[i].nome, 40, stdin);//pega o nome do recordista
                for(j=0; j<strlen(jogadores[i].nome); j++)
                    if(jogadores[i].nome[j] == '\n')
                        jogadores[i].nome[j] = '\0';
            }
            hscheck = 'n';
            imprime_scores(jogadores, 5);//imprime scores para ele visualizar os lugares
            escreve_arquivo("highscores.bin", jogadores, 5);
            pos_curs = 0;
            printf("\nAperte 'e' para voltar ao menu principal");
            while(com != 'e')// se apertar e volta para a tela principal
                com = getch();
        }
        if(pos_curs == 2)//apertou em sair
        {
            fecha_jogo = true;
        }

    }
    gotoxy(1,17);
    return 0;
}
int move_arqueiro(char comand, int alt, char arqueiro[][8])
{
    if(comand == 'w' && alt > 6)
    {
        gotoxy(3,alt+3);
        printf("        ");
        imprime_arqueiro(alt, arqueiro, comand);
        return -1;
    }
    if(comand == 's' && alt < ALTURA_MAX - 4)
    {
        gotoxy(3,alt);
        printf("        ");
        imprime_arqueiro(alt, arqueiro, comand);
        return 1;
    }
    return 0;
}
int printa_flechas(int z, int flechal[], int flechac[], int u, int estourado[], int apagaumavez[], int flecha, int nivel, int monstrol[], int monstroc[], int monstro, int *pontos, int *numeromonstro, int *fim, char flechav[], int flecha2)
{
    int d,p,x;
    for(d = 0; d <= flecha && nivel == 1; d++)// aqui tentamos descobrir se estourou balao ou n para cada flecha
    {
        if(flechal[d] <= 27-(u/40) && flechal[d] >=25-(u/40))// se tiver dentro das cordenadas y de onde tem balao
        {
            estourado[(flechac[d]-15)/3] = 1; // isso fala exatamente que balao foi estourado nessa rodada a variavel dele vai reservar um que representa o estado dele
        }
    }
    for(d = 0; d <= flecha && nivel == 2; d++)// aqui tentamos descobrir se a flecha acertou um monstro
    {
        for(p = 0; p <= monstro && nivel == 2; p++)
        {
            if(flechal[d] <= monstrol[p]+4 && flechal[d] >= monstrol[p])// se tiver dentro das cordenadas y de onde tem balao
            {
                if(flechac[d] >= monstroc[p] && flechac[d] <= monstroc[p]+2)
                {
                    gotoxy(flechac[d]-3, flechal[d]);
                    printf("   ");
                    for(x= 0; x<=4; x++)
                    {
                        gotoxy(monstroc[p], monstrol[p]+x);
                        printf("           ");
                    }
                    monstroc[p] = monstroc[monstro-1];// monstro morto usa sua variavel para reseravar a localizacao do monsto mais longe para
                    monstrol[p] = monstrol[monstro-1]; //vagar outra variavel que possa ser mais util
                    monstroc[monstro-1] = 60;
                    monstrol[monstro-1] = 0;
                    monstro--;
                    flechac[d] = 65;
                    *pontos = *pontos +200;
                    gotoxy(10,2);
                    printf("%d    ", *pontos);
                    if(*numeromonstro == 30 && monstro == 0)// se o mosntro morto for o ultimo vivo, acaba o jogo
                    {
                        *fim = 1;
                        apaga();
                        gotoxy(25,20);
                        printf("zerou o jogo! parabens");
                        gotoxy(25,21);
                        printf("bonus de %d flechas, + %d", 30-flecha2,(30-flecha2)*50);

                        *pontos = *pontos +(30-flecha2)*50;
                        gotoxy(10,2);
                        printf("%d    ", *pontos);
                        Sleep(3000);
                    }
                }
            }
        }

    }
    for(p = 0; p<=flecha; p++)// ele executa isso só até ter mexido o numero de flechas que ja foram lançadas
    {
        if(flechal[p] != 0)// se fosse zero, a linha da flecha nunca teria sido mexida, logo n tem flecha lancada
        {
            if(flechac[p] < 65) // se a flecha de parametro p ainda estiver rodando no campo(n chegou na posicao maxima 65) nos movemos ela
            {

                if(flechac[p] != 13)// ela só apaga o que tem antes se ja tiver andado pelo menos uma vez, se não ela apaga o arqueiro
                {
                    gotoxy(flechac[p]-3,flechal[p]);
                    printf(" ");
                }
                gotoxy(flechac[p]-2,flechal[p]);
                printf("%s",flechav);

                flechac[p]++;//esse vetor guarda em cada posicao, o x que a flecha ta, entao sempre que passamos aqui temos que botar +1
            }
            else // se não, apagamos ela uma vez pra que ela n fique parada no fim do campo moscando
            {
                if(apagaumavez[p] == 0)//para que n tenhamos que apagra pra sempre uma flecha que chegou no fim, cada flecha so entra aqui uma vez
                {
                    if(flecha == 15 && nivel == 1 && flechac[flecha-1] >= 65)// se acabaram as flechas na mao e rodando o campo, retorna -2
                    {
                        return -2;
                    }
                    gotoxy(flechac[p]-1,flechal[p]);// apaga flecha parada
                    printf(" ");
                    gotoxy(flechac[p]-2,flechal[p]);
                    printf(" ");
                    gotoxy(flechac[p]-3,flechal[p]);
                    printf(" ");
                    apagaumavez[p] = 1;
                }

            }
        }
    }
    return monstro;//retorna o valor de monstros no campo caso haja
}
void printa_baloes(int u, int estourado[], char balao[][4])
{
    int i, p, w, j;
    for (i = 0; i < BALOES; i++)// cada i representa o balao 0, dps 1, dps 2...
    {

        for(w = 1; w <= BALOES; w++) // estourados é o numero de baloes estourados
        {
            if(estourado[i] == 1)// esse veotr guarda em cada posicao que balao foi estourado naquela rodada
            {
                gotoxy(15+(i*3),25-(u/40));// se for verdade, ele apaga o balao
                for ( j=0; j<4; j++ )
                {
                    printf("   ");
                    gotoxy(15+(i*3),26+j-(u/40));
                }
                i++;// e passa pro proximo pra imprimir
            }
        }
        if(15+(i*3) <= 59)
        {
            gotoxy(15+(i*3),26-(u/40)); // vai pra proxima posicao do balao
            for ( j=0; j<5; j++)// printa balao
            {
                printf("%s", balao[j]);
                gotoxy(15+(i*3),25-(u/40)+j);
            }
        }
    }
    if((u/40) == 20)//apaga resto quando atinge o maximo
    {

        gotoxy(15,5);
        printf("                                                    ");
        gotoxy(15,6);
        printf("                                                    ");
        gotoxy(15,7);
        printf("                                                    ");
        gotoxy(15,8);
        printf("                                                    ");


    }
}
int le_arquivo(char nome_arquivo[], TIPO_JOGADOR arranjo[])
{
    FILE *arq;
    int ret = 0;
    if(arq = fopen(nome_arquivo, "rb"))
    {
        while(!feof(arq))
        {
            ret += fread(&arranjo[ret], sizeof(TIPO_JOGADOR), 1, arq);
        }
    }
    else
    {
        ret = -1;
    }
    fclose(arq);
    return ret;
}
void le_arqueiro(FILE *arq, char arqueiro[][COLUNAS])
{
    int j;
    for(int i=0; i<4; i++)
    {
        for(j=0; j<COLUNAS; j++)
        {
            if(fread(&arqueiro[i][j], sizeof(char), 1, arq) != 1)
                printf("Erro na leitura do arquivo");
            if(arqueiro[i][j] == '\n')
                if(fread(&arqueiro[i][j], sizeof(char), 1, arq) != 1)
                    printf("Erro na leitura do arquivo");
        }
    }
}
void le_balao(FILE *arq, char arqueiro[][4])
{
    int j;
    for(int i=1; i<4; i++)
    {
        arqueiro[0][i-1] = ' ';
        arqueiro[4][i-1] = ' ';
        for(j=0; j<3; j++)
        {
            if(fread(&arqueiro[i][j], sizeof(char), 1, arq) != 1)
                printf("Erro na leitura do arquivo");
            if(arqueiro[i][j] == '\n')
                if(fread(&arqueiro[i][j], sizeof(char), 1, arq) != 1)
                    printf("Erro na leitura do arquivo");
        }
        arqueiro[i][j] = '\0';
    }
}
void imprime_scores(TIPO_JOGADOR arranjo[], int num_elementos)
{
    int i;
    printf("Nome                     score\n");//adicionado o cabecalho para mais praticidade
    for(i=0; i<num_elementos; i++)
    {
        printf("%-25s", arranjo[i].nome);
        printf("%5d\n", arranjo[i].score);
    }
}
int imprime_tela(int qual)
{
    gotoxy(1,1);
    int i, j;
    for(i=0; i<31; i++)
    {
        for(j=0; j<81; j++)
            printf(" ");
        printf("\n");
    }
    gotoxy(1,1);
    TIPO_JOGADOR arranj[10];
    char linha[82], nome_arq[] = "highscores.bin";
    FILE *arq;
    if(qual == MENU)
    {
        if(arq = fopen("menu.txt", "r"))
        {
            while(!feof(arq))
            {
                fread(linha, sizeof(char), 81, arq);
                printf("%s", linha);
            }
            fclose(arq);
            return MENU;
        }
        else
            printf("Erro na leitura do arquivo menu.txt");
    }
    if(qual == JOGO)
    {
        if(arq = fopen("mapa_jogo.txt", "r"))
        {
            while(!feof(arq))
            {
                fread(linha, sizeof(char), 81, arq);
                printf("%s", linha);
            }
            fclose(arq);
            return JOGO;
        }
        else
            printf("Erro na leitura do arquivo mapa_jogo.txt");
    }
    if(qual == HIGHSCORE)
    {
        int elem = le_arquivo(nome_arq, arranj);
        imprime_scores(arranj, elem);
    }
    return -1;
}
void escreve_tela(int qual)
{
    gotoxy(1,1);
    FILE *arq;
    if(qual == MENU)
    {
        if(arq = fopen("menu.txt", "w"))
        {
            fprintf(arq, " ------------------------------------------------------------------------------ \n:         __  __             |              __   __   __   __                  :\n:        |__\)|  | \\  /\\  /    |\\  |        |__| |__| |__| |  |\\  /\\  /         :\n:        |__\)|__|  \\/  \\/     |  \\|        |  | |  \\ |  \\ |__| \\/  \\/          :\n:                                                                              :\n------------------------------------------------------------------------------ \n:                                                                              :\n:                                                                              :\n:                                  NOVO JOGO                                   :\n:                                  MAIORES PLACARES                            :\n:                                  SAIR                                        :\n:                                                                              :\n:                            SELECIONE A OPCAO DESEJADA                        :\n:                                                                              :\n:                                                                              :\n ------------------------------------------------------------------------------ \0");
            fclose(arq);
        }
        else
            printf("Erro na leitura do menu");
    }
    if(qual == JOGO)
    {
        if(arq = fopen("mapa_jogo.txt", "w"))
        {
            fprintf(arq, " ---------------------------------> BOW AND ARROW <---------------------------- \n: SCORE: xxxxx                         LEVEL 1                                 :\n: HIGH SCORE: xxxxx                    ARROWS                   ::::::::::::::: \n ------------------------------------------------------------------------------ \n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n:                                                                              :\n ------------------------------------------------------------------------------ \0");
            fclose(arq);
        }
        else
            printf("Erro na leitura do menu");
    }
}

void imprime_arqueiro(int alt, char arqueiro[4][8], char comand)
{
    if(comand == 'w' && alt > 5)
    {
        int j, i;
        for(i=0; i<4; i++)
        {
            gotoxy(3,alt-1+i);
            for(j=0; j<8; j++)
                printf("%c", arqueiro[i][j]);
        }
    }
    else if(comand == 's' && alt > 5)
    {
        int j, i;
        for(i=0; i<4; i++)
        {
            gotoxy(3, alt+1+ i);
            for(j=0; j<8; j++)
                printf("%c", arqueiro[i][j]);
        }
    }
    else
    {
        int j, i;
        for(i=0; i<4; i++)
        {
            gotoxy(3, alt+ i);
            for(j=0; j<8; j++)
                printf("%c", arqueiro[i][j]);
        }
    }
}
int move__monstro(int monstro,int monstrol[],int monstroc[], char matrizmonstro[][6],int *numeromonstro, int last_monster,int *fim, int flecha2, int *pontos, int alt)
{
    int x,y;
    for(x = 0; x < monstro; x++)
    {
        gotoxy(monstroc[x],monstrol[x]);
        printf(" "); //limpa o "rabinho" do monstro
        monstroc[x]--;//a cada rodada a coluna referencia se paroxima do lado esquerdo
        gotoxy(monstroc[x],monstrol[x]);
        imprime_monstro(matrizmonstro, monstrol, monstroc,monstro);//imprime monstro um pra frente
        if(monstroc[x] <11)//caso o mosntro teha invadido o territorio do arqueiro
        {
            if(monstrol[x] < alt+4&& monstrol[x] > alt-5)// se o monstro se encontrar com o arqueiro, retorna -2
            {
                return -2;
            }
            if(monstroc[x] == 2)//se chegou no fim... soma
            {
                if(*numeromonstro == 30 && monstro == 1)// se o ultimo monstro vivo morrer, o jogo acaba
                {
                    *fim = 1;
                    apaga();
                    gotoxy(25,20);
                    printf("zerou o jogo! parabens");
                    gotoxy(25,21);
                    printf("bonus de %d flechas, + %d", 30-flecha2,(30-flecha2)*50);

                    *pontos = *pontos +(30-flecha2)*50;
                    gotoxy(10,2);
                    printf("%d    ", *pontos);
                    Sleep(3000);
                }
                for(y= 0; y<=4; y++)
                {
                    gotoxy(monstroc[x], monstrol[x]+y);
                    printf("           ");
                }
                monstroc[x] = monstroc[monstro-1];
                monstrol[x] = monstrol[monstro-1];
                monstroc[monstro-1] = 60;
                monstrol[monstro-1] = 0;
                monstro--;
            }
        }
    }
    return monstro;
}
int adiciona__monstro(int r, int x, int monstrol[], int monstroc[], char matrizmonstro[][6], int monstro, int *numeromonstro, int *last_monster)
{
    int linha;
    if(*numeromonstro <= 29)// toda rodada, antes do monstro 30, aparece um monstro novo com localizacao decidida por um rand logo antes da subotina
    {
        *numeromonstro = *numeromonstro+1;//variavel conta monstro
        linha = ((int) r / (double) RAND_MAX * 23)+5;//numero aleatorio de 5 a 28
        gotoxy(60,linha);
        imprime_monstro(matrizmonstro,  monstrol, monstroc, monstro);
        monstrol[x] = linha;
        monstro++;
        if(numeromonstro == 30)
        {
            *last_monster = x; // qual eh o ultimo monstro
        }
    }
    return monstro;
}
void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
                             (COORD)
    {
        x-1,y-1
    });
}
void apaga()
{
    int x,y;
    gotoxy(11,5);
    for(x =0; x<30; x++)
    {
        for(y = 0; y<60; y++)
        {
            printf(" ");
        }
        gotoxy(11,x+5);

    }
}
void imprime_monstro(char matrizmonstro[][6], int monstrol[], int monstroc[],int monstro )
{
    int j, i, x;
    for(x = 0; x<monstro; x++)
    {
        for(i=0; i<5; i++)
        {
            gotoxy(monstroc[x], monstrol[x]+ i);
            for(j=0; j<6; j++)
                printf("%c", matrizmonstro[i][j]);
        }
        gotoxy(monstroc[x]+6,monstrol[x]+4);
        printf(" ");

    }


}
char insere_score(TIPO_JOGADOR nova_entrada, TIPO_JOGADOR arranjo[])
{
    char ret = 'n';//retorno inicializa em 'n' e é editado caso ocorra mudanca no highscore
    int k;
    for(int i=0; i<5; i++)
    {
        if(nova_entrada.score > arranjo[i].score)//como o arranjo ja esta em ordem decrescente, é feita uma analise
        {
            //                                        indo dos maiores aos menores, para que a posicao do score ja esteja certa
            for(int j=5; j!=i; j--)//ja que o ultimo valor pode ser descartado, o laco é feito do ultimo até o
            {
                //                                 primeiro para evitar o uso de uma variavel auxiliar intermediadora
                k = 0;//inicaliza o contador
                while(arranjo[j-1].nome[k] != '\0')//copia o conteudo da posicao de cima para a em questao
                {
                    arranjo[j].nome[k] = arranjo[j-1].nome[k];
                    k++;
                }
                arranjo[j].nome[k] = '\0';
                arranjo[j].score = arranjo[j-1].score;
            }
            k = 0;
            while(nova_entrada.nome[k] != '\0')//copia o conteudo do score a ser inserido na posicao em questao
            {
                arranjo[i].nome[k] = nova_entrada.nome[k];
                k++;
            }
            arranjo[i].nome[k] = '\0';
            arranjo[i].score = nova_entrada.score;
            ret = 's';//retorna 's' quando ha modificação no arranjo
            break;//quando eh encontrado a posicao correta, nao ha porque seguir a repeticao
        }
    }
    return ret;//retorna 's' se ha modificacao no arranjo e 'n' se nao ha
}
void escreve_arquivo(char nome_arquivo[], TIPO_JOGADOR arranjo[], int num)
{
    FILE *arq;//ponteiro para o arquivo
    if(arq = fopen(nome_arquivo, "wb"))//testa se o arquivo foi aberto com sucesso
        for(int i=0; i<num; i++)
            fwrite( &arranjo[i], sizeof(TIPO_JOGADOR), 1, arq);//escreve cada elemento do arranjo no arquivo
}
void le_flecha(FILE *arq, char flechav[])
{
    int j;
    for(int i=0; i<3; i++)
    {
        if(fread(&flechav[i], sizeof(char), 1, arq) != 1)
            printf("Erro na leitura do arquivo");
    }
}
