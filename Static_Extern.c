/* ---------- static / extern / Deðiþken Ömrü ---------- */

#include <stdint.h>                     // Sabit geniþlikli veri tipleri


/* --- Global Deðiþken (Varsayýlan Davranýþ) --- */

// Program çalýþtýðý sürece RAM’de yaþar (.data / .bss segment)
// Tüm dosyalardan extern ile eriþilebilir
uint32_t systemTick;                    // SysTick veya genel zamanlayýcý sayacý


/* --- static Global Deðiþken --- */

// Sadece tanýmlandýðý .c dosyasý içinden eriþilebilir (internal linkage)
static uint8_t errorFlag;               // Modüle özel hata bayraðý


/* --- static Local Deðiþken --- */

// Fonksiyon çaðrýlarý arasýnda deðerini korur (RAM’de saklanýr)
void Timer_Task(void)
{
    static uint16_t counter = 0;        // Ýlk çaðrýda init edilir (.bss/.data)
    counter++;                          // Her çaðrýda artar (state tutma)
}


/* --- Normal Local Deðiþken --- */

// Stack üzerinde oluþur, fonksiyon bitince yok edilir
void Read_ADC(void)
{
    uint16_t adcValue;                  // Stack deðiþkeni (geçici)
    adcValue = 2048;                    
}


/* --- extern Kullanýmý (Dosyalar Arasý Eriþim) --- */

// Deðiþkenin gerçek tanýmý baþka bir .c dosyasýndadýr
extern uint8_t uartRxFlag;              // ISR tarafýndan set edilen UART bayraðý

void UART_Task(void)
{
    if (uartRxFlag)                     
    {
        uartRxFlag = 0;                 // Bayrak temizlenir
    }
}


/* --- extern Deðiþken Tanýmý (Baþka Dosyada) --- */

// Örn: uart.c içinde
// uint8_t uartRxFlag = 0;               // Gerçek taným (memory allocate edilir)


/* --- static ve extern Karþýlaþtýrmasý --- */

// static › internal linkage (dosya dýþýna kapalý)
// extern › external linkage (dosyalar arasý eriþim)

// static uint8_t state;                // dýþarýdan eriþilemez
// uint8_t state;                       // extern ile eriþilebilir


/* --- static Fonksiyon Kullanýmý --- */

// Sadece bu dosya içinden çaðrýlabilir (encapsulation)
static void Internal_Reset(void)
{
    errorFlag = 0;                      // Modül içi reset iþlemi
}


/* --- STM32 Driver Tarzý Kullaným --- */

// Driver’a ait state bilgisi dýþarýya kapatýlýr
static uint8_t driverState;             

void Driver_Init(void)
{
    driverState = 1;                    
}

uint8_t Driver_GetState(void)
{
    return driverState;                 // Kontrollü eriþim (getter)
}


/* --- static + volatile Kullanýmý --- */

// ISR ile paylaþýlan ama dosya dýþýna kapalý veri
static volatile uint8_t isrFlag;

void EXTI_IRQHandler(void)
{
    isrFlag = 1;                        // Interrupt içinde set edilir
}

void MainLoop_Task(void)
{
    if (isrFlag)
    {
        isrFlag = 0;                    // Flag temizlenir
    }
}


/* --- Global vs Static RAM Yerleþimi --- */

// .data › initialize edilmiþ global/static
// .bss  › initialize edilmemiþ global/static
// stack › local deðiþkenler
// heap  › malloc (genelde kullanýlmaz)

static uint32_t initVar = 10;           // .data segment
static uint32_t uninitVar;              // .bss segment


/* --- const ile Flash Kullanýmý --- */

// const deðiþkenler genelde Flash’a yerleþtirilir
const uint8_t lookupTable[4] = {1,2,3,4};   // Flash (ROM) kullanýmý


/* --- static const Kullanýmý --- */

// Hem dosya içi hem Flash tabanlý sabit veri
static const uint16_t adcLut[3] = {100, 200, 300};


/* --- Fonksiyon Ýçinde static Buffer --- */

// Stack yerine kalýcý buffer (özellikle büyük veri için)
void ProcessData(void)
{
    static uint8_t buffer[128];         // Stack taþmasýný önler
    buffer[0] = 1;
}


/* --- Yanlýþ Kullaným Riski --- */

// Büyük static deðiþkenler RAM'i kalýcý tüketir
// static uint8_t bigArray[10000];      // RAM overflow riski


/* --- Embedded Perspektif --- */

// static neden önemli?
// - Modül izolasyonu (encapsulation)
// - Global namespace kirlenmesini önler
// - ISR ile güvenli veri paylaþýmý
// - Driver state yönetimi

// extern neden önemli?
// - Modüller arasý veri paylaþýmý
// - Peripheral flag eriþimi
// - RTOS task haberleþmesi (bazý senaryolar)

// kritik not:
// ISR ile paylaþýlan deðiþkenler › volatile olmalý
