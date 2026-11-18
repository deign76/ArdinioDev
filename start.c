#include "start.h"
#include <stdint.h>

#define port1reg1 *(volatile uint32_t *)0x40040020u
#define port1reg2 (const volatile uint32_t *)0x40040024u

#define INTERNAL_LED = 102;

#define DEMCR 0xE000EDFC
#define DWT_CTRL 0xE0001000
#define DWT_CYCCNT *(volatile uint32_t *)0xE0001004

#define CC_MS 48000u

void setup() {

  *(volatile uint32_t *)DEMCR |= (1u << 24);
  *(volatile uint32_t *)DWT_CTRL |= 1;

  uint64_t cycleCount = 0;
  uint32_t previous = DWT_CYCCNT;

  cycleCount += DWT_CYCCNT - previous;

  // sercial

//   *(volatile uint8_t *)0x40070042u = 0u; // SCR: disable TX/RX during setup.
//   *(volatile uint8_t *)0x40070040u = 0u; // SMR: UART, 8N1.
//   *(volatile uint8_t *)0x40070047u = 0u; // SEMR: standard baud clock.
//   *(volatile uint8_t *)0x40047000u &= ~(1u << 29);
//   *(volatile uint8_t *)0x40070041 |= 155u;

//   *(volatile uint8_t *)0x40040D03u = 0u;
//   *(volatile uint8_t *)0x40040D03u |= (1u << 6);

//   *(volatile uint8_t *)0x400408C4u |= (1u << 26);
//   *(volatile uint8_t *)0x400408C8u |= (1u << 26);

//   *(volatile uint8_t *)0x400408C4u |= (1u << 16);
//   *(volatile uint8_t *)0x400408C8u |= (1u << 16);

//   *(volatile uint8_t *)0x40040D03u = 0u;
//   *(volatile uint8_t *)0x40040D03u |= (1u << 7);

//   *(volatile uint8_t *)0x40070042 |= (1u << 5);

// *(volatile uint32_t *)0x40047000u &= ~(1u << 29); // Start SCI2.
// (void)*(volatile uint32_t *)0x40047000u;

// *(volatile uint8_t *)0x40070042u = 0u;   // SCR: disable TX/RX during setup.
// *(volatile uint8_t *)0x40070040u = 0u;   // SMR: UART, 8N1.
// *(volatile uint8_t *)0x40070047u = 0u;   // SEMR: standard baud clock.
// *(volatile uint8_t *)0x40070041u = 155u; // BRR: about 9600 baud at 48 MHz PCLKA.

// *(volatile uint8_t *)0x40040D03u = 0u;
// *(volatile uint8_t *)0x40040D03u = 0x40u;

// *(volatile uint32_t *)0x400408C8u = 4u << 24;  // P302/D1: select SCI2 TX.
// *(volatile uint32_t *)0x400408C8u |= 1u << 16; // Enable peripheral mode.

// *(volatile uint8_t *)0x40040D03u = 0u;
// *(volatile uint8_t *)0x40040D03u = 0x80u;

// *(volatile uint8_t *)0x40070042u = 1u << 5; // SCR: enable TX.

    *(volatile uint32_t *)0x40047000u &= ~(1u << 22); // Start SCI9.
    (void)*(volatile uint32_t *)0x40047000u;

    *(volatile uint8_t *)0x40070122u = 0x00u; // SCR: disable TX and RX.
*(volatile uint8_t *)0x40070120u = 0x00u; // SMR: 8N1.
*(volatile uint8_t *)0x40070126u = 0xF2u; // SCMR
*(volatile uint8_t *)0x40070127u = 0x84u; // SEMR: enable baud modulation.
*(volatile uint8_t *)0x40070121u = 0x8Cu; // BRR
*(volatile uint8_t *)0x40070132u = 0xE7u; // MDDR

    *(volatile uint8_t *)0x40040D03u = 0u;
    *(volatile uint8_t *)0x40040D03u = 0x40u; // Unlock pin configuration.

    *(volatile uint32_t *)0x40040864u = 0u; // Clear PMR on P109.
    *(volatile uint32_t *)0x40040868u = 0u; // Clear PMR on P110.
    *(volatile uint32_t *)0x40040864u = 5u << 24; // P109: SCI9 TX.
    *(volatile uint32_t *)0x40040868u = 5u << 24; // P110: SCI9 RX.
    *(volatile uint32_t *)0x40040864u |= 1u << 16;
    *(volatile uint32_t *)0x40040868u |= 1u << 16;

    *(volatile uint8_t *)0x40040D03u = 0u;
    *(volatile uint8_t *)0x40040D03u = 0x80u; // Lock pin configuration.

    *(volatile uint8_t *)0x40070122u = 0x30u; // Enable TX and RX.

  // // *(volatile int32_t*)0x40070000 |= (1u<<0);
  // // *(volatile int32_t*)0x40070000 |= (1u<<1);

  // *(volatile uint32_t*)0x40070046 |= (1u<<0);

  // *(volatile uint32_t*)0x40070041 |= 9600;

  // *(volatile int32_t*)0x40070006 |= (0u<<0);

  // setPinWrtie(102,1);

  // setPortPinDir(1,3,0);
  //   setPinDir(103, 0);

  //   unsigned i = (*port1reg2 >> 3u) & 1u;

  //    *(volatile uint32_t*)0x40070043 = 1;

  while (1) {

    if ((*(volatile const uint8_t *)0x40070124u & (1u << 7)) != 0u) {
    *(volatile uint8_t *)0x40070123u = 'f';
}

    if (cycleCount / CC_MS > 500) {
            // *(volatile uint8_t *)0x40070043 = 'f';

      setPinWrtie(102, 1);
      if (cycleCount / CC_MS > 502) {
            

        cycleCount = 0;
      }
    } else {
      setPinWrtie(102, 0);
    }

    cycleCount += DWT_CYCCNT - previous;
    previous = DWT_CYCCNT;
  }
}

uint8_t setPortPinDir(uint32_t port, uint8_t pin, uint8_t isOutput) {
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040000u + (0x20 * (uintptr_t)port));
  return isOutput ? (*newPort |= (1u << pin)) : (*newPort &= ~(1u << pin));
}

uint8_t setPinDir(uint8_t pin, uint8_t isOutput) {
  uint8_t port = pin / 100;
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040000u + (0x20 * (uintptr_t)port));
  return isOutput ? (*newPort |= (1u << ((pin % 100))))
                  : (*newPort &= ~(1u << ((pin % 100))));
}

uint8_t setPortPinWrtie(uint32_t port, uint8_t pin, uint8_t Output) {
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040000u + (0x20 * (uintptr_t)port));
  return Output ? (*newPort |= (1u << (pin + 16)))
                : (*newPort &= ~(1u << (pin + 16)));
}

uint8_t setPinWrtie(uint32_t pin, uint8_t Output) {
  uint8_t port = pin / 100;
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040000u + (0x20 * (uintptr_t)port));
  return Output ? (*newPort |= (1u << ((pin % 100) + 16)))
                : (*newPort &= ~(1u << ((pin % 100) + 16)));
}

uint8_t readPortPin(uint32_t port, uint8_t pin) {
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040004u + (0x20 * (uintptr_t)port));
  return (*newPort >> pin) & 1u;
}

uint8_t readPin(uint32_t pin) {
  uint8_t port = pin / 100;
  volatile uint32_t *newPort =
      (volatile uint32_t *)(0x40040004u + (0x20 * (uintptr_t)port));
  return (*newPort >> (pin % 100)) & 1u;
}