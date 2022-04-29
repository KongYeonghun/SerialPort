#include <atlstr.h>

#include "SerialPort.h"

int main()
{
	CSerialPort _serial;

	_serial.ClosePort();

	if (_serial.OpenPort(L"COM3"))
	{
		::OutputDebugString(_T("Succeded to Open Port\n"));
		_serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT);

		// Timeout configuration
		_serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

		// memory buffer
		BYTE* pByte = new BYTE[512];

		CString strData = _T("");
		CString strTmp = _T("");

		if (_serial.ReadByte(pByte, 512))
		{
			pByte[511] = '\0';

			strData = CString(reinterpret_cast<char*>(pByte));
			strTmp = _T("DATA :");

			strTmp += strData;
			strTmp += _T("\n");

			::OutputDebugString(strTmp);
		}
		else
		{
			::OutputDebugString(_T("Failed to read data\n"));
		}

		delete[] pByte;
	}
	else
	{
		::OutputDebugString(_T("Failed to Open Port\n"));
	}

	_serial.ClosePort();
}