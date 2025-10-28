#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SATIR_UZUNLUK 100

struct kitap {
    long long isbnNumarasi;
    char kitapAd[50];
    char yazarAd[50];
    char tur[50];
    int sayfaSayisi;
    struct kitap *next;
    struct kitap *prev;
};

struct kitap *head = NULL;
struct kitap *kuyrukOn = NULL;
struct kitap *kuyrukArka = NULL;

void listeyiYazdir();
void kuyruktanBagliListeyeTasi();
void dosyadanOkuVeKuyrugaEkle();
void kuyrugaEkle(struct kitap *yeniKitap);
void listedeAra();
void isbnNumarayaGoreSil();
void yazaraGoreSirala();
void yerDegistir(struct kitap *a, struct kitap *b);
void kutuphaneBilgisiHesapla();
void siraliKitapEkle(struct kitap *yeniKitap);
void tumVeriYapilariniTemizle();
struct kitap *kitapOlustur(long long isbnNumarasi, const char *kitapAd, const char *yazarAd, const char *tur,
                           int sayfaSayisi);

struct kitap *kuyruktanCikar();

int main() {
    while (1) {
        printf("\n--- Kütüphane Yönetim Sistemi ---\n");
        printf("1- Listeyi Dosyadan Oku ve Oluştur\n");
        printf("2- Listeden ISBN Numarasına Göre Kitap Sil\n");
        printf("3- Listede Türüne Göre Kitap Ara\n");
        printf("4- Listeyi Yazar Adına Göre Alfabetik Sırala\n");
        printf("5- Kütüphane Bilgilerini Göster\n");
        printf("6- Çıkış\n");
        printf("Seçiminiz: ");

        int girdi;
        scanf("%d", &girdi);

        switch (girdi) {
            case 1:
                tumVeriYapilariniTemizle();
                dosyadanOkuVeKuyrugaEkle();
                kuyruktanBagliListeyeTasi();
                listeyiYazdir();
                break;
            case 2:
                isbnNumarayaGoreSil();
                break;
            case 3:
                listedeAra();
                break;
            case 4:
                yazaraGoreSirala();
                listeyiYazdir();
                break;
            case 5:
                kutuphaneBilgisiHesapla();
                break;
            case 6:
                tumVeriYapilariniTemizle();
                printf("Programdan çıkılıyor...\n");
                return 0;
            default:
                printf("Yanlış bir değer girdiniz. Lütfen 1-6 arası seçin.\n");
        }
    }
}

void tumVeriYapilariniTemizle() {
    if (head != NULL) {
        struct kitap *temp = head;
        head->prev->next = NULL;

        while (temp != NULL) {
            struct kitap *next = temp->next;
            free(temp);
            temp = next;
        }
        head = NULL;
    }

    if (kuyrukOn != NULL) {
        struct kitap *current = kuyrukOn;
        while (current != NULL) {
            struct kitap *next = current->next;
            free(current);
            current = next;
        }
        kuyrukOn = NULL;
        kuyrukArka = NULL;
    }
}

void listeyiYazdir() {
    if (head == NULL) {
        printf("Liste şu anda boş.\n");
        return;
    }

    struct kitap *temp = head;

    printf("\n--- Kütüphane Güncel Listesi ---\n");

    do {
        printf("ISBN: %-13lld | Kitap: %-30s | Yazar: %-25s | Tür: %-10s | Sayfa: %d\n",
               temp->isbnNumarasi,
               temp->kitapAd,
               temp->yazarAd,
               temp->tur,
               temp->sayfaSayisi);

        temp = temp->next;
    } while (temp != head);

    printf("----------------------------------\n");
}

void kuyrugaEkle(struct kitap *yeniKitap) {
    if (yeniKitap == NULL) return;

    yeniKitap->next = NULL;
    yeniKitap->prev = NULL;

    if (kuyrukArka == NULL) {
        kuyrukOn = yeniKitap;
        kuyrukArka = yeniKitap;
    } else {
        kuyrukArka->next = yeniKitap;
        kuyrukArka = yeniKitap;
    }
}

struct kitap *kuyruktanCikar() {
    if (kuyrukOn == NULL) {
        return NULL;
    }

    struct kitap *temp = kuyrukOn;
    kuyrukOn = kuyrukOn->next;

    if (kuyrukOn == NULL) {
        kuyrukArka = NULL;
    }
    return temp;
}

struct kitap *kitapOlustur(long long isbnNumarasi, const char *kitapAd, const char *yazarAd, const char *tur,
                           int sayfaSayisi) {
    struct kitap *yeniKitap = (struct kitap *) malloc(sizeof(struct kitap));

    yeniKitap->isbnNumarasi = isbnNumarasi;
    strcpy(yeniKitap->kitapAd, kitapAd);
    strcpy(yeniKitap->yazarAd, yazarAd);
    strcpy(yeniKitap->tur, tur);
    yeniKitap->sayfaSayisi = sayfaSayisi;

    yeniKitap->next = NULL;
    yeniKitap->prev = NULL;

    return yeniKitap;
}

void dosyadanOkuVeKuyrugaEkle() {
    FILE *dosya = fopen("kitaplar.txt", "r");
    if (dosya == NULL) {
        printf("Hata: kitaplar.txt dosyası açılamadı. Programın bulunduğu dizinde olduğundan emin olun.\n");
        exit(1);
    }

    char satir[MAX_SATIR_UZUNLUK];
    fgets(satir, MAX_SATIR_UZUNLUK, dosya);

    while (fgets(satir, MAX_SATIR_UZUNLUK, dosya) != NULL) {
        long long int isbn;
        char kitapAdi[50];
        char yazarAdi[50];
        char turu[50];
        int sayfaSayi;

        char *karakter;

        karakter = strtok(satir, "|");
        if (karakter == NULL) continue;
        isbn = atoll(karakter);

        karakter = strtok(NULL, "|");
        if (karakter == NULL) continue;
        strcpy(kitapAdi, karakter);

        karakter = strtok(NULL, "|");
        if (karakter == NULL) continue;
        strcpy(yazarAdi, karakter);

        karakter = strtok(NULL, "|");
        if (karakter == NULL) continue;
        strcpy(turu, karakter);

        karakter = strtok(NULL, "|\n");
        if (karakter == NULL) continue;
        sayfaSayi = atoi(karakter);

        kuyrugaEkle(kitapOlustur(isbn, kitapAdi, yazarAdi, turu, sayfaSayi));
    }

    fclose(dosya);
    printf("\nDosyadan okuma tamamlandı. Kitaplar kuyruğa eklendi.\n");
}

void siraliKitapEkle(struct kitap *yeniKitap) {
    if (head == NULL) {
        head = yeniKitap;
        yeniKitap->next = head;
        yeniKitap->prev = head;
        return;
    }

    if (yeniKitap->isbnNumarasi < head->isbnNumarasi) {
        struct kitap *last = head->prev;

        yeniKitap->next = head;
        yeniKitap->prev = last;

        last->next = yeniKitap;
        head->prev = yeniKitap;

        head = yeniKitap;
        return;
    }

    struct kitap *current = head;

    while (current->next != head && current->next->isbnNumarasi < yeniKitap->isbnNumarasi) {
        current = current->next;
    }

    yeniKitap->next = current->next;
    yeniKitap->prev = current;
    current->next->prev = yeniKitap;
    current->next = yeniKitap;
}

void kuyruktanBagliListeyeTasi() {
    printf("Kuyruktan listeye ISBN sırasına göre taşınıyor...\n");

    struct kitap *tasinacakKitap;
    while ((tasinacakKitap = kuyruktanCikar()) != NULL) {
        siraliKitapEkle(tasinacakKitap);
    }
    printf("Tüm kitaplar listeye eklendi.\n");
}

void isbnNumarayaGoreSil() {
    if (head == NULL) {
        printf("Liste zaten boş. Silinecek kitap yok.\n");
        return;
    }

    printf("Silinecek kitabın ISBN numarasını giriniz: ");
    long long silincekIsbn;
    scanf("%lld", &silincekIsbn);

    struct kitap *temp = head;
    int bulundu = 0;

    do {
        if (temp->isbnNumarasi == silincekIsbn) {
            bulundu = 1;
            break;
        }
        temp = temp->next;
    } while (temp != head);

    if (!bulundu) {
        printf("Kitap bulunamadı.\n"); //
        return;
    }

    char silinenKitapAdi[50];
    char silinenYazarAdi[50];
    strcpy(silinenKitapAdi, temp->kitapAd);
    strcpy(silinenYazarAdi, temp->yazarAd);

    if (temp->next == temp && temp == head) {
        head = NULL;
    } else if (temp == head) {
        head = temp->next;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    } else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    printf("%s (%s) listeden silinmiştir.\n", silinenKitapAdi, silinenYazarAdi); //

    free(temp);
    listeyiYazdir();
}

void listedeAra() {
    if (head == NULL) {
        printf("Liste boş. Aranacak kitap yok.\n");
        return;
    }

    char kitapTur[50];
    printf("Tür giriniz: ");
    scanf("%s", kitapTur);

    struct kitap *temp = head;
    int bulundu = 0;

    do {
        if (strcmp(temp->tur, kitapTur) == 0) {
            bulundu = 1;

            printf("%s - %d sayfa ", temp->kitapAd, temp->sayfaSayisi);

            if (temp->sayfaSayisi <= 200) {
                printf("(Kısa)\n");
            } else if (temp->sayfaSayisi <= 400) {
                printf("(Orta uzunlukta)\n");
            } else {
                printf("(Uzun)\n");
            }
        }
        temp = temp->next;
    } while (temp != head);

    if (bulundu == 0) {
        printf("Bu türe ait kitap bulunamadı.\n");
    }
}

void yazaraGoreSirala() {
    if (head == NULL || head->next == head) {
        printf("Listede sıralanacak yeterli eleman yok.\n");
        return;
    }

    int degistirildi;
    struct kitap *temp;

    do {
        degistirildi = 0;
        temp = head;

        while (temp->next != head) {
            int yazarKarsilastir = strcmp(temp->yazarAd, temp->next->yazarAd);
            int takasGerekli = 0;

            if (yazarKarsilastir > 0) {
                takasGerekli = 1;
            } else if (yazarKarsilastir == 0) {
                int kitapKarsilastir = strcmp(temp->kitapAd, temp->next->kitapAd);
                if (kitapKarsilastir > 0) {
                    takasGerekli = 1;
                }
            }

            if (takasGerekli) {
                yerDegistir(temp, temp->next);
                degistirildi = 1;
            }
            temp = temp->next;
        }
    } while (degistirildi);

    printf("\n--- Yazar Adına Göre Sıralanmış Liste ---\n");
    temp = head;
    do {
        printf("%s - %s - %d s.\n",
               temp->kitapAd,
               temp->yazarAd,
               temp->sayfaSayisi);
        temp = temp->next;
    } while (temp != head);

    printf("----------------------------------\n");
}

void yerDegistir(struct kitap *a, struct kitap *b) {
    long long tempIsbn = a->isbnNumarasi;
    char tempKitap[50], tempYazar[50], tempTur[50];
    int tempSayfa = a->sayfaSayisi;

    strcpy(tempKitap, a->kitapAd);
    strcpy(tempYazar, a->yazarAd);
    strcpy(tempTur, a->tur);

    a->isbnNumarasi = b->isbnNumarasi;
    strcpy(a->kitapAd, b->kitapAd);
    strcpy(a->yazarAd, b->yazarAd);
    strcpy(a->tur, b->tur);
    a->sayfaSayisi = b->sayfaSayisi;

    b->isbnNumarasi = tempIsbn;
    strcpy(b->kitapAd, tempKitap);
    strcpy(b->yazarAd, tempYazar);
    strcpy(b->tur, tempTur);
    b->sayfaSayisi = tempSayfa;
}

void kutuphaneBilgisiHesapla() {
    if (head == NULL) {
        printf("Liste boş. İstatistik hesaplanamıyor.\n");
        return;
    }

    int topkitap = 0;
    int topSayfa = 0;
    float ortSayfa = 0;
    struct kitap *temp = head;

    do {
        topkitap++;
        topSayfa += temp->sayfaSayisi;
        temp = temp->next;
    } while (temp != head);

    ortSayfa = (float) topSayfa / (float) topkitap;

    printf("\n--- Kütüphane İstatistikleri ---\n");
    printf("Toplam kitap sayısı: %d\n", topkitap);
    printf("Toplam sayfa sayısı: %d\n", topSayfa);
    printf("Ortalama sayfa sayısı: %.2f\n", ortSayfa);
    printf("----------------------------------\n");
}
