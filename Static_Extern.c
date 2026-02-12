/* ---------- static / extern / Deðiþken Ömrü ---------- */

#include <stdint.h>                     // Sabit geniþlikli veri tipleri

// Global Deðiþken (Varsayýlan Davranýþ)
// Program çalýþtýðý sürece RAM’de yaþar, tüm dosyalardan eriþilebilir
uint32_t systemTick;                    // SysTick veya genel zamanlayýcý sayacý

// static Global Deðiþken
// Sadece tanýmlandýðý .c dosyasý içinden eriþilebilir
static uint8_t errorFlag;               // Driver veya modüle özel hata bayraðý

// static Local Deðiþken
// Fonksiyon çaðrýlarý arasýnda deðerini korur
void Timer_Task(void)
{
    static uint16_t counter = 0;         // Ýlk çaðrýda init edilir, RAM’de kalýcýdýr
    counter++;                           // Her çaðrýda artar
}

// Normal Local Deðiþken
// Stack üzerinde oluþur, fonksiyon bitince yok edilir
void Read_ADC(void)
{
    uint16_t adcValue;                  // Sadece fonksiyon içinde geçerli
    adcValue = 2048;                    // Geçici ADC okuma deðeri
}

// extern Kullanýmý (Dosyalar Arasý Eriþim)
// Deðiþkenin gerçek tanýmý baþka bir .c dosyasýndadýr
extern uint8_t uartRxFlag;              // ISR tarafýndan set edilen UART bayraðý

void UART_Task(void)
{
    if (uartRxFlag)                     // UART verisi geldi mi kontrolü
    {
        uartRxFlag = 0;                 // Bayrak temizlenir
    }
}

// static ve extern Karþýlaþtýrmasý
// static tanýmlanan deðiþken extern ile eriþilemez
// static uint8_t state;                // ? Dosya dýþýna kapalý
// uint8_t state;                       // ? extern ile eriþilebilir

// static Fonksiyon Kullanýmý
// Sadece bu dosya içinden çaðrýlabilir
static void Internal_Reset(void)
{
    errorFlag = 0;                      // Modül içi reset iþlemi
}

// STM32 Driver Tarzý Kullaným
// Driver’a ait state bilgisi dýþarýya kapatýlýr
static uint8_t driverState;             // Driver çalýþma durumu

void Driver_Init(void)
{
    driverState = 1;                    // Driver aktif hale getirilir
}

uint8_t Driver_GetState(void)
{
    return driverState;                 // State bilgisi kontrollü þekilde döndürülür
}

