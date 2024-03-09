#include "stm32f10x.h"                  // Device header
#include "GPIODriver.h"

char data[32];

void reset(char data[32]){
	for(int i = 0;i < 33;i++) {data[i] = 0;}
}

void setupUSB();

int main(void) {
	setupUSB();
	//=================================
	int i=0;
	bool resetuj=false;
	int pm25,checksum;
	
for(;;) {
	
		if(USART1->SR& USART_SR_RXNE) {
			
			if(USART1 -> DR == 0x42) { //kad detektujemo hex 0x42 reset niz
				i = 0;
				data[i] = USART1->DR; 
				i++;
				while(!(USART1 -> SR & USART_SR_TC)){;}
					
				for(;;) { 
					if(USART1->SR& USART_SR_RXNE) {
						data[i] = USART1->DR; // Kupi bajt i cuva 
						i++; 
						while(!(USART1 -> SR & USART_SR_TC)){;}
						
						if(i == 32) {
							checksum = 0;
							pm25=0;
							
							if(data[0] == 0x42 && data[1] == 0x4d) {
								for(int j = 0; j < 31;j++){
									checksum += data[j];
								}
								
								if(checksum == ((data[30] << 8) + data[31])) {
									pm25 = (data[12] << 8) + data[13]; 
								}
							}
						
							break;
						}
					}
				}
			}
		}
		

		
			
	}

}


void setupUSB() {
  RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	
	ZeroRegister(GPIOA,A9);
	GPIOA -> CRH |= (11 << 4);
	
	// Enable USB
	USART1 -> BRR = 0x1D4C;
	
	USART1 -> CR1 |= (1 << 2);
	USART1 -> CR1 |= (1 << 3);
	USART1 -> CR1 |= (1 << 13);
	

}