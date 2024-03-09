#include"GPIODriver.h"

//------------------------------------------------------------------------
void ZeroRegister(GPIO_TypeDef *pGPIOx, uint16_t pinNumber){
	int pinPos = pinNumber * 4;
	
	if(pinNumber > 7) {
		pinPos -= 32;
		pGPIOx -> CRH &=~ (1 << pinPos);
		pGPIOx -> CRH &=~ (1 << (pinPos + 1) );
		pGPIOx -> CRH &=~ (1 <<  pinPos + 2  );
		pGPIOx -> CRH &=~ (1 << (pinPos + 3) );
	}
	
	else {
		pGPIOx -> CRL &=~ (1 <<  pinPos);
		pGPIOx -> CRL &=~ (1 << (pinPos + 1) );
		pGPIOx -> CRL &=~ (1 <<  pinPos + 2);
		pGPIOx -> CRL &=~ (1 << (pinPos + 3) );

	} 
}
//------------------------------------------------------------------------
void pinMode(uint16_t pinNumber, uint16_t pinMode, uint32_t pinSpeed) {
	
	GPIO_TypeDef *pGPIOx;
	// Port and pin parser
	if(pinNumber < 16) {pGPIOx = GPIOA;}
	else if(pinNumber < 32) {pGPIOx = GPIOB;pinNumber -= 16;}
	else {pGPIOx = GPIOC;pinNumber -= 19;}
	//-----Port Activation------------
	if(pGPIOx == GPIOA) {
		RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	}
	
	else if(pGPIOx == GPIOB) {
		RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	
	else if(pGPIOx == GPIOC) {
		RCC -> APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	//---------------------------------
	int pinPos = pinNumber * 4;
		// pinMode = 1 is INPUT
		if(pinMode == INPUT) {
			ZeroRegister(pGPIOx,pinNumber);
			if (pinNumber > 7){
				pinPos -= 32;
				pGPIOx -> CRH |= (1 << pinPos + 3 );
			}
			
			else {
				pGPIOx -> CRL |= (1 << pinPos+3 );
			}
			
		}
		// pinMode = 2 is OUTPUT
		if(pinMode == OUTPUT) {
			ZeroRegister(pGPIOx,pinNumber);
			if (pinNumber > 7) {
				pinPos -= 32;
				pGPIOx -> CRH |= ( pinSpeed << pinPos );
			}
			else {
				pGPIOx -> CRL |= ( pinSpeed << pinPos );
			}
			
		}
}

//----------------------------Digital Write-----------------------------
void digitalWrite(uint16_t pinNumber, int Value) {
	//-------------Parser----------------
	GPIO_TypeDef *pGPIOx;
	if(pinNumber < 8) {pGPIOx = GPIOA;}
	else if(pinNumber < 16) {pGPIOx = GPIOA;}
	else if(pinNumber < 32) {pGPIOx = GPIOB;pinNumber -= 16;}
	else {pGPIOx = GPIOC; pinNumber -= 19;}
	//-------------------------------------
	
	if(Value) {
		pGPIOx -> ODR |= (Value << pinNumber);
	}
	else {
		pGPIOx -> ODR &=~ (Value << pinNumber);
	}
}

//------------------------------Digital Read--------------------------
bool digitalRead(uint16_t pinNumber) {
	//-------------Parser----------------
	GPIO_TypeDef *pGPIOx;
	if(pinNumber < 8) {pGPIOx = GPIOA;}
	else if(pinNumber < 16) {pGPIOx = GPIOA;}
	else if(pinNumber < 32) {pGPIOx = GPIOB;pinNumber -= 16;}
	else {pGPIOx = GPIOC; pinNumber -= 19;}
	//-------------------------------------
	 
	if(pGPIOx -> IDR & (1<<pinNumber)) {
		return HIGH;
	}
	else
	{
	return LOW;
	}
}

//------------------------------toggleOutputPin-------------------------

void toggleOutputPin(uint16_t pinNumber) {
	//-------------Parser----------------
	GPIO_TypeDef *pGPIOx;
	if(pinNumber < 8) {pGPIOx = GPIOA;}
	else if(pinNumber < 16) {pGPIOx = GPIOA;}
	else if(pinNumber < 32) {pGPIOx = GPIOB;pinNumber -= 16;}
	else {pGPIOx = GPIOC; pinNumber -= 19;}
	//-------------------------------------
	
	pGPIOx -> ODR ^=  (1 << pinNumber);
	

}
