/* probatio_pictor_documentum_aurum.c - volumen pictoris ut aurum
 *
 * Oraculum ante sectionem (scriba-plan H0): volumen a
 * pictor_documentum HODIERNO scriptum - ictus per duos checkpoints,
 * revocatio, ramus cum checkpoint post eum, refectio recusata,
 * apertio iterata - in textum effunditur (acta sine momento,
 * plagulae cum contento et origine, massae, sigilla, cursores) et
 * cum auro commisso confertur. Post extractionem machinae (historia,
 * H1-H2) idem textus OCTETIS IISDEM exire debet.
 *
 * PICTOR_DOCUMENTUM_AURUM_SCRIBERE=1 aurum scribit (solum ante
 * sectionem, aut mutatione formae voluminis NOMINATA).
 *
 * pictor-strata L2 (mutatio formae NOMINATA): checkpoints codificati,
 * sigillum status stratorum - aurum repromotum. Aurum VETUS congelatum
 * (fixa/pictor_documentum/aurum_ante_strata.txt): sigillum COMPOSITI
 * omnis gradus = sigillum vetus eius gradus - picturae ante strata
 * pixelis iisdem redduntur. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "pictor_documentum.h"
#include "credo.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AURUM_VIA "probationes/fixa/pictor_documentum/aurum.txt"
#define AURUM_ANTE_STRATA \
    "probationes/fixa/pictor_documentum/aurum_ante_strata.txt"

hic_manens Piscina* piscina_aurei;

/* sigillum gradus in auro vetere ("gradus <g>" ... "  sigillum <h>") */
interior chorda
sigillum_vetus (
    constans character* gradus)
{
      FILE* f;
 character  linea[CCLVI];
 character  quaesitum[CXXVIII];
       b32  in_gradu;
       i32  n;

    sprintf(quaesitum, "gradus %s\n", gradus);
    in_gradu  = FALSUM;
    f         = lectiones_fopen(AURUM_ANTE_STRATA, "rb");
    si (!f)
    {
        redde chorda_ex_literis("", piscina_aurei);
    }
    dum (fgets(linea, (integer)magnitudo(linea), f))
    {
        si (strcmp(linea, quaesitum) == ZEPHYRUM)
        {
            in_gradu = VERUM;
            perge;
        }
        si (in_gradu && strncmp(linea, "  sigillum ", XI) == ZEPHYRUM)
        {
            fclose(f);
            n = (i32)strlen(linea + XI);
            si (n > ZEPHYRUM && linea[XI + n - I] == '\n')
            {
                linea[XI + n - I] = '\0';
            }
            redde chorda_ex_literis(linea + XI, piscina_aurei);
        }
    }
    fclose(f);
    redde chorda_ex_literis("", piscina_aurei);
}

/* ictus unus: linea horizontalis (x0..x1, y) magnitudine I */
interior chorda
ictus (
    Piscina* p,
        s32  y)
{
    chorda s;

    s = chorda_ex_literis("<ictus instrumentum=\"penicillus\""
                          " color=\"0\" magnitudo=\"1\">"
                          "<punctum x=\"0\" y=\"", p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s,
        chorda_ex_literis("\"/><punctum x=\"31\" y=\"",
        p), p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\"/></ictus>", p), p);
    redde s;
}

interior vacuum
linea_numeri (
     ChordaAedificator* a,
    constans character* titulus,
                   s64  n)
{
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_character(a, ' ');
    chorda_aedificator_appendere_s32(a, (s32)n);
    chorda_aedificator_appendere_character(a, '\n');
}

/* status documenti: cursor, finis, vivi, sigillum */
interior vacuum
status_effundere (
     ChordaAedificator* a,
    constans character* gradus,
      PictorDocumentum* doc,
               Piscina* p)
{
    chorda_aedificator_appendere_literis(a, "gradus ");
    chorda_aedificator_appendere_literis(a, gradus);
    chorda_aedificator_appendere_character(a, '\n');
    linea_numeri(a, "  cursor", pictor_documentum_cursor(doc));
    linea_numeri(a, "  finis", pictor_documentum_finis(doc));
    linea_numeri(a, "  vivi",
        (s64)pictor_documentum_numerus_vivorum(doc));
    chorda_aedificator_appendere_literis(a, "  sigillum ");
    chorda_aedificator_appendere_chorda(a,
        pictor_documentum_sigillum_hex(doc, p));
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_literis(a, "  compositum ");
    chorda_aedificator_appendere_chorda(a,
        pictor_documentum_sigillum_compositi_hex(doc, p));
    chorda_aedificator_appendere_character(a, '\n');
    /* pixela visa = picturae veteris (aurum ante strata) */
    imprimere("  gradus %s: compositum = sigillum vetus\n", gradus);
    CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_compositi_hex(doc,
        p), sigillum_vetus(gradus));
}

/* volumen totum: acta (sine momento), plagulae cum contento */
interior vacuum
volumen_effundere (
    ChordaAedificator* a,
              Volumen* vol,
              Piscina* p)
{
             Xar* acta;
             Xar* plagulae;
    VolumenActum* actum;
  VolumenPlagula* plagula;
             i32  i;
             b32  inventum;

    acta = volumen_acta_legere(vol, ZEPHYRUM, p);
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        actum = (VolumenActum*)xar_obtinere(acta, i);
        chorda_aedificator_appendere_literis(a, "actum ");
        chorda_aedificator_appendere_s32(a, (s32)actum->seq);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, actum->genus);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, actum->datum);
        chorda_aedificator_appendere_character(a, '\n');
    }
    plagulae = volumen_plagulas_enumerare(vol, p);
    per (i = ZEPHYRUM; i < xar_numerus(plagulae); i++)
    {
        plagula = (VolumenPlagula*)xar_obtinere(plagulae, i);
        chorda_aedificator_appendere_literis(a, "plagula ");
        chorda_aedificator_appendere_chorda(a, plagula->via);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, plagula->origo);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, plagula->sigillum_hex);
        chorda_aedificator_appendere_literis(a, "\n  contentum ");
        chorda_aedificator_appendere_chorda(a,
            volumen_plagulam_promere(vol, plagula->via, p, &inventum));
        chorda_aedificator_appendere_character(a, '\n');
    }
    linea_numeri(a, "summa_actorum", volumen_summa_actorum(vol));
    linea_numeri(a, "summa_plagularum", volumen_summa_plagularum(vol));
    linea_numeri(a, "summa_massarum", volumen_summa_massarum(vol));
}

interior chorda
aurum_legere (
    Piscina* p)
{
     FILE* f;
    chorda c;
      long n;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    f          = lectiones_fopen(AURUM_VIA, "rb");
    si (!f)
    {
        redde c;
    }
    fseek(f, ZEPHYRUM, SEEK_END);
    n = ftell(f);
    fseek(f, ZEPHYRUM, SEEK_SET);
    c.datum    = (i8*)piscina_allocare(p, (memoriae_index)n + I);
    c.mensura  = (i32)fread(c.datum, I, (size_t)n, f);
    fclose(f);
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
       PictorDocumentum* doc2;
      ChordaAedificator* a;
                 chorda  textus;
                 chorda  aurum;
                    i32  i;
                    i32  linea;
                    b32  scribere;

    piscina =
        piscina_generare_dynamicum("probatio_pictor_documentum_aurum",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    piscina_aurei  = piscina;
    intern         = internamentum_creare(piscina);
    thema_initiare();
    a = chorda_aedificator_creare(piscina, (memoriae_index)(LXIV * M));

    imprimere("\n--- Volumen: V ictus, checkpoints II et IV ---\n");
    vol = volumen_temporarium(piscina,
        "probatio_pictor_documentum_aurum");
    CREDO_NON_NIHIL(vol);
    doc = pictor_documentum_creare(piscina, intern, vol, "", XXXII, XVI,
        II);
    CREDO_NON_NIHIL(doc);
    status_effundere(a, "creatum", doc, piscina);
    per (i = ZEPHYRUM; i < V; i++)
    {
        CREDO_VERUM(pictor_documentum_actum(doc,
            ictus(piscina, (s32)(II + III * i))) > ZEPHYRUM);
    }
    status_effundere(a, "quinque_ictus", doc, piscina);

    imprimere("\n--- Revocare III, ramus, checkpoint post ramum ---\n");
    per (i = ZEPHYRUM; i < III; i++)
    {
        CREDO_VERUM(pictor_documentum_revocare(doc));
    }
    status_effundere(a, "revocata_tria", doc, piscina);
    CREDO_VERUM(pictor_documentum_actum(doc, ictus(piscina, III))
        > ZEPHYRUM);
    CREDO_VERUM(pictor_documentum_actum(doc, ictus(piscina, IV))
        > ZEPHYRUM);
    status_effundere(a, "ramus_duo_ictus", doc, piscina);
    CREDO_FALSUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(pictor_documentum_revocare(doc));
    status_effundere(a, "revocatum_post_ramum", doc, piscina);
    CREDO_VERUM(pictor_documentum_reficere(doc));
    status_effundere(a, "refectum", doc, piscina);
    CREDO_VERUM(pictor_documentum_verificare(doc));

    imprimere("\n--- Aperire iterum: status idem ---\n");
    doc2 = pictor_documentum_aperire(piscina, intern, vol, "");
    CREDO_NON_NIHIL(doc2);
    status_effundere(a, "apertum", doc2, piscina);
    volumen_effundere(a, vol, piscina);
    textus = chorda_aedificator_spectare(a);

    imprimere("\n--- Aurum: octeti iidem ---\n");
    scribere = (b32)(lectiones_ambitus(
        "PICTOR_DOCUMENTUM_AURUM_SCRIBERE") != NIHIL);
    si (scribere)
    {
        FILE* f = lectiones_fopen(AURUM_VIA, "wb");
        CREDO_NON_NIHIL(f);
        si (f)
        {
            fwrite(textus.datum, I, (size_t)textus.mensura, f);
            fclose(f);
        }
        imprimere("  aurum scriptum: %s (%d octeti)\n", AURUM_VIA,
            (integer)textus.mensura);
    }
    aurum = aurum_legere(piscina);
    CREDO_NON_NIHIL(aurum.datum);
    si (aurum.datum && !chorda_aequalis(aurum, textus))
    {
        /* linea prima discors nominatur */
        linea = I;
        per (i = ZEPHYRUM; i < textus.mensura && i < aurum.mensura
            && textus.datum[i] == aurum.datum[i]; i++)
        {
            si (textus.datum[i] == '\n')
            {
                linea++;
            }
        }
        imprimere("  FRACTA: aurum discors in linea %d (octetus %d; "
            "aurum %d, hodie %d)\n", (integer)linea, (integer)i,
            (integer)aurum.mensura, (integer)textus.mensura);
    }
    CREDO_VERUM(aurum.datum && chorda_aequalis(aurum, textus));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
