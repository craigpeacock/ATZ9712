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

#include "ATZ9712.h"
#include <Windows.h>
#include <stdint.h>
#include <stdio.h>

#pragma pack(1)
struct READ_COIL_REQUEST {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t	coilNumber;
	uint16_t	CRC;
};

struct READ_COIL_RESPONSE {
	uint8_t		address;
	uint8_t		functionCode;
	uint8_t		byteCount;
	uint8_t		coilStatus;
	uint16_t	CRC;
};

struct WRITE_COIL_REQUEST {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t	data;
	uint16_t	CRC;
};

struct WRITE_COIL_RESPONSE {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t	data;
	uint16_t	CRC;
};

struct READ_REG_REQUEST {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t	registersToRead;
	uint16_t	CRC;
};

struct WRITE_REG_FLOAT_REQUEST {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t    noRegistersToWrite;
	uint8_t		byteCount;
	uint8_t		data0;
	uint8_t		data1;
	uint8_t		data2;
	uint8_t		data3;
	uint16_t	CRC;
};

struct WRITE_REG_RESPONSE {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;
	uint16_t	noRegistersToWrite;
	uint16_t	CRC;
};

struct READ_REG_FLOAT_RESPONSE {
	uint8_t		address;
	uint8_t		functionCode;
	uint8_t		byteCount;
	uint8_t		data0;
	uint8_t		data1;
	uint8_t		data2;
	uint8_t		data3;
	uint16_t	CRC;
};

struct WRITE_REG_INT_REQUEST {
	uint8_t		address;
	uint8_t		functionCode;
	uint16_t	reg;					// Address of first holding register to preset/write (16-bit)
	uint16_t    noRegistersToWrite;		// Number of holding registers to preset/write (16-bit)
	uint8_t		byteCount;				// Number of bytes of register values to follow (8-bit)
	uint16_t	data;					// New values of holding registers (16 bits per register)
	uint16_t	CRC;
};

struct READ_REG_INT_RESPONSE {
	uint8_t		address;
	uint8_t		functionCode;
	uint8_t		byteCount;
	uint8_t		data0;
	uint8_t		data1;
	uint16_t	CRC;
};

bool ReadCoil(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, bool *value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	struct READ_COIL_REQUEST request;
	request.address			= dev_address;
	request.functionCode	= FUNCTION_READ_COILS;
	request.reg				= _byteswap_ushort(reg_addr);
	request.coilNumber		= _byteswap_ushort(0x01);
	request.CRC				= Calc_CRC(&request, sizeof(request)-2);
		
	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	}

	Sleep(20);

	struct READ_COIL_RESPONSE response;

	ReadFile(hPort, &response, sizeof(response), &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response)-2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(request)-2));
	}
	   
	*value = response.coilStatus & 0x01;
	
	return(1);
}

bool ReadRegisterFloat(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, float *value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	struct READ_REG_REQUEST request;
	request.address = dev_address;
	request.functionCode = FUNCTION_READ_MULTIPLE_HOLDING_REGISTERS;
	request.reg = _byteswap_ushort(reg_addr);
	request.registersToRead = _byteswap_ushort(0x02);
	request.CRC = Calc_CRC(&request, sizeof(request)-2);

	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	}

	Sleep(20);

	struct READ_REG_FLOAT_RESPONSE response;

	ReadFile(hPort, &response, sizeof(response), &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response)-2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(response)-2));
	}

	if (response.byteCount == 4) {
		uint8_t * float_buf = (uint8_t *)value;
		float_buf[0] = response.data3;
		float_buf[1] = response.data2;
		float_buf[2] = response.data1;
		float_buf[3] = response.data0;
		return(1);
	}
	else
		return(ERROR);
	
}

bool ReadRegisterInt(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, uint16_t *value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	struct READ_REG_REQUEST request;
	request.address = dev_address;
	request.functionCode = FUNCTION_READ_MULTIPLE_HOLDING_REGISTERS;
	request.reg = _byteswap_ushort(reg_addr);
	request.registersToRead = _byteswap_ushort(0x01);
	request.CRC = Calc_CRC(&request, sizeof(request)-2);

	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	}

	Sleep(20);

	struct READ_REG_INT_RESPONSE response;

	ReadFile(hPort, &response, sizeof(response), &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response)-2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(response) - 2));
	}

	//printf(" %02X %02X %d\r\n", response.data0, response.data1, response.byteCount);

	if (response.byteCount == 2) {
		uint8_t * int_buf = (uint8_t *)value;
		int_buf[0] = response.data0;
		int_buf[1] = response.data1;
		return(1);
	}
	else
		return(ERROR);

}

bool WriteCoil(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, bool value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	struct WRITE_COIL_REQUEST request;
	request.address = dev_address;
	request.functionCode = FUNCTION_WRITE_SINGLE_COIL;
	request.reg = _byteswap_ushort(reg_addr);
	if (value) request.data = _byteswap_ushort(0x0001);
	else       request.data = _byteswap_ushort(0x0000);
	request.CRC = Calc_CRC(&request, sizeof(request)-2);

	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	} 

	Sleep(20);

	struct WRITE_COIL_RESPONSE response;

	ReadFile(hPort, &response, 20, &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response)-2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(response)-2));
	}

	printf("Returned %d \r\n", response.data && 0x0100); 

	return(1);
}

bool WriteRegisterFloat(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, float value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	uint8_t * float_buf = (uint8_t *)&value;

	struct WRITE_REG_FLOAT_REQUEST request;
	request.address = dev_address;
	request.functionCode = FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS;
	request.reg = _byteswap_ushort(reg_addr);
	request.noRegistersToWrite = _byteswap_ushort(0x02);
	request.byteCount = 0x04;

	request.data0 = float_buf[3];
	request.data1 = float_buf[2];
	request.data2 = float_buf[1];
	request.data3 = float_buf[0];

	//printf(" %02X %02X %02X %02X \r\n", request.data0, request.data1, request.data2, request.data3);

	request.CRC = Calc_CRC(&request, sizeof(request) - 2);

	//printf(" CRC %04X \r\n", request.CRC);

	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	}

	Sleep(20);

	struct WRITE_REG_RESPONSE response;

	ReadFile(hPort, &response, sizeof(response), &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response) - 2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(response) - 2));
	}

	if (response.noRegistersToWrite == request.noRegistersToWrite) {
		return (1);
	} else {
		return(ERROR);
	}
}

bool WriteRegisterInt(HANDLE hPort, uint8_t dev_address, uint16_t reg_addr, uint16_t value)
{
	DWORD dwBytesWritten;
	DWORD dwBytesRead;

	struct WRITE_REG_INT_REQUEST request;
	request.address = dev_address;
	request.functionCode = FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS;
	request.reg = _byteswap_ushort(reg_addr);
	request.noRegistersToWrite = _byteswap_ushort(0x01);
	request.byteCount = 0x02;
	request.data = _byteswap_ushort(value);
	request.CRC = Calc_CRC(&request, sizeof(request)-2);

	bool retVal = WriteFile(hPort, &request, sizeof(request), &dwBytesWritten, NULL);
	if (dwBytesWritten != sizeof(request)) {
		printf("Error: Request bytes sent not equal to size of request. Size of request = %d bytes. Bytes written = %d bytes.\r\n", (uint8_t) sizeof(request), dwBytesWritten);
		return(ERROR);
	}

	Sleep(20);

	struct WRITE_REG_RESPONSE response;

	ReadFile(hPort, &response, sizeof(response), &dwBytesRead, 0);

	if (dwBytesRead != sizeof(response)) {
		printf("Error: Reponse has a different packet size that expected. Expected size of response %d bytes. Bytes read = %d\r\n", (uint8_t) sizeof(response), dwBytesRead);
		return(ERROR);
	}

	if (response.address != request.address) {
		printf("Warning: Response incorrectly addressed.\r\n");
		return(ERROR);
	}

	if (response.CRC != Calc_CRC(&response, sizeof(response)-2)) {
		printf("Warning: CRC invalid. Received CRC = 0x%04X != Calculated CRC 0x%04X \r\n ", response.CRC, Calc_CRC(&response, sizeof(response) -2));
	}

	if (response.noRegistersToWrite == request.noRegistersToWrite) {
		return (1);
	}
	else {
		return(ERROR);
	}
}

uint16_t Calc_CRC(LPCVOID pointer, uint16_t length)
{
	uint8_t* buffer = (uint8_t*)pointer;

	uint16_t crc = 0xFFFF;

	for (int pos = 0; pos < length; pos++) {
		crc ^= (uint16_t)buffer[pos];          // XOR byte into least sig. byte of crc

		for (int i = 8; i != 0; i--) {    // Loop over each bit
			if ((crc & 0x0001) != 0) {      // If the LSB is set
				crc >>= 1;                    // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else                            // Else LSB is not set
				crc >>= 1;                    // Just shift right
		}
	}
	// Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
	return crc;
}

HANDLE OpenATZ912Port(LPCWSTR PortName)
{
	HANDLE hSerialPort1;

	hSerialPort1 = CreateFile(PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hSerialPort1 == INVALID_HANDLE_VALUE)
	{
		printf("Unable to open serial port\n");
		return 0;
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	bool Status = GetCommState(hSerialPort1, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_9600;	// Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;			// Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;	// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;		// Setting Parity = None
	
	SetCommState(hSerialPort1, &dcbSerialParams);

	// All timeouts in milliseconds
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(hSerialPort1, &timeouts);

	PurgeComm(hSerialPort1, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);

	return (hSerialPort1);
}