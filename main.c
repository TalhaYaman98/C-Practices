/* ---------- Struct ---------- */ 

/* struct, farklý tipteki verileri tek bir mantýksal yapý altýnda toplamak için kullanýlýr. Gömülü sistemlerde bu genellikle peripheral, sensör veya konfigürasyon modeli anlamýna gelir. */
struct SensorData
{
    uint16_t raw;             // ADC’den okunan ham deðer
    float voltage;            // Hesaplanan voltaj
    uint8_t status;           // Sensör durumu (0: hata, 1: OK)
};

// struct Deðiþkeni Tanýmlama ve Kullanma
struct SensorData sensor1;    // SensorData tipinde deðiþken

sensor1.raw = 2000;           // Struct üyesine eriþim
sensor1.voltage = 1.65f;      // Nokta (.) operatörü kullanýlýr
sensor1.status = 1;           // Sensör aktif


/* typedef ile Daha Okunabilir Struct Tanýmý. STM32 projelerinde standart kullaným þeklidir. */
typedef struct
{
    uint16_t raw;             // ADC ham veri
    float voltage;            // Voltaj deðeri
    uint8_t status;           // Sensör durumu
} Sensor_t;

Sensor_t sensor2;             // Artýk struct keyword gerekmez
sensor2.raw = 3000;           
sensor2.voltage = 2.4f;       
sensor2.status = 0;           


/* Struct Pointer Kullanýmý (-> Operatörü). Driver ve HAL fonksiyonlarýnda en sýk kullanýlan yapý. */
Sensor_t sensor3;             
Sensor_t *pSensor;            

pSensor = &sensor3;            // Struct adresi pointer’a atanýr

pSensor->raw = 1500;           
pSensor->voltage = 1.2f;       
pSensor->status = 1;           


/* Fonksiyonlara Struct Pointer Gönderme. STM32 driver mimarisinin temelidir. */
void Sensor_Update(Sensor_t *sensor)   
{
    sensor->raw = 2048;                // ADC okuma simülasyonu
    sensor->voltage = 1.65f;           // Voltaj hesaplanýr
    sensor->status = 1;                // Sensör geçerli
}


/* Struct Return Eden Fonksiyon. Küçük yapýlar için kullanýþlýdýr (copy maliyeti dikkate alýnmalý). */
Sensor_t Sensor_Create(void)
{
    Sensor_t s;

    s.raw = 1000;              
    s.voltage = 0.8f;          
    s.status = 1;              

    return s;                  // Struct return edilir
}


/* Struct Ýçinde Struct Kullanýmý (Nested Struct). Kompleks peripheral modellerinde kullanýlýr. */
typedef struct
{
    uint16_t raw;
    float voltage;
} ADC_Data_t;

typedef struct
{
    ADC_Data_t adc;            // Ýç içe struct
    uint8_t status;
} SensorEx_t;

SensorEx_t sensorEx;

sensorEx.adc.raw = 2500;       // Nested eriþim
sensorEx.status = 1;


/* Struct ile Peripheral Konfigürasyonu (STM32 Tarzý). HAL yapýlarýna birebir benzer örnek. */
typedef struct
{
    uint32_t baudRate;         // UART baud rate
    uint8_t dataBits;          
    uint8_t stopBits;          
} UART_Config_t;

UART_Config_t uart1Config;

uart1Config.baudRate = 115200;
uart1Config.dataBits = 8;
uart1Config.stopBits = 1;


/* Struct Dizisi (Birden Fazla Sensör / Peripheral) */
Sensor_t sensors[3];           

sensors[0].raw = 1000;         
sensors[1].raw = 2000;         
sensors[2].raw = 3000;         


/* Struct + Döngü Kullanýmý. Çoklu sensör tarama (scan) senaryosu */
for (int i = 0; i < 3; i++)
{
    sensors[i].status = 1;     // Tüm sensörleri aktif yap
}


/* Struct Ýçinde Pointer Kullanýmý (DMA / Buffer Yönetimi) */
typedef struct
{
    uint8_t *pData;            // Veri buffer pointer
    uint16_t size;             // Buffer boyutu
} Buffer_t;

uint8_t dataArray[10];

Buffer_t buffer;

buffer.pData = dataArray;      // Array adresi atanýr
buffer.size = sizeof(dataArray);


/* Pointer + Struct = STM32 HAL Mantýðý (Kavramsal). */
typedef struct
{
    volatile uint32_t *ODR;    // Output Data Register adresi
    volatile uint32_t *IDR;    // Input Data Register adresi
} GPIO_Port_t;


/* Register Map Simülasyonu */
GPIO_Port_t gpioA;

gpioA.ODR = (uint32_t*)0x40020014;   // Örnek adres (STM32 GPIOA ODR)
gpioA.IDR = (uint32_t*)0x40020010;   // GPIOA IDR adresi

*(gpioA.ODR) |= (1 << 5);            // PA5 HIGH yapýlýr (register eriþimi)


/* Const Struct Kullanýmý (Flash'ta Sabit Konfigürasyon) */
const UART_Config_t uartDefault =
{
    .baudRate = 9600,
    .dataBits = 8,
    .stopBits = 1
};                                  // Deðiþtirilemez konfigürasyon


/* Volatile Struct Kullanýmý (Interrupt ile Deðiþen Veri) */
typedef struct
{
    volatile uint8_t flag;     // ISR tarafýndan deðiþtirilebilir
    volatile uint16_t value;
} ISR_Data_t;

ISR_Data_t isrData;

if (isrData.flag)             // Compiler optimizasyonu engellenir
{
    /* ISR tetiklenmiþ */
}


int main(void)
{
    Sensor_t mySensor;

    Sensor_Update(&mySensor);   // Pointer ile güncelleme

    Sensor_t newSensor = Sensor_Create();  // Struct return kullanýmý

    while (1)
    {
        /* main loop */
    }
}
