/* mundus - instrumentum: loci mundi ex dispositionibus (apps/mundus/
 * loci) in volumen, et pictura rei in loco ut latus $world eam
 * pingeret (project-specs/plastica-spec.md IV.7, IV.8; plastica-plan
 * T9). Logica in lib/mundus_locus.c, lib/statua_iunctio.c,
 * lib/mundus_pictura.c; hic argumenta et effusio sola.
 *
 * Usus:
 *
 *   mundus condere -volumen V [-loci DIR]
 *   mundus specimen <res.stml> -o <via.png> [-locus "x z"] [-gyrus G]
 *
 * condere: omnem dispositionem *.stml in DIR (ordinarium apps/mundus/
 * loci), ordine nominum: locum nudum (GLB, attributum basis) legit,
 * rem quamque semel componit (plastica), res in locis suis ponit
 * (mundus_locus_struere), locum instructum in GLB scribit, per
 * lectorem relegit et ut LOCUM in volumen addit sub titulo
 * dispositionis. Linea per dispositionem:
 *
  *   addita    TITULUS   N res, V vertices, cellae gressibiles A -> B
 *   mutata    TITULUS   ...   octeti substituti
 *   eadem     TITULUS   ...   octeti iidem: nihil scriptum
 *   VIA:LINEA: CAUSA          dispositio, positio aut descriptio
 *                             rei recusata (stderr)
 *   VIA: recusata: CAUSA      basis legi non potest; titulus iam
 *                             persona aut res est, aut bis in hoc
 *                             cursu (stderr)
 *
 * Dispositio recusata NIHIL scribit et ceteras non impedit. NIHIL
 * UMQUAM REMOVET. Viae in dispositione ad directorium currens
 * referuntur (radix repositorii).
 *
 * specimen: rem unam in loco nudo ordinario (probationes/fixa/statuae/
 * planum.glb) ponit, personam (fungus.glb) iuxta eam, et quadrum
 * pingit ut latus $world: visus fixus mundi, humus redacta, persona
 * exemplariis. Tabulae duae: humus Atkinson (sinistra), humus
  * exemplaria (dextra). Per tabulam digestum quadri et pixela
 * personae imprimuntur (fumus ea inspicit). Locus ordinarius:
 * initium loci (ubi persona incipit). Res ibi stare non potest:
 * causa nominatur, exitus 1.
 *
 * -volumen SEMPER nominandum; volumen absens creatur.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata); 2 usus.
 *
 * Aedificatio: ./tools/mundus_struere.sh -> bin/mundus. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/mundus.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "volumen.h"
#include "thema.h"
#include "dithering.h"
#include "imago_png.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_pictura.h"
#include "statua_glb.h"
#include "statua_glb_scriptor.h"
#include "statuae_repositorium.h"
#include "plastica.h"
#include "plastica_specimen.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "mundus_pictura.h"
#include "mundus_locus.h"
#include "provenientia.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCI_ORDINARII     "apps/mundus/loci"
#define BASIS_ORDINARIA    "probationes/fixa/statuae/planum.glb"
#define PERSONA_ORDINARIA  "probationes/fixa/statuae/fungus.glb"
#define PLAGULAE_MAXIMAE   CCLVI
#define VIA_MAXIMA         MXXIV
#define QUADRI_LATITUDO    480
#define QUADRI_ALTITUDO    360
#define CHARTA_PALETTAE    V       /* Albus */
#define DIRECTIONES        XVI

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: mundus condere -volumen V [-loci DIR]\n"
        "      mundus specimen <res.stml> -o <via.png>"
        " [-locus \"x z\"] [-gyrus G]\n");
    redde II;
}

/* chorda in litteras C (in piscina) */
interior constans character*
literae (
     chorda  textus,
    Piscina* piscina)
{
    character* p = (character*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura + I);

    si (!p)
    {
        redde "";
    }
    memcpy(p, textus.datum, textus.mensura);
    p[textus.mensura] = '\0';
    redde p;
}

interior vacuum
recusatam_nuntiare (
    constans character* via,
    constans character* causa,
                chorda  additum)
{
    fprintf(stderr, "%s: recusata: %s%s%.*s\n", via, causa,
        additum.mensura ? ": " : "", (integer)additum.mensura,
        (constans character*)additum.datum);
}

/* rem componere; NIHIL (causa cum via et linea nuntiata) si
 * recusata */
interior Statua*
rem_componere (
    constans character* via,
               Piscina* piscina)
{
    PlasticaFructus f;
             chorda textus = filum_legere_totum(via, piscina);

    si (textus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "%s:0: the asset's description cannot be"
            " read\n", via);
        redde NIHIL;
    }
    f = plastica_legere(textus, piscina);
    si (!f.successus)
    {
        fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde NIHIL;
    }
    redde f.statua;
}

/* locum nudum legere; NIHIL (causa nuntiata) si recusatus */
interior Statua*
basim_legere (
    constans character* via_dispositionis,
    constans character* via,
               Piscina* piscina)
{
    StatuaFructus f;
           chorda octeti = filum_legere_totum(via, piscina);

    si (octeti.mensura == ZEPHYRUM)
    {
        recusatam_nuntiare(via_dispositionis,
            "basis legi non potest",
            chorda_ex_literis(via, piscina));
        redde NIHIL;
    }
    f = statua_glb_legere(octeti, piscina);
    si (!f.successus)
    {
        recusatam_nuntiare(via_dispositionis,
            "basis GLB sana non est", f.error);
        redde NIHIL;
    }
    redde f.statua;
}

interior i32
cellae_gressibiles (
    constans MundusSolum* s)
{
    memoriae_index k;
    memoriae_index cellae;
               i32 numerus = ZEPHYRUM;

    si (!s || !s->genera)
    {
        redde ZEPHYRUM;
    }
    cellae = (memoriae_index)s->latitudo * s->altitudo;
    per (k = ZEPHYRUM; k < cellae; k++)
    {
        numerus += (i32)(s->genera[k] == MUNDUS_SOLUM_GRESSIBILE);
    }
    redde numerus;
}


/* ==================================================
 * condere
 * ================================================== */

nomen structura {
    i32 additae;
    i32 mutatae;
    i32 eaedem;
    i32 recusatae;
} Summa;

interior b32
est_plagula_stml (
    chorda titulus)
{
    redde titulus.mensura > V
        && memcmp(titulus.datum + titulus.mensura - V, ".stml", V)
            == ZEPHYRUM;
}

/* nomina plagularum *.stml directorii, ordine octetorum; -1 si
 * directorium aperiri non potest aut plagulae nimis multae */
interior s32
plagulas_colligere (
    constans character* directorium,
                chorda* nomina,
               Piscina* piscina)
{
     DirectoriumIterator* iter =
         directorium_iterator_aperire(directorium, piscina);
    DirectoriumIntroitus* introitus;
                     s32  numerus = ZEPHYRUM;
                     s32  k;
                     s32  j;

    si (!iter)
    {
        redde -I;
    }
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        si (   introitus->genus != INTROITUS_FILUM
            || !est_plagula_stml(introitus->titulus))
        {
            perge;
        }
        si (numerus >= PLAGULAE_MAXIMAE)
        {
            directorium_iterator_claudere(iter);
            redde -I;
        }
        nomina[numerus++] =
            chorda_transcribere(introitus->titulus, piscina);
    }
    directorium_iterator_claudere(iter);
    per (k = I; k < numerus; k++)
    {
        chorda hoc = nomina[k];

        per (j = k; j > ZEPHYRUM
            && chorda_comparare(nomina[j - I], hoc) > ZEPHYRUM; j--)
        {
            nomina[j] = nomina[j - I];
        }
        nomina[j] = hoc;
    }
    redde numerus;
}

interior StatuaeInscriptio*
inscriptio_tituli (
       Xar* index,
    chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        StatuaeInscriptio* x =
            (StatuaeInscriptio*)xar_obtinere(index, k);

        si (x && chorda_aequalis(x->titulus, titulus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

/* res dispositionis componere: fons idem semel (index idem). FALSUM
 * si qua recusata (causa nuntiata) */
interior b32
res_componere (
          constans character*  via,
 constans MundusLocusFructus*  d,
             constans Statua** statuae,
                     Piscina*  piscina)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < d->numerus; k++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (chorda_aequalis(d->res[j].fons, d->res[k].fons))
            {
                frange;
            }
        }
        si (j < k)
        {
            statuae[k] = statuae[j];
            perge;
        }
        statuae[k] = rem_componere(literae(d->res[k].fons, piscina),
            piscina);
        si (!statuae[k])
        {
            fprintf(stderr, "%s:%u: the asset of this placement was"
                " refused\n", via, (insignatus)d->res[k].linea);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* dispositionem unam struere et in volumen addere; titulus in
 * *titulus (vacuus si legi non potuit). FALSUM = recusata */
interior b32
locum_condere (
    constans character* via,
               Volumen* volumen,
                chorda* tituli,
                   s32  titulorum,
                chorda* titulus,
                 Summa* summa,
               Piscina* piscina)
{
       MundusLocusFructus   d;
    MundusLocusInstructus   instructus;
            StatuaFructus   relecta;
        StatuaeInscriptio*  prior;
        StatuaeInscriptio*  nova;
          constans Statua** statuae;
                   Statua*  basis;
                   chorda   vacua;
                   chorda   textus;
                   chorda   octeti;
                   chorda   sigillum;
       constans character*  status;
                      i32   ante;
                      i32   post;
                      s32   k;

    vacua.datum       = NIHIL;
    vacua.mensura     = ZEPHYRUM;
    titulus->datum    = NIHIL;
    titulus->mensura  = ZEPHYRUM;
    textus            = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        recusatam_nuntiare(via, "plagula legi non potest", vacua);
        redde FALSUM;
    }
    d = mundus_locus_legere(textus, piscina);
    si (!d.successus)
    {
        fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)d.linea,
            (integer)d.error.mensura,
            (constans character*)d.error.datum);
        redde FALSUM;
    }
    *titulus = d.titulus;
    si (!statuae_titulus_validus(*titulus))
    {
        recusatam_nuntiare(via, "titulus loci invalidus (I..LXIV"
            " octeti, sine '/')", *titulus);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < titulorum; k++)
    {
        si (chorda_aequalis(tituli[k], *titulus))
        {
            recusatam_nuntiare(via, "titulus iam ab alia dispositione"
                " huius cursus datus", *titulus);
            redde FALSUM;
        }
    }
    si (   inscriptio_tituli(statuae_enumerare_generis(volumen,
               STATUAE_GENUS_STATUA, piscina), *titulus)
        || inscriptio_tituli(statuae_enumerare_generis(volumen,
               STATUAE_GENUS_RES, piscina), *titulus))
    {
        recusatam_nuntiare(via, "titulus iam persona aut res in"
            " volumine est", *titulus);
        redde FALSUM;
    }
    basis    = basim_legere(via, literae(d.basis, piscina), piscina);
    statuae  = (constans Statua**)piscina_allocare(piscina,
        (d.numerus + I) * magnitudo(Statua*));
    si (   !basis || !statuae
        || !res_componere(via, &d, statuae, piscina))
    {
        redde FALSUM;
    }
    instructus = mundus_locus_struere(basis, d.res, statuae,
        d.numerus, piscina);
    si (!instructus.successus)
    {
        fprintf(stderr, "%s:%u: %.*s\n", via,
            (insignatus)instructus.linea,
            (integer)instructus.error.mensura,
            (constans character*)instructus.error.datum);
        redde FALSUM;
    }
    octeti = statua_glb_scribere(instructus.statua, NIHIL, piscina);
    si (!octeti.datum)
    {
        recusatam_nuntiare(via, "locus in GLB scribi non potest",
            vacua);
        redde FALSUM;
    }
    relecta = statua_glb_legere(octeti, piscina);
    si (!relecta.successus)
    {
        recusatam_nuntiare(via, "GLB scriptum relegi non potest",
            relecta.error);
        redde FALSUM;
    }
    ante   = cellae_gressibiles(mundus_solum_struere(basis, piscina));
    post   = cellae_gressibiles(mundus_solum_struere(relecta.statua,
        piscina));
    prior  = inscriptio_tituli(statuae_enumerare_generis(volumen,
        STATUAE_GENUS_LOCUS, piscina), *titulus);
    sigillum = prior ? prior->sigillum_hex : vacua;
    si (!statuae_addere_generis(volumen, *titulus, octeti,
        STATUAE_GENUS_LOCUS))
    {
        recusatam_nuntiare(via, "volumen additionem recusavit",
            chorda_ex_literis(volumen_error(volumen)
                ? volumen_error(volumen) : "?", piscina));
        redde FALSUM;
    }
    nova = inscriptio_tituli(statuae_enumerare_generis(volumen,
        STATUAE_GENUS_LOCUS, piscina), *titulus);
    si (!prior)
    {
        summa->additae++;
        status = "addita";
    }
    alioquin si (nova && chorda_aequalis(nova->sigillum_hex, sigillum))
    {
        summa->eaedem++;
        status = "eadem";
    }
    alioquin
    {
        summa->mutatae++;
        status = "mutata";
    }
    imprimere("%-9s %.*s   %u res, %u vertices,"
        " cellae gressibiles %u -> %u\n", status,
        (integer)titulus->mensura,
        (constans character*)titulus->datum, (insignatus)d.numerus,
        (insignatus)relecta.statua->verticum, (insignatus)ante,
        (insignatus)post);
    redde VERUM;
}

interior s32
exsequi_condere (
    constans character* via_voluminis,
    constans character* directorium,
               Piscina* piscina)
{
      Volumen* volumen;
        Summa  summa;
       chorda* nomina;
       chorda* tituli;
    character  via[VIA_MAXIMA];
          s32  numerus;
          s32  titulorum = ZEPHYRUM;
          s32  k;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    nomina = (chorda*)piscina_allocare(piscina,
        PLAGULAE_MAXIMAE * magnitudo(chorda));
    tituli = (chorda*)piscina_allocare(piscina,
        PLAGULAE_MAXIMAE * magnitudo(chorda));
    si (!nomina || !tituli)
    {
        fprintf(stderr, "mundus: memoria deficit\n");
        redde I;
    }
    numerus = plagulas_colligere(directorium, nomina, piscina);
    si (numerus < ZEPHYRUM)
    {
        fprintf(stderr, "mundus: directorium '%s' legi non potest"
            " (aut plus quam %u plagulae)\n", directorium,
            (insignatus)PLAGULAE_MAXIMAE);
        redde I;
    }
    si (numerus == ZEPHYRUM)
    {
        fprintf(stderr, "mundus: nullae plagulae *.stml in '%s'\n",
            directorium);
        redde I;
    }
    volumen = volumen_aperire_aut_creare(piscina, via_voluminis);
    si (!volumen)
    {
        fprintf(stderr, "mundus: volumen %s aperiri non potest\n",
            via_voluminis);
        redde I;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        chorda titulus;

        si (strlen(directorium) + (memoriae_index)nomina[k].mensura
            + II > VIA_MAXIMA)
        {
            fprintf(stderr, "mundus: via nimis longa in '%s'\n",
                directorium);
            summa.recusatae++;
            perge;
        }
        sprintf(via, "%s/%.*s", directorium,
            (integer)nomina[k].mensura,
            (constans character*)nomina[k].datum);
        si (!locum_condere(via, volumen, tituli, titulorum, &titulus,
            &summa, piscina))
        {
            summa.recusatae++;
        }
        si (titulus.mensura)
        {
            tituli[titulorum++] = titulus;
        }
    }
    imprimere("mundus: %u additae, %u mutatae, %u eaedem,"
        " %u recusatae\n", (insignatus)summa.additae,
        (insignatus)summa.mutatae, (insignatus)summa.eaedem,
        (insignatus)summa.recusatae);
    volumen_claudere(volumen);
    redde summa.recusatae ? I : ZEPHYRUM;
}


/* ==================================================
 * specimen
 * ================================================== */

/* duo numeri "x z"; FALSUM si non */
interior b32
locum_legere (
    constans character* textus,
                   f32* x,
                   f32* z)
{
    character* finis;
          f64  a;
          f64  b;

    a = strtod(textus, &finis);
    si (finis == textus)
    {
        redde FALSUM;
    }
    textus  = finis;
    b       = strtod(textus, &finis);
    si (finis == textus)
    {
        redde FALSUM;
    }
    dum (*finis == ' ')
    {
        finis++;
    }
    si (*finis != '\0')
    {
        redde FALSUM;
    }
    *x = (f32)a;
    *z = (f32)b;
    redde VERUM;
}

/* persona iuxta rem: inter XVI directiones ad distantiam datam ea
 * ubi persona stare potest et quae in quadro maxime DEXTRORSUM
 * iacet (res non tegitur). FALSUM si nusquam stare potest */
interior b32
personam_ponere (
    constans Mundus* mundus,
       MundusCorpus* corpus,
                f32  x,
                f32  z,
                f32  distantia)
{
    StatuaScaenaVisus visus      = mundus_visus(ZEPHYRUM, ZEPHYRUM);
                  f32 optimum_x  = 0.0f;
                  f32 optimum_z  = 0.0f;
                  f32 optimum    = 0.0f;
                  b32 inventum   = FALSUM;
                  i32 k;

    per (k = ZEPHYRUM; k < DIRECTIONES; k++)
    {
        MundusCorpus proba = *corpus;
                 f32 angulus = 2.0f * STEREOMETRIA_PI * (f32)k
                     / (f32)DIRECTIONES;
                 f32 punctum_x = x + distantia * (f32)cos((f64)angulus);
                 f32 punctum_z = z + distantia * (f32)sin((f64)angulus);
                 f32 px = 0.0f;
                 f32 py = 0.0f;
                 f32 profunditas = 0.0f;

        si (!mundus_corpus_ponere(mundus, &proba, punctum_x, punctum_z))
        {
            perge;
        }
        statua_scaena_proicere(&visus,
            stereometria_vector(punctum_x, proba.y, punctum_z), &px,
            &py,
            &profunditas);
        si (!inventum || px > optimum)
        {
            inventum   = VERUM;
            optimum    = px;
            optimum_x  = punctum_x;
            optimum_z  = punctum_z;
        }
    }
    redde inventum
        && mundus_corpus_ponere(mundus, corpus, optimum_x, optimum_z);
}

interior s32
exsequi_specimen (
    constans character* via,
    constans character* exitus,
    constans character* locus,
                   f32  gyrus,
               Piscina* piscina)
{
                           MundusLocusInstructus  instructus;
                                  MundusLocusRes  positio;
                                PlasticaMensurae  mensurae;
                                   StatuaFructus  persona;
                                     MundusSolum* nudum;
                                          Mundus  mundus;
                                    MundusCorpus  corpus;
                                     MundusForma  forma;
                                      PngFructus  png;
                                           Imago  pagina;
                                 constans Statua* statuae[I];
                                          Statua* res;
                                          Statua* basis;
                                          chorda  octeti;
                                             b32  activi[XVI];
                                              i8* indices;
                                              i8* nuda;
                                  memoriae_index  k;
                                             f32  ambitus;
                                             s32  origo_x = ZEPHYRUM;
                                             s32  origo_y = ZEPHYRUM;
                                             i32  pixela_personae[II];
                                             i32  digesta[II];
                                             i32  tabula;
                                             i32  y;

    thema_initiare();
    res    = rem_componere(via, piscina);
    basis  = res ? basim_legere(via, BASIS_ORDINARIA, piscina) : NIHIL;
    si (!res || !basis)
    {
        redde I;
    }
    octeti   = filum_legere_totum(PERSONA_ORDINARIA, piscina);
    persona  = statua_glb_legere(octeti, piscina);
    nudum    = mundus_solum_struere(basis, piscina);
    si (!persona.successus || !nudum)
    {
        fprintf(stderr, "mundus: persona '%s' aut tabula gressus"
            " legi non potest\n", PERSONA_ORDINARIA);
        redde I;
    }
    memset(&positio, ZEPHYRUM, magnitudo(positio));
    positio.gyrus = gyrus;
    si (locus)
    {
        si (!locum_legere(locus, &positio.x, &positio.z))
        {
            redde usum_imprimere();
        }
    }
    alioquin si (!mundus_solum_initium(nudum, MUNDUS_CORPORIS_RADIUS,
                 &positio.x, &positio.z))
    {
        fprintf(stderr, "mundus: locus initium non habet\n");
        redde I;
    }
    statuae[ZEPHYRUM]  = res;
    instructus = mundus_locus_struere(basis, &positio, statuae, I,
        piscina);
    si (!instructus.successus)
    {
        fprintf(stderr, "%s: ad (%.2f, %.2f): %.*s\n", via,
            (f64)positio.x, (f64)positio.z,
            (integer)instructus.error.mensura,
            (constans character*)instructus.error.datum);
        redde I;
    }
    /* persona ad ambitum rei + I unitatem */
    mensurae  = plastica_metiri(res, piscina);
    ambitus   = mensurae.maximum.x;
    si (-mensurae.minimum.x > ambitus)
    {
        ambitus = -mensurae.minimum.x;
    }
    si (mensurae.maximum.z > ambitus)
    {
        ambitus = mensurae.maximum.z;
    }
    si (-mensurae.minimum.z > ambitus)
    {
        ambitus = -mensurae.minimum.z;
    }
    memset(&mundus, ZEPHYRUM, magnitudo(mundus));
    memset(&corpus, ZEPHYRUM, magnitudo(corpus));
    mundus.solum     = mundus_solum_struere(instructus.statua,
        piscina);
    mundus.corpora   = &corpus;
    mundus.corporum  = I;
    corpus.radius    = MUNDUS_CORPORIS_RADIUS;
    forma            = mundus_forma(persona.statua, piscina);
    si (   !mundus.solum
        || !personam_ponere(&mundus, &corpus, positio.x, positio.z,
               ambitus + 1.0f))
    {
        fprintf(stderr, "%s: persona iuxta rem ad (%.2f, %.2f) stare"
            " non potest\n", via, (f64)positio.x, (f64)positio.z);
        redde I;
    }
    dithering_praeparare_omnes(activi);
    pagina.latitudo = QUADRI_LATITUDO * II;
    pagina.altitudo = QUADRI_ALTITUDO;
    pagina.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)pagina.latitudo * pagina.altitudo * IV);
        indices          = (i8*)piscina_allocare(piscina,
            (memoriae_index)QUADRI_LATITUDO * QUADRI_ALTITUDO);
    nuda             = (i8*)piscina_allocare(piscina,
        (memoriae_index)QUADRI_LATITUDO * QUADRI_ALTITUDO);
    si (!pagina.pixela || !indices || !nuda)
    {
        fprintf(stderr, "mundus: memoria deficit\n");
        redde I;
    }
    per (tabula = ZEPHYRUM; tabula < II; tabula++)
    {
        MundusHumus* humus = mundus_humus_creare_modi(
            instructus.statua, activi, tabula ? MUNDUS_HUMUS_EXEMPLARIA
                                              : MUNDUS_HUMUS_ATKINSON,
            piscina);

        si (   !humus
            || !mundus_quadrum_pingere(humus, &mundus, &forma, I,
                   ZEPHYRUM, activi, CHARTA_PALETTAE, QUADRI_LATITUDO,
                   QUADRI_ALTITUDO, indices, piscina))
        {
            fprintf(stderr, "mundus: quadrum pingi non potuit\n");
            redde I;
        }
        /* humus sola eiusdem quadri: pixela quae differunt
         * persona sunt */
        si (   !mundus_quadri_origo(&mundus, ZEPHYRUM, QUADRI_LATITUDO,
                   QUADRI_ALTITUDO, &origo_x, &origo_y)
            || !mundus_humus_copiare(humus, origo_x, origo_y,
                   QUADRI_LATITUDO, QUADRI_ALTITUDO, CHARTA_PALETTAE,
                   nuda, NIHIL))
        {
            fprintf(stderr, "mundus: humus copiari non potuit\n");
            redde I;
        }
        pixela_personae[tabula]  = ZEPHYRUM;
        digesta[tabula]          = 2166136261u;
        per (k = ZEPHYRUM;
            k < (memoriae_index)QUADRI_LATITUDO * QUADRI_ALTITUDO; k++)
        {
            pixela_personae[tabula] += (i32)(indices[k] != nuda[k]);
            digesta[tabula] = (digesta[tabula] ^ indices[k])
                * 16777619u;
        }
        per (y = ZEPHYRUM; y < QUADRI_ALTITUDO; y++)
        {
            dithering_indices_ad_rgba(
                indices + (memoriae_index)y * QUADRI_LATITUDO,
                pagina.pixela + ((memoriae_index)y * pagina.latitudo
                    + (memoriae_index)tabula * QUADRI_LATITUDO) * IV,
                QUADRI_LATITUDO, I);
        }
    }
    png = imago_png_scribere(&pagina, exitus, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "mundus: PNG scribi non potest: %s\n", exitus);
        redde I;
    }
    imprimere("%s: res ad (%.2f, %.2f) altitudo %.2f, gyrus %.0f;"
        " persona ad (%.2f, %.2f)\n", via, (f64)positio.x,
        (f64)positio.z,
        (f64)instructus.statua->nodi[basis->nodorum].translatio.y,
        (f64)gyrus, (f64)corpus.x, (f64)corpus.z);
        imprimere("cellae gressibiles %u -> %u\n",
            (insignatus)cellae_gressibiles(nudum),
            (insignatus)cellae_gressibiles(mundus.solum));
    imprimere("tabula atkinson: digestum %08x, persona %u pixela\n",
        (insignatus)digesta[ZEPHYRUM],
        (insignatus)pixela_personae[ZEPHYRUM]);
    imprimere("tabula exemplaria: digestum %08x, persona %u pixela\n",
        (insignatus)digesta[I], (insignatus)pixela_personae[I]);
    imprimere("%s: %u x %u\n", exitus, (insignatus)pagina.latitudo,
        (insignatus)pagina.altitudo);
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_mundus;

s32
principale (
      integer   argc,
    character** argv)
{
    constans character* positivum  = NIHIL;
    constans character* volumen    = NIHIL;
    constans character* loci       = NIHIL;
    constans character* exitus     = NIHIL;
    constans character* locus      = NIHIL;
    constans character* gyrus      = NIHIL;
               Piscina* piscina;
             character* finis;
                   f64  gradus = 0.0;
                   b32  condere;
               integer  k;
                   s32  r;

    si (provenientia_respondere((s32)argc, argv, &provenientia_mundus))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        redde usum_imprimere();
    }
    condere = strcmp(argv[I], "condere") == ZEPHYRUM;
    si (!condere && strcmp(argv[I], "specimen") != ZEPHYRUM)
    {
        redde usum_imprimere();
    }
    per (k = II; k < argc; k++)
    {
        constans character** scopus = NIHIL;

        si (strcmp(argv[k], "-volumen") == ZEPHYRUM)
        {
            scopus = &volumen;
        }
        alioquin si (strcmp(argv[k], "-loci") == ZEPHYRUM)
        {
            scopus = &loci;
        }
        alioquin si (strcmp(argv[k], "-o") == ZEPHYRUM)
        {
            scopus = &exitus;
        }
        alioquin si (strcmp(argv[k], "-locus") == ZEPHYRUM)
        {
            scopus = &locus;
        }
        alioquin si (strcmp(argv[k], "-gyrus") == ZEPHYRUM)
        {
            scopus = &gyrus;
        }
        si (scopus)
        {
            si (k + I >= argc || *scopus)
            {
                redde usum_imprimere();
            }
            *scopus = argv[++k];
            perge;
        }
        /* numerus negativus positivum non est: vexillum ignotum */
        si (argv[k][ZEPHYRUM] == '-' || positivum)
        {
            redde usum_imprimere();
        }
        positivum = argv[k];
    }
    si (condere
        ? (!volumen || positivum || exitus || locus || gyrus)
        : (!positivum || !exitus || volumen || loci))
    {
        redde usum_imprimere();
    }
    si (gyrus)
    {
        gradus = strtod(gyrus, &finis);
        si (finis == gyrus || *finis != '\0')
        {
            redde usum_imprimere();
        }
    }
    piscina = piscina_generare_dynamicum("mundus", M * M);
    r = condere
        ? exsequi_condere(volumen, loci ? loci : LOCI_ORDINARII,
              piscina)
        : exsequi_specimen(positivum, exitus, locus, (f32)gradus,
              piscina);
    piscina_destruere(piscina);
    redde r;
}
