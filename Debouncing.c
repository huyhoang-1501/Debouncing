#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
	volatile uint32_t CR; //0x00: Clock Control Register
    volatile uint32_t CFGR; //0x04 Clock Configuration Register
    volatile uint32_t CIR; // 0x08 Clock Interrupt Register
    volatile uint32_t APB2RSTR; //0x0C APB2 Peripheral Reset 
    volatile uint32_t APB1RSTR; // 0x10 APB1 Peripheral Reset
    volatile uint32_t AHBENR; // 0x14 AHB Peripheral Clock Enable
    volatile uint32_t APB2ENR; // 0x18 APB2 Peripheral Clock Enable
    volatile uint32_t APB1ENR; // 0x1C APB1 Peripheral Clock Enable
    volatile uint32_t BDCR; // 0x20 Backup Domain Control
    volatile uint32_t CSR; // 0x24 Control/Status Register
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

#define GPIOA               ((GPIO_TypeDef*)0x40010800)
#define RCC                 ((RCC_TypeDef*)0x40021000)
#define SysTick             ((SysTick_TypeDef*)0xE000E010)

// Bit
#define RCC_APB2ENR_IOPAEN  (1 << 2)
#define GPIO_PIN_0          (1 << 0)
#define GPIO_PIN_1          (1 << 1)


#define BTN_PIN             0       // PA0 - button
#define LED_PIN             1       // PA1 - LED
#define DEBOUNCE_MS         20

// Read btn raw (active low)
static inline bool btn_raw(void) {
    return (GPIOA->IDR & GPIO_PIN_0) == 0;
}

typedef enum {
    IDLE,
    BOUNCE_ON,
    PRESSED,
    BOUNCE_OFF
} btn_state_t;

typedef struct {
    btn_state_t state;
    uint16_t timer;
    void (*on_press)(void);
    void (*on_release)(void);
} button_t;

button_t my_button;

void led_on(void) {
    GPIOA->BSRR = GPIO_PIN_1;           // Bat LED
}

void led_off(void) {
    GPIOA->BSRR = (GPIO_PIN_1 << 16);   // Tat LED
}

void btn_init(button_t *btn) {
    btn->state = IDLE;
    btn->timer = 0;
    btn->on_press = led_on;
    btn->on_release = led_off;
}


void btn_debounce_tick(button_t *btn) {
    bool current = btn_raw();

    switch (btn->state) {
        case IDLE:
            if (current) {
                btn->state = BOUNCE_ON;
                btn->timer = DEBOUNCE_MS;
            }
            break;

        case BOUNCE_ON:
            if (--btn->timer == 0) {
                if (current) {
                    btn->state = PRESSED;
                    if (btn->on_press) btn->on_press();
                } else {
                    btn->state = IDLE;
                }
            }
            break;

        case PRESSED:
            if (!current) {
                btn->state = BOUNCE_OFF;
                btn->timer = DEBOUNCE_MS;
            }
            break;

        case BOUNCE_OFF:
            if (--btn->timer == 0) {
                if (!current) {
                    btn->state = IDLE;
                    if (btn->on_release) btn->on_release();
                } else {
                    btn->state = PRESSED;
                }
            }
            break;
    }
}
///SysTick_Handler = "Moi 1ms, kiem tra button 1 lan" do Systick dem 8000 lan (8Mhz -> 1ms) dung de chuyen trang thai FSM
void SysTick_Handler(void) {
    btn_debounce_tick(&my_button);
}

int main(void) {
    // Bat clock GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA0: Input pull-up
    GPIOA->CRL &= ~0xF;
    GPIOA->CRL |= 0x8;          // Input pull-up/down
    GPIOA->ODR |= GPIO_PIN_0;   // Kich hoat pull-up

    // PA1: Output push-pull 2MHz
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x2 << 4);

    // SysTick 1ms 8MHz HSI
    SysTick->LOAD = 8000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x7;  // Enable + interrupt + HCLK High-speed Clock for CPU and AHB bus (bao gom: Core, RAM, Flash, DMA...)

    btn_init(&my_button);

    while (1) {
    }
}