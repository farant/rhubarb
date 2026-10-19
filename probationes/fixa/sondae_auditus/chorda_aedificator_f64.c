#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <locale.h>
#include <float.h>

static Piscina* pp;

static void casus_f64(const char* tag, double v, i32 dec)
{
    ChordaAedificator* a = chorda_aedificator_creare(pp, 16);
    b32 r = chorda_aedificator_appendere_f64(a, v, dec);
    chorda s = chorda_aedificator_spectare(a);
    printf("F64\t%s\t%u\tr=%d\tlen=%u\t[%.*s]\n", tag, dec, (int)r, s.mensura, (int)(s.mensura > 60 ? 60 : s.mensura), (char*)s.datum);
}

int main(void)
{
    double zero = 0.0;
    pp = piscina_generare_dynamicum("sonda", 65536);
    casus_f64("3.14159", 3.14159, 2);
    casus_f64("-0.5", -0.5, 0);
    casus_f64("-0.5", -0.5, 1);
    casus_f64("-0.001", -0.001, 2);
    casus_f64("-0.0", -0.0, 2);
    casus_f64("0.999", 0.999, 2);
    casus_f64("9.9995", 9.9995, 3);
    casus_f64("0.5", 0.5, 0);
    casus_f64("1.5", 1.5, 0);
    casus_f64("2.5", 2.5, 0);
    casus_f64("0.125", 0.125, 2);
    casus_f64("1e20", 1e20, 2);
    casus_f64("4294967296.5", 4294967296.5, 1);
    casus_f64("9.3e18", 9.3e18, 0);
    casus_f64("5e-324", 5e-324, 30);
    casus_f64("NaN", zero/zero, 2);
    casus_f64("+Inf", 1.0/zero, 2);
    casus_f64("-Inf", -1.0/zero, 2);
    casus_f64("1", 1.0, 0);
    casus_f64("1", 1.0, 30);
    casus_f64("1", 1.0, 31);
    casus_f64("1", 1.0, 0xFFFFFFFFu);
    casus_f64("1e100", 1e100, 30);
    casus_f64("1e100", 1e100, 2);
    casus_f64("1e128", 1e128, 2);
    casus_f64("1e130", 1e130, 0);
    casus_f64("1e200", 1e200, 2);
    casus_f64("DBL_MAX", DBL_MAX, 0);
    casus_f64("-1e130", -1e130, 0);
    if (setlocale(LC_NUMERIC, "de_DE.UTF-8") || setlocale(LC_NUMERIC, "de_DE"))
    {
        casus_f64("LOCALE_de_3.14159", 3.14159, 2);
    }
    else printf("LOCALE de_DE indisponibilis\n");
    return 0;
}
