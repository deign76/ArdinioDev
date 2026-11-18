#include "start.h"
#include <stdint.h>

#define port1reg1 *(volatile uint32_t *)0x40040020u
#define port1reg2 (const volatile uint32_t *)0x40040024u

#define INTERNAL_LED = 102;

#define DEMCR 0xE000EDFC
#define DWT_CTRL 0xE0001000
#define DWT_CYCCNT *(volatile uint32_t *)0xE0001004

#define CC_MS 48000u

#define MSTPCRB (volatile uint32_t *)0x40047000
#define MSTP_SCI9 (1u << 22)

#define SCI9_SCR (volatile uint8_t *)0x40070122
#define SCR_ENABLETXRX 0b00110000  

#define SCI9_SMR (volatile uint8_t *)0x40070120

#define SCI9_SCMR (volatile uint8_t *)0x40070126
#define SCMR_CHR1_OFFSET (1u << 4)
#define SCMR_BCP2_OFFSET (1u << 7)

#define SCI9_SEMR (volatile uint8_t *)0x40070127
#define SEMR_BRME_OFFSET (1u << 2)
#define SEMR_RXDESEL_OFFSET (1u << 7)

#define SCI9_BRR (volatile uint8_t *)0x40070121
#define BRR_OFFSET 0b10001100

#define SCI9_MDDR (volatile uint8_t*)0x40070132
#define MDDR_OFFSET 0b11100111

#define PWPR (volatile uint8_t*)0x40040D03
#define PFSR_P109 (volatile uint32_t*)0x40040864
#define PFSR_P110 (volatile uint32_t*)0x40040868
#define PFSR_PMR_OFFSET 1u << 16
#define PFSR_PSEL_OFFSET 0b00101 << 24



void setup() {

	startCycleCounter();

	uint64_t cycleCount = 0;
	uint32_t previous = DWT_CYCCNT;

	cycleCount += DWT_CYCCNT - previous;

	startSerialAT9600();

	while (1) {

		if ((*(volatile const uint8_t *)0x40070124u & (1u << 7)) != 0u) {
			*(volatile uint8_t *)0x40070123u = 'b';
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

void startCycleCounter() {
	*(volatile uint32_t *)DEMCR |= (1u << 24);
	*(volatile uint32_t *)DWT_CTRL |= 1;
}

void startSerialAT9600(){
  *MSTPCRB &= ~(MSTP_SCI9); // Start SCI9.
	(void)*(volatile uint32_t *)MSTPCRB;

	*SCI9_SCR = 0x0u;

	*SCI9_SMR = 0x0u;  
	*SCI9_SCMR |= SCMR_CHR1_OFFSET; 
	*SCI9_SCMR |= SCMR_BCP2_OFFSET;

	*SCI9_SEMR |= SEMR_BRME_OFFSET;
	*SCI9_SEMR |= SEMR_RXDESEL_OFFSET;

	*SCI9_BRR = BRR_OFFSET;
	*SCI9_MDDR = MDDR_OFFSET;

	*(volatile uint8_t *)PWPR = 0u;
	*(volatile uint8_t *)PWPR = (1u<<6);

  

	*PFSR_P109 = 0u;       // Clear PMR on P109.
	*PFSR_P110 = 0u;       // Clear PMR on P110.

	*PFSR_P109 = PFSR_PSEL_OFFSET; // P109: SCI9 TX.
	*PFSR_P110 = PFSR_PSEL_OFFSET; // P110: SCI9 RX.

	*PFSR_P109 |= PFSR_PMR_OFFSET;
	*PFSR_P110 |= PFSR_PMR_OFFSET;

	*PWPR = 0b10000000;

	*SCI9_SCR = SCR_ENABLETXRX; // Enable TX and RX.
}