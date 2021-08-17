/******************************************************************************************************************************************
  FlashStorage_STM32F1.h
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

#ifndef FlashStorage_STM32F1_h
#define FlashStorage_STM32F1_h

#if !( defined(STM32F1xx) || defined(STM32F3xx) )
  #error This code is intended to run on STM32F1/F3 platform! Please check your Tools->Board setting.  
#endif

#define FLASH_STORAGE_STM32F1_VERSION     "FlashStorage_STM32F1 v1.0.0"

#include <Arduino.h>

/////////////////////////////////////////////////////

#ifndef FLASH_DEBUG
  #define FLASH_DEBUG               0
#endif

#if !defined(FLASH_DEBUG_OUTPUT)
  #define FLASH_DEBUG_OUTPUT    Serial
#endif

const char FLASH_MARK[]  = "[FLASH] ";
const char FLASH_SP[]    = " ";

#define FLASH_PRINT          FLASH_DEBUG_OUTPUT.print
#define FLASH_PRINTLN        FLASH_DEBUG_OUTPUT.println
#define FLASH_FLUSH          FLASH_DEBUG_OUTPUT.flush

#define FLASH_PRINT_MARK     FLASH_PRINT(FLASH_MARK)
#define FLASH_PRINT_SP       FLASH_PRINT(FLASH_SP)

/////////////////////////////////////////////////////

#define FLASH_LOGERROR(x)         if(FLASH_DEBUG>0) { FLASH_PRINT("[FLASH] "); FLASH_PRINTLN(x); }
#define FLASH_LOGERROR0(x)        if(FLASH_DEBUG>0) { FLASH_PRINT(x); }
#define FLASH_HEXLOGERROR0(x)     if(FLASH_DEBUG>0) { FLASH_PRINTLN(x, HEX); }
#define FLASH_LOGERROR1(x,y)      if(FLASH_DEBUG>0) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINTLN(y); }
#define FLASH_LOGERROR2(x,y,z)    if(FLASH_DEBUG>0) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINT(y); FLASH_PRINT_SP; FLASH_PRINTLN(z); }
#define FLASH_LOGERROR3(x,y,z,w)  if(FLASH_DEBUG>0) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINT(y); FLASH_PRINT_SP; FLASH_PRINT(z); FLASH_PRINT_SP; FLASH_PRINTLN(w); }

/////////////////////////////////////////////////////

#define FLASH_LOGDEBUG(x)         if(FLASH_DEBUG>1) { FLASH_PRINT("[FLASH] "); FLASH_PRINTLN(x); }
#define FLASH_LOGDEBUG0(x)        if(FLASH_DEBUG>1) { FLASH_PRINT(x); }
#define FLASH_HEXLOGDEBUG0(x)     if(FLASH_DEBUG>1) { FLASH_PRINTLN(x, HEX); }
#define FLASH_LOGDEBUG1(x,y)      if(FLASH_DEBUG>1) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINTLN(y); }
#define FLASH_LOGDEBUG2(x,y,z)    if(FLASH_DEBUG>1) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINT(y); FLASH_PRINT_SP; FLASH_PRINTLN(z); }
#define FLASH_LOGDEBUG3(x,y,z,w)  if(FLASH_DEBUG>1) { FLASH_PRINT("[FLASH] "); FLASH_PRINT(x); FLASH_PRINT_SP; FLASH_PRINT(y); FLASH_PRINT_SP; FLASH_PRINT(z); FLASH_PRINT_SP; FLASH_PRINTLN(w); }


//////////////////////////////////////////////////////////

class FlashClass 
{
public:
  FlashClass(const void *flash_addr = NULL, uint32_t size = 0);

  void write(const void *data) { write(flash_address, data); }
  void erase()                 { erase(flash_address);       }
  void read(void *data)        { read(flash_address, data);  }

  void write(const volatile void *flash_ptr, const void *data);
  
  void read(const volatile void *flash_ptr, void *data);

private:
  void erase(const volatile void *flash_ptr);

  const uint32_t PAGE_SIZE, PAGES, MAX_FLASH, ROW_SIZE;
  const volatile void *flash_address;
  const uint32_t flash_size;
};

//////////////////////////////////////////////////////////

template<class T>
class FlashStorageClass
{
public:
  FlashStorageClass(const void *flash_addr) : flash(flash_addr, sizeof(T)) { };

  // Write data into flash memory
  inline void write(T &data)  { flash.erase(); flash.write(&data); }

  // Read data from flash memory.
  inline void read(T &data)  { flash.read(&data); }

private:
  FlashClass flash;
};

#include "FlashStorage_STM32F1.hpp"
      
#endif    //#ifndef FlashStorage_STM32F1_h
