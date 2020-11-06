#include "tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void PLL_Init(void);
void GPIO_Init(void);
void PortN_Output(uint32_t valor);

int brilhoLed = 1;
char buf[80];
int aceso = 0;

void outChar(char c) {
    while (UART0_FR_R & UART_FR_TXFF) {}

    UART0_DR_R = c;
}

void print(char toPrint[]) {
    int i;
    for (i = 0; toPrint[i] != 0; ++i) {
        outChar(toPrint[i]);
    }
}

void enableUART() {
    SYSCTL_RCGCUART_R = 0x1;
    while (SYSCTL_PRUART_R & 0x1 == 0) {}
    UART0_CTL_R &= ~UART_CTL_UARTEN;

    UART0_IBRD_R = 260;
    UART0_FBRD_R = 27;

    UART0_LCRH_R = 0x70;
    UART0_CC_R = 0;

    UART0_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);

    SYSCTL_RCGCGPIO_R |= 0x1;
    while (SYSCTL_PRGPIO_R & 0x1 == 0) {}

    GPIO_PORTA_AHB_AMSEL_R = 0x00;
    GPIO_PORTA_AHB_PCTL_R = 0x11;
    GPIO_PORTA_AHB_AFSEL_R = 0X3;
    GPIO_PORTA_AHB_DEN_R = 0x3;
}

void setBrilho(char tecla) {
    switch (tecla) {
    case '0':
        brilhoLed = 1;
        break;
    case '1':
        brilhoLed = 20;
        break;
    case '2':
        brilhoLed = 40;
        break;
    case '3':
        brilhoLed = 60;
        break;
    case '4':
        brilhoLed = 80;
        break;
    case '5':
        brilhoLed = 99;
        break;
    default:
        break;
    }

    sprintf(buf, "\r\nLED a %d%%", brilhoLed);

    print(buf);
}

void inChar() {
    while(UART0_FR_R & UART_FR_RXFE) {}

    char tecla = UART0_DR_R;

    setBrilho(tecla);
}

const int CONTAGEM = 56e6 - 1;

void enableTimer() {
    SYSCTL_RCGCTIMER_R |= 0x4;
    while(!(SYSCTL_PRTIMER_R & 0x4)) {}
    TIMER2_CTL_R &= ~0x1;
    TIMER2_CFG_R &= ~0x7;
    TIMER2_TAMR_R &= ~0x3;
    TIMER2_TAMR_R |= 0x2;
    TIMER2_TAILR_R = 80000 * brilhoLed / 100;
    TIMER2_TAPR_R &= ~0xFF;
    TIMER2_ICR_R |= 0x1;
    TIMER2_IMR_R |= 0x1;
    NVIC_PRI5_R = 4 << 29;
    NVIC_EN0_R = 1 << 23;
    TIMER2_CTL_R |= 0x1;
}

void toggleLed() {
    if (aceso) {
        TIMER2_TAILR_R = 80000 * (100 - brilhoLed) / 100;
    } else {
        TIMER2_TAILR_R = 80000 * brilhoLed / 100;
    }
    aceso = !aceso;
    PortN_Output(aceso);
}

void timer2AHandler() {
    TIMER2_ICR_R = 0x01;
    toggleLed();
}

int main(void)
{
    PLL_Init();
    GPIO_Init();
    enableUART();
    enableTimer();

    print("Start 1%");

    while (1) {
        inChar();
    }

	return 0;
}
