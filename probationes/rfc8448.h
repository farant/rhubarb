/* rfc8448.h - lector textus RFC 8448 (Example Handshake Traces for TLS
 * 1.3) pro probationibus: textum IMMUTATUM legit
 * (probationes/fixa/tls13/rfc8448.txt) et campum quemvis actionis
 * cuiusvis reddit - nulla hex manu copiata (tls13-plan P1).
 *
 * Forma RFC: sectio '3.  Simple 1-RTT Handshake'; actio '   {server}
 * derive secret "tls13 c hs traffic":' (caput interdum in lineam
 * proximam fluit); campus '      PRK (32 octets):  1d c8 ...' cum
 * lineis continuationis; '(empty)' = octeti nulli. Interruptiones
 * paginae ('Thomson ... [Page N]', 'RFC 8448 ... January 2019',
 * \f) praetereuntur. Numerus declaratus contra collectos verificatur:
 * lectio fracta = FALSUM (lector ipse probatur).
 *
 * Functiones INTERIORES (more ludus_toy.h): includens AMBOS aditus
 * vocare debet - rfc8448_legere et rfc8448_quaerere. */
#ifndef RFC8448_H
#define RFC8448_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include <string.h>
#include <stdlib.h>

nomen structura {
          i32 sectio;    /* 3, 5, 7 ... */
    character latus;     /* 'c' client, 's' server */
       chorda actio;     /* caput sine '{latus}  ' et ':' finali */
       chorda campus;    /* 'PRK', 'complete record', 'key expanded' */
       chorda octeti;
} Rfc8448Campus;

nomen structura {
    Xar* campi;           /* Rfc8448Campus */
} Rfc8448;

interior b32
_rfc8448_linea_pagina (
    chorda l)
{
    i32 k;

    per (k = ZEPHYRUM; k < l.mensura; k++)
    {
        si (l.datum[k] == '\f')
        {
            redde VERUM;
        }
    }
    redde (l.mensura >= VIII && memcmp(l.datum, "RFC 8448", VIII)
               == ZEPHYRUM)
        || (l.mensura >= VII && memcmp(l.datum, "Thomson", VII)
               == ZEPHYRUM);
}

interior s32
_rfc8448_hex (
    character c)
{
    si (c >= '0' && c <= '9') redde (s32)(c - '0');
    si (c >= 'a' && c <= 'f') redde (s32)(c - 'a' + X);
    redde -I;
}

/* octetos hex lineae (paria 'ab' spatiis separata) ad aedificatorem */
interior b32
_rfc8448_hex_colligere (
    constans character* s,
                   i32  n,
                    i8* capsa,
                   i32* numerus,
                   i32  capacitas)
{
    i32 k = ZEPHYRUM;

    dum (k < n)
    {
        s32 a;
        s32 b;

        si (s[k] == ' ')
        {
            k++;
            perge;
        }
        si (k + I >= n)
        {
            redde FALSUM;
        }
        a = _rfc8448_hex(s[k]);
        b = _rfc8448_hex(s[k + I]);
        si (a < ZEPHYRUM || b < ZEPHYRUM || *numerus >= capacitas)
        {
            redde FALSUM;
        }
        capsa[*numerus] = (i8)((a << IV) | b);
        (*numerus)++;
        k += II;
    }
    redde VERUM;
}

interior b32
rfc8448_legere (
               Rfc8448* r,
    constans character* via,
               Piscina* p)
{
       chorda textus  = filum_legere_totum(via, p);
          i32 i       = ZEPHYRUM;
          i32 sectio  = ZEPHYRUM;
    character latus   = ' ';
       chorda actio;
    character caput[CCLVI];
          i32 caput_n = ZEPHYRUM;
    character linea[CCLVI];   /* filum_legere_totum NUL non promittit:
                                * strstr/atoi in copia terminata */

    r->campi       = xar_creare(p, (i32)magnitudo(Rfc8448Campus));
    actio.datum    = NIHIL;
    actio.mensura  = ZEPHYRUM;
    si (textus.mensura == ZEPHYRUM || r->campi == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < textus.mensura)
    {
                    chorda  l;
                       i32  j = i;
        constans character* s;

        dum (j < textus.mensura && textus.datum[j] != '\n')
        {
            j++;
        }
        l.datum    = textus.datum + i;
        l.mensura  = j - i;
        i          = j + I;
        si (   l.mensura == ZEPHYRUM || l.mensura >= CCLVI
            || _rfc8448_linea_pagina(l))
        {
            perge;
        }
        memcpy(linea, l.datum, l.mensura);
        linea[l.mensura]  = '\0';
        s                 = linea;
        /* sectio: '3.  Simple 1-RTT Handshake' */
        si (s[ZEPHYRUM] >= '1' && s[ZEPHYRUM] <= '9')
        {
            sectio = (i32)atoi(s);
            perge;
        }
        /* caput actionis */
        si (l.mensura > XI && memcmp(s, "   {", IV) == ZEPHYRUM)
        {
            latus    = (s[IV] == 'c') ? 'c' : 's';
            caput_n  = ZEPHYRUM;
            j        = IV;
            dum (j < l.mensura && s[j] != '}')
            {
                j++;
            }
            j += III;   /* '}' + duo spatia */
            dum (j < l.mensura && caput_n + I < CCLVI)
            {
                caput[caput_n++] = s[j++];
            }
            actio = chorda_ex_buffer((i8*)caput, caput_n);
            actio = chorda_transcribere(actio, p);
            perge;
        }
        /* campus: sex spatia + titulus + ' (N octets):' */
        si (   l.mensura > VI && memcmp(s, "      ", VI) == ZEPHYRUM
            && s[VI] != ' ')
        {
            constans character* par = strstr(s, " octets):");
            constans character* aperta;
                           i32  numerus = ZEPHYRUM;
                           i32  declaratus;
                            i8* capsa;
                 Rfc8448Campus* c;

            si (par == NIHIL || par > s + l.mensura)
            {
                /* caput in lineam secundam fluens ('secret)') aut
                 * 'salt:  0 (all zero octets)' - non campus
                 * octetorum */
                perge;
            }
            aperta = par;
            dum (aperta > s && *aperta != '(')
            {
                aperta--;
            }
            declaratus = (i32)atoi(aperta + I);
            capsa = (i8*)piscina_allocare(p, (memoriae_index)(declaratus
                + I));
            si (capsa == NIHIL)
            {
                redde FALSUM;
            }
            par += IX;   /* ' octets):' */
            si (   strncmp(par, "  (empty)", IX) != ZEPHYRUM
                && !_rfc8448_hex_colligere(par, (i32)(s + l.mensura
                    - par),
                       capsa, &numerus, declaratus))
            {
                redde FALSUM;
            }
            /* continuationes: novem+ spatia, solum hex */
            dum (numerus < declaratus && i < textus.mensura)
            {
                chorda m;

                j = i;
                dum (j < textus.mensura && textus.datum[j] != '\n')
                {
                    j++;
                }
                m.datum    = textus.datum + i;
                m.mensura  = j - i;
                i          = j + I;
                si (m.mensura == ZEPHYRUM || _rfc8448_linea_pagina(m))
                {
                    perge;
                }
                si (!_rfc8448_hex_colligere(
                        (constans character*)m.datum, m.mensura,
                        capsa, &numerus, declaratus))
                {
                    redde FALSUM;
                }
            }
            si (numerus != declaratus)
            {
                redde FALSUM;
            }
            c          = (Rfc8448Campus*)xar_addere(r->campi);
            c->sectio  = sectio;
            c->latus   = latus;
            c->actio   = actio;
            c->campus  = chorda_transcribere(chorda_ex_buffer(
                (i8*)(linea + VI), (i32)(aperta - I - (s + VI))), p);
            c->octeti  = chorda_ex_buffer(capsa, numerus);
        }
    }
    redde xar_numerus(r->campi) > ZEPHYRUM;
}

/* campus n-tus (ordo ab 0) sectionis/lateris cuius actio cum 'actio'
 * INCIPIT et campus 'campus' AEQUAT; NIHIL si abest */
interior constans Rfc8448Campus*
rfc8448_quaerere (
       constans Rfc8448* r,
                    i32  sectio,
              character  latus,
     constans character* actio,
                    i32  ordo,
     constans character* campus)
{
    i32 k;
    i32 n   = ZEPHYRUM;
    i32 la  = (i32)strlen(actio);
    i32 lc  = (i32)strlen(campus);

    per (k = ZEPHYRUM; k < xar_numerus(r->campi); k++)
    {
        constans Rfc8448Campus* c = (constans Rfc8448Campus*)
            xar_obtinere(r->campi, k);

        si (   c->sectio == sectio && c->latus == latus
            && c->actio.mensura >= la
            && memcmp(c->actio.datum, actio, la) == ZEPHYRUM
            && c->campus.mensura == lc
            && memcmp(c->campus.datum, campus, lc) == ZEPHYRUM)
        {
            si (n == ordo)
            {
                redde c;
            }
            n++;
        }
    }
    redde NIHIL;
}

#endif /* RFC8448_H */
