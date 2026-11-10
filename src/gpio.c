#include "gpio.h"

inline void gpio_set_af(uint16_t pin, uint8_t af)
{
    volatile struct gpio * gp = GPIO(PINBANK(pin));
    int n = PINNO(pin);

    volatile uint32_t * afr_reg = (n < 8) ? &gp->AFRL : &gp->AFRH;

    uint32_t shift = (uint32_t)(n & 7) * 4;

    *afr_reg &= ~(15UL << shift);
    *afr_reg |= ((uint32_t)af) << shift;
}

// Set GPIO pin mode
inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio* gpio = GPIO(PINBANK(pin));  // GPIO bank
    int n = PINNO(pin);                      // Pin number
    gpio->MODER &= ~(3U << (n * 2));         // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2);    // Set new mode
}

inline void gpio_set_pull_up(uint16_t pin)
{
    struct gpio *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->PUPDR &= ~(3U << (n * 2));
    gpio->PUPDR |=  (1U << (n * 2));   // 01 = pull-up
}

// Write value to GPIO pin
inline void gpio_write(uint16_t pin, bool val) {
    struct gpio* gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

inline int gpio_read(uint16_t pin)
{
    struct gpio *gpio = GPIO(PINBANK(pin));
    return (gpio->IDR >> PINNO(pin)) & 1U;
}

/* inline void gpio_set_modes(uint16_t pin[], uint8_t modes, int size) */
/* { */
/*     for (int i = 0; i < size; ++i) */
/*     { */
/*         gpio_set_mode(pin[i], modes); */
/*     } */
/* } */

inline void gpio_set_open_drain(uint16_t pin)
{
    struct gpio * gpio = GPIO(PINBANK(pin));
    gpio->OTYPER |= (1U << PINNO(pin));
}
