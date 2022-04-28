#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct zaznam{//vytvorenie struktury
        char meno[52];
        char druh[32];
        int vyska;
        float vaha;
        long int datum_nar;
        long int datum_krm;
        char oset[52];
        struct zaznam *dalsi;
    }ZAZNAM;
int pocet_zaznamov(FILE **subor){
    int pocet_z=0;
    int pocet_r=1;
    char riadok='c';
    int i=0;
    fseek(*subor,0,SEEK_SET);
    while (riadok!=EOF){
            riadok=fgetc(*subor);
            if (riadok=='\n'){ //hladanie novych riadkov, musi zacinat od 1, pretoze vzdy je aspon 1 riadok
                pocet_r++;
            }
        }
    while (i<=pocet_r){
        if ((i%8)==1){ //kazdy siedmy riadok sa berie ako kontrolny na pripocitanie pacienta
            pocet_z++;
        }
        i++;
    }
    return pocet_z;
}
void n(FILE **subor,int *kontrol, ZAZNAM **zoznam){
    if (*kontrol!=0){
        ZAZNAM *p=(*zoznam);
        while ((*zoznam)!=NULL){//ak N uz bolo zavolane, najprv sa musia dealokovat postupne vsetci clenovia zoznamu
            p=(*zoznam);
            *zoznam=((*zoznam)->dalsi);
            free(p);
        }
    }
    *kontrol=1;
    *subor=fopen("zvierata.txt","r"); //otvorenie suboru a ak sa neda otvorit, tak ukonci program
    if (*subor==NULL){ 
        printf("Zaznamy neboli nacitane");
        exit(0);
    }
    int pocet_z=pocet_zaznamov(&(*subor));//zistenie poctu zaznamov v subore
    char buf[52];
    rewind(*subor);
    for (int i=1;i<=pocet_z;i++){
        ZAZNAM *novy_zaznam=(ZAZNAM *)malloc(sizeof(ZAZNAM));//alokujem si pamat na novy prvok
        ZAZNAM *posledny= *zoznam; //kopia zoznamu
        int y=1;
        char x;
        fgets(buf,52,*subor); //nacitanie vsetkych clenov struktury
        fgets(buf,52,*subor);
        buf[strcspn(buf, "\n")]=0;
        strcpy((novy_zaznam->meno),buf);   
        fgets(buf,32,*subor);
        buf[strcspn(buf, "\n")]=0;
        strcpy((novy_zaznam->druh),buf);   
        fscanf(*subor,"%d",&(novy_zaznam->vyska));
        fgets(buf,52,*subor);
        fscanf(*subor,"%g",&(novy_zaznam->vaha));
        fgets(buf,52,*subor);
        fscanf(*subor,"%ld",&(novy_zaznam->datum_nar));
        fgets(buf,52,*subor);
        fscanf(*subor,"%ld",&(novy_zaznam->datum_krm));
        fgets(buf,52,*subor);
        fgets(buf,52,*subor);
        buf[strcspn(buf, "\n")] = 0;
        strcpy((novy_zaznam->oset),buf);
        novy_zaznam->dalsi=NULL; //ukoncim ju NULL
        if (*zoznam==NULL){//ak sme v prvej iteracii, tak sa zoznam len bude rovnat novemu prvku
            *zoznam=novy_zaznam;
            continue;
        }
        while ((posledny->dalsi)!=NULL){//v dalsich iteraciach sa najde a prida na koniec 
            posledny=posledny->dalsi;
        }
        posledny->dalsi=novy_zaznam;
    }
    printf("Nacitalo sa %d zaznamov\n",pocet_z);
}
void v(ZAZNAM **zoznam){
    ZAZNAM *zaz= *zoznam;
    int i=1;
    while (zaz!=NULL){//vypisujeme jednotlive cleny struktury az po koniec
        printf("%d.\n",i);
        printf("Meno: %s\n",zaz->meno);
        printf("Druh: %s\n",zaz->druh);
        printf("Vyska: %d\n",zaz->vyska);
        printf("Vaha: %g\n",zaz->vaha);
        printf("Datum narodenia: %ld\n",zaz->datum_nar);
        printf("Datum krmenia: %ld\n",zaz->datum_krm);
        printf("Meno osetrovatela: %s\n",zaz->oset);
        zaz=zaz->dalsi;
        i++;
    }
}
void p(FILE **subor,ZAZNAM **zoznam){
    int miesto;
    scanf("%d",&miesto);
    int pocet_z=pocet_zaznamov(&(*subor));
    if (miesto>pocet_z){//ak zada viac ako je zaznamov, prida na koniec
        miesto=(pocet_z+1);
    }
    else if (miesto<1){//ak zada mensie ako 1, da sa na zaciatok
        miesto=1;
    }
    char buf[52];
    ZAZNAM *novy_zaznam=(ZAZNAM *)malloc (sizeof(ZAZNAM)); //alokujeme nove miesto pre nas novy prvok
    fgets(buf,52,stdin);
    fgets(buf,52,stdin);
    buf[strcspn(buf, "\n")] = 0;
    strcpy(novy_zaznam->meno,buf);//nacitanie novych prvkov novej struktury
    fgets(buf,52,stdin);
    buf[strcspn(buf, "\n")] = 0;
    strcpy(novy_zaznam->druh,buf);
    scanf("%d",&(novy_zaznam->vyska));
    scanf("%g",&(novy_zaznam->vaha));
    scanf("%ld",&(novy_zaznam->datum_nar));
    scanf("%ld",&(novy_zaznam->datum_krm));
    fgets(buf,52,stdin);
    fgets(buf,52,stdin);
    buf[strcspn(buf, "\n")] = 0;
    strcpy(novy_zaznam->oset,buf);
    int i=0;
    ZAZNAM *po_zaznam=*zoznam;//najdeme si prvok, ktory bude nasledovat po nasom novom
    for (i;i<(miesto-1);i++){
        po_zaznam=po_zaznam->dalsi;
    }
    i=0;
    ZAZNAM *pred_zaznam=*zoznam; //najdeme si prvok, ktory bude predchadzat nasmu novemu
    for (i;i<(miesto-2);i++){
        pred_zaznam=pred_zaznam->dalsi;
    }
    novy_zaznam->dalsi=po_zaznam;//staci uz len nastavit pointre
    if (i!=0){
        pred_zaznam->dalsi=novy_zaznam;
    }
    else{//ak ale davame na prve miesto, tak staci nastavit len nasledujuci pointer a do celeho zoznamu pridat 
        (*zoznam)=novy_zaznam;
    }
}
void z(FILE **subor, ZAZNAM **zoznam){
    int pocet_z=pocet_zaznamov(&(*subor));//najdenie poctu zaznamov
    ZAZNAM *zaz= *zoznam;//vytvorenie potrebnych kopii
    ZAZNAM *akt=*zoznam;
    char meno_akt[52];
    char vstup[52];
    fgets(vstup,52,stdin);//nacitanie z klavesnice
    fgets(vstup,52,stdin);
    vstup[strcspn(vstup, "\n")]=0;//odstranenie ENTERU zo vstupu
    strlwr(vstup); //normalizovanie velkosti pismen
    int kontrol=0;
    while(zaz!=NULL){
        strcpy(meno_akt,zaz->meno);//vytvorenie kopie mena, aby sa nepropagovala zmena do hlavneho zoznamu
        strlwr(meno_akt);//normalizovanie velkosti pismen
        if (strcmp(meno_akt,vstup)!=0){ //nasli sme spravny prvok na vymazanie?
            zaz=zaz->dalsi;//ak nie, skus dalsi prvok
            if(kontrol!=0){
                akt=akt->dalsi; //o 1 iteraciu omeskany aktualny prvok
            }
            kontrol=1;
        }
        else{
            if (kontrol==0){//ak vymazavame prvy prvok, staci len posunut cely zoznam
                (*zoznam)=zaz->dalsi;
            }
            else{
                akt->dalsi=zaz->dalsi;//ak vymazeme iny prvok, tak ho vynechame v zozname a aktualizujeme pointre
            }
            printf("Zviera s menom %s bolo vymazane.\n",zaz->meno);
            break;
        }
    }
}
void h(ZAZNAM **zoznam){
    ZAZNAM *zaz= *zoznam;
    long int vstup;
    scanf("%ld",&vstup); //nacitanie vstupu
    int i=0;
    while (zaz!=NULL){
        if(zaz->datum_krm<vstup){//vyhladavanie vsetkych prvkov zoznamu, ktore vyhovuju podmienke a ich vypisanie
            i++;
            printf("%d.\n",i);
            printf("Meno: %s\n",zaz->meno);
            printf("Druh: %s\n",zaz->druh);
            printf("Vyska: %d\n",zaz->vyska);
            printf("Vaha: %g\n",zaz->vaha);
            printf("Datum narodenia: %ld\n",zaz->datum_nar);
            printf("Datum krmenia: %ld\n",zaz->datum_krm);
            printf("Meno osetrovatela: %s\n",zaz->oset);
            zaz=zaz->dalsi;
        }
        else{
            zaz=zaz->dalsi;
        }
    }
    if(i==0){
        printf("Vsetky zvierata boli k datumu %d nakrmene.\n",vstup);
    }
}
void a(ZAZNAM **zoznam){
    ZAZNAM *zaz= *zoznam;
    char vstupm[52];
    long int vstupd;
    fgets(vstupm,52,stdin);//nacitanie vstupu
    fgets(vstupm,52,stdin);
    vstupm[strcspn(vstupm, "\n")] = 0;
    scanf("%ld",&vstupd);
    while (zaz!=NULL){
        if(strcmp(vstupm,zaz->meno)==0){//najdeme nas zvoleny prvok zoznamu
            zaz->datum_krm=vstupd;//zmenime jeho hodnotu datumu krmenia
            printf("Zviera s menom %s bolo naposledy nakrmene dna %d\n",vstupm,vstupd);
            break;
        }
        else{
            zaz=zaz->dalsi;
        }
    }
}
int main(void){
    ZAZNAM *zoznam_zac=NULL;
    FILE *fr;
    int control=0;
    int x=0;
    char menu;
    while (x==0){
        menu=getchar();//uzivatelske menu
        switch (menu){
            case 'n':
                n(&fr,&control,&zoznam_zac);//volanie jednotlivych funkcii
                break;
            case 'v':
                v(&zoznam_zac);
                break;
            case 'p':
                p(&fr,&zoznam_zac);
                break;
            case 'z':
                z(&fr,&zoznam_zac);
                break;
            case 'h':
                h(&zoznam_zac);
                break;
            case 'a':
                a(&zoznam_zac);
                break;    
            case 'k':
                x=1;
                if (control!=0){
                    ZAZNAM *p=zoznam_zac;
                    while (zoznam_zac!=NULL){//dealokovanie celeho zoznamu a vypnutie programu
                        p=zoznam_zac;
                        zoznam_zac=zoznam_zac->dalsi;
                        free(p);
                    }
                    fclose(fr);
                }
                break;
        }
    }
    return 0;
}