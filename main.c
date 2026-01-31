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







int main(int argc, char *argv[]) {
	
	printf("C dili pratikleri");
	return 0;
}
