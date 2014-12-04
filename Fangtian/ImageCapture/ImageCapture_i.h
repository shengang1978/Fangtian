

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Dec 04 23:22:29 2014
 */
/* Compiler settings for ImageCapture.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
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

#ifndef __ImageCapture_i_h__
#define __ImageCapture_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICImageCapture_FWD_DEFINED__
#define __ICImageCapture_FWD_DEFINED__
typedef interface ICImageCapture ICImageCapture;

#endif 	/* __ICImageCapture_FWD_DEFINED__ */


#ifndef __CImageCapture_FWD_DEFINED__
#define __CImageCapture_FWD_DEFINED__

#ifdef __cplusplus
typedef class CImageCapture CImageCapture;
#else
typedef struct CImageCapture CImageCapture;
#endif /* __cplusplus */

#endif 	/* __CImageCapture_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICImageCapture_INTERFACE_DEFINED__
#define __ICImageCapture_INTERFACE_DEFINED__

/* interface ICImageCapture */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICImageCapture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2429B01B-CE7C-403E-BC58-E8E9C3120B9B")
    ICImageCapture : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICImageCaptureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICImageCapture * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICImageCapture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICImageCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICImageCapture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICImageCapture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICImageCapture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICImageCapture * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ICImageCaptureVtbl;

    interface ICImageCapture
    {
        CONST_VTBL struct ICImageCaptureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICImageCapture_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICImageCapture_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICImageCapture_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICImageCapture_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICImageCapture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICImageCapture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICImageCapture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICImageCapture_INTERFACE_DEFINED__ */



#ifndef __ImageCaptureLib_LIBRARY_DEFINED__
#define __ImageCaptureLib_LIBRARY_DEFINED__

/* library ImageCaptureLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ImageCaptureLib;

EXTERN_C const CLSID CLSID_CImageCapture;

#ifdef __cplusplus

class DECLSPEC_UUID("2704D3A9-1EEE-488E-B4B8-A38E4148FF12")
CImageCapture;
#endif
#endif /* __ImageCaptureLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


