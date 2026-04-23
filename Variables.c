/* ---------- Deðiþken tanýmlamalarý ---------- */ 

#include <stdint.h>                     // Sabit geniþlikli veri tipleri


/* --- Temel Deðiþken Tanýmlamalarý --- */

int counter;                 // Ýþaretli tam sayý (genelde 32-bit)
unsigned int tick;           // Negatif olmayan sayaçlar
char rxByte;                 // 1 byte veri (UART vb.)
float temperature;           // Ondalýklý veri (FPU yoksa maliyetli)


/* --- STM32’de Sýk Kullanýlan Tipler (stdint.h) --- */

uint8_t  uartData;           
uint16_t adcValue_local;     
uint32_t systemTick;         
int16_t  motorSpeed;         


/* --- Baþlangýç Deðeri Verilerek Tanýmlama --- */

uint8_t ledState = 0;        
uint16_t adcRaw = 0;         
float voltage = 0.0f;        


/* --- Global Deðiþken Tanýmý (TEK YERDE) --- */

// globals.c
uint16_t adcValue = 0;                   
volatile uint8_t uartRxFlag = 0;         


/* --- extern Kullanýmý (Baþka Dosyada) --- */

// globals.h
// extern uint16_t adcValue;
// extern volatile uint8_t uartRxFlag;

// #include sadece bildirimi getirir, bellekte yer ayýrmaz
// extern › "taným baþka yerde" demektir


/* --- volatile Temel Kullaným --- */

volatile uint8_t uartRxFlag2;             
volatile uint32_t msCounter;              


/* --- volatile Olmadan (Hatalý Senaryo) --- */

uint8_t buttonPressed;                    

void WaitButton(void)
{
    while (buttonPressed == 0)             // Compiler optimize edebilir
    {
        // Sonsuz döngü riski
    }
}


/* --- volatile ile Doðru Kullaným --- */

volatile uint8_t buttonPressedVol;         

void WaitButtonSafe(void)
{
    while (buttonPressedVol == 0)          
    {
        // Her seferinde RAM’den okunur
    }
}


/* --- ISR Kullanýmý --- */

void USART1_IRQHandler(void)
{
    uartRxFlag = 1;                       
}


/* --- Donaným Register Eriþimi --- */

#define GPIO_ODR   (*(volatile uint32_t*)0x48000014)

void LedToggle(void)
{
    GPIO_ODR ^= (1 << 5);                 
}


/* --- volatile Pointer Kullanýmý --- */

volatile uint32_t *timerCnt = (uint32_t*)0x40000024;

uint32_t ReadTimer(void)
{
    return *timerCnt;                     
}


/* --- volatile Pointer Türleri --- */

volatile uint8_t *pVolData;               
uint8_t * volatile pVolPtr;               
volatile uint8_t * volatile pFullVol;     


/* --- const + volatile --- */

const volatile uint16_t ADC_DR = 0x0000;  

// Yazýlamaz, sadece okunur (donaným deðiþtirir)


/* --- Fonksiyon Parametresi --- */

void ClearFlag(volatile uint8_t *flag)
{
    if (flag != 0)
    {
        *flag = 0;
    }
}


/* --- volatile Kritik Not --- */

// volatile = thread-safe DEÐÝLDÝR (atomic deðildir)


/* --- const Temel Kullaným --- */

const uint16_t ADC_MAX = 4095;            
const float VREF = 3.3f;                 


/* --- const Olmadan --- */

uint16_t adcLimit = 4095;                 


/* --- const Parametre --- */

void ProcessAdcValue(const uint16_t adcValue)
{
    // adcValue deðiþtirilemez
}


/* --- const Pointer Türleri --- */

uint8_t buffer1[10];
uint8_t * const pBufferFixed = buffer1;   

const uint8_t buffer2[10] = {0};
const uint8_t *pReadOnly = buffer2;       

const uint8_t * const pFullConst = buffer2;


/* --- Lookup Table (Flash) --- */

const uint16_t sinTable[5] =
{
    0, 707, 1000, 707, 0
};


/* --- const Struct --- */

typedef struct
{
    uint32_t baudrate;
    uint8_t  parity;
    uint8_t  stopBits;
} uart_config_t;

const uart_config_t uart1Config =
{
    .baudrate = 115200,
    .parity   = 0,
    .stopBits = 1
};


/* --- const vs #define --- */

#define TIMEOUT_MS   1000

const uint32_t TIMEOUT_MS_CONST = 1000;


/* --- static Global --- */

static uint8_t errorCounter;              
static uint16_t adcInternalValue;         

void Adc_Update(uint16_t value)
{
    adcInternalValue = value;
}


/* --- static Local --- */

void ButtonHandler(void)
{
    static uint8_t pressCount = 0;        
    pressCount++;
}


/* --- Normal Local --- */

void ButtonHandler_Normal(void)
{
    uint8_t pressCount = 0;               
    pressCount++;
}


/* --- ISR içinde static --- */

void TIM2_IRQHandler(void)
{
    static uint32_t tick = 0;             
    tick++;
}


/* --- static Fonksiyon --- */

static void HardwareReset(void)
{
    // Donaným reset
}

void SystemReset(void)
{
    HardwareReset();
}


/* --- static + const --- */

static const uint16_t adcMaxValue = 4095;


/* --- Memory Segment Perspektifi --- */

// .data › initialize edilmiþ global/static
// .bss  › initialize edilmemiþ global/static
// stack › local deðiþkenler
// heap  › malloc (genelde kullanýlmaz)


/* --- Embedded Kritik Notlar --- */

/*
- global deðiþkenler › tek yerde tanýmlanmalý
- extern › paylaþým, static › izolasyon
- volatile › ISR / donaným için zorunlu
- const › Flash kullanýmý (RAM tasarrufu)
- static local › state machine için ideal
- volatile ? atomic
- büyük veri › stack yerine static/const tercih edilmeli
*/
