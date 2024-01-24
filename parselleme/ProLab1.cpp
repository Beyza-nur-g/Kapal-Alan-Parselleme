#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <math.h>

int xKoordinat[100];
int yKoordinat[100];
int koseSayisi=0;
int boyut=10;

int icindeMi(int x, int y) {
    int i, j, c = 0;
    for (i = 0, j = koseSayisi - 1; i < koseSayisi; j = i++) {
        if ((((yKoordinat[i] > y) != (yKoordinat[j] > y)) &&
            (x < (xKoordinat[j] - xKoordinat[i]) * (y - yKoordinat[i]) / (yKoordinat[j] - yKoordinat[i]) + xKoordinat[i]))) {
            c = !c;
        }
    }
    return c;
}
int renkliKareSay(int anaRenk){
	int sayac = 0;
	for (int x = 1; x < getmaxx(); x += boyut) {
     for (int y = 1; y < getmaxy(); y += boyut) {
        
            int renk = getpixel(x, y);
            if ( renk==anaRenk  ){
                sayac++;
            }
        }
    }
    return sayac;
}
int kareYerlestirme(int boyut,int renk) {
   int kareSayisi=0;
    for (int y = 0; y < getmaxy(); y += boyut) {
        for (int x = 0; x < getmaxx(); x += boyut) {
            int icinde = icindeMi(x + boyut / 2, y + boyut / 2);
            if (icinde) {
                setfillstyle(SOLID_FILL, renk);	
                bar(x, y, x + boyut, y + boyut);   
                kareSayisi++;
		   }
	       rectangle(x, y, x + boyut, y + boyut);	
        }
    } 
    return kareSayisi;
}
void boyama(int x, int y, int size, int color) {
    setfillstyle(SOLID_FILL, color);
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            rectangle(i, j, i + 1, j + 1); // Her bir piksel için bir dikdörtgen çiz
            floodfill(i, j, getpixel(i, j)); // Dikdörtgeni doldur
        }
    }
}
void cokgenCiz(){
    setcolor(YELLOW);
    int var=0;
    for (int i = 0; i < koseSayisi; i++) {
        int j = (i + 1) % koseSayisi; 
        for (int k = i + 1; k < koseSayisi; k++) {
            if (xKoordinat[k] == xKoordinat[i] && yKoordinat[k] == yKoordinat[i]) {
                 var=k;
                  break;
                }
        }
        if (i!=var) {
      line(xKoordinat[i], yKoordinat[i], xKoordinat[j], yKoordinat[j]);
        }
    }
    setcolor(WHITE);
}
int birimKontrol(int alan){
    for (int x = 0; x < getmaxx(); x += boyut) {
        for (int y = 0; y < getmaxy(); y += boyut) {
            if (icindeMi(x + boyut / 2, y + boyut / 2)) {
                boyama(x, y, boyut, YELLOW);
                alan++;
                }
         setfillstyle(SOLID_FILL, YELLOW);
         rectangle(x, y, x + boyut, y + boyut);
        }
    }
     printf("Alan : %d\n",alan);
     return alan ;
 }
int urlOkuma(const char *url, int satirNumarasi,int gecici) {
    char komut[200];
    snprintf(komut, sizeof(komut), "curl %s > prolab1.txt", url);
    system(komut);
    FILE *dosya = fopen("prolab1.txt", "r");
    if (dosya == NULL) {
        perror("Dosya açma hatasi");
        exit(1);
    }

    char satir[100];
    char *ayrac;

    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        memset(xKoordinat, 0, sizeof(xKoordinat));
        memset(yKoordinat, 0, sizeof(yKoordinat));
        int sayac = 0;
        ayrac = strtok(satir, "(),");
        while (ayrac !=NULL) {
            ayrac = strtok(NULL, "(,");
            xKoordinat[sayac] = atof(ayrac)*10;
            ayrac = strtok(NULL, ")");
            yKoordinat[sayac] = atof(ayrac)*10;
            sayac++;
        }
        koseSayisi = sayac - 1;
        if (satirNumarasi == gecici) {
            satirNumarasi++;
            return satirNumarasi;
        }
        gecici++;
    }
   // fclose(file);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");
    int satirNumarasi=0;  
    int platform=0;
    int sondaj=0;
    int renk[4]={RED,BLUE,GREEN,YELLOW};
        
    const char *url =  "http://zplusorg.com/prolab.txt";
    while (1){
         printf("Kacinci satiri okumak istiyorsunuz?");
         scanf("%d", &satirNumarasi);
        int gecici = 1;
        urlOkuma(url, satirNumarasi,gecici);
        cleardevice();
        int alan=0;
         alan=birimKontrol(alan);
        
         cokgenCiz();
         
         printf("Platform maliyetini giriniz\n");
         scanf("%d", &platform);
        while(1){
           printf("Sondaj maliyetini giriniz(1-10)\n");
           scanf("%d", &sondaj);
             if(sondaj>0 && sondaj<11){
	        	break;	
		     }else{
			 printf("Lütfen 1 ile 10 arasinda bir deger giriniz");
	       	 }
	    }
        printf("Rezerv deger miktari : %d\n",alan*10);
        
		for(int i=10 , j=0 ; i<41 ,j<3 ; i=i*2 , j++){ 	
        kareYerlestirme(i,renk[j]);
        } 
        int pSayisi=renkliKareSay(GREEN)/16+renkliKareSay(BLUE)/4+renkliKareSay(RED);
        int sSayisi=renkliKareSay(GREEN)+renkliKareSay(BLUE)+renkliKareSay(RED);
		 printf("Platform ve sondaj sayisi : %d\n", pSayisi);
	    int pMaliyet=pSayisi*platform;
		 printf("Platform maliyeti : %d\n", pMaliyet);
	    int sMaliyet=sSayisi*sondaj;
	     printf("Sondaj maliyeti: %d\n", sMaliyet);
	     printf("Toplam maliyet : %d\n",sMaliyet+pMaliyet);
         printf("Toplam kar : %d\n",alan*10 - sMaliyet - pMaliyet);
		cokgenCiz();
    }
    getch();
    closegraph();

    return 0;
}
