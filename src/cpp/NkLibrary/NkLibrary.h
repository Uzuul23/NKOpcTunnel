/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once

#include "error/error.h"
#include "error/exception.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "error/winapiexception.h"
#include "trace/trace.h"
#include "trace/logfile.h"
#include "windows/globalmem.h"
#include "windows/registry.h"
#include "types/conversion.h"
#include "types/String.h"
#include "types/uniquepointer.h"
#include "types/conversion.h"
#include "threading/event.h"
#include "threading/lock.h"
#include "threading/mutex.h"
#include "threading/workerthread.h"
#include "socket/socket.h"
#include "socket/socketlistener.h"
#include "stream/stream.h"
#include "stream/mtiodevice.h"
#include "com/bstr.h"
#include "com/comstring.h"
#include "com/comptr.h"
#include "com/variant.h"
#include "com/safearray.h"
#include "com/vector.h"
#include "com/module.h"
#include "com/enumfar.h"
#include "com/enumnear.h"
#include "com/classfactoryfar.h"
#include "com/classfactorynear.h"
#include "com/connectionpointfar.h"
#include "com/connectionpointnear.h"
#include "com/connectionpointcontainerfar.h"
#include "com/connectionpointcontainernear.h"
#include "com/comfarsrv.h"
#include "com/comnearsrv.h"
#include "opc/opcfarsrv.h"
#include "opc/opcnearsrv.h"
#include "opc/opctypes.h"
#include "opc/opcadvisesinkfar.h"
#include "opc/opcadvisesinknear.h"
#include "opc/opcdatacallbackfar.h"
#include "opc/opcdatacallbacknear.h"
#include "opc/opcserverfar.h"
#include "opc/opcservernear.h"
#include "opc/opcserverlist2far.h"
#include "opc/opcserverlist2near.h"
#include "opc/opcshutdownfar.h"
#include "opc/opcshutdownnear.h"
#include "opc/opcgroupfar.h"
#include "opc/opcgroupnear.h"
#include "opc/opcregistry.h"
#include "util/crc.h"
#include "util/passvault.h"
#include "windows/localmachine.h"
#include "service/ServiceBase.h"
#include "service/ServiceManager.h"

#pragma comment(lib, "Iphlpapi.lib")	
#pragma comment(lib, "Credui.lib")
#pragma comment(lib, "Userenv.lib")

#if defined (_DEBUG)
	#pragma comment(lib, "lib/NkLibraryD.lib")
#else
	#pragma comment(lib, "lib/NkLibrary.lib")
#endif

#include "ssl/opensslctx.h"
#include "ssl/sslsocket.h"
#include "ssl/sha.h"
#include "ssl/aes.h"

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")


