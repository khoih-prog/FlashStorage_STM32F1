/******************************************************************************************************************************************
  FlashAsEEPROM_STM32F1.h
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

#ifndef FlashAsEEPROM_STM32F1_h
#define FlashAsEEPROM_STM32F1_h

#if !( defined(STM32F1xx) || defined(STM32F3xx) )
  #error This code is intended to run on STM32F1/F3 platform! Please check your Tools->Board setting.  
#endif

#ifndef EEPROM_EMULATION_SIZE
  #define EEPROM_EMULATION_SIZE ( 1024 - sizeof(bool) - sizeof(uint32_t) )
#endif

const uint32_t STM32F1_EEPROM_EMULATION_SIGNATURE = 0xFEEDDEED;

typedef struct 
{
  byte data[EEPROM_EMULATION_SIZE];
  bool valid;
  uint32_t signature;
} EEPROM_EMULATION;

//////////////////////////////////////////////////////////

class EEPROMClass 
{
  public:
    EEPROMClass();

    /**
     * Read an eeprom cell
     * @param index
     * @return value
     */
    uint8_t read(int address);

    /**
     * Write value to an eeprom cell
     * @param index
     * @param value
     */
    void write(int address, uint8_t value);

    /**
     * Update a eeprom cell
     * @param index
     * @param value
     */
    void update(int address, uint8_t value);
 
    /**
     * Read from eeprom cells to an object
    * @param index
    * @param value
    */
    //Functionality to 'get' data to objects to from EEPROM.
    template< typename T > T& get( int idx, T &t );
    
    /**
    * Read from eeprom cells to an object
    * @param index
    * @param value
    */
    //Functionality to 'get' data to objects to from EEPROM.
    template< typename T > const T& put( int idx, const T &t );
    
    /**
     * Check whether the eeprom data is valid
     * @return true, if eeprom data is valid (has been written at least once), false if not
     */
    bool isValid();

    /**
     * Write previously made eeprom changes to the underlying flash storage
     * Use this with care: Each and every commit will harm the flash and reduce it's lifetime (like with every flash memory)
     */
    void commit();

    uint16_t length() { return EEPROM_EMULATION_SIZE; }
    
    void setCommitASAP(bool value = true) { _commitASAP = value; }
    bool getCommitASAP() { return _commitASAP; }

    void init();
    
  private:
    bool _initialized;
    EEPROM_EMULATION _eeprom;
    bool _dirty;
    bool _commitASAP;
};


#include "FlashAsEEPROM_STM32F1_Impl.h"

#endif    //#ifndef FlashAsEEPROM_STM32F1_h
