#include <stdint.h>

#define PISCA_PERIODO 250

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void PortF_Output(uint32_t leds);
void Pisca_leds(void);
void Enable_Int(void);
uint32_t flag = 0;

enum semState {VM_VM_1, VM_VD, VM_AM, VM_VM_2, VD_VM, AM_VM, VM_VM_P};

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	Enable_Int();

	enum semState estado = VM_VM_1;
	while (1)
	{
		switch (estado) {
		case VM_VM_1: {
		    PortN_Output(0x3);
		    PortF_Output(0x11);
		    SysTick_Wait1ms(1000);
		    estado = VM_VD;
		    break;
		}
		case VM_VD: {
            PortN_Output(0x3);
            PortF_Output(0x10);
            SysTick_Wait1ms(6000);
            estado = VM_AM;
            break;
        }
		case VM_AM: {
            PortN_Output(0x3);
            PortF_Output(0x1);
            SysTick_Wait1ms(2000);
            estado = VM_VM_2;
            break;
        }
		case VM_VM_2: {
            PortN_Output(0x3);
            PortF_Output(0x11);
            SysTick_Wait1ms(1000);
            estado = VD_VM;
            break;
        }
		case VD_VM: {
            PortN_Output(0x2);
            PortF_Output(0x11);
            SysTick_Wait1ms(6000);
            estado = AM_VM;
            break;
        }
		case AM_VM: {
            PortN_Output(0x1);
            PortF_Output(0x11);
            SysTick_Wait1ms(2000);
            if (flag) {
                estado = VM_VM_P;
            }
            else {
                estado = VM_VM_1;
            }
            break;
        }
		case VM_VM_P: {
		    uint32_t loops = 5000/(2*PISCA_PERIODO);
		    while (loops--) {
		        Pisca_leds();
		    }
		    flag = 0;
            estado = VM_VD;
            break;
        }
		}
	}
}

void flagHandler(void) {
    flag = 1;
}

void Pisca_leds(void)
{
	PortN_Output(0x2);
	PortF_Output(0x1);
	SysTick_Wait1ms(PISCA_PERIODO);
	PortN_Output(0x1);
	PortF_Output(0x10);
	SysTick_Wait1ms(PISCA_PERIODO);
}
