/* energia_macos.m - status pilae per IOKit (fontes energiae). API C
 * puri (IOPowerSources), sed capita CoreFoundation non C89: ergo .m,
 * ut fenestra et clipboard. Pila interna prima (kIOPSInternalBattery
 * Type); nulla (machina mensae): FALSUM. */

/* capita systematis ANTE latina.h: macra eius ('character', numeri
 * Romani) capita CoreFoundation alioquin corrumpunt */
#include "postulata_macos.h"   /* PRIMUM: ante capita Apple */
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <string.h>

#include "energia.h"

/* numerus integer ex dictionario; FALSUM si abest */
interior b32
numerus_ex (
    CFDictionaryRef  d,
        CFStringRef  clavis,
               s32*  valor)
{
    CFNumberRef n;
            int v;

    n = (CFNumberRef)CFDictionaryGetValue(d, clavis);
    si (!n || CFGetTypeID(n) != CFNumberGetTypeID()
        || !CFNumberGetValue(n, kCFNumberIntType, &v))
    {
        redde FALSUM;
    }
    *valor = (s32)v;
    redde VERUM;
}

b32
energia_status (
    EnergiaStatus* status)
{
       CFTypeRef  info;
      CFArrayRef  index;
 CFDictionaryRef  d;
     CFStringRef  genus;
    CFBooleanRef  oneratur;
             s32  actualis;
             s32  maxima;
            CFIndex  i;
            CFIndex  n;
             b32  inventum;

    si (!status)
    {
        redde FALSUM;
    }
    memset(status, ZEPHYRUM, magnitudo(*status));
    inventum = FALSUM;
    info = IOPSCopyPowerSourcesInfo();
    si (!info)
    {
        redde FALSUM;
    }
    index = IOPSCopyPowerSourcesList(info);
    n = index ? CFArrayGetCount(index) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < n && !inventum; i++)
    {
        d = IOPSGetPowerSourceDescription(info,
            CFArrayGetValueAtIndex(index, i));
        si (!d)
        {
            perge;
        }
        genus = (CFStringRef)CFDictionaryGetValue(d,
            CFSTR(kIOPSTypeKey));
        si (   !genus
            || CFStringCompare(genus, CFSTR(kIOPSInternalBatteryType),
                   ZEPHYRUM) != kCFCompareEqualTo)
        {
            perge;
        }
        si (   !numerus_ex(d, CFSTR(kIOPSCurrentCapacityKey), &actualis)
            || !numerus_ex(d, CFSTR(kIOPSMaxCapacityKey), &maxima)
            || maxima <= ZEPHYRUM)
        {
            perge;
        }
        status->centesimae = (actualis * C + maxima / II) / maxima;
        status->centesimae = status->centesimae > C ? C
                           : status->centesimae < ZEPHYRUM ? ZEPHYRUM
                           : status->centesimae;
        oneratur = (CFBooleanRef)CFDictionaryGetValue(d,
            CFSTR(kIOPSIsChargingKey));
        status->oneratur  = oneratur && CFBooleanGetValue(oneratur);
        status->adest     = VERUM;
        inventum          = VERUM;
    }
    si (index)
    {
        CFRelease(index);
    }
    CFRelease(info);
    redde inventum;
}
