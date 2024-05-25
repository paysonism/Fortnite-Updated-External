#ifndef _KEYGEN64_H
#define _KEYGEN64_H

#include <wtypes.h>		// needed for FILETIME definition

#pragma comment(lib, "keygen64.lib")

// #define OBSIDIUM_USE_NAMESPACE in your project to wrap the Obsidium API 
// into a namespace
// Any API calls etc. will need to be prefixed with obsidium::
#ifdef __cplusplus
#ifdef OBSIDIUM_USE_NAMESPACE
namespace obsidium {
#endif
#endif

//#define HAVE_STDINT
#ifdef HAVE_STDINT
	#include <stdint.h>
#else
	#ifdef _MSC_VER
		typedef unsigned __int8 uint8_t;
		typedef unsigned __int16 uint16_t;
		typedef unsigned __int32 uint32_t;
	#else
		typedef unsigned char uint8_t;
		typedef unsigned short uint16_t;
		typedef unsigned long uint32_t;
	#endif
#endif

#pragma pack(push,1)

// error codes returned by GenerateLicenseLong and GenerateLicenseShort
#define KEYGEN_ERROR				0		// internal error
#define KEYGEN_INVALID_PARAMETER	-1		// invalid parameter passed to function
#define KEYGEN_OPF_OPEN_ERROR		-2		// error opening project file
#define KEYGEN_OPF_INVALID			-3		// invalid project file
#define KEYGEN_OPF_ERROR			-4		// no keys found in project file or read error
#define KEYGEN_FAIL_ALLOC			-5		// memory allocation error
#define KEYGEN_FAIL_BUSY			-6
#define KEYGEN_FAIL_USB				-7

// error codes returned by VerifyLicenseLong and VerifyLicenseShort
#define KEYVER_OK					1		// keyfile verification successful
#define KEYVER_FAIL					0		// keyfile is invalid
#define KEYVER_INVALID_PARAMETER	-1		// invalid parameter passed to function
#define KEYVER_OPF_OPEN_ERROR		-2		// error opening project file
#define KEYVER_OPF_INVALID			-3		// invalid project file
#define KEYVER_FAIL_ALLOC			-4		// memory allocation error
#define KEYVER_FAIL_BUSY			-5		// the library is busy

// flags passed to GetLicenseHash
#define LICENSE_BIN					0		// binary key
#define LICENSE_TEXT				1		// text key
#define LICENSE_SHORT				2		// short key

// flags used in KEYGEN_INFO_LONG and KEYVER_INFO_LONG
#define LONGKEY_FLAG_TEXT			1		// generate or verify text key instead of binary key
#define LONGKEY_FLAG_LEGACY_SYSID	2		// use old system ID format (Obsidium < 1.4.2)

// structure passed to GenerateLicenseLong
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const char*		lpInfo1;				// license info
	const char*		lpInfo2;				// ""
	const char*		lpInfo3;				// ""
	const char*		lpExtInfo;				// ""
	const char*		lpSystemId;				// system ID string
	const char*		lpUsbId;				// USB ID string or drive letter
	const FILETIME*	lpExpDate;				// expiration date
	void*			lpBuffer;				// output buffer
	const char*		lpProjectFile;			// project filename
} KEYGEN_INFO_LONG;

// structure passed to GenerateLicenseLongW
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const wchar_t*	lpInfo1;				// license info
	const wchar_t*	lpInfo2;				// ""
	const wchar_t*	lpInfo3;				// ""
	const wchar_t*	lpExtInfo;				// ""
	const wchar_t*	lpSystemId;				// system ID string
	const wchar_t*	lpUsbId;				// USB ID string or drive letter
	const FILETIME*	lpExpDate;				// expiration date
	void*			lpBuffer;				// output buffer
	const wchar_t*	lpProjectFile;			// project filename
} KEYGEN_INFO_LONGW;

// structure passed to VerifyLicenseLong
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	char*			lpInfo1;				// license info
	char*			lpInfo2;				// ""
	char*			lpInfo3;				// ""
	char*			lpExtInfo;				// ""
	char*			lpSystemId;				// system ID string
	char*			lpUsbId;				// USB ID string
	FILETIME*		lpExpDate;				// expiration date
	FILETIME*		lpCreationDate;			// creation date
	const void*		lpBuffer;				// input buffer
	const char*		lpProjectFile;			// project filename
} KEYVER_INFO_LONG;

// structure passed to VerifyLicenseLongW
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	wchar_t*		lpInfo1;				// license info
	wchar_t*		lpInfo2;				// ""
	wchar_t*		lpInfo3;				// ""
	wchar_t*		lpExtInfo;				// ""
	wchar_t*		lpSystemId;				// system ID string
	wchar_t*		lpUsbId;				// USB ID string
	FILETIME*		lpExpDate;				// expiration date
	FILETIME*		lpCreationDate;			// creation date
	const void*		lpBuffer;				// input buffer
	const wchar_t*	lpProjectFile;			// project filename
} KEYVER_INFO_LONGW;

// flags used in KEYGEN_INFO_SHORT and KEYVER_INFO_SHORT
#define SHORTKEY_FLAG_ADVANCED		1		// generate advanced short license key
#define SHORTKEY_FLAG_EXPIRE		2		// license key expires and lpDate is valid, SHORTKEY_FLAG_ADVANCED must be set
#define SHORTKEY_FLAG_LEGACY_SYSID	4		// use old system ID format (Obsidium < 1.4.2)
#define SHORTKEY_FLAG_PAD			8		// pad license keys to next separator character boundary
#define SHORTKEY_FLAG_CREATION_DATE 16		// lpDate specifies a creation date, SHORTKEY_FLAG_ADVANCED must be set,
											// SHORTKEY_FLAG_EXPIRE must not be set

// structure passed to GenerateLicenseShort
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const char*		lpInfo;					// license info string
	const char*		lpSystemId;				// system ID string
	const FILETIME*	lpDate;					// expiration or creation date, depending on flags
	char*			lpKey;					// output buffer
	const char*		lpProjectFile;			// project file name
	uint16_t		dwCustom;				// custom value
} KEYGEN_INFO_SHORT;

// structure passed to GenerateLicenseShortW
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const wchar_t*	lpInfo;					// license info string
	const wchar_t*	lpSystemId;				// system ID string
	const FILETIME*	lpDate;					// expiration or creation date, depending on flags
	wchar_t*		lpKey;					// output buffer
	const wchar_t*	lpProjectFile;			// project file name
	uint16_t		dwCustom;				// custom value
} KEYGEN_INFO_SHORTW;

// structure passed to VerifyLicenseShort
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const char*		lpInfo;					// license info string
	const char*		lpSystemId;				// system ID string
	FILETIME*		lpExpDate;				// expiration date
	FILETIME*		lpCreationDate;			// creation date
	const char*		lpKey;					// license key
	const char*		lpProjectFile;			// project file name
	uint16_t		dwCustom;				// custom value
} KEYVER_INFO_SHORT;

// structure passed to VerifyLicenseShortW
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	uint32_t		dwFlags;				// flags
	const wchar_t*	lpInfo;					// license info string
	const wchar_t*	lpSystemId;				// system ID string
	FILETIME*		lpExpDate;				// expiration date
	FILETIME*		lpCreationDate;			// creation date
	const wchar_t*	lpKey;					// license key
	const wchar_t*	lpProjectFile;			// project file name
	uint16_t		dwCustom;				// custom value
} KEYVER_INFO_SHORTW;

// structure passed to GenerateAppCertificate
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	const char*		lpId;					// ID
	const char*		lpName;					// name
	const char*		lpUserData;				// user data, optional
	const char*		lpServerId;				// server ID, optional
	uint32_t		dwLicenseCount;			// number of licenses
	uint32_t		dwFlags;				// APPCERT_FLAG_xxx
	FILETIME*		lpStartDate;			// start date, optional
	FILETIME*		lpEndDate;				// end date, optional
	void*			lpBuffer;				// buffer for generated certificate
	uint32_t*		lpBufferSize;			// size of buffer, set to 0 to receive required size
} APPCERT_INFO;

// flags used for GenerateAppCertificate
#define APPCERT_FLAG_CLIENT_LIMIT		0x01	// only allow one license per client
#define	APPCERT_FLAG_LICENSE_SHARING	0x02	// enable license sharing
#define APPCERT_FLAG_SHARE_PER_USER		0x04	// share per user, depends on APPCERT_FLAG_LICENSE_SHARING
#define APPCERT_FLAG_UTF8				0x08	// interpret name and user data as UTF-8 strings

// error codes returned by GenerateAppCertificate
#define APPCERT_GEN_OK					1		// certificate generation successful
#define APPCERT_GEN_INVALID_PARAMETER	-1		// invalid parameter passed to function
#define APPCERT_GEN_BUFFER_SIZE			-2		// buffer too small, required size in variable pointed to
												// by lpBufferSize
#define APPCERT_GEN_BAD_ID				-3		// the lpId parameter is invalid
#define APPCERT_GEN_WRITE_ERROR			-4		// error writing the full application certificate to disk
#define APPCERT_GEN_NETLIC_CERT			-5		// no valid network licensing certificate found

// structure passed to GenerateServerConfiguration
typedef struct
{
	uint32_t		dwSize;					// size of this structure
	const char*		lpName;					// name
	char*			lpId;					// ID buffer, optional
	uint32_t		dwFlags;				// SERVERCFG_FLAG_xxx
	const char*		lpSystemId;				// system ID the server is locked to, optional
	uint16_t		wChangeComponents;		// number of hardware components that may change
	uint16_t		wPort;					// port (0 = default)
	const char*		lpUsbId;				// USB ID, optional
	const char*		lpAdminPassword;		// administrator password, optional
	const wchar_t*	lpProjectFile;			// project file name, required if hardware locking is used
} SERVERCFG_INFO;

// flags used for GenerateServerConfiguration
#define SERVERCFG_FLAG_HW_LOCK_CPU		0x0001	// lock to CPU
#define SERVERCFG_FLAG_HW_LOCK_HDVOL	0x0002	// lock to HDD (volume serial)
#define SERVERCFG_FLAG_HW_LOCK_HDDEV	0x0004	// lock to HDD (device serial)
#define SERVERCFG_FLAG_HW_LOCK_MAC		0x0008	// lock to MAC address
#define SERVERCFG_FLAG_HW_LOCK_OS		0x0010	// lock to OS
#define SERVERCFG_FLAG_HW_LOCK_MB		0x0020	// lock to mainboard
#define	SERVERCFG_FLAG_NO_VM			0x0040	// prevent server from running in VMs
#define SERVERCFG_FLAG_ALLOW_CFG_CHANGE	0x0080	// admin may change server config
#define SERVERCFG_FLAG_LOCALHOST_ONLY	0x0100	// allow admin connections from localhost only
#define SERVERCFG_FLAG_LOG_SERVICE		0x0200	// log service events
#define SERVERCFG_FLAG_LOG_APP_CERT		0x0400	// log app cert events
#define SERVERCFG_FLAG_LOG_LICENSING	0x0800	// log licensing events
#define SERVERCFG_FLAG_UTF8				0x1000	// interpret name and password as UTF-8 strings

// error codes returned by GenerateServerConfiguration
#define SERVERCFG_GEN_OK				1		// configuration generation successful
#define SERVERCFG_GEN_INVALID_PARAMETER	-1
#define SERVERCFG_GEN_NETLIC_CERT		-2
#define SERVERCFG_GEN_WRITE_ERROR		-3
#define SERVERCFG_GEN_PROJECT_FILE_ERROR -4


#define KEYGEN_API __declspec(dllimport)

extern "C" int KEYGEN_API GenerateLicenseLong(const KEYGEN_INFO_LONG* lpInfo);
extern "C" int KEYGEN_API GenerateLicenseLongW(const KEYGEN_INFO_LONGW* lpInfo);
extern "C" int KEYGEN_API VerifyLicenseLong(const KEYVER_INFO_LONG* lpInfo);
extern "C" int KEYGEN_API VerifyLicenseLongW(const KEYVER_INFO_LONGW* lpInfo);
extern "C" int KEYGEN_API GenerateLicenseShort(const KEYGEN_INFO_SHORT* lpInfo);
extern "C" int KEYGEN_API GenerateLicenseShortW(const KEYGEN_INFO_SHORTW* lpInfo);
extern "C" int KEYGEN_API VerifyLicenseShort(KEYVER_INFO_SHORT* lpInfo);
extern "C" int KEYGEN_API VerifyLicenseShortW(KEYVER_INFO_SHORTW* lpInfo);
extern "C" int KEYGEN_API ConvertLicenseLongToStr(const void* lpLic, uint32_t dwLen, char* lpBuffer);
extern "C" int KEYGEN_API ConvertLicenseLongToBin(const char* lpLic, void* lpBuffer);
extern "C" bool KEYGEN_API GetLicenseHash(const void* lpLicense, uint32_t dwType, void* lpHash);
extern "C" bool KEYGEN_API GenerateSignature(const char* lpszProjectFile, 
											 const void* lpData, uint32_t dwLen, char* lpszSig);
extern "C" bool KEYGEN_API GenerateSignatureW(const wchar_t* lpszProjectFile, 
											 const void* lpData, uint32_t dwLen, wchar_t* lpszSig);
extern "C" bool KEYGEN_API GenerateSignatureFromFile(const char* lpszProjectFile, 
													 const char *lpszFilename, char* lpszSig);
extern "C" bool KEYGEN_API GenerateSignatureFromFileW(const wchar_t* lpszProjectFile, 
													  const wchar_t *lpszFilename, wchar_t* lpszSig);
extern "C" bool KEYGEN_API VerifySignature(const char* lpszProjectFile, 
										   const void* lpData, uint32_t dwLen, const char* lpszSig);
extern "C" bool KEYGEN_API VerifySignatureW(const wchar_t* lpszProjectFile, 
											const void* lpData, uint32_t dwLen, const wchar_t* lpszSig);
extern "C" bool KEYGEN_API VerifySignatureFromFile(const char* lpszProjectFile, 
												   const char *lpszFilename, const char* lpszSig);
extern "C" bool KEYGEN_API VerifySignatureFromFileW(const wchar_t* lpszProjectFile, 
													const wchar_t *lpszFilename, const wchar_t* lpszSig);
extern "C" int KEYGEN_API CompareSystemIds(const char *lpszProjectFile, const char *lpszSystemId1,
										   const char *lpszSystemId2);
extern "C" int KEYGEN_API CompareSystemIdsW(const wchar_t *lpszProjectFile, const wchar_t *lpszSystemId1,
										    const wchar_t *lpszSystemId2);
extern "C" int KEYGEN_API GenerateAppCertificate(const APPCERT_INFO *lpInfo);
extern "C" int KEYGEN_API GenerateServerConfiguration(const SERVERCFG_INFO *lpInfo);


#pragma pack(pop)

#ifdef __cplusplus
#ifdef OBSIDIUM_USE_NAMESPACE
}
#endif
#endif

#endif

