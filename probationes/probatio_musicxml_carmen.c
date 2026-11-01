/* probatio_musicxml_carmen.c - conversor MusicXML -> carmen
 * (include/musicxml_carmen.h, plan P10)
 *
 * Quaeque probatio fixum convertit, fragmenta textus asserit, et textum
 * SEMPER per carmina_legere relegit ("textus manet veritas": exitus
 * carmen validum esse debet). Acceptio: nata_lux_80317.mxl stricte
 * conversum = nata_lux libri (carmina.stml) nota pro nota per partem.
 */

#include "latina.h"
#include "credo.h"
#include "musicxml.h"
#include "musicxml_carmen.h"
#include "carmen.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                  b32 successus;
               chorda textus;       /* <carmen>...</carmen> */
MusicxmlCarmenRelatio relatio;
              Carmina liber;        /* textus relectus */
                  b32 lectum;
} Conversio;

interior Conversio
_convertere_datum (
          CredoContextus* c,
                  chorda  datum,
                     b32  strictus)
{
              Conversio  v;
           MusicxmlOpus* o;
    MusicxmlCarmenForma  f;
      ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
          MMMCMXCIX);
    ChordaAedificator* b = chorda_aedificator_creare(c->piscina,
        MMMCMXCIX);
       CarminisVitium vitium;

    memset(&v, ZEPHYRUM, magnitudo(v));
    o = musicxml_legere(datum, c->piscina);
    musicxml_carmen_formam_ordinariam(&f);
    f.strictus = strictus;
    v.successus = o != NIHIL && musicxml_carmen_scribere(o, NIHIL, &f,
        c->piscina, a, &v.relatio);
    v.textus = chorda_aedificator_finire(a);
    si (v.successus)
    {
        chorda_aedificator_appendere_literis(b, "<carmina>\n");
        chorda_aedificator_appendere_chorda(b, v.textus);
        chorda_aedificator_appendere_literis(b, "</carmina>\n");
        v.lectum = carmina_legere(chorda_aedificator_finire(b),
            c->piscina,
            &v.liber, &vitium);
        si (!v.lectum)
        {
            imprimere("  carmina_legere: %s (pars %.*s, offset %u)\n",
                vitium.causa != NIHIL ? vitium.causa : "?",
                (integer)vitium.pars.mensura, (constans character*)
                vitium.pars.datum, (insignatus integer)vitium.offset);
            imprimere("%.*s\n", (integer)v.textus.mensura,
                (constans character*)v.textus.datum);
        }
    }
    redde v;
}

interior Conversio
_convertere (
          CredoContextus* c,
      constans character* titulus,
                     b32  strictus)
{
    character via[CCLVI];

    strcpy(via, "probationes/fixa/musicxml/");
    strcat(via, titulus);
    redde _convertere_datum(c, filum_legere_totum(via, c->piscina),
        strictus);
}

interior b32
_continet (
                 chorda  t,
     constans character* acus)
{
    i32 n = (i32)strlen(acus);
    i32 i;

    per (i = ZEPHYRUM; i + n <= t.mensura; i++)
    {
        si (memcmp(t.datum + i, acus, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fragmenta (litterae < DIX octetis, C89) in chordam unam */
interior chorda
_iungere (
         CredoContextus* c,
    constans character* constans* fragmenta)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        MMMCMXCIX);
                   i32 i;

    per (i = ZEPHYRUM; fragmenta[i] != NIHIL; i++)
    {
        chorda_aedificator_appendere_literis(a, fragmenta[i]);
    }
    redde chorda_aedificator_finire(a);
}

/* octetus primus acus in t, aut -I */
interior s32
_situs (
                 chorda  t,
     constans character* acus)
{
    i32 n = (i32)strlen(acus);
    i32 i;

    per (i = ZEPHYRUM; i + n <= t.mensura; i++)
    {
        si (memcmp(t.datum + i, acus, (size_t)n) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior i32
_numerare (
                 chorda  t,
     constans character* acus)
{
    i32 n = (i32)strlen(acus);
    i32 i;
    i32 k = ZEPHYRUM;

    per (i = ZEPHYRUM; i + n <= t.mensura; i++)
    {
        si (memcmp(t.datum + i, acus, (size_t)n) == ZEPHYRUM)
        {
            k++;
        }
    }
    redde k;
}

interior i32
_approximationes (
    constans MusicxmlCarmenRelatio* r,
         MusicxmlApproximatioGenus  genus)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < r->numerus_approximationum; i++)
    {
        si (r->approximationes[i].genus == genus)
        {
            n++;
        }
    }
    redde n;
}

/* nota k-ta partis p (frusta ordine), pulsus absolutus in *pulsus */
interior constans CarmenNota*
_nota (
    constans Carmen* k,
                i32  p,
                i32  index,
                s64* pulsus)
{
    i32 f;

    si (p >= k->numerus_partium)
    {
        redde NIHIL;
    }
    per (f = ZEPHYRUM; f < k->partes[p].numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &k->partes[p].frusta[f];

        si (index < fr->numerus)
        {
            *pulsus = fr->pulsus + fr->notae[index].pulsus;
            redde &fr->notae[index];
        }
        index -= fr->numerus;
    }
    redde NIHIL;
}

interior i32
_notae_partis (
    constans Carmen* k,
                i32  p)
{
    i32 f;
    i32 n = ZEPHYRUM;

    per (f = ZEPHYRUM; f < k->partes[p].numerus_frustorum; f++)
    {
        n += k->partes[p].frusta[f].numerus;
    }
    redde n;
}

interior vacuum
_nota_una_probare (
    CredoContextus* c)
{
              Conversio  v = _convertere(c, "nota_una.musicxml", VERUM);
    constans CarmenNota* n;
                    s64  pulsus = ZEPHYRUM;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "metrum=\"1/4\""));
    CREDO_VERUM (_continet(v.textus, "<pars titulus=\"cantus\""));
    CREDO_VERUM (_continet(v.textus, "o4 c4"));
    CREDO_VERUM (_continet(v.textus, "Anonymus"));
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);
    n = _nota(&v.liber.carmina[ZEPHYRUM], ZEPHYRUM, ZEPHYRUM, &pulsus);
    CREDO_NECESSE (n != NIHIL);
    CREDO_AEQUALIS_S64 (pulsus, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (n->longitudo, (s64)CMLX);
    CREDO_AEQUALIS_I32 (n->nota, (i32)LX);
}

interior vacuum
_longitudines_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "longitudines.musicxml", VERUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "c4"));
    CREDO_VERUM (_continet(v.textus, "d2."));
    CREDO_VERUM (_continet(v.textus, "e6 e6 e6"));
    CREDO_VERUM (_continet(v.textus, "g1&g4"));
    CREDO_VERUM (_continet(v.textus, "a20 a20 a20 a20 a20"));
    CREDO_VERUM (_continet(v.textus,
        "<metrum mensura=\"3\" valor=\"5/4\"/>"));

    /* septupla: VII approximationes, summa mensurae servata */
    v = _convertere(c, "ligaturae_tupli.musicxml", FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_EXTRA_RETICULUM), (i32)VII);
    v = _convertere(c, "ligaturae_tupli.musicxml", VERUM);
    CREDO_FALSUM (v.successus);

    /* rotundatio sub pulso quae longitudinem dicibilem dat: tamen
     * approximatio (numquam tacita) */
    v = _convertere(c, "reticulum_tacitum.musicxml", FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_EXTRA_RETICULUM), (i32)I);
    v = _convertere(c, "reticulum_tacitum.musicxml", VERUM);
    CREDO_FALSUM (v.successus);
}

/* M7: vox quaeque et nota acordi quaeque pars sua */
interior vacuum
_voces_partes_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "voces.musicxml", VERUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (v.liber.carmina[ZEPHYRUM].numerus_partium,
        (i32)II);
    CREDO_VERUM (_continet(v.textus, "<pars titulus=\"cantus\""));
    CREDO_VERUM (_continet(v.textus, "<pars titulus=\"cantus_2\""));
    CREDO_MAIOR_I32 (v.relatio.numerus_structurarum, ZEPHYRUM);
    v = _convertere(c, "acordum.musicxml", VERUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (v.liber.carmina[ZEPHYRUM].numerus_partium,
        (i32)IV);
    CREDO_VERUM (_continet(v.textus, "<pars titulus=\"organum_4\""));
}

/* M14: anacrusis -> metrum mensurae suae; commentarium numerorum */
interior vacuum
_tactus_brevis_metrum_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "tactus_brevis.musicxml", VERUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "metrum=\"1/4\""));
    CREDO_VERUM (_continet(v.textus,
        "<metrum mensura=\"2\" valor=\"3/4\"/>"));
    CREDO_VERUM (_continet(v.textus, "1=0"));
}

/* M6: senza-misura -> liberum, incisum per mensuram */
interior vacuum
_liberum_probare (
    CredoContextus* c)
{
    Conversio v = _convertere_datum(c, filum_legere_totum(
        "musica/fontes/nata_lux_80317.mxl", c->piscina), VERUM);
          chorda t;
             i32 lineae = ZEPHYRUM;
             i32 i;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "metrum=\"liberum\""));
    t =
        v.liber.carmina[ZEPHYRUM].partes[ZEPHYRUM].frusta[ZEPHYRUM].textus;
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '|')
        {
            lineae++;
        }
    }
    /* V visitationes (0 1 2 3 3) -> IV lineae */
    CREDO_AEQUALIS_I32 (lineae, (i32)IV);
}

interior vacuum
_dynamica_tempus_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "directiones.musicxml", FALSUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "valor=\"mf\""));
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_DYNAMICA_IGNOTA), ZEPHYRUM);
    CREDO_VERUM (_continet(v.textus, "usque=\"2\""));
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_CLIVUS_MOTUS), ZEPHYRUM);
    CREDO_VERUM (_continet(v.textus, "valor=\"73\""));
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_TEMPUS_ROTUNDATUM), ZEPHYRUM);
    CREDO_VERUM (_continet(v.textus, "<fermata "));
    CREDO_VERUM (_continet(v.textus, "factor=\"2\""));

    v = _convertere(c, "conversio_varia.musicxml", FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "tempus=\"400\""));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_TEMPUS_EXTRA_FINES), (i32)I);
    CREDO_VERUM (_continet(v.textus,
        "<clavis mensura=\"2\" valor=\"d maior\"/>"));
    CREDO_VERUM (_continet(v.textus,
        "<metrum mensura=\"2\" valor=\"3/4\"/>"));
}

interior vacuum
_approximationes_variae_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "conversio_varia.musicxml", FALSUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_ALTERATIO_FRACTA), (i32)I);
    CREDO_VERUM (_continet(v.textus, "Ignotus"));
    CREDO_VERUM (_continet(v.textus, "Poeta"));
    CREDO_VERUM (_continet(v.textus, "titulus=\"varia\""));

    v = _convertere(c, "attributa.musicxml", FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "valor=\"5/8\""));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_METRUM_ADDITIVUM), (i32)I);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_CLAVIS_NON_TRADITA), (i32)I);

    v = _convertere(c, "percussio.musicxml", FALSUM);
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_PERCUSSIO_OMISSA), ZEPHYRUM);

    v = _convertere(c, "gratiae.musicxml", FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_PARVA_OMISSA), (i32)I);
}

/* Focus recensionis 3: verba in acordis et ligaturis */
interior vacuum
_verba_acorda_ligaturae_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "verba_acorda.musicxml", FALSUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "<verba!>La da _ te"));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_VERSUS_OMISSUS), (i32)I);
    /* pars acordi (E5) sine verbis */
    CREDO_AEQUALIS_I32 (v.liber.carmina[ZEPHYRUM].numerus_partium,
        (i32)II);
}

interior vacuum
_signa_non_approximationes_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "signa_sola.musicxml", VERUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (v.relatio.signa[MUSICXML_CARMEN_ARCUS],
        (i32)II);
    CREDO_AEQUALIS_I32 (v.relatio.signa[MUSICXML_CARMEN_ARTICULATIO],
        (i32)I);
    CREDO_AEQUALIS_I32 (v.relatio.signa[MUSICXML_CARMEN_VERBA_DIRECTIONIS],
        (i32)I);
    CREDO_AEQUALIS_I32 (v.relatio.signa[MUSICXML_CARMEN_EXERCITIUM],
        (i32)I);
}

interior vacuum
_strictus_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "gratiae.musicxml", VERUM);

    CREDO_FALSUM (v.successus);
    CREDO_AEQUALIS_I32 (v.textus.mensura, ZEPHYRUM);
    CREDO_NECESSE (v.relatio.numerus_approximationum == I);
    CREDO_AEQUALIS_S32 ((s32)v.relatio.approximationes[ZEPHYRUM].genus,
        (s32)MUSICXML_CARMEN_GRATIA_OMISSA);
    CREDO_AEQUALIS_I32 (v.relatio.approximationes[ZEPHYRUM].linea,
        (i32)XI);
    CREDO_AEQUALIS_I32 (v.relatio.numerus_structurarum, ZEPHYRUM);
    v = _convertere(c, "gratiae.musicxml", FALSUM);
    CREDO_VERUM (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_GRATIA_OMISSA), (i32)II);
}

interior vacuum
_transpositio_carmen_probare (
    CredoContextus* c)
{
         Conversio v = _convertere(c, "transpositio.musicxml", VERUM);
    constans CarmenNota* n;
               s64 pulsus;

    CREDO_NECESSE (v.successus && v.lectum);
    n = _nota(&v.liber.carmina[ZEPHYRUM], ZEPHYRUM, ZEPHYRUM, &pulsus);
    CREDO_NECESSE (n != NIHIL);
    CREDO_AEQUALIS_I32 (n->nota, (i32)LXXII);
    CREDO_AEQUALIS_S32 ((s32)n->littera, (s32)'c');
}

/* M8: repetitiones evolutae */
interior vacuum
_repetitiones_evolutae_probare (
    CredoContextus* c)
{
    Conversio v = _convertere(c, "repetitiones.musicxml", VERUM);
          i32 k;
          b32 nota = FALSUM;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_notae_partis(&v.liber.carmina[ZEPHYRUM],
        ZEPHYRUM), (i32)VIII);
    per (k = ZEPHYRUM; k < v.relatio.numerus_structurarum; k++)
    {
        si (_continet(v.relatio.structurae[k], "repetitiones evolutae"))
        {
            nota = VERUM;
        }
    }
    CREDO_VERUM (nota);
}

interior vacuum
_relatio_scripta_probare (
    CredoContextus* c)
{
            Conversio  v = _convertere(c, "gratiae.musicxml", FALSUM);
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MXXIV);
               chorda  t;

    CREDO_NECESSE (v.successus);
    CREDO_VERUM (musicxml_relationem_scribere(&v.relatio, "g.musicxml",
        a));
    t = chorda_aedificator_finire(a);
    CREDO_VERUM (_continet(t, "g.musicxml: P1 m.1 t.1 11:7: "));
    CREDO_VERUM (_continet(t, musicxml_approximationis_titulus(
        MUSICXML_CARMEN_GRATIA_OMISSA)));
}

/* corpus silvestre (P10): carmen mutationem intra clivum, fermatam
 * intra priorem, ordinem inversum vetat. Gradus 'p' post cuneum in
 * fonte (eadem positione) ante eum scribitur; 'ff' intra cuneum (finis
 * in medio mensurae II -> initium III) fit finis cunei; fermata P2
 * (mensura III) ante fermatam P1 (ictus III, nota ligata - nullum
 * initium novum) scribitur, P1 intra eam omittitur */
hic_manens constans character* constans CONFLICTUS[] = {
    "<?xml version=\"1.0\"?><score-partwise version=\"4.0\"><part-list>"
    "<score-part id=\"P1\"><part-name>Cantus</part-name></score-part>"
    "<score-part id=\"P2\"><part-name>Bassus</part-name></score-part>"
    "</part-list>",
    "<part id=\"P1\"><measure number=\"1\"><attributes><divisions>1"
    "</divisions><time><beats>4</beats><beat-type>4</beat-type></time>"
    "</attributes><direction><direction-type><wedge type=\"crescendo\" "
    "number=\"1\"/></direction-type></direction>",
    "<direction><direction-type><dynamics><p/></dynamics></direction-type>"
    "</direction><note><pitch><step>C</step><octave>4</octave></pitch>"
    "<duration>4</duration><voice>1</voice><type>whole</type></note>"
    "</measure>",
    "<measure number=\"2\"><note><pitch><step>C</step><octave>4</octave>"
    "</pitch><duration>2</duration><voice>1</voice><type>half</type>"
    "</note><direction><direction-type><wedge type=\"stop\" number=\"1\"/>"
    "</direction-type></direction>",
    "<direction><direction-type><dynamics><ff/></dynamics></direction-type>"
    "</direction><note><pitch><step>C</step><octave>4</octave></pitch>"
    "<duration>2</duration><voice>1</voice><type>half</type></note>"
    "</measure>",
    "<measure number=\"3\"><note><pitch><step>C</step><octave>4</octave>"
    "</pitch><duration>2</duration><tie type=\"start\"/><voice>1</voice>"
    "<type>half</type><notations><tied type=\"start\"/></notations>"
    "</note>",
    "<note><pitch><step>C</step><octave>4</octave></pitch><duration>2"
    "</duration><tie type=\"stop\"/><voice>1</voice><type>half</type>"
    "<notations><tied type=\"stop\"/><fermata/></notations></note>"
    "</measure></part>",
    "<part id=\"P2\"><measure number=\"1\"><attributes><divisions>1"
    "</divisions><time><beats>4</beats><beat-type>4</beat-type></time>"
    "</attributes><note><pitch><step>C</step><octave>3</octave></pitch>"
    "<duration>4</duration><voice>1</voice><type>whole</type></note>"
    "</measure>",
    "<measure number=\"2\"><note><pitch><step>C</step><octave>3</octave>"
    "</pitch><duration>4</duration><voice>1</voice><type>whole</type>"
    "</note></measure><measure number=\"3\"><note><pitch><step>C</step>"
    "<octave>3</octave></pitch><duration>4</duration><voice>1</voice>"
    "<type>whole</type><notations><fermata/></notations></note>"
    "</measure></part></score-partwise>",
    NIHIL
};

interior vacuum
_conflictus_elementorum_probare (
    CredoContextus* c)
{
    Conversio v = _convertere_datum(c, _iungere(c, CONFLICTUS), FALSUM);
          s32 gradus;
          s32 cuneus;

    CREDO_NECESSE (v.successus && v.lectum);
    gradus = _situs(v.textus,
        "<dynamica pars=\"cantus\" mensura=\"1\" valor=\"p\"/>");
    cuneus = _situs(v.textus,
        "<dynamica pars=\"cantus\" mensura=\"1\" valor=\"ff\" "
        "usque=\"3\"/>");
    CREDO_MAIOR_S32 (gradus, (s32)-I);
    CREDO_MAIOR_S32 (cuneus, gradus);
    CREDO_AEQUALIS_I32 (_numerare(v.textus, "valor=\"ff\""), (i32)I);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_CLIVUS_MOTUS), (i32)II);
    CREDO_AEQUALIS_I32 (_numerare(v.textus, "<fermata "), (i32)I);
    CREDO_VERUM (_continet(v.textus, "<fermata mensura=\"3\""));
}

/* longitudo mensurae extra reticulum LX (exportator rotundans): termini
 * mensurarum in reticulo, metrum semper dicibile, nota ultra finem
 * ultimum secta (non '&' in nihilum); vox II (nota una CXXVIII-a, in
 * reticulo nulla) linea sine nota -> non scribitur */
hic_manens constans character* constans MENSURA_LONGIOR[] = {
    "<?xml version=\"1.0\"?><score-partwise version=\"4.0\"><part-list>"
    "<score-part id=\"P1\"><part-name>Cantus</part-name></score-part>"
    "</part-list><part id=\"P1\"><measure number=\"1\"><attributes>"
    "<divisions>48</divisions><time><beats>4</beats><beat-type>4"
    "</beat-type></time></attributes>",
    "<note><pitch><step>C</step><octave>4</octave></pitch><duration>192"
    "</duration><voice>1</voice><type>whole</type></note><backup>"
    "<duration>192</duration></backup><forward><duration>192</duration>"
    "<voice>2</voice></forward>",
    "<note><pitch><step>C</step><octave>4</octave></pitch><duration>1"
    "</duration><voice>2</voice><type>128th</type></note></measure>",
    "<measure number=\"2\"><note><pitch><step>D</step><octave>4</octave>"
    "</pitch><duration>192</duration><voice>1</voice><type>whole</type>"
    "</note></measure></part></score-partwise>",
    NIHIL
};

/* LXV/LXIV: N > LXIV - longitudo proxima dicibilis XXXIII/XXXII */
hic_manens constans character* constans MENSURA_PRAELONGA[] = {
    "<?xml version=\"1.0\"?><score-partwise version=\"4.0\"><part-list>"
    "<score-part id=\"P1\"><part-name>Cantus</part-name></score-part>"
    "</part-list><part id=\"P1\"><measure number=\"1\"><attributes>"
    "<divisions>16</divisions><time><beats>4</beats><beat-type>4"
    "</beat-type></time></attributes>",
    "<note><pitch><step>C</step><octave>4</octave></pitch><duration>64"
    "</duration><voice>1</voice><type>whole</type></note><note><pitch>"
    "<step>D</step><octave>4</octave></pitch><duration>1</duration>"
    "<voice>1</voice><type>64th</type></note></measure>",
    "<measure number=\"2\"><note><pitch><step>E</step><octave>4</octave>"
    "</pitch><duration>64</duration><voice>1</voice><type>whole</type>"
    "</note></measure></part></score-partwise>",
    NIHIL
};

interior vacuum
_mensura_extra_reticulum_probare (
    CredoContextus* c)
{
    Conversio v = _convertere_datum(c, _iungere(c, MENSURA_LONGIOR),
        FALSUM);

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "c1 | d1</textus>"));
    CREDO_AEQUALIS_I32 (v.liber.carmina[ZEPHYRUM].numerus_partium,
        (i32)I);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_METRUM_IMPOSSIBILE), ZEPHYRUM);
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_EXTRA_RETICULUM), ZEPHYRUM);

    v = _convertere_datum(c, _iungere(c, MENSURA_PRAELONGA), FALSUM);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(v.textus, "metrum=\"33/32\""));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_METRUM_IMPOSSIBILE), ZEPHYRUM);
    CREDO_MAIOR_I32 (_approximationes(&v.relatio,
        MUSICXML_CARMEN_EXTRA_RETICULUM), ZEPHYRUM);
}

/* ACCEPTIO (spec par. IV): nata_lux stricte conversum = nata_lux libri */
interior vacuum
_nata_lux_probare (
    CredoContextus* c)
{
         Conversio v = _convertere_datum(c, filum_legere_totum(
                           "musica/fontes/nata_lux_80317.mxl",
                           c->piscina), VERUM);
            Carmina  liber;
     CarminisVitium  vitium;
    constans Carmen* nos;
    constans Carmen* illud;
                i32  p;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_NECESSE (carmina_ordinaria(c->piscina, &liber, &vitium));
    illud = carmina_invenire(&liber, chorda_ex_literis("nata_lux",
        c->piscina));
    CREDO_NECESSE (illud != NIHIL);
    nos = &v.liber.carmina[ZEPHYRUM];
    CREDO_NECESSE (nos->numerus_partium == illud->numerus_partium);
    per (p = ZEPHYRUM; p < nos->numerus_partium; p++)
    {
        i32 n = _notae_partis(nos, p);
        i32 k;

        CREDO_AEQUALIS_I32 (n, _notae_partis(illud, p));
        per (k = ZEPHYRUM; k < n; k++)
        {
                            s64  a;
                            s64  b;
            constans CarmenNota* x = _nota(nos, p, k, &a);
            constans CarmenNota* y = _nota(illud, p, k, &b);

            si (   x            == NIHIL || y == NIHIL || a != b
                || x->longitudo != y->longitudo || x->nota != y->nota
                || x->littera   != y->littera
                || !chorda_aequalis(x->syllaba, y->syllaba))
            {
                imprimere("  nata_lux pars %u nota %u: nostra %ld/%ld/%u "
                    "illius %ld/%ld/%u\n", (insignatus integer)p,
                    (insignatus integer)k,
                    x ? (longus)a : -1L, x ? (longus)x->longitudo : -1L,
                    x ? (insignatus integer)x->nota : 0U,
                    y ? (longus)b : -1L, y ? (longus)y->longitudo : -1L,
                    y ? (insignatus integer)y->nota : 0U);
                CREDO_VERUM (FALSUM);
                frange;
            }
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nota una", _nota_una_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "longitudines MML et reticulum", _longitudines_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "voces et acorda -> partes (M7)", _voces_partes_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "anacrusis -> metrum (M14)", _tactus_brevis_metrum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "senza-misura -> liberum (M6)", _liberum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "dynamica, tempus, fermata, clavis, metrum",
        _dynamica_tempus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "approximationes variae", _approximationes_variae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "verba: acorda, ligaturae, melisma",
        _verba_acorda_ligaturae_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa non approximationes", _signa_non_approximationes_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "strictus", _strictus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transpositio: sonans", _transpositio_carmen_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "repetitiones evolutae (M8)", _repetitiones_evolutae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "relatio scripta", _relatio_scripta_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "conflictus dynamicae et fermatae",
        _conflictus_elementorum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensura extra reticulum", _mensura_extra_reticulum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "ACCEPTIO: nata lux", _nata_lux_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("musicxml_carmen", SECTIONES, NIHIL,
        NIHIL);
}
