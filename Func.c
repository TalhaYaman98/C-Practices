/* ---------- Fonksiyonlar ve Parametre Geçirme ---------- */

/* ==================== functions.h ======================== */

#ifndef FUNCTIONS_H                      // Header guard – çoklu include önler
#define FUNCTIONS_H

#include <stdint.h>                      // uint16_t, uint8_t gibi tipler için

/* --- LED Kontrol Fonksiyonlarý --- */

// Donanýma baðlý, parametresiz LED yakma fonksiyonu
void Led_On(void);                       // LED açma iþlemini yapar

/* --- Parametre Geçirme Örnekleri --- */

// Deðer ile parametre alan fonksiyon (call by value)
void IncreaseValue(uint16_t value);      // Gönderilen deðerin kopyasý üzerinde çalýþýr

// Pointer ile parametre alan fonksiyon (call by reference)
void IncreaseValuePtr(uint16_t *value);  // Gönderilen adres üzerinden asýl deðiþkeni deðiþtirir

/* --- Hesaplama Fonksiyonlarý --- */

// ADC ham deðerini voltaja çeviren fonksiyon
float ConvertAdcToVoltage(uint16_t adcRaw); // ADC sayýsal deðerini V cinsine çevirir

/* --- ISR / Donaným Uyumlu Fonksiyon --- */

// Interrupt içinde deðiþtirilen bayraklarý temizlemek için
void ClearUartFlag(volatile uint8_t *flag); // volatile zorunlu, donaným deðiþkeni

/* ---------- extern Fonksiyon Bildirimleri ---------- */
/* Header dosyasýnda tanýmlanan tüm fonksiyonlar varsayýlan olarak extern'dir */
/* Yani baþka .c dosyalarýndan çaðrýlabilirler */

#endif /* FUNCTIONS_H */


/* ==================== functions.c ======================== */

#include "functions.h"                  // Kendi header dosyamýz

/* --- Fonksiyon Tanýmý: Led_On --- */

// LED yakma iþlemini yapan fonksiyon
// Parametre almamasýnýn sebebi: LED pin bilgisi genelde makro veya global tanýmlýdýr
void Led_On(void)
{
    // GPIO_SetPin(LED_PORT, LED_PIN);    // HAL/LL ile GPIO set iþlemi
    // Bu fonksiyon donanýma doðrudan etki eder
}

/* --- Call by Value Örneði --- */

// Bu fonksiyona gönderilen "value", orijinal deðiþkenin kopyasýdýr
// Fonksiyon bittiðinde yapýlan deðiþiklikler kaybolur
void IncreaseValue(uint16_t value)
{
    value++;                            // Sadece yerel kopya artar
}

/* --- Call by Reference Örneði --- */

// Pointer kullanýldýðý için gerçek deðiþkenin adresi ile çalýþýlýr
// Bu yöntem gömülü sistemlerde sýklýkla tercih edilir
void IncreaseValuePtr(uint16_t *value)
{
    if (value != 0)                     // Null pointer kontrolü (güvenlik)
    {
        (*value)++;                    // Asýl deðiþken artýrýlýr
    }
}

/* --- Fonksiyon Kullaným Senaryosu --- */

// Bu fonksiyon, yukarýdaki iki yaklaþýmýn farkýný göstermek içindir
void Example_FunctionUsage(void)
{
    uint16_t counter = 10;              // Yerel deðiþken, stack üzerinde

    IncreaseValue(counter);             // counter deðiþmez (10)
    IncreaseValuePtr(&counter);          // counter = 11 olur
}

/* --- ADC Dönüþüm Fonksiyonu --- */

// ADC’den gelen ham sayýsal deðeri gerilim cinsine çevirir
// Donanýmdan baðýmsýz saf hesaplama fonksiyonudur
float ConvertAdcToVoltage(uint16_t adcRaw)
{
    const float VREF = 3.3f;            // Referans voltaj (Flash’ta tutulur)
    return (adcRaw * VREF) / 4095.0f;    // 12-bit ADC için dönüþüm formülü
}

/* --- ISR Uyumlu Fonksiyon --- */

// UART veya timer interrupt içinde kullanýlan bayrak temizleme fonksiyonu
// volatile kullanýmý: compiler optimizasyonunu engeller
void ClearUartFlag(volatile uint8_t *flag)
{
    if (flag != 0)                      // Güvenlik kontrolü
    {
        *flag = 0;                     // Bayrak temizlenir
    }
}

