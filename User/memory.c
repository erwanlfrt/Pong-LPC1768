#include "global.h"
#include "lpc17xx_i2c.h"

/**
* Init the memory
*/
void init_i2c_eeprom(void) {
	I2C_Init(LPC_I2C0, 500000); //Init I2C0 with a frequency of 500 kHz
	I2C_Cmd(LPC_I2C0, ENABLE); //Enable I2C0
	
}

/**
* Write in memory
* @param addr (uint16_t) - address where to store data (between 0 and 2047)
* @param data (uint8_t*) - data to store
* @param length (int) - length of data.
*/
void i2c_eeprom_write(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type setup;
	
	int slaveAddress = (0XA0 >>1) + (addr>>8); // slave address (01010000) + (00000(A2)(A1)(A0)) -> 3 bits to select the page
	
	int wordAddress = (addr) - ((addr>>8) << 8); // word address on 8 bits wordAddress = wordAddress - (A2A1A0)00000000
	uint8_t newDatas[newLength]; //new array in order to add the word address as first element of written datas. 
	int i;
	for(i = 0 ; i<length + 1; i++){
		if(i==0) {
			newDatas[i] = (uint8_t) wordAddress; //first element of datas is the word address (8 bits)
		}
		else {
			if(data != NULL) {
				newDatas[i] = data[i-1]; //add elements to newDatas.
			}
		}
	}
	
	setup.sl_addr7bit = slaveAddress;
	setup.tx_data = newDatas;
	setup.tx_length = length;
	setup.rx_length = 0;
	setup.retransmissions_max = 1;
	
	I2C_MasterTransferData(LPC_I2C0, &setup, I2C_TRANSFER_POLLING);
}

/**
* read datas from memory
* @param addr (uint16_t) - address in memory where to read.
* @param data (uint8_t*) - parameter where read datas are stored
* param length (uint8_t*) - length of datas to read.
*/
void i2c_eeprom_read(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type setup;
	int slaveAdress = (0XA0 >>1) + (addr>>8); // slave address = (01010000) + (00000(A2)(A1)(A0)) -> 3 bits to select the page
	
	uint8_t datas[1]; //array of one element to write the word address in memory
	datas[0] = (addr) - ((addr>>8) << 8); //store the word address in the first element of the array
	
	setup.sl_addr7bit = slaveAdress;
	setup.tx_data = datas; //write the word address
	setup.tx_length = 1;
	setup.rx_data = data; //then store read datas into data.
	setup.rx_length = length;
	setup.retransmissions_max = 1;
	
	I2C_MasterTransferData(LPC_I2C0, &setup, I2C_TRANSFER_POLLING);
	
}