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

/* --- Donaným Register Maskeleme Örneði --- */

#define UART_TX_EN  BIT3                    // UART TX enable biti
#define UART_RX_EN  BIT4                    // UART RX enable biti

/* --- Parametreli Makrolar --- */

// Fonksiyon çaðrýsý yoktur, inline çalýþýr
#define ABS(x)      ((x) < 0 ? -(x) : (x))  // Mutlak deðer makrosu

#define LIMIT(x,min,max) \
                ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
                                              // Deðeri sýnýrlar

/* --- Güvenli Makro Yazýmý --- */

// Hatalý kullaným (öncelik problemi)
// #define SQR(x) x*x                        // ? 3+2 › 3+2*3+2

// Doðru kullaným
#define SQR(x)      ((x) * (x))             // ? Parantez zorunlu

/* --- Çok Satýrlý Makro --- */

// STM32 tarzý do-while(0) yapýsý
#define GPIO_SET_RESET(port, setMask, rstMask) \
    do {                                       \
        (port)->BSRR = (setMask);              \
        (port)->BRR  = (rstMask);              \
    } while(0)

/* --- Debug Amaçlý Makrolar --- */

// Debug açýk/kapalý kontrolü
#define DEBUG_ENABLE   1

#if DEBUG_ENABLE
    #define DEBUG_LOG(x)   (x)                // Debug aktif
#else
    #define DEBUG_LOG(x)   ((void)0)           // Derlemede yok sayýlýr
#endif

/* --- Makro vs const Karþýlaþtýrmasý --- */

// Makro
#define TIMEOUT_MS   1000                     // Tip güvenliði yok

// const deðiþken
const uint32_t timeout_ms = 1000;             // Tip güvenliði var

/* --- Embedded Sistemlerde Tipik Kullaným --- */

// Register adresleri
#define GPIOA_BASE_

