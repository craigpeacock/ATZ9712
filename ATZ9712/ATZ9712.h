/*
ATTEN ATZ9712 DC Load MODBUS Communication Routines
Copyright (C) 2019 Craig Peacock

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#pragma once

#include <Windows.h>
#include <stdint.h>

HANDLE OpenATZ912Port(LPCWSTR PortName);
bool ReadCoil(HANDLE hPort, uint8_t address, uint16_t coil_number, bool *value);
bool ReadRegisterFloat(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, float *value);
bool ReadRegisterInt(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, uint16_t *value);

bool WriteCoil(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, bool value);
bool WriteRegisterFloat(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, float value);
bool WriteRegisterInt(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, uint16_t value);

uint16_t Calc_CRC(LPCVOID pointer, uint16_t length);

#define FUNCTION_READ_COILS							0x01
#define FUNCTION_READ_MULTIPLE_HOLDING_REGISTERS	0x03
#define FUNCTION_WRITE_SINGLE_COIL					0x05
#define FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS	0x10

// Bits (Modbus 'Coils')
#define BIT_PC1			0x500
#define BIT_PC2			0x501
#define BIT_TRIG		0x502
#define BIT_REMOTE		0x503
#define BIT_ISTATE		0x510
#define BIT_TRACK		0x511
#define BIT_MEMORY		0x512
#define BIT_VOICEEN		0x513
#define BIT_CONNECT		0x514
#define BIT_ATESTS		0x515
#define BIT_ATESTUN		0x516
#define BIT_ATESTPASS	0x517
#define BIT_IOVER		0x520
#define BIT_UOVER		0x521
#define BIT_POVER		0x522
#define BIT_HEAT		0x523
#define BIT_REVERSE		0x524
#define BIT_UNREG		0x525
#define BIT_ERREP		0x526
#define BIT_ERRCAL		0x527

// Registers
#define REG_CMD			0x0A00
#define REG_IFIX		0x0A01
#define REG_UFIX		0x0A03
#define REG_PFIX		0x0A05
#define REG_RFIX		0x0A07
#define REG_TMCCS		0x0A09
#define REG_TMCVS		0x0A0B
#define REG_UCCONSET	0x0A0D
#define REG_UCCOFFSET	0x0A0F
#define REG_UCVONSET	0x0A11
#define REG_UCVOFFSET	0x0A13
#define REG_UCPONSET	0x0A15
#define REG_UCPOFFSET	0x0A17
#define REG_UCRONSET	0x0A19
#define REG_UCROFFSET	0x0A1B
#define REG_UCCCV		0x0A1D
#define REG_UCRCV		0x0A1F
#define REG_IA			0x0A21
#define REG_IB			0x0A23
#define REG_TMAWD		0x0A25
#define REG_TMBWD		0x0A27
#define REG_TMTRANSRIS	0x0A29
#define REG_TMTTRANSFAL	0x0A2B
#define REG_MODETRAN	0x0A2D
#define REG_UBATTEND	0x0A2E
#define REG_BATT		0x0A30
#define REG_SERLIST		0x0A32
#define REG_SERATEST	0x0A33
#define REG_IMAX		0x0A34
#define REG_UMAX		0x0A36
#define REG_PMAX		0x0A38
#define REG_ILCAL		0x0A3A
#define REG_IHCAL		0x0A3C
#define REG_ULCAL		0x0A3E
#define REG_UHCAL		0x0A40
#define REG_TAGSCAL		0x0A42
#define REG_U			0x0B00
#define REG_I			0x0B02
#define REG_SETMODE		0x0B04
#define REG_INPUTMODE	0x0B05
#define REG_MODEL		0x0B06
#define REG_EDITION		0x0B07

