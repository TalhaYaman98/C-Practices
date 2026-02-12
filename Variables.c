/* ---------- Deðiþken tanýmlamalarý ---------- */ 

// Temel Deðiþken Tanýmlamalarý
int counter;                 // Varsayýlan olarak RAM’de yer alan, iþareti olan tam sayý deðiþken
unsigned int tick;           // Negatif deðer almayan sayaçlar için tercih edilir
char rxByte;                 // UART vb. haberleþmelerde tek bayt veri için kullanýlýr
float temperature;           // Sensörlerden gelen ondalýklý veriler için

// STM32’de Sýk Kullanýlan Sabit Geniþlikli Tipler (stdint.h). Gömülü sistemlerde taþýnabilirlik ve bellek kontrolü için bu tipler kritik önemdedir.
uint8_t  uartData;            // 8-bit, UART RX/TX için ideal
uint16_t adcValue;            // 12-bit ADC verileri genelde 16-bit deðiþkende tutulur
uint32_t systemTick;          // SysTick veya zamanlayýcý sayaçlarý için
int16_t  motorSpeed;          // Pozitif/negatif yönlü motor hýz bilgisi

// Baþlangýç Deðeri Verilerek Tanýmlama. STM32 tarafýnda ilk deðer verilmemiþ deðiþkenler RAM’de rastgele deðer içerebilir.
uint8_t ledState = 0;         // LED baþlangýçta kapalý
uint16_t adcRaw = 0;          // ADC ölçümü baþlamadan önce sýfýrlanýr
float voltage = 0.0f;         // Hesaplamalarda belirsizlik olmamasý için

// Global deðiþkenlerin TEK tanýmý 
uint16_t adcValue = 0;
volatile uint8_t uartRxFlag = 0;

// Global deðiþkenlerin TEK tanýmý
extern uint16_t adcValue;          // Baþka .c dosyalarýnda kullanýlan. #include "globals.h" edilmeli.
extern volatile uint8_t uartRxFlag;

// #include deðiþkeni paylaþmaz, #include sadece bildirimi görünür kýlar. Asýl paylaþýmý saðlayan þey: extern.
// #include = “Bu bildirimi buraya kopyala”
// extern = “Tanýmý baþka bir yerde var”

// static global › sadece tanýmlandýðý .c dosyasýnda eriþilebilir
// Header’a extern yazýlsa bile eriþilemez
// Modül içi gizleme (encapsulation) için idealdir

//---------------------------------------------------------------------------------------------------------------------------------------

// volatile Temel Kullaným
// volatile ile tanýmlanan deðiþkenler, derleyiciye “bu deðiþken her an dýþ etkenle deðiþebilir” mesajý verir
// Derleyici bu deðiþkeni cache’lemez, her eriþimde RAM’den okur
volatile uint8_t uartRxFlag;               // UART interrupt içinde set edilen bayrak
volatile uint32_t msCounter;               // Timer interrupt ile artan sayaç

// volatile Olmadan Kullaným (Hatalý Senaryo)
// Derleyici optimizasyonu nedeniyle sonsuz döngü riski oluþur
uint8_t buttonPressed;                     // ISR içinde deðiþtiriliyor varsayalým

void WaitButton(void)
{
    while (buttonPressed == 0)             // ? Compiler bunu sabit kabul edebilir
    {
        // Sonsuz döngüye girme riski
    }
}

// volatile ile Doðru Kullaným
volatile uint8_t buttonPressedVol;          // ISR tarafýndan deðiþtirilen bayrak

void WaitButtonSafe(void)
{
    while (buttonPressedVol == 0)           // ? Her döngüde RAM’den okunur
    {
        // Güvenli bekleme
    }
}

// volatile ve ISR (Interrupt Service Routine) Kullanýmý
// ISR içinde deðiþtirilen her global deðiþken volatile OLMALIDIR
void USART1_IRQHandler(void)
{
    uartRxFlag = 1;                         // Ana döngüye veri geldi bilgisi verilir
}

// volatile ve Donaným Register Mantýðý
// STM32 register’larý zaten volatile olarak tanýmlýdýr
#define GPIO_ODR   (*(volatile uint32_t*)0x48000014)  // GPIO output data register

void LedToggle(void)
{
    GPIO_ODR ^= (1 << 5);                   // Donaným register doðrudan deðiþtirilir
}

// volatile ve Pointer Kullanýmý
// Pointer’ýn gösterdiði veri volatile
volatile uint32_t *timerCnt = (uint32_t*)0x40000024; // Timer counter register

uint32_t ReadTimer(void)
{
    return *timerCnt;                       // Donanýmdan her seferinde okunur
}

// volatile Pointer Türleri (Kritik Ayrým)

// 1 Veri volatile, pointer normal
volatile uint8_t *pVolData;                // Gösterilen veri deðiþken

// 2 Pointer volatile, veri normal
uint8_t * volatile pVolPtr;                // Pointer adresi deðiþebilir (DMA senaryosu)

// 3 Hem pointer hem veri volatile
volatile uint8_t * volatile pFullVol;      // Donaným + DMA + ISR senaryolarý

// volatile ve const Birlikte Kullanýmý
// Okunabilir ama dýþ etkenle deðiþebilir veri
const volatile uint16_t ADC_DR = 0x0000;    // ADC data register simülasyonu

// ADC_DR = 100;                            // ? Yazýlamaz
// Okuma serbest, deðiþim donaným kaynaklý

// volatile ve Fonksiyon Parametreleri
// ISR veya donanýmdan gelen pointer’lar volatile olmalýdýr
void ClearFlag(volatile uint8_t *flag)
{
    *flag = 0;                              // Compiler optimizasyonu engellenir
}

// volatile ve extern Kullanýmý
// Header dosyasýnda:
// extern volatile uint8_t uartRxFlag;

// Bu .c dosyasýnda tanýmlanýr
volatile uint8_t uartRxFlag;                // ISR + main loop ortak deðiþken

// volatile Yanlýþ Kullaným Örnekleri

// Gereksiz volatile (performans düþürür)
volatile uint16_t localCounter;             // ? ISR/donaným yoksa gereksiz

// volatile Thread-Safe DEÐÝLDÝR
// Atomic iþlem saðlamaz, sadece optimizasyonu engeller

// Embedded Kritik Notlar
/*
- ISR içinde deðiþtirilen TÜM global deðiþkenler volatile olmalý
- Donaným register eriþimleri volatile olmadan YAPILAMAZ
- volatile = senkronizasyon deðildir
- volatile fazla kullanýlýrsa performans düþer
- const + volatile birlikte kullanýmý register okumalarý için idealdir
*/

//---------------------------------------------------------------------------------------------------------------------------------------

// const Temel Kullaným
// const ile tanýmlanan deðiþkenler deðiþtirilemez
// STM32’de genellikle Flash bellekte tutulurlar
const uint16_t ADC_MAX = 4095;             // 12-bit ADC maksimum deðeri
const float VREF = 3.3f;                   // Referans voltaj

// const Olmadan Tanýmlama (RAM Tüketir)
// const kullanýlmazsa deðiþken RAM’de yer kaplar
uint16_t adcLimit = 4095;                  // RAM kullanýr, deðiþtirilebilir

// const ve Fonksiyon Parametreleri
// Fonksiyon içinde parametrenin yanlýþlýkla deðiþtirilmesini önler
void ProcessAdcValue(const uint16_t adcValue)
{
    // adcValue = 100;                      // ? Derleme hatasý
}

// const Pointer Kullanýmý
// 1 Pointer sabit, veri deðiþebilir
uint8_t buffer1[10];
uint8_t * const pBufferFixed = buffer1;    // Pointer adresi sabit

// pBufferFixed = baþka_adres;              // ? Yasak
pBufferFixed[0] = 0x55;                    // ? Veri deðiþebilir

// 2 Veri sabit, pointer deðiþebilir
const uint8_t buffer2[10] = {0};
const uint8_t *pReadOnly = buffer2;        // Salt okunur veri

// pReadOnly[0] = 0xAA;                     // ? Yasak
pReadOnly++;                               // ? Pointer baþka adrese ilerleyebilir

// 3 Hem pointer hem veri sabit
const uint8_t * const pFullConst = buffer2; // Tam korumalý yapý

// const ve Flash Tabanlý Lookup Table
// Lookup table’lar mutlaka const olmalýdýr
// Aksi halde RAM’i gereksiz tüketir
const uint16_t sinTable[5] =
{
    0,
    707,
    1000,
    707,
    0
};

// const ve Struct Kullanýmý
// Konfigürasyon yapýlarý Flash’ta tutulur
typedef struct
{
    uint32_t baudrate;                     // UART baudrate
    uint8_t  parity;                       // Parity ayarý
    uint8_t  stopBits;                     // Stop bit sayýsý
} uart_config_t;

// Sistem sabit konfigürasyonu
const uart_config_t uart1Config =
{
    .baudrate = 115200,
    .parity   = 0,
    .stopBits = 1
};

// const ve extern Kullanýmý

// Header dosyasýnda:
// extern const uint16_t ADC_MAX;

// Bu .c dosyasýnda tanýmlanýr
// const olduðu için Flash’ta yer alýr

// const vs #define

// #define › Tip güvenliði yok
#define TIMEOUT_MS   1000

// const › Tip güvenliði var, debugger’da izlenebilir
const uint32_t TIMEOUT_MS_CONST = 1000;

// Embedded Kritik Notlar 
/*
- const = Flash kullanýmý (RAM tasarrufu)
- Büyük tablolar ve konfigürasyonlar mutlaka const olmalý
- ISR içinde deðiþen deðiþkenler const OLAMAZ
- const + volatile birlikte kullanýlabilir (örn: status register)
*/

//---------------------------------------------------------------------------------------------------------------------------------------

// static Temel Kullaným (Global)
// static global deðiþkenler SADECE tanýmlandýðý .c dosyasýnda eriþilebilirdir
// Lifetime = program süresi, Scope = dosya ile sýnýrlý
static uint8_t errorCounter;               // Modül içi hata sayacý
// errorCounter baþka .c dosyalarýndan eriþilemez (extern ile bile)

// static Olmadan Global Kullaným
// static yoksa global deðiþken tüm projeye açýktýr (extern ile)
uint8_t systemMode;                        // Tüm dosyalardan eriþilebilir
// Header dosyasýnda:
// extern uint8_t systemMode;

// static Global ile Encapsulation
// Driver iç durumu dýþ dünyadan gizlemek için static tercih edilir
static uint16_t adcInternalValue;          // Sadece adc.c içinde kullanýlýr

void Adc_Update(uint16_t value)
{
    adcInternalValue = value;              // Ýç durum güncellenir
}

// static Local (Fonksiyon Ýçi) Kullaným
// static local deðiþkenler stack üzerinde DEÐÝL, RAM’dedir
// Deðerlerini fonksiyon çaðrýlarý arasýnda KORURLAR
void ButtonHandler(void)
{
    static uint8_t pressCount = 0;          // Ýlk çaðrýda bir kez init edilir
    pressCount++;                           // Fonksiyon her çaðrýldýðýnda artar
}
// pressCount fonksiyon dýþýndan eriþilemez
// pressCount deðeri fonksiyon bitse bile kaybolmaz

// static Olmadan Local Deðiþken
void ButtonHandler_Normal(void)
{
    uint8_t pressCount = 0;                 // Her çaðrýda yeniden oluþturulur
    pressCount++;                           // Fonksiyon çýkýnca yok olur
}

// static ve ISR Senaryosu
// ISR içinde kullanýlan static local deðiþken
void TIM2_IRQHandler(void)
{
    static uint32_t tick = 0;               // ISR çaðrýlarý arasýnda korunur
    tick++;                                 // Zaman sayacý
}
// tick sadece bu ISR içinde bilinir

// static Fonksiyon Kullanýmý
// static fonksiyonlar sadece tanýmlandýðý .c dosyasýnda çaðrýlabilir
// Header dosyasýna YAZILMAZ
static void HardwareReset(void)
{
    // Donaným reset iþlemleri
}

// static Fonksiyon Olmadan
void SystemReset(void)
{
    HardwareReset();                        // Ayný dosya içinden çaðrýlabilir
}
// static + const Kullanýmý
// static + const = dosya içi sabit
// Header’a koymaya gerek yok, RAM tüketmez
static const uint16_t adcMaxValue = 4095;   // Sadece bu modülde geçerli

// static ve extern Birlikte Kullanýlamaz
// static uint8_t data;
// extern static uint8_t data;              // ? Mantýksal olarak anlamsýz

// static › içe kapalý
// extern › dýþa açýk

// static Yanlýþ Kullaným Örnekleri
// Gereksiz static local (okunabilirliði düþürür)
void Calculate(void)
{
    static uint8_t temp;                    // ? Korunmasý gerekmiyorsa yanlýþ
    temp = 5;
}

// Embedded Kritik Notlar
/*
- static global = modül içi kapsülleme
- static local = fonksiyon çaðrýlarý arasýnda veri koruma
- static fonksiyon = private API
- static RAM kullanýr, stack kullanmaz
- ISR sayaçlarý için static local idealdir
- Header dosyalarýnda static KULLANILMAZ (istisna: inline)
*/

//---------------------------------------------------------------------------------------------------------------------------------------

