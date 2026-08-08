/* probationes/probatio_natura_canones.c
 *
 * CANONES GENERATI - CUSTODIA CORPORIS
 *
 * Opera priora catenam struxerunt
 *
 *   natura/<modulus>.genera
 *       |  natura_canones
 *   natura/cocta/<modulus>.canon  (+ monolithus individua.canon)
 *       |  canon_coquere
 *   lector C typatus
 *
 * eamque MANU probaverunt. Haec plagula id quod manus semel vidit
 * in suitam transfert: proprietas quae hic non asseritur proxima
 * mutatione generatoris TACITE perit, quia littera generata
 * committitur et diff nemo legit.
 *
 * LEX HUIUS PLAGULAE - ORACULUM ANTE
 *
 *   De omni assertione quaeritur: 'quid HERI fecisset, ante quam
 *   mechanismus exstitit?' Si responsum 'praeteriisset' est,
 *   assertio aliud probat quam scriptor putat.
 *
 *   Opus quartum id mensuravit: asserere 'nullam citationem
 *   emissam esse' ANTE emendationem quoque praeteriisset - quod
 *   vere discrevit fuit census, id est sedes de sacco in saccum
 *   MOTA. Ideo hic fere ubique PAR ponitur cuius altera pars
 *   RUERE debet:
 *
 *     - documentum quod citationem RESOLVIT / quod eam PENDENTEM
 *       relinquit (altera sola nihil probat: canon sine ulla
 *       citatione utramque acciperet)
 *     - corpus nominum sanorum quod SCRIBITUR / corpus geminum
 *       quod RECUSATUR (sine priore, 'exitus II' etiam defectum
 *       scriptionis significare posset)
 *     - pars naturae NECESSARIA / NON necessaria quae in canone
 *       INDISCERNIBILES fiunt (sine posteriore, assertio 'nullum
 *       minimum' vacua esset)
 *
 * NUMERI CORPORIS HIC NON FIGUNTUR. Nemo scribit 'citationes CCXVII'
 * aut 'canones XXXIV': numerus fixus mentiri incipit quo momento
 * exemplar additur - id ipsum quod praefatio canonis de se ipsa
 * dicit. Loco eorum RELATIONES asseruntur (census se ipsum
 * reconciliat; canones cocti tot sunt quot exemplaria + I).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "canon.h"
#include "stml.h"
#include "filum.h"
#include "iter_directoria.h"
#include "processus.h"
#include <stdio.h>
#include <sys/stat.h>


/* ==================================================
 * Adiutoria - arbor STML
 * ================================================== */

/* Nodos elementorum numerare qui attributum nominatum ferunt.
 * valor NIHIL = valor quilibet. Si primum datum est, primum
 * inventum ibi ponitur - unde assertio 'unum solum, ET hoc'
 * uno transitu fit.
 *
 * NOTA: nomina attributorum, non valores. '<liberum nomen="radix"/>'
 * attributum 'radix' NON fert (nomen eius radix est), ergo hoc
 * cum '<elementum radix="verum">' non confunditur. */
interior i32
_libera_cum_minimo_numerare(
    StmlNodus*  n)
{
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    si (!n)
    {
        redde ZEPHYRUM;
    }
    si (n->genus == STML_NODUS_ELEMENTUM &&
        chorda_aequalis_literis(*n->titulus, "liberum") &&
        stml_attributum_capere(n, "minimum"))
    {
        summa++;
    }
    si (n->liberi)
    {
        per (i = ZEPHYRUM; i < xar_numerus(n->liberi); i++)
        {
            summa += _libera_cum_minimo_numerare(
                *(StmlNodus**)xar_obtinere(n->liberi, i));
        }
    }
    redde summa;
}

interior i32
_ferentes_numerare(
    StmlNodus*           n,
    constans character*  attributum,
    constans character*  valor,
    StmlNodus**          primum)
{
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    si (!n)
    {
        redde ZEPHYRUM;
    }

    si (n->genus == STML_NODUS_ELEMENTUM)
    {
        chorda* v;

        v = stml_attributum_capere(n, attributum);
        si (v && (!valor || chorda_aequalis_literis(*v, valor)))
        {
            si (primum && !*primum)
            {
                *primum = n;
            }
            summa++;
        }
    }

    si (n->liberi)
    {
        per (i = ZEPHYRUM; i < xar_numerus(n->liberi); i++)
        {
            summa += _ferentes_numerare(
                *(StmlNodus**)xar_obtinere(n->liberi, i),
                attributum, valor, primum);
        }
    }
    redde summa;
}

/* Nodos elementorum titulo dato numerare (tota arbore). */
interior i32
_titulo_numerare(
    StmlNodus*           n,
    constans character*  titulus)
{
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    si (!n)
    {
        redde ZEPHYRUM;
    }

    si (n->genus == STML_NODUS_ELEMENTUM && n->titulus &&
        chorda_aequalis_literis(*n->titulus, titulus))
    {
        summa++;
    }

    si (n->liberi)
    {
        per (i = ZEPHYRUM; i < xar_numerus(n->liberi); i++)
        {
            summa += _titulo_numerare(
                *(StmlNodus**)xar_obtinere(n->liberi, i), titulus);
        }
    }
    redde summa;
}

/* Elementum canonis nomine dato inter liberos radicis invenire,
 * ADSTRICTA (intra=) praetermissa.
 *
 * CUR adstricta praetermittuntur: nomina eorum per genera
 * communicantur ('historia' in canone monolitho centies apparet),
 * ergo tabula ex illis facta se ipsam obrueret. Citatio autem
 * per titulum EXACTUM congruit et intra= non observat, ergo
 * generum elementa (non adstricta) sunt praecise quod hic
 * quaeritur. */
interior StmlNodus*
_elementum_invenire(
    StmlNodus*  canon_radix,
    chorda      titulus)
{
    i32 i;

    si (!canon_radix || !canon_radix->liberi)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < xar_numerus(canon_radix->liberi); i++)
    {
        StmlNodus* l;
        chorda*    n;

        l = *(StmlNodus**)xar_obtinere(canon_radix->liberi, i);
        si (l->genus != STML_NODUS_ELEMENTUM || !l->titulus ||
            !chorda_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        si (stml_attributum_habet(l, "intra"))
        {
            perge;
        }
        n = stml_attributum_capere(l, "nomen");
        si (n && chorda_aequalis(*n, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* Elementum ADSTRICTUM (intra=) nomine et scopo datis invenire -
 * comes eius supra.
 *
 * Scopus hic COMPARATUR, non praetermittitur: haec est sola via
 * qua adstrictio ipsa asseri potest, et adstrictio nusquam aliter
 * custoditur. Canon eam custodire NEQUIT: unicitas per DOCUMENTUM
 * currit, ergo 'nomen elementi semel per canonem' inexpressibile
 * est PRAECISE quia intra= nomina repetita licita facit. */
interior StmlNodus*
_elementum_adstrictum_invenire(
    StmlNodus*  canon_radix,
    chorda      titulus,
    chorda      scopus)
{
    i32 i;

    si (!canon_radix || !canon_radix->liberi)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < xar_numerus(canon_radix->liberi); i++)
    {
        StmlNodus* l;
        chorda*    n;
        chorda*    intra;

        l = *(StmlNodus**)xar_obtinere(canon_radix->liberi, i);
        si (l->genus != STML_NODUS_ELEMENTUM || !l->titulus ||
            !chorda_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        intra = stml_attributum_capere(l, "intra");
        si (!intra || !chorda_aequalis(*intra, scopus))
        {
            perge;
        }
        n = stml_attributum_capere(l, "nomen");
        si (n && chorda_aequalis(*n, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* Nomina elementorum NON adstrictorum bis declarata numerare.
 *
 * PROPRIETAS QUAM CANON DICERE NEQUIT, ideo hic: elementa sine
 * intra= per canonem unica esse DEBENT; adstricta libere nomina
 * communicant ('historia' per genera omnia). Si generator intra=
 * omittere coeperit, elementa antea adstricta nuda fiunt et
 * COLLIDUNT - quod canon_examen exitu 0 acciperet (mensuratum) et
 * nulla alia porta videret. */
interior i32
_nuda_gemina_numerare(
    StmlNodus*  canon_radix,
    Piscina*    piscina)
{
    TabulaDispersa* visa;
    i32             gemina;
    i32             i;

    visa   = tabula_dispersa_creare_chorda(piscina, LXIV);
    gemina = ZEPHYRUM;

    si (!canon_radix || !canon_radix->liberi)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(canon_radix->liberi); i++)
    {
        StmlNodus* l;
        chorda*    n;

        l = *(StmlNodus**)xar_obtinere(canon_radix->liberi, i);
        si (l->genus != STML_NODUS_ELEMENTUM || !l->titulus ||
            !chorda_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        si (stml_attributum_habet(l, "intra"))
        {
            perge;
        }
        n = stml_attributum_capere(l, "nomen");
        si (!n)
        {
            perge;
        }
        si (tabula_dispersa_continet(visa, *n))
        {
            gemina++;
        }
        alioquin
        {
            tabula_dispersa_inserere(visa, *n, l);
        }
    }
    redde gemina;
}

/* Utrum elementum liberum tituli dati cum nomine dato declaret
 * ('attributum' aut 'liberum'). */
interior StmlNodus*
_declarationem_invenire(
    StmlNodus*           elementum,
    constans character*  species,
    chorda               titulus)
{
    i32 i;

    si (!elementum || !elementum->liberi)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < xar_numerus(elementum->liberi); i++)
    {
        StmlNodus* l;
        chorda*    n;

        l = *(StmlNodus**)xar_obtinere(elementum->liberi, i);
        si (l->genus != STML_NODUS_ELEMENTUM || !l->titulus ||
            !chorda_aequalis_literis(*l->titulus, species))
        {
            perge;
        }
        n = stml_attributum_capere(l, "nomen");
        si (n && chorda_aequalis(*n, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* Omnes nodos tituli dati in Xar colligere (transitus profundus). */
interior vacuum
_titulo_colligere(
    StmlNodus*           n,
    constans character*  titulus,
    Xar*                 ex)
{
    i32 i;

    si (!n)
    {
        redde;
    }

    si (n->genus == STML_NODUS_ELEMENTUM && n->titulus &&
        chorda_aequalis_literis(*n->titulus, titulus))
    {
        *(StmlNodus**)xar_addere(ex) = n;
    }

    si (n->liberi)
    {
        per (i = ZEPHYRUM; i < xar_numerus(n->liberi); i++)
        {
            _titulo_colligere(
                *(StmlNodus**)xar_obtinere(n->liberi, i), titulus, ex);
        }
    }
}


/* ==================================================
 * Adiutoria - textus praefationis
 * ================================================== */

/* Numerum QUI SIGNUM PRAECEDIT legere.
 *
 * Praefatio canonis prosa est ('... ex 2056 sedibus relationum
 * ...'), et numerus a locutione sua per lineam novam separari
 * potest. Ergo: signum invenire, spatium album retro transire,
 * cifras retro legere.
 *
 * Redde FALSUM si signum abest aut cifra ante id nulla - quod
 * ipsum assertio est: praefatio quae numerum suum amisit de se
 * mentitur. */
interior b32
_numerum_ante(
    chorda               fons,
    constans character*  signum,
    Piscina*             piscina,
    i32*                 ex)
{
    s32    sedes;
    s32    i;
    s32    finis;
    chorda cifrae;

    sedes = chorda_invenire_index(fons,
                                  chorda_ex_literis(signum, piscina));
    si (sedes < ZEPHYRUM)
    {
        redde FALSUM;
    }

    i = sedes - I;
    dum (i >= ZEPHYRUM)
    {
        character c;

        c = (character)fons.datum[i];
        si (c != ' ' && c != '\n' && c != '\t' && c != '\r')
        {
            frange;
        }
        i--;
    }

    finis = i + I;
    dum (i >= ZEPHYRUM)
    {
        character c;

        c = (character)fons.datum[i];
        si (c < '0' || c > '9')
        {
            frange;
        }
        i--;
    }

    si (finis == i + I)
    {
        redde FALSUM;
    }

    cifrae = chorda_sectio(fons, (i32)(i + I), (i32)finis);
    redde chorda_ut_i32(cifrae, ex);
}


/* ==================================================
 * Adiutoria - directoria
 * ================================================== */

/* Vias plagularum directorii extensione dato colligere.
 * Redde: Xar de chorda (via plena, ex piscina). */
interior Xar*
_vias_colligere(
    constans character*  directorium,
    constans character*  extensio,
    Piscina*             piscina)
{
    DirectoriumIterator*   iter;
    DirectoriumIntroitus*  e;
    Xar*                   ex;
    chorda                 praefixum;
    chorda                 suffixum;

    ex        = xar_creare(piscina, (i32)magnitudo(chorda));
    praefixum = chorda_concatenare(
                    chorda_ex_literis(directorium, piscina),
                    chorda_ex_literis("/", piscina), piscina);
    suffixum  = chorda_ex_literis(extensio, piscina);

    iter = directorium_iterator_aperire(directorium, piscina);
    si (!iter)
    {
        redde ex;
    }

    dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
    {
        si (e->genus != INTROITUS_FILUM)
        {
            perge;
        }
        si (!chorda_terminatur(e->titulus, suffixum))
        {
            perge;
        }
        *(chorda*)xar_addere(ex) =
            chorda_concatenare(praefixum, e->titulus, piscina);
    }
    directorium_iterator_claudere(iter);
    redde ex;
}


/* ==================================================
 * Adiutoria - generator vocatus
 * ================================================== */

/* Fontes quorum mutatio binarium stalum facit. Idem index quem
 * ./tools/natura_canones.sh custodit; hic repetitur quia probatio
 * de BINARIO iudicat, non de canonibus, et duo iudicia seiuncta
 * manere debent.
 *
 * INDEX PLENUS, ne commentarium supra mentiatur: natura_struere.sh
 * clausuram naturae TOTAM in bin/natura_canones nectit, ergo
 * lib/natura.c, include/natura.h, lib/stml.c et include/stml.h
 * binarium aeque stalum faciunt quam fontes ipsius instrumenti.
 * MENSURATUM cum tribus solis: lib/natura.c recentiore, porta
 * shell exitu II recusabat dum haec probatio eodem binario
 * LXXIII/LXXIII viridis manebat - id est index qui paritatem
 * CLAMAT et dimidiam praestat, quod peius est quam nullus, quia
 * lectorem ab ipsa reconciliatione avertit. */
staticus constans character* NC_FONTES[] = {
    "tools/natura_canones.c",
    "tools/natura_canones_emissio.c",
    "tools/natura_canones.h",
    "lib/natura.c",
    "include/natura.h",
    "lib/stml.c",
    "include/stml.h",
    NIHIL
};

/* Fons qui binario RECENTIOR est, vel NIHIL si nullus.
 *
 * CUR OMNINO: nihil binarium ad fontem suum ligat, et
 * compile_tests.sh id numquam struit. MENSURATUM: custode e
 * tools/natura_canones.c amoto sed binario vetere manente, haec
 * probatio LXVII/LXVII viridis manebat - id est suita quae
 * generatorem cuius fons custodem iam non continet APPROBAT.
 *
 * Absentia hic NON tractatur (vocans eam prius solvit): binarium
 * numquam structum et binarium fonti suo dissimile causae DIVERSAE
 * sunt, et qui eas confundit auctori dicit ut struat cum res vera
 * aliud poscat. */
interior constans character*
_fons_recentior_binario(
    constans character*  binarium)
{
    structura stat  b;
    i32             i;

    si (stat(binarium, &b) != ZEPHYRUM)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; NC_FONTES[i] != NIHIL; i++)
    {
        structura stat f;

        si (stat(NC_FONTES[i], &f) != ZEPHYRUM)
        {
            perge;   /* absentiam _fons_absens sola nuntiat */
        }
        si (f.st_mtime > b.st_mtime)
        {
            redde NC_FONTES[i];
        }
    }
    redde NIHIL;
}

/* Primum fontem custodiae qui NON EXSTAT reddere, aut NIHIL.
 *
 * Gemellus in crusta bis clamat ('custos X ABEST' - natura_canones.sh
 * et canon_coquere.sh); hic tacebat. Index custodum qui fontem
 * nominat qui non est custodit MINUS quam pollicetur, et tacite:
 * exactum quod indices utrosque emendare coegit. Tertia instantia
 * eiusdem generis; ideo hic quoque clamat. */
interior constans character*
_fons_absens(
    vacuum)
{
    structura stat  f;
    i32             i;

    per (i = ZEPHYRUM; NC_FONTES[i] != NIHIL; i++)
    {
        si (stat(NC_FONTES[i], &f) != ZEPHYRUM)
        {
            redde NC_FONTES[i];
        }
    }
    redde NIHIL;
}

nomen structura {
    b32  cucurrit;         /* processus generatus et exspectatus */
    i32  codex;            /* codex exitus */
    b32  plagula_scripta;  /* an canon in disco iaceat */
    b32  clamavit;         /* an erratum custodem nominet */
} NcCursus;

/* bin/natura_canones super corpore dato currere.
 *
 * Plagula petita ANTE deletur: 'scripta' aliter ex cursu priore
 * superesse posset, et custos qui nihil scribit sic scripsisse
 * VIDERETUR. */
interior NcCursus
_generatorem_currere(
    constans character*  radix_corporis,
    constans character*  ad,
    Piscina*             piscina)
{
    constans character* argumenta[VII];
    ProcessusResultus   r;
    NcCursus            ex;

    filum_delere(ad);

    argumenta[ZEPHYRUM] = "bin/natura_canones";
    argumenta[I]        = "-radix";
    argumenta[II]       = radix_corporis;
    argumenta[III]      = "-totum";
    argumenta[IV]       = "-ad";
    argumenta[V]        = ad;
    argumenta[VI]       = NIHIL;

    r = processus_exsequi(argumenta, 60000, piscina);

    ex.cucurrit        = r.successus;
    ex.codex           = r.codex_exitus;
    ex.plagula_scripta = filum_existit(ad);
    ex.clamavit        = chorda_continet(r.erratum,
        chorda_ex_literis("GEMINA - monolithus RECUSATUR", piscina));

    si (!r.successus)
    {
        imprimere("  processus NON cucurrit: %s\n",
                  processus_error_nomen(r.error));
    }
    redde ex;
}

/* Documentum contra canonem iudicare: generis_ex numerum vitiorum
 * generis quaesiti recipit, omnia_ex numerum totalem.
 *
 * Redde FALSUM si documentum ipsum parsari nequit. Signum in
 * REDITU stat, non in numero: i32 INSIGNATUS est, ergo '-I' pro
 * defectu numerum immanem pareret quem assertio 'aequalis
 * ZEPHYRO' ut vitium legeret - vera causa (documentum fractum)
 * tacita maneret. */
interior b32
_documentum_iudicare(
    Canon*                canon,
    constans character*   documentum,
    CanonVitiumGenus      quaesitum,
    i32*                  generis_ex,
    i32*                  omnia_ex,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    StmlResultus  res;
    Xar*          vitia;
    i32           summa;
    i32           i;

    *generis_ex = ZEPHYRUM;
    *omnia_ex   = ZEPHYRUM;

    res = stml_legere_ex_literis(documentum, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        imprimere("  FRACTA: documentum parsari nequit\n");
        redde FALSUM;
    }

    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    summa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        CanonVitium* v;

        v = (CanonVitium*)xar_obtinere(vitia, i);
        si (v->genus == quaesitum)
        {
            summa++;
        }
    }
    *generis_ex = summa;
    *omnia_ex   = xar_numerus(vitia);
    redde VERUM;
}


/* ==================================================
 * Documenta probationis
 *
 * 'aedificium' et 'inscriptio' electa sunt quia relatio
 * 'inscriptio_eius' petitum FOLIUM habet - clausura titulo uno,
 * forma simplicissima citationis. Par clausurae infra petitum
 * cum posteris probat (pollinatur_a -> animal, clavis sub
 * titulo SUBGENERIS stans).
 * ================================================== */

/* PAR, pars prior: clavis in documento adest -> sanum */
staticus constans character* DOC_RESOLVENS =
    "<individua>\n"
    "  <aedificium nomen=\"#domus-nostra\" inscriptio-eius=\"#via-lata-x\"/>\n"
    "  <inscriptio nomen=\"#via-lata-x\"/>\n"
    "</individua>\n";

/* PAR, pars posterior: EADEM structura, clavis quae nihil nominat */
staticus constans character* DOC_PENDENS =
    "<individua>\n"
    "  <aedificium nomen=\"#domus-nostra\" inscriptio-eius=\"#nemo-omnino\"/>\n"
    "  <inscriptio nomen=\"#via-lata-x\"/>\n"
    "</individua>\n";

/* PAR CLAUSURAE: pollinatur_a petitum 'animal' habet, quod
 * posteros fert (apis res eius est). Clavis sub titulo apis
 * stat - forma simplici (titulo solo 'animal') citatio eam non
 * caperet et documentum RECTUM rueret; ante clausuram citatio
 * nulla omnino erat et par hoc indiscernibile. */
staticus constans character* DOC_CLAUSURA_RESOLVENS =
    "<individua>\n"
    "  <apis nomen=\"#apis-prima\"/>\n"
    "  <rosa nomen=\"#rosa-prima\" pollinatur-a=\"#apis-prima\"/>\n"
    "</individua>\n";

/* pars posterior: EADEM structura, clavis quae nihil nominat -
 * ANTE clausuram haec quoque transibat (citatio nulla), ergo
 * assertio 'ruere debet' sola mutationem probat */
staticus constans character* DOC_CLAUSURA_PENDENS =
    "<individua>\n"
    "  <apis nomen=\"#apis-prima\"/>\n"
    "  <rosa nomen=\"#rosa-prima\" pollinatur-a=\"#nemo-omnino\"/>\n"
    "</individua>\n";

/* '.apis' = genus e vocabulario - sententia gradus GENERIS
 * ('a apibus pollinatur, apis nulla certa') ANTE signa
 * inexprimibilis erat */
staticus constans character* DOC_GENUS_REF =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" pollinatur-a=\".apis\"/>\n"
    "</individua>\n";

staticus constans character* DOC_GENUS_IGNOTUM =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" pollinatur-a=\".piscis-volans\"/>\n"
    "</individua>\n";

/* INSCRIPTIO (spec census): census sodales suos sedet - per tag
 * GENERIS (individuum tags mortua): linea una individuum
 * dictionarii praesentat, et referentia resolvit */
staticus constans character* DOC_INSCRIPTUS =
    "<individua>\n"
    "  <persona nomen=\"#carl_linnaeus\"/>\n"
    "  <rosa nomen=\"#rosa-una\" auctor-nominis=\"#carl_linnaeus\"/>\n"
    "</individua>\n";

staticus constans character* DOC_NON_INSCRIPTUS =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" auctor-nominis=\"#carl_linnaeus\"/>\n"
    "</individua>\n";

/* CANON MODULI: communicatio petitum intra modulum habet
 * (nuntius missus-a -> actor) - canon moduli citare potest */
staticus constans character* DOC_NUNTII_SANUS =
    "<individua>\n"
    "  <actor nomen=\"#actor-unus\"/>\n"
    "  <nuntius nomen=\"#n-unus\" missus-a=\"#actor-unus\"/>\n"
    "</individua>\n";

staticus constans character* DOC_NUNTII_PENDENS =
    "<individua>\n"
    "  <nuntius nomen=\"#n-unus\" missus-a=\"#nemo\"/>\n"
    "</individua>\n";

/* RESTRICTIO: documentum uni-modulare sine referentiis trans
 * modulos verdictum IDEM sub canone moduli et monolitho accipit
 * (spec par. 1 - divergentia = scopus, numquam accidens) */
staticus constans character* DOC_PURUS =
    "<individua>\n"
    "  <rosa-canina nomen=\"#rosa-una\" status-vita=\"florens\"/>\n"
    "</individua>\n";

staticus constans character* DOC_PURUS_VITIOSUS =
    "<individua>\n"
    "  <rosa-canina nomen=\"#rosa-una\" status-vita=\"volans\"/>\n"
    "</individua>\n";

/* UNICITAS trans genera: genera DUO DIVERSA idem nomen ferunt.
 * Hoc est quod spatium nominum UNUM probat - spatium per
 * elementum hoc acciperet. */
staticus constans character* DOC_GEMINUM_TRANS =
    "<individua>\n"
    "  <aedificium nomen=\"#idem-nomen\"/>\n"
    "  <inscriptio nomen=\"#idem-nomen\"/>\n"
    "</individua>\n";

/* UNICITAS intra genus idem - forma facilior, quam spatium per
 * elementum quoque caperet. Ideo sola non sufficit. */
staticus constans character* DOC_GEMINUM_IDEM =
    "<individua>\n"
    "  <inscriptio nomen=\"#idem-nomen\"/>\n"
    "  <inscriptio nomen=\"#idem-nomen\"/>\n"
    "</individua>\n";


s32
principale(
    vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    chorda               monolithus_fons;
    StmlResultus         monolithus_res;
    StmlNodus*           monolithus_radix;
    Canon*               monolithus;
    b32                  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_natura_canones",
                                         8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    monolithus       = NIHIL;
    monolithus_radix = NIHIL;


    /* ========================================================
     * I. CUSTOS NOMINUM GEMINORUM
     *
     * Monolithus spatia nominum XXXIII in unum fundit; duo
     * genera eodem nomine elementum duplex emitterent et
     * tabula elementorum posterius prius obrueret - canon
     * dimidiam veritatem pro tota praebens.
     *
     * TRES CURSUS, non unus: sine corpore SANO 'exitus II'
     * etiam significare posset directorium non scribi posse,
     * et tunc probatio custodem laudaret ubi defectus est.
     * ======================================================== */

    {
        NcCursus sana;
        NcCursus gemina;
        NcCursus kebab;

        imprimere("\n--- I. custos nominum geminorum ---\n");

        /* DUAE CAUSAE, DUAE VIAE - eas confundere est auctori
         * dicere ut struat cum res vera aliud poscat.
         *
         * (a) ABSENS: struere semel. Probatio quae rubet quia
         *     instrumentum nondum structum est de instrumento
         *     nihil dicit, et suita quae in arbore recenti rubet
         *     mox neglegitur.
         * (b) STALUS: RECUSARE. Binarium fonti suo dissimile
         *     probationem de codice qui iam non exsistit facit.
         *     MENSURATUM: custode e fonte amoto sed binario
         *     vetere manente, haec probatio tota VIRIDIS manebat. */
        si (!filum_existit("bin/natura_canones"))
        {
            constans character* struere[II];
            ProcessusResultus   rs;

            imprimere("  (bin/natura_canones abest - struo semel)\n");
            struere[ZEPHYRUM] = "./tools/natura_struere.sh";
            struere[I]        = NIHIL;
            rs = processus_exsequi(struere, 300000, piscina);
            si (!rs.successus || rs.codex_exitus != ZEPHYRUM)
            {
                imprimere("  structor fefellit (codex %u)\n",
                          rs.codex_exitus);
            }
        }
        CREDO_VERUM (filum_existit("bin/natura_canones"));

        {
            constans character* absens;
            constans character* stalum;

            absens = _fons_absens();
            si (absens)
            {
                imprimere("  custos '%s' ABEST - index custodum fontem"
                          " nominat qui non est\n", absens);
            }
            CREDO_NIHIL (absens);

            stalum = _fons_recentior_binario("bin/natura_canones");
            si (stalum)
            {
                imprimere("  bin/natura_canones STALUS ('%s' recentior)"
                          " - strue primum: ./tools/natura_struere.sh\n",
                          stalum);
            }
            CREDO_NIHIL (stalum);
        }

        CREDO_VERUM (filum_directorium_creare_si_necesse(
                         "build/probatio_natura_canones"));

        /* PROBA CONTRARIA: nomina distincta -> canon SCRIPTUS */
        sana = _generatorem_currere(
                   "probationes/exempla/nc_nomina_sana",
                   "build/probatio_natura_canones/sana.canon",
                   piscina);
        CREDO_VERUM (sana.cucurrit);
        CREDO_AEQUALIS_I32 (sana.codex, (i32)ZEPHYRUM);
        CREDO_VERUM (sana.plagula_scripta);
        CREDO_FALSUM (sana.clamavit);

        /* FORMA PRIMA: scriptio eadem in exemplaribus duobus */
        gemina = _generatorem_currere(
                     "probationes/exempla/nc_nomina_gemina",
                     "build/probatio_natura_canones/gemina.canon",
                     piscina);
        CREDO_VERUM (gemina.cucurrit);
        CREDO_AEQUALIS_I32 (gemina.codex, (i32)II);
        CREDO_FALSUM (gemina.plagula_scripta);
        CREDO_VERUM (gemina.clamavit);

        /* FORMA SECUNDA: 'res_communis' et 'res-communis' -
         * nomina naturae DIVERSA quae in idem nomen canonis
         * plicantur. Custos qui snake solum compararet hanc
         * TACITE emitteret. */
        kebab = _generatorem_currere(
                    "probationes/exempla/nc_nomina_kebab",
                    "build/probatio_natura_canones/kebab.canon",
                    piscina);
        CREDO_VERUM (kebab.cucurrit);
        CREDO_AEQUALIS_I32 (kebab.codex, (i32)II);
        CREDO_FALSUM (kebab.plagula_scripta);
        CREDO_VERUM (kebab.clamavit);
    }


    /* ========================================================
     * II. MONOLITHUS ONERATUR
     * ======================================================== */

    {
        chorda causa;

        imprimere("\n--- II. monolithus oneratur ---\n");

        monolithus_fons = filum_legere_totum(
                              "natura/cocta/individua.canon", piscina);
        CREDO_MAIOR_I32 (monolithus_fons.mensura, (i32)ZEPHYRUM);

        causa.datum   = NIHIL;
        causa.mensura = ZEPHYRUM;
        monolithus = canon_legere(monolithus_fons, piscina, intern,
                                  &causa);
        CREDO_NON_NIHIL (monolithus);
        CREDO_CHORDA_VACUA (causa);

        monolithus_res = stml_legere(monolithus_fons, piscina, intern);
        CREDO_VERUM (monolithus_res.successus);
        monolithus_radix = monolithus_res.elementum_radix;
        CREDO_NON_NIHIL (monolithus_radix);
    }

    si (!monolithus || !monolithus_radix)
    {
        imprimere("FRACTA: monolithus legi nequit - cetera omissa\n");
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }


    /* ========================================================
     * III. CITATIONES - PAR, ambae partes necessariae
     *
     * 'nulla citatio irrita' sola nihil probat: canon qui
     * NULLAM citationem fert utrumque documentum acciperet.
     * Quod discernit est documentum PENDENS quod ruere DEBET
     * dum resolvens transit.
     * ======================================================== */

    {
        i32 omnia;
        i32 irrita;

        imprimere("\n--- III. citationes (par) ---\n");

        /* mechanismus omnino vivus est */
        CREDO_MAIOR_I32 (xar_numerus(monolithus->citationes),
                         (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_RESOLVENS,
            CANON_CITATIO_IRRITA, &irrita, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_PENDENS,
            CANON_CITATIO_IRRITA, &irrita, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);

        /* PAR CLAUSURAE: petitum cum posteris. Resolvens clavem
         * sub titulo SUBGENERIS fert - indicem ad= probat (forma
         * titulo solo rueret); pendens ruere DEBET - ante
         * clausuram citatio nulla erat et utraque transibat. */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_CLAUSURA_RESOLVENS, CANON_CITATIO_IRRITA, &irrita,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_CLAUSURA_PENDENS, CANON_CITATIO_IRRITA, &irrita,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);
    }


    /* ========================================================
     * IV. UNICITAS TRANS GENERA
     *
     * Genera DUO DIVERSA idem nomen ferentia: hoc solum probat
     * spatium nominum UNUM esse, non unum per elementum.
     * ======================================================== */

    {
        i32 omnia;
        i32 bis;

        imprimere("\n--- IV. unicitas trans genera ---\n");

        CREDO_MAIOR_I32 (xar_numerus(monolithus->unicitates),
                         (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_GEMINUM_TRANS, CANON_NOMEN_BIS, &bis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (bis, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);

        /* forma facilior, quam spatium per elementum quoque
         * caperet - ideo sola non sufficeret */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_GEMINUM_IDEM, CANON_NOMEN_BIS, &bis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (bis, (i32)I);

        /* et NON clamat ubi nomina distincta sunt (documentum
         * resolvens supra duo nomina diversa fert) */
        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_RESOLVENS,
            CANON_NOMEN_BIS, &bis, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (bis, (i32)ZEPHYRUM);
    }


    /* ========================================================
     * V. CENSUS RELATIONUM se ipsum reconciliat
     *
     * Praefatio numeros ad generationem COMPUTATOS fert.
     * Numerus quem lector credit loco proprii computandi
     * ligatus esse debet ad id quod re vera emissum est -
     * aliter canon de se ipso mentitur, quod peius est quam
     * tacere.
     * ======================================================== */

    {
        i32 sedes;
        i32 citatae;
        i32 nudae;
        i32 apertae;
        i32 multiplices;
        i32 ignotae;

        imprimere("\n--- V. census relationum ---\n");

        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "sedibus relationum", piscina, &sedes));
        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "citationes VERAE", piscina, &citatae));
        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "nomine solo iudicantur", piscina, &nudae));
        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "petitum apertum habent", piscina, &apertae));
        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "multiplices sunt", piscina, &multiplices));
        CREDO_VERUM (_numerum_ante(monolithus_fons,
            "petitum non resolvunt", piscina, &ignotae));

        imprimere("  sedes %u = citatae %u + nudae %u"
                  " (apertae %u + multiplices %u + ignotae %u)\n",
                  sedes, citatae, nudae,
                  apertae, multiplices, ignotae);

        /* causae omnes sedes nudas exhauriunt */
        CREDO_AEQUALIS_I32 (apertae + multiplices + ignotae,
                            nudae);
        /* et nudae + citatae sedes omnes */
        CREDO_AEQUALIS_I32 (citatae + nudae, sedes);

        /* PROSA STRUCTURAE LIGATA: numerus quem praefatio dicit
         * numerus citationum vere emissarum est. Sine hac
         * assertione praefatio libere errare posset - et
         * numerus in capite est id quod lector credit loco
         * canonis ipsius legendi. */
        CREDO_AEQUALIS_I32 (citatae,
                            xar_numerus(monolithus->citationes));
        CREDO_AEQUALIS_I32 (citatae,
                            _titulo_numerare(monolithus_radix,
                                             "citatio"));
    }


    /* ========================================================
     * VI. CITATIO OMNIS AD REM VERAM SPECTAT
     *
     * Opus IV hoc SEMEL manu recensuit; hic manet. Vitium quod
     * capit iam bis vere accidit: citatio in genus emissa quod
     * auctor numquam nominavit, censu tamen ut RESOLUTA
     * numerata (octetus nullus in 'ad='; caecitas 'etiam=').
     * Utrumque tacitum erat.
     * ======================================================== */

    {
        Xar* citationes;
        i32  malae;
        i32  i;

        imprimere("\n--- VI. citationes omnes veras esse ---\n");

        citationes = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        _titulo_colligere(monolithus_radix, "citatio", citationes);
        CREDO_MAIOR_I32 (xar_numerus(citationes), (i32)ZEPHYRUM);

        malae = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(citationes); i++)
        {
            StmlNodus* c;
            StmlNodus* petitum;
            StmlNodus* citans;
            chorda*    ad;
            chorda*    super;
            chorda*    attributum;
            s32        virgula;

            c          = *(StmlNodus**)xar_obtinere(citationes, i);
            ad         = stml_attributum_capere(c, "ad");
            super      = stml_attributum_capere(c, "super");
            attributum = stml_attributum_capere(c, "attributum");

            si (!ad || !super || !attributum)
            {
                malae++;
                perge;
            }

            /* ad="titulus.../attributum" - tituli OMNES indicis
             * resolvi debent et clavem declarare (clausura in
             * genus numquam nominatum emissa = idem vitium quod
             * hoc recensum genuit, forma nova) */
            virgula = chorda_invenire_index(*ad,
                          chorda_ex_literis("/", piscina));
            si (virgula <= ZEPHYRUM)
            {
                malae++;
                perge;
            }

            {
                chorda_fissio_fructus f;
                i32                   k;
                i32                   sani;
                b32                   mala_haec;

                f = chorda_fissio(
                        chorda_sectio(*ad, (i32)ZEPHYRUM,
                                      (i32)virgula),
                        ' ', piscina);
                sani      = ZEPHYRUM;
                mala_haec = FALSUM;
                per (k = ZEPHYRUM; k < f.numerus; k++)
                {
                    chorda t;

                    t = chorda_praecidere(f.elementa[k]);
                    si (t.mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    petitum = _elementum_invenire(monolithus_radix,
                                                  t);
                    si (petitum &&
                        _declarationem_invenire(petitum,
                            "attributum",
                            chorda_sectio(*ad, (i32)(virgula + I),
                                          ad->mensura)))
                    {
                        sani++;
                    }
                    alioquin
                    {
                        mala_haec = VERUM;
                        frange;
                    }
                }
                si (mala_haec || sani == ZEPHYRUM)
                {
                    malae++;
                    perge;
                }
            }

            citans = _elementum_invenire(monolithus_radix, *super);
            si (!citans)
            {
                malae++;
                perge;
            }
            /* et attributum citans in citante declaratum */
            si (!_declarationem_invenire(citans, "attributum",
                                         *attributum))
            {
                malae++;
            }
        }
        imprimere("  citationes %u recensitae, malae %u\n",
                  xar_numerus(citationes), malae);
        CREDO_AEQUALIS_I32 (malae, (i32)ZEPHYRUM);
    }


    /* ========================================================
     * IX. SIGNA - vocabularium et inscriptio (sub monolitho)
     * ======================================================== */

    {
        i32 irrita;
        i32 vocab;
        i32 omnia;

        imprimere("\n--- IX. signa (vocabularium/inscriptio) ---\n");

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_GENUS_REF,
            CANON_VOCABULUM_IGNOTUM, &vocab, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (vocab, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_GENUS_IGNOTUM, CANON_VOCABULUM_IGNOTUM, &vocab,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (vocab, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_INSCRIPTUS,
            CANON_CITATIO_IRRITA, &irrita, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_NON_INSCRIPTUS, CANON_CITATIO_IRRITA, &irrita,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);
    }


    /* ========================================================
     * X. CANON MODULI CITAT (communicatio: actor intra
     *     modulum) et RESTRICTIO (planta: verdicta congruunt)
     * ======================================================== */

    {
        chorda  fons_moduli;
        chorda  causa;
        Canon*  communicatio;
        Canon*  planta;
        i32     irrita;
        i32     mala;
        i32     omnia_m;
        i32     omnia_t;

        imprimere("\n--- X. canon moduli + restrictio ---\n");

        causa.datum   = NIHIL;
        causa.mensura = ZEPHYRUM;
        fons_moduli = filum_legere_totum(
                          "natura/cocta/communicatio.canon",
                          piscina);
        CREDO_MAIOR_I32 (fons_moduli.mensura, (i32)ZEPHYRUM);
        communicatio = canon_legere(fons_moduli, piscina, intern,
                                    &causa);
        CREDO_NON_NIHIL (communicatio);
        CREDO_MAIOR_I32 (xar_numerus(communicatio->citationes),
                         (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(communicatio,
            DOC_NUNTII_SANUS, CANON_CITATIO_IRRITA, &irrita,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(communicatio,
            DOC_NUNTII_PENDENS, CANON_CITATIO_IRRITA, &irrita,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);

        fons_moduli = filum_legere_totum(
                          "natura/cocta/planta.canon", piscina);
        CREDO_MAIOR_I32 (fons_moduli.mensura, (i32)ZEPHYRUM);
        planta = canon_legere(fons_moduli, piscina, intern, &causa);
        CREDO_NON_NIHIL (planta);

        /* restrictio: verdicta TOTA congruunt (omnia, non genus
         * unum) - sub utroque canone */
        CREDO_VERUM (_documentum_iudicare(planta, DOC_PURUS,
            CANON_VALOR_MALUS, &mala, &omnia_m, piscina, intern));
        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_PURUS,
            CANON_VALOR_MALUS, &mala, &omnia_t, piscina, intern));
        CREDO_AEQUALIS_I32 (omnia_m, omnia_t);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(planta,
            DOC_PURUS_VITIOSUS, CANON_VALOR_MALUS, &mala,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (mala, (i32)I);
        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_PURUS_VITIOSUS, CANON_VALOR_MALUS, &mala,
            &omnia_t, piscina, intern));
        CREDO_AEQUALIS_I32 (mala, (i32)I);
        CREDO_AEQUALIS_I32 (omnia_m, omnia_t);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)I);
    }


    /* ========================================================
     * XI. INDIVIDUA NON SUNT TAGS (spec census par. 1)
     * ======================================================== */

    {
        i32    ignota;
        i32    omnia;
        chorda cl;

        imprimere("\n--- XI. individua non tags ---\n");

        cl = chorda_ex_literis("carl-linnaeus", piscina);
        CREDO_NIHIL (_elementum_invenire(monolithus_radix, cl));

        /* foramen impostoris mortuum: tag individui IGNOTUM est
         * (ante: <carl-linnaeus nomen="#impostor"/> licitum erat -
         * tag Linnaeum dicebat, identitas aliud, nemo clamabat) */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <carl-linnaeus nomen=\"#impostor\"/>\n"
            "</individua>\n",
            CANON_ELEMENTUM_IGNOTUM, &ignota, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (ignota, (i32)I);

        /* SEMINA: dictionarium in censum proiectum - data generata
         * sub grammatica generata (porta se-iudicans, spec census
         * par. 5): vitium emissoris in UTROVIS fructu hic rubet */
        {
            chorda semina;

            semina = filum_legere_totum(
                         "natura/cocta/semina.census", piscina);
            CREDO_MAIOR_I32 (semina.mensura, (i32)ZEPHYRUM);
            CREDO_VERUM (chorda_invenire_index(semina,
                chorda_ex_literis(
                    "persona nomen=\"#carl_linnaeus\"",
                    piscina)) >= ZEPHYRUM);

            /* COMPLETUDO, non sola validitas: porta se-iudicans
             * omissionem non videt - semina sine relatis OMNIBUS
             * valida manebat (lacuna mensurata 2026-08-08:
             * genealogia C tacite aberat). Relatum unum attributi
             * (orta-ex) et unum multiplex (auctor) asseruntur. */
            CREDO_VERUM (chorda_invenire_index(semina,
                chorda_ex_literis("orta-ex=\"#b\"",
                                  piscina)) >= ZEPHYRUM);
            CREDO_VERUM (chorda_invenire_index(semina,
                chorda_ex_literis("<auctor ad=\"#dennis_ritchie\"",
                                  piscina)) >= ZEPHYRUM);

            CREDO_VERUM (_documentum_iudicare(monolithus,
                chorda_ut_cstr(semina, piscina),
                CANON_ELEMENTUM_IGNOTUM, &ignota, &omnia,
                piscina, intern));
            /* TOLERA vulnus UNUM: quando="-312" (via_appia,
             * vectura) - dies annos a.C.n. dicere nequit, res
             * 01KZC7F388 APERTA (ictus tertius). Cum regula
             * cadat et vulnus sanetur, haec assertio RUBET et
             * toleratio deletur - id ipsum volumus. */
            CREDO_AEQUALIS_I32 (omnia, (i32)I);
        }
    }


    /* ========================================================
     * VII. LENITAS - NECESSITAS ONTOLOGICA NON EST
     *      OBLIGATIO DOCUMENTALIS (spec par. 3.4)
     *
     * Custodia REGRESSIONIS, et fatemur eam talem esse: haec
     * ANTE quoque praeteriisset, quia generator minimum=
     * numquam emisit. Vim habet quia in fonte DISCRIMEN VERUM
     * iacet quod canon consulto DEPONIT - radix/caulis/folium
     * necessaria sunt, flos/fructus/semen non; in canone
     * INDISCERNIBILES fiunt. Qui necessaria= in minimum="1"
     * verteret hoc par statim frangeret.
     * ======================================================== */

    {
        chorda        genera_fons;
        StmlResultus  genera_res;
        chorda        planta_fons;
        StmlResultus  planta_res;
        StmlNodus*    rosa_canina;
        StmlNodus*    liberum_radix;
        StmlNodus*    liberum_flos;

        imprimere("\n--- VII. lenitas (nullum minimum) ---\n");

        /* (a) FONS discrimen vere fert */
        genera_fons = filum_legere_totum("natura/planta.genera",
                                         piscina);
        CREDO_MAIOR_I32 (genera_fons.mensura, (i32)ZEPHYRUM);
        genera_res = stml_legere(genera_fons, piscina, intern);
        CREDO_VERUM (genera_res.successus);
        CREDO_MAIOR_I32 (_ferentes_numerare(genera_res.elementum_radix,
                             "necessaria", "verum", NIHIL),
                         (i32)ZEPHYRUM);
        CREDO_MAIOR_I32 (_ferentes_numerare(genera_res.elementum_radix,
                             "necessaria", "falsum", NIHIL),
                         (i32)ZEPHYRUM);

        /* (b) CANON id deponit: partes ambae eodem modo emissae */
        planta_fons = filum_legere_totum("natura/cocta/planta.canon",
                                         piscina);
        CREDO_MAIOR_I32 (planta_fons.mensura, (i32)ZEPHYRUM);
        planta_res = stml_legere(planta_fons, piscina, intern);
        CREDO_VERUM (planta_res.successus);

        rosa_canina = _elementum_invenire(planta_res.elementum_radix,
                          chorda_ex_literis("rosa-canina", piscina));
        CREDO_NON_NIHIL (rosa_canina);

        liberum_radix = _declarationem_invenire(rosa_canina, "liberum",
                            chorda_ex_literis("radix", piscina));
        liberum_flos  = _declarationem_invenire(rosa_canina, "liberum",
                            chorda_ex_literis("flos", piscina));
        CREDO_NON_NIHIL (liberum_radix);
        CREDO_NON_NIHIL (liberum_flos);

        si (liberum_radix && liberum_flos)
        {
            /* pars NECESSARIA: minimum nullum */
            CREDO_FALSUM (stml_attributum_habet(liberum_radix,
                                                "minimum"));
            CREDO_VERUM (stml_attributum_habet(liberum_radix,
                                               "maximum"));
            /* pars NON necessaria: forma EADEM */
            CREDO_FALSUM (stml_attributum_habet(liberum_flos,
                                                "minimum"));
            CREDO_VERUM (stml_attributum_habet(liberum_flos,
                                               "maximum"));
        }
    }


    /* ========================================================
     * VII bis. ADSTRICTIO (intra=) - spec par. 4.2
     *
     * Partes et proprietates multiplices intra="<genus>"
     * declarantur, et id NECESSARIUM est, non ornamentum: in
     * corpore vero nomen partis nomini generis congruit
     * (directorium, eventum, proiectio) et nomen proprietatis
     * quoque (ambitus, codex, index, titulus...).
     *
     * NIHIL ALIUD HOC CUSTODIT, et canon custodire NEQUIT:
     * unicitas per DOCUMENTUM currit, ergo 'nomen elementi semel
     * per canonem' inexpressibile est praecise quia intra= nomina
     * repetita licita facit. MENSURATUM: generatore intra=
     * omittente, canon_examen duo <elementum nomen="radix"> exitu
     * 0 accipit.
     *
     * PAR: adstrictum ADEST, et nudum eiusdem nominis ABEST.
     * Prior sola non sufficit - generator intra= omittens
     * elementum adstrictum RELINQUERET et nudum ADDERET.
     * ======================================================== */

    {
        chorda        planta_fons;
        StmlResultus  planta_res;

        imprimere("\n--- VII bis. adstrictio (intra=) ---\n");

        planta_fons = filum_legere_totum("natura/cocta/planta.canon",
                                         piscina);
        planta_res  = stml_legere(planta_fons, piscina, intern);
        CREDO_VERUM (planta_res.successus);

        /* pars 'radix' ad rosam-caninam ADSTRICTA est */
        CREDO_NON_NIHIL (_elementum_adstrictum_invenire(
            planta_res.elementum_radix,
            chorda_ex_literis("radix", piscina),
            chorda_ex_literis("rosa-canina", piscina)));

        /* ...et NUDA nusquam declaratur. Haec est medietas quae
         * regressionem vere capit. */
        CREDO_NIHIL (_elementum_invenire(
            planta_res.elementum_radix,
            chorda_ex_literis("radix", piscina)));

        /* et idem generaliter: nomina nuda per canonem UNICA */
        CREDO_AEQUALIS_I32 (_nuda_gemina_numerare(
                                planta_res.elementum_radix, piscina),
                            (i32)ZEPHYRUM);
    }


    /* ========================================================
     * VIII. CORPUS TOTUM CANONUM COCTORUM
     *
     * Per plagulam: nullum minimum usquam; radix una eaque
     * <individua>; unicitas una; citationes SOLI monolitho.
     *
     * Numerus plagularum non figitur sed DERIVATUR: canones
     * cocti tot sunt quot exemplaria naturae + I (monolithus).
     * Numerus fixus mentiri inciperet quo momento exemplar
     * additur; forma derivata contra exemplar novum cuius
     * canon numquam generatus est CLAMAT.
     * ======================================================== */

    {
        Xar* canones;
        Xar* exemplaria;
        i32  i;
        i32  minima_omnia;
        i32  radices_malae;
        i32  unicitates_malae;
        i32  citantes_per_modulum;
        i32  moduli_citantes;
        i32  monolithi;
        i32  nuda_gemina;
        i32  recensiti;

        imprimere("\n--- VIII. corpus canonum coctorum ---\n");

        canones    = _vias_colligere("natura/cocta", ".canon", piscina);
        exemplaria = _vias_colligere("natura", ".genera", piscina);

        CREDO_MAIOR_I32 (xar_numerus(exemplaria), (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(canones),
                            xar_numerus(exemplaria) + I);

        minima_omnia         = ZEPHYRUM;
        radices_malae        = ZEPHYRUM;
        unicitates_malae     = ZEPHYRUM;
        citantes_per_modulum = ZEPHYRUM;
        moduli_citantes      = ZEPHYRUM;
        monolithi            = ZEPHYRUM;
        nuda_gemina          = ZEPHYRUM;
        recensiti            = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(canones); i++)
        {
            /* piscina PROPRIA per plagulam: arbores XXXIV
             * simul servatae memoriam sine causa tenerent
             * (monolithus solus DCXL kibioctetos fonte est).
             * Nihil ex hac piscina hanc ansam superat. */
            Piscina*             pp;
            InternamentumChorda* pi;
            chorda               via;
            character*           via_c;
            chorda               fons;
            StmlResultus         res;
            StmlNodus*           radix_nodus;
            i32                  minima;
            i32                  radices;
            i32                  unicitates;
            i32                  citationes;
            b32                  est_monolithus;

            via   = *(chorda*)xar_obtinere(canones, i);
            pp    = piscina_generare_dynamicum("probatio_nc_plagula",
                                               2097152);
            si (!pp)
            {
                imprimere("  FRACTA: piscina plagulae\n");
                frange;
            }
            pi    = internamentum_creare(pp);
            via_c = chorda_ut_cstr(via, pp);
            fons  = filum_legere_totum(via_c, pp);
            res   = stml_legere(fons, pp, pi);

            si (!res.successus || !res.elementum_radix)
            {
                imprimere("  FRACTA: '%s' parsari nequit\n", via_c);
                piscina_destruere(pp);
                radices_malae++;
                perge;
            }

            radix_nodus    = NIHIL;
            /* libera SOLA: minimum= in attributo FINES sunt (spec
             * fines, legitima), in libero necessitas (lenitas
             * vetat). Numerator elementum discernit. */
            minima         = _libera_cum_minimo_numerare(
                                 res.elementum_radix);
            radices        = _ferentes_numerare(res.elementum_radix,
                                 "radix", "verum", &radix_nodus);
            unicitates     = _titulo_numerare(res.elementum_radix,
                                 "unicitas");
            citationes     = _titulo_numerare(res.elementum_radix,
                                 "citatio");
            est_monolithus = chorda_terminatur(via,
                                 chorda_ex_literis("/individua.canon",
                                                   pp));

            minima_omnia += minima;

            si (radices != I)
            {
                imprimere("  '%s': radices %u (una exspectata)\n",
                          via_c, radices);
                radices_malae++;
            }
            alioquin
            {
                chorda* n;

                n = stml_attributum_capere(radix_nodus, "nomen");
                si (!n || !chorda_aequalis_literis(*n, "individua"))
                {
                    imprimere("  '%s': radix non 'individua'\n", via_c);
                    radices_malae++;
                }
            }

            si (unicitates != I)
            {
                imprimere("  '%s': unicitates %u (una exspectata)\n",
                          via_c, unicitates);
                unicitates_malae++;
            }

            si (minima > ZEPHYRUM)
            {
                imprimere("  '%s': minimum= %u sedibus\n",
                          via_c, minima);
            }

            /* ADSTRICTIO trans corpus: elementa NUDA nomina unica
             * ferre debent. Generator intra= omittens elementa
             * antea adstricta nuda facit, quae COLLIDUNT - et
             * canon_examen id exitu 0 accipit (mensuratum). */
            {
                i32 gemina;

                gemina = _nuda_gemina_numerare(res.elementum_radix, pp);
                si (gemina > ZEPHYRUM)
                {
                    imprimere("  '%s': nomina nuda gemina %u"
                              " (intra= omissum?)\n", via_c, gemina);
                    nuda_gemina += gemina;
                }
            }

            si (est_monolithus)
            {
                monolithi++;
                si (citationes == ZEPHYRUM)
                {
                    imprimere("  monolithus citationes NULLAS fert\n");
                    citantes_per_modulum++;
                }
            }
            alioquin si (citationes > ZEPHYRUM)
            {
                /* spec canon-referentia par. 6: canon moduli citat
                 * ubi clausura intra modulum tota iacet - numerus
                 * positivus nunc SANITAS est, non vitium */
                moduli_citantes++;
            }

            piscina_destruere(pp);
            recensiti++;
        }

        imprimere("  canones %u ex %u recensiti\n",
                  recensiti, xar_numerus(canones));

        /* TRANSITUS INTEGER, ante cetera: quinque ex sex
         * assertionibus infra accumulatorem cum ZEPHYRO conferunt,
         * et accumulator numquam auctus ZEPHYRUM est - ergo ansa
         * abrupta (piscina defecta: 'frange') OMNES praeteriret,
         * dum summa supra numerum quem legere INTENDIT nuntiaret.
         * Numerus peractorum eum ligat, et omnem formam exitus
         * praemature facti in assertionem RUBRAM vertit, non hanc
         * solam. */
        CREDO_AEQUALIS_I32 (recensiti, xar_numerus(canones));
        /* LENITAS super corpus totum: nusquam <liberum minimum=>
         * (necessitas ontologica non obligatio documentalis).
         * Fines attributorum (spec fines) legitimi, non numerati. */
        CREDO_AEQUALIS_I32 (minima_omnia, (i32)ZEPHYRUM);
        /* 'individua' radix SOLA, in omni canone */
        CREDO_AEQUALIS_I32 (radices_malae, (i32)ZEPHYRUM);
        /* identitas: unicitas una in omni canone */
        CREDO_AEQUALIS_I32 (unicitates_malae, (i32)ZEPHYRUM);
        /* adstrictio (par. 4.2): nomina nuda unica per canonem */
        CREDO_AEQUALIS_I32 (nuda_gemina, (i32)ZEPHYRUM);
        /* monolithus sine citationibus = mechanismus mortuus
         * (accumulator nunc ex ramo monolithi solo augetur) */
        CREDO_AEQUALIS_I32 (citantes_per_modulum, (i32)ZEPHYRUM);
        /* et canones moduli VERE citant (spec canon-referentia
         * par. 6: clausura intra modulum) - communicatio saltem */
        CREDO_MAIOR_I32 (moduli_citantes, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (monolithi, (i32)I);
    }


    /* ========================================================
     * XII. glossae in canones transcriptae (spec glossae par. 4)
     * ======================================================== */

    {
        chorda moduli_canon;
        chorda monolithus;
        chorda semina;

        imprimere("\n--- XII. glossae transcriptae ---\n");

        /* COMPLETUDO, non sola validitas (lex 2026-08-08): porta
         * se-iudicans omissionem non videt - sententiae certae
         * asseruntur, in canone moduli ET in monolitho */
        moduli_canon = filum_legere_totum(
                           "natura/cocta/planta.canon", piscina);
        CREDO_MAIOR_I32 (moduli_canon.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(moduli_canon,
            chorda_ex_literis("<glossa lingua=\"en\">",
                              piscina)) >= ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(moduli_canon,
            chorda_ex_literis("photosynthesis",
                              piscina)) >= ZEPHYRUM);

        monolithus = filum_legere_totum(
                         "natura/cocta/individua.canon", piscina);
        CREDO_MAIOR_I32 (monolithus.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(monolithus,
            chorda_ex_literis("<glossa lingua=\"fr\">",
                              piscina)) >= ZEPHYRUM);

        /* census glossas NUMQUAM fert - docs GENERA describunt,
         * individua data sunt (spec par. 4) */
        semina = filum_legere_totum(
                     "natura/cocta/semina.census", piscina);
        CREDO_MAIOR_I32 (semina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(semina,
            chorda_ex_literis("<glossa", piscina)) < ZEPHYRUM);
    }


    /* ========================================================
     * XIII. fines in canones transcripti (spec fines)
     * ======================================================== */

    {
        chorda moduli_canon;
        Canon* monolithus;
        chorda fons;
        chorda causa;
        i32    generis;
        i32    omnia;

        imprimere("\n--- XIII. fines transcripti ---\n");

        /* COMPLETUDO: sententia certa in canone moduli iudicii
         * (probabilitas periculi - lacuna L4 documentationis) */
        moduli_canon = filum_legere_totum(
                           "natura/cocta/iudicium.canon", piscina);
        CREDO_MAIOR_I32 (moduli_canon.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(moduli_canon,
            chorda_ex_literis(
                "fractio=\"verum\" minimum=\"0\" maximum=\"1\"",
                piscina)) >= ZEPHYRUM);

        /* sonda descriptoris nunc VIRIDIS: decimalis intra fines */
        fons = filum_legere_totum("natura/cocta/individua.canon",
                                  piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        monolithus = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (monolithus);
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <periculum nomen=\"#p\" probabilitas=\"0.7\"/>\n"
            "</individua>\n",
            CANON_VALOR_MALUS, &generis, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        /* et extra fines vitio PROPRIO clamat */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <periculum nomen=\"#p\" probabilitas=\"1.5\"/>\n"
            "</individua>\n",
            CANON_VALOR_EXTRA_FINES, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
