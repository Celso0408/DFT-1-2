/*BY FILIPE MATUSALEM, MARCH 2014     filipematus@gmail.com */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[])
{
FILE *potcar,*potcarnew,*vfile,*vfile05,*inp;
int i,j,jumps,nupon,nuk,n,iniciovez,orbital,core,valence;
float amplitude;
double a, b, r[1500], patomo[1500], pion[1500], VAR[1500], kmax, VVK[1500],CUT,ca,inicio, inicial, final, alpha, beta, pi, fourier,betaca;
char ch, str[200]="POTCARcut", str1[50];
char Linha[200];

 if( argc < 2 ){
printf("\n\n");
printf("EEEEEEE   RRRRRRRR   RRRRRRRR   OOOOOOO   RRRRRRRR\n");
printf("EE        RR    RR   RR    RR   OO   OO   RR    RR\n");
printf("EE        RR    RR   RR    RR   OO   OO   RR    RR\n");
printf("EEEE      RRRRRRRR   RRRRRRRR   OO   OO   RRRRRRRR\n");
printf("EE        RRRR       RRRR       OO   OO   RRRR\n");
printf("EE        RR  RR     RR  RR     OO   OO   RR  RR\n");
printf("EEEEEEE   RR    RR   RR    RR   OOOOOOO   RR    RR\n\n");

printf("Enter the CUT value as argument for the program! e.g. ./add2POTCAR_F 3.70\n"); 
printf("The amplitude can be entered as a second argument, default A = 1.0 e.g. ./add2POTCAR_F 3.70 0.8\n");
printf("\n\n"); exit(0);}

CUT = atof(argv[1]);

if(argc > 2) amplitude = atof(argv[2]);

else {
amplitude=1;
}

alpha=13.6058038;
beta=0.529177068;
pi=3.1415926548;

n=8;


potcar = fopen("POTCAR","r"); /* Arquivo ASCII, para leitura */
if(!potcar)
{
printf( "Error opening POTCAR file\n");
exit(0);
}

sprintf(str1, "%.3f",CUT);
strcat(str,str1);
sprintf(str1,"%.1f",amplitude);

if(amplitude != 1.0){
strcat(str,"A");
strcat(str,str1);}


potcarnew = fopen(str,"w"); /* Arquivo ASCII, para escrita */
if(!potcarnew)
{
printf("Error creating POTCARcut file%s\n",str);
exit(0);
}

vfile = fopen("VTOTAL1.ae","r"); /* Arquivo ASCII, para leitura */
if(!vfile)
{
printf( "Error opening VTOTAL1.ae file\n");
exit(0);
}

vfile05 = fopen("VTOTAL1.ae-05","r"); /* Arquivo ASCII, para leitura */
if(!vfile05)
{
printf( "Error opening VTOTAL1.ae-05 file\n");
exit(0);
}

inp = fopen("INP.ae-05","r"); /* Arquivo ASCII, para leitura */
if(!inp)
{
printf( "Error opening INP.ae-05 file\n");
exit(0);
}

do
ch = getc(vfile);              /*pula uma linha*/
while(ch!='\n');
do
ch = getc(vfile05);              /*pula uma linha*/
while(ch!='\n');

nupon=0;
fscanf(vfile,"%s",str);
while(strcmp(str,"Down")!=0){
fscanf(vfile,"%s",str);
nupon++;}                                     /*posiciona o ponteiro antes da palavra down*/


for(i=0;i<nupon;i++){ 
fscanf(vfile05,"%lg",&r[i]);
}

do
ch = getc(vfile05);              /*pula uma linha*/
while(ch!='\n');

for(i=0;i<2;i++){
do
ch = getc(vfile);              /*pula uma linha*/
while(ch!='\n');
do
ch = getc(vfile05);              /*pula uma linha*/
while(ch!='\n');
}

for(i=0;i<nupon;i++){
fscanf(vfile,"%lg",&patomo[i]);
fscanf(vfile05,"%lg",&pion[i]);
}

for(i=0;i<nupon;i++){
      VAR[i]=0;
      if(r[i]<CUT) VAR[i]=4*pi*alpha*pow(beta,3)*pow(1-pow(r[i],n)/pow(CUT,n),3)*(pion[i]-patomo[i])*amplitude;
}


while(strcmp(str,"PSCTR-controll")!=0)fscanf(potcar,"%s",str);

// pega a posição corrente de leitura no arquivo
        a = ftell(potcar);

rewind(potcar);

jumps=0;
b = 0;
while(b<=a){
ch = getc(potcar);
if(ch=='\n')jumps++;
b = ftell(potcar);}

jumps=jumps+2;




for(i=0;i<5;i++){
do
ch = getc(inp);              /*pula uma linha*/
while(ch!='\n');}

fscanf(inp,"%d",&valence);
fscanf(inp,"%d",&valence);

strcpy(str1,"");
for(i=0;i<valence;i++){
fscanf(inp,"%d",&core);
fscanf(inp,"%d",&orbital);
fscanf(inp,"%lg",&b);

if(orbital==0)sprintf(str,"%ds%.3lg ",core,b);
if(orbital==1)sprintf(str,"%dp%.3lg ",core,b);
if(orbital==2)sprintf(str,"%dd%.3lg ",core,b);
if(orbital==3)sprintf(str,"%df%.3lg ",core,b);

strcat(str1,str);
do
ch = getc(inp);              /*pula uma linha*/
while(ch!='\n');
}




rewind(potcar);
fscanf(potcar,"%200[^\n]",Linha); /*gets(Linha, 100, potcar);  // o 'fgets' lê até 99 caracteres ou até o '\n'*/

sprintf(str, "%.3f ",CUT); 
strcat(str,str1);
sprintf(str1,"%.1f",amplitude);
strcat(Linha," CUT=");
strcat(Linha,str);
strcat(Linha," Amp. = ");
strcat(Linha,str1);
strcat(Linha," LDA-1/2 "__DATE__);


fprintf(potcarnew,"%s",Linha);  
 

for(i=0;i<jumps;i++){
do
{ch = getc(potcar);
fprintf(potcarnew,"%c",ch);}           
while(ch!='\n');}

fscanf(potcar,"%lg",&kmax);
fprintf(potcarnew," %.18lg\n",kmax);

nuk=1000;
/*fscanf(potcar,"%s",str);
while(strcmp(str,"atomic")!=0){
fscanf(potcar,"%s",str);
nuk++;} 

rewind(potcar);
for(i=0;i<jumps+1;i++){
do
ch = getc(potcar);             
while(ch!='\n');} */

for(i=0;i<nuk;i++) fscanf(potcar,"%lg",&VVK[i]);

ca=0;

for(j=0;j<nuk;j++){
ca=ca+kmax/nuk;


inicial=0;
inicio=0;
final=0;
fourier=0;
iniciovez=0;
betaca=beta*ca;
   for(i=1;i<nupon;i++){
      if(r[i]<inicio){}
      else {
              if(iniciovez==0){
              fourier=fourier+(VAR[i]*sin(betaca*r[i])+inicial*sin(betaca*sin(betaca*inicio)))*(r[i]-inicio)/2;
              iniciovez=1; }
              else {
                     if(r[i]>CUT)fourier=fourier+(VAR[i]*sin(betaca*r[i])+final*sin(betaca*sin(betaca*CUT)))*(CUT-r[i])/2;
                     else {
                            fourier=fourier+(VAR[i]*sin(betaca*r[i])+VAR[i-1]*sin(betaca*r[i-1]))*(r[i]-r[i-1])/2; 

}}}
   
}



VVK[j]=VVK[j]+fourier/beta/ca;
if(j>0 && j%5==0)fprintf(potcarnew,"\n"); /*pula linha após 5 valores*/
fprintf(potcarnew," %15.8E",VVK[j]);
}

/*fprintf(potcarnew,"\n");*/


while((ch = getc(potcar)) != EOF)
fprintf(potcarnew,"%c",ch);           



fclose(potcar);
fclose(potcarnew);
fclose(vfile);
fclose(vfile05);
fclose(inp);   
}
