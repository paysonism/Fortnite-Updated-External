/*
	Obsidium protection API
	
*/
#ifndef OBSIDIUM_H
#define OBSIDIUM_H

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || (defined(__BORLANDC__) && defined(__clang__))
	#include "obsidium64.h"
#elif defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__i386) || defined(__IA32__) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__I86__) || defined(__x86_64) || defined(__x86_64__)  || (defined(__BORLANDC__) && !defined(__clang__))
	#include "obsidium32.h"
#else
	#error Unable to detect CPU architecture, please include obsidium32.h or obsidium64.h directly
#endif

#endif
