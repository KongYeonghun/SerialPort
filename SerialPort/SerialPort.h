#pragma once
 
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <atlstr.h>


#define SERIAL_PORT_BUFFER_SIZE 512

using namespace std;

class CSerialPort
{
public:
    CSerialPort()  ;
    virtual ~CSerialPort();
    bool OpenPort();
    bool ConfigurePort();
    bool SetCommunicationTimeouts( DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant );
    bool WriteByte( BYTE* mByte , DWORD mByteLength);
    bool ReadByte( BYTE &resp );
    bool ReadByte( BYTE* &resp, UINT size );
    void ClosePort();

private:
    HANDLE          m_hComm;
    DCB             m_dcb;
    COMMTIMEOUTS    m_CommTimeouts;
    BOOL            m_bPortReady;
    BOOL            m_bWriteRC;
    BOOL            m_bReadRC;
    DWORD           m_iBytesWritten;
    DWORD           m_iBytesRead;
    DWORD           m_dwBytesRead;
};
