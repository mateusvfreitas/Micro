#include <stdint.h>
#include "tm4c1294ncpdt.h"

void PLL_Init(void);
void GPIO_Init(void);
void Enable_Int(void);

const int CONTAGEM = 56e6 - 1;


void enableTimer() {
    SYSCTL_RCGCTIMER_R |= 0x4;
    while(!(SYSCTL_PRTIMER_R & 0x4)) {}
    TIMER2_CTL_R &= ~0x1;
    TIMER2_CFG_R &= ~0x7;
    TIMER2_TAMR_R &= ~0x3;
    TIMER2_TAMR_R |= 0x2;
    TIMER2_TAILR_R = CONTAGEM;
    TIMER2_TAPR_R &= ~0xFF;
    TIMER2_ICR_R |= 0x1;
    TIMER2_IMR_R |= 0x1;
    NVIC_PRI5_R = 4 << 29;
    NVIC_EN0_R = 1 << 23;
//    TIMER2_CTL_R |= 0x1;
}

int main(void) {
    PLL_Init();
    GPIO_Init();
    Enable_Int();
    enableTimer();

    while(1) {
        // loop vazio
    }
}
