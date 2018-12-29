#### OPCTunneller
Allows OPC (classic) data transfers between computers over TCP/IP without DCOM
##### Components
- Server side
	- NKOpcTnlSrv.exe (Service) 
		- (ssl) socket server listen for incomming client calls
		- Interact with the OPC servers and serialize in/out COM function calls from/to TCP/IP stream
	- NkTnlServerCnf.exe
		- Configuration Tool: change server properties; interact with the service 
- Client side
	- NKTnlClient.dll
		- COM server component and proxy for the remote OPC Server
		- For the OPC Client is the network connection fully transparent 
	- NkTnlClientCnf.exe
		- Manage the tunnel connections
		- Select remote OPC Servers
		- Test connections
##### Operating System
- Microsoft Windows7 or (maybe) greater
##### Build Environment
- Microsoft Visual Studio 2017
- OpenSSL 1.0.2x (x86) installed e.g. -> Win32 OpenSSL	
	- User Makro
		- Set OpenSSL_Include_Path to OpenSSl Header Path e.g. "C:\Program Files (x86)\OpenSSL-Win32\include\"
		- Set OpenSSL_Lib_Path to OpenSSl library Path e.g. "C:\Program Files (x86)\OpenSSL-Win32\lib"
- Register bin\NKTnlClient(D).dll with regsrv32.exe and call bin\ NkTnlSrv(D).exe -register 
to add information to the registry for the configuration tools 
##### Dependencies
- [Microsoft Visual C++ Redistributable 2017 (x86) (VC++ 14.0)](https://support.microsoft.com/de-de/help/2977003/the-latest-supported-visual-c-downloads)
- [Microsoft Visual C++ Redistributable 2013 (x86) (VC++ 12.0)](https://support.microsoft.com/de-de/help/2977003/the-latest-supported-visual-c-downloads)
- OpenSSL runtime DLL's
- For testing you need a "classic OPC DA (demo) Server" and a "classic OPC DA (test) Client" 
- OPC Core Components from www.opcfoundation.org
