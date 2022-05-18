#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

static char *encryptedValue; // sifrelenmis veriyi tutacagimiz degisken
static int key = 4;         // kac satir otelenecegini belirten degisken


void caesar(char* value, int num,int encdec)    // sezar algoritmasi fonksiyonu 1.yazi 2.oteleme sayisi 3.ilerigeri
{
    int i,len = strlen(value);  // cumledeki karakter sayisi ve for degiskeni
    char ch, *result = (char*)malloc(sizeof(char)*len); // dinamik degisken tanimi
 
    for (i=0;i<len;i++) // cumlede ki karakter sayisina kadar dongu
    {
        ch = value[i];  // value indisinin i lerini ch'a atip
        if(encdec==1){  // key degeri kadar ileri oteleme if-else blogu
            if(ch >= 'a' && ch <= 'z'){
                ch = ch + key;
                if(ch > 'z'){
                    ch = ch - 'z' + 'a' - 1;
                }
            }
            else if(ch >= 'A' && ch <= 'Z'){
                ch = ch + key;
                if(ch > 'Z'){
                    ch = ch - 'Z' + 'A' - 1;
                }
            }
        }
        else{           // key degeri kadar geriye oteleme if-else blogu
            if(ch >= 'a' && ch <= 'z'){
                ch = ch - key;
                if(ch < 'a'){
                    ch = ch + 'z' - 'a' + 1;
                }
            }
            else if(ch >= 'A' && ch <= 'Z'){
                ch = ch - key;
                if(ch < 'A'){
                    ch = ch + 'Z' - 'A' + 1;
                }
            }
        }
        result[i] = ch;     // oteleme islemi bittikten sonra degiskenin result dizisine gonderilmesi
    }
    result[i]='\0';         // result string sonu belirleme
    strcpy(value,result);   // result degerini value degerinin icine atar..strcpy fonksiyon ozelligi
}

void *caesarThread(void *vargp) // threadlerin icerisine gonderilmesi gereken sifreli mesaj olusturan fonksiyon
{
    char* val = (char*)vargp;   // sezarda kullanilan threadler icin kullanilan deger
    caesar(val,key,1);          // 0 ise -key kadar oteler 1 ise +key kadar oteler
    return NULL;
}

void substr(char* source,char* target,int start,int end){   // ramde ki degiskenleri depolayip sonradan kullanmak icin gereken fonksiyon
    int i,j=0;  //degisken tanimlamalari
    char* temp = (char*)malloc(sizeof(char)*(end-start+1)); // dinamik olarak olusturulan gecici degisken tanimlamasi
    for(i=start;i<end;i++){ // alinan dizi indislerinde ki degerlerin atandigi dongu
        temp[j++] = source[i];
    }
    temp[i]='\0';   // string temp ifadesinin sonunu belirleme
    strcpy(target,temp);    //strcpy fonksiyonu ile 2.degeri 1. degere yazdirma.
}

int main(void)      // Main fonksiyonu
{
        int     fd[2], len; //degisken olusturma
        pid_t   childpid;   //proses olusturma
        char    buffer[500];//buffer icin yer olusturma 500 byte lik yer..
        pipe(fd);           //pipe kullanma
        if((childpid = fork()) == -1)   //eger childpid forklandiktan sonra -1 degeri donduruyorsa hatali veya fork islemi olmamis demektir.
        {
            perror("fork error");
            exit(1);
        }
        if(childpid == 0) //child prosesin bolgesi
        {
            close(fd[0]);
            char string[] = "Ey Türk gençliği! Birinci vazifen; Türk istiklalini, Türk cumhuriyetini, ilelebet muhafaza ve müdafaa etmektir.Mevcudiyetinin ve istikbalinin yegâne temeli budur. Bu temel, senin en kıymetli hazinendir. İstikbalde dahi seni bu hazineden mahrum etmek isteyecek dâhilî ve haricî bedhahların olacaktır. Bir gün, istiklal ve cumhuriyeti müdafaa mecburiyetine düsersen, vazifeye atılmak için içinde bulunacağın vaziyetin imkân ve şeraitini düşünmeyeceksin.\n";
            write(fd[1], string, (strlen(string)+1));   // yukarida ki alinan string ifadenin karakter sayisi ve veri tipi ile fd dizisine yazdirma
            wait(NULL);                                 // ve proses beklemeye gecer.
        }
        else //parent prosesin bolgesi
        {
            close(fd[1]);   
            len = read(fd[0], buffer, sizeof(buffer));  // fd dosyasindan buffer bellegine,bufferda ki byte kadar yer acar.ve karakter sayisini belirler.unistd.h kutuphanesinden read fonksiyonu
            encryptedValue = mmap(NULL, (sizeof *encryptedValue)*len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);   //okunabilir yazilabilir ve paylasimli olarak proses adreslerine ve iceriklerine erisimi saglayan mmap fonksiyonu kullanimi..sys/mman.h kutuphanesinden mmap fonksiyonu
            int pieceLen = (len/4)+1;   // metinde ki karakter sayisinin threadler icin 4e bolumu
            
            char* piece1 =(char*)malloc(sizeof(char)*pieceLen); //bellekte piece1 degiskeni icin pieceLen (bizde120) degeri kadar yer acar
            char* piece2 =(char*)malloc(sizeof(char)*pieceLen); //bellekte piece2 degiskeni icin pieceLen (bizde120) degeri kadar yer acar
            char* piece3 =(char*)malloc(sizeof(char)*pieceLen); //bellekte piece3 degiskeni icin pieceLen (bizde120) degeri kadar yer acar
            char* piece4 =(char*)malloc(sizeof(char)*pieceLen); //bellekte piece4 degiskeni icin pieceLen (bizde120) degeri kadar yer acar
            
            substr(buffer,piece1,0,pieceLen);               // bufferdan 0 ile pieceLen arasinda ki degerleri alip piece 1 degiskenine koyar..orn: piece=40 olsun 0-40 arasi
            substr(buffer,piece2,pieceLen,pieceLen*2);      // bufferdan piece1 ile pieceLen*2 arasinda ki degerleri alip piece 2 degiskenine koyar..orn 40-80 arasi
            substr(buffer,piece3,pieceLen*2,pieceLen*3);    // bufferdan piece2 ile pieceLen*3 arasinda ki degerleri alip piece 3 degiskenine koyar..orn 80-120 arasi
            substr(buffer,piece4,pieceLen*3,pieceLen*4);    // bufferdan piece3 ile pieceLen*4 arasinda ki degerleri alip piece 4 degiskenine koyar..orn 120-160 arasi
            
            pthread_t thread_id;                                    // yeni thread icin id tanimlamasi
            pthread_create(&thread_id, NULL, caesarThread,piece1);  // yeni thread baslatir..piece1
            pthread_join(thread_id, NULL);                          // fork join kismi
            pthread_create(&thread_id, NULL, caesarThread,piece2);  // yeni thread baslatir..piece2
            pthread_join(thread_id, NULL);                          // fork join kismi
            pthread_create(&thread_id, NULL, caesarThread,piece3);  // yeni thread baslatir..piece3
            pthread_join(thread_id, NULL);                          // fork join kismi
            pthread_create(&thread_id, NULL, caesarThread,piece4);  // yeni thread baslatir..piece4
            pthread_join(thread_id, NULL);                          // fork join kismi
            
            strcat(encryptedValue,piece1);  // strcat ile pice1 degerini encryptedValue degerine kopyalar.
            strcat(encryptedValue,piece2);  // strcat ile pice2 degerini encryptedValue degerine kopyalar.
            strcat(encryptedValue,piece3);  // strcat ile pice3 degerini encryptedValue degerine kopyalar.
            strcat(encryptedValue,piece4);  // strcat ile pice4 degerini encryptedValue degerine kopyalar.
            
            childpid = fork();              // fork islemi
            
            if(childpid!=0 && childpid!=-1){// sonuc kismi
                wait(NULL);                 // tum prosesler gorevini yerine getirdikten sonra
                printf("\nCümlemizde ki harf sayisi:\n%d\n",len);               // cumlede ki harf sayisi cok onemli degil.
                printf("\nÇocuğun Şifrelediği veri:\n%s\n", encryptedValue);    // sifrelenmis verinin birlesmis halinin yazdirilmasi
                caesar(encryptedValue,key,0);                                   // sezar algoritmasi ile degerin tersine cozumlenmesi
                printf("\nÇözümlenmiş Veri:\n%s\n", encryptedValue);            // cozumlenmis verinin ekrana yazdirilmasi
                munmap(encryptedValue, sizeof *encryptedValue);                 // mmap işleminin tersini yaparak ilgili sürecin bellek alanındaki mapping'i kaldırır.
            }
        }
        return(0);
}