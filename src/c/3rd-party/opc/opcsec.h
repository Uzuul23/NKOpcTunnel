

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun Aug 09 18:01:46 2015
 */
/* Compiler settings for ..\..\IDL\OPC\opcSec.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __opcSec_h_h__
#define __opcSec_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOPCSecurityNT_FWD_DEFINED__
#define __IOPCSecurityNT_FWD_DEFINED__
typedef interface IOPCSecurityNT IOPCSecurityNT;

#endif 	/* __IOPCSecurityNT_FWD_DEFINED__ */


#ifndef __IOPCSecurityPrivate_FWD_DEFINED__
#define __IOPCSecurityPrivate_FWD_DEFINED__
typedef interface IOPCSecurityPrivate IOPCSecurityPrivate;

#endif 	/* __IOPCSecurityPrivate_FWD_DEFINED__ */


#ifndef __IOPCSecurityNT_FWD_DEFINED__
#define __IOPCSecurityNT_FWD_DEFINED__
typedef interface IOPCSecurityNT IOPCSecurityNT;

#endif 	/* __IOPCSecurityNT_FWD_DEFINED__ */


#ifndef __IOPCSecurityPrivate_FWD_DEFINED__
#define __IOPCSecurityPrivate_FWD_DEFINED__
typedef interface IOPCSecurityPrivate IOPCSecurityPrivate;

#endif 	/* __IOPCSecurityPrivate_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IOPCSecurityNT_INTERFACE_DEFINED__
#define __IOPCSecurityNT_INTERFACE_DEFINED__

/* interface IOPCSecurityNT */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCSecurityNT;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7AA83A01-6C77-11d3-84F9-00008630A38B")
    IOPCSecurityNT : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsAvailableNT( 
            /* [out] */ BOOL *pbAvailable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryMinImpersonationLevel( 
            /* [out] */ DWORD *pdwMinImpLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChangeUser( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOPCSecurityNTVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOPCSecurityNT * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOPCSecurityNT * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOPCSecurityNT * This);
        
        HRESULT ( STDMETHODCALLTYPE *IsAvailableNT )( 
            IOPCSecurityNT * This,
            /* [out] */ BOOL *pbAvailable);
        
        HRESULT ( STDMETHODCALLTYPE *QueryMinImpersonationLevel )( 
            IOPCSecurityNT * This,
            /* [out] */ DWORD *pdwMinImpLevel);
        
        HRESULT ( STDMETHODCALLTYPE *ChangeUser )( 
            IOPCSecurityNT * This);
        
        END_INTERFACE
    } IOPCSecurityNTVtbl;

    interface IOPCSecurityNT
    {
        CONST_VTBL struct IOPCSecurityNTVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCSecurityNT_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOPCSecurityNT_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOPCSecurityNT_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOPCSecurityNT_IsAvailableNT(This,pbAvailable)	\
    ( (This)->lpVtbl -> IsAvailableNT(This,pbAvailable) ) 

#define IOPCSecurityNT_QueryMinImpersonationLevel(This,pdwMinImpLevel)	\
    ( (This)->lpVtbl -> QueryMinImpersonationLevel(This,pdwMinImpLevel) ) 

#define IOPCSecurityNT_ChangeUser(This)	\
    ( (This)->lpVtbl -> ChangeUser(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOPCSecurityNT_INTERFACE_DEFINED__ */


#ifndef __IOPCSecurityPrivate_INTERFACE_DEFINED__
#define __IOPCSecurityPrivate_INTERFACE_DEFINED__

/* interface IOPCSecurityPrivate */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCSecurityPrivate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7AA83A02-6C77-11d3-84F9-00008630A38B")
    IOPCSecurityPrivate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsAvailablePriv( 
            /* [out] */ BOOL *pbAvailable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Logon( 
            /* [string][in] */ LPCWSTR szUserID,
            /* [string][in] */ LPCWSTR szPassword) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Logoff( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOPCSecurityPrivateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOPCSecurityPrivate * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOPCSecurityPrivate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOPCSecurityPrivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *IsAvailablePriv )( 
            IOPCSecurityPrivate * This,
            /* [out] */ BOOL *pbAvailable);
        
        HRESULT ( STDMETHODCALLTYPE *Logon )( 
            IOPCSecurityPrivate * This,
            /* [string][in] */ LPCWSTR szUserID,
            /* [string][in] */ LPCWSTR szPassword);
        
        HRESULT ( STDMETHODCALLTYPE *Logoff )( 
            IOPCSecurityPrivate * This);
        
        END_INTERFACE
    } IOPCSecurityPrivateVtbl;

    interface IOPCSecurityPrivate
    {
        CONST_VTBL struct IOPCSecurityPrivateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCSecurityPrivate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOPCSecurityPrivate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOPCSecurityPrivate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOPCSecurityPrivate_IsAvailablePriv(This,pbAvailable)	\
    ( (This)->lpVtbl -> IsAvailablePriv(This,pbAvailable) ) 

#define IOPCSecurityPrivate_Logon(This,szUserID,szPassword)	\
    ( (This)->lpVtbl -> Logon(This,szUserID,szPassword) ) 

#define IOPCSecurityPrivate_Logoff(This)	\
    ( (This)->lpVtbl -> Logoff(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOPCSecurityPrivate_INTERFACE_DEFINED__ */



#ifndef __OPCSEC_LIBRARY_DEFINED__
#define __OPCSEC_LIBRARY_DEFINED__

/* library OPCSEC */
/* [helpstring][version][uuid] */ 




EXTERN_C const IID LIBID_OPCSEC;
#endif /* __OPCSEC_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


