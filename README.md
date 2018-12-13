# OPCTunneller
allows OPC (classic) data transfers between computers over TCP/IP without DCOM

operating system: Microsoft Windows7 or greater
build environment: Microsoft Visual Studio 2017

- need OpenSSL -> Win32 OpenSSL 1.0.2l installed
	- User Makro
		- Set OpenSSL_Include_Path To OpenSSl Header Path e.g. "C:\Program Files (x86)\OpenSSL-Win32\include\"
		- Set OpenSSL_Lib_Path To OpenSSl library Path e.g. "C:\Program Files (x86)\OpenSSL-Win32\lib"

- after building: register bin\NKTnlClient(D).dll with regsrv32.exe and call bin\ NkTnlSrv(D).exe -register 
	to add information to the registry for the config tools 

- for testing you need a "classic OPC DA (demo) Server" and a "classic OPC DA (test) Client" 
- require OPC Core Components from www.opcfoundation.org