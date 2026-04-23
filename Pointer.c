/* ---------- Pointer ---------- */

// Pointer, bir deðiþkenin RAM’deki adresini tutan deðiþkendir.
int value = 10;              
int *pValue;                 
pValue = &value;             


/* --- Pointer ile Deðere Eriþim (Dereference) --- */

// * operatörü ile pointer’ýn gösterdiði adresteki deðere ulaþýlýr.
int value2 = 10;             
int *pValue2 = &value2;      

value2 = 20;                 
*pValue2 = 30;                // Ayný deðiþken pointer üzerinden deðiþtirilir


/* --- Pointer ve Sabit Geniþlikli Tipler --- */

// Gömülü sistemlerde pointer tipi veri geniþliði ile uyumlu olmalýdýr
uint16_t adcValue = 0;       
uint16_t *pAdcValue;         
pAdcValue = &adcValue;       


/* --- Fonksiyonlara Pointer ile Parametre Gönderme --- */

// HAL ve driver yapýlarýnda temel kullaným
void ReadADC(uint16_t *value)    
{
    if (value != 0)               // Null pointer kontrolü
    {
        *value = 2048;           
    }
}

int main(void)
{
    uint16_t adcResult = 0;      
    ReadADC(&adcResult);         
}


/* --- Pointer ile Donaným Register Eriþimi --- */

// STM32 register’larý sabit adreslerdedir
#define GPIOA_ODR   ((uint32_t*)0x48000014)   

*GPIOA_ODR = 0x00000001;       // PA0 HIGH


/* --- volatile ve Pointer Birlikteliði --- */

// Donaným register’larý volatile olmalýdýr
#define GPIOA_IDR   ((volatile uint32_t*)0x48000010)   

uint32_t buttonState;
buttonState = *GPIOA_IDR;      // Donanýmdan anlýk veri okunur


/* --- Pointer ile Dizi Ýliþkisi --- */

// Dizinin adý zaten pointer’dýr (ilk elemanýn adresi)
uint8_t rxBuffer[10];          
uint8_t *pRxBuffer;            

pRxBuffer = rxBuffer;          

pRxBuffer[0] = 0x55;           // Index ile eriþim
*(pRxBuffer + 1) = 0xAA;       // Pointer aritmetiði ile eriþim


/* --- Pointer Aritmetiði --- */

// Pointer veri tipine göre artar
uint16_t arr[3] = {10, 20, 30};
uint16_t *pArr = arr;

pArr++;                        // 2 byte ileri gider (uint16_t)
uint16_t val = *pArr;          // 20


/* --- Pointer to Pointer (Çift Pointer) --- */

// Özellikle buffer yönetimi ve dinamik yapýlarýn temelidir
uint8_t data = 5;
uint8_t *pData = &data;
uint8_t **ppData = &pData;

**ppData = 10;                 // data = 10 olur


/* --- Const Pointer Kullanýmý --- */

// Veri sabit, pointer deðiþebilir
const uint8_t val1 = 10;
const uint8_t *p1 = &val1;     // Veri deðiþtirilemez

// Pointer sabit, veri deðiþebilir
uint8_t val2 = 20;
uint8_t *const p2 = &val2;     // Pointer adresi deðiþtirilemez

// Ýkisi de sabit
const uint8_t *const p3 = &val1;


/* --- Struct Pointer (HAL Mantýðý) --- */

typedef struct
{
    uint32_t ODR;
    uint32_t IDR;
} GPIO_t;

GPIO_t gpio;
GPIO_t *pGpio = &gpio;

pGpio->ODR = 1;                // HAL tarzý eriþim


/* --- Function Pointer (Callback Mekanizmasý) --- */

// Interrupt ve event tabanlý sistemlerin temelidir
void Led_On(void)
{
    /* LED yak */
}

void (*funcPtr)(void);         // Fonksiyon pointer

funcPtr = Led_On;              
funcPtr();                     // Fonksiyon çaðrýlýr


/* --- Void Pointer (Generic Kullaným) --- */

// Tip baðýmsýz pointer (generic API'lerde kullanýlýr)
void WriteData(void *data)
{
    uint8_t *p = (uint8_t*)data;   // Cast zorunlu
    *p = 0xFF;
}


/* --- Null Pointer Güvenliði --- */

uint8_t *pNull = 0;

if (pNull != 0)               // NULL kontrolü zorunlu
{
    *pNull = 1;
}


/* --- Embedded Perspektif --- */

// Pointer neden kritik?
// - Register eriþimi (memory mapped IO)
// - DMA buffer yönetimi
// - ISR veri paylaþýmý
// - HAL driver yapýlarý
// - Performans (kopya yerine adres ile çalýþma)
