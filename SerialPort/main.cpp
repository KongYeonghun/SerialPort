#include <stdio.h>
#include <iostream>
#include <windows.h>

#include "SerialPort.h"
 
int main(void)
{
	// memory buffer
	BYTE* rxBuffer = new BYTE[SERIAL_PORT_BUFFER_SIZE];
	BYTE txBuffer[SERIAL_PORT_BUFFER_SIZE];
	

	CString strData = _T("");
	CString strTmp = _T("");

	CSerialPort SerialPort;

	if ( SerialPort.ReadByte( rxBuffer, SERIAL_PORT_BUFFER_SIZE ) )
	{
		rxBuffer[511] = '\0';

		strData = CString( reinterpret_cast<char*>( rxBuffer ) );
		strTmp = _T("DATA :");

		strTmp += strData;
		strTmp += _T("\n");
	}
	else
	{
		::OutputDebugString( _T("Failed to read data\n") );
	}
	
	while(1);

	SerialPort.WriteByte( txBuffer , SERIAL_PORT_BUFFER_SIZE);

	delete [] rxBuffer;
	
	SerialPort.ClosePort();
}
