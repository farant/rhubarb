/* xml_commune.h - adiutores sondarum xml */
#include "latina.h"
#include "xml.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sd_chorda(chorda* c)
{
    i32 k;
    if (!c) { printf("(NIHIL)"); return; }
    printf("\"");
    for (k = 0; k < c->mensura; k++) {
        unsigned char b = (unsigned char)c->datum[k];
        if (b < 32 || b == '"' || b == '\\' || b > 126) printf("\\x%02x", b);
        else printf("%c", b);
    }
    printf("\"");
}

static void sd_arbor(XmlNodus* n)
{
    i32 k, na, nl;
    if (!n) { printf("NIHIL"); return; }
    if (n->genus == XML_NODUS_TEXTUS) { printf("T:"); sd_chorda(n->valor); return; }
    if (n->genus == XML_NODUS_COMMENTUM) { printf("C:"); sd_chorda(n->valor); return; }
    printf("E:"); sd_chorda(n->titulus);
    na = n->attributa ? xar_numerus(n->attributa) : 0;
    for (k = 0; k < na; k++) {
        XmlAttributum* a = (XmlAttributum*)xar_obtinere(n->attributa, k);
        printf(" @"); sd_chorda(a->titulus); printf("="); sd_chorda(a->valor);
    }
    nl = n->liberi ? xar_numerus(n->liberi) : 0;
    if (nl) {
        printf("[");
        for (k = 0; k < nl; k++) {
            if (k) printf(",");
            sd_arbor(*(XmlNodus**)xar_obtinere(n->liberi, k));
        }
        printf("]");
    }
}

static void sd_legere_n(const char* titulus_casus, const char* data, i32 len,
                        Piscina* p, InternamentumChorda* it)
{
    chorda in; XmlResultus r; char* buf;
    buf = (char*)malloc(len ? (size_t)len : 1);  /* exacte, sine NUL */
    memcpy(buf, data, (size_t)len);
    in.datum = (i8*)buf; in.mensura = len;
    r = xml_legere(in, p, it);
    printf("%-28s succ=%d status=%d lin=%u col=%u err=%u : ", titulus_casus,
           (int)r.successus, (int)r.status, r.linea_erroris, r.columna_erroris,
           r.error.mensura);
    sd_arbor(r.radix);
    printf("\n");
    free(buf);
}

#define SD_LEGERE(t, s) sd_legere_n(t, s, (i32)(sizeof(s) - 1), p, it)
