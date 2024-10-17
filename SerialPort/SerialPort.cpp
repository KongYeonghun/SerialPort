
#include "SerialPort.h"
 
using namespace std;
 
// *******************************************
// Class Constructor
// *******************************************
CSerialPort::CSerialPort()  
{
	ClosePort();
	if ( OpenPort() )
	{
		cout << "Sucess to Open Port" << endl;
		ConfigurePort();
		// Timeout configuration
		SetCommunicationTimeouts( 0, 0, 0, 0, 0 );	
	}
	else
	{
        cout << "Failed to Open Port" << endl;
	}
}
 
 
// *******************************************
// Class Destructor
// *******************************************
CSerialPort::~CSerialPort()
{
    //
}
 
// *******************************************
// Open Port
// *******************************************
bool CSerialPort::OpenPort()
{
    char* cBuf = NULL; 
	cBuf = (char*)malloc(sizeof(char) * 256);

	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("SERIAL_PORT", "Port ", "-", cBuf, 256, ".\\setting.ini");
	CString Port  = cBuf;
	cout << "Port  : "<< Port  << endl; 

    m_hComm = CreateFile( L"//./" + Port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );
 
    if ( m_hComm == INVALID_HANDLE_VALUE )
        return false;
    else
        return true;
}
 
 
// *******************************************
// Configure Port
// *******************************************
bool CSerialPort::ConfigurePort()
{
    char* cBuf = NULL; 
	cBuf = (char*)malloc(sizeof(char) * 256);

    memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("SERIAL_PORT", "BaudRate ", "-", cBuf, 256, ".\\setting.ini");
	int  BaudRate  = atoi(cBuf);
	cout << "BaudRate  : "<< BaudRate  << endl; 


    if ( ( m_bPortReady = GetCommState( m_hComm, &m_dcb ) ) == 0 )
    {
        ::OutputDebugString( _T("GetCommState Error") );
        CloseHandle( m_hComm );
 
        return false;
    }
 
    m_dcb.BaudRate          = BaudRate;
    m_dcb.ByteSize          = 8;
    m_dcb.Parity            = NOPARITY;
    m_dcb.StopBits          = ONESTOPBIT;
    m_dcb.fBinary           = true;
    m_dcb.fDsrSensitivity   = false;
    m_dcb.fParity           = FALSE;
    m_dcb.fOutX             = false;
    m_dcb.fInX              = false;
    m_dcb.fNull             = false;
    m_dcb.fAbortOnError     = true;
    m_dcb.fOutxCtsFlow      = false;
    m_dcb.fOutxDsrFlow      = false;
    m_dcb.fDtrControl       = DTR_CONTROL_DISABLE;
    m_dcb.fDsrSensitivity   = false;
    m_dcb.fRtsControl       = RTS_CONTROL_DISABLE;
    m_dcb.fOutxCtsFlow      = false;
    m_dcb.fOutxCtsFlow      = false;
 
    m_bPortReady = SetCommState( m_hComm, &m_dcb );
 
    if ( m_bPortReady == 0 )
    {
        ::OutputDebugString( _T("SetCommState Error") );
        CloseHandle( m_hComm );
 
        return false;
    }
 
    return true;
}
 
 
// *******************************************
// Set Communication Timeouts
// *******************************************
bool CSerialPort::SetCommunicationTimeouts( DWORD ReadIntervalTimeout,
                                            DWORD ReadTotalTimeoutMultiplier,
                                            DWORD ReadTotalTimeoutConstant,
                                            DWORD WriteTotalTimeoutMultiplier,
                                            DWORD WriteTotalTimeoutConstant )
{
    if ( ( m_bPortReady = GetCommTimeouts( m_hComm, &m_CommTimeouts ) ) == 0 )
        return false;
 
    m_CommTimeouts.ReadIntervalTimeout          = ReadIntervalTimeout;
    m_CommTimeouts.ReadTotalTimeoutConstant     = ReadTotalTimeoutConstant;
    m_CommTimeouts.ReadTotalTimeoutMultiplier   = ReadTotalTimeoutMultiplier;
    m_CommTimeouts.WriteTotalTimeoutConstant    = WriteTotalTimeoutConstant;
    m_CommTimeouts.WriteTotalTimeoutMultiplier  = WriteTotalTimeoutMultiplier;
     
    m_bPortReady = SetCommTimeouts( m_hComm, &m_CommTimeouts );
     
    if ( m_bPortReady == 0 )
    {
        ::OutputDebugString( _T("StCommTimeouts function failed : COM Port Error") );
        CloseHandle( m_hComm );
 
        return false;
    }
 
    return true;
}
 
 
// *******************************************
// Write Byte
// *******************************************
bool CSerialPort::WriteByte( BYTE* mByte , DWORD mByteLength)
{
    int iBytesWritten = 0;
     
    if ( WriteFile( m_hComm, &mByte, mByteLength, &m_iBytesWritten, NULL ) == 0 )
        return false;
    else
        return true;
}
 
 
// *******************************************
// Read Byte
// *******************************************
bool CSerialPort::ReadByte( BYTE &resp )
{
    BYTE rx;
    resp = 0;
 
    DWORD dwBytesTransferred=0;
 
    if ( ReadFile( m_hComm, &rx, 1, &dwBytesTransferred, 0 ) )
    {
        if ( dwBytesTransferred == 1 )
        {
            resp = rx;
            return true;
        }
    }
               
    return false;
}
 
 
// *******************************************
// Read Byte (Overriding)
// *******************************************
bool CSerialPort::ReadByte( BYTE* &resp, UINT size )
{
    DWORD dwBytesTransferred = 0;
 
    if ( ReadFile( m_hComm, resp, size, &dwBytesTransferred, 0 ) )
    {
        if ( dwBytesTransferred == size )
            return true;
    }
 
    return false;
}
 
 
// *******************************************
// Close Port
// *******************************************
void CSerialPort::ClosePort()
{
    CloseHandle( m_hComm );
    return;
}