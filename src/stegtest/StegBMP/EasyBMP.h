
#ifdef _MSC_VER 
#endif

#include <iostream>
#include <cmath>

#ifndef EasyBMP
#define EasyBMP

#ifdef __BCPLUSPLUS__ 
// The Borland compiler must use this because something
// is wrong with their cstdio file. 
#include <stdio.h>
#else 
#include <cstdio>
#endif

#ifdef __GNUC__
// If g++ specific code is ever required, this is 
// where it goes. 
#endif

#ifdef __INTEL_COMPILER
// If Intel specific code is ever required, this is 
// where it goes. 
#endif

#ifndef _DefaultXPelsPerMeter_
#define _DefaultXPelsPerMeter_
#define DefaultXPelsPerMeter 3780
// set to a default of 96 dpi 
#endif

#ifndef _DefaultYPelsPerMeter_
#define _DefaultYPelsPerMeter_
#define DefaultYPelsPerMeter 3780
// set to a default of 96 dpi
#endif

#include "EasyBMP_DataStructures.h"
#include "EasyBMP_BMP.h"
#include "EasyBMP_VariousBMPutilities.h"

#ifndef _EasyBMP_Version_
#define _EasyBMP_Version_ 1.04
#define _EasyBMP_Version_Integer_ 104
#define _EasyBMP_Version_String_ "1.04"
#endif

#ifndef _EasyBMPwarnings_
#define _EasyBMPwarnings_
#endif

void SetEasyBMPwarningsOff( void );
void SetEasyBMPwarningsOn( void );
bool GetEasyBMPwarningState( void );

#endif
