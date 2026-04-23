/* ---------- Struct ---------- */ 

/* struct, farklý tipteki verileri tek bir mantýksal yapý altýnda toplamak için kullanýlýr. Gömülü sistemlerde bu genellikle peripheral, sensör veya konfigürasyon modeli anlamýna gelir. */
struct SensorData
{
    uint16_t raw;             
    float voltage;            
    uint8_t status;           
};

// struct Deðiþkeni Tanýmlama ve Kullanma
struct SensorData sensor1;    

sensor1.raw = 2000;           
sensor1.voltage = 1.65f;      
sensor1.status = 1;           


/* typedef ile Daha Okunabilir Struct Tanýmý */
typedef struct
{
    uint16_t raw;             
    float voltage;            
    uint8_t status;           
} Sensor_t;

Sensor_t sensor2;             
sensor2.raw = 3000;           
sensor2.voltage = 2.4f;       
sensor2.status = 0;           


/* Struct Pointer Kullanýmý (-> Operatörü) */
Sensor_t sensor3;             
Sensor_t *pSensor;            

pSensor = &sensor3;            

pSensor->raw = 1500;           
pSensor->voltage = 1.2f;       
pSensor->status = 1;           


/* Fonksiyonlara Struct Pointer Gönderme */
void Sensor_Update(Sensor_t *sensor)   
{
    if (sensor != 0)                     // Null pointer güvenliði
    {
        sensor->raw = 2048;
        sensor->voltage = 1.65f;
        sensor->status = 1;
    }
}


/* Struct Return Eden Fonksiyon */
Sensor_t Sensor_Create(void)
{
    Sensor_t s;

    s.raw = 1000;
    s.voltage = 0.8f;
    s.status = 1;

    return s;                           // Küçük struct’lar için uygundur
}


/* Struct Ýçinde Struct (Nested Struct) */
typedef struct
{
    uint16_t raw;
    float voltage;
} ADC_Data_t;

typedef struct
{
    ADC_Data_t adc;            // Ýç struct
    uint8_t status;
} SensorEx_t;

SensorEx_t sensorEx;

sensorEx.adc.raw = 2500;       
sensorEx.status = 1;


/* Struct Dizisi */
Sensor_t sensors[3];           

sensors[0].raw = 1000;         
sensors[1].raw = 2000;         
sensors[2].raw = 3000;         


/* Struct + Loop Kullanýmý (Scan yapýsý) */
for (int i = 0; i < 3; i++)
{
    sensors[i].status = 1;     // Tüm sensörleri aktif et
}


/* Struct Ýçinde Pointer (DMA / Buffer Yönetimi) */
typedef struct
{
    uint8_t *pData;            
    uint16_t size;             
} Buffer_t;

uint8_t dataArray[10];

Buffer_t buffer;

buffer.pData = dataArray;      
buffer.size = sizeof(dataArray);


/* Struct ile Peripheral Konfigürasyonu */
typedef struct
{
    uint32_t baudRate;         
    uint8_t dataBits;          
    uint8_t stopBits;          
} UART_Config_t;

UART_Config_t uart1Config;

uart1Config.baudRate = 115200;
uart1Config.dataBits = 8;
uart1Config.stopBits = 1;


/* Bit-field Struct (Register Modelleme) */

// Donaným register’larýný bit seviyesinde temsil etmek için kullanýlýr
typedef struct
{
    uint32_t EN   : 1;         // Enable biti
    uint32_t MODE : 2;         // Mode seçimi
    uint32_t RES  : 29;        // Reserved alan
} CTRL_Reg_t;

CTRL_Reg_t ctrl;

ctrl.EN = 1;                  // Enable aktif
ctrl.MODE = 2;                // Mode ayarý


/* Packed Struct (Hafýza Optimizasyonu) */

// Padding kaldýrýlýr (protokol frame’leri için önemli)
typedef struct __attribute__((packed))
{
    uint8_t id;
    uint16_t value;
} Frame_t;

Frame_t frame;

frame.id = 1;
frame.value = 100;


/* Struct Alignment (Hizalama) */

// CPU eriþim performansý için önemlidir
typedef struct
{
    uint8_t a;
    uint32_t b;               // Alignment nedeniyle padding oluþur
} Align_t;


/* Pointer + Struct = STM32 HAL Mantýðý */

typedef struct
{
    volatile uint32_t *ODR;    
    volatile uint32_t *IDR;    
} GPIO_Port_t;

GPIO_Port_t gpioA;

gpioA.ODR = (uint32_t*)0x40020014;
gpioA.IDR = (uint32_t*)0x40020010;

*(gpioA.ODR) |= (1 << 5);     // Register üzerinden pin set


/* Const Struct (Flash’ta Sabit Konfigürasyon) */

const UART_Config_t uartDefault =
{
    .baudRate = 9600,
    .dataBits = 8,
    .stopBits = 1
};


/* Volatile Struct (ISR ile Paylaþýlan Veri) */

typedef struct
{
    volatile uint8_t flag;
    volatile uint16_t value;
} ISR_Data_t;

ISR_Data_t isrData;

if (isrData.flag)
{
    isrData.flag = 0;          // ISR flag temizleme
}


/* Embedded Perspektif */

// Struct neden kritik?
// - Peripheral soyutlama (HAL)
// - Register mapping (CMSIS)
// - DMA buffer yönetimi
// - Driver state machine tasarýmý
// - Kod okunabilirliði ve modülerlik
