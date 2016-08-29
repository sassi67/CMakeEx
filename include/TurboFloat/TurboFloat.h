#ifndef TURBOFLOAT_H_INCLUDED
#define TURBOFLOAT_H_INCLUDED

/* Define "uint32_t" for old versions of MS C++ (VS 2008 and below) */
#if !defined(_WIN32) || _MSC_VER >= 1600
    #include <stdint.h>
#else
    typedef unsigned int uint32_t;
#endif

#ifdef _WIN32
    /*
    These defines assume you're using the MSVC or Intel compilers on Windows.
    We do *not* recommend using the GNU / Mingw compilers on Windows.
    These compilers are buggy (among other problems). However, if you choose
    to use the GNU / Mingw on Windows then you'll have to modify these defines
    to properly support these buggy compilers.

    Note: using GNU compilers on Linux / Mac / Solaris / etc. is fine.
    These recommendations are for Windows only.
    */
    #include <windows.h>

    #ifdef TURBOFLOAT_EXPORTS
        #ifdef TURBOFLOAT_STATIC
            #define TURBOFLOAT_API extern "C"
        #else
            #define TURBOFLOAT_API extern "C" __declspec(dllexport)
        #endif
    #else
        #ifdef __cplusplus
            #ifdef TURBOFLOAT_STATIC
                #define TURBOFLOAT_API extern "C"
            #else
                #define TURBOFLOAT_API extern "C" __declspec(dllimport)
            #endif
        #else
            #ifdef TURBOFLOAT_STATIC
                #define TURBOFLOAT_API
            #else
                #define TURBOFLOAT_API __declspec(dllimport)
            #endif
        #endif
    #endif

    #if defined(USE_STDCALL_TF_DLL) && !defined(TURBOFLOAT_STATIC)
        #define TF_CC __stdcall
    #else
        #define TF_CC __cdecl
    #endif
    #define TF_DEPRECATED(func) __declspec(deprecated) func

    typedef LPWSTR STRTYPE;
    typedef LPCWSTR STRCTYPE;
#else
    #if __GNUC__ >= 4 || defined(__SUNPRO_CC)
        #ifdef __cplusplus
            #define TURBOFLOAT_API extern "C" __attribute__((visibility("default")))
        #else
            #define TURBOFLOAT_API __attribute__((visibility("default")))
        #endif
    #else
        #ifdef __cplusplus
            #define TURBOFLOAT_API extern "C"
        #else
            #define TURBOFLOAT_API
        #endif
    #endif

    #define TF_CC
    #ifdef __GNUC__
        #define TF_DEPRECATED(func) func __attribute__ ((deprecated))
    #else
        #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
        #define TF_DEPRECATED(func) func
    #endif

    typedef char* STRTYPE;
    typedef const char* STRCTYPE;
    typedef int32_t HRESULT;
#endif

/* Flags for the TF_SaveServer() function. */
#define TF_SYSTEM ((uint32_t)1)
#define TF_USER ((uint32_t)2)


/*
   Gets the handle that will be used for 


   Returns: 0 on failure. All other return codes are successful.

   If you're getting a "0" return code then it's most likely a result
   of TurboFloat being unable to load your TurboActivate.dat file. So
   use TF_PDetsFromPath() to load the file first and then call this function
   if you're getting an error.
*/
TURBOFLOAT_API uint32_t TF_CC TF_GetHandle(STRCTYPE versionGUID);


/*
   Saves the TurboFloat server location on the disk. You must call this function at
   least once before requesting a lease from the server. A good place to call this function
   is from your installer (so an IT admin could set the server location once).


   Note: If you pass in the TF_SYSTEM flag and you don't have "admin" or "elevated"
   permission then the call will fail.

   If you call this function once in the past with the flag TF_SYSTEM and the calling
   process was an admin process then subsequent calls with the TF_SYSTEM flag will
   succeed even if the calling process is *not* admin/elevated.

   If you want to take advantage of this behavior from an admin process
   (e.g. an installer) but the user hasn't entered a product key then you can
   call this function with a null string:

            TF_SaveServer(YourHandle, 0, 0, TF_SYSTEM);

   This will set everything up so that subsequent calls with the TF_SYSTEM flag will
   succeed even if from non-admin processes.


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_PERMISSION,
                          TF_E_INVALID_FLAGS, TF_E_INVALID_HANDLE
*/
TURBOFLOAT_API HRESULT TF_CC TF_SaveServer(uint32_t handle, STRTYPE host_address, short port, uint32_t flags);


/*
   The lease callback function type.
*/
typedef void (TF_CC *LeaseCallbackType)(uint32_t);


/*
   Set the function that will be called by a separate background thread that notifies
   your app of lease events (e.g. the lease expired, the features changed, etc.).

   If there's a lease currently assigned to your app, then you won't be able to change
   the callback until either the lease expires (and can't be renewed) or until you
   drop the lease by calling the TF_DropLease() function.

   The lease callback function should handle everything defined below (see:
   "Possible callback statuses" at the bottom of this header). Everything that's
   not defined should be handled as a failure to renew the lease.


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_INVALID_HANDLE
*/
TURBOFLOAT_API HRESULT TF_CC TF_SetLeaseCallback(uint32_t handle, LeaseCallbackType callback);


/*
   Requests a floating license lease from the TurboFloat Server. You should run
   this at the top of your app after calling TF_SetLeaseCallback().


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_INVALID_HANDLE, TF_E_SERVER, TF_E_INET,
                          TF_E_COM, TF_E_NO_CALLBACK, TF_E_LEASE_EXISTS, 
                          TF_E_NO_FREE_LEASES, TF_E_WRONG_TIME, TF_E_WRONG_SERVER_PRODUCT
*/
TURBOFLOAT_API HRESULT TF_CC TF_RequestLease(uint32_t handle);


/*
   Drops the active lease from the TurboFloat Server. This frees up the lease
   so it can be used by another instance of your app.

   We recommend calling this before your application exits.

   Note: This function does *not* call the lease callback function. If you want that
         behavior then you can do it like this:


         if (TF_DropLease(tfHandle) == TF_OK)
         {
             YourLeaseCallbackFunction(TF_CB_EXPIRED);
         }


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_INVALID_HANDLE, TF_E_SERVER, TF_E_INET,
                          TF_E_NO_LEASE, TF_E_COM, TF_E_WRONG_SERVER_PRODUCT
*/
TURBOFLOAT_API HRESULT TF_CC TF_DropLease(uint32_t handle);


/*
   Let's you know whether there's an active lease for the handle specified. This function
   isn't necessary if you're tracking the responses from TF_RequestLease(), TF_DropLease(),
   and the callback function that you set in TF_SetLeaseCallback().


   Returns: TF_OK on success (in other words you have a lease). Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_INVALID_HANDLE
*/
TURBOFLOAT_API HRESULT TF_CC TF_HasLease(uint32_t handle);


/*
   Gets the value of a feature.


   lpValueStr
   [out] Pointer to a buffer that receives the value of the string.

   cchValue
   [in] Size (in wide characters on Windows or characters in Unix) of the buffer
        pointed to by the lpValueStr parameter.

   If 'cchValue' is zero, the function returns the required buffer size (in wide characters
   on Windows, characters on Unix) and makes no use of the lpValueStr buffer.


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_INVALID_HANDLE, TF_E_NO_LEASE,
                          TF_E_INSUFFICIENT_BUFFER
*/
TURBOFLOAT_API HRESULT TF_CC TF_GetFeatureValue(uint32_t handle, STRCTYPE featureName, STRTYPE lpValueStr, int cchValue);


/*
   Gets the stored TurboFloat Server location.


   lpHostStr
   [out] Pointer to a buffer that receives the value of the string.

   cchHost
   [in] Size (in wide characters on Windows or characters in Unix) of the buffer
        pointed to by the lpHostStr parameter.

   If 'cchHost' is zero, the function returns the required buffer size (in wide characters
   on Windows, characters on Unix) and makes no use of the lpHostStr buffer.


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_SERVER, TF_E_INVALID_HANDLE, TF_E_INSUFFICIENT_BUFFER
*/
TURBOFLOAT_API HRESULT TF_CC TF_GetServer(uint32_t handle, STRTYPE lpHostStr, int cchHost, short * port);


/*
   Loads the "TurboActivate.dat" file from a path rather than loading it
   from the same dir as TurboFloat.dll on Windows or the app that
   uses libTurboFloat.dylib / libTurboFloat.so on Mac / Linux.

   You can load multiple *.dat files for licensing multiple products within
   one process. You can switch between the products by using the
   TF_SetCurrentProduct() with the VersionGUID of the product you want to use
   licensing for.


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL, TF_E_PDETS


   Note: Once the product details file has loaded, all subsequent calls to "PDetsFromPath"
   will return TF_FAIL
*/
TURBOFLOAT_API HRESULT TF_CC TF_PDetsFromPath(STRCTYPE filename);


/*
   You should call this before your application exits. This frees up any
   allocated memory for all open handles. If you have an active license
   lease then you should call TF_DropLease() before you call TF_Cleanup().


   Returns: TF_OK on success. Handle all other return codes as failures.

   Possible return codes: TF_OK, TF_FAIL
*/
TURBOFLOAT_API HRESULT TF_CC TF_Cleanup();


/*
 General Success and Failure return codes.
*/
#define TF_OK               ((HRESULT)0L)
#define TF_FAIL             ((HRESULT)1L)

/*
 MessageId: TF_E_SERVER

 MessageText:

 There's no server specified. You must call TF_SaveServer() at least once to save the server.
*/
#define TF_E_SERVER                 ((HRESULT)0x00000002L)


/*
 MessageId: TF_E_NO_CALLBACK

 MessageText:

 You didn't specify a callback. Do so using the TF_SetLeaseCallback() function.
*/
#define TF_E_NO_CALLBACK            ((HRESULT)0x00000003L)


/*
 MessageId: TF_E_INET

 MessageText:

 Connection to the TurboFloat server failed.
*/
#define TF_E_INET                   ((HRESULT)0x00000004L)


/*
 MessageId: TF_E_NO_FREE_LEASES

 MessageText:

 There are no more free leases available from the TurboFloat server. Either
 increase the number of allowed floating licenses for the TurboFloat server
 or wait for one of the other leases to expire.
*/
#define TF_E_NO_FREE_LEASES         ((HRESULT)0x00000005L)


/*
 MessageId: TF_E_LEASE_EXISTS

 MessageText:

 The lease has already been acquired. TurboFloat automatically renews the lease
 when it needs to based on the information the TurboFloat Server provides.
*/
#define TF_E_LEASE_EXISTS           ((HRESULT)0x00000006L)


/*
 MessageId: TF_E_WRONG_TIME

 MessageText:

 This computer's system time is more than 5 minutes (before/after)
 different from the TurboFloat Server's system time. Make sure
 the server's Date, Time, and Timezone are set correctly and make
 sure this computer's Date, Time, and Timezone are set correctly.

 Note: That TurboFloat and TurboFloat Server work even if the timezones
       of the computer/server are different. In other words, your
       TurboFloat Server could be hosted in France and have a "client"
       computer running in New York (a 6 hour time difference) and
       everything will work fine provided both server and computer
       have their date, time, and timzones correctly configured.
*/
#define TF_E_WRONG_TIME        ((HRESULT)0x00000007L)


/*
 MessageId: TF_E_PDETS

 MessageText:

 The product details file "TurboActivate.dat" failed to load.
*/
#define TF_E_PDETS                  ((HRESULT)0x00000008L)


/*
 MessageId: TF_E_INVALID_HANDLE

 MessageText:

 The handle is not valid. To get a handle use TF_GetHandle().
*/
#define TF_E_INVALID_HANDLE         ((HRESULT)0x00000009L)


/*
 MessageId: TF_E_NO_LEASE

 MessageText:

 There's no lease. Your must first have a lease before you can
 drop it or get information on it.
*/
#define TF_E_NO_LEASE                ((HRESULT)0x0000000AL)


/*
 MessageId: TF_E_COM

 MessageText:

 The hardware id couldn't be generated due to an error in the COM setup.
 Re-enable Windows Management Instrumentation (WMI) in your group policy
 editor or reset the local group policy to the default values. Contact
 your system admin for more information.

 This error is Windows only.

 This error can also be caused by the user (or another program) disabling
 the "Windows Management Instrumentation" service. Make sure the "Startup type"
 is set to Automatic and then start the service.


 To further debug WMI problems open the "Computer Management" (compmgmt.msc),
 expand the "Services and Applications", right click "WMI Control" click
 "Properties" and view the status of the WMI.
*/
#define TF_E_COM                    ((HRESULT)0x0000000BL)


/*
 MessageId: TF_E_INSUFFICIENT_BUFFER

 MessageText:

 The the buffer size was too small. Create a larger buffer and try again.
*/
#define TF_E_INSUFFICIENT_BUFFER    ((HRESULT)0x0000000CL)


/*
 MessageId: TF_E_PERMISSION

 MessageText:

 Insufficient system permission. Either start your process as an
 admin / elevated user or call the function again with the
 TF_USER flag instead of the TF_SYSTEM flag.
*/
#define TF_E_PERMISSION           ((HRESULT)0x0000000DL)


/*
 MessageId: TF_E_INVALID_FLAGS

 MessageText:

 The flags you passed to SaveServer(...) were invalid or missing.
 Flags like "TF_SYSTEM" and "TF_USER" are mutually exclusive --
 you can only use one or the other.
*/
#define TF_E_INVALID_FLAGS        ((HRESULT)0x0000000EL)


/*
 MessageId: TF_E_WRONG_SERVER_PRODUCT
 
 MessageText:
 
 The TurboFloat Server you're trying to contact can't give license
 leases for this product version.
*/
#define TF_E_WRONG_SERVER_PRODUCT     ((HRESULT)0x0000000FL)



/* Possible callback statuses: */


/* Called when the lease has expired and couldn't be renewed. You
   should disable your app immediately when this return code is called.
   Of course give your user a way to save the current state of their
   data and/or request a lease renewal from the server.

   In other words, don't make the user mad. Make sure you test this
   extensively with real end-users so you get the best behavior.
*/
#define TF_CB_EXPIRED               ((HRESULT)0x00000000UL)

/* Called when the lease has expired and couldn't be renewed due to
   failure to connect to the TurboFloat Server.
*/
#define TF_CB_EXPIRED_INET          ((HRESULT)0x00000001UL)

/* Called when the lease was renewed and some or all of the custom
   license fields have since changed. If you use the TF_GetFeatureValue()
   function then it behooves you to get the latest changed feature
   values upon this callback status.
*/
#define TF_CB_FEATURES_CHANGED      ((HRESULT)0x00000002UL)

#endif /* TURBOFLOAT_H_INCLUDED */
