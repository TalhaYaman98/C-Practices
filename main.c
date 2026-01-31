#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>           // Sabit bit geniþliðinde veri tipleri

//-----------------------------------------------------------------------------------------------------------------------------
// Deðiþken tanýmlamalarý 

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

//const Kullanýmý (Flash Bellek Odaklý). Sabitler Flash bellekte tutulur, RAM tüketmez.
const uint16_t ADC_MAX = 4095;    // 12-bit ADC maksimum deðeri
const float VREF = 3.3f;          // Referans voltaj sabiti

// volatile Kullanýmý (Donaným Register / ISR). Donaným tarafýndan deðiþebilen deðiþkenler için zorunludur.
volatile uint8_t uartRxFlag;       // UART interrupt içinde deðiþtirilen bayrak
volatile uint32_t msCounter;       // Timer interrupt ile artan sayaç

// Global ve Local Deðiþken Örneði
uint8_t errorCode;                 // Global deðiþken, tüm dosyada eriþilebilir
void ReadSensor(void)
{
    uint16_t sensorValue;          // Local deðiþken, sadece fonksiyon içinde geçerli
    sensorValue = 1234;            // Sensör okuma simülasyonu
}

// STM32 HAL Tarzý Tip Tanýmlama (typedef). Kod okunabilirliðini ciddi þekilde artýrýr.
typedef uint8_t bool_t;            // Gömülü projelerde sýk kullanýlan boolean tanýmý
bool_t isSystemReady;              // Sistem hazýr mý bayraðý

//-----------------------------------------------------------------------------------------------------------------------------
// Pointer 

// Pointer, bir deðiþkenin RAM’deki adresini tutan deðiþkendir.
int value = 10;              // RAM’de bir tamsayý deðiþken
int *pValue;                 // int tipinde bir deðiþkenin adresini tutacak pointer
pValue = &value;             // value deðiþkeninin adresi pValue içine atanýr

// Pointer ile Deðere Eriþim (Dereference). * operatörü ile pointer’ýn gösterdiði adresteki deðere ulaþýlýr. STM32 tarafýnda bu mekanizma register eriþiminin temelidir.
int value = 10;              // Normal deðiþken
int *pValue = &value;        // value’nun adresi pointer’a atanýr
value = 20;                  // Deðiþken doðrudan deðiþtirilir
*pValue = 30;                // Pointer üzerinden ayný deðiþken deðiþtirilir

// Pointer ve Sabit Geniþlikli Tipler (stdint.h). Gömülü sistemlerde pointer tipi, iþaret edilen veriyle birebir uyumlu olmalýdýr.
uint16_t adcValue = 0;       // ADC ölçüm deðeri
uint16_t *pAdcValue;         // 16-bit veri gösteren pointer
pAdcValue = &adcValue;       // Adres atamasý

// Fonksiyonlara Pointer ile Parametre Gönderme. STM32 HAL ve driver yapýlarýnda çok sýk kullanýlýr.
void ReadADC(uint16_t *value)    // ADC sonucunu yazmak için pointer alýnýr
{
    *value = 2048;               // Ölçüm sonucu pointer üzerinden yazýlýr
}

int main(void)
{
    uint16_t adcResult = 0;      // ADC sonucu tutulacak deðiþken
    ReadADC(&adcResult);         // Deðiþkenin adresi fonksiyona gönderilir
}

// Pointer ile Donaným Register Eriþimi (Temel Mantýk). STM32 register’larý belirli bellek adreslerindedir. Bu yapý: CMSIS HAL Bare-metal STM32 kodlarýnýn temelidir
#define GPIOA_ODR   ((uint32_t*)0x48000014)   // GPIOA Output Data Register adresi
*GPIOA_ODR = 0x00000001;       // PA0 pinini 1 yapar (LED yakma örneði)

// volatile ve Pointer Birlikteliði. Donaným register’larý volatile olmak zorundadýr.
#define GPIOA_IDR   ((volatile uint32_t*)0x48000010)   // GPIOA Input Data Register
uint32_t buttonState;
buttonState = *GPIOA_IDR;      // Buton durumu okunur

// Pointer ile Dizi Ýliþkisi (STM32 Buffer Kullanýmý). UART, SPI, I2C buffer mantýðý bu yapý üzerine kuruludur.
uint8_t rxBuffer[10];          // UART RX buffer
uint8_t *pRxBuffer;            // Buffer baþlangýcýný gösteren pointer
pRxBuffer = rxBuffer;          // Dizinin ilk eleman adresi

//-----------------------------------------------------------------------------------------------------------------------------
// Struct

// struct, farklý tipteki verileri tek bir mantýksal yapý altýnda toplamak için kullanýlýr. Gömülü sistemlerde bu genellikle peripheral, sensör veya konfigürasyon modeli anlamýna gelir.
struct SensorData
{
    uint16_t raw;             // ADC’den okunan ham deðer
    float voltage;            // Hesaplanan voltaj
    uint8_t status;           // Sensör durumu
};

// struct Deðiþkeni Tanýmlama ve Kullanma
struct SensorData sensor1;    // SensorData tipinde deðiþken

sensor1.raw = 2000;           // Struct üyesine eriþim
sensor1.voltage = 1.65f;      // Nokta (.) operatörü kullanýlýr
sensor1.status = 1;           // Sensör aktif

// typedef ile Daha Okunabilir Struct Tanýmý. STM32 projelerinde standart kullaným þeklidir.
typedef struct
{
    uint16_t raw;             // ADC ham veri
    float voltage;            // Voltaj deðeri
    uint8_t status;           // Sensör durumu
} Sensor_t;

Sensor_t sensor1;             // Artýk struct keyword gerekmez
sensor1.raw = 3000;           // Üye eriþimi
sensor1.voltage = 2.4f;       // Hesaplanan deðer
sensor1.status = 0;           // Sensör pasif

// Struct Pointer Kullanýmý (-> Operatörü). Driver ve HAL fonksiyonlarýnda en sýk kullanýlan yapý.
Sensor_t sensor1;             // Sensor yapýsý
Sensor_t *pSensor;            // Sensor yapýsýný gösteren pointer

pSensor = &sensor1;            // Struct adresi pointer’a atanýr

pSensor->raw = 1500;           // Pointer ile struct üyesine eriþim
pSensor->voltage = 1.2f;       // -> operatörü kullanýlýr
pSensor->status = 1;           // Sensör aktif

// Fonksiyonlara Struct Pointer Gönderme. STM32 driver mimarisinin temelidir.
void Sensor_Update(Sensor_t *sensor)   // Struct pointer parametre olarak alýnýr
{
    sensor->raw = 2048;                // ADC okuma simülasyonu
    sensor->voltage = 1.65f;           // Voltaj hesaplanýr
    sensor->status = 1;                // Sensör geçerli
}

int main(void)
{
    Sensor_t mySensor;                 // Sensor instance
    Sensor_Update(&mySensor);           // Adres fonksiyona gönderilir
}

// Struct ile Peripheral Konfigürasyonu (STM32 Tarzý). HAL yapýlarýna birebir benzer örnek.
typedef struct
{
    uint32_t baudRate;         // UART baud rate
    uint8_t dataBits;          // Veri bit sayýsý
    uint8_t stopBits;          // Stop bit sayýsý
} UART_Config_t;

UART_Config_t uart1Config;     // UART konfigürasyon yapýsý

uart1Config.baudRate = 115200; // UART hýzý
uart1Config.dataBits = 8;      // 8-bit veri
uart1Config.stopBits = 1;      // 1 stop bit

// Struct Dizisi (Birden Fazla Sensör / Peripheral)
Sensor_t sensors[3];           // 3 adet sensör yapýsý

sensors[0].raw = 1000;         // Ýlk sensör
sensors[1].raw = 2000;         // Ýkinci sensör
sensors[2].raw = 3000;         // Üçüncü sensör

// Embedded Perspektifinden Neden Struct?
//STM32 tarafýnda struct kullanýmý: Peripheral konfigürasyonlarý, Driver state yönetimi, Sensör soyutlama, Kod okunabilirliði, Bakým ve geniþletilebilirlik için kritik önemdedir.

// Pointer + Struct = STM32 HAL Mantýðý (Kavramsal). Bu yapý, STM32’nin CMSIS register haritalamasýnýn temel fikridir.
typedef struct
{
    volatile uint32_t *ODR;    // GPIO Output Data Register adresi
    volatile uint32_t *IDR;    // GPIO Input Data Register adresi
} GPIO_Port_t;



int main(int argc, char *argv[]) {
	
	printf("C dili pratikleri");
	return 0;
}
