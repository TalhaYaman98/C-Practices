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
void IncreaseValue(uint16_t value);      

// Pointer ile parametre alan fonksiyon (call by reference)
void IncreaseValuePtr(uint16_t *value);  

/* --- Hesaplama Fonksiyonlarý --- */

float ConvertAdcToVoltage(uint16_t adcRaw); 

/* --- ISR / Donaným Uyumlu Fonksiyon --- */

void ClearUartFlag(volatile uint8_t *flag); 

/* --- Pointer ile Veri Doldurma Fonksiyonu --- */

// Buffer doldurma (DMA / UART RX senaryosu)
void FillBuffer(uint8_t *buf, uint16_t size, uint8_t value); 

/* --- Callback Fonksiyon Tanýmý --- */

// Fonksiyon pointer typedef (HAL callback mantýðý)
typedef void (*Callback_t)(void);

void RegisterCallback(Callback_t cb);   // Callback kaydetme
void ExecuteCallback(void);             // Callback çaðýrma

/* --- Inline Fonksiyon --- */

// Küçük fonksiyonlar için performans optimizasyonu
static inline uint16_t Add(uint16_t a, uint16_t b)
{
    return a + b;                      // Fonksiyon çaðrýsý overhead yok
}

/* ---------- extern Fonksiyon Bildirimleri ---------- */

#endif /* FUNCTIONS_H */


/* ==================== functions.c ======================== */

#include "functions.h"

/* --- Statik Global Deðiþken --- */

// Sadece bu dosya içinde eriþilebilir
static Callback_t userCallback = 0;     // Callback pointer


/* --- Fonksiyon Tanýmý: Led_On --- */

void Led_On(void)
{
    // GPIO_SetPin(LED_PORT, LED_PIN); 
}


/* --- Call by Value Örneði --- */

void IncreaseValue(uint16_t value)
{
    value++;                            // Kopya deðiþir
}


/* --- Call by Reference Örneði --- */

void IncreaseValuePtr(uint16_t *value)
{
    if (value != 0)                     // Null kontrolü
    {
        (*value)++;                    // Gerçek deðiþken deðiþir
    }
}


/* --- Fonksiyon Kullaným Senaryosu --- */

void Example_FunctionUsage(void)
{
    uint16_t counter = 10;

    IncreaseValue(counter);             // Deðiþmez
    IncreaseValuePtr(&counter);         // Artar
}


/* --- ADC Dönüþüm Fonksiyonu --- */

float ConvertAdcToVoltage(uint16_t adcRaw)
{
    const float VREF = 3.3f;            
    return (adcRaw * VREF) / 4095.0f;
}


/* --- ISR Uyumlu Fonksiyon --- */

void ClearUartFlag(volatile uint8_t *flag)
{
    if (flag != 0)
    {
        *flag = 0;
    }
}


/* --- Buffer Ýþleme Fonksiyonu --- */

// DMA sonrasý buffer temizleme / doldurma
void FillBuffer(uint8_t *buf, uint16_t size, uint8_t value)
{
    if (buf == 0) return;               // Güvenlik

    for (uint16_t i = 0; i < size; i++)
    {
        buf[i] = value;                 // Buffer doldurulur
    }
}


/* --- Callback Mekanizmasý --- */

// Callback kaydetme (ör: interrupt sonrasý çalýþtýrýlacak fonksiyon)
void RegisterCallback(Callback_t cb)
{
    userCallback = cb;                  // Fonksiyon adresi saklanýr
}

// Callback çalýþtýrma
void ExecuteCallback(void)
{
    if (userCallback != 0)              // Null kontrolü
    {
        userCallback();                // Fonksiyon pointer çaðrýlýr
    }
}


/* --- Static Fonksiyon (Dosya Ýçi Kullaným) --- */

// Sadece bu .c dosyasýnda eriþilebilir
static uint16_t Multiply(uint16_t a, uint16_t b)
{
    return a * b;
}


/* --- ISR Senaryosu Örneði --- */

// Simüle ISR handler
void UART_IRQHandler(void)
{
    static volatile uint8_t uartFlag = 1;  // ISR içinde deðiþen flag

    if (uartFlag)
    {
        ClearUartFlag(&uartFlag);      // Flag temizlenir
    }
}


/* --- Fonksiyon Pointer Kullanýmý --- */

void Led_Toggle(void)
{
    // GPIO toggle iþlemi
}

void FunctionPointerExample(void)
{
    void (*funcPtr)(void);             // Fonksiyon pointer

    funcPtr = Led_Toggle;              // Fonksiyon adresi atanýr
    funcPtr();                         // Fonksiyon çaðrýlýr
}
