	.thumb
    .align 2
    .text                 ; Instru��es do tipo Thumb-2

GPIO_PORTJ_AHB_IS_R     .field 0x40060404
GPIO_PORTJ_AHB_IBE_R    .field 0x40060408
GPIO_PORTJ_AHB_IM_R     .field 0x40060410
GPIO_PORTJ_AHB_IEV_R    .field 0x4006040C
GPIO_PORTJ_AHB_ICR_R    .field 0x4006041C
GPIO_PORTJ_AHB_RIS_R    .field 0x40060414
GPIO_PORTJ_AHB_MIS_R    .field 0x40060418

NVIC_EN1_R              .field 0xE000E104
NVIC_PRI12_R            .field 0xE000E430


	.export Enable_Int
	.export GPIOPortJ_Handler
	.import PortN_Output
	.import flagHandler

Enable_Int
	; Desabilita Interrupcoes J0 e J1
	LDR R1, GPIO_PORTJ_AHB_IM_R
	LDR R0, [R1]
	BIC R0, #0x3
	STR R0, [R1]
	; Configura como borda J0 e J1
	LDR R1, GPIO_PORTJ_AHB_IS_R
	LDR R0, [R1]
	BIC R0, #0x3
	STR R0, [R1]
	; Configura como borda unica J0 e J1
	LDR R1, GPIO_PORTJ_AHB_IBE_R
	LDR R0, [R1]
	BIC R0, #0x3
	STR R0, [R1]
	; Configura como de descida J0 e subida J1
	LDR R1, GPIO_PORTJ_AHB_IEV_R
	LDR R0, [R1]
	BIC R0, #0x3
	ORR R0, #0x2
	STR R0, [R1]
	; Realiza ack no GPIOICR
	LDR R1, GPIO_PORTJ_AHB_ICR_R
	LDR R0, [R1]
	ORR R0, #0x3
	STR R0, [R1]
	; Reabilita interrupcao para J0 e J1
	LDR R1, GPIO_PORTJ_AHB_IM_R
	LDR R0, [R1]
	ORR R0, #0x3
	STR R0, [R1]

	; Ativa fonte de interupcao NVIC
	LDR R1, NVIC_EN1_R
	LDR R0, [R1]
	ORR R0, #0x80000
	STR R0, [R1]
	; Configurar prioridade da interrupcao no NVIC
	LDR R1, NVIC_PRI12_R
	LDR R0, [R1]
	BIC R0, #0xE0000000
	ORR R0, #0xA0000000
	STR R0, [R1]

	BX LR




GPIOPortJ_Handler
	PUSH {LR}
	; recebe interrupcao
	LDR R11, GPIO_PORTJ_AHB_RIS_R
	LDR R5, [R11]
	; Realiza ack no GPIOICR
	LDR R1, GPIO_PORTJ_AHB_ICR_R
	LDR R0, [R1]
	ORR R0, #0x3
	STR R0, [R1]
	; testa J0
	ANDS R2, R5, #1
	IT NE
		BLNE flagHandler


	POP {LR}
	BX LR



    .align                           ; garante que o fim da se��o est� alinhada
    .end                             ; fim do arquivo
