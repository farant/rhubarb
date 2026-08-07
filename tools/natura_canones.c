/* tools/natura_canones.c - natura -> canon (proiectio pura)
 *
 * Genus quodque in elementum canonis proicitur. Vide
 * project-specs/natura-canones-generati-spec.md.
 *
 * PROIECTIO PURA: nihil hic manu emendatur; si generator
 * distinctionem poscit quam natura non fert, natura eam accipit
 * SOLUM si vere ontologica est (spec par. 3.3).
 *
 * Usus:
 *   bin/natura_canones -index          index entium (TSV)
 *   bin/natura_canones -inspicere G    exemplar elementi unius (TSV)
 *   bin/natura_canones -radix DIR      alia sedes exemplarium
 *
 * Exitus: 0 = sanum; 2 = NIHIL CURSUM EST (disciplina domus:
 *   nihil onerari NON est successus).
 */
#include "natura_canones.h"
#include "filum.h"
#include "iter_directoria.h"
#include <string.h>

#define EXTENSIO ".genera"

/* custos catenae - mos natura_apparatus (catena circularis
 * oneratorem transire non debet, sed hic eam non iudicamus) */
#define NC_CATENA_MAXIMA XXXII

/* Praefatio monolithi - in canonem ipsum scripta.
 *
 * LECTOR SCIRE DEBET monolithum canones per modulum NON vincere.
 * Fortior est in ALIQUIBUS relationibus, INFIRMIOR in actionibus,
 * et qui contrarium sumit canonem laxiorem pro strictiore
 * adhibebit.
 *
 * NUMERI VIVI, non fixi: quot relationes vere examinentur ex
 * corpore hodierno computatur et hic scribitur. Numerus in
 * commentario fixus eo ipso momento mentiretur quo quis exemplar
 * addit - et 'citationes veras habet' sine numero lectorem
 * omnes examinari credere sinit.
 *
 * (Nomina elementorum SINE uncis scribuntur consulto: uncus in
 * commentario instrumenta quae per lineas quaerunt fallit -
 * numerus citationum mea ipsa prosa iam semel corruptus est.) */
#define NC_PRAEFATIO_FORMA \
"  MONOLITHUS: genera omnia corporis unum canonem faciunt, ergo\n" \
"  documentum unum genera libere miscet - plantam, personam quae\n" \
"  eam nominavit, opus ubi nomen editum est.\n" \
"\n" \
"  QUID HIC EXAMINETUR, ET QUID NON:\n" \
"\n" \
"  RELATIONES - ex %u sedibus relationum huius canonis, %u\n" \
"  citationes VERAE fiunt (clavis intra documentum resoluta).\n" \
"  Petitum cum posteris per CLAUSURAM citatur: subgenera resque\n" \
"  eius omnes in indicem ad= enumerantur, quia claves per\n" \
"  titulum EXACTUM colliguntur et subgenus titulum alium fert.\n" \
"  %u nomine solo iudicantur, id est sicut in canone per\n" \
"  modulum. Causae, quae omnes limites mechanismi sunt, non\n" \
"  neglegentiae:\n" \
"    %u petitum apertum habent (ad='*') - clavis nulla est;\n" \
"    %u multiplices sunt - super= titulo solo congruit nec\n" \
"      intra= observat, et nomen liberi per genera communicatur\n" \
"      quorum petita diversa sunt;\n" \
"    %u petitum non resolvunt.\n" \
"  ERGO relationes hic FORTIUS iudicantur quam in canone per\n" \
"  modulum, sed NON OMNES - reliquae aeque laxae sunt.\n" \
"\n" \
"  ACTIONES - INFIRMIUS quam in canone per modulum. Electio\n" \
"  eventum/actio hic actiones TOTIUS CORPORIS unit, non unius\n" \
"  moduli: canon intra= ad parentem PROXIMUM solum ligat, et\n" \
"  parens eventi semper 'historia' est, quae per genera omnia\n" \
"  communis est - unde actio cuiuslibet moduli ubique accipitur.\n" \
"  Canon per modulum eandem electionem ad modulum suum angustat.\n" \
"\n" \
"  IDENTITAS - unicitas trans genera omnia: spatium nominum UNUM.\n" \
"\n" \
"  SUMMA: NEUTER CANON ALTERUM VINCIT. Qui relationes examinari\n" \
"  vult hunc sumat; qui actiones arte astringi vult canonem\n" \
"  moduli sui."

/* receptaculum praefationis - forma ipsa ~1600 octetos fert,
 * numeri sex ~40 addunt. RECUSATIO potius quam truncatio: canon
 * cuius praefatio dimidia est de se ipso mentitur. */
#define NC_PRAEFATIO_MAXIMA MMMM

/* genus valoris quod canon non habet - ad textum cadit, sed
 * NUMERATUM: degradatio tacita vitium domus est */
nomen structura {
    chorda*  genus;      /* nomen generis ut natura id dicit */
        i32  numerus;    /* quot declarationes in corpore */
} NcGenusIgnotum;

/* cur relatio citatio fiat aut non fiat */
nomen enumeratio {
    NC_CIT_FIT       = I,   /* petitum resolutum - citatio vera,
                             * clausura posterorum enumerata */
    NC_CIT_APERTA    = II,  /* ad="*" aut absens - clavis nulla */
    NC_CIT_MULTIPLEX = III, /* super= adstringi nequit */
    NC_CIT_IGNOTA    = IV   /* petitum non resolvitur */
} NcCitatioStatus;

/* census relationum - NUMERI IN CANONEM IPSUM SCRIBUNTUR.
 *
 * Non hic in commentario fixi: corpus crescit, et numerus fixus
 * mentiretur eo ipso momento quo quis exemplar addit. Canon
 * generatus de SE dicat quantum vere examinet.
 *
 * SEDES numerantur (membra huius canonis), non declarationes
 * generum: res apparatum generis sui hereditant, unde numerus
 * sedium maior est. Sedes autem sunt quae in canone apparent. */
nomen structura {
    i32  omnes;
    i32  citatae;
    i32  apertae;
    i32  multiplices;
    i32  ignotae;
} NcCensusRelationum;

interior b32       _extensionem_habet(constans chorda* t);
interior vacuum    _stirpem_scribere(constans chorda* t, character* ex,
                                     i32 tectum);
interior s32       _comparare_titulos(constans vacuum* a,
                                      constans vacuum* b);
interior b32       _corpus_onerare(NaturaBibliotheca* bib,
                                   constans character* radix,
                                   Piscina* piscina,
                                   i32* vulnera_ex);
interior Xar*      _entia_colligere(NaturaBibliotheca* bib,
                                    Piscina* piscina);
interior NcEns*    _ens_invenire(Xar* entia,
                                 constans character* titulus);
interior NcMembrum* _membrum_invenire(Xar* membra,
                                      constans chorda* titulus,
                                      constans character* praefixum);
interior b32       _membrum_adest(Xar* membra, constans chorda* titulus,
                                  constans character* praefixum);
interior b32       _actio_adest(Xar* actiones, constans chorda* titulus);
interior b32       _genus_valoris_notum(constans chorda* g);
interior vacuum    _ignotum_numerare(Xar* ignota, chorda* g);
interior vacuum    _genera_ignota_generis(Xar* ignota, NaturaGenus* g);
interior vacuum    _genera_ignota_nuntiare(NaturaBibliotheca* bib,
                                           Piscina* piscina);
interior Xar*      _optiones_colligere(StmlNodus* n, Piscina* piscina);
interior Xar*      _status_colligere(StmlNodus* n, Piscina* piscina);
interior b32       _machina_ad_individuum(StmlNodus* n);
interior constans character* _praefixum_machinae(
                                 constans chorda* titulus);
interior vacuum    _valores_ex_nodo(NcElementum* el, StmlNodus* n,
                                    Piscina* piscina);
interior vacuum    _valores_applicare(NcElementum* el, NcEns* ens,
                                      Piscina* piscina);
interior constans character* _cstr_tutum(constans chorda* c,
                                     constans character* unde,
                                     Piscina* piscina);
interior vacuum    _titulum_semel_addere(Xar* tituli, chorda* t);
interior vacuum    _genera_clausurae_colligere(NaturaGenus* g,
                                               Xar* genera);
interior Xar*      _clausuram_colligere(NaturaBibliotheca* bib,
                                        NaturaGenus* g,
                                        Piscina* piscina);
interior NcCitatioStatus _petitum_citabile(NaturaBibliotheca* bib,
                                     StmlNodus* nodus, b32 multiplex,
                                     Xar** clausura_ex,
                                     Piscina* piscina);
interior vacuum    _censum_notare(NcCensusRelationum* census,
                                  NcCitatioStatus status);
interior vacuum    _praefationem_scribere(character* ex, i32 tectum,
                                  constans NcCensusRelationum* census);
interior vacuum    _apparatum_plicare(NaturaBibliotheca* bib,
                                      NcElementum* el, Xar* apparatus,
                                      b32 monolithus,
                                      NcCensusRelationum* census,
                                      Piscina* piscina);
interior NcElementum* _elementum_aedificare(NaturaBibliotheca* bib,
                                            NcEns* ens,
                                            b32 monolithus,
                                            NcCensusRelationum* census,
                                            Piscina* piscina);
interior chorda    _kebab_chorda(constans chorda* t, Piscina* piscina);
interior i32       _nomina_gemina_numerare(Xar* entia,
                                           Piscina* piscina);
interior b32       _canonem_modulo_scribere(NaturaBibliotheca* bib,
                                            Xar* entia,
                                            constans character* modulus,
                                            constans character* via,
                                            Piscina* piscina);
interior b32       _canonem_totum_scribere(NaturaBibliotheca* bib,
                                           Xar* entia,
                                           constans character* via,
                                           Piscina* piscina);

/* an titulus in ".genera" desinat - stirps VACUA non sufficit,
 * ergo aequalitas quoque reicitur (mos natura_examen) */
interior b32
_extensionem_habet(
    constans chorda*  t)
{
    i32 mensura_ext;

    mensura_ext = (i32)strlen(EXTENSIO);
    si (t->mensura <= mensura_ext)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(t->datum + (t->mensura - mensura_ext),
                       EXTENSIO, (memoriae_index)mensura_ext) ==
                ZEPHYRUM);
}

/* 'mensura.genera' -> 'mensura'
 *
 * PRAECONDICIO INTRA MUNUS AFFIRMATA: i32 INSIGNATUS est, ergo
 * titulus extensione BREVIOR subtractionem circumvolveret, n ad
 * tectum - I clamparetur, et memcpy CCLV octetos ex chorda pauciora
 * ferente legeret - lectio ULTRA FINES, non ramus mortuus.
 * _extensionem_habet id hodie praestat, sed custos et usus in
 * ansis DIVERSIS nunc stant, et munus quod praecondicionem suam
 * ipsum non tenet eam amittit quo momento vocans alius nascitur. */
interior vacuum
_stirpem_scribere(
    constans chorda*  t,
    character*        ex,
    i32               tectum)
{
    i32 mensura_ext;
    i32 n;

    mensura_ext = (i32)strlen(EXTENSIO);
    si (tectum == ZEPHYRUM)
    {
        redde;
    }
    si (t->mensura <= mensura_ext)
    {
        fprintf(stderr,
            "natura_canones: titulus '%.*s' extensionem '%s' non "
            "fert - stirps vacua\n",
            (integer)t->mensura, (constans character*)t->datum,
            EXTENSIO);
        ex[ZEPHYRUM] = '\0';
        redde;
    }

    n = t->mensura - mensura_ext;
    si (n >= tectum)
    {
        n = tectum - I;
    }
    memcpy(ex, t->datum, (memoriae_index)n);
    ex[n] = '\0';
}

/* ordo alphabeticus titulorum - ordinem directorii (qui ordo
 * systematis filorum est) in ordinem STABILEM vertit */
interior s32
_comparare_titulos(
    constans vacuum*  a,
    constans vacuum*  b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

/* exemplaria OMNIA onerare - sub= fines modulorum transit,
 * ergo unum onerare non sufficit (mos natura_examen).
 *
 * ORDO: tituli PRIMUM colliguntur et ORDINANTUR, deinde onerantur.
 * Ordo directorii ordo systematis filorum est, non ordo stabilis:
 * canon inde generatus ordinem elementorum mutaret quotiens
 * exemplar additur aut machina mutatur, unde porta rancoris falso
 * clamaret - et porta quae falso clamat neglegitur. Introitus
 * praeterea usque ad vocationem proximam SOLUM valet (vide
 * iter_directoria.h), ergo titulus transcribendus est.
 *
 * CORPUS INCOMPLETUM proiectionem abicit: exemplar quod parsari
 * aut legi nequit corpus minus tacite relinqueret, id est ipsa
 * condicio quam numeri CLXXVII/CCCLXXXIV deprehendere debent.
 * Vulnera nexurae ALIUD sunt - ea corpus AEGRUM signant, non
 * MINUS, ergo reddita sunt ut vocans ea nuntiet et pergat. */
interior b32
_corpus_onerare(
    NaturaBibliotheca*   bib,
    constans character*  radix,
    Piscina*             piscina,
    i32*                 vulnera_ex)
{
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    Xar*                  tituli;
    character             via[DXII];
    character             stirps[CCLVI];
    i32                   i;
    i32                   onerata;
    i32                   fracta;

    *vulnera_ex = ZEPHYRUM;

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_canones: '%s' aperiri nequit\n", radix);
        redde FALSUM;
    }

    tituli = xar_creare(piscina, (i32)magnitudo(chorda));
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        chorda* locus;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        si ((memoriae_index)introitus->titulus.mensura +
            strlen(radix) + II > magnitudo(via))
        {
            perge;
        }

        locus  = (chorda*)xar_addere(tituli);
        *locus = chorda_transcribere(introitus->titulus, piscina);
    }
    directorium_iterator_claudere(iter);

    xar_ordinare(tituli, _comparare_titulos);

    onerata = ZEPHYRUM;
    fracta  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(tituli); i++)
    {
        constans chorda* titulus;
        chorda           fons;

        titulus = (constans chorda*)xar_obtinere(tituli, i);

        sprintf(via, "%s/%.*s", radix, (integer)titulus->mensura,
                (constans character*)titulus->datum);
        _stirpem_scribere(titulus, stirps, (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_canones: '%s' legi nequit\n", via);
            fracta++;
            perge;
        }
        si (!natura_legere(bib, fons, stirps))
        {
            fprintf(stderr, "natura_canones: '%s' parsari nequit\n",
                    via);
            fracta++;
            perge;
        }
        onerata++;
    }

    si (fracta > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: exemplaria %u NON onerata - corpus "
            "INCOMPLETUM, proiectio abicitur\n", fracta);
        redde FALSUM;
    }

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: NULLUM exemplar in '%s' (extensio '%s')\n",
            radix, EXTENSIO);
        redde FALSUM;
    }

    *vulnera_ex = natura_nectere(bib);
    redde VERUM;
}

/* genera et res dictionarii in indicem unum - discrimen relativum
 * est (spec par. 3.2), ergo ambo elementa fiunt. */
interior Xar*
_entia_colligere(
    NaturaBibliotheca*  bib,
    Piscina*            piscina)
{
    Xar* entia;
    i32  i;

    entia = xar_creare(piscina, (i32)magnitudo(NcEns));

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        NcEns*       e;

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus     = g->titulus;
        e->modulus     = g->modulus;
        e->genus       = g;
        e->genus_etiam = NIHIL;   /* etiam= rerum solum est */
        e->nodus       = g->nodus;
        e->est_res     = FALSUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;
        NcEns*     e;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus     = r->titulus;
        e->modulus     = r->modulus;
        e->genus       = r->genus_suum;
        e->genus_etiam = r->genus_etiam;
        e->nodus       = r->nodus;
        e->est_res     = VERUM;
    }

    redde entia;
}

/* ens nomine nudo - modus inspiciendi hominis est, ergo primum
 * congruens sufficit (nomina generum per corpus unica sunt; res
 * homonymae theoretice possibiles, nondum factae) */
interior NcEns*
_ens_invenire(
    Xar*                 entia,
    constans character*  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(entia); i++)
    {
        NcEns* e;

        e = (NcEns*)xar_obtinere(entia, i);
        si (chorda_aequalis_literis(*e->titulus, titulus))
        {
            redde e;
        }
    }
    redde NIHIL;
}

/* membrum iam positum? (apparatus NON deduplicat: primum
 * occurrens = maxime proprium, ergo primum vincit) */
interior NcMembrum*
_membrum_invenire(
    Xar*                 membra,
    constans chorda*     titulus,
    constans character*  praefixum)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(membra, i);
        si (!chorda_aequalis(*m->titulus, *titulus))
        {
            perge;
        }
        si ((m->praefixum == NIHIL) != (praefixum == NIHIL))
        {
            perge;
        }
        si (m->praefixum &&
            strcmp(m->praefixum, praefixum) != ZEPHYRUM)
        {
            perge;
        }
        redde m;
    }
    redde NIHIL;
}

interior b32
_membrum_adest(
    Xar*                 membra,
    constans chorda*     titulus,
    constans character*  praefixum)
{
    redde (b32)(_membrum_invenire(membra, titulus, praefixum) != NIHIL);
}

/* Catena una actionem bis non fert (natura_apparatus maiores semel
 * ascendit), sed catenae DUAE rei etiam= maiorem communem habere
 * possunt: scriptum_conchae sub scripto_exsecutabili stat et
 * plagulam_fontis etiam nominat, quae AMBAE sub plagula_computatrali
 * sunt - unde actiones eius bis venirent. MENSURATUM, non
 * praevisum: exemplar quinque actiones geminas ostendit. */
interior b32
_actio_adest(
    Xar*              actiones,
    constans chorda*  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        si (chorda_aequalis(**(chorda**)xar_obtinere(actiones, i),
                            *titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* an canon hoc genus valoris ferre possit. Vocabularium canonis
 * (lib/canon.c genus_legere): nomen | numerus | veritas | dies |
 * electio | compositum | textus. Natura horum quinque sola in
 * <proprietas genus=> dicit; quicquid aliud dicit ad textum cadit,
 * quia canon aliud non habet. */
interior b32
_genus_valoris_notum(
    constans chorda*  g)
{
    redde (b32)(chorda_aequalis_literis(*g, "textus")  ||
                chorda_aequalis_literis(*g, "electio") ||
                chorda_aequalis_literis(*g, "numerus") ||
                chorda_aequalis_literis(*g, "veritas") ||
                chorda_aequalis_literis(*g, "dies"));
}

interior vacuum
_ignotum_numerare(
    Xar*     ignota,
    chorda*  g)
{
    NcGenusIgnotum* e;
    i32             i;

    per (i = ZEPHYRUM; i < xar_numerus(ignota); i++)
    {
        e = (NcGenusIgnotum*)xar_obtinere(ignota, i);
        si (chorda_aequalis(*e->genus, *g))
        {
            e->numerus++;
            redde;
        }
    }
    e          = (NcGenusIgnotum*)xar_addere(ignota);
    e->genus   = g;
    e->numerus = I;
}

/* proprietates generis unius (SUAS, non hereditatas) numerare */
interior vacuum
_genera_ignota_generis(
    Xar*          ignota,
    NaturaGenus*  g)
{
    i32 i;
    i32 numerus;

    numerus = stml_numerus_liberorum(g->nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* continens;
        i32        j;
        i32        n_intus;

        continens = stml_liberum_ad_indicem(g->nodus, i);
        si (!continens || continens->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*continens->titulus,
                                     "proprietates"))
        {
            perge;
        }

        n_intus = stml_numerus_liberorum(continens);
        per (j = ZEPHYRUM; j < n_intus; j++)
        {
            StmlNodus* p;
            chorda*    gv;

            p = stml_liberum_ad_indicem(continens, j);
            si (!p || p->genus != STML_NODUS_ELEMENTUM ||
                !chorda_aequalis_literis(*p->titulus, "proprietas"))
            {
                perge;
            }
            gv = stml_attributum_capere(p, "genus");
            si (gv && !_genus_valoris_notum(gv))
            {
                _ignotum_numerare(ignota, gv);
            }
        }
    }
}

/* DECLARATIONES numerantur, non membra plicata: hereditas idem
 * genus per posteros centies ferret, et numerus inflatus quaestioni
 * vocabularii (spec par. 8) mentiretur.
 *
 * Nuntius modo NON pendet - corpus quod genera ignota fert ea fert
 * sive index sive elementum unum petitur. */
interior vacuum
_genera_ignota_nuntiare(
    NaturaBibliotheca*  bib,
    Piscina*            piscina)
{
    Xar* ignota;
    i32  i;

    ignota = xar_creare(piscina, (i32)magnitudo(NcGenusIgnotum));

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        _genera_ignota_generis(
            ignota, *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i));
    }

    per (i = ZEPHYRUM; i < xar_numerus(ignota); i++)
    {
        NcGenusIgnotum* e;

        e = (NcGenusIgnotum*)xar_obtinere(ignota, i);
        fprintf(stderr,
            "natura_canones: genus proprietatis '%.*s' extra canonem "
            "- textus adhibetur (%u instantiae)\n",
            (integer)e->genus->mensura,
            (constans character*)e->genus->datum,
            e->numerus);
    }
}

/* liberos <optio> in Xar de chorda* colligere */
interior Xar*
_optiones_colligere(
    StmlNodus*  n,
    Piscina*    piscina)
{
    Xar* optiones;
    i32  i;
    i32  numerus;

    optiones = xar_creare(piscina, (i32)magnitudo(chorda*));
    numerus  = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(n, i);
        si (l && l->genus == STML_NODUS_ELEMENTUM &&
            chorda_aequalis_literis(*l->titulus, "optio"))
        {
            chorda*  valor;
            chorda** locus;

            valor  = (chorda*)piscina_allocare(piscina,
                                               magnitudo(chorda));
            *valor = stml_textus_normalizatus(l, piscina);
            locus  = (chorda**)xar_addere(optiones);
            *locus = valor;
        }
    }
    redde optiones;
}

/* idem, sed nomina <status> - electio machinae ex nominibus
 * statuum fit, non ex textu */
interior Xar*
_status_colligere(
    StmlNodus*  n,
    Piscina*    piscina)
{
    Xar* nomina;
    i32  i;
    i32  numerus;

    nomina  = xar_creare(piscina, (i32)magnitudo(chorda*));
    numerus = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(n, i);
        si (l && l->genus == STML_NODUS_ELEMENTUM &&
            chorda_aequalis_literis(*l->titulus, "status"))
        {
            chorda*  titulus;
            chorda** locus;

            titulus = stml_attributum_capere(l, "nomen");
            si (!titulus)
            {
                perge;
            }
            locus  = (chorda**)xar_addere(nomina);
            *locus = titulus;
        }
    }
    redde nomina;
}

/* an machina statuum ad INDIVIDUUM pertineat (spec par. 4.1).
 * gerens= absens: natura praestitutum non dicit - vide spec
 * par. 8.1. Interim 'individuum' sumimus, quod casus usitatus est. */
interior b32
_machina_ad_individuum(
    StmlNodus*  n)
{
    chorda* gerens;

    gerens = stml_attributum_capere(n, "gerens");
    si (!gerens)
    {
        redde VERUM;
    }
    redde chorda_aequalis_literis(*gerens, "individuum");
}

/* Praefixum 'status_' machinae, NISI nomen machinae stirpem
 * 'status' iam fert.
 *
 * DECRETUM (Fran), non casus specialis furtim insertus: haec
 * condicio transformationi puritatem sine exceptione aufert, quae
 * ipsa erat ratio contra eam. Vicit tamen, quia canon GENERATUS hoc
 * solo valet quod homo nomen attributi ex exemplari praevidere
 * potest - et 'status-status-civilis' nomen est quod nemo
 * coniceret. Duae viae aliae reiectae: nomen turpe mechanice
 * ferre, aut machinam in natura renominare (natura recte dicit
 * quod dicit; generator cedat, non exemplar).
 *
 *   planta  machina 'vita'           -> status-vita
 *   persona machina 'status_civilis' -> status-civilis
 *   coctura machina 'status'         -> status
 *
 * STIRPS quaeritur, non praefixum plenum: machina simpliciter
 * 'status' nominata lineolam sequentem non fert, et balbutiem
 * eandem ('status-status') pareret quam decretum tollere iubet.
 *
 * Duae chordae distinctae CONSULTO: 'status' quaeritur, 'status_'
 * redditur. Si una eademque essent, machina 'vita' attributum
 * 'statusvita' pareret.
 *
 * Praefixum reddito UTENDUM est et in inquisitione duplicationis
 * et in membro ipso: si discreparent, machina sub uno nomine
 * quaereretur et sub alio poneretur. */
interior constans character*
_praefixum_machinae(
    constans chorda*  titulus)
{
    constans character* praefixum;
    constans character* stirps;
    i32                 mensura;

    praefixum = "status_";
    stirps    = "status";
    mensura   = (i32)strlen(stirps);

    si (titulus->mensura >= mensura &&
        memcmp(titulus->datum, stirps,
               (memoriae_index)mensura) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde praefixum;
}

/* liberos <valor> nodi unius ad membra applicare.
 * Membrum iam praestitutum SERVATUR: vocans ab origine maxime
 * propria ascendit, ergo primum scriptum vincit. */
interior vacuum
_valores_ex_nodo(
    NcElementum*  el,
    StmlNodus*    n,
    Piscina*      piscina)
{
    i32 i;
    i32 numerus;

    numerus = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus*  l;
        chorda*     titulus;
        chorda*     valor;
        NcMembrum*  m;

        l = stml_liberum_ad_indicem(n, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "valor"))
        {
            perge;
        }
        titulus = stml_attributum_capere(l, "nomen");
        si (!titulus)
        {
            perge;
        }
        m = _membrum_invenire(el->membra, titulus, NIHIL);
        si (!m || m->praestitutum)
        {
            perge;
        }
        valor  = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        *valor = stml_textus_normalizatus(l, piscina);

        m->praestitutum = valor;
    }
}

/* elementum <valor> ad quemlibet gradum stat (spec par. 4):
 * rosa 'habitus frutex' dicit, unde rosa_canina eum fert.
 * Utraque lectio valoris (constringens et typificans, par. 8.2) ad
 * ordinarius= tuto cadit, ergo hereditas eius secura est.
 *
 * ORDO: proprium, deinde res continentes, deinde catena generum -
 * maxime proprium primum, quia primum scriptum vincit.
 * Continentia rei specificatio est (cultivar in specie, species
 * in genere), ergo valores eius descendunt sicut generum. */
interior vacuum
_valores_applicare(
    NcElementum*  el,
    NcEns*        ens,
    Piscina*      piscina)
{
    NaturaGenus* g;
    StmlNodus*   n;
    i32          gradus;

    si (ens->est_res)
    {
        gradus = ZEPHYRUM;
        per (n = ens->nodus; n && gradus < NC_CATENA_MAXIMA;
             n = n->parens)
        {
            si (n->genus != STML_NODUS_ELEMENTUM)
            {
                frange;
            }
            si (n != ens->nodus &&
                !chorda_aequalis_literis(*n->titulus, "species") &&
                !chorda_aequalis_literis(*n->titulus, "individuum") &&
                !chorda_aequalis_literis(*n->titulus, "cultivar"))
            {
                frange;
            }
            _valores_ex_nodo(el, n, piscina);
            gradus++;
        }
    }

    gradus = ZEPHYRUM;
    per (g = ens->genus; g && gradus < NC_CATENA_MAXIMA;
         g = g->parens)
    {
        _valores_ex_nodo(el, g->nodus, piscina);
        gradus++;
    }

    /* catena etiam= POST suam, eodem ordine quo apparatus plicatus
     * est: aliter membrum ex illa hereditatum praestitutum suum
     * amitteret quamquam natura id dicit */
    gradus = ZEPHYRUM;
    per (g = ens->genus_etiam; g && gradus < NC_CATENA_MAXIMA;
         g = g->parens)
    {
        _valores_ex_nodo(el, g->nodus, piscina);
        gradus++;
    }
}

/* chorda -> littera C, RECUSANS si octetum nullum fert.
 *
 * PORTA UNICA CONSULTO, non custodia per sedem: chorda mensuram
 * fert, non terminatorem, ergo octetum nullum ferre POTEST -
 * chorda_ut_cstr eum fideliter transcribit et terminatorem
 * addit, sed quaesitor qui litteras C comparat ad eum desinit.
 * Effectus non est defectio sed DEVIATIO TACITA: 'folium\0zzz'
 * genus 'folium' invenit, et citatio in genus oritur quod auctor
 * numquam nominavit.
 *
 * Classis eadem in semita valoris clausa est (72149cc,
 * NC_VALOR_NULLUS); hic in semita QUAESTIONIS reaperta erat.
 * Ideo porta hic stat et non ad quinque sedes dispersa: sedes
 * proxima quae chordam in litteras vertit hanc protectionem
 * HEREDITET, non iterum mereatur.
 *
 * NIHIL redditum = recusatio; vocans eam ut irresolubilem
 * tractet (quod VERA est: nomen tale in corpore non est). */
interior constans character*
_cstr_tutum(
    constans chorda*     c,
    constans character*  unde,
    Piscina*             piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < c->mensura; i++)
    {
        si ((character)c->datum[i] == '\0')
        {
            fprintf(stderr,
                "natura_canones: '%s' octetum nullum fert - "
                "quaestio prooemium solum iudicaret et citationem "
                "in genus alienum dirigeret; RECUSATUR\n", unde);
            redde NIHIL;
        }
    }
    redde chorda_ut_cstr(*c, piscina);
}

/* titulum semel addere - clausurae parvae sunt (maxima corporis
 * sub XX titulis), inquisitio linearis sufficit */
interior vacuum
_titulum_semel_addere(
    Xar*     tituli,
    chorda*  t)
{
    i32      i;
    chorda** locus;

    per (i = ZEPHYRUM; i < xar_numerus(tituli); i++)
    {
        si (chorda_aequalis(**(chorda**)xar_obtinere(tituli, i),
                            *t))
        {
            redde;
        }
    }
    locus  = (chorda**)xar_addere(tituli);
    *locus = t;
}

/* genera clausurae: petitum ipsum et subgenera transitive */
interior vacuum
_genera_clausurae_colligere(
    NaturaGenus*  g,
    Xar*          genera)
{
    NaturaGenus** locus;
    i32           i;

    locus  = (NaturaGenus**)xar_addere(genera);
    *locus = g;
    per (i = ZEPHYRUM; i < xar_numerus(g->liberi); i++)
    {
        _genera_clausurae_colligere(
            *(NaturaGenus**)xar_obtinere(g->liberi, i), genera);
    }
}

/* clausura petiti: tituli omnium entium quae VERE petitum sunt -
 * genus ipsum, subgenera transitive, res eorum nidificatae,
 * resque quae etiam= in quodvis eorum ferunt.
 *
 * ETIAM= QUOQUE, non sola nidificatio: res quae etiam="G" fert
 * VERE G est (natura.h: 'membrum essentiale duplex'), sed
 * lib/natura.c eam in G->res_suae NON ponit - ille index
 * nidificatione sola impletur. Sine hac inquisitione talis res
 * extra indicem staret, id est documentum rectum reiceretur quod
 * eam nominat - idem vitium quod regulam foliorum priorem semel
 * momordit, forma minore rediens. */
interior Xar*
_clausuram_colligere(
    NaturaBibliotheca*  bib,
    NaturaGenus*        g,
    Piscina*            piscina)
{
    Xar* genera;
    Xar* tituli;
    i32  i;
    i32  j;

    genera = xar_creare(piscina, (i32)magnitudo(NaturaGenus*));
    tituli = xar_creare(piscina, (i32)magnitudo(chorda*));
    _genera_clausurae_colligere(g, genera);

    per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
    {
        NaturaGenus* g2;

        g2 = *(NaturaGenus**)xar_obtinere(genera, i);
        _titulum_semel_addere(tituli, g2->titulus);
        per (j = ZEPHYRUM; j < xar_numerus(g2->res_suae); j++)
        {
            _titulum_semel_addere(tituli,
                (*(NaturaRes**)xar_obtinere(g2->res_suae,
                                            j))->titulus);
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        si (!r->genus_etiam)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(genera); j++)
        {
            si (r->genus_etiam ==
                *(NaturaGenus**)xar_obtinere(genera, j))
            {
                _titulum_semel_addere(tituli, r->titulus);
                frange;
            }
        }
    }

    redde tituli;
}

/* Clausura entium quae relatio petit, SI citari potest - aliter
 * status causam recusationis dicit et *clausura_ex NIHIL manet.
 *
 * CUR CLAUSURA ENUMERATUR (decretum alterum huius operis; prius
 * folium poscebat et sedes DCCCIV tunc mensuratas sine porta
 * relinquebat): citatio claves ex nodis titulo EXACTO
 * congruentibus colligit (lib/canon.c), et subgenus titulum
 * ALIUM fert - relatio ad 'persona' titulo solo citata
 * documentum RECTUM reiceret quod carl-linnaeus nomen="lin"
 * ponit et 'lin' citat. Subsumptio ergo hic COMPILATUR: petitum
 * cum posteris omnibus in indicem ad= it, ut clavis sub quovis
 * eorum titulorum colligatur - canon hereditatem numquam discit,
 * sicut nec plicaturam sub= didicit. Posterus novus additus
 * regeneratione in indicem venit; porta staleness catenam iam
 * custodit.
 *
 * Res dictionarii (carl_linnaeus) posteros in grapho ferre non
 * potest: clausura eius titulus unus est. */
interior NcCitatioStatus
_petitum_citabile(
    NaturaBibliotheca*  bib,
    StmlNodus*          nodus,
    b32                 multiplex,
    Xar**               clausura_ex,
    Piscina*            piscina)
{
    chorda*             ad;
    chorda*             modulus;
    constans character* ad_cstr;
    constans character* modulus_cstr;
    NaturaGenus*        g;

    *clausura_ex = NIHIL;

    /* MULTIPLEX PRIMUM: exclusio categorica est, petito
     * qualicumque. Citans est LIBERUM relationis, et super=
     * titulo nodi SOLO congruit nec intra= observat - nomen
     * autem liberi per genera communicatur, quorum petita
     * diversa sunt ('continet' septem habet). Citatio una ergo
     * ceteris falso clamaret. */
    si (multiplex)
    {
        redde NC_CIT_MULTIPLEX;
    }

    ad = stml_attributum_capere(nodus, "ad");
    si (!ad || chorda_aequalis_literis(*ad, "*"))
    {
        redde NC_CIT_APERTA;
    }
    modulus = stml_attributum_capere(nodus, "modulus");

    /* CONVERSIO SEMEL, ad portam: quaestiones infra litteras C
     * comparant, ergo quicquid eas ingreditur per _cstr_tutum
     * transire debet. Recusatum irresolubile est - quod VERUM
     * est, quia nomen octetum nullum ferens in corpore non est. */
    ad_cstr = _cstr_tutum(ad, "relatio ad=", piscina);
    si (!ad_cstr)
    {
        redde NC_CIT_IGNOTA;
    }
    modulus_cstr = NIHIL;
    si (modulus)
    {
        modulus_cstr = _cstr_tutum(modulus, "relatio modulus=",
                                   piscina);
        si (!modulus_cstr)
        {
            redde NC_CIT_IGNOTA;
        }
    }

    si (modulus_cstr)
    {
        g = natura_genus_in(bib, modulus_cstr, ad_cstr);
    }
    alioquin
    {
        g = natura_genus(bib, ad_cstr);
    }
    si (g)
    {
        *clausura_ex = _clausuram_colligere(bib, g, piscina);
        redde NC_CIT_FIT;
    }

    si (modulus_cstr)
    {
        NaturaEns* e;

        e = natura_ens_in(bib, modulus_cstr, ad_cstr);
        si (e && e->discrimen == NATURA_ENS_RES)
        {
            Xar*     tituli;
            chorda** locus;

            tituli = xar_creare(piscina, (i32)magnitudo(chorda*));
            locus  = (chorda**)xar_addere(tituli);
            *locus = ((NaturaRes*)e->corpus)->titulus;
            *clausura_ex = tituli;
            redde NC_CIT_FIT;
        }
    }
    redde NC_CIT_IGNOTA;
}

/* causam in censum referre - numeri in praefationem canonis eunt */
interior vacuum
_censum_notare(
    NcCensusRelationum*  census,
    NcCitatioStatus      status)
{
    si (!census)
    {
        redde;
    }
    census->omnes++;
    commutatio (status)
    {
        casus NC_CIT_FIT:       census->citatae++;     frange;
        casus NC_CIT_APERTA:    census->apertae++;     frange;
        casus NC_CIT_MULTIPLEX: census->multiplices++; frange;
        ordinarius:             census->ignotae++;     frange;
    }
}

/* apparatum unius catenae (iam hereditate solutum) in exemplar
 * plicare. Vocari BIS potest (sub= et etiam=); deduplicatio per
 * membra iam posita currit, ergo catena prior vincit. */
interior vacuum
_apparatum_plicare(
    NaturaBibliotheca*   bib,
    NcElementum*         el,
    Xar*                 apparatus,
    b32                  monolithus,
    NcCensusRelationum*  census,
    Piscina*             piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(apparatus); i++)
    {
        NaturaApparatusMembrum* am;
        chorda*                 titulus;
        chorda*                 multiplex;
        NcMembrum*              m;

        am      = (NaturaApparatusMembrum*)xar_obtinere(apparatus, i);
        titulus = stml_attributum_capere(am->nodus, "nomen");

        si (chorda_aequalis_literis(*am->nodus->titulus, "actio"))
        {
            chorda** locus;

            si (!titulus || _actio_adest(el->actiones, titulus))
            {
                perge;
            }
            locus  = (chorda**)xar_addere(el->actiones);
            *locus = titulus;
            perge;
        }

        si (chorda_aequalis_literis(*am->nodus->titulus,
                                    "machina_statuum"))
        {
            constans character* praefixum;

            si (!titulus || !_machina_ad_individuum(am->nodus))
            {
                perge;
            }
            praefixum = _praefixum_machinae(titulus);
            si (_membrum_adest(el->membra, titulus, praefixum))
            {
                perge;
            }
            m                = (NcMembrum*)xar_addere(el->membra);
            m->discrimen     = NC_MEMBRUM_ATTRIBUTUM;
            m->origo         = NC_ORIGO_MACHINA;
            m->titulus       = titulus;
            m->praefixum     = praefixum;
            m->genus_valoris = "electio";
            m->optiones      = _status_colligere(am->nodus, piscina);
            m->praestitutum  = NIHIL;
            m->citatio_ad    = NIHIL;
            perge;
        }

        si (!titulus || _membrum_adest(el->membra, titulus, NIHIL))
        {
            perge;
        }
        multiplex = stml_attributum_capere(am->nodus, "multiplex");

        m               = (NcMembrum*)xar_addere(el->membra);
        m->titulus      = titulus;
        m->praefixum    = NIHIL;
        m->optiones     = NIHIL;
        m->praestitutum = NIHIL;
        m->citatio_ad   = NIHIL;
        m->discrimen    = (multiplex &&
                           chorda_aequalis_literis(*multiplex, "verum"))
                          ? NC_MEMBRUM_LIBERUM : NC_MEMBRUM_ATTRIBUTUM;

        si (chorda_aequalis_literis(*am->nodus->titulus, "pars"))
        {
            /* necessaria= CONSULTO ignoratur: necessitas ontologica
             * obligatio documenti non est (spec par. 3.4) */
            m->discrimen     = NC_MEMBRUM_LIBERUM;
            m->origo         = NC_ORIGO_PARS;
            m->genus_valoris = "textus";
        }
        alioquin si (chorda_aequalis_literis(*am->nodus->titulus,
                                             "relatio"))
        {
            m->origo         = NC_ORIGO_RELATIO;
            m->genus_valoris = "nomen";

            si (monolithus)
            {
                /* LATIUS SCRIBERE PAR EST AC LEGERE (decretum huius
                 * operis). Identitas 'compositum' est (par. 5.3:
                 * datum consumentis, non clavis naturae), ergo
                 * 'rosa-ad-murum' licite DECLARATUR. Referens sub
                 * 'nomine' relictus eam nominare NEQUIRET - id est
                 * identitas licita et inaccessibilis, quod vitium
                 * gravius est quam laxitas. Compositum superset
                 * strictum nominis est: nihil prius licitum
                 * reicitur, clavis naturae snake ('carl_linnaeus')
                 * eodem modo transit.
                 *
                 * OMNIBUS relationibus datur, non citatis solis: in
                 * monolitho valor cuiusvis relationis identitatem
                 * documenti nominare potest (id ipsum monolithus
                 * est), et canon LIMITAT, non poscit (par. 3.4) -
                 * orthographiam ibi vetare ubi nihil resolvimus
                 * severitas sine causa esset.
                 *
                 * certitudo= et fons= CONSULTO intacta manent:
                 * ea claves NATURAE citant (<fontes clavis=>), non
                 * identitates documenti, ergo snake rectum est. */
                m->genus_valoris = "compositum";

                {
                    Xar*            clausura;
                    NcCitatioStatus status;

                    status = _petitum_citabile(bib, am->nodus,
                        (b32)(m->discrimen == NC_MEMBRUM_LIBERUM),
                        &clausura, piscina);
                    si (status == NC_CIT_FIT)
                    {
                        m->citatio_ad = clausura;
                    }
                    _censum_notare(census, status);
                }
            }
        }
        alioquin
        {
            chorda* g;

            g = stml_attributum_capere(am->nodus, "genus");
            m->origo         = NC_ORIGO_PROPRIETAS;
            m->genus_valoris = "textus";
            si (g && chorda_aequalis_literis(*g, "electio"))
            {
                m->genus_valoris = "electio";
                m->optiones      = _optiones_colligere(am->nodus,
                                                       piscina);
            }
            alioquin si (g && chorda_aequalis_literis(*g, "numerus"))
            {
                m->genus_valoris = "numerus";
            }
            alioquin si (g && chorda_aequalis_literis(*g, "veritas"))
            {
                m->genus_valoris = "veritas";
            }
            alioquin si (g && chorda_aequalis_literis(*g, "dies"))
            {
                m->genus_valoris = "dies";
            }
        }
    }
}

/* exemplar elementi ex ente aedificare.
 *
 * CATENAE DUAE cum res etiam= fert: natura.h dicit apparatum
 * AMBARUM catenarum tali rei deberi (scriptum conchae essentialiter
 * ET scriptum ET fons est). Catena SUA prima plicatur, deinde
 * altera: deduplicatio primum vincere sinit, ergo genus proprium
 * ligamina aequa vincit et 'maxime proprium primum' servatur.
 * Valores utramque catenam sequuntur eodem ordine, ne membrum ab
 * altera hereditatum praestitutum suum tacite amittat. */
interior NcElementum*
_elementum_aedificare(
    NaturaBibliotheca*   bib,
    NcEns*               ens,
    b32                  monolithus,
    NcCensusRelationum*  census,
    Piscina*             piscina)
{
    NcElementum* el;

    el           = (NcElementum*)piscina_allocare(piscina,
                       magnitudo(NcElementum));
    el->ens      = ens;
    el->membra   = xar_creare(piscina, (i32)magnitudo(NcMembrum));
    el->actiones = xar_creare(piscina, (i32)magnitudo(chorda*));

    si (!ens->genus)
    {
        redde el;
    }

    _apparatum_plicare(bib, el,
                       natura_apparatus(bib, ens->genus, piscina),
                       monolithus, census, piscina);
    si (ens->genus_etiam)
    {
        _apparatum_plicare(
            bib, el, natura_apparatus(bib, ens->genus_etiam, piscina),
            monolithus, census, piscina);
    }

    _valores_applicare(el, ens, piscina);
    redde el;
}

/* canonem unius moduli in plagulam scribere.
 *
 * CORPUS TOTUM iam oneratum est (sub= fines modulorum transit),
 * sed elementa huius moduli SOLA emittuntur - ordo idem quo entia
 * collecta sunt, id est ordo plagularum ORDINATUS, unde porta
 * rancoris comparationem byte-pro-byte facere potest.
 *
 * Plagula PRIMUM aperitur, deinde scribitur: si emissio recusat,
 * plagulam SEMIPLENAM delemus. Canon dimidius mendacium est quod
 * porta proxima pro vero acciperet. */
interior b32
_canonem_modulo_scribere(
    NaturaBibliotheca*   bib,
    Xar*                 entia,
    constans character*  modulus,
    constans character*  via,
    Piscina*             piscina)
{
    Xar*  elementa;
    FILE* f;
    b32   sanum;
    i32   i;

    elementa = xar_creare(piscina, (i32)magnitudo(NcElementum*));
    per (i = ZEPHYRUM; i < xar_numerus(entia); i++)
    {
        NcEns*         e;
        NcElementum**  locus;

        e = (NcEns*)xar_obtinere(entia, i);
        si (!chorda_aequalis_literis(*e->modulus, modulus))
        {
            perge;
        }
        locus  = (NcElementum**)xar_addere(elementa);
        *locus = _elementum_aedificare(bib, e, FALSUM, NIHIL,
                                      piscina);
    }

    si (xar_numerus(elementa) == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: modulus '%s' NULLUM ens habet - "
            "canon vacuus non scribitur\n", modulus);
        redde FALSUM;
    }

    f = fopen(via, "w");
    si (!f)
    {
        fprintf(stderr, "natura_canones: '%s' scribi nequit\n", via);
        redde FALSUM;
    }

    sanum = _canonem_emittere(f, elementa, modulus, modulus, NIHIL);

    /* FCLOSE IUDICATUR: scriptio pendens hic demum in discum it,
     * ergo defectus (discus plenus, quota exhausta) hic SOLUM
     * apparet. Neglectus caudam perditam sub nuntio 'scriptus'
     * abscondit, exitum 0 reddit, et '&& git commit' canonem
     * TRUNCUM committit - quod caput huius plagulae ipsum vetat
     * ('canon dimidius mendacium est'). Porta shell hoc capere
     * NEQUIT: plagulam vacuam solam probat. */
    si (fclose(f) != ZEPHYRUM)
    {
        sanum = FALSUM;
        fprintf(stderr,
            "natura_canones: '%s' claudi nequit - scriptio ultima "
            "perdita, canon truncus esset\n", via);
    }

    si (!sanum)
    {
        remove(via);
        fprintf(stderr,
            "natura_canones: '%s' RECUSATUS et deletus\n", via);
        redde FALSUM;
    }

    fprintf(stderr, "natura_canones: '%s' scriptus (elementa %u)\n",
            via, xar_numerus(elementa));
    redde VERUM;
}

/* nomen naturae (snake) -> nomen canonis (kebab), ut chorda.
 * _kebab_scribere idem in FILE facit; hic COMPARANDUM est, non
 * scribendum, et comparari debet id quod re vera emittitur. */
interior chorda
_kebab_chorda(
    constans chorda*  t,
    Piscina*          piscina)
{
    chorda c;
    i32    i;

    c = chorda_transcribere(*t, piscina);
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si ((character)c.datum[i] == '_')
        {
            c.datum[i] = (i8)'-';
        }
    }
    redde c;
}

/* CUSTOS UNICITATIS NOMINUM - reddere ZEPHYRUM DEBET.
 *
 * Monolithus spatia nominum XXXIII in unum fundit, et
 * _canonem_emittere nomina trans elementa NON deduplicat (intra
 * elementum solum). Duo <elementum nomen="X"> canonem tacite
 * corrumperent: tabula elementorum posterius prius obrueret, et
 * canon dimidiam veritatem legenti pro tota praeberet.
 *
 * HODIE NIHIL INVENIT, et id MENSURATUM est non speratum: nomina
 * DLXI (genera CLXXVII + res CCCLXXXIV) omnia unica sunt, sive
 * snake sive kebab comparata. Porta tamen stat, quia condicio
 * quae eam excitaret eadem prorsus est quae escapum '::' (spec
 * par. 9) tandem posceret - et tunc CLAMARE debet, non tacere.
 *
 * KEBAB comparatur, non snake: 'a_b' et 'a-b' nomina naturae
 * distincta essent sed elementum unum emitterent. */
interior i32
_nomina_gemina_numerare(
    Xar*      entia,
    Piscina*  piscina)
{
    TabulaDispersa* visa;
    i32             gemina;
    i32             i;

    visa   = tabula_dispersa_creare_chorda(piscina, M);
    gemina = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(entia); i++)
    {
        NcEns* e;
        chorda k;

        e = (NcEns*)xar_obtinere(entia, i);
        k = _kebab_chorda(e->titulus, piscina);
        si (tabula_dispersa_continet(visa, k))
        {
            gemina++;
            fprintf(stderr,
                "natura_canones: nomen '%.*s' (modulus '%.*s') GEMINUM "
                "- monolithus elementum duplex emitteret\n",
                (integer)k.mensura, (constans character*)k.datum,
                (integer)e->modulus->mensura,
                (constans character*)e->modulus->datum);
        }
        alioquin
        {
            tabula_dispersa_inserere(visa, k, e);
        }
    }
    redde gemina;
}

/* praefationem cum numeris vivis in receptaculum scribere.
 *
 * sprintf adhibetur quia forma NOSTRA est et numeri sex soli
 * variant; receptaculum tamen large mensuratum est et mensura
 * ANTE scriptionem probatur, ne canon praefatione trunca
 * nascatur. */
interior vacuum
_praefationem_scribere(
    character*                    ex,
    i32                           tectum,
    constans NcCensusRelationum*  census)
{
    i32 nudae;

    /* i32 INSIGNATUS est: subtractio quae infra nihilum caderet
     * numerum immanem pareret. Non potest hic (citatae pars
     * omnium est), sed custos gratis stat. */
    nudae = (census->omnes >= census->citatae)
            ? (census->omnes - census->citatae) : ZEPHYRUM;

    si ((i32)strlen(NC_PRAEFATIO_FORMA) + C >= tectum)
    {
        fprintf(stderr,
            "natura_canones: praefatio receptaculum excedit - "
            "canon de se ipso mentiretur\n");
        ex[ZEPHYRUM] = '\0';
        redde;
    }

    sprintf(ex, NC_PRAEFATIO_FORMA,
            census->omnes, census->citatae, nudae,
            census->apertae, census->multiplices,
            census->ignotae);
}

/* MONOLITHUS: corpus totum in canonem unum.
 *
 * Idem emissor, idem exemplar - elementa SOLA differunt (omnia,
 * non unius moduli) et relationes citationes fiunt.
 *
 * Praefatio lectorem monet monolithum canones per modulum NON
 * vincere: in relationibus plus praestat, in actionibus minus. */
interior b32
_canonem_totum_scribere(
    NaturaBibliotheca*   bib,
    Xar*                 entia,
    constans character*  via,
    Piscina*             piscina)
{
    NcCensusRelationum census;
    character          praefatio[NC_PRAEFATIO_MAXIMA];
    Xar*               elementa;
    FILE*              f;
    b32                sanum;
    i32                gemina;
    i32                i;

    census.omnes       = ZEPHYRUM;
    census.citatae     = ZEPHYRUM;
    census.apertae     = ZEPHYRUM;
    census.multiplices = ZEPHYRUM;
    census.ignotae     = ZEPHYRUM;

    gemina = _nomina_gemina_numerare(entia, piscina);
    si (gemina > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: nomina %u GEMINA - monolithus RECUSATUR "
            "(vide spec par. 9: escapum '::' hic poscitur)\n", gemina);
        redde FALSUM;
    }

    elementa = xar_creare(piscina, (i32)magnitudo(NcElementum*));
    per (i = ZEPHYRUM; i < xar_numerus(entia); i++)
    {
        NcEns*        e;
        NcElementum** locus;

        e      = (NcEns*)xar_obtinere(entia, i);
        locus  = (NcElementum**)xar_addere(elementa);
        *locus = _elementum_aedificare(bib, e, VERUM, &census,
                                       piscina);
    }
    _praefationem_scribere(praefatio, (i32)magnitudo(praefatio),
                           &census);

    f = fopen(via, "w");
    si (!f)
    {
        fprintf(stderr, "natura_canones: '%s' scribi nequit\n", via);
        redde FALSUM;
    }

    sanum = _canonem_emittere(f, elementa, "individua",
                              "natura tota", praefatio);

    /* FCLOSE IUDICATUR - vide _canonem_modulo_scribere supra */
    si (fclose(f) != ZEPHYRUM)
    {
        sanum = FALSUM;
        fprintf(stderr,
            "natura_canones: '%s' claudi nequit - scriptio ultima "
            "perdita, canon truncus esset\n", via);
    }

    si (!sanum)
    {
        remove(via);
        fprintf(stderr,
            "natura_canones: '%s' RECUSATUS et deletus\n", via);
        redde FALSUM;
    }

    fprintf(stderr, "natura_canones: '%s' scriptus (elementa %u)\n",
            via, xar_numerus(elementa));
    redde VERUM;
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*            piscina;
    NaturaBibliotheca*  bib;
    Xar*                entia;
    constans character* radix;
    constans character* inspiciendum;
    constans character* modulus;
    constans character* ad;
    b32                 modus_index;
    b32                 modus_totum;
    i32                 vulnera;
    s32                 i;

    radix        = "natura";
    inspiciendum = NIHIL;
    modulus      = NIHIL;
    ad           = NIHIL;
    modus_index  = FALSUM;
    modus_totum  = FALSUM;
    vulnera      = ZEPHYRUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-index") == ZEPHYRUM)
        {
            modus_index = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-totum") == ZEPHYRUM)
        {
            modus_totum = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-inspicere") == ZEPHYRUM &&
                     i + I < numerus)
        {
            inspiciendum = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-modulus") == ZEPHYRUM &&
                     i + I < numerus)
        {
            modulus = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-ad") == ZEPHYRUM &&
                     i + I < numerus)
        {
            ad = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_canones [-index] [-inspicere GENUS] "
                "[-modulus NOMEN -ad VIA] [-totum -ad VIA] "
                "[-radix DIR]\n");
            redde II;
        }
    }

    /* -modulus et -totum se excludunt: elementa eorum discrepant
     * (unius moduli / omnia) et -ad unum est. Ambo data, alterum
     * TACITE periret. */
    si (modus_totum && modulus)
    {
        fprintf(stderr,
            "natura_canones: -totum et -modulus simul dari nequeunt\n");
        redde II;
    }

    /* par -modulus/-ad INTEGRUM poscitur: '-modulus planta' solum
     * canonem in nihilum scriberet, quod successus VIDERETUR.
     * -totum eadem ratione -ad poscit. */
    si ((modulus != NIHIL) != (ad != NIHIL) && !modus_totum)
    {
        fprintf(stderr,
            "natura_canones: -modulus et -ad simul dari debent\n");
        redde II;
    }
    si (modus_totum && !ad)
    {
        fprintf(stderr, "natura_canones: -totum -ad VIA poscit\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("natura_canones", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib || !_corpus_onerare(bib, radix, piscina, &vulnera))
    {
        redde II;
    }

    /* corpus AEGRUM proiectionem non impedit (decretum: vulnera
     * stantia iudicia aperta hominis sunt, et generator qui ob ea
     * recusaret usque ad eorum solutionem inutilis esset), sed
     * TACITE numquam transit */
    si (vulnera > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: corpus vulnera %u - proiectio tamen "
            "pergit\n", vulnera);
    }

    /* degradatio VISIBILIS: genus quod canon non fert ad textum
     * cadit (recte - aliud non est), sed numquam TACITE */
    _genera_ignota_nuntiare(bib, piscina);

    entia = _entia_colligere(bib, piscina);
    si (xar_numerus(entia) == ZEPHYRUM)
    {
        fprintf(stderr, "natura_canones: NULLUM ens inventum\n");
        redde II;
    }

    si (modus_index)
    {
        i32 j;

        per (j = ZEPHYRUM; j < xar_numerus(entia); j++)
        {
            NcEns* e;

            e = (NcEns*)xar_obtinere(entia, j);
            imprimere("%.*s\t%.*s\t",
                      (integer)e->modulus->mensura,
                      (constans character*)e->modulus->datum,
                      (integer)e->titulus->mensura,
                      (constans character*)e->titulus->datum);
            _kebab_scribere(stdout, e->titulus);
            imprimere("\t%s\n", e->est_res ? "res" : "genus");
        }
        redde ZEPHYRUM;
    }

    si (inspiciendum)
    {
        NcEns*       e;
        NcElementum* el;

        e = _ens_invenire(entia, inspiciendum);
        si (!e)
        {
            fprintf(stderr, "natura_canones: ens '%s' non inventum\n",
                    inspiciendum);
            redde II;
        }
        el = _elementum_aedificare(bib, e, FALSUM, NIHIL, piscina);
        _elementum_inspicere(stdout, el);
        redde ZEPHYRUM;
    }

    si (modulus)
    {
        si (!_canonem_modulo_scribere(bib, entia, modulus, ad,
                                      piscina))
        {
            redde II;
        }
        redde ZEPHYRUM;
    }

    si (modus_totum)
    {
        si (!_canonem_totum_scribere(bib, entia, ad, piscina))
        {
            redde II;
        }
        redde ZEPHYRUM;
    }

    fprintf(stderr,
        "natura_canones: nihil petitum (adde -index aut -inspicere)\n");
    redde II;
}
