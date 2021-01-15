/*
 * spi_flash.c
 *
 *  Created on: May 12, 2020
 *      Author: Andrei
 */

#include "spi_flash.h"
#include "main.h"

void selectFlash() {
	HAL_GPIO_WritePin(SPIFLASH_CS_PORT, SPIFLASH_CS_PIN, GPIO_PIN_RESET);
}

void unselectFlash() {
	HAL_GPIO_WritePin(SPIFLASH_CS_PORT, SPIFLASH_CS_PIN, GPIO_PIN_SET);
}

uint8_t receiveFlash(){
	uint8_t byte;
	HAL_SPI_Receive(&SPIFLASH_SPI_DEVICE, &byte, (uint16_t)1, 10);
	return byte;
}

uint8_t transferFlash(uint8_t data){
	HAL_SPI_Transmit(&SPIFLASH_SPI_DEVICE,&data,1,10);
	return 1;
}


uint8_t readFlashStatus()
{
	selectFlash();
	transferFlash(SPIFLASH_STATUSREAD);
	uint8_t status = receiveFlash();
	unselectFlash();
	return status;
}

uint8_t isFlashBusy()
{
	return readFlashStatus() & 1;
}


void commandFlash(uint8_t cmd, uint8_t write){
	if(write){
		commandFlash(SPIFLASH_WRITEENABLE, 0);
		unselectFlash();
	}
	if (cmd != SPIFLASH_WAKE) while(isFlashBusy());
	selectFlash();
	HAL_SPI_Transmit(&SPIFLASH_SPI_DEVICE, &cmd, 1, 10);
}

uint32_t readFlashID(){
	selectFlash();
	commandFlash(SPIFLASH_IDREAD, 0);
	uint32_t id = 0;
	transferFlash(0);
	id |= receiveFlash()<<8;
	id |= receiveFlash();
	unselectFlash();
	return id;
}

uint8_t readByteFlash(uint32_t address){
	selectFlash();
	commandFlash(SPIFLASH_ARRAYREAD, 0);
	transferFlash(address >> 16);
	transferFlash(address >> 8);
	transferFlash(address);
	transferFlash(0);
	uint8_t result = receiveFlash(1);
	unselectFlash();
	return result;
}

void readBytesFlash(uint32_t address, uint8_t* ret, int len){
	selectFlash();
	commandFlash(SPIFLASH_ARRAYREAD, 0);
	transferFlash(address >> 16);
	transferFlash(address >> 8);
	transferFlash(address);
	transferFlash(0);
	//result = HAL_SPI_TransmitReceive(&SPIFLASH_SPI_DEVICE,bs,ret,len,10);
	HAL_SPI_Receive(&SPIFLASH_SPI_DEVICE, ret, len, 100);
	unselectFlash();
	return;
}

void writeByteFlash(uint32_t addr, uint8_t byt) {
	commandFlash(SPIFLASH_BYTEPAGEPROGRAM, 1);  // Byte/Page Program
	transferFlash(addr >> 16);
	transferFlash(addr >> 8);
	transferFlash(addr);
	transferFlash(byt);
	unselectFlash();
}

void writeBytesFlash(uint32_t addr, void* buf, uint16_t len) {
	uint32_t addressOffset = 0;
	uint32_t currentAddress = addr;
	uint16_t currentTransferSize;
	do
	{
		currentAddress = addr + addressOffset;
		currentTransferSize = (len - addressOffset > 256) ? 256 : len-addressOffset;
		commandFlash(SPIFLASH_BYTEPAGEPROGRAM, 1);
		HAL_SPI_Transmit(&SPIFLASH_SPI_DEVICE, (uint8_t*)&currentAddress, 3, 10);
		HAL_SPI_Transmit(&SPIFLASH_SPI_DEVICE, buf + addressOffset, currentTransferSize, 100);
		unselectFlash();
		addressOffset += 256;
	}while(addressOffset < len);

}

void chipEraseFlash() {
	commandFlash(SPIFLASH_CHIPERASE, 1);
	unselectFlash();
}

/// erase a 4Kbyte sector in a block
void sectorErase4KFlash(uint32_t addr) {
	commandFlash(SPIFLASH_BLOCKERASE_4K, 1); // Block Erase
	transferFlash(addr >> 16);
	transferFlash(addr >> 8);
	transferFlash(addr);
	unselectFlash();
}

/// erase a 32Kbyte block
void blockErase32KFlash(uint32_t addr) {
	commandFlash(SPIFLASH_BLOCKERASE_32K, 1); // Block Erase
	transferFlash(addr >> 16);
	transferFlash(addr >> 8);
	transferFlash(addr);
	unselectFlash();
}

/// erase a 64Kbyte block
void blockErase64KFlash(uint32_t addr) {
	commandFlash(SPIFLASH_BLOCKERASE_64K, 1); // Block Erase
	transferFlash(addr >> 16);
	transferFlash(addr >> 8);
	transferFlash(addr);
	unselectFlash();
}

void sleepFlash() {
	commandFlash(SPIFLASH_SLEEP, 0);
	unselectFlash();
}

void wakeUpFlash() {
	commandFlash(SPIFLASH_WAKE, 0);
	unselectFlash();
}
