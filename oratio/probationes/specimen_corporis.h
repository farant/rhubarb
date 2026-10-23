/* specimen_corporis.h - quae plagulae corporis md probantur, et
 * quantum cuiusque (probatio_oratio_stml, probatio_oratio_canon)
 *
 * CUR (2026-10-22, parca 01M5GKMDGSVZEKPXA54Y0VC87H et
 * 01M5GH4FA11PP0GGZ9TZ5E2Y7J): specimen olim 'linea quinta quaeque'
 * indicis oratio/build/corpus_md.txt erat, et apex portae plagulam
 * MAXIMAM speciminis sequitur (piscina nova per plagulam, circuitus
 * STML ~IV-V KiB apicis per octetum fontis). Plagula nova ubicumque
 * addita lineas sequentes movebat: silva/phase-log.md (788 KB) in
 * specimen cecidit et apex ~1 -> ~3.3 GiB saluit. Ergo duo:
 *
 *   ELECTIO per FNV-1a viae (modulo V), non per positionem - plagula
 *   addita ceteras non movet. ORATIO_CORPUS_TOTUS=1 omnes eligit.
 *
 *   CAPUT: plagula ultra SPECIMEN_OCTETI_MAXIMI per caput suum
 *   probatur, ad lineam vacuam ultimam ante finem sectum (aliter ad
 *   '\n' ultimam, aliter ad limitem puncti codicis UTF-8) - contentum
 *   magnarum plagularum adhuc probatur, apex autem ligatus manet.
 *
 * Functiones INTERIORES (mos probationes/ludus_toy.h): probatio quae
 * hoc caput includit AMBAS vocare debet (-Wunused-function). */
#ifndef SPECIMEN_CORPORIS_H
#define SPECIMEN_CORPORIS_H

#include "latina.h"
#include "friatio.h"
#include <stdlib.h>
#include <string.h>

/* CXXVIII KiB: circuitus STML ~DC MiB, canon ~CL MiB (mensura
 * 2026-10-22: CLXXII KB -> DCCCIV / CXCVIII MiB) */
#define SPECIMEN_OCTETI_MAXIMI (CXXVIII * MXXIV)

/* VERUM si plagula 'via' (relativa ad radicem, ut in indice) in
 * specimine est */
interior b32
_specimen_eligere (
    constans character* via)
{
    si (getenv("ORATIO_CORPUS_TOTUS") != NIHIL)
    {
        redde VERUM;
    }
    redde (b32)(friatio_fnv1a_literis(via, (i32)strlen(via)) % V
                == ZEPHYRUM);
}

/* Mensura capitis probandi: 'mensura' ipsa si intra limitem; aliter
 * finis lineae vacuae ultimae (post '\n\n') ante limitem, aliter post
 * '\n' ultimam, aliter limes ad initium puncti codicis retractus.
 * textus[reddita] = '\0' ponitur (buffer _plagulam_legere octetum
 * unum ultra fert). */
interior i32
_caput_plagulae (
    character* textus,
          i32  mensura)
{
    s32 i;      /* signatus: ambulatio deorsum ad ZEPHYRUM */
    i32 finis;

    si (mensura <= (i32)SPECIMEN_OCTETI_MAXIMI)
    {
        redde mensura;
    }
    finis = ZEPHYRUM;
    per (i = (s32)SPECIMEN_OCTETI_MAXIMI - I; i > ZEPHYRUM; i--)
    {
        si (textus[i] == '\n' && textus[i - I] == '\n')
        {
            finis = (i32)i + I;
            frange;
        }
    }
    si (finis == ZEPHYRUM)
    {
        per (i = (s32)SPECIMEN_OCTETI_MAXIMI - I; i >= ZEPHYRUM; i--)
        {
            si (textus[i] == '\n')
            {
                finis = (i32)i + I;
                frange;
            }
        }
    }
    si (finis == ZEPHYRUM)
    {
        finis = (i32)SPECIMEN_OCTETI_MAXIMI;
        dum (   finis > ZEPHYRUM
             && ((i8)textus[finis] & 0xC0) == 0x80)
        {
            finis--;
        }
    }
    textus[finis] = '\0';
    redde finis;
}

#endif /* SPECIMEN_CORPORIS_H */
