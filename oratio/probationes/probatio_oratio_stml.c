/* probatio_oratio_stml.c - Circuitus proiectionis STML orationis (T4)
 *
 * Scribere -> legere -> scribere BIS; oracula: octeti (emissio ex arbore
 * lecta contra fontem), comparator STRUCTURALIS et FIDELIS (LINEA munus
 * LINEA fert). Canalis derivatorum (gradus III) hic iam exercetur:
 * lexema derivatum in loco 'classes' manu positum f="1" et positionem
 * portatam fert, cursorem non movet, emissor id omittit. CORPUS:
 * plagulae md totae ut oratio (octeti quilibet). Culpa plantata: uncus
 * originis sublatus (consilium->origo NIHIL).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_lexema.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "materia_arbor.h"
#include "materia_sedes.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include "specimen_corporis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

/* Circuitus plenus super arborem datam. VISIO SEDIUM (materia-sedes
 * A2): elementa verificata in *elementa / *derivata sectionis vocantis
 * numerantur - porta quae nihil verificavit mortua est. */
interior b32
_circuitum_arboris (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                      MateriaNodus* radix,
                constans character* fons,
                               i32  mensura,
                               b32  fidelis,
                               i32* elementa,
                               i32* derivata)
{
               MateriaNodus* lecta;
               MateriaNodus* relecta;
      MateriaArborScriptura  s1;
      MateriaArborScriptura  s2;
      MateriaArborScriptura  s3;
         MateriaArborVitium  vitium;
    MateriaArborDifferentia  d;

    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }
    {
        MateriaSedesRelatio sedes;

        si (!materia_sedes_verificare(piscina, radix, consilium, fons,
                mensura, &sedes))
        {
            imprimere("    (sedes: %s, elementum %d)\n",
                sedes.causa ? sedes.causa : "-", (integer)sedes.index);
        }
        CREDO_VERUM (sedes.sana);
        *elementa += sedes.elementa;
        *derivata += sedes.derivata;
    }
    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }
    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (   !s2.successus || s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
        (size_t)s1.textus.mensura)
            != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari)\n");
        redde FALSUM;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus, consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        imprimere("    (lectio II)\n");
        redde FALSUM;
    }
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   !s3.successus || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
        (size_t)s1.textus.mensura)
            != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS,
            &d))
    {
        imprimere("    (arbor dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }
    si (   fidelis && !materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (fidelitas dispar: %s @ %s)\n",
            d.campus ? d.campus : "-",
            d.via);
        redde FALSUM;
    }
    {
                 MateriaScriptura emissa;
        MateriaScripturaConsilium cs;

        materia_scriptura_consilium_nudum(&cs, &ORATIO_REGISTRUM);
        cs.fons_index  = ORATIO_FONS_PLAGULAE;
        emissa         = materia_scribere_nodum(piscina, lecta, &cs);
        si (   !emissa.successus || emissa.textus.mensura != mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons, (size_t)mensura)
                    != ZEPHYRUM))
        {
            imprimere("    (emissio ex arbore lecta != fons)\n");
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                               b32  fidelis,
                               i32* elementa,
                               i32* derivata)
{
    MateriaNodus* radix = oratio_arbor_parsare(piscina, fons, mensura);

    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }
    redde _circuitum_arboris(piscina, consilium, radix, fons, mensura,
        fidelis, elementa, derivata);
}

/* FIXURA SUITAE: lexicon et consilium arboris semel facta; sectiones
 * ea per monstratorem constantem LEGUNT. */
nomen structura {
      MateriaLexiconRatum ratum;
    MateriaArborConsilium consilium;
} FixuraStml;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
            FixuraStml* f;
    MateriaLexIudicium  iudicium;

    f = (FixuraStml*)piscina_allocare(c->piscina,
        magnitudo(FixuraStml));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    si (!materia_lexicon_ratum_facere(&f->ratum, &ORATIO_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON ORATIONIS RECUSATUM\n");
        redde;
    }
    oratio_stml_consilium(&f->consilium, &f->ratum);
    c->datum = f;
}

interior vacuum
_casus_circuitus_probare (
    CredoContextus* c)
{
    hic_manens constans character* CASUS[] = {
        "Hello world. This is it.\n\nSecond para.\n",
        "  Indented start.\n\n\n  Next one\n",
        "Mr. Smith went home. He slept.",
        "\"Hello!\" she said. Then left. It cost $3.50. Wait... What?",
        "well-known don't e.g B2 1980s 1,024 3.14 x",
        "a.\r\n\r\nb.\r\n",
        "\xC3\x86r\xC3\xB8 \xC2\xA0x \xE2\x80\x9Cq\xE2\x80\x9D \xE2\x80\x94 don\xE2\x80\x99t \x01\x02.",
        "text </or-litterae> in textu\n",
        "",
        "\n\n \t\n"
    };
    constans FixuraStml* f = (constans FixuraStml*)c->suita;
                    i32  numerus = (i32)(magnitudo(CASUS)
                                 / magnitudo(CASUS[0]));
                    i32 i;
                    i32 elementa = ZEPHYRUM;
                    i32 derivata = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        CREDO_VERUM (_circuitum_probare(c->piscina, &f->consilium,
            CASUS[i],
            (i32)strlen(CASUS[i]), VERUM, &elementa, &derivata));
    }
}

interior vacuum
_canalem_derivatorum_probare (
    CredoContextus* c)
{
    constans FixuraStml* fixura   = (constans FixuraStml*)c->suita;
                Piscina* piscina  = c->piscina;
     constans character* f        = "Rosa alba.\n";
          MateriaNodus* radix;
          MateriaNodus* par;
          MateriaNodus* sen;
          MateriaNodus* voc;
          MateriaToken* origo;
          MateriaToken* classes;
 MateriaArborScriptura  s;
                   i32  elementa = ZEPHYRUM;
                   i32  derivata = ZEPHYRUM;

    si (fixura == NIHIL)
    {
        redde;
    }
    radix = oratio_arbor_parsare(piscina, f, (i32)strlen(f));
    CREDO_NON_NIHIL (radix);
    par =
        materia_valor_lista_obtinere(radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI],
        ZEPHYRUM)->datum.nodus;
    sen =
        materia_valor_lista_obtinere(par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE],
        ZEPHYRUM)->datum.nodus;
    voc =
        materia_valor_lista_obtinere(sen->loci[ORATIO_SENTENTIA_ELEMENTA],
        ZEPHYRUM)->datum.nodus;
    CREDO_AEQUALIS_S32 (voc->genus, (s32)ORATIO_GENUS_VOCABULUM);
    origo =
        materia_valor_lista_obtinere(voc->loci[ORATIO_VOCABULUM_PARTES],
        ZEPHYRUM)->datum.token;
    classes = oratio_lexema_derivatum(piscina,
        (s32)ORATIO_LEX_DERIVATUM,
        chorda_ex_literis("substantivum adiectivum", piscina),
        origo);
    CREDO_NON_NIHIL (classes);
    CREDO_VERUM (materia_nodus_ponere(voc,
        (i32)ORATIO_VOCABULUM_CLASSES,
        materia_valor_token(classes), MATERIA_LOCUS_TOKEN));
    s = materia_arbor_scribere_nodum(piscina, radix,
        &fixura->consilium);
    CREDO_VERUM (s.successus);
    CREDO_NON_NIHIL (strstr((character*)s.textus.datum, "f=\"1\""));
    CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
        "<or-derivatum"));
    CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
        "substantivum adiectivum"));
    CREDO_NIHIL (strstr((character*)s.textus.datum, "<ante>"));
    /* circuitus cum derivato: structuralis + fidelis + emissio == fons */
    CREDO_VERUM (_circuitum_arboris(piscina, &fixura->consilium, radix,
        f, (i32)strlen(f), VERUM, &elementa, &derivata));
}

/* CORPUS - et visio sedium: corpus elementa verificare DEBET (porta
 * mortua aliter) */
interior vacuum
_corpus_probare (
    CredoContextus* c)
{
    constans FixuraStml* f = (constans FixuraStml*)c->suita;
     constans character* radix_viae;
              character  via[DXII];
                  FILE* lista;
                   i32  plagulae       = ZEPHYRUM;
                   i32  fractae        = ZEPHYRUM;
                   i32  truncatae      = ZEPHYRUM;
                   i32  elementa       = ZEPHYRUM;
                   i32  derivata       = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
    lista = fopen(via, "r");
    si (lista == NIHIL)
    {
        CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
    }
    alioquin
    {
        character linea[DXII];

        dum (fgets(linea, (integer)magnitudo(linea), lista)
            != NIHIL)
        {
              Piscina* p;
            character* textus;
                  i32  mensura  = ZEPHYRUM;
               size_t  l        = strlen(linea);

            dum (   l > ZEPHYRUM
                 && (linea[l - I] == '\n' || linea[l - I] == '\r'))
            {
                linea[--l] = '\0';
            }
            si (l == ZEPHYRUM)
            {
                perge;
            }
            /* specimen: plagula quinta quaeque PER FRIATIONEM VIAE,
             * non per positionem (specimen_corporis.h; corpus totum:
             * ORATIO_CORPUS_TOTUS=1) - circuitus STML arboris
             * verborum XL s super MCXXXIV plagulas est */
            si (!_specimen_eligere(linea))
            {
                perge;
            }
            p = piscina_generare_dynamicum("oratio_corpus_stml",
                67108864);
            sprintf(via, "%s/%s", radix_viae, linea);
            textus = _plagulam_legere(p, via, &mensura);
            si (textus == NIHIL)
            {
                piscina_destruere(p);
                perge;
            }
            plagulae++;
            si (mensura > (i32)SPECIMEN_OCTETI_MAXIMI)
            {
                truncatae++;
                mensura = _caput_plagulae(textus, mensura);
            }
            si (!_circuitum_probare(p, &f->consilium, textus, mensura,
                VERUM, &elementa, &derivata))
            {
                fractae++;
                si (fractae <= V)
                {
                    imprimere("    FRACTA: %s\n", linea);
                }
            }
            piscina_destruere(p);
        }
        fclose(lista);
        imprimere("    plagulae %d, fractae %d, per caput probatae %d\n",
            (integer)plagulae,
            (integer)fractae,
            (integer)truncatae);
        CREDO_MAIOR_I32 (plagulae, (i32)200);
        CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
    }
    imprimere("  sedes (visio): %d elementa verificata, %d derivata\n",
        (integer)elementa, (integer)derivata);
    CREDO_VERUM (elementa > ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "circuitus STML (structuralis + fidelis)",
        _casus_circuitus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "canalis derivatorum (classes f=\"1\", emissor omittit)",
        _canalem_derivatorum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    /* gravis: circuitus STML corporis md ut oratio - apex plagulam
     * MAXIMAM speciminis sequitur (~IV-V KiB per octetum fontis);
     * caput CXXVIII KiB (specimen_corporis.h) eum ligat: ~DCL MiB
     * specimine, ~DCCXXX corpore toto (2026-10-22). Sumptus per
     * nodum proiectionis: park 01M5EGAWD5NT504BX9ZCR6JC37 */
    { "CORPUS: plagulae md totae ut oratio, circuitus STML",
        _corpus_probare, NIHIL, NIHIL, "gravis", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("oratio_stml", SECTIONES,
        _suitam_parare, NIHIL);
}
