
/***************************************************************Trabalho Computacional – Programação  II – 2019/2 – Ciência/Engenharia  da Computação Grupo: Hugo Lima Otoch e Estevão Nunes da Silva ***************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ID_JOGADOR_1  1
#define ID_JOGADOR_2  2

typedef struct {
        int linha,coluna;
        char caractere;
    }tCelAtaque; 

typedef struct{

    int qtdLinhas,qtdColunas;
    char MATtabuleiro [100][100];
    int fimAtaque;
    int vidaj1;
    int vidaj2;
    int encF;
    int encA;
    tCelAtaque celAtaque;

}tTabuleiro;

typedef struct{
        int i,j;
    }tLocalizacao;


typedef struct{
    int vida;
    char tipo;
    char tipoDeCelula;
    char reliquia;
    int id;
    char tipoDeCelulaAntiga;
    tLocalizacao localizacao;

    
}tJogador;

typedef struct{
    tJogador j1;
    tJogador j2;
    tTabuleiro tabuleiro;
    tTabuleiro tabuleiroSec;
    int FimJogo;
    int turno;
    int jogadorVez;
    int empate;
}tJogo;

typedef struct{
    int qtdCasas;
    char direcao[20];
    char acao[20];
    char comando[20];
}tJogada;

//funcoes de tabuleiro
tTabuleiro ataqueFogo(tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro queimaPlanta(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro derreteGelo(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro vaporizaAgua(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro vaporizaAguaTabSec(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);

tTabuleiro ataqueAgua(tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro congelaAgua(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro semearPlanta(tTabuleiro tabuleiro,tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiroSec);
tTabuleiro apagaFogo(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);
tTabuleiro apagaFogoTabSec(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec);

tTabuleiro EfetuaMovimento(tJogada jogada,tTabuleiro tabuleiro,tJogador jogador);

tTabuleiro IniciaTabuleiro(){// Le o arquivo field.txt para criar o tabuleiro inicial.
    tTabuleiro tabuleiro;
    int i=0,j=0;
    FILE*arq;
    arq=fopen("field.txt","r");
    if(arq==NULL){
        printf("Erro na abertura de arquivo! Programa terminando...");
        exit(1);
    }
    fscanf(arq,"%d",&tabuleiro.qtdLinhas);
    fscanf(arq, "%d",&tabuleiro.qtdColunas);
    for(i=0;i<tabuleiro.qtdLinhas;i++){
        for(j=0;j<tabuleiro.qtdColunas;j++){
            fscanf(arq,"\n%c",&tabuleiro.MATtabuleiro[i][j]);
        }
    }
    fclose(arq);
    return tabuleiro;
}



//funcoes de jogador
tJogador LocalizaJogador(tJogador jogador,tTabuleiro tabuleiro);
tJogador VerificaEmQualCelulaEstaJogador(tJogador jogador,tTabuleiro tabuleiro);
tJogada VerificaSeAtravessaObstaculoJogada(tTabuleiro tabuleiro,tJogada jogada,tJogador jogador);

tJogador CriaJogador(int idJogador){
    tJogador jogador;
    jogador.id = idJogador;

    if(jogador.id==1){
        jogador.tipo = 'f';
        jogador.reliquia = '*';
    }else{
        jogador.tipo = 'a';
        jogador.reliquia = '$';
    }
    jogador.vida = 30;
    jogador.tipoDeCelula = '.';
    
    return jogador;
}
int VerificaSeMorreJogador(tJogador jogador){
    if(jogador.vida<=0){
        return 1;
    }
    else{
        return 0;
    }
     
}


// funcoes de jogo
tJogo CriaJogo(){
    tJogo jogo;
  
    jogo.j1.id = 1;
    jogo.j2.id = 2;

    jogo.tabuleiro = IniciaTabuleiro();
    jogo.j1 = CriaJogador(ID_JOGADOR_1);
    jogo.j2 = CriaJogador(ID_JOGADOR_2);
    jogo.FimJogo = 0;

    return jogo;
}

tJogo AnalisaReliquia(tJogador jogador,tJogo jogo,tTabuleiro tabuleiro);
tJogo verificaEmpate(tJogo jogo);

tJogador LocalizaJogador(tJogador jogador,tTabuleiro tabuleiro){// Localiza o jogador no tabuleiro.
    int i=0,j=0;
    if(jogador.tipo=='f'){
        for(i=0;i<tabuleiro.qtdLinhas;i++){
            for(j=0;j<tabuleiro.qtdColunas;j++){
                if(tabuleiro.MATtabuleiro[i][j]=='f'){
                    jogador.localizacao.i=i;
                    jogador.localizacao.j=j;
                }
            }
        }
    }
    if(jogador.tipo=='a'){
        for(i=0;i<tabuleiro.qtdColunas;i++){
            for(j=0;j<tabuleiro.qtdColunas;j++){
                if(tabuleiro.MATtabuleiro[i][j]=='a'){
                    jogador.localizacao.i=i;
                    jogador.localizacao.j=j;
                }
            }
        }
    }
    
    return jogador;
}

tJogo VerificaEncurralamentoFogo(tJogo jogo){// Verifica se o mago do fogo foi encurralado ou terminou o turno sobre uma celula de agua.           
        if(jogo.j1.tipoDeCelula=='A' ||  jogo.tabuleiro.encF==1 && jogo.j1.tipoDeCelula!='F' && jogo.j1.tipoDeCelula!='*'){
            jogo.FimJogo=2;
            jogo.empate++;
        } 
    
    return jogo;
}

tJogo VerificaEncurralamentoAgua(tJogo jogo){// Verifica se o mago da agua foi encurralado ou terminou o turno sobre uma celula de fogo.
     
        if(jogo.tabuleiro.encA==4 && jogo.j2.tipoDeCelula=='A'){
            jogo.FimJogo=1;
            jogo.empate++;
        }
       //encurrala agua no gelo ou agua perde no fogo
        if(jogo.j2.tipoDeCelula=='F' || jogo.j2.tipoDeCelula!='.' && jogo.tabuleiro.encA==1 && jogo.j2.tipoDeCelula!='A' && jogo.j2.tipoDeCelula!='$'){
            jogo.FimJogo=1;
            jogo.empate++;
        }
        //encurrala agua na planta
        if(jogo.tabuleiro.encA==1 && jogo.j2.tipoDeCelula=='.'){
            jogo.FimJogo=1;
            jogo.empate++;
        }
             
    
    return jogo;
}

tJogo verificaEmpate(tJogo jogo){
    if(jogo.empate>=2){
        jogo.FimJogo=3;
    }
    return jogo;
} 

tJogador VerificaEmQualCelulaEstaJogador(tJogador jogador,tTabuleiro tabuleiro){// Verifica a celula em que o jogador se encontra no tabuleiro.
    jogador = LocalizaJogador(jogador,tabuleiro);
    
    if(tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=='A'){
        jogador.tipoDeCelula='A';
    }
    if(tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=='F'){
        jogador.tipoDeCelula='F';
    }
    if(tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=='.'){
        jogador.tipoDeCelula='.';
    }
    if(tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=='*'){
        jogador.tipoDeCelula='*';
    }
    if(tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=='$'){
        jogador.tipoDeCelula='$';
    }
    return jogador;
}



//funcoes de jogada

tJogada RecebeJogada(tJogador jogador1,tJogador jogador2,tJogo jogo){// Le a jogada inserida pelo jogador.
    tJogada jogada;
    int qtdCasas;
    char s1[20];

    scanf("%s",jogada.comando);
    if(jogada.comando[0]!='a'){  
        strcpy(jogada.direcao,jogada.comando);
        scanf("%d",&jogada.qtdCasas);
    }
    printf("\n\n\n");
    return jogada;

}

int VerificaValidezJogada(tTabuleiro tabuleiro,tJogada jogada,tJogador jogador){// Verifica se o numero de casas a ser jogada é maior ou igual a 1.
   if(jogada.qtdCasas<1){
       return 0;
   }
   return 1;
    
}

tJogada VerificaSeAtravessaObstaculoJogada(tTabuleiro tabuleiro,tJogada jogada,tJogador jogador){// Verifica se o movimento jogado é impedido por alguma barreira.
    char s1[20]="direita";
    char s2[20]="esquerda";
    char s3[20]="tras";
    char s4[20]="frente";
    int i=0,j=0;
    int cont=0;
 

    if(jogada.direcao[0]=='d'){// Verifica pra direita.
        for(j=jogador.localizacao.j+1;j<=jogador.localizacao.j+jogada.qtdCasas;j++){
	        i = jogador.localizacao.i;
                if(jogador.tipo=='f'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='a'||tabuleiro.MATtabuleiro[i][j]=='$'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                    }
            }
            if(jogador.tipo=='a'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='f'||tabuleiro.MATtabuleiro[i][j]=='*'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                     }
            }
        }
        if(jogador.localizacao.j+jogada.qtdCasas>tabuleiro.qtdColunas-1){
            jogada.qtdCasas=tabuleiro.qtdColunas-jogador.localizacao.j -1;
        }
    }     
    if(jogada.direcao[0]=='e'){// Verifica pra esquerda.
        for(j=jogador.localizacao.j-1;j>=jogador.localizacao.j-jogada.qtdCasas;j--){
	        i = jogador.localizacao.i;
                if(jogador.tipo=='f'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='a'||tabuleiro.MATtabuleiro[i][j]=='$'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                    }
            }
            if(jogador.tipo=='a'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='f'||tabuleiro.MATtabuleiro[i][j]=='*'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                    }
            }
        }
        if(jogador.localizacao.j-jogada.qtdCasas<0){
            jogada.qtdCasas=jogador.localizacao.j;
        }
    }     
    if(jogada.direcao[0]=='t'){// Verifica pra tras.
        for(i=jogador.localizacao.i+1;i<=jogador.localizacao.i+jogada.qtdCasas;i++){
	        j = jogador.localizacao.j;
             if(jogador.tipo=='f'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='a'||tabuleiro.MATtabuleiro[i][j]=='$'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                     }
            }
            if(jogador.tipo=='a'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='f'||tabuleiro.MATtabuleiro[i][j]=='*'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                    cont++;
                    }
            }
        }
        if(jogador.localizacao.i+jogada.qtdCasas>tabuleiro.qtdLinhas -1){
            jogada.qtdCasas=tabuleiro.qtdLinhas-jogador.localizacao.i -1;
        }
    }     
    if(jogada.direcao[0]=='f'){// Verifica pra frente.
        for(i=jogador.localizacao.i-1;i>=jogador.localizacao.i-jogada.qtdCasas;i--){
	        j = jogador.localizacao.j;
             if(jogador.tipo=='f'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='a'||tabuleiro.MATtabuleiro[i][j]=='$'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                else{
                    cont++;
                }
            }
            if(jogador.tipo=='a'){
                    if(tabuleiro.MATtabuleiro[i][j]=='P'||tabuleiro.MATtabuleiro[i][j]=='G'||tabuleiro.MATtabuleiro[i][j]=='X'||tabuleiro.MATtabuleiro[i][j]=='f'||tabuleiro.MATtabuleiro[i][j]=='*'){           
                        jogada.qtdCasas=cont;                
                        break;
                    }
                    else{
                        cont++;
                    }
            }
        }
        if(jogador.localizacao.i-jogada.qtdCasas<0){
            jogada.qtdCasas=jogador.localizacao.i;
        }
    }     
    return jogada;
}     


tTabuleiro Ataque(tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiro,tJogo jogo){// Analisa cada celula em que o ataque atingira e aplica os efeitos necessarios no tabuleiro.
    
    int i=0,j=0,k=0;
	if(jogo.jogadorVez==1){//Aplica os efeitos dos ataques do mago de fogo.
		for(i=jogador1.localizacao.i;i>=0;i--){
            j= jogador1.localizacao.j;
            jogo.tabuleiroSec.celAtaque.caractere = jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;           
            jogo.tabuleiro = ataqueFogo(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            if(jogo.tabuleiro.fimAtaque==1){// Verifica se o ataque acabou.

                jogo.tabuleiro.fimAtaque=0;
                break;
                
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='A'){
                jogo.tabuleiro=vaporizaAgua(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=vaporizaAguaTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
                
            }
        }
    
	    
        for(i=jogador1.localizacao.i;i<=jogo.tabuleiro.qtdLinhas;i++){
            j=jogador1.localizacao.j;
            jogo.tabuleiroSec.celAtaque.caractere= jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro = ataqueFogo(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break;
                
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='A'){
                jogo.tabuleiro=vaporizaAgua(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=vaporizaAguaTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
               
            }


        }
        for(j=jogador1.localizacao.j;j>=0;j--){
            i=jogador1.localizacao.i;
            jogo.tabuleiroSec.celAtaque.caractere=jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro = ataqueFogo(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break;
                
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='A'){
                jogo.tabuleiro=vaporizaAgua(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=vaporizaAguaTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
      
            }


        }
        for(j=jogador1.localizacao.j;j<=jogo.tabuleiro.qtdColunas;j++){
            i=jogador1.localizacao.i;
            jogo.tabuleiroSec.celAtaque.caractere=jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro = ataqueFogo(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break; 
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='A'){
                jogo.tabuleiro=vaporizaAgua(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=vaporizaAguaTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
               
            }


        }
	}
    if(jogo.jogadorVez==2){// Aplica os efeitos dos ataques do mago de agua.
        for(i=jogador2.localizacao.i;i>=0;i--){
			j=jogador2.localizacao.j;
            jogo.tabuleiroSec.celAtaque.caractere = jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro =ataqueAgua(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            k=i;
           
            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break;
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='F'){
                jogo.tabuleiro=apagaFogo(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=apagaFogoTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
               
            }
                             
        }
        for(i=jogador2.localizacao.i;i<=jogo.tabuleiro.qtdLinhas;i++){
            j=jogador2.localizacao.j;
            jogo.tabuleiroSec.celAtaque.caractere= jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro =ataqueAgua(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            k=i;
           
            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break;
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='F'){
                jogo.tabuleiro=apagaFogo(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=apagaFogoTabSec(jogo.tabuleiro,jogo.tabuleiroSec);               
            }
                     


        }
        for(j=jogador2.localizacao.j;j>=0;j--){
            i=jogador2.localizacao.i;
            jogo.tabuleiroSec.celAtaque.caractere=jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro=ataqueAgua(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            k=j;
           

            if(jogo.tabuleiro.fimAtaque==1){

                jogo.tabuleiro.fimAtaque=0;
                break;
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='F'){
                jogo.tabuleiro=apagaFogo(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=apagaFogoTabSec(jogo.tabuleiro,jogo.tabuleiroSec);
                
            }
                      


        }
        for(j=jogador2.localizacao.j;j<=jogo.tabuleiro.qtdColunas;j++){
            i=jogador2.localizacao.i;
            jogo.tabuleiroSec.celAtaque.caractere=jogo.tabuleiroSec.MATtabuleiro[i][j];
            jogo.tabuleiroSec.celAtaque.linha=i;
            jogo.tabuleiroSec.celAtaque.coluna=j;
            jogo.tabuleiro=ataqueAgua(jogador1,jogador2,jogo.tabuleiro,jogo.tabuleiroSec);
            k=j;
            
            if(jogo.tabuleiro.fimAtaque==1){
                
                jogo.tabuleiro.fimAtaque=0;
                break;
            }
            if(jogo.tabuleiroSec.MATtabuleiro[i][j]=='F'){
                jogo.tabuleiro=apagaFogo(jogo.tabuleiro,jogo.tabuleiroSec);
                jogo.tabuleiroSec=apagaFogoTabSec(jogo.tabuleiro,jogo.tabuleiroSec);        
            }                      
        }      
    }	
			
    return jogo.tabuleiro;

}

tTabuleiro ataqueAgua(tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){//Efeitos do ataque de agua.
    if(tabuleiroSec.celAtaque.caractere=='f'){
        tabuleiro.vidaj1=tabuleiro.vidaj1-10;
    }
    if(tabuleiroSec.celAtaque.caractere=='P'){
        tabuleiro=semearPlanta(tabuleiro,jogador1,jogador2,tabuleiroSec);
        tabuleiro.fimAtaque = 1;
    }
    
    if(tabuleiroSec.celAtaque.caractere=='A'){
        tabuleiro=congelaAgua(tabuleiro,tabuleiroSec);
        tabuleiro.fimAtaque = 1;
    }
    if(tabuleiroSec.celAtaque.caractere=='G' || tabuleiroSec.celAtaque.caractere== 'X'){
        tabuleiro.fimAtaque =1;
    }
    return tabuleiro;
}

tTabuleiro semearPlanta(tTabuleiro tabuleiro,tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiroSec){//Semea a planta e verifica o encurralamento por planta.
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='.'){
        tabuleiro.MATtabuleiro[i-1][j]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='f'){
       tabuleiro.encF = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='a'){
        tabuleiro.encA = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='.'){
        tabuleiro.MATtabuleiro[i+1][j]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='f'){
        tabuleiro.encF = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='a'){
        tabuleiro.encA = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='.'){
        tabuleiro.MATtabuleiro[i][j-1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='f'){
        tabuleiro.encF = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='a'){
        tabuleiro.encA = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='.'){
        tabuleiro.MATtabuleiro[i][j+1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='f'){
        tabuleiro.encF=1;
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='a'){
        tabuleiro.encA=1;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='.'){
        tabuleiro.MATtabuleiro[i-1][j-1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='f'){
        tabuleiro.encF=1;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='a'){
        tabuleiro.encA=1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='.'){
        tabuleiro.MATtabuleiro[i+1][j+1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='f'){
        tabuleiro.encF=1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='a'){
        tabuleiro.encA=1;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='.'){
        tabuleiro.MATtabuleiro[i-1][j+1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='f'){
        tabuleiro.encF = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='a'){
        tabuleiro.encA = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='.'){
        tabuleiro.MATtabuleiro[i+1][j-1]='P';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='f'){
        tabuleiro.encF = 1;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='a'){
        tabuleiro.encA = 1;
    }
    return tabuleiro;      
}

tTabuleiro apagaFogoTabSec(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;
    if(tabuleiroSec.MATtabuleiro[i][j]=='F'){
        tabuleiroSec.MATtabuleiro[i][j]='.';
        tabuleiro.MATtabuleiro[i][j]='.';
    }
    return tabuleiroSec;
}

tTabuleiro apagaFogo(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;
    if(tabuleiroSec.MATtabuleiro[i][j]=='F'){
        tabuleiroSec.MATtabuleiro[i][j]='.';
        tabuleiro.MATtabuleiro[i][j]='.';
    }
    return tabuleiro;
}
tTabuleiro congelaAgua(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;
    if(tabuleiroSec.MATtabuleiro[i][j]=='A'){
        tabuleiro.MATtabuleiro[i][j]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i][j]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='A'){
        tabuleiro.MATtabuleiro[i-1][j]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='A'){
        tabuleiro.MATtabuleiro[i+1][j]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='A'){
        tabuleiro.MATtabuleiro[i][j-1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='A'){
        tabuleiro.MATtabuleiro[i][j+1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='A'){
        tabuleiro.MATtabuleiro[i-1][j-1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='A'){
        tabuleiro.MATtabuleiro[i+1][j+1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='A'){
        tabuleiro.MATtabuleiro[i-1][j+1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='a'){
        tabuleiro.encA=4;
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='A'){
        tabuleiro.MATtabuleiro[i+1][j-1]='G';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='a'){
        tabuleiro.encA=4;
    }
    return tabuleiro;
}

tTabuleiro ataqueFogo(tJogador jogador1,tJogador jogador2,tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){//Efeitos do ataque de fogo.
    if(tabuleiroSec.celAtaque.caractere=='a'){
        tabuleiro.vidaj2=tabuleiro.vidaj2-10;      
    }
    if(tabuleiroSec.celAtaque.caractere=='P'){
        tabuleiro=queimaPlanta(tabuleiro,tabuleiroSec);
        tabuleiro.fimAtaque = 1;
    }
    if(tabuleiroSec.celAtaque.caractere=='G'){
        tabuleiro=derreteGelo(tabuleiro,tabuleiroSec);
        tabuleiro.fimAtaque = 1;
    }
    if(tabuleiroSec.celAtaque.caractere=='X'){
        tabuleiro.fimAtaque = 1;
    }
    return tabuleiro;
} 
tTabuleiro queimaPlanta(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;

    tabuleiro.MATtabuleiro[i][j]='F';
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='P'){
        tabuleiro.MATtabuleiro[i-1][j]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='P'){
        tabuleiro.MATtabuleiro[i+1][j]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='P'){
        tabuleiro.MATtabuleiro[i][j-1]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='P'){
        tabuleiro.MATtabuleiro[i-1][j-1]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='P'){
        tabuleiro.MATtabuleiro[i-1][j+1]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='P'){
        tabuleiro.MATtabuleiro[i+1][j-1]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='P'){
        tabuleiro.MATtabuleiro[i+1][j+1]='F';
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='P'){
        tabuleiro.MATtabuleiro[i][j+1]='F';
    }

    return tabuleiro;
}

tTabuleiro derreteGelo(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;

    tabuleiro.MATtabuleiro[i][j]='A';
    if(tabuleiroSec.MATtabuleiro[i-1][j]=='G' || tabuleiroSec.MATtabuleiro[i-1][j]=='.'){
        tabuleiro.MATtabuleiro[i-1][j]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j]=='G' || tabuleiroSec.MATtabuleiro[i+1][j]=='.'){
        tabuleiro.MATtabuleiro[i+1][j]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i][j-1]=='G' || tabuleiroSec.MATtabuleiro[i][j-1]=='.'){
        tabuleiro.MATtabuleiro[i][j-1]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j-1]=='G' || tabuleiroSec.MATtabuleiro[i-1][j-1]=='.'){
        tabuleiro.MATtabuleiro[i-1][j-1]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i-1][j+1]=='G' || tabuleiroSec.MATtabuleiro[i-1][j+1]=='.'){
        tabuleiro.MATtabuleiro[i-1][j+1]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j-1]=='G' || tabuleiroSec.MATtabuleiro[i+1][j-1]=='.'){
        tabuleiro.MATtabuleiro[i+1][j-1]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i+1][j+1]=='G' || tabuleiroSec.MATtabuleiro[i+1][j+1]=='.'){
        tabuleiro.MATtabuleiro[i+1][j+1]='A';
    }
    if(tabuleiroSec.MATtabuleiro[i][j+1]=='G' || tabuleiroSec.MATtabuleiro[i][j+1]=='.'){
        tabuleiro.MATtabuleiro[i][j+1]='A';
    }

    return tabuleiro;
}
tTabuleiro vaporizaAguaTabSec(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;
    if(tabuleiroSec.MATtabuleiro[i][j]=='A'){
        tabuleiroSec.MATtabuleiro[i][j] = '.';
        tabuleiro.MATtabuleiro[i][j]='.';
    }


    return tabuleiroSec;
}
tTabuleiro vaporizaAgua(tTabuleiro tabuleiro,tTabuleiro tabuleiroSec){
    int i=0,j=0;
    i=tabuleiroSec.celAtaque.linha;
    j=tabuleiroSec.celAtaque.coluna;

    if(tabuleiroSec.MATtabuleiro[i][j]=='A'){
        tabuleiroSec.MATtabuleiro[i][j] = '.';
        tabuleiro.MATtabuleiro[i][j]='.';
    }

    return tabuleiro;
}

tTabuleiro AtualizaTabuleiro(tTabuleiro tabuleiro,tJogada jogada,tJogador jogador){//Atualiza a celula em que o mago se encontrava.
    int i=0,j=0,qtdC=0;
    qtdC=jogada.qtdCasas;

    if(jogada.direcao[0]=='e'){
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=jogador.tipoDeCelulaAntiga;
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j-qtdC]=jogador.tipo;
    }
    if(jogada.direcao[0]=='d'){
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=jogador.tipoDeCelulaAntiga;
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j+qtdC]=jogador.tipo;
    }
    if(jogada.direcao[0]=='f'){
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=jogador.tipoDeCelulaAntiga;
        tabuleiro.MATtabuleiro[jogador.localizacao.i-qtdC][jogador.localizacao.j]=jogador.tipo;
    }
    if(jogada.direcao[0]=='t'){
        tabuleiro.MATtabuleiro[jogador.localizacao.i][jogador.localizacao.j]=jogador.tipoDeCelulaAntiga;
        tabuleiro.MATtabuleiro[jogador.localizacao.i+qtdC][jogador.localizacao.j]=jogador.tipo;
    }


    return tabuleiro;
}
tJogador verificaCelulaFinal(tJogador jogador,tTabuleiro tabuleiro,tJogada jogada){//Verifica a celula em que o jogador ira se encontrar ao final do turni.
    int i=0,j=0;
    i=jogador.localizacao.i;
    j=jogador.localizacao.j;
    if(jogada.direcao[0]=='e'){
        jogador.tipoDeCelula=tabuleiro.MATtabuleiro[i][j-jogada.qtdCasas];
    }
    if(jogada.direcao[0]=='d'){
        jogador.tipoDeCelula=tabuleiro.MATtabuleiro[i][j+jogada.qtdCasas];
    }
    if(jogada.direcao[0]=='f'){
        jogador.tipoDeCelula=tabuleiro.MATtabuleiro[i-jogada.qtdCasas][j];
    }
    if(jogada.direcao[0]=='t'){
        jogador.tipoDeCelula=tabuleiro.MATtabuleiro[i+jogada.qtdCasas][j];
    }
    return jogador;
}

tJogada CorrigeJogada(tJogada jogada,tTabuleiro tabuleiro,tJogador jogador){//Corrige o numero de casas que o mago ira andar se houver um obstaculo no caminho.
    
        jogada = VerificaSeAtravessaObstaculoJogada(tabuleiro,jogada,jogador); 
          
    
    return jogada;
    
}


tJogo AnalisaReliquia(tJogador jogador,tJogo jogo,tTabuleiro tabuleiro){//Verifica se o jogador encontrou sua reliquia.
    jogador = VerificaEmQualCelulaEstaJogador(jogador,tabuleiro);
    if(jogador.tipo=='f'){
        if(jogador.tipoDeCelula=='*'){
            jogo.FimJogo = 1;
        }
    }else{  
        if(jogador.tipo=='a'){
            if(jogador.tipoDeCelula=='$'){
                jogo.FimJogo = 2;
            }   
        }  
    }
    return jogo;
}

void ImprimeTabMenu(tJogo jogo){//Imprime o tabuleiro e o menu com as informaçoes.
    int i=0,j=0;
    for(i=0;i<jogo.tabuleiro.qtdLinhas;i++){
        for(j=0;j<jogo.tabuleiro.qtdColunas;j++){
            printf("%c",jogo.tabuleiro.MATtabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    if(jogo.jogadorVez==1){
        printf("Jogador da vez: %c\n",jogo.j1.tipo);
    }else{
        printf("Jogador da vez: %c\n",jogo.j2.tipo);
    }
    printf("f: %c\n",jogo.j1.tipoDeCelula);
    printf("a: %c\n",jogo.j2.tipoDeCelula);
    printf("Vida f: %d\n",jogo.j1.vida);
    printf("Vida a: %d\n",jogo.j2.vida);
    printf("\nComando: ");
    
}


void ImprimeTabVITORIA(tJogo jogo){// Imprime o tabuleiro e o menu de vitoria ao acabar o jogo.
    int i=0,j=0;
    for(i=0;i<jogo.tabuleiro.qtdLinhas;i++){
        for(j=0;j<jogo.tabuleiro.qtdColunas;j++){
            printf("%c",jogo.tabuleiro.MATtabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    if(jogo.jogadorVez==1){
        printf("Jogador da vez: %c\n",jogo.j2.tipo);
    }else{
        printf("Jogador da vez: %c\n",jogo.j1.tipo);
    }
    printf("f: %c\n",jogo.j1.tipoDeCelula);
    printf("a: %c\n",jogo.j2.tipoDeCelula);
    printf("Vida f: %d\n",jogo.j1.vida);
    printf("Vida a: %d\n",jogo.j2.vida);
}


tJogador verificaSeTomaDano(tJogador jogador,tTabuleiro tabuleiro,tJogada jogada){//Verifica se o mago adversario perde vida por um ataque.
    int i=0,j=0;
    
    if(jogador.tipo=='f'){
        if(jogada.direcao[0]=='d'){
            i=jogador.localizacao.i;
            for(j=jogador.localizacao.j;j<=jogador.localizacao.j+jogada.qtdCasas;j++){
                if(tabuleiro.MATtabuleiro[i][j]=='A'){
                    jogador.vida=jogador.vida-10;
                }
                
            }
        }
        if(jogada.direcao[0]=='e'){
            i=jogador.localizacao.i;
            for(j=jogador.localizacao.j;j>=jogador.localizacao.j-jogada.qtdCasas;j--){
                if(tabuleiro.MATtabuleiro[i][j]=='A'){
                    jogador.vida=jogador.vida-10;
                }
            }

        }
        if(jogada.direcao[0]=='t'){
            j=jogador.localizacao.j;
            for(i=jogador.localizacao.i;i<=jogador.localizacao.i+jogada.qtdCasas;i++){
                if(tabuleiro.MATtabuleiro[i][j]=='A'){
                    jogador.vida=jogador.vida-10;
                }
            }
        }
        if(jogada.direcao[0]=='f'){
            j=jogador.localizacao.j;
            for(i=jogador.localizacao.i;i>=jogador.localizacao.i-jogada.qtdCasas;i--){
               if(tabuleiro.MATtabuleiro[i][j]=='A'){
                   jogador.vida=jogador.vida-10;
               } 
            }
        }
    }
    if(jogador.tipo=='a'){
        if(jogada.direcao[0]=='d'){
            i=jogador.localizacao.i;
            for(j=jogador.localizacao.j;j<=jogador.localizacao.j+jogada.qtdCasas;j++){
                if(tabuleiro.MATtabuleiro[i][j]=='F'){
                    jogador.vida=jogador.vida-10;
                }
                
            }
        }
        if(jogada.direcao[0]=='e'){
            i=jogador.localizacao.i;
            for(j=jogador.localizacao.j;j>=jogador.localizacao.j-jogada.qtdCasas;j--){
                if(tabuleiro.MATtabuleiro[i][j]=='F'){
                    jogador.vida=jogador.vida-10;
                }
            }

        }
        if(jogada.direcao[0]=='t'){
            j=jogador.localizacao.j;
            for(i=jogador.localizacao.i;i<=jogador.localizacao.i+jogada.qtdCasas;i++){
                if(tabuleiro.MATtabuleiro[i][j]=='F'){
                    jogador.vida=jogador.vida-10;
                }
            }
        }
        if(jogada.direcao[0]=='f'){
            j=jogador.localizacao.j;
            for(i=jogador.localizacao.i;i>=jogador.localizacao.i-jogada.qtdCasas;i--){
               if(tabuleiro.MATtabuleiro[i][j]=='F'){
                   jogador.vida=jogador.vida-10;
               } 
            }
        }
    }

    return jogador;
}

int verificaJogadorVez(tJogo jogo){//Verifica de quem é a vez de jogar.
    if(jogo.turno%2==0){
        return 2;
    }else{
        return 1;
    }
}
tJogador VerificaCelulaAntiga(tJogador jogador,tJogo jogo){//Verifica a celula em que o jogador se encontrava antes da jogada.
    jogador.tipoDeCelulaAntiga=jogador.tipoDeCelula;
    return jogador;
}

tJogo IgualaTabuleiros(tJogo jogo){// iguala o tabuleiro secundario(antigo) com o principal(novo).
    int i=0,j=0;
    jogo.tabuleiroSec.qtdLinhas=jogo.tabuleiro.qtdLinhas;
    jogo.tabuleiroSec.qtdColunas=jogo.tabuleiro.qtdColunas;
    for(i=0;i<jogo.tabuleiro.qtdLinhas;i++){
            for(j=0;j<jogo.tabuleiro.qtdColunas;j++){
                jogo.tabuleiroSec.MATtabuleiro[i][j]=jogo.tabuleiro.MATtabuleiro[i][j];
            }
        }
        return jogo;

}
tJogo AplicaDanoNoTab(tJogo jogo){// Altera a vida dos jogadores no menu.
    jogo.tabuleiro.vidaj1=jogo.j1.vida;
    jogo.tabuleiroSec.vidaj1=jogo.j1.vida;
    jogo.tabuleiro.vidaj2=jogo.j2.vida;
    jogo.tabuleiroSec.vidaj2=jogo.j2.vida;

    return jogo;
}

void ComecaJogo(tJogo jogo){
    tJogada jogada;
    int k,i=0,j=0;
    jogo.tabuleiro.encA=0;
    jogo.tabuleiro.encF=0;
    jogo.turno = 0;
    jogo.tabuleiro=IniciaTabuleiro(jogo.tabuleiro);
    jogo.j1.tipoDeCelula = '.';
    jogo.j2.tipoDeCelula = '.';
    jogo.j1.tipoDeCelulaAntiga='.';
    jogo.j2.tipoDeCelulaAntiga='.';
    jogo.tabuleiro.vidaj1=30;
    jogo.tabuleiro.vidaj2=30;
    jogo=IgualaTabuleiros(jogo);
    while(1){
        jogo.tabuleiro.encA=0;
        jogo.tabuleiro.encF=0;
        k=1;
        jogo.turno++;
        jogo=IgualaTabuleiros(jogo);
        jogo.jogadorVez = verificaJogadorVez(jogo);
        jogo.j1 = LocalizaJogador(jogo.j1,jogo.tabuleiro);
        jogo.j2 = LocalizaJogador(jogo.j2,jogo.tabuleiro);
        jogo.j1= VerificaCelulaAntiga(jogo.j1,jogo);
        jogo.j2=VerificaCelulaAntiga(jogo.j2,jogo);
       

        ImprimeTabMenu(jogo);
        while(1){
            jogada=RecebeJogada(jogo.j1,jogo.j2,jogo);            
           if(jogada.qtdCasas>=1){
               break;
           }
           if(jogada.comando[0]=='a'){
               break;
           }
           else{
               printf("JOGADA INVALIDA");
           }
           
        }
        if(jogada.comando[0]=='a'){
            jogo.tabuleiro = Ataque(jogo.j1,jogo.j2,jogo.tabuleiro,jogo);
            jogo=IgualaTabuleiros(jogo);
            jogo.j1.vida=jogo.tabuleiro.vidaj1;
            jogo.j2.vida=jogo.tabuleiro.vidaj2;
            jogo = VerificaEncurralamentoAgua(jogo);
            jogo = VerificaEncurralamentoFogo(jogo);
            jogo = verificaEmpate(jogo);
            
        }
        else{
            if(jogo.turno%2==0){//Se for o turno do jogador 2
                
                jogada =CorrigeJogada(jogada,jogo.tabuleiro,jogo.j2);
              
                jogo.j2=VerificaCelulaAntiga(jogo.j2,jogo);
                

                jogo.j2= verificaCelulaFinal(jogo.j2,jogo.tabuleiro,jogada);
                if(jogada.qtdCasas==0){
                    jogo.j2.tipoDeCelula=jogo.j2.tipoDeCelulaAntiga;
                }


                jogo.j2= verificaSeTomaDano(jogo.j2,jogo.tabuleiro,jogada);
                jogo=AplicaDanoNoTab(jogo);

                jogo.tabuleiro = AtualizaTabuleiro(jogo.tabuleiro,jogada,jogo.j2);
                jogo=AnalisaReliquia(jogo.j2,jogo,jogo.tabuleiro);
                jogo = VerificaEncurralamentoFogo(jogo);
                jogo = VerificaEncurralamentoAgua(jogo);
                jogo= verificaEmpate(jogo);
                
                }
                           
            if(jogo.turno%2!=0){//Se for o turno do jogador 1
                
                jogada =CorrigeJogada(jogada,jogo.tabuleiro,jogo.j1);
               
                jogo.j1=VerificaCelulaAntiga(jogo.j1,jogo);
               

                jogo.j1= verificaCelulaFinal (jogo.j1,jogo.tabuleiro,jogada);

                 if(jogada.qtdCasas==0){
                    jogo.j1.tipoDeCelula=jogo.j1.tipoDeCelulaAntiga;
                }

                jogo.j1= verificaSeTomaDano(jogo.j1,jogo.tabuleiro,jogada);
                jogo=AplicaDanoNoTab(jogo);


                jogo.tabuleiro = AtualizaTabuleiro(jogo.tabuleiro,jogada,jogo.j1);
                jogo=AnalisaReliquia(jogo.j1,jogo,jogo.tabuleiro);

                jogo = VerificaEncurralamentoFogo(jogo);
                jogo = VerificaEncurralamentoAgua(jogo);
                jogo= verificaEmpate(jogo);
               
                                
            }
            
        }
        if(jogo.j1.vida<=0){
            jogo.FimJogo=2;
            ImprimeTabVITORIA(jogo);
        }
        if(jogo.j2.vida<=0){
            jogo.FimJogo=1;
            ImprimeTabVITORIA(jogo);
        }
        jogo=verificaEmpate(jogo);

        if(jogo.FimJogo==3){//Caso de empate
            ImprimeTabVITORIA(jogo);
            printf("\n");
            printf("O jogo terminou empatado!\n");
            exit(1);
        }
        if(jogo.FimJogo==1){//Caso de vitoria do mago do fogo
            ImprimeTabVITORIA(jogo);
            printf("\n");
            printf("O mago do fogo venceu!\n");
            exit(1);
        }
        if(jogo.FimJogo==2){//Caso de vitoria do mago de agua
            ImprimeTabVITORIA(jogo);
            printf("\n");
            printf("O mago da agua venceu!\n");
            exit(1);
        }
        
    }
}


int main(){
    tJogo jogo;
    jogo=CriaJogo();
    ComecaJogo(jogo);

    return 0;
}




        