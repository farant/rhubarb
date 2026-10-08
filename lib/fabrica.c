/* fabrica.c - nucleus iudicis fabricae (T1): auxilia, sutura,
 * manifestum aedilis, iudicium (sigillum actionis, vestigia lectionum,
 * thesaurus, regeneratio, strategiae, fabrica_iudicare)
 *
 * Machina pura: discum et processus per FabricaSutura solam tangit.
 * Partes ceterae (fabrica-6 H2): fabrica_genera.c (genera ingressuum),
 * fabrica_declarationes.c (declarationes, composita, praecondiciones),
 * fabrica_ordo.c (dependentiae, ordo, vestigia scripturae, undae),
 * fabrica_sanare.c (sanare), fabrica_gradus.c (gradus: areae,
 * ambitus, registrum, composita graduum), fabrica_probationes_c.c
 * (genus gradus probationes_c); auxilia communia in
 * fabrica_interna.h. Vide include/fabrica.h et
 * project-specs/fabrica-spec-v2.md. */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "stml.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

chorda
fabricae_iungere (
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post)
{
    ChordaAedificator* aedificator;

    aedificator = chorda_aedificator_creare(piscina, 128);
    si (aedificator == NIHIL)
    {
        redde medium;
    }
    (vacuum)chorda_aedificator_appendere_literis(aedificator, ante);
    (vacuum)chorda_aedificator_appendere_chorda(aedificator, medium);
    (vacuum)chorda_aedificator_appendere_literis(aedificator, post);
    redde chorda_aedificator_finire(aedificator);
}

vacuum
fabricae_causam_ponere (
                 chorda* causa_out,
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post)
{
    si (causa_out != NIHIL)
    {
        *causa_out = fabricae_iungere(piscina, ante, medium, post);
    }
}

Xar*
fabricae_xar_chordarum (
    Piscina* piscina)
{
    redde xar_creare(piscina, (i32)magnitudo(chorda));
}

vacuum
fabricae_chordam_addere (
       Xar* xar,
    chorda  valor)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = valor;
    }
}

vacuum
fabrica_suturam_parare (
    FabricaSutura* sutura)
{
    /* totum primum (fabrica-6 T5): index membrorum infra iam
     * deficiebat (particulae, verdictum_ponere, repositorium) - membrum
     * novum numquam stercus manet */
    memset(sutura, ZEPHYRUM, magnitudo(FabricaSutura));
    sutura->datum               = NIHIL;
    sutura->legere              = NIHIL;
    sutura->enumerare           = NIHIL;
    sutura->currere             = NIHIL;
    sutura->rogare              = NIHIL;
    sutura->meminisse           = NIHIL;
    sutura->inscribere          = NIHIL;
    sutura->sigilla             = NIHIL;
    sutura->regenerationes      = NIHIL;
    sutura->digesta             = NIHIL;
    sutura->agere               = NIHIL;
    sutura->agere_simul         = NIHIL;
    sutura->currere_simul       = NIHIL;
    sutura->praevisio           = NIHIL;
    sutura->vestigium_capere    = NIHIL;
    sutura->cursum_inscribere   = NIHIL;
    sutura->cursum_legere       = NIHIL;
    sutura->lectiones_legere    = NIHIL;
    sutura->lectiones_scribere  = NIHIL;
    sutura->radix.datum         = NIHIL;
    sutura->radix.mensura       = ZEPHYRUM;
    sutura->auditus             = ZEPHYRUM;
    sutura->lectiones_ultimae   = NIHIL;
    sutura->ambitus             = NIHIL;
    sutura->species             = NIHIL;
    sutura->exitus_noti         = NIHIL;
    sutura->identitas           = NIHIL;

    sutura->effectus  = NIHIL;
    sutura->audita    = NIHIL;
}


/* ==================================================
 * Manifestum aedilis
 * ================================================== */

interior vacuum
_sectionem_colligere (
             StmlNodus* radix,
    constans character* sectio_titulus,
    constans character* liberi_titulus,
               Piscina* piscina,
                   Xar* viae)
{
    StmlNodus* sectio;
          Xar* liberi;
          i32  i;
          i32  numerus;

    sectio = stml_invenire_liberum(radix, sectio_titulus);
    si (sectio == NIHIL)
    {
        redde;
    }
    liberi = stml_invenire_omnes_liberos(sectio, liberi_titulus,
        piscina);
    si (liberi == NIHIL)
    {
        redde;
    }
    numerus = xar_numerus(liberi);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* nodus;
           chorda* via;

        nodus  = *(StmlNodus**)xar_obtinere(liberi, i);
        via    = stml_attributum_capere(nodus, "via");
        si (via != NIHIL)
        {
            fabricae_chordam_addere(viae, *via);
        }
    }
}

b32
fabrica_manifestum_legere (
                 chorda   contentum,
                Piscina*  piscina,
                    Xar** viae_out,
                    Xar** inresolutae_out,
                 chorda*  causa_out)
{
    InternamentumChorda* intern;
           StmlResultus  lectum;

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde FALSUM;
    }
    lectum = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "aedilis-manifestum"))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_ex_literis("manifestum malformatum",
                piscina);
        }
        redde FALSUM;
    }
    *viae_out         = fabricae_xar_chordarum(piscina);
    *inresolutae_out  = fabricae_xar_chordarum(piscina);
    /* SCOPUS = fons principalis: manifestum eum in attributo SOLO
     * nominat, numquam inter obiecta - sine hoc mutatio fontis
     * principalis binarium stalum non faceret (T7, inventum per
     * digesta aequalia canon_examen / canon_coquere) */
    {
        chorda* scopus;

        scopus = stml_attributum_capere(lectum.elementum_radix,
            "scopus");
        si (scopus != NIHIL)
        {
            fabricae_chordam_addere(*viae_out, *scopus);
        }
    }
    /* systemata consulto omissa: plagulae nostrae non sunt */
    _sectionem_colligere(lectum.elementum_radix, "obiecta", "obiectum",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "capita", "caput",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "vendores", "fons",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "inresolutae",
        "caput", piscina, *inresolutae_out);
    redde VERUM;
}


/* ==================================================
 * Iudicium
 * ================================================== */

/* numerus linearum quae differunt (positione; cauda longior tota) */
interior i32
_lineas_differentes_numerare (
    chorda a,
    chorda b)
{
    i32 i;
    i32 j;
    i32 numerus;

    i        = ZEPHYRUM;
    j        = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    dum (i < a.mensura || j < b.mensura)
    {
        i32 finis_a;
        i32 finis_b;
        b32 differt;

        finis_a = i;
        dum (finis_a < a.mensura && a.datum[finis_a] != '\n')
        {
            finis_a++;
        }
        finis_b = j;
        dum (finis_b < b.mensura && b.datum[finis_b] != '\n')
        {
            finis_b++;
        }
        differt = (i >= a.mensura || j >= b.mensura)
            || (finis_a - i) != (finis_b - j)
            || memcmp(a.datum + i, b.datum + j, (memoriae_index)(finis_a
                - i))
                != 0;
        si (differt)
        {
            numerus++;
        }
        i = (finis_a < a.mensura) ? finis_a + 1 : finis_a;
        j = (finis_b < b.mensura) ? finis_b + 1 : finis_b;
        si (i >= a.mensura && j >= b.mensura)
        {
            frange;
        }
    }
    redde numerus;
}

FabricaIudicium
fabricae_iudicium (
           chorda artificium,
    FabricaStatus status,
           chorda causa)
{
    FabricaIudicium iudicium;

    iudicium.artificium  = artificium;
    iudicium.status      = status;
    iudicium.causa       = causa;
    redde iudicium;
}

interior FabricaIudicium
_relationem_iudicare (
    constans FabricaSutura* sutura,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                   Piscina* piscina)
{
       chorda relatio;
       chorda signum;
       chorda hodie;
    character hex[SIGILLUM_HEX_MENSURA];
          s32 inventum;
          i32 index;
          i32 finis;

    si (   sutura->rogare == NIHIL
        || !sutura->rogare(sutura->datum,
               chorda_ut_cstr(exitus->via, piscina), piscina, &relatio))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
            fabricae_iungere(piscina, "sine provenientia: ",
            exitus->via,
                " -provenientia nihil reddit"));
    }
    signum    = chorda_ex_literis("ingressus ", piscina);
    inventum  = chorda_invenire_index(relatio, signum);
    si (inventum < 0)
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
            chorda_ex_literis("relatio sine linea ingressus",
                piscina));
    }
    index = (i32)inventum + signum.mensura;
    finis = index;
    dum (finis < relatio.mensura && relatio.datum[finis] != '\n')
    {
        finis++;
    }
    sigillum_hex(ingressus, hex);
    hodie = chorda_ex_literis(hex, piscina);
    si (   (finis - index) == hodie.mensura
        && memcmp(relatio.datum + index, hodie.datum,
               (memoriae_index)hodie.mensura) == 0)
    {
        redde fabricae_iudicium(exitus->via, FABRICA_RECENS,
            chorda_ex_literis("relatio congruit", piscina));
    }
    redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
        fabricae_iungere(piscina,
        "ingressus mutati post institutionem (hodie ",
            chorda_sectio(hodie, 0, VIII), "...)"));
}

/* sigillum actionis memoratum per cursum (sutura->digesta) */
nomen structura {
         b32 bonum;
    Sigillum sigillum;
      chorda causa;
} DigestumMemoratum;

/* fabrica_actionem_sigillare SEMEL per actionem et cursum: exitus
 * multi (silva XXII) ingressus eosdem non iterum explicant. Defectus
 * quoque memoratur (causa eadem). */
b32
fabricae_actionem_sigillare_semel (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out)
{
               vacuum* memoratum;
    DigestumMemoratum* locus;
                  b32  bonum;

    si (   sutura->digesta != NIHIL
        && tabula_dispersa_invenire(sutura->digesta, actio->titulus,
               &memoratum))
    {
        locus          = (DigestumMemoratum*)memoratum;
        *sigillum_out  = locus->sigillum;
        *causa_out     = locus->causa;
        redde locus->bonum;
    }
    bonum = fabrica_actionem_sigillare(sutura, actio, piscina,
        sigillum_out, causa_out);
    si (sutura->digesta != NIHIL)
    {
        locus = (DigestumMemoratum*)piscina_allocare(piscina,
            magnitudo(DigestumMemoratum));
        si (locus != NIHIL)
        {
            locus->bonum     = bonum;
            locus->sigillum  = *sigillum_out;
            locus->causa     = *causa_out;
            (vacuum)tabula_dispersa_inserere(sutura->digesta,
                actio->titulus, locus);
        }
    }
    redde bonum;
}

/* Clavis memoriae: sigillum ingressuum ET mandati (verbum NUL ...).
 * Radix nova in mandato declarata ingressus nondum mutat (manifesta
 * eius nondum scripta) - sine mandato verificatio vetus congrueret.
 * Digesta institutionum (relatio) intacta manent. */
Sigillum
fabricae_clavem_memoriae (
    constans FabricaActio* actio,
        constans Sigillum* ingressus)
{
    SigillumContextus contextus;
                  i32 i;

    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, ingressus->octeti, SIGILLUM_OCTETI);
    per (i = ZEPHYRUM; i < xar_numerus(actio->mandatum); i++)
    {
        chorda verbum;

        verbum = *(chorda*)xar_obtinere(actio->mandatum, i);
        sigillum_addere(&contextus, verbum.datum,
            (memoriae_index)verbum.mensura);
        sigillum_addere(&contextus, "", 1);
    }
    redde sigillum_finire(&contextus);
}


/* ==================================================
 * Vestigia lectionum (plan 2 T2)
 *
 * Actio lectiones="verum": generator cum FABRICA_LECTIONES currit
 * (via a nucleo electa, per suturam data); post regenerationem
 * CONGRUENTEM liber per suturam legitur, quaeque lectio statu suo
 * hodierno sigillatur, vestigium sub clave (titulus, ingressus,
 * artificium) servatur. Iudicium proximum: vestigio congruente RECENS
 * sine regeneratione.
 * ================================================== */

chorda
fabrica_liber_via (
     chorda  titulus,
    Piscina* piscina)
{
    redde fabricae_iungere(piscina, LIBRI_DIRECTORIUM, titulus, ".tsv");
}
/* THESAURUS (plan 2 T3): blobi, actiones, generationes sunt cache per
 * sigilla ingressuum iam lectorum - non ingressus. Radix ipsa
 * (obiecta .o in build/aedilis/obiecta) ingressus verus manet. */
constans character* constans fabricae_thesauri_partes[III] = {
    "build/aedilis/obiecta/blobi/",
    "build/aedilis/obiecta/actiones/",
    "build/aedilis/obiecta/generationes/"
};

interior b32
_in_thesauro (
      chorda  via,
     Piscina* piscina)
{
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (chorda_incipit(via,
            chorda_ex_literis(fabricae_thesauri_partes[k],
                piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

hic_manens constans character* constans SIGNUM_ABSENS   = "\001absens";
hic_manens constans character* constans SIGNUM_PRAESENS =
    "\001praesens";

/* status viae hodiernus ut sigillum: L contenta; A/X praesentia;
 * D nomina ordinata. FALSUM = genus non verificabile (E). */
b32
fabricae_lectionem_sigillare (
    constans FabricaSutura* sutura,
               LectioGenus  genus,
                    chorda  via,
                   Piscina* piscina,
                  Sigillum* sigillum_out)
{
     constans character* via_cstr;
                 chorda  contentum;
                    Xar* nomina;
                    b32  legibile;
                    b32  est_directorium;

    via_cstr     = chorda_ut_cstr(via, piscina);
    /* E (spec 3 par. XII): via = titulus variabilis; valor quem fabrica
     * portae dat. Absens et vacua distinguuntur. */
    si (genus == LECTIO_AMBITUS)
    {
        chorda valor;

        si (sutura->ambitus == NIHIL)
        {
            redde FALSUM;
        }
        *sigillum_out = sutura->ambitus(sutura->datum, via_cstr,
            piscina,
                &valor)
            ? sigillum_computare(valor.datum,
                  (memoriae_index)valor.mensura)
            : sigillum_computare(SIGNUM_ABSENS, strlen(SIGNUM_ABSENS));
        redde VERUM;
    }
    /* L (fabrica-6 T10): per MEMORIAM CURSUS (sutura->sigilla, via ->
     * sigillum octetorum, ut ingressus declarati) - membra gradus
     * corpus idem legunt (toml: VIII membra MMD plagulas easdem), olim
     * quodque iterum legebat et sigillabat; nec directorium pro L
     * enumeratur */
    si (genus == LECTIO_LEGIT)
    {
        vacuum* memoratum;

        si (   sutura->sigilla != NIHIL
            && tabula_dispersa_invenire(sutura->sigilla, via,
            &memoratum))
        {
            *sigillum_out = *(Sigillum*)memoratum;
            redde VERUM;
        }
        si (!sutura->legere(sutura->datum, via_cstr, piscina,
            &contentum))
        {
            *sigillum_out = sigillum_computare(SIGNUM_ABSENS,
                strlen(SIGNUM_ABSENS));
            redde VERUM;
        }
        *sigillum_out = sigillum_computare(contentum.datum,
            (memoriae_index)contentum.mensura);
        si (sutura->sigilla != NIHIL)
        {
            Sigillum* locus = (Sigillum*)piscina_allocare(piscina,
                magnitudo(Sigillum));

            si (locus != NIHIL)
            {
                *locus = *sigillum_out;
                (vacuum)tabula_dispersa_inserere(sutura->sigilla, via,
                    locus);
            }
        }
        redde VERUM;
    }
    legibile     = sutura->legere(sutura->datum, via_cstr, piscina,
        &contentum);
    est_directorium  = (sutura->enumerare != NIHIL)
        && sutura->enumerare(sutura->datum, via_cstr, piscina, &nomina);
    commutatio (genus)
    {
        casus LECTIO_LEGIT:
            *sigillum_out = legibile
                ? sigillum_computare(contentum.datum,
                      (memoriae_index)contentum.mensura)
                : sigillum_computare(SIGNUM_ABSENS,
                      strlen(SIGNUM_ABSENS));
            redde VERUM;
        casus LECTIO_ABSENS:
        casus LECTIO_EXSTAT:
            *sigillum_out = (legibile || est_directorium)
                ? sigillum_computare(SIGNUM_PRAESENS,
                      strlen(SIGNUM_PRAESENS))
                : sigillum_computare(SIGNUM_ABSENS,
                      strlen(SIGNUM_ABSENS));
            redde VERUM;
        casus LECTIO_ENUMERAVIT:
            si (!est_directorium)
            {
                *sigillum_out = sigillum_computare(SIGNUM_ABSENS,
                    strlen(SIGNUM_ABSENS));
            }
            alioquin
            {
                SigillumContextus contextus;
                              i32 i;

                sigillum_incipere(&contextus);
                per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
                {
                    chorda titulus;

                    titulus = *(chorda*)xar_obtinere(nomina, i);
                    sigillum_addere(&contextus, titulus.datum,
                        (memoriae_index)titulus.mensura);
                    sigillum_addere(&contextus, "\n", 1);
                }
                *sigillum_out = sigillum_finire(&contextus);
            }
            redde VERUM;
        casus LECTIO_SCRIPSIT:
        casus LECTIO_AMBITUS:
            redde FALSUM;
    }
    redde FALSUM;
}

interior LectioGenus
_genus_litterae (
    character  littera,
          b32* notum_out);

/* radices systematis: in vestigio portae omittuntur - identitas_clang
 * eas figit (spec 3 par. XII, A1). Ceterae viae absolutae sigillantur. */
hic_manens constans character* constans _radices_systematis[] = {
    "/usr/", "/Library/Developer/", "/Applications/Xcode.app/",
    "/System/", NIHIL
};

/* radices TEMPORARIAE (plan 5 T5b): status ambientis, non ingressus -
 * probationes areas suas ibi faciunt (mkdir per shell, S non notatum)
 * et /tmp ipsum enumerant; ut effectus classis temporaria (spec-2
 * par. IX). Radix ipsa aut via sub ea. */
hic_manens constans character* constans _radices_temporariae[] = {
    "/tmp", "/private/tmp", "/var/folders", "/private/var/folders",
    NIHIL
};

interior b32
_sub_temporaria (
    chorda via)
{
    i32 k;

    per (k = ZEPHYRUM; _radices_temporariae[k] != NIHIL; k++)
    {
        memoriae_index n = strlen(_radices_temporariae[k]);

        si (   (memoriae_index)via.mensura                   >= n
            && memcmp(via.datum, _radices_temporariae[k], n) == ZEPHYRUM
            && (   (memoriae_index)via.mensura == n
                || via.datum[n] == (i8)'/'))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* sub build/ : 'build/...' aut '.../build/...' */
interior b32
_sub_build (
    chorda via)
{
    i32 k;

    si (via.mensura >= VI && memcmp(via.datum, "build/", VI) == 0)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k + VII <= via.mensura; k++)
    {
        si (memcmp(via.datum + k, "/build/", VII) == 0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* VESTIGIUM TRANSITUS (spec 3 par. XII): liber cursus IN LOCO actionis
 * iudicium. Leges (generator via _lectiones_colligere manet):
 *   - via cum S in libro eodem = EXITUS cursus: L/X/A eius omittuntur
 *   - L/X sub build/ sine S: exitus actionis declaratae (exitus_noti)
 *     esse debet - aliter IGNOTUM 'ingressus build/ sine domino'
 *   - absoluta sub radicibus systematis omittitur; cetera sigillatur
 *   - species ALIA (FIFO, socket, machina) -> IGNOTUM
 *   - E: servatur solum si valor = ambitus quem fabrica portae dat
 *     (externus); differens intra portam positus est - omittitur
 *   - via INGRESSUS DECLARATI actionis omittitur: clavis eam iam tegit,
 *     lege generis sui (v4: instrumentum_domus per provenientiam -
 *     aedilis octetos binarii sui legit, relinkatio vestigium aliter
 *     moveret)
 * FALSUM + causa: transitus non reutilis (vestigium nullum). */
b32
fabricae_lectiones_transitus_colligere (
    constans FabricaSutura*  sutura,
     constans FabricaActio*  actio,
        constans character*  liber_via,
                   Piscina*  piscina,
                       Xar** lectiones_out,
                    chorda*  causa_out)
{
             chorda  liber;
                Xar* lectiones;
     TabulaDispersa* visae;
     TabulaDispersa* scriptae;
                i32  i;
                i32  initium;
                i32  transitus;

    si (!sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        *causa_out = chorda_ex_literis("liber lectionum absens",
            piscina);
        redde FALSUM;
    }
    lectiones  = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    visae      = tabula_dispersa_creare_chorda(piscina, 1024);
    scriptae   = tabula_dispersa_creare_chorda(piscina, 256);
    si (lectiones == NIHIL || visae == NIHIL || scriptae == NIHIL)
    {
        *causa_out = chorda_ex_literis("memoria deficit", piscina);
        redde FALSUM;
    }
    /* ingressus declarati: clavis eos tegit */
    per (i = ZEPHYRUM; i < xar_numerus(actio->ingressus); i++)
    {
        (vacuum)tabula_dispersa_inserere(scriptae,
            ((FabricaIngressus*)xar_obtinere(actio->ingressus, i))->via,
            NIHIL);
    }
    /* transitus 0: S colliguntur; transitus I: lectiones */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        initium = ZEPHYRUM;
        per (i = ZEPHYRUM; i < liber.mensura; i++)
        {
                   chorda  linea;
                   chorda  via;
              LectioGenus  genus;
                      b32  notum;
            FabricaLectio* lectio;
                   chorda  clavis;

            si (liber.datum[i] != '\n')
            {
                perge;
            }
            linea    = chorda_sectio(liber, initium, i);
            initium  = i + I;
            si (linea.mensura < III || linea.datum[I] != '\t')
            {
                perge;
            }
            genus = _genus_litterae((character)linea.datum[0], &notum);
            si (!notum)
            {
                perge;
            }
            via = chorda_sectio(linea, II, linea.mensura);
            si (genus == LECTIO_AMBITUS)
            {
                chorda titulus;
                chorda valor;
                chorda hodie;
                   b32 adest;
                   b32 hodie_adest;
                   s32 t;

                si (transitus == ZEPHYRUM)
                {
                    perge;
                }
                si (sutura->ambitus == NIHIL)
                {
                    *causa_out = chorda_ex_literis(
                        "ambitus non verificabilis (sutura)", piscina);
                    redde FALSUM;
                }
                t = chorda_invenire_index(via, chorda_ex_literis("\t",
                    piscina));
                adest    = t >= ZEPHYRUM;
                titulus  = adest ? chorda_sectio(via, 0, (i32)t) : via;
                valor    = adest ? chorda_sectio(via, (i32)t + I,
                    via.mensura) : chorda_ex_literis("", piscina);
                hodie_adest = sutura->ambitus(sutura->datum,
                    chorda_ut_cstr(titulus, piscina), piscina, &hodie);
                si (   adest != hodie_adest
                    || (adest && !chorda_aequalis(valor, hodie)))
                {
                    perge;   /* intra portam positum (cursor) */
                }
                clavis = fabricae_iungere(piscina, "E", titulus, "");
                si (tabula_dispersa_continet(visae, clavis))
                {
                    perge;
                }
                (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
                lectio = (FabricaLectio*)xar_addere(lectiones);
                si (lectio == NIHIL)
                {
                    redde FALSUM;
                }
                lectio->genus  = LECTIO_AMBITUS;
                lectio->via    = titulus;
                lectio->sigillum = adest
                    ? sigillum_computare(valor.datum,
                          (memoriae_index)valor.mensura)
                    : sigillum_computare(SIGNUM_ABSENS,
                          strlen(SIGNUM_ABSENS));
                perge;
            }
            si (   sutura->radix.mensura > 0
                && via.mensura > sutura->radix.mensura
                && chorda_incipit(via, sutura->radix)
                && via.datum[sutura->radix.mensura] == '/')
            {
                via = chorda_sectio(via, sutura->radix.mensura + I,
                    via.mensura);
            }
            dum (   via.mensura > II && via.datum[0] == '.'
                 && via.datum[I] == '/')
            {
                via = chorda_sectio(via, II, via.mensura);
            }
            si (via.mensura == 0)
            {
                perge;
            }
            si (genus == LECTIO_SCRIPSIT)
            {
                si (transitus == ZEPHYRUM)
                {
                    (vacuum)tabula_dispersa_inserere(scriptae, via,
                        NIHIL);
                }
                perge;
            }
            si (transitus == ZEPHYRUM)
            {
                perge;
            }
            /* exitus cursus ipsius: non ingressus */
            si (   tabula_dispersa_continet(scriptae, via)
                || chorda_incipit(via,
                chorda_ex_literis(LIBRI_DIRECTORIUM,
                       piscina))
                || _in_thesauro(via, piscina))
            {
                perge;
            }
            /* in vestigio PROPRIO (non communi): opus actionis, non
             * ingressus - filii (sqlite, redirectio) S non notant
             * (plan 5 T5b). Commune alienis patet: non excusat. */
            si (   actio->vestigia != NIHIL
                && fabricae_in_locis(actio->vestigia, via, piscina))
            {
                perge;
            }
            si (via.datum[0] == '/')
            {
                i32 k;
                b32 systematis = FALSUM;

                per (k = ZEPHYRUM; _radices_systematis[k] != NIHIL; k++)
                {
                    si (chorda_incipit(via, chorda_ex_literis(
                            _radices_systematis[k], piscina)))
                    {
                        systematis = VERUM;
                    }
                }
                si (systematis || _sub_temporaria(via))
                {
                    perge;
                }
            }
            alioquin si (   (genus == LECTIO_LEGIT
                         || genus == LECTIO_EXSTAT)
                         && _sub_build(via)
                         && (   sutura->exitus_noti == NIHIL
                             || !tabula_dispersa_continet(
                                    sutura->exitus_noti, via)))
            {
                *causa_out = fabricae_iungere(piscina,
                    "ingressus build/ sine domino: ", via, "");
                redde FALSUM;
            }
            si (   (genus == LECTIO_LEGIT || genus == LECTIO_EXSTAT)
                && sutura->species != NIHIL
                && sutura->species(sutura->datum,
                       chorda_ut_cstr(via, piscina))
                   == (i32)FABRICA_SPECIES_ALIA)
            {
                *causa_out = fabricae_iungere(piscina,
                    "lectio non sigillabilis (FIFO, socket, machina): ",
                    via, "");
                redde FALSUM;
            }
            clavis = fabricae_iungere(piscina, "", chorda_sectio(linea,
                0, I),
                chorda_ut_cstr(via, piscina));
            si (tabula_dispersa_continet(visae, clavis))
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
            lectio = (FabricaLectio*)xar_addere(lectiones);
            si (lectio == NIHIL)
            {
                redde FALSUM;
            }
            lectio->genus  = genus;
            lectio->via    = via;
            si (!fabricae_lectionem_sigillare(sutura, genus, via,
                piscina,
                    &lectio->sigillum))
            {
                *causa_out = fabricae_iungere(piscina,
                    "lectio non sigillabilis: ",
                    via, "");
                redde FALSUM;
            }
        }
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

interior LectioGenus
_genus_litterae (
    character  littera,
          b32* notum_out)
{
    *notum_out = VERUM;
    commutatio (littera)
    {
        casus 'L': redde LECTIO_LEGIT;
        casus 'A': redde LECTIO_ABSENS;
        casus 'X': redde LECTIO_EXSTAT;
        casus 'D': redde LECTIO_ENUMERAVIT;
        casus 'S': redde LECTIO_SCRIPSIT;
        casus 'E': redde LECTIO_AMBITUS;
    }
    *notum_out = FALSUM;
    redde LECTIO_LEGIT;
}

/* librum legere et vestigium facere: S praetermittitur (exitus, non
 * ingressus); radix absoluta et './' demuntur; scriptura iudicis,
 * libri ipsi et viae absolutae extra arborem praetermittuntur;
 * (genus, via) semel. FALSUM = liber absens aut lectio non
 * verificabilis (E) - nullum vestigium (regeneratio semper). */
interior b32
_lectiones_colligere (
    constans FabricaSutura*  sutura,
        constans character*  liber_via,
                    chorda   scriptura_dir,
                   Piscina*  piscina,
                       Xar** lectiones_out)
{
             chorda  liber;
                Xar* lectiones;
     TabulaDispersa* visae;
                i32  i;
                i32  initium;

    si (!sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        redde FALSUM;
    }
    lectiones  = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    visae      = tabula_dispersa_creare_chorda(piscina, 256);
    si (lectiones == NIHIL || visae == NIHIL)
    {
        redde FALSUM;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
               chorda  linea;
               chorda  via;
          LectioGenus  genus;
                  b32  notum;
        FabricaLectio* lectio;

        si (liber.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(liber, initium, i);
        initium  = i + I;
        si (linea.mensura < III || linea.datum[I] != '\t')
        {
            perge;
        }
        genus = _genus_litterae((character)linea.datum[0], &notum);
        si (!notum || genus == LECTIO_SCRIPSIT)
        {
            perge;
        }
        si (genus == LECTIO_AMBITUS)
        {
            redde FALSUM;   /* ambitus a nucleo non verificatur */
        }
        via = chorda_sectio(linea, II, linea.mensura);
        si (   sutura->radix.mensura > 0
            && via.mensura > sutura->radix.mensura
            && chorda_incipit(via, sutura->radix)
            && via.datum[sutura->radix.mensura] == '/')
        {
            via = chorda_sectio(via, sutura->radix.mensura + I,
                via.mensura);
        }
        dum (   via.mensura > II && via.datum[0] == '.'
             && via.datum[I] == '/')
        {
            via = chorda_sectio(via, II, via.mensura);
        }
        si (   via.mensura == 0 || via.datum[0] == '/'
            || chorda_incipit(via, scriptura_dir)
            || chorda_incipit(via, chorda_ex_literis(LIBRI_DIRECTORIUM,
                   piscina))
            || _in_thesauro(via, piscina))
        {
            perge;
        }
        {
            chorda clavis;

            clavis = fabricae_iungere(piscina, "", chorda_sectio(linea,
                0, I),
                chorda_ut_cstr(via, piscina));
            si (tabula_dispersa_continet(visae, clavis))
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
        }
        lectio = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL)
        {
            redde FALSUM;
        }
        lectio->genus  = genus;
        lectio->via    = via;
        si (!fabricae_lectionem_sigillare(sutura, genus, via, piscina,
                &lectio->sigillum))
        {
            redde FALSUM;
        }
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* VERUM si omnis lectio vestigii hodie idem sigillum dat */
interior b32
_lectiones_congruunt (
    constans FabricaSutura* sutura,
              constans Xar* lectiones,
                   Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio;
                      Sigillum  hodie;

        lectio = (constans FabricaLectio*)xar_obtinere(lectiones, i);
        si (   !fabricae_lectionem_sigillare(sutura, lectio->genus,
            lectio->via,
                   piscina, &hodie)
            || memcmp(hodie.octeti, lectio->sigillum.octeti,
                   SIGILLUM_OCTETI) != 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* auditus memoriae: ictus hic regenerandus? (sub -plenus solum;
 * specimen determinatum per clavem - nullus numerator servandus) */
interior b32
_auditum_eligere (
    constans FabricaSutura* sutura,
                       b32  plenus,
         constans Sigillum* clavis)
{
    si (!plenus || sutura->auditus == 0 || sutura->currere == NIHIL)
    {
        redde FALSUM;
    }
    si (sutura->auditus == I)
    {
        redde VERUM;
    }
    redde (clavis->octeti[0] % sutura->auditus) == 0;
}

/* Generatorem actionis currere SEMEL per cursum (exitus multi,
 * generator unus): exitus cursus - felix aut causa fracti - in
 * sutura->regenerationes memoratur; exitus sequentes scripturam
 * eandem legunt. */
interior b32
_regenerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  scriptura_dir,
        constans character* liber_via,
                   Piscina* piscina,
                    chorda* causa_out)
{
    vacuum* memoratum;
    chorda* locus;
       b32  felix;
       i32  duratio;

    si (   sutura->regenerationes != NIHIL
        && tabula_dispersa_invenire(sutura->regenerationes,
               actio->titulus, &memoratum))
    {
        *causa_out = *(chorda*)memoratum;
        redde causa_out->mensura == 0;
    }
    /* PRAEVISIO (T6b): petitio colligitur (semel per titulum), nihil
     * curritur, nihil memoratur - iudicium hoc abicitur */
    si (sutura->praevisio != NIHIL)
    {
        i32 k;
        b32 nota;

        nota = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(sutura->praevisio); k++)
        {
            si (chorda_aequalis((*(FabricaActio**)xar_obtinere(
                    sutura->praevisio, k))->titulus, actio->titulus))
            {
                nota = VERUM;
                frange;
            }
        }
        si (!nota && actio->lectiones)
        {
            *(constans FabricaActio**)xar_addere(sutura->praevisio) =
                actio;
        }
        *causa_out = chorda_ex_literis("praevisio", piscina);
        redde FALSUM;
    }
    causa_out->datum    = NIHIL;
    causa_out->mensura  = ZEPHYRUM;
    duratio             = ZEPHYRUM;
    felix = sutura->currere(sutura->datum, actio->mandatum,
        chorda_ut_cstr(scriptura_dir, piscina), liber_via, piscina,
        causa_out, &duratio);
    si (felix)
    {
        causa_out->datum    = NIHIL;
        causa_out->mensura  = ZEPHYRUM;
    }
    alioquin si (causa_out->mensura == 0)
    {
        /* fractus sine causa: memoria 'mensura 0 = felix' falleret */
        *causa_out = chorda_ex_literis("causa ignota", piscina);
    }
    si (sutura->regenerationes != NIHIL)
    {
        locus = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (locus != NIHIL)
        {
            *locus = *causa_out;
            (vacuum)tabula_dispersa_inserere(sutura->regenerationes,
                actio->titulus, locus);
        }
    }
    /* cursus iudicis (parcum …AR15): omnis regeneratio VERA (non
     * memorata) metitur - ubi tempus iudicii consumitur */
    si (sutura->cursum_inscribere != NIHIL)
    {
        FabricaSanatio iudicium;

        memset(&iudicium, ZEPHYRUM, magnitudo(iudicium));
        iudicium.actio         = actio;
        iudicium.eventus       = FABRICA_IUDICIUM;
        iudicium.causa         = *causa_out;
        iudicium.duratio_ms    = duratio;
        iudicium.tempus_notum  = VERUM;
        sutura->cursum_inscribere(sutura->datum, &iudicium);
    }
    redde felix;
}

vacuum
fabrica_regenerationes_praevidere (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                       b32  plenus,
                   Piscina* piscina)
{
               FabricaSutura   copia;
                         Xar*  petitae;
       constans FabricaActio** membra;
          constans character** scripturae;
          constans character** libri;
                         b32*  felices;
                      chorda*  causae;
                         i32*  durationes;
                         i32   i;
                         i32   j;
                         i32   n;

    si (   sutura->currere_simul  == NIHIL
        || sutura->regenerationes == NIHIL)
    {
        redde;
    }
    /* I. iudicium in modo colligendi (copia suturae: tabulae communes) */
    petitae = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
    si (petitae == NIHIL)
    {
        redde;
    }
    copia            = *sutura;
    copia.praevisio  = petitae;
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(actiones, i);
        si (!actio->lectiones)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            FabricaExitus* exitus;

            exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
            si (exitus->strategia->iudicatur)
            {
                (vacuum)fabrica_iudicare(&copia, actio, exitus, plenus,
                    piscina);
            }
        }
    }
    n = xar_numerus(petitae);
    si (n == ZEPHYRUM)
    {
        redde;
    }
    /* ordo tituli: cursus deterministici */
    per (i = I; i < n; i++)
    {
        per (j = i; j > ZEPHYRUM; j--)
        {
            constans FabricaActio** a;
            constans FabricaActio** b;
            constans FabricaActio*  t;

            a = (constans FabricaActio**)xar_obtinere(petitae, j - I);
            b = (constans FabricaActio**)xar_obtinere(petitae, j);
            si (chorda_comparare((*a)->titulus, (*b)->titulus) <= 0)
            {
                frange;
            }
            t   = *a;
            *a  = *b;
            *b  = t;
        }
    }
    /* II. simul: scriptura et liber ut in iudicio (eaedem viae) */
    membra     = (constans FabricaActio**)piscina_allocare(piscina,
        magnitudo(FabricaActio*) * (memoriae_index)n);
    scripturae = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)n);
    libri      = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)n);
    felices    = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)n);
    causae     = (chorda*)piscina_allocare(piscina,
        magnitudo(chorda) * (memoriae_index)n);
    durationes = (i32*)piscina_allocare(piscina,
        magnitudo(i32) * (memoriae_index)n);
    si (   membra  == NIHIL || scripturae == NIHIL || libri == NIHIL
        || felices == NIHIL || causae == NIHIL || durationes == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        membra[i] = *(constans FabricaActio**)xar_obtinere(petitae, i);
        scripturae[i] = chorda_ut_cstr(fabricae_iungere(piscina,
            "build/fabrica/scriptura/", membra[i]->titulus, ""),
            piscina);
        libri[i] = chorda_ut_cstr(fabricae_iungere(piscina,
            LIBRI_DIRECTORIUM,
            membra[i]->titulus, ".tsv"), piscina);
        felices[i]     = FALSUM;
        causae[i]      = chorda_ex_literis("", piscina);
        durationes[i]  = ZEPHYRUM;
    }
    sutura->currere_simul(sutura->datum,
        (constans FabricaActio* constans*)membra, n,
        (constans character* constans*)scripturae,
        (constans character* constans*)libri, piscina, felices, causae,
        durationes);
    /* III. memoria regenerationum ut _regenerare eam scriberet */
    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda* locus;

        si (felices[i])
        {
            causae[i] = chorda_ex_literis("", piscina);
        }
        alioquin si (causae[i].mensura == 0)
        {
            causae[i] = chorda_ex_literis("causa ignota", piscina);
        }
        locus = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (locus != NIHIL)
        {
            *locus = causae[i];
            (vacuum)tabula_dispersa_inserere(sutura->regenerationes,
                membra[i]->titulus, locus);
        }
        si (sutura->cursum_inscribere != NIHIL)
        {
            FabricaSanatio iudicium;

            memset(&iudicium, ZEPHYRUM, magnitudo(iudicium));
            iudicium.actio         = membra[i];
            iudicium.eventus       = FABRICA_IUDICIUM;
            iudicium.causa         = causae[i];
            iudicium.duratio_ms    = durationes[i];
            iudicium.tempus_notum  = VERUM;
            sutura->cursum_inscribere(sutura->datum, &iudicium);
        }
    }
}


/* ==================================================
 * Strategiae: verbum iudicare
 * ================================================== */

interior FabricaIudicium
_relatione_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
    (vacuum)actio;
    (vacuum)plenus;
    redde _relationem_iudicare(sutura, exitus, ingressus, piscina);
}

/* REGENERATIO, memoria ante eam (actionibus memorabilibus solis) */
interior FabricaIudicium
_regeneratione_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
              Sigillum  clavis;
              Sigillum  artificium;
                chorda  causa;
                chorda  commissum;
                chorda  effusio;
                chorda  scriptura_dir;
                chorda  scriptura_via;
                chorda  generator;
             character  numerus[32];
                   Xar* vestigium;
    constans character* liber_via;
                   b32  ex_memoria;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), piscina, &commissum))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
            chorda_ex_literis("artificium absens", piscina));
    }
    artificium = sigillum_computare(commissum.datum,
        (memoriae_index)commissum.mensura);
    clavis      = fabricae_clavem_memoriae(actio, ingressus);
    ex_memoria  = FALSUM;
    /* VESTIGIUM LECTIONUM (plan 2 T2): actio lectiones="verum" quam
     * cursus congruens sub eadem clave ingressuum et eodem artificio
     * legit - si omnis lectio hodie idem sigillum dat, RECENS sine
     * regeneratione (etiam celer). */
    si (   actio->lectiones
        && sutura->lectiones_legere != NIHIL
        && sutura->lectiones_legere(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina),
               chorda_ut_cstr(exitus->via, piscina), &clavis,
               &artificium, piscina, &vestigium)
        && _lectiones_congruunt(sutura, vestigium, piscina))
    {
        si (!_auditum_eligere(sutura, plenus, &clavis))
        {
            redde fabricae_iudicium(exitus->via, FABRICA_RECENS,
                chorda_ex_literis("lectiones congruunt (vestigium)",
                    piscina));
        }
        ex_memoria = VERUM;   /* auditus: regeneratur tamen */
    }
    /* memoria SOLUM pro actione memorabili: ingressus aliter non
     * probabiliter pleni, et verificatio vetus mutationem ingressus
     * non declarati celaret */
    si (   !ex_memoria
        && actio->memorabilis
        && sutura->meminisse != NIHIL
        && sutura->meminisse(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina), &clavis,
               &artificium))
    {
        si (!_auditum_eligere(sutura, plenus, &clavis))
        {
            redde fabricae_iudicium(exitus->via, FABRICA_RECENS,
                chorda_ex_literis("memoria", piscina));
        }
        ex_memoria = VERUM;
    }
    /* celer: regeneratio omissa nisi actio 'celer' (vilis) sit */
    si ((!plenus && !actio->celer) || sutura->currere == NIHIL)
    {
        redde fabricae_iudicium(exitus->via, FABRICA_NON_IUDICATUM,
            chorda_ex_literis("celer: regeneratio omissa", piscina));
    }

    generator = (xar_numerus(actio->mandatum) > 0)
        ? *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM)
        : actio->titulus;
    scriptura_dir = fabricae_iungere(piscina,
        "build/fabrica/scriptura/",
        actio->titulus, "");
    liber_via = actio->lectiones
        ? chorda_ut_cstr(fabricae_iungere(piscina, LIBRI_DIRECTORIUM,
              actio->titulus, ".tsv"), piscina)
        : NIHIL;
    si (!_regenerare(sutura, actio, scriptura_dir, liber_via, piscina,
            &causa))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
            fabricae_iungere(piscina, "generator fractus: ", generator,
                chorda_ut_cstr(fabricae_iungere(piscina, ": ", causa,
                ""),
                    piscina)));
    }
    scriptura_via = fabricae_iungere(piscina, "",
        fabricae_iungere(piscina, "", scriptura_dir, "/"),
        chorda_ut_cstr(exitus->scriptura, piscina));
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(scriptura_via, piscina), piscina,
            &effusio))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
            fabricae_iungere(piscina, "generator nihil scripsit: ",
                scriptura_via, ""));
    }
    si (chorda_aequalis(effusio, commissum))
    {
        si (   actio->lectiones && liber_via != NIHIL
            && sutura->lectiones_scribere != NIHIL
            && _lectiones_colligere(sutura, liber_via, scriptura_dir,
                   piscina, &vestigium))
        {
            sutura->lectiones_scribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina),
                chorda_ut_cstr(exitus->via, piscina), &clavis,
                &artificium, vestigium);
        }
        si (actio->memorabilis && sutura->inscribere != NIHIL)
        {
            sutura->inscribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina), &clavis,
                &artificium);
        }
        redde fabricae_iudicium(exitus->via, FABRICA_RECENS,
            chorda_ex_literis(
            ex_memoria ? "auditus: regeneratio congruit"
                       : "regeneratio congruit", piscina));
    }
    sprintf(numerus, "%u",
        (insignatus integer)_lineas_differentes_numerare(commissum,
            effusio));
    si (ex_memoria)
    {
        /* memoria aut vestigium RECENS dicebat: foramen ingressus -
         * nominatur et in cursu notatur */
        si (sutura->cursum_inscribere != NIHIL)
        {
            FabricaSanatio auditum;

            memset(&auditum, ZEPHYRUM, magnitudo(auditum));
            auditum.actio    = actio;
            auditum.eventus  = FABRICA_AUDITUM_DISCORS;
            auditum.causa         = chorda_ex_literis(
                "memoria RECENS, regeneratio differt", piscina);
            auditum.duratio_ms    = ZEPHYRUM;
            auditum.tempus_notum  = FALSUM;
            sutura->cursum_inscribere(sutura->datum, &auditum);
        }
        redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
            fabricae_iungere(piscina,
            "AUDITUM DISCORS: memoria/vestigium RECENS dicebat,"
            " regeneratio differt (lineae differentes: ",
            chorda_ex_literis(numerus, piscina), ")"));
    }
    redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
        fabricae_iungere(piscina,
        "regeneratio differt (lineae differentes: ",
            chorda_ex_literis(numerus, piscina), ")"));
}

/* via lectionis primae vestigii quae hodie aliud sigillum dat (aut
 * 'ambitus <nomen>'); chorda vacua si omnes congruunt */
interior chorda
_lectionem_differentem (
    constans FabricaSutura* sutura,
              constans Xar* lectiones,
                   Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio;
                      Sigillum  hodie;

        lectio = (constans FabricaLectio*)xar_obtinere(lectiones, i);
        si (   !fabricae_lectionem_sigillare(sutura, lectio->genus,
            lectio->via,
                   piscina, &hodie)
            || memcmp(hodie.octeti, lectio->sigillum.octeti,
                   SIGILLUM_OCTETI) != 0)
        {
            redde lectio->via;
        }
    }
    redde chorda_ex_literis("", piscina);
}

/* PARTICULAE TRANSITUS (plan-5 T1): ingressus declarati singuli,
 * ordine (exclusa ut fabrica_actionem_sigillare), cum '<mandatum>' et
 * '<verdictum>' - clavis transitus ex eisdem fit, ergo differentia
 * earum nominat QUIS eam mutavit */
b32
fabricae_particulas_transitus (
    constans FabricaSutura*  sutura,
     constans FabricaActio*  actio,
         constans Sigillum*  artificium,
                   Piscina*  piscina,
                       Xar** particulae_out)
{
                  Xar* exclusa = fabricae_xar_chordarum(piscina);
                  Xar* particulae = xar_creare(piscina,
                      (i32)magnitudo(FabricaParticula));
               chorda  causa;
     FabricaParticula* p;
    SigillumContextus  contextus;
                  i32  i;

    causa = chorda_ex_literis("", piscina);
    si (exclusa == NIHIL || particulae == NIHIL)
    {
        redde FALSUM;
    }
    fabricae_chordam_addere(exclusa, chorda_ex_literis(
        "build/fabrica/provenientia/", piscina));
    si (!fabricae_particulas_colligere(sutura, actio, exclusa, piscina,
            particulae, &causa))
    {
        redde FALSUM;
    }
    xar_ordinare(particulae, fabricae_particulas_comparare);
    sigillum_incipere(&contextus);
    per (i = ZEPHYRUM; i < xar_numerus(actio->mandatum); i++)
    {
        chorda verbum = *(chorda*)xar_obtinere(actio->mandatum, i);

        sigillum_addere(&contextus, verbum.datum,
            (memoriae_index)verbum.mensura);
        sigillum_addere(&contextus, "", 1);
    }
    p = (FabricaParticula*)xar_addere(particulae);
    si (p != NIHIL)
    {
        p->via     = chorda_ex_literis("<mandatum>", piscina);
        p->octeti  = sigillum_finire(&contextus);
    }
    p = (FabricaParticula*)xar_addere(particulae);
    si (p != NIHIL)
    {
        p->via     = chorda_ex_literis("<verdictum>", piscina);
        p->octeti  = *artificium;
    }
    *particulae_out = particulae;
    redde VERUM;
}

/* particulae mutatae, additae, deletae inter transitum servatum et
 * hodiernum: viae (III, deinde '+N'); vacua = nulla */
interior chorda
_particulas_mutatas_nominare (
    constans Xar* vetus,
    constans Xar* nova,
         Piscina* piscina)
{
    TabulaDispersa* veteres = tabula_dispersa_creare_chorda(piscina,
        CCLVI);
    TabulaDispersa* hodiernae = tabula_dispersa_creare_chorda(piscina,
        CCLVI);
            chorda nuntius = chorda_ex_literis("", piscina);
               i32 numerus = ZEPHYRUM;
               i32 i;
               i32 pars;

    si (veteres == NIHIL || hodiernae == NIHIL)
    {
        redde nuntius;
    }
    per (i = ZEPHYRUM; i < xar_numerus(vetus); i++)
    {
        FabricaParticula* q = (FabricaParticula*)xar_obtinere(vetus, i);

        (vacuum)tabula_dispersa_inserere(veteres, q->via, q);
    }
    per (pars = ZEPHYRUM; pars < II; pars++)
    {
        constans Xar* haec = pars == ZEPHYRUM ? nova : vetus;

        per (i = ZEPHYRUM; i < xar_numerus(haec); i++)
        {
            constans FabricaParticula* q = (constans FabricaParticula*)
                xar_obtinere(haec, i);
                               vacuum* v = NIHIL;
                                  b32  mutata;

            si (pars == ZEPHYRUM)
            {
                (vacuum)tabula_dispersa_inserere(hodiernae, q->via,
                    NIHIL);
                mutata = !tabula_dispersa_invenire(veteres, q->via, &v)
                    || memcmp(((constans FabricaParticula*)v)->octeti
                        .octeti, q->octeti.octeti, SIGILLUM_OCTETI)
                        != ZEPHYRUM;
            }
            alioquin
            {
                mutata = !tabula_dispersa_continet(hodiernae, q->via);
            }
            si (!mutata)
            {
                perge;
            }
            si (numerus < III)
            {
                nuntius = fabricae_iungere(piscina, "", nuntius,
                    numerus > ZEPHYRUM ? ", " : "");
                nuntius = chorda_concatenare(nuntius, q->via, piscina);
            }
            numerus++;
        }
    }
    si (numerus > III)
    {
        nuntius = fabricae_iungere(piscina, "", nuntius, " +");
        nuntius = chorda_concatenare(nuntius, numerus_romanus_exprimere(
            (i64)(numerus - III), NIHIL, piscina), piscina);
    }
    redde nuntius;
}

/* QUIS TRANSITUM MUTAVIT (plan-5 T1): clavis transitus non inventa -
 * particulae servatae contra hodiernas (ingressus declaratus,
 * mandatum, verdictum), deinde lectiones vestigii ultimi; aliter
 * numquam servatum */
interior chorda
_transitum_mutatum_nominare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* artificium,
                   Piscina* piscina)
{
       Xar* vetus;
       Xar* nova;
       Xar* ultimae;
    chorda  nominata;

    si (   sutura->particulas_legere != NIHIL
        && sutura->particulas_legere(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina),
               chorda_ut_cstr(exitus->via, piscina), piscina, &vetus)
        && fabricae_particulas_transitus(sutura, actio, artificium,
        piscina,
               &nova))
    {
        nominata = _particulas_mutatas_nominare(vetus, nova, piscina);
        si (nominata.mensura > ZEPHYRUM)
        {
            redde fabricae_iungere(piscina, "ingressus mutatus: ",
                nominata,
                "");
        }
    }
    si (   sutura->lectiones_ultimae != NIHIL
        && sutura->lectiones_ultimae(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina), piscina,
               &ultimae))
    {
        nominata = _lectionem_differentem(sutura, ultimae, piscina);
        si (nominata.mensura > ZEPHYRUM)
        {
            redde fabricae_iungere(piscina, "lectio transitus mutata: ",
                nominata, "");
        }
    }
    redde chorda_ex_literis("nullum vestigium transitus (numquam "
        "servatum, aut causa non nominabilis)", piscina);
}

/* VERDICTUM (spec 3 par. XII): iudicium SOLUM, numquam currit - porta
 * in iudicio curreret. Plagula verdicti absens = STALUM; vestigium
 * transitus sub (titulus, exitus, clavis ingressuum, sigillum verdicti)
 * nullum = IGNOTUM; lectio differens = STALUM nominata; omnes
 * congruunt = RECENS. Defectus numquam servatur (vestigium solum post
 * transitum scribitur), ergo 'RECENS' = transitus idem. */
interior FabricaIudicium
_verdicto_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
    Sigillum  clavis;
    Sigillum  artificium;
      chorda  verdictum;
      chorda  differens;
         Xar* vestigium;

    (vacuum)plenus;
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), piscina, &verdictum))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
            chorda_ex_literis(
            "verdictum absens (transitus nullus)", piscina));
    }
    artificium  = sigillum_computare(verdictum.datum,
        (memoriae_index)verdictum.mensura);
    clavis      = fabricae_clavem_memoriae(actio, ingressus);
    si (   sutura->lectiones_legere == NIHIL
        || !sutura->lectiones_legere(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina),
               chorda_ut_cstr(exitus->via, piscina), &clavis,
               &artificium, piscina, &vestigium))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
            _transitum_mutatum_nominare(sutura, actio, exitus,
                &artificium, piscina));
    }
    differens = _lectionem_differentem(sutura, vestigium, piscina);
    si (differens.mensura > ZEPHYRUM)
    {
        redde fabricae_iudicium(exitus->via, FABRICA_STALUM,
            fabricae_iungere(piscina,
            "lectio transitus mutata: ", differens, ""));
    }
    redde fabricae_iudicium(exitus->via, FABRICA_RECENS,
        chorda_ex_literis(
        "transitus: lectiones congruunt (vestigium)", piscina));
}

/* IGNOTA: numquam iudicatur (machina eam non vocat; vocata tamen
 * IGNOTUM honestum reddit) - praecondicio sola (spec 1b par. II.3) */
interior FabricaIudicium
_ignote_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
    (vacuum)sutura;
    (vacuum)actio;
    (vacuum)ingressus;
    (vacuum)plenus;
    redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM,
        chorda_ex_literis(
        "ignota: praecondicio, numquam iudicatur", piscina));
}

/* ordo membrorum: titulus, genus_ordinarium, octetis_comparat,
 * iudicatur, iudicare */
interior constans FabricaStrategia _strategia_regeneratio = {
    "regeneratio", "fasciculus", VERUM, VERUM, _regeneratione_iudicare
};

interior constans FabricaStrategia _strategia_relatio = {
    "relatio", "binarium", FALSUM, VERUM, _relatione_iudicare
};

interior constans FabricaStrategia _strategia_ignota = {
    "ignota", "fasciculus", FALSUM, FALSUM, _ignote_iudicare
};

/* verdictum: octetis non comparat (regeneratio = porta tota; '-tacta'
 * eum numquam iudicat), iudicatur (per vestigium solum) */
interior constans FabricaStrategia _strategia_verdictum = {
    "verdictum", "fasciculus", FALSUM, VERUM, _verdicto_iudicare
};

interior constans FabricaStrategia* constans _strategiae[] = {
    &_strategia_regeneratio,
    &_strategia_relatio,
    &_strategia_ignota,
    &_strategia_verdictum
};

constans FabricaStrategia*
fabrica_strategia_invenire (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_strategiae) / magnitudo(_strategiae[0]));
         i++)
    {
        si (chorda_aequalis_literis(titulus, _strategiae[i]->titulus))
        {
            redde _strategiae[i];
        }
    }
    redde NIHIL;
}

FabricaIudicium
fabrica_iudicare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
     constans FabricaExitus* exitus,
                        b32  plenus,
                    Piscina* piscina)
{
    Sigillum ingressus;
      chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (!fabricae_actionem_sigillare_semel(sutura, actio, piscina,
        &ingressus,
            &causa))
    {
        redde fabricae_iudicium(exitus->via, FABRICA_IGNOTUM, causa);
    }
    redde exitus->strategia->iudicare(sutura, actio, exitus, &ingressus,
        plenus, piscina);
}
