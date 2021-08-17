/******************************************************************************************************************************************
  FlashAsEEPROM_STM32F1_Impl.h
  For STM32F1 using Flash emulated-EEPROM

  The FlashStorage_STM32F1 library aims to provide a convenient way to store and retrieve user's data using the non-volatile flash memory
  of STM32F1. It now supports writing and reading the whole object, not just byte-and-byte.

  Based on and modified from Cristian Maglie's FlashStorage (https://github.com/cmaglie/FlashStorage)

  Built by Khoi Hoang https://github.com/khoih-prog/FlashStorage_STM32F1
  Licensed under LGPLv3 license
  
  Orginally written by A. Christian
  
  Copyright (c) 2015-2016 Arduino LLC.  All right reserved.
  Copyright (c) 2021 Khoi Hoang.
  
  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
  as published bythe Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License along with this library. 
  If not, see (https://www.gnu.org/licenses/)
  
  Version: 1.0.0

  Version Modified By   Date        Comments
  ------- -----------  ----------   -----------
  1.0.0   K Hoang      16/08/2021  Initial coding to support to STM32F1
 ******************************************************************************************************************************************/

#pragma once

#ifndef FlashAsEEPROM_STM32F1_Impl_h
#define FlashAsEEPROM_STM32F1_Impl_h

#define REGISTERED_NUMBER_FLASH_SECTORS       (*((uint16_t *) FLASH_SIZE_DATA_REGISTER))

#if !defined(USING_FLASH_SECTOR_NUMBER)
  #define USING_FLASH_SECTOR_NUMBER           (REGISTERED_NUMBER_FLASH_SECTORS - 1)
#endif

#define START_FLASH_ADDRESS                   0x8000000

#define START_FLASH_STORAGE_ADDRESS           (START_FLASH_ADDRESS + USING_FLASH_SECTOR_NUMBER * 1024) 

FlashStorageClass<EEPROM_EMULATION> eeprom_storage( (const void *) (START_FLASH_STORAGE_ADDRESS) );

//////////////////////////////////////////////////////////

EEPROMClass::EEPROMClass() : _initialized(false), _dirty(false), _commitASAP(true)  
{
  // Empty
}

//////////////////////////////////////////////////////////

uint8_t EEPROMClass::read(int address)
{
  if (!_initialized) 
    init();
    
  return _eeprom.data[address];
}

//////////////////////////////////////////////////////////

void EEPROMClass::update(int address, uint8_t value)
{
  if (!_initialized) 
    init();
    
  if (_eeprom.data[address] != value) 
  {
    _dirty = true;
    _eeprom.data[address] = value;
  }
}

//////////////////////////////////////////////////////////

void EEPROMClass::write(int address, uint8_t value)
{
  update(address, value);
}

//////////////////////////////////////////////////////////

void EEPROMClass::init()
{
  FLASH_LOGDEBUG0(F("Start Flash Address: 0x")); FLASH_HEXLOGDEBUG0((uint32_t) START_FLASH_STORAGE_ADDRESS);
  FLASH_LOGDEBUG1(F("REGISTERED_NUMBER_FLASH_SECTORS (KB) ="), REGISTERED_NUMBER_FLASH_SECTORS);
  FLASH_LOGDEBUG1(F("USING_FLASH_SECTOR_NUMBER = "), USING_FLASH_SECTOR_NUMBER);

  // Use reference
  eeprom_storage.read(_eeprom);
  
  if (_eeprom.signature != STM32F1_EEPROM_EMULATION_SIGNATURE)
  {
    memset(_eeprom.data, 0xFF, EEPROM_EMULATION_SIZE);
    _eeprom.signature = STM32F1_EEPROM_EMULATION_SIGNATURE;
  }
  
  _eeprom.valid = true;
   
  _initialized = true;
}

//////////////////////////////////////////////////////////
 
/**
 * Read from eeprom cells to an object
* @param index
* @param value
*/
//Functionality to 'get' data to objects to from EEPROM.
template< typename T > T& EEPROMClass::get( int idx, T &t )
{       
  // Copy the data from the flash to the buffer if not yet
  if (!_initialized) 
    init();
    
  uint16_t offset = idx;
  uint8_t* _pointer = (uint8_t *) &t;
  
  for ( uint16_t count = sizeof(T) ; count ; --count, ++offset )
  {  
    *_pointer++ = _eeprom.data[offset];
  }
    
  return t;
}

//////////////////////////////////////////////////////////

/**
* Read from eeprom cells to an object
* @param index
* @param value
*/
//Functionality to 'get' data to objects to from EEPROM.
template< typename T > const T& EEPROMClass::put( int idx, const T &t )
{            
  // Copy the data from the flash to the buffer if not yet
  if (!_initialized) 
    init();
  
  uint16_t offset = idx;
     
  const uint8_t* _pointer = (const uint8_t *) &t;
  
  for ( uint16_t count = sizeof(T) ; count ; --count, ++offset )
  {              
    _eeprom.data[offset] = *_pointer++;
  }

  if (_commitASAP)
  {
    _dirty = false;
    _eeprom.valid = true;
    // Save the data from the buffer
    eeprom_storage.write(_eeprom);
  }
  else  
  {
    // Delay saving the data from the buffer to the flash. Just flag and wait for commit() later
    _dirty = true;    
  }
       
  return t;
}

//////////////////////////////////////////////////////////

bool EEPROMClass::isValid()
{
  if (!_initialized) 
    init();
    
  return _eeprom.valid;
}

//////////////////////////////////////////////////////////

void EEPROMClass::commit()
{
  if (!_initialized) 
    init();
    
  if (_dirty) 
  {
    _dirty = false;
    _eeprom.valid = true;
    // Save the data from the buffer
    eeprom_storage.write(_eeprom);
  }
}

//////////////////////////////////////////////////////////

EEPROMClass EEPROM;

#endif    //FlashAsEEPROM_STM32F1_Impl_h
