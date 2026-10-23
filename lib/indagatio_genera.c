/* indagatio_genera.c - GENERA QUAERENDA domus (indagatio S2). Vide
 * indagatio_genera.h.
 *
 * Ordines in piscina conspectus aedificantur (conspectus copiat quod
 * ostendit). Textus ostensus anglicus, ut vicus; separator " - "
 * ASCII (litterae vici). Libri per scriba_liber lectione sola: index
 * absens nihil scribit. */

#include "indagatio_genera.h"
#include "indagatio.h"
#include "conspectus_infixa.h"
#include "scriba_liber.h"
#include "fasti.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
vacua (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

interior IndagatioConspectus*
conspectum_novum (
    Piscina* piscina)
{
    IndagatioConspectus* v;

    v = (IndagatioConspectus*)piscina_allocare(piscina,
        magnitudo(IndagatioConspectus));
    si (!v)
    {
        redde NIHIL;
    }
    v->ordines = xar_creare(piscina, (i32)magnitudo(IndagatioOrdo));
    redde v->ordines ? v : NIHIL;
}

/* chordae iam in piscina conspectus */
interior vacuum
ordinem_addere (
    IndagatioConspectus* v,
                 chorda  titulus,
                 chorda  contextus,
                 chorda  corpus,
                 chorda  semita)
{
    IndagatioOrdo* o;

    o = (IndagatioOrdo*)xar_addere(v->ordines);
    si (!o)
    {
        redde;
    }
    o->titulus    = titulus;
    o->contextus  = contextus;
    o->corpus     = corpus;
    o->semita     = semita;
}

/* "Mon 19 Oct 2026" */
interior vacuum
diem_scribere (
    ChordaAedificator* a,
                 Dies  d,
              Piscina* piscina)
{
    character numerus[XVI];

    chorda_aedificator_appendere_chorda(a, fasti_nomen_diei_hebdomadis(
        fasti_dies_hebdomadis(d), VERUM, FASTI_LINGUA_ANGLICA,
        piscina));
    sprintf(numerus, " %d ", (integer)d.dies);
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_chorda(a, fasti_nomen_mensis(d.mensis,
        VERUM, FASTI_LINGUA_ANGLICA, piscina));
    sprintf(numerus, " %d", (integer)d.annus);
    chorda_aedificator_appendere_literis(a, numerus);
}

interior vacuum
diem_iso_scribere (
    ChordaAedificator* a,
                 Dies  d)
{
    character buffer[XVI];

    sprintf(buffer, "%04d-%02d-%02d", (integer)d.annus,
        (integer)d.mensis, (integer)d.dies);
    chorda_aedificator_appendere_literis(a, buffer);
}

/* lineae separatore '\n' iunctae, a prima data */
interior chorda
lineas_iungere (
        Xar* lineae,
        i32  prima,
    Piscina* piscina)
{
    ChordaAedificator* a;
                  i32  i;

    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    per (i = prima; i < xar_numerus(lineae); i++)
    {
        si (i > prima)
        {
            chorda_aedificator_appendere_character(a, '\n');
        }
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            lineae, i));
    }
    redde chorda_aedificator_finire(a);
}

/* spatia initialia remota */
interior chorda
sinistram_purgare (
    chorda c)
{
    dum (c.mensura > ZEPHYRUM && c.datum[ZEPHYRUM] == ' ')
    {
        c.datum++;
        c.mensura--;
    }
    redde c;
}

/* rubricae ut "#a #b" (vacua si nullae) */
interior vacuum
rubricas_scribere (
         ChordaAedificator* a,
                       Xar* rubricae,
        constans character* praefixum)
{
    i32 i;

    per (i = ZEPHYRUM; rubricae && i < xar_numerus(rubricae); i++)
    {
        chorda_aedificator_appendere_literis(a, i
            == ZEPHYRUM ? praefixum
            : " ");
        chorda_aedificator_appendere_character(a, '#');
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            rubricae, i));
    }
}

interior chorda
literis_iungere (
     constans character* a,
                 chorda  b,
                Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis(a, piscina), b, piscina);
}


/* ==================================================
 * Genus 'res' (agenda per-rem)
 * ================================================== */

interior vacuum*
res_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
       IndagatioFontes* f;
    constans AgendaRes* r;
   IndagatioConspectus* v;
     ChordaAedificator* a;
                chorda  contextus;
                chorda  semita;
                   i32  i;

    (vacuum)repertorium;
    f = (IndagatioFontes*)ctx;
    v = conspectum_novum(piscina);
    r = v ? agenda_res(f->agenda, clavis) : NIHIL;
    si (!r)
    {
        redde v;
    }
    /* contextus: perfecta, assignata, sine die; rubricae */
    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    si (r->perfecta)
    {
        chorda_aedificator_appendere_literis(a, "done ");
        diem_scribere(a, r->dies_perfectionis, piscina);
    }
    alioquin si (r->dies_posita)
    {
        chorda_aedificator_appendere_literis(a, "due ");
        diem_scribere(a, r->dies, piscina);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "no date");
    }
    rubricas_scribere(a, r->rubricae, " - ");
    contextus = chorda_aedificator_finire(a);
    /* semita: dies assignata, aliter agenda */
    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    si (r->dies_posita)
    {
        chorda_aedificator_appendere_literis(a, "calendarium/dies/");
        diem_iso_scribere(a, r->dies);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "calendarium/agenda");
    }
    chorda_aedificator_appendere_literis(a, "?res_electa=");
    chorda_aedificator_appendere_chorda(a, r->id);
    semita = chorda_aedificator_finire(a);
    /* corpus: notae, deinde rubricae */
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    per (i = ZEPHYRUM; r->notae && i < xar_numerus(r->notae); i++)
    {
        chorda_aedificator_appendere_chorda(a,
            ((AgendaNota*)xar_obtinere(
            r->notae, i))->textus);
        chorda_aedificator_appendere_character(a, '\n');
    }
    per (i = ZEPHYRUM; r->rubricae && i < xar_numerus(r->rubricae); i++)
    {
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            r->rubricae, i));
        chorda_aedificator_appendere_character(a, ' ');
    }
    ordinem_addere(v, chorda_transcribere(r->titulus, piscina),
        contextus, chorda_aedificator_finire(a), semita);
    redde v;
}

interior i32
res_numerus (
    constans vacuum* documentum)
{
    redde agenda_numerus_rerum((constans AgendaDocumentum*)documentum);
}

interior chorda
res_id (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    (vacuum)piscina;
    redde agenda_res_index((constans AgendaDocumentum*)documentum,
        index)->id;
}

interior chorda
res_vestigium (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    redde agenda_rem_codificare((constans AgendaDocumentum*)documentum,
        index, piscina);
}

/* clavis rei = id suum */
interior vacuum
res_claves (
             vacuum* ctx,
    constans vacuum* documentum,
                i32  index,
                Xar* claves,
            Piscina* piscina)
{
    (vacuum)ctx;
    (vacuum)piscina;
    *(chorda*)xar_addere(claves) = agenda_res_index(
        (constans AgendaDocumentum*)documentum, index)->id;
}

interior Xar*
res_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    IndagatioFontes* f;
                Xar* claves;
                i32  i;

    (vacuum)repertorium;
    f       = (IndagatioFontes*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; claves && i < agenda_numerus_rerum(f->agenda);
         i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            agenda_res_index(f->agenda, i)->id, piscina);
    }
    redde claves;
}


/* ==================================================
 * Genus 'notae' (libri dierum, ordo per paginam)
 * ================================================== */

interior vacuum*
notas_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
       IndagatioFontes* f;
   IndagatioConspectus* v;
                  Dies  d;
                   Xar* nomina;
                chorda  praefixum;
                chorda  semita;
                   i32  i;

    (vacuum)repertorium;
    f = (IndagatioFontes*)ctx;
    v = conspectum_novum(piscina);
    si (!v || !fasti_parsare_iso(clavis, &d))
    {
        redde v;
    }
    praefixum  = literis_iungere("dies/", clavis, piscina);
    semita     = literis_iungere("calendarium/dies/", clavis, piscina);
    nomina     = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!scriba_liber_nomina_legere(piscina, f->intern, f->volumen,
            chorda_ut_cstr(praefixum, piscina), nomina))
    {
        redde v;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        ChordaAedificator* a;
                      Xar* lineae;
                character  numerus[XXXII];

        lineae = xar_creare(piscina, (i32)magnitudo(chorda));
        (vacuum)scriba_liber_paginam_legere(piscina, f->intern,
            f->volumen, chorda_ut_cstr(praefixum, piscina),
            *(chorda*)xar_obtinere(nomina, i), lineae);
        si (xar_numerus(lineae) == ZEPHYRUM)
        {
            perge;
        }
        a = chorda_aedificator_creare(piscina, (memoriae_index)XXXII);
        diem_scribere(a, d, piscina);
        si (xar_numerus(nomina) > I)
        {
            sprintf(numerus, " - page %d of %d", (integer)(i + I),
                (integer)xar_numerus(nomina));
            chorda_aedificator_appendere_literis(a, numerus);
        }
        /* D5 S3c: semita paginam suam fert (?notae.pagina=) */
        ordinem_addere(v, chorda_aedificator_finire(a), vacua(),
            lineas_iungere(lineae, ZEPHYRUM, piscina),
            chorda_concatenare(
            semita, literis_iungere("?notae.pagina=",
            *(chorda*)xar_obtinere(nomina, i), piscina), piscina));
    }
    redde v;
}

/* dies quorum index libri exstat: plagulae
 * "dies/<YYYY-MM-DD>/paginae" */
interior Xar*
notas_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    IndagatioFontes* f;
                Xar* plagulae;
                Xar* claves;
                i32  i;

    (vacuum)repertorium;
    f         = (IndagatioFontes*)ctx;
    claves    = xar_creare(piscina, (i32)magnitudo(chorda));
    plagulae  = volumen_plagulas_enumerare(f->volumen, piscina);
    per (i = ZEPHYRUM; claves && plagulae
        && i < xar_numerus(plagulae); i++)
    {
        chorda via = ((VolumenPlagula*)xar_obtinere(plagulae, i))->via;
        chorda clavis;
          Dies d;

        si (   via.mensura != (i32)strlen("dies/YYYY-MM-DD/paginae")
            || memcmp(via.datum, "dies/", V) != ZEPHYRUM
            || memcmp(via.datum + XV, "/paginae", VIII) != ZEPHYRUM)
        {
            perge;
        }
        clavis.datum    = via.datum + V;
        clavis.mensura  = X;
        si (fasti_parsare_iso(clavis, &d))
        {
            *(chorda*)xar_addere(claves) = chorda_transcribere(clavis,
                piscina);
        }
    }
    redde claves;
}


/* ==================================================
 * Genus 'liber' (liber communis, clavis per paginam)
 * ================================================== */

interior vacuum*
librum_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
       IndagatioFontes* f;
   IndagatioConspectus* v;
                   Xar* nomina;
                   Xar* lineae;
                   s32  locus;
                   i32  i;
             character  numerus[XXXII];

    (vacuum)repertorium;
    f       = (IndagatioFontes*)ctx;
    v       = conspectum_novum(piscina);
    nomina  = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   !v || !nomina || !lineae
        || !scriba_liber_nomina_legere(piscina, f->intern, f->volumen,
               NIHIL, nomina))
    {
        redde v;
    }
    locus = -I;
    per (i = ZEPHYRUM; locus < ZEPHYRUM && i < xar_numerus(nomina); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(nomina, i), clavis))
        {
            locus = (s32)i;
        }
    }
    si (locus < ZEPHYRUM)
    {
        redde v;
    }
    (vacuum)scriba_liber_paginam_legere(piscina, f->intern, f->volumen,
        NIHIL, clavis, lineae);
    si (xar_numerus(lineae) == ZEPHYRUM)
    {
        redde v;
    }
    sprintf(numerus, "page %d", (integer)(locus + I));
    ordinem_addere(v, sinistram_purgare(*(chorda*)xar_obtinere(lineae,
        ZEPHYRUM)), chorda_ex_literis(numerus, piscina), lineas_iungere(
        lineae, I, piscina), literis_iungere("scriba/", clavis,
        piscina));
    redde v;
}

interior Xar*
librum_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    IndagatioFontes* f;
                Xar* claves;

    (vacuum)repertorium;
    f       = (IndagatioFontes*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   claves
        && !scriba_liber_nomina_legere(piscina, f->intern, f->volumen,
               NIHIL, claves))
    {
        xar_truncare(claves, ZEPHYRUM);
    }
    redde claves;
}


/* ==================================================
 * Genus 'personae' (personae per-rem + liber notarum)
 * ================================================== */

/* telephonum: cifrae solae a quoque grege (praeter ultimum, iam
 * verbum suum) ad finem - "+1 (555) 123-4567" -> "15551234567
 * 5551234567 1234567" */
interior vacuum
digitos_scribere (
    ChordaAedificator* a,
               chorda  telephonum)
{
    i32 i;
    i32 k;
    b32 in_grege;
    i32 greges;

    greges    = ZEPHYRUM;
    in_grege  = FALSUM;
    per (i = ZEPHYRUM; i < telephonum.mensura; i++)
    {
        b32 cifra = telephonum.datum[i] >= '0'
            && telephonum.datum[i] <= '9';

        si (cifra && !in_grege)
        {
            greges++;
        }
        in_grege = cifra;
    }
    in_grege = FALSUM;
    per (i = ZEPHYRUM; greges > I && i < telephonum.mensura; i++)
    {
        b32 cifra = telephonum.datum[i] >= '0'
            && telephonum.datum[i] <= '9';

        si (cifra && !in_grege)
        {
            greges--;
            per (k = i; k < telephonum.mensura; k++)
            {
                si (   telephonum.datum[k] >= '0'
                    && telephonum.datum[k] <= '9')
                {
                    chorda_aedificator_appendere_character(a,
                        (character)telephonum.datum[k]);
                }
            }
            chorda_aedificator_appendere_character(a, ' ');
            si (greges == I)
            {
                frange;
            }
        }
        in_grege = cifra;
    }
}

interior vacuum
chordas_scribere (
    ChordaAedificator* a,
                  Xar* l)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(l,
            i));
        chorda_aedificator_appendere_character(a, '\n');
    }
}

interior vacuum*
personam_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
      IndagatioFontes* f;
     constans Persona* p;
  IndagatioConspectus* v;
    ChordaAedificator* a;
               chorda  contextus;
                  i32  i;

    (vacuum)repertorium;
    f = (IndagatioFontes*)ctx;
    v = conspectum_novum(piscina);
    p = v ? personae_persona(f->personae, clavis) : NIHIL;
    si (!p)
    {
        redde v;
    }
    /* contextus: epistula prima - telephonum primum */
    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    si (p->epistulae && xar_numerus(p->epistulae) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            p->epistulae, ZEPHYRUM));
    }
    si (p->telephona && xar_numerus(p->telephona) > ZEPHYRUM)
    {
        si (chorda_aedificator_longitudo(a) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " - ");
        }
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            p->telephona, ZEPHYRUM));
    }
    contextus = chorda_aedificator_finire(a);
    /* corpus: nomina altera, rubricae, epistulae, telephona, notae */
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (p->genus == PERSONA_HOMO)
    {
        chorda_aedificator_appendere_chorda(a, p->titulus);
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(a, p->praenomen);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, p->cognomen);
    }
    chorda_aedificator_appendere_character(a, '\n');
    chordas_scribere(a, p->rubricae);
    chordas_scribere(a, p->epistulae);
    chordas_scribere(a, p->telephona);
    per (i = ZEPHYRUM; p->telephona && i < xar_numerus(p->telephona);
         i++)
    {
        digitos_scribere(a, *(chorda*)xar_obtinere(p->telephona, i));
    }
    si (f->volumen)
    {
        Xar* lineae;

        lineae = xar_creare(piscina, (i32)magnitudo(chorda));
        (vacuum)scriba_liber_lineas_legere(piscina, f->intern,
            f->volumen, chorda_ut_cstr(literis_iungere("persona/",
            clavis,
            piscina), piscina), ZEPHYRUM, lineae);
        chorda_aedificator_appendere_character(a, '\n');
        chordas_scribere(a, lineae);
    }
    ordinem_addere(v, persona_nomen(p, piscina), contextus,
        chorda_aedificator_finire(a), literis_iungere("nomenclator/",
        clavis, piscina));
    redde v;
}

interior i32
personae_numerus_rerum (
    constans vacuum* documentum)
{
    redde personae_numerus((constans PersonaeDocumentum*)documentum);
}

interior chorda
personae_id (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    (vacuum)piscina;
    redde personae_index((constans PersonaeDocumentum*)documentum,
        index)->id;
}

/* vestigium privatum: campi omnes ostensi aut quaesiti ('\x1f'
 * separati) - personae codicem rei unius non praebent */
interior chorda
personae_vestigium (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
     constans Persona* p;
    ChordaAedificator* a;

    p = personae_index((constans PersonaeDocumentum*)documentum, index);
    a = chorda_aedificator_creare(piscina, (memoriae_index)CXXVIII);
    chorda_aedificator_appendere_character(a, p->genus == PERSONA_HOMO
        ? 'h' : 's');
    chorda_aedificator_appendere_character(a, '\x1f');
    chorda_aedificator_appendere_chorda(a, p->praenomen);
    chorda_aedificator_appendere_character(a, '\x1f');
    chorda_aedificator_appendere_chorda(a, p->cognomen);
    chorda_aedificator_appendere_character(a, '\x1f');
    chorda_aedificator_appendere_chorda(a, p->titulus);
    chorda_aedificator_appendere_character(a, '\x1f');
    chordas_scribere(a, p->rubricae);
    chorda_aedificator_appendere_character(a, '\x1f');
    chordas_scribere(a, p->epistulae);
    chorda_aedificator_appendere_character(a, '\x1f');
    chordas_scribere(a, p->telephona);
    redde chorda_aedificator_finire(a);
}

interior vacuum
personae_claves (
             vacuum* ctx,
    constans vacuum* documentum,
                i32  index,
                Xar* claves,
            Piscina* piscina)
{
    (vacuum)ctx;
    (vacuum)piscina;
    *(chorda*)xar_addere(claves) = personae_index(
        (constans PersonaeDocumentum*)documentum, index)->id;
}

interior Xar*
personas_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    IndagatioFontes* f;
                Xar* claves;
                i32  i;

    (vacuum)repertorium;
    f       = (IndagatioFontes*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; claves && i < personae_numerus(f->personae); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            personae_index(f->personae, i)->id, piscina);
    }
    redde claves;
}


/* ==================================================
 * Registrum
 * ================================================== */

/* declarationem infixam legere, opera et enumerationem ponere */


/* ==================================================
 * Genus 'sancti' (D5 S4: corpus sanctorum, fons pura)
 * ================================================== */

/* textum (si plenum) ad corpus addere, linea nova */
interior vacuum
corpori_addere (
     ChordaAedificator* a,
                chorda  textus)
{
    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    chorda_aedificator_appendere_chorda(a, textus);
    chorda_aedificator_appendere_character(a, '\n');
}

/* Xar de chorda ad corpus */
interior vacuum
chordas_corpori_addere (
    ChordaAedificator* a,
                  Xar* x)
{
    i32 i;

    per (i = ZEPHYRUM; x && i < xar_numerus(x); i++)
    {
        corpori_addere(a, *(chorda*)xar_obtinere(x, i));
    }
}

interior vacuum*
sanctum_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
         IndagatioFontes* f;
     IndagatioConspectus* v;
        constans Sanctus* s;
   constans SanctiFestum* festum;
       ChordaAedificator* contextus;
       ChordaAedificator* corpus;
                  chorda  aetas;
                     i32  i;
               character  dies[XVI];

    (vacuum)repertorium;
    f = (IndagatioFontes*)ctx;
    v = conspectum_novum(piscina);
    s = (v && f->sancti) ? sancti_invenire(f->sancti, clavis) : NIHIL;
    si (!s)
    {
        redde v;
    }
    /* contextus: festum primum - aetas */
    contextus = chorda_aedificator_creare(piscina,
        (memoriae_index)XXXII);
    festum  = sancti_festum_primum(s);
    aetas   = sancti_aetas_textus(s, piscina);
    si (festum && festum->mensis > ZEPHYRUM && festum->dies > ZEPHYRUM)
    {
        sprintf(dies, "%d ", (integer)festum->dies);
        chorda_aedificator_appendere_literis(contextus, dies);
        chorda_aedificator_appendere_chorda(contextus,
            fasti_nomen_mensis(
            festum->mensis, VERUM, FASTI_LINGUA_ANGLICA, piscina));
        si (aetas.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(contextus,
                " \302\267 ");
        }
    }
    chorda_aedificator_appendere_chorda(contextus, aetas);
    /* corpus: nomina, munera, patrocinia, loci, socii, paragraphi */
    corpus = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    corpori_addere(corpus, s->titulus_latinus);
    per (i = ZEPHYRUM; s->nomina && i < xar_numerus(s->nomina); i++)
    {
        corpori_addere(corpus, ((SanctiNomen*)xar_obtinere(s->nomina,
            i))
            ->textus);
    }
    chordas_corpori_addere(corpus, s->munera);
    chordas_corpori_addere(corpus, s->patrocinia);
    corpori_addere(corpus, s->ortus.locus);
    corpori_addere(corpus, s->obitus.locus);
    per (i = ZEPHYRUM; s->socii && i < xar_numerus(s->socii); i++)
    {
        corpori_addere(corpus, ((SanctiSocius*)xar_obtinere(s->socii,
            i))
            ->textus);
    }
    chordas_corpori_addere(corpus, s->paragraphi);
    ordinem_addere(v, chorda_transcribere(s->titulus, piscina),
        chorda_aedificator_finire(contextus),
        chorda_aedificator_finire(corpus),
        literis_iungere("sancti/sanctus/", s->id, piscina));
    redde v;
}

/* omnes sancti corporis (ordine id) */
interior Xar*
sanctos_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    IndagatioFontes* f;
                Xar* claves;
                i32  i;

    (vacuum)repertorium;
    f       = (IndagatioFontes*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; claves && f->sancti
                       && i < sancti_numerus(f->sancti); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            sancti_ad_indicem(f->sancti, i)->id, piscina);
    }
    redde claves;
}

interior b32
genus_declarare (
     ConspectusRegistrum*  repertorium,
     constans character*   titulus,
        IndagatioFontes*   fontes,
                  vacuum* (*computare)(vacuum*, ConspectusRegistrum*,
                                       chorda, Piscina*),
     ConspectusEnumerare   enumerare,
                    Xar**  vitia)
{
    ConspectusOpera  opera;
            Piscina* p;
                b32  sana;

    p = piscina_generare_dynamicum("indagatio_genera_declaratio",
        MXXIV);
    si (!p)
    {
        redde FALSUM;
    }
    sana = conspectus_declarare(repertorium,
        conspectus_declaratio_infixa(titulus, p), vitia);
    piscina_destruere(p);
    si (!sana)
    {
        redde FALSUM;
    }
    opera.computare   = computare;
    opera.codificare  = indagatio_conspectum_codificare;
    opera.ctx         = fontes;
    redde conspectus_opera_ponere(repertorium, titulus, opera)
        && conspectus_enumerationem_ponere(repertorium, titulus,
               enumerare, fontes);
}

b32
indagatio_genera_registrare (
    ConspectusRegistrum*  repertorium,
        IndagatioFontes*  fontes,
                    Xar** vitia)
{
    ConspectusPerRem per_rem;

    si (vitia)
    {
        *vitia = NIHIL;
    }
    si (!repertorium || !fontes || !fontes->intern)
    {
        redde FALSUM;
    }
    si (fontes->agenda)
    {
        per_rem.numerus    = res_numerus;
        per_rem.id         = res_id;
        per_rem.vestigium  = res_vestigium;
        per_rem.claves     = res_claves;
        per_rem.ctx        = NIHIL;
        si (   !genus_declarare(repertorium, "res", fontes,
                   res_computare, res_enumerare, vitia)
            || !conspectus_per_rem_ponere(repertorium, "res", "agenda",
                   per_rem)
            || !conspectus_documentum_ligare(repertorium, "agenda",
                   fontes->agenda))
        {
            redde FALSUM;
        }
    }
    si (fontes->volumen)
    {
        si (   !genus_declarare(repertorium, "notae", fontes,
                   notas_computare, notas_enumerare, vitia)
            || !genus_declarare(repertorium, "liber", fontes,
                   librum_computare, librum_enumerare, vitia))
        {
            redde FALSUM;
        }
    }
    si (fontes->personae)
    {
        per_rem.numerus    = personae_numerus_rerum;
        per_rem.id         = personae_id;
        per_rem.vestigium  = personae_vestigium;
        per_rem.claves     = personae_claves;
        per_rem.ctx        = NIHIL;
        si (   !genus_declarare(repertorium, "personae", fontes,
                   personam_computare, personas_enumerare, vitia)
            || !conspectus_per_rem_ponere(repertorium, "personae",
                   "personae", per_rem)
            || !conspectus_documentum_ligare(repertorium, "personae",
                   fontes->personae))
        {
            redde FALSUM;
        }
    }
    /* D5 S4: sancti - fons pura, semel per processum */
    si (   fontes->sancti
        && !genus_declarare(repertorium, "sancti", fontes,
               sanctum_computare, sanctos_enumerare, vitia))
    {
        redde FALSUM;
    }
    redde VERUM;
}
