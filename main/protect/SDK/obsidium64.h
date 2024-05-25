/*
	Obsidium protection API
	Version 1.7
*/
#ifndef OBSIDIUM_64_H
#define OBSIDIUM_64_H
#include <wtypes.h>		// needed for FILETIME, DWORD, alternatively use windows.h

// define OBSIDIUM_NO_LINK if you intend on dynamically linking to obsidium64.dll
// or if these pragmas don't work for your compiler 
#ifndef OBSIDIUM_NO_LINK
#if defined(__clang__) && !defined(_MSC_VER)
	#pragma link "obsidium64.a"
	#pragma link "obsidiumlib.a"
#else
	#pragma comment(lib, "obsidium64.lib")
#endif
#endif

// #define OBSIDIUM_USE_NAMESPACE in your project to wrap the Obsidium API 
// into a namespace
// Any API calls etc. will need to be prefixed with obsidium::,
// eg. obsidium::obsGetTrialDays() instead of just obsGetTrialDays()
#ifdef __cplusplus
	#ifdef OBSIDIUM_USE_NAMESPACE
		namespace obsidium {
	#endif
	#define OBS_BOOL bool
	extern "C" {
#else
	#include <stdint.h>
	#define OBS_BOOL uint8_t
#endif

unsigned short __declspec(dllimport) obsGetCustomValue();
int __declspec(dllimport) obsGetLicenseStatus();
OBS_BOOL __declspec(dllimport) obsGetLicenseInfo(unsigned long dwInfoNr, char* lpBuffer);
OBS_BOOL __declspec(dllimport) obsGetLicenseInfoW(unsigned long dwInfoNr, wchar_t* lpBuffer);
OBS_BOOL __declspec(dllimport) obsGetLicenseInfoEx(unsigned long dwInfoNr, char* lpBuffer, unsigned long dwBufSize);
OBS_BOOL __declspec(dllimport) obsGetLicenseInfoExW(unsigned long dwInfoNr,	wchar_t* lpBuffer, unsigned long dwBufSize);
OBS_BOOL __declspec(dllimport) obsIsLicensed();
OBS_BOOL __declspec(dllimport) obsGetLicenseHash(unsigned char *hash);
OBS_BOOL __declspec(dllimport) obsGetLicenseExpiration(FILETIME *lpExpiration);
OBS_BOOL __declspec(dllimport) obsGetLicenseCreation(FILETIME *lpDate);
void __declspec(dllimport) obsDisableLicense();
OBS_BOOL __declspec(dllimport) obsSetLicense(const char* lpszLic);
__declspec(dllimport) OBS_BOOL obsSetLicenseW(const wchar_t* lpszLic);
__declspec(dllimport) OBS_BOOL obsSetLicenseShort(const char* lpszInfo, const char* lpszKey, const char* lpszSysId);
__declspec(dllimport) OBS_BOOL obsSetLicenseShortW(const wchar_t* lpszInfo, const wchar_t* lpszKey, const wchar_t* lpszSysId);
__declspec(dllimport) OBS_BOOL obsDeleteLicenseData();
__declspec(dllimport) OBS_BOOL obsStoreLicense(const char* lpszLic);
__declspec(dllimport) OBS_BOOL obsStoreLicenseW(const wchar_t* lpszLic);
__declspec(dllimport) OBS_BOOL obsStoreLicenseShort(const char* lpszInfo, const char* lpszKey, const char* lpszSysId);
__declspec(dllimport) OBS_BOOL obsStoreLicenseShortW(const wchar_t* lpszInfo, const wchar_t* lpszKey, const wchar_t* lpszSysId);
int __declspec(dllimport) obsGetTrialDays();
int __declspec(dllimport) obsGetTrialRuns();
int __declspec(dllimport) obsGetInitialTrialDays();
int __declspec(dllimport) obsGetInitialTrialRuns();
void __declspec(dllimport) obsGetExpirationDate(FILETIME *lpExpiration);
void __declspec(dllimport) obsDeleteTrialData();
OBS_BOOL __declspec(dllimport) obsGetSystemId(char *lpBuffer);
OBS_BOOL __declspec(dllimport) obsIsProtected();
OBS_BOOL __declspec(dllimport) obsSetExternalKey(const unsigned char* lpBuffer, unsigned long dwSize);
const char __declspec(dllimport) *obsSecureString(const char* lpStr);
const wchar_t __declspec(dllimport) *obsSecureStringW(const wchar_t* lpStr);
OBS_BOOL __declspec(dllimport) obsReprotectString();
int __declspec(dllimport) obsGetInstanceCount();
OBS_BOOL __declspec(dllimport) obsIsVm();
int __declspec(dllimport) obsGetTrialCounter(DWORD dwCtrIdx);
int __declspec(dllimport) obsGetInitialTrialCounter(DWORD dwCtrIdx);
OBS_BOOL __declspec(dllimport) obsDecTrialCounter(DWORD dwCtrIdx, short wCount);
OBS_BOOL __declspec(dllimport) obsVerifySignatureData(const void* lpData, 
	unsigned long dwSize, const char* lpszSignature);
OBS_BOOL __declspec(dllimport) obsVerifySignatureFile(const char* lpszFilename, 
	const char* lpszSignature);
OBS_BOOL __declspec(dllimport) obsVerifySignatureFileW(const wchar_t* lpszFilename, 
	const wchar_t* lpszSignature);
void __declspec(dllimport) obsGetProtectionDate(FILETIME *date);
OBS_BOOL __declspec(dllimport) obsUsbGetDeviceId(const char *lpszVolume, char *lpszDeviceId,
	wchar_t *lpszManufacturer, DWORD dwManufacturerSize, wchar_t *lpszProduct, DWORD dwProductSize,
	wchar_t *lpszSerialNumber, DWORD dwSerialNumberSize);
OBS_BOOL __declspec(dllimport) obsUsbGetLicenseDeviceId(char *lpszDeviceId);
typedef void (*obsUsbCallback)(unsigned long event, unsigned long eventInfo, void *param);
void __declspec(dllimport) obsUsbRegisterCallback(obsUsbCallback callback, void *param);
typedef OBS_BOOL (*obsUsbEnumCallback)(const char *lpszDeviceId, const wchar_t *lpszManufacturer,
	const wchar_t *lpszProduct, const wchar_t *lpszSerialNumber, void *param);
OBS_BOOL __declspec(dllimport) obsUsbEnumDevices(obsUsbEnumCallback callback, void *param);
OBS_BOOL __declspec(dllimport) obsGetLicenseSystemId(char *lpBuffer);
OBS_BOOL __declspec(dllimport) obsReloadLicense();
OBS_BOOL __declspec(dllimport) obsUsbReadData(unsigned long dwOffset, unsigned long dwSize, 
	unsigned long dwStorage, void *lpBuffer);
OBS_BOOL __declspec(dllimport) obsUsbWriteData(unsigned long dwOffset, unsigned long dwSize, 
	const void *lpBuffer);
unsigned long __declspec(dllimport) obsUsbExecute(unsigned short wId, unsigned long dwInBufSize, 
	const void *lpInBuf, unsigned long dwOutBufSize, void *lpOutBuf, unsigned long *lpdwBytesWritten);
OBS_BOOL __declspec(dllimport) obsUsbEncrypt(void *lpData, unsigned long dwSize, unsigned long dwMode, 
	const void *lpIV, unsigned long dwReserved);
OBS_BOOL __declspec(dllimport) obsUsbDecrypt(void *lpData, unsigned long dwSize, unsigned long dwMode, 
	const void *lpIV, unsigned long dwReserved);
unsigned long __declspec(dllimport) obsGetActiveLicensingSystem();
int __declspec(dllimport) obsConvertLicenseToBinary(const char *lpszLicense, void *lpOutput, unsigned long *lpOutputSize);
int __declspec(dllimport) obsConvertLicenseToString(const void *lpLicense, unsigned long dwSize, char *lpszOutput, unsigned long *lpOutputSize);
OBS_BOOL __declspec(dllimport) obsNetLicConnect(const char *host, unsigned int port);
void __declspec(dllimport) obsNetLicDisconnect();
typedef void (*obsNetLicCallback)(unsigned long event, unsigned long eventInfo, void *param);
void __declspec(dllimport) obsNetLicRegisterCallback(obsNetLicCallback function, void *param);
OBS_BOOL __declspec(dllimport) obsNetLicGetAppCertName(wchar_t *name);
OBS_BOOL __declspec(dllimport) obsNetLicGetAppCertUserData(wchar_t *userData);
OBS_BOOL __declspec(dllimport) obsNetLicGetAppCertId(wchar_t *name);
int __declspec(dllimport) obsGetLicenseData(void *lpBuffer, unsigned long *lpBufSize);
OBS_BOOL __declspec(dllimport) obsGetTrialEndDate(FILETIME *lpDate);
int __declspec(dllimport) obsGetTrialIdentifier(wchar_t *lpBuffer, unsigned long *lpBufSize);
OBS_BOOL __declspec(dllimport) obsVerifyLicenseShort(const char *lpszInfo, const char *lpszKey, const char *lpszSystemId,
	unsigned short *lpCustomValue, FILETIME *lpExpirationDate, FILETIME *lpCreationDate, unsigned char *lpHash, void *reserved);
OBS_BOOL __declspec(dllimport) obsVerifyLicenseShortW(const wchar_t *lpszInfo, const wchar_t *lpszKey, const wchar_t *lpszSystemId,
	unsigned short *lpCustomValue, FILETIME *lpExpirationDate, FILETIME *lpCreationDate, unsigned char *lpHash, void *reserved);
OBS_BOOL __declspec(dllimport) obsBlacklistLicenses(const unsigned char *lpHashes, unsigned long dwCount);
OBS_BOOL __declspec(dllimport) obsVerifyLicense(const char *lpLicense, char *lpszInfo1, char *lpszInfo2, char *lpszInfo3, char *lpszExtInfo,
	char *lpszSystemId, char *lpszUsbId, FILETIME *lpExpirationDate, FILETIME *lpCreationDate, unsigned char *lpHash, void *reserved);
OBS_BOOL __declspec(dllimport) obsEncDecData(void *lpData, unsigned long dwSize, unsigned long dwOffset, const void* lpKey,
	unsigned long dwKeyBits, unsigned long dwIVLow, unsigned long dwIVHigh);


// getLicenseStatus return values
#define LIC_STATUS_NOTFOUND		0		// no license key found
#define LIC_STATUS_VALID		1		// valid license key present
#define LIC_STATUS_EXPIRED		2		// license key has expired
#define LIC_STATUS_BAD_SYSTEMID	3		// system ID does not match
#define LIC_STATUS_INVALID		4		// invalid or corrupt key
#define LIC_STATUS_BLACKLISTED	5		// license key is blacklisted
#define LIC_STATUS_USB_MISSING  6       // required USB device is missing

// obsGetActiveLicensingSystem return values (lower 16 bits)
#define LIC_SYSTEM_NONE         0		// no licensing system / external system
#define LIC_SYSTEM_SHORT_KEYS   1		// short license keys
#define LIC_SYSTEM_LONG_KEYS    2		// long license keys
#define LIC_SYSTEM_NETWORK      3		// network licensing
#define LIC_SYSTEM_SL_C2        4		// Senselock Clave 2
#define LIC_SYSTEM_SL_EL        5		// Senselock EL
// obsGetActiveLicensingSystem flags (upper 16 bits)
#define LIC_SYSTEM_FLAG_USB     1		// USB licensing is enabled
#define LIC_SYSTEM_FLAG_HW      2		// hardware locking is enabled

// Network licensing callback events
#define OBS_NETLIC_EVENT_CONNECT			0x0001
#define OBS_NETLIC_EVENT_DISCONNECT			0x0002
#define OBS_NETLIC_EVENT_LICENSE_DENIED		0x0003
#define OBS_NETLIC_EVENT_LICENSE_GRANTED	0x0004

// Network licensing callback event info (OBS_NETLIC_EVENT_LICENSE_DENIED)
#define OBS_NETLIC_EVENT_INFO_NONE				0x0000
#define OBS_NETLIC_EVENT_INFO_NO_CERT			0x0001	// server does not have matching app cert
#define OBS_NETLIC_EVENT_INFO_NO_LICENSE		0x0002	// all licenses in use
#define OBS_NETLIC_EVENT_INFO_EXPIRED			0x0003	// license is expired
#define OBS_NETLIC_EVENT_INFO_ALREADY_CONNECTED	0x0004	// client is already connected, multiple connections not allowed
#define OBS_NETLIC_EVENT_INFO_BAD_SERVER_ID		0x0005	// client is locked to a different server ID
#define OBS_NETLIC_EVENT_INFO_NOT_VALID_YET     0x0006	// app cert is not valid yet
#define OBS_NETLIC_EVENT_INFO_SERVER_DISABLED   0x0007	// server is disabled (e.g. due to missing USB device)

// USB licensing callback events
#define OBS_USB_EVENT_UNLICENSED		0x0001
#define OBS_USB_EVENT_LICENSED			0x0002

// USB licensing callback event info
#define OBS_USB_EVENT_INFO_NONE			0x0000

// Senselock storage areas
#define OBS_USB_STORAGE_READ_ONLY				0
#define OBS_USB_STORAGE_READ_WRITE				1

// Senselock encryption modes
#define OBS_USB_ENCDEC_MODE_ECB					0
#define OBS_USB_ENCDEC_MODE_CBC					1

// Senselock EL encryption keys
#define OBS_USB_ENCDEC_USER_KEY_1				12
#define OBS_USB_ENCDEC_USER_KEY_2				13
#define OBS_USB_ENCDEC_USER_KEY_3				14
#define OBS_USB_ENCDEC_USER_KEY_4				15

// pattern marking (always use the macros defined below!)
void obsidium_pattern_marker_a();
void obsidium_pattern_marker_b();
void obsidium_pattern_marker_c();
void obsidium_pattern_marker_d();
void obsidium_pattern_marker_e();
void obsidium_pattern_marker_f();

#define OBSIDIUM_ENC_START obsidium_pattern_marker_a(); obsidium_pattern_marker_c(); obsidium_pattern_marker_b();
#define OBSIDIUM_ENC_END obsidium_pattern_marker_b(); obsidium_pattern_marker_c(); obsidium_pattern_marker_a();
#define OBSIDIUM_LIC_START obsidium_pattern_marker_c(); obsidium_pattern_marker_b(); obsidium_pattern_marker_d();
#define OBSIDIUM_LIC_END obsidium_pattern_marker_d(); obsidium_pattern_marker_b(); obsidium_pattern_marker_c();
#define OBSIDIUM_VM_START obsidium_pattern_marker_e(); obsidium_pattern_marker_a(); obsidium_pattern_marker_f();
#define OBSIDIUM_VM_END obsidium_pattern_marker_f(); obsidium_pattern_marker_a(); obsidium_pattern_marker_e();

#ifdef __cplusplus
	}	// extern "C"
	#ifdef OBSIDIUM_USE_NAMESPACE
		}
	#endif
#endif

#endif
