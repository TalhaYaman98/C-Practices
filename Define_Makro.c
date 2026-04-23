/* ---------- #define ve Makrolar ---------- */

#include <stdint.h>                         // Standart veri tipleri

/* --- Basit Sabit Tanýmlarý --- */

// Derleme zamanýnda yerine yazýlýr, RAM/FLASH tüketmez
#define TRUE        1                       // Mantýksal doðru
#define FALSE       0                       // Mantýksal yanlýþ

#define MAX_SPEED   3000                    // Sistem için maksimum hýz limiti
#define ADC_MAX     4095                    // 12-bit ADC maksimum deðeri


/* --- Bit Maskeleri --- */

// Register veya flag kontrolü için kullanýlýr
#define BIT0        (1U << 0)               // 0000 0001
#define BIT1        (1U << 1)               // 0000 0010
#define BIT2        (1U << 2)               // 0000 0100
#define BIT3        (1U << 3)               // 0000 1000
#define BIT4        (1U << 4)               // 0001 0000


/* --- Donaným Register Maskeleme Örneði --- */

#define UART_TX_EN  BIT3                    // UART TX enable biti
#define UART_RX_EN  BIT4                    // UART RX enable biti


/* --- Parametreli Makrolar --- */

// Fonksiyon çaðrýsý yoktur, inline çalýþýr
#define ABS(x)      ((x) < 0 ? -(x) : (x))  // Mutlak deðer makrosu

#define LIMIT(x,min,max) \
                ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
                                              // Deðeri sýnýrlar

#define MIN(a,b)    ((a) < (b) ? (a) : (b))   // Minimum deðer
#define MAX(a,b)    ((a) > (b) ? (a) : (b))   // Maksimum deðer


/* --- Güvenli Makro Yazýmý --- */

// Hatalý kullaným (öncelik problemi)
// #define SQR(x) x*x                        // 3+2 › 3+2*3+2

// Doðru kullaným
#define SQR(x)      ((x) * (x))             // Parantez zorunlu


/* --- Çok Satýrlý Makro --- */

// STM32 tarzý do-while(0) yapýsý
#define GPIO_SET_RESET(port, setMask, rstMask) \
    do {                                       \
        (port)->BSRR = (setMask);              \
        (port)->BRR  = (rstMask);              \
    } while(0)


/* --- Bit Ýþlem Makrolarý --- */

// Register bit set / clear / toggle
#define SET_BIT(REG, BIT)      ((REG) |= (BIT))    // Bit set edilir
#define CLEAR_BIT(REG, BIT)    ((REG) &= ~(BIT))   // Bit temizlenir
#define TOGGLE_BIT(REG, BIT)   ((REG) ^= (BIT))    // Bit toggle edilir
#define READ_BIT(REG, BIT)     ((REG) & (BIT))     // Bit okunur


/* --- Register Alan (Field) Manipülasyonu --- */

// Belirli bit alanýný temizleyip yeni deðer yazma
#define WRITE_REG_FIELD(REG, MASK, VAL) \
            ((REG) = ((REG) & ~(MASK)) | ((VAL) & (MASK)))   // Field update


/* --- Debug Amaçlý Makrolar --- */

// Debug açýk/kapalý kontrolü
#define DEBUG_ENABLE   1

#if DEBUG_ENABLE
    #define DEBUG_LOG(x)   (x)                // Debug aktif
#else
    #define DEBUG_LOG(x)   ((void)0)          // Derlemede yok sayýlýr
#endif


/* --- Derleme Zamaný Kontrolleri --- */

// Compile-time assert (C11)
#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

STATIC_ASSERT(sizeof(uint32_t) == 4, "uint32_t 4 byte olmalý");   // Platform kontrolü


/* --- Makro vs const Karþýlaþtýrmasý --- */

// Makro
#define TIMEOUT_MS   1000                     // Tip güvenliði yok

// const deðiþken
const uint32_t timeout_ms = 1000;             // Tip güvenliði var


/* --- Embedded Sistemlerde Tipik Kullaným --- */

// Register adresleri
#define GPIOA_BASE   0x40020000U              // GPIOA base adresi
#define GPIOA_ODR    (*(volatile uint32_t*)(GPIOA_BASE + 0x14))   // Output Data Register

// Register eriþimi
GPIOA_ODR |= BIT5;                           // PA5 HIGH yapýlýr


/* --- Pointer Tabanlý Register Makrosu --- */

// Struct ile register eriþimi (CMSIS benzeri yaklaþým)
#define PERIPH_REG(addr)   (*(volatile uint32_t*)(addr))  // Adrese eriþim makrosu

PERIPH_REG(0x40020014) |= BIT5;              // Direkt register yazýmý


/* --- Zamanlama Makrolarý (Delay / Tick Bazlý) --- */

// Basit delay (cycle bazlý - optimize edilebilir)
#define NOP() __asm("nop")                   // 1 cycle boþ komut

#define DELAY_LOOP(count)                   \
    do {                                    \
        for (volatile uint32_t i = 0; i < (count); i++) \
        {                                   \
            NOP();                          \
        }                                   \
    } while(0)


/* --- Flag Kontrol Makrosu --- */

// Flag set mi kontrolü
#define IS_FLAG_SET(REG, FLAG)   (((REG) & (FLAG)) != 0U)

if (IS_FLAG_SET(GPIOA_ODR, BIT5))   // Pin HIGH mý kontrolü
{
    /* iþlem */
}
