#include <stdint.h>
#include "tm4c1294ncpdt.h"

void PortN_Output(uint32_t valor);
void PortF_Output(uint32_t valor);

int atual = 0, enabled = 0;

void invertePino0() {
    PortN_Output(atual << 1);
    atual = !atual;
}

void timer2AHandler() {
	TIMER2_ICR_R = 0x01;
	invertePino0();
}



void portJHandler() {
    int interr = GPIO_PORTJ_AHB_RIS_R;

    GPIO_PORTJ_AHB_ICR_R = 0x3;

    if (interr & 0x1) {
        if (enabled) {
            TIMER2_CTL_R &= ~0x1;
        } else {
            TIMER2_CTL_R |= 0x1;
        }
        enabled = !enabled;
    }
}
