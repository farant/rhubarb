/* probatio_materia_exemplaria.c - Extractor ordinum gradus II
 * (project-specs/exemplaria-c-spec.md par. II)
 *
 * Documentum EXPANSUM manu scribitur et per stml_legere_ex_literis
 * legitur: extractio a PARSURA et PROIECTIONE sic separatur, et
 * probatio regulam nullam, clientem nullum, plagulam nullam tangit -
 * quod ipsum est quod 'cliente caecum' significat.
 *
 * FIXTURA FORMAM VERAM SEQUITUR: INVOLUCRUM notam fert, nodus
 * INFRA eum sedem ('nota involucri proximi sedem INFRA se nominat',
 * silva._relata). Forma prior notam ET sedem in elemento eodem
 * ponebat - unde planta 'nota ambiens neglecta' VIRIDIS mansit, quia
 * hereditas notae numquam exercebatur. Porta quae falli non potest.
 *
 * Sedes quoque DISPARES (7-10, 0-12) et nodus interior (1-2) intra
 * alterum: aliter lex descensus tacentis probari non posset.
 */

#include "latina.h"
#include "credo.h"
#include "materia_exemplaria.h"
#include "materia_diagnostica.h"
#include "chorda.h"
#include "piscina.h"
#include "stml.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* Documentum ut post stml_expandere: involucrum <situs> ordinem unum
 * fert, et in eo involucra DUO sedes suas. Nodus <c> INTRA <b> iacet
 * et sedem PROPRIAM fert - descensus eum videre NON debet. */
#define DOC_UNUS \
    "<relatum lint=\"x\" gravitas=\"monitum\" causa=\"c\">" \
      "<situs>" \
        "<u nota=\"hic\">" \
          "<a sedes=\"1:8-1:11\" octeti=\"7-10\">-nt</a>" \
        "</u>" \
        "<v nota=\"ibi\">" \
          "<b sedes=\"1:1-1:13\" octeti=\"0-12\">" \
            "<c sedes=\"1:2-1:3\" octeti=\"1-2\">z</c>" \
          "</b>" \
        "</v>" \
      "</situs>" \
    "</relatum>"

/* Filii BINI ipsius <relatum> = inventa DUO */
#define DOC_BINI \
    "<relatum lint=\"y\">" \
      "<p sedes=\"1:1-1:2\" octeti=\"0-1\">a</p>" \
      "<q sedes=\"2:1-2:2\" octeti=\"5-6\">b</q>" \
    "</relatum>"

/* Regula nihil declarans: gravitas erratum, causa = nomen lintris */
#define DOC_NUDUS \
    "<relatum lint=\"z\">" \
      "<s sedes=\"1:1-1:3\" octeti=\"0-2\">ab</s>" \
    "</relatum>"

/* INTERNAMENTUM NECESSARIUM: sine eo tituli elementorum NIHIL
 * redeunt et parsura tacite fallit - vitium idem quod collectorem
 * annotationum momordit (materia_annotationes.c, eodem die). */
hic_manens Xar*
_extrahere (
               Piscina* piscina,
    constans character* documentum)
{
    InternamentumChorda* intern = internamentum_creare(piscina);
           StmlResultus  r;

    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    r = stml_legere_ex_literis(documentum, piscina, intern);

    si (!r.successus)
    {
        redde NIHIL;
    }
    redde materia_exemplaria_extrahere(piscina, r.radix);
}


/* ==================================================
 * PROBARE: ordo unus, attributa declarata
 * ================================================== */

interior vacuum
_ordinem_unum_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
                                 Xar* d = _extrahere(piscina, DOC_UNUS);
        constans MateriaDiagnosticum* p;

        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)I);
        p = (constans MateriaDiagnosticum*)xar_obtinere(d, ZEPHYRUM);
        CREDO_VERUM (strcmp(p->codex, "lint:x") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (p->gravitas,
            (s32)MATERIA_GRAVITAS_MONITUM);
        CREDO_VERUM (strcmp(p->causa, "c") == ZEPHYRUM);

        CREDO_GRADUS ("Sedes PRIMA primaria, cum nota sua");
        CREDO_AEQUALIS_S32 (p->tractus.initium, (s32)7);
        CREDO_AEQUALIS_S32 (p->tractus.finis, (s32)10);
        CREDO_NON_NIHIL (p->nota);
        CREDO_VERUM (strcmp(p->nota, "hic") == ZEPHYRUM);

        CREDO_GRADUS ("DESCENSUS TACET post capturam");
        /* Involucrum alterum sedem dat; nodus INTRA eum sedem
         * PROPRIAM fert sed apparere NON debet. Lex tota haec est.
         * (Tags hic NON nominantur: commentarium quod tag aperit
         * annotatio videtur - regula quam heri aedificavimus.) */
        CREDO_AEQUALIS_I32 (p->numerus_relatorum, (i32)I);
        si (p->numerus_relatorum == (i32)I)
        {
            CREDO_AEQUALIS_S32 (p->relata[ZEPHYRUM].tractus.initium,
                ZEPHYRUM);
            CREDO_AEQUALIS_S32 (p->relata[ZEPHYRUM].tractus.finis,
                (s32)12);
            CREDO_NON_NIHIL (p->relata[ZEPHYRUM].nota);
            CREDO_VERUM (strcmp(p->relata[ZEPHYRUM].nota, "ibi")
                == ZEPHYRUM);
        }
    }
}


/* ==================================================
 * PROBARE: filii bini = inventa duo
 * ================================================== */

interior vacuum
_filios_binos_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        Xar* d = _extrahere(piscina, DOC_BINI);

        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)II);
        si (xar_numerus(d) == (i32)II)
        {
            constans MateriaDiagnosticum* a =
                (constans MateriaDiagnosticum*)xar_obtinere(d,
                    ZEPHYRUM);
            constans MateriaDiagnosticum* b =
                (constans MateriaDiagnosticum*)xar_obtinere(d, (i32)I);

            /* sedes DISPARES: inventum quodque suam fert */
            CREDO_AEQUALIS_S32 (a->tractus.initium, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (b->tractus.initium, (s32)5);
            CREDO_AEQUALIS_I32 (a->numerus_relatorum, ZEPHYRUM);
        }
    }
}


/* ==================================================
 * PROBARE: ordinaria: erratum, causa = lint
 * ================================================== */

interior vacuum
_ordinaria_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
                                 Xar* d;
        constans MateriaDiagnosticum* p;

        d = _extrahere(piscina, DOC_NUDUS);

        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)I);
        p = (constans MateriaDiagnosticum*)xar_obtinere(d, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (p->gravitas,
            (s32)MATERIA_GRAVITAS_ERRATUM);
        CREDO_VERUM (strcmp(p->causa, "z") == ZEPHYRUM);
        CREDO_VERUM (p->nota == NIHIL);
    }
}


/* ==================================================
 * PROBARE: ordo sine sede transit
 * ================================================== */

interior vacuum
_sine_sede_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
                                 Xar* d;
        constans MateriaDiagnosticum* p;

        /* Regula quae CONGRUIT sed nodum non capit: '<situs/>' vacuum.
         * Sedes nulla, ergo inventum locari non potest - sed TACITE
         * CADERE id ipsum est quod hic arcus prohibet, quia regula quae
         * congruit et nihil reddit a regula quae nihil invenit non
         * differret. Ordo TRANSIT tractu vacuo; recusatio consumptoris
         * est (materia_diagnostica_plena). */
        d = _extrahere(piscina,
                "<relatum lint=\"vacuum\"><situs/></relatum>");

        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)I);
        si (xar_numerus(d) == (i32)I)
        {
            p = (constans MateriaDiagnosticum*)xar_obtinere(d,
                    ZEPHYRUM);
            CREDO_AEQUALIS_S32 (p->tractus.initium, (s32)-I);
            CREDO_AEQUALIS_S32 (p->tractus.finis, (s32)-I);
            CREDO_VERUM (strcmp(p->codex, "lint:vacuum") == ZEPHYRUM);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordo unus, attributa declarata", _ordinem_unum_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "filii bini = inventa duo", _filios_binos_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "ordinaria: erratum, causa = lint", _ordinaria_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "ordo sine sede transit", _sine_sede_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("materia_exemplaria", SECTIONES);
}
