/*
ATTEN ATZ9712 DC Load Example
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

#include "ATZ9712.h"
#include <Windows.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
	printf("ATZ9712 Example\r\nwww.beyondlogic.org\r\n");

	HANDLE hDCLoad = OpenATZ912Port(TEXT("COM5"));

	bool state;
	float value, voltage, current;
	uint16_t command;

	printf("\r\nMaximum Settings:\r\n");
	ReadRegisterFloat(hDCLoad, 1, REG_IMAX, &value);
	printf("I-MAX %f\r\n", value);
	ReadRegisterFloat(hDCLoad, 1, REG_UMAX, &value);
	printf("V-MAX %f\r\n", value);
	ReadRegisterFloat(hDCLoad, 1, REG_PMAX, &value);
	printf("P-MAX %f\r\n", value);

	printf("\r\nPresent Settings:\r\n");
	ReadRegisterFloat(hDCLoad, 1, REG_IFIX, &value);
	printf("I-Set %f\r\n", value);
	ReadRegisterFloat(hDCLoad, 1, REG_UFIX, &value);
	printf("V-Set %f\r\n", value);
	ReadRegisterFloat(hDCLoad, 1, REG_PFIX, &value);
	printf("P-Set %f\r\n", value);
	ReadRegisterFloat(hDCLoad, 1, REG_RFIX, &value);
	printf("R-Set %f\r\n", value);

	printf("\r\nCurrent Mode: ");
	ReadRegisterInt(hDCLoad, 1, REG_SETMODE, &command);
	uint8_t mode = (command & 0xFF00) >> 8;
	switch (mode) {
	case 1: printf("Constant Current Mode\r\n");
		break;
	case 2: printf("Constant Voltage Mode\r\n");
		break;
	case 3: printf("Constant Power Mode\r\n");
		break;
	case 4: printf("Constant Resistance Mode\r\n");
		break;
	case 26: printf("Short Circuit Mode\r\n");
		break;
	case 38: printf("Battery Test Mode\r\n");
		break;
	default: printf("Unknown Mode %02X\r\n", mode);
	}

	ReadCoil(hDCLoad, 1, BIT_REMOTE, &state);
	printf("Terminal select = %s\r\n", state ? "REAR" : "FRONT");

	ReadCoil(hDCLoad, 1, BIT_PC1, &state);
	printf("Remote Control (PC1) = %s\r\n", state ? "ON" : "OFF");

	ReadCoil(hDCLoad, 1, BIT_ISTATE, &state);
	printf("Input = %s\r\n", state ? "ON" : "OFF");

	// Set Current 
	WriteRegisterFloat(hDCLoad, 1, REG_IFIX, 2.467);
	// Switch to CC;
	WriteRegisterInt(hDCLoad, 1, REG_CMD, 0x0001);

	ReadRegisterFloat(hDCLoad, 1, REG_U, &voltage);
	printf("Voltage %fV\r\n", voltage);

	ReadRegisterFloat(hDCLoad, 1, REG_I, &current);
	printf("Current %fA\r\n", current);

	// Close serial/USB port. 
	CloseHandle(hDCLoad);
}