/* canon.c - schema STML generale (gradus II)
 *
 * Vide include/canon.h pro doctrina et pro eo QUOD CANON NON
 * PRAESTAT consulto (citationes, hereditas, co-occurrentiae).
 */
#include "canon.h"
#include "similitudo.h"
#include <string.h>

/* ==================================================
 * Structurae interiores
 * ================================================== */

nomen structura {
    chorda*  titulus;
    i32      minimum;
    /* SIGNATUM CONSULTO: -1 = infinitum. i32 INSIGNATUS est, ergo
     * '-1' in numerum immanem verteretur et 'maximum >= ZEPHYRUM'
     * semper verum esset - rami qui numquam currunt. Uncus examinis
     * id statim nominavit (codex: comparatio vana). */
    s32      maximum;   /* -1 = infinitum */
} CanonLiberum;

nomen structura {
    chorda*            titulus;
    CanonGenusValoris  genus;
    b32                necessarium;
    Xar*               optiones;   /* Xar de chorda* (electio) */
    /* valor praestitutus cum attributum abest (NIHIL = nullus) -
     * attributum STML 'ordinarius=' fert, sed id verbum macro
     * latina est (default), ergo campus 'praestitutum'. Canon eum
     * non APPLICAT (iudex est, non lector) - eum DICIT:
     * documentatio, catalogus generatus, generatio codicis
     * (initiatio structurarum). Conformitas generi in lectione
     * cogitur: praestitutum mendax canonem clamans frangit. */
    chorda*            praestitutum;
    /* FINES (spec fines) - numerus solus: fractio = punctum
     * decimale licet; minimum/maximum INCLUSIVA, forma eadem
     * lexicali scripta (fractio finium formam regit). Constrictio
     * spatii valorum, numquam repraesentatio machinae. */
    b32                fractio;
    chorda*            finis_minimus;
    chorda*            finis_maximus;
} CanonAttributum;

nomen structura {
    chorda*  titulus;
    /* intra= : definitio ad PARENTEM adstricta. NIHIL = globalis.
     * Necessarium quia idem nomen formas duas ferre potest:
     * aedilis <regula> intra <nexus> obiectum= poscit et
     * <vexillum> continet; intra <irregularia> caput= poscit et
     * <obiectum> continet. Declarationes GLOBALES (mos DTD) hoc
     * dicere non possunt; contextus (mos RELAX NG) potest.
     * INVENTUM dialecto SECUNDO describendo - natura eo caruit. */
    chorda*  intra;
    b32      radix;
    b32      textus_licet;
    /* TEXTUS ubi licet genus ferre potest (textus="numerus"):
     * textus non vacuus generi congruere debet. TEXTUS = sine
     * genere (textus="verum" vetus). */
    CanonGenusValoris  textus_genus;
    Xar*     attributa;  /* Xar de CanonAttributum */
    Xar*     liberi;     /* Xar de CanonLiberum */
} CanonElementum;

nomen structura {
    chorda*  titulus;
    chorda*  attributum;
    Xar*     super;      /* Xar de chorda* - genera elementorum */
    /* intra= : scopus per INSTANTIAM parentis nominati - unicitas
     * intra quamque instantiam separatim iudicatur (postulatio
     * quaestionum: parametra intra quaestionem unica, trans
     * quaestiones libere iterata). NIHIL = documentum totum. */
    chorda*  intra;
} CanonUnicitas;

/* citatio: clavis-relatio intra documentum (mos xs:key/keyref).
 * Elementum sub 'super' (aut quodlibet si vacuum) attributum
 * ferens valorem clavis alicuius tituli in 'ad_elementa' (cum
 * 'ad_attributum') aequare debet. Tituli PLURES subsumptionem
 * compilatam ferunt: claves per titulum EXACTUM colliguntur et
 * subgenus titulum alium fert, ergo generator petitum cum
 * posteris omnibus enumerat - canon ipse hereditatem numquam
 * discit. Cum intra=: claves et citantes intra EANDEM instantiam
 * quaeruntur - transitus statum machinae SUAE citat, non alienae. */
nomen structura {
    chorda*  titulus;
    chorda*  attributum;
    Xar*     super;          /* vacuum = elementum quodlibet */
    chorda*  intra;
    Xar*     ad_elementa;    /* Xar de chorda* - tituli clavigeri */
    chorda*  ad_attributum;
} CanonCitatio;

/* ==================================================
 * Prototypa
 * ================================================== */

interior CanonGenusValoris genus_legere(constans chorda* s);
interior b32 valor_congruit(constans chorda* v, CanonAttributum* a);
interior vacuum _numerum_secare(constans chorda* v, b32* negativus,
                                i32* integri_a, i32* integri_n,
                                i32* fracti_a, i32* fracti_n);
interior s32 _magnitudines_comparare(constans chorda* a, i32 ia,
                                     i32 na, i32 fa, i32 nfa,
                                     constans chorda* b, i32 ib,
                                     i32 nb, i32 fb, i32 nfb);
interior s32 _numeros_comparare(constans chorda* a,
                                constans chorda* b);
interior b32 _intra_fines(constans chorda* v, CanonAttributum* a);
interior b32 _super_congruit(StmlNodus* n, constans chorda* t);
interior vacuum vitium_addere(Xar* vitia, CanonVitiumGenus genus,
    StmlNodus* nodus, chorda* elementum, chorda* detail,
    i32 numerus, i32 limes);
interior CanonElementum* elementum_quaerere(Canon* c,
    StmlNodus* n);
interior chorda clavis_scopi(Piscina* p, constans chorda* intra,
    constans chorda* titulus);
interior vacuum nodum_iudicare(Canon* c, StmlNodus* n, Xar* vitia,
    Piscina* piscina);
interior vacuum _augmenta_cusasque_colligere(Canon* c, StmlNodus* n,
    StmlNodus* infixus, Xar* augmenta, TabulaDispersa* cusa,
    Piscina* piscina);
interior vacuum _augmentum_iudicare(Canon* c, StmlNodus* a,
    TabulaDispersa* cusa, Xar* vitia, Piscina* piscina);
interior b32 album_solum(constans chorda* s);
interior vacuum _scopos_colligere(StmlNodus* radix,
    constans chorda* intra, StmlNodus* infixus, Xar* scopi,
    Piscina* piscina);
interior chorda* _suggestio(constans chorda* quaestio,
    constans chorda* candidati, i32 numerus, Piscina* piscina);
interior vacuum _subarborem_colligere(StmlNodus* scopus,
    constans chorda* limes, StmlNodus* infixus, Xar* nodi);

/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
album_solum(
    constans chorda* s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s->mensura; i++)
    {
        character c;

        c = (character)s->datum[i];
        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

interior CanonGenusValoris
genus_legere(
    constans chorda* s)
{
    si (chorda_aequalis_literis(*s, "nomen"))   redde CANON_GENUS_NOMEN;
    si (chorda_aequalis_literis(*s, "numerus")) redde CANON_GENUS_NUMERUS;
    si (chorda_aequalis_literis(*s, "veritas")) redde CANON_GENUS_VERITAS;
    si (chorda_aequalis_literis(*s, "dies"))    redde CANON_GENUS_DIES;
    si (chorda_aequalis_literis(*s, "electio")) redde CANON_GENUS_ELECTIO;
    si (chorda_aequalis_literis(*s, "identitas"))
        redde CANON_GENUS_IDENTITAS;
    si (chorda_aequalis_literis(*s, "referentia"))
        redde CANON_GENUS_REFERENTIA;
    si (chorda_aequalis_literis(*s, "compositum"))
        redde CANON_GENUS_COMPOSITUM;
    si (chorda_aequalis_literis(*s, "titulus"))
        redde CANON_GENUS_TITULUS;

    redde CANON_GENUS_TEXTUS;
}

/* yyyy | yyyy-mm | yyyy-mm-dd */
interior b32
dies_bene_formata(
    constans chorda* d)
{
    i32 i;

    /* annus ante Christum natum (decretum Franis 2026-08-08, res
     * 01KZC7F388): '-' praefixum lectione HISTORICA - '-312' =
     * annus 312 a.C.n., annus zephyrus NON est. ISO 8601
     * astronomicum CONSULTO reiectum: liber omnis '312 a.C.n.'
     * dicit, et lectio uno differens venenum tacitum esset.
     * Annus solus digitis I..IV sine suppletione ('-4' = Herodes
     * moritur); formae cum mense post signum quattuor digitos
     * servant (recursio in reliquum, digito primo custodito ne
     * '--312' signum geminum ferat). */
    si (d->mensura > I &&
        (character)d->datum[ZEPHYRUM] == '-')
    {
        si ((character)d->datum[I] < '0' ||
            (character)d->datum[I] > '9')
        {
            redde FALSUM;
        }
        si (d->mensura <= V)
        {
            per (i = I; i < d->mensura; i++)
            {
                character c;

                c = (character)d->datum[i];
                si (c < '0' || c > '9')
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }
        {
            chorda reliqua;

            reliqua.datum   = d->datum + I;
            reliqua.mensura = d->mensura - I;
            redde dies_bene_formata(&reliqua);
        }
    }

    si (d->mensura != IV && d->mensura != VII && d->mensura != X)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < d->mensura; i++)
    {
        character c;

        c = (character)d->datum[i];
        si (i == IV || i == VII)
        {
            si (c != '-')
            {
                redde FALSUM;
            }
        }
        alioquin si (c < '0' || c > '9')
        {
            redde FALSUM;
        }
    }
    si (d->mensura >= VII)
    {
        i32 m;

        m = (i32)(d->datum[V] - '0') * X + (i32)(d->datum[VI] - '0');
        si (m < I || m > XII)
        {
            redde FALSUM;
        }
    }
    si (d->mensura == X)
    {
        i32 dd;

        dd = (i32)(d->datum[VIII] - '0') * X +
             (i32)(d->datum[IX] - '0');
        si (dd < I || dd > XXXI)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* partes numeri lexicalis secare: signum, digiti integri (zephyris
 * ducentibus demptis), digiti fracti (zephyris caudalibus demptis).
 * Formam SANAM praesumit (valor_congruit ante iudicavit).
 * '-0' == '0': signum sine digitis significantibus tollitur. */
interior vacuum
_numerum_secare(
    constans chorda*  v,
    b32*              negativus,
    i32*              integri_a,
    i32*              integri_n,
    i32*              fracti_a,
    i32*              fracti_n)
{
    i32 initium;
    i32 punctum;
    i32 finis;
    i32 i;

    initium    = ZEPHYRUM;
    *negativus = FALSUM;
    si (v->mensura > ZEPHYRUM)
    {
        character c;

        c = (character)v->datum[ZEPHYRUM];
        si (c == '-')
        {
            *negativus = VERUM;
            initium = I;
        }
        alioquin si (c == '+')
        {
            initium = I;
        }
    }

    punctum = v->mensura;
    per (i = initium; i < v->mensura; i++)
    {
        si ((character)v->datum[i] == '.')
        {
            punctum = i;
            frange;
        }
    }

    dum (initium < punctum &&
         (character)v->datum[initium] == '0')
    {
        initium++;
    }
    *integri_a = initium;
    *integri_n = punctum - initium;

    si (punctum < v->mensura)
    {
        i32 post;

        post  = punctum + I;
        finis = v->mensura;
        dum (finis > post &&
             (character)v->datum[finis - I] == '0')
        {
            finis--;
        }
        *fracti_a = post;
        *fracti_n = finis - post;
    }
    alioquin
    {
        *fracti_a = ZEPHYRUM;
        *fracti_n = ZEPHYRUM;
    }

    si (*integri_n == ZEPHYRUM && *fracti_n == ZEPHYRUM)
    {
        *negativus = FALSUM;
    }
}

interior s32
_magnitudines_comparare(
    constans chorda* a, i32 ia, i32 na, i32 fa, i32 nfa,
    constans chorda* b, i32 ib, i32 nb, i32 fb, i32 nfb)
{
    i32 i;
    i32 maxima;

    si (na != nb)
    {
        redde na < nb ? -I : I;
    }
    per (i = ZEPHYRUM; i < na; i++)
    {
        character ca;
        character cb;

        ca = (character)a->datum[ia + i];
        cb = (character)b->datum[ib + i];
        si (ca != cb)
        {
            redde ca < cb ? -I : I;
        }
    }
    maxima = nfa > nfb ? nfa : nfb;
    per (i = ZEPHYRUM; i < maxima; i++)
    {
        character ca;
        character cb;

        ca = i < nfa ? (character)a->datum[fa + i] : '0';
        cb = i < nfb ? (character)b->datum[fb + i] : '0';
        si (ca != cb)
        {
            redde ca < cb ? -I : I;
        }
    }
    redde ZEPHYRUM;
}

/* numeros lexicales SINE fluitantibus comparare: negativum si a<b,
 * zephyrum si aequales, positivum si a>b. Formas sanas praesumit -
 * comparatio digitorum, numquam conversio. */
interior s32
_numeros_comparare(
    constans chorda*  a,
    constans chorda*  b)
{
    b32 neg_a;
    b32 neg_b;
    i32 ia; i32 na; i32 fa; i32 nfa;
    i32 ib; i32 nb; i32 fb; i32 nfb;
    s32 ordo;

    _numerum_secare(a, &neg_a, &ia, &na, &fa, &nfa);
    _numerum_secare(b, &neg_b, &ib, &nb, &fb, &nfb);

    si (neg_a != neg_b)
    {
        redde neg_a ? -I : I;
    }
    ordo = _magnitudines_comparare(a, ia, na, fa, nfa,
                                   b, ib, nb, fb, nfb);
    redde neg_a ? -ordo : ordo;
}

/* congruentia super=: 'titulus' = elementum quodlibet eo titulo;
 * 'parens/titulus' = elementum eo titulo SUB parente illo (spec
 * super-adstrictum). Liberi multiplicati nomen trans parentes
 * communicant clausuris diversis - via sola eos discernit; ante
 * eam generator multiplicia citare non poterat et sedes CDII
 * nomine solo iudicabantur. */
interior b32
_super_congruit(
    StmlNodus*        n,
    constans chorda*  t)
{
    b32 inventus;
    i32 solidus;
    i32 i;

    inventus = FALSUM;
    solidus  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        si ((character)t->datum[i] == '/')
        {
            solidus  = i;
            inventus = VERUM;
            frange;
        }
    }
    si (!inventus)
    {
        redde chorda_aequalis(*n->titulus, *t);
    }

    {
        chorda pars_parentis;
        chorda pars_liberi;

        pars_parentis = chorda_sectio(*t, ZEPHYRUM, solidus);
        pars_liberi   = chorda_sectio(*t, solidus + I, t->mensura);
        si (!chorda_aequalis(*n->titulus, pars_liberi))
        {
            redde FALSUM;
        }
        si (!n->parens ||
            n->parens->genus != STML_NODUS_ELEMENTUM ||
            !n->parens->titulus)
        {
            redde FALSUM;
        }
        redde chorda_aequalis(*n->parens->titulus, pars_parentis);
    }
}

/* an valor (formae iam sanae) intra fines declaratos sit - numerus
 * solus fines fert (lectio canonis id cogit) */
interior b32
_intra_fines(
    constans chorda*  v,
    CanonAttributum*  a)
{
    si (a->genus != CANON_GENUS_NUMERUS)
    {
        redde VERUM;
    }
    si (a->finis_minimus &&
        _numeros_comparare(v, a->finis_minimus) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (a->finis_maximus &&
        _numeros_comparare(v, a->finis_maximus) > ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
valor_congruit(
    constans chorda*  v,
    CanonAttributum*  a)
{
    i32 i;

    commutatio (a->genus)
    {
        casus CANON_GENUS_TEXTUS:
            redde VERUM;

        casus CANON_GENUS_NOMEN:
        casus CANON_GENUS_COMPOSITUM:
        casus CANON_GENUS_TITULUS:
        {
            i32 initium;

            /* titulus: punctum UNUM ducens licitum ('.species'),
             * corpus dein compositum; punctum solum vetatur */
            initium = ZEPHYRUM;
            si (a->genus == CANON_GENUS_TITULUS &&
                v->mensura > I &&
                (character)v->datum[ZEPHYRUM] == '.')
            {
                initium = I;
            }

            si (v->mensura == initium)
            {
                redde FALSUM;
            }
            per (i = initium; i < v->mensura; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z') ||
                      (c >= '0' && c <= '9') ||
                      c == '_' || c == '*' ||
                      (c == '-' &&
                       a->genus != CANON_GENUS_NOMEN)))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }

        casus CANON_GENUS_NUMERUS:
        {
            b32 punctum_visum;
            b32 digitus_ante;
            b32 digitus_post;

            si (v->mensura == ZEPHYRUM)
            {
                redde FALSUM;
            }
            punctum_visum = FALSUM;
            digitus_ante  = FALSUM;
            digitus_post  = FALSUM;
            per (i = ZEPHYRUM; i < v->mensura; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (i == ZEPHYRUM && (c == '-' || c == '+'))
                {
                    perge;
                }
                si (c == '.' && a->fractio && !punctum_visum &&
                    digitus_ante)
                {
                    punctum_visum = VERUM;
                    perge;
                }
                si (c < '0' || c > '9')
                {
                    redde FALSUM;
                }
                si (punctum_visum)
                {
                    digitus_post = VERUM;
                }
                alioquin
                {
                    digitus_ante = VERUM;
                }
            }
            si (!digitus_ante)
            {
                redde FALSUM;
            }
            si (punctum_visum && !digitus_post)
            {
                redde FALSUM;
            }
            redde VERUM;
        }

        casus CANON_GENUS_VERITAS:
            redde (b32)(chorda_aequalis_literis(*v, "verum") ||
                        chorda_aequalis_literis(*v, "falsum"));

        casus CANON_GENUS_DIES:
            redde dies_bene_formata(v);

        casus CANON_GENUS_ELECTIO:
            per (i = ZEPHYRUM; i < xar_numerus(a->optiones); i++)
            {
                chorda** o;

                o = (chorda**)xar_obtinere(a->optiones, i);
                si (chorda_aequalis(*v, **o))
                {
                    redde VERUM;
                }
            }
            redde FALSUM;

        casus CANON_GENUS_IDENTITAS:
        casus CANON_GENUS_REFERENTIA:
        {
            character signum;
            i32       finis_corporis;

            si (v->mensura < II)
            {
                redde FALSUM;   /* signum sine corpore, aut nihil */
            }
            signum = (character)v->datum[ZEPHYRUM];

            /* decretum 2026-08-10: '&nomen;' = individuum
             * (spatium mundi - syntaxis quam STML_SPEC ipsa
             * reservavit: singleton nominatum, semantica
             * wikilink); '.genus' = vocabularium (referentia
             * sola). Signum vetus '#' spatio DOCUMENTI
             * (fragmenta STML) redditum est - valor
             * '#'-praefixus hic vitium est, numquam litterale
             * tacitum (custos migrationis). Terminator ';'
             * obligatorius ubique: prosam tutam facit (AT&T
             * litterale manet quia ';' deest) et grammaticam
             * UNAM inter attributa et textum servat. */
            si (signum == '&')
            {
                si (v->mensura < III ||
                    (character)v->datum[v->mensura - I] != ';')
                {
                    redde FALSUM;
                }
                finis_corporis = v->mensura - I;
            }
            alioquin si (a->genus == CANON_GENUS_REFERENTIA &&
                         signum == '.')
            {
                finis_corporis = v->mensura;
            }
            alioquin
            {
                redde FALSUM;
            }

            per (i = I; i < finis_corporis; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z') ||
                      (c >= '0' && c <= '9') ||
                      c == '_' || c == '*' || c == '-'))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }

        ordinarius:
            redde VERUM;
    }
}

interior vacuum
vitium_addere(
    Xar*              vitia,
    CanonVitiumGenus  genus,
    StmlNodus*        nodus,
    chorda*           elementum,
    chorda*           detail,
    i32               numerus,
    i32               limes)
{
    CanonVitium* v;

    v = (CanonVitium*)xar_addere(vitia);
    si (!v)
    {
        redde;
    }

    v->genus     = genus;
    v->nodus     = nodus;
    v->elementum = elementum;
    v->detail    = detail;
    v->numerus   = numerus;
    v->limes     = limes;
}

/* 'an X?' pro nomine ignoto - similitudo_optima_decurtata quia
 * pro NOMINIBUS TOTIS MALE CONIECTIS nata est (cauda decurtatur
 * donec candidati appareant - errores transpositionis capit quos
 * subsequentia nuda non potest). NIHIL si nihil simile. */
interior chorda*
_suggestio(
    constans chorda*  quaestio,
    constans chorda*  candidati,
    i32               numerus,
    Piscina*          piscina)
{
    SimilitudoFructus fructus;
    chorda*           d;
    constans chorda*  optimus;

    si (numerus == ZEPHYRUM ||
        similitudo_optima_decurtata(*quaestio, candidati, numerus,
                                    &fructus, I) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    optimus = &candidati[fructus.index];

    d = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    d->mensura = III + optimus->mensura + I;
    d->datum = (i8*)piscina_allocare(piscina,
                                     (memoriae_index)d->mensura);
    memcpy(d->datum, "an ", III);
    memcpy(d->datum + III, optimus->datum,
           (memoriae_index)optimus->mensura);
    d->datum[d->mensura - I] = (i8)'?';

    redde d;
}

/* clavis tabulae: "parens/nomen" pro definitione adstricta */
interior chorda
clavis_scopi(
    Piscina*          p,
    constans chorda*  intra,
    constans chorda*  titulus)
{
    chorda k;

    k.mensura = intra->mensura + I + titulus->mensura;
    k.datum   = (i8*)piscina_allocare(p, (memoriae_index)k.mensura);
    memcpy(k.datum, intra->datum, (memoriae_index)intra->mensura);
    k.datum[intra->mensura] = (i8)'/';
    memcpy(k.datum + intra->mensura + I, titulus->datum,
           (memoriae_index)titulus->mensura);

    redde k;
}

/* Definitionem ADSTRICTAM prius quaerere, deinde globalem:
 * specialius vincit. */
interior CanonElementum*
elementum_quaerere(
    Canon*      c,
    StmlNodus*  n)
{
    vacuum* valor;

    si (n->parens && n->parens->genus == STML_NODUS_ELEMENTUM &&
        n->parens->titulus)
    {
        PiscinaNotatio nota;
        chorda         k;
        b32            inventum;

        nota = piscina_notare(c->piscina);
        k = clavis_scopi(c->piscina, n->parens->titulus, n->titulus);
        inventum = tabula_dispersa_invenire(c->elementa, k, &valor);
        piscina_reficere(c->piscina, nota);
        si (inventum)
        {
            redde (CanonElementum*)valor;
        }
    }

    si (!tabula_dispersa_invenire(c->elementa, *n->titulus, &valor))
    {
        redde NIHIL;
    }

    redde (CanonElementum*)valor;
}

/* instantias scopi colligere: intra NIHIL = radix sola (scopus
 * documenti); aliter POSTERI OMNES titulo intra (etiam nidificati
 * - instantia interior scopus suus est), subarbore infixi
 * praetermissa */
interior vacuum
_scopos_colligere(
    StmlNodus*        radix,
    constans chorda*  intra,
    StmlNodus*        infixus,
    Xar*              scopi,
    Piscina*          piscina)
{
    Xar* acervus;
    i32  j;

    si (!intra)
    {
        StmlNodus** locus;

        locus = (StmlNodus**)xar_addere(scopi);
        *locus = radix;
        redde;
    }

    acervus = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    {
        StmlNodus** locus;

        locus = (StmlNodus**)xar_addere(acervus);
        *locus = radix;
    }

    per (j = ZEPHYRUM; j < xar_numerus(acervus); j++)
    {
        StmlNodus* n;
        i32        numerus;
        i32        k;

        n = *(StmlNodus**)xar_obtinere(acervus, j);
        si (n->titulus && chorda_aequalis(*n->titulus, *intra))
        {
            StmlNodus** locus;

            locus = (StmlNodus**)xar_addere(scopi);
            *locus = n;
        }

        numerus = stml_numerus_liberorum(n);
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            StmlNodus*  l;
            StmlNodus** locus;

            l = stml_liberum_ad_indicem(n, k);
            si (!l || l == infixus ||
                l->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            locus = (StmlNodus**)xar_addere(acervus);
            *locus = l;
        }
    }
}

/* elementa subarboris scopi colligere (scopo ipso incluso).
 * LIMES: in instantias ALIAS tituli 'limes' non descendit -
 * scopus interior claves suas ipse tenet, exterior eas non
 * videt. Subarbor infixi semper praetermissa. */
interior vacuum
_subarborem_colligere(
    StmlNodus*        scopus,
    constans chorda*  limes,
    StmlNodus*        infixus,
    Xar*              nodi)
{
    i32 j;

    {
        StmlNodus** locus;

        locus = (StmlNodus**)xar_addere(nodi);
        *locus = scopus;
    }

    per (j = ZEPHYRUM; j < xar_numerus(nodi); j++)
    {
        StmlNodus* n;
        i32        numerus;
        i32        k;

        n = *(StmlNodus**)xar_obtinere(nodi, j);
        numerus = stml_numerus_liberorum(n);
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            StmlNodus*  l;
            StmlNodus** locus;

            l = stml_liberum_ad_indicem(n, k);
            si (!l || l == infixus ||
                l->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            si (limes && l->titulus &&
                chorda_aequalis(*l->titulus, *limes))
            {
                perge;   /* instantia interior: scopus suus */
            }
            locus = (StmlNodus**)xar_addere(nodi);
            *locus = l;
        }
    }
}

StmlNodus*
canon_infixum_invenire(
    StmlNodus* elementum_radix)
{
    i32 numerus;
    i32 i;

    si (!elementum_radix ||
        elementum_radix->genus != STML_NODUS_ELEMENTUM)
    {
        redde NIHIL;
    }

    numerus = stml_numerus_liberorum(elementum_radix);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(elementum_radix, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;   /* textus albus et commentaria non numerant */
        }
        si (l->titulus &&
            chorda_aequalis_literis(*l->titulus, "canon"))
        {
            redde l;
        }
        redde NIHIL;   /* liberum elementare PRIMUM solum */
    }

    redde NIHIL;
}

/* ==================================================
 * Lectio canonis
 * ================================================== */

Canon*
canon_legere(
    chorda                fons,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    chorda*               causa)
{
    StmlResultus r;

    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        si (causa)
        {
            *causa = chorda_ex_literis("canon parsari nequit",
                                       piscina);
        }
        redde NIHIL;
    }

    redde canon_ex_nodo(r.elementum_radix, piscina, intern, causa);
}

Canon*
canon_ex_nodo(
    StmlNodus*            elementum,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    chorda*               causa)
{
    Canon*  c;
    i32     numerus;
    i32     i;

    si (causa)
    {
        causa->datum   = NIHIL;
        causa->mensura = ZEPHYRUM;
    }

    si (!elementum ||
        elementum->genus != STML_NODUS_ELEMENTUM ||
        !elementum->titulus ||
        !chorda_aequalis_literis(*elementum->titulus, "canon"))
    {
        si (causa)
        {
            *causa = chorda_ex_literis("radix non est <canon>",
                                       piscina);
        }
        redde NIHIL;
    }

    c = (Canon*)piscina_allocare(piscina, magnitudo(Canon));
    c->piscina    = piscina;
    c->intern     = intern;
    c->dialectus  = stml_attributum_capere(elementum, "dialectus");
    c->versio     = stml_attributum_capere(elementum, "versio");
    c->elementa   = tabula_dispersa_creare_chorda(piscina, LXIV);
    c->unicitates = xar_creare(piscina, (i32)magnitudo(CanonUnicitas*));
    c->citationes = xar_creare(piscina, (i32)magnitudo(CanonCitatio*));
    c->tituli     = xar_creare(piscina, (i32)magnitudo(chorda*));
    c->radix      = NIHIL;
    c->claves_externae = NIHIL;
    c->claves_fons     = NIHIL;

    numerus = stml_numerus_liberorum(elementum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* n;
        chorda*    titulus;

        n = stml_liberum_ad_indicem(elementum, i);
        si (!n || n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        /* claves externae (librarium W1): involucrum SINE nomen= -
         * ante custodem infra tractandum, ne tacite caderet (custos
         * :infra liberos innominatos praeterit) */
        si (chorda_aequalis_literis(*n->titulus, "claves-externae"))
        {
            i32 nl;
            i32 j;

            c->claves_fons = stml_attributum_capere(n, "fons");
            si (!c->claves_externae)
            {
                c->claves_externae = tabula_dispersa_creare_chorda(
                    piscina, CXXVIII);
            }
            nl = stml_numerus_liberorum(n);
            per (j = ZEPHYRUM; j < nl; j++)
            {
                StmlNodus*          cl;
                CanonClavisExterna* ce;
                chorda              textus;

                cl = stml_liberum_ad_indicem(n, j);
                si (!cl || cl->genus != STML_NODUS_ELEMENTUM ||
                    !cl->titulus ||
                    !chorda_aequalis_literis(*cl->titulus, "clavis"))
                {
                    perge;
                }
                /* textus VERBATIM = clavis sigillata (&nomen; -
                 * entia ignota litteralia manent, ergo octeta
                 * eadem quae comparatio citationum videt) */
                textus = stml_textus_internus(cl, piscina);
                si (textus.mensura == ZEPHYRUM)
                {
                    perge;
                }
                ce = (CanonClavisExterna*)piscina_allocare(piscina,
                    magnitudo(CanonClavisExterna));
                ce->clavis = chorda_internare(intern, textus);
                ce->genus  = stml_attributum_capere(cl, "genus");
                tabula_dispersa_inserere(c->claves_externae,
                                         *ce->clavis, ce);
            }
            perge;
        }

        titulus = stml_attributum_capere(n, "nomen");
        si (!titulus)
        {
            perge;
        }

        si (chorda_aequalis_literis(*n->titulus, "elementum"))
        {
            CanonElementum* e;
            chorda*         attr;
            i32             nl;
            i32             j;

            e = (CanonElementum*)piscina_allocare(piscina,
                magnitudo(CanonElementum));
            e->titulus      = titulus;
            e->attributa    = xar_creare(piscina,
                                (i32)magnitudo(CanonAttributum));
            e->liberi       = xar_creare(piscina,
                                (i32)magnitudo(CanonLiberum));
            e->intra = stml_attributum_capere(n, "intra");
            attr = stml_attributum_capere(n, "radix");
            e->radix = (b32)(attr &&
                chorda_aequalis_literis(*attr, "verum"));
            /* textus=: verum = licet sine genere; genus nominatum
             * (numerus/dies/...) = licet et congruere debet;
             * absens aut ignotus = vetitus. 'electio' pro textu
             * mechanismum optionum non habet - ut verum tractatur
             * (canon.canon eam ex electione sua excludit). */
            e->textus_licet = FALSUM;
            e->textus_genus = CANON_GENUS_TEXTUS;
            attr = stml_attributum_capere(n, "textus");
            si (attr)
            {
                si (chorda_aequalis_literis(*attr, "verum"))
                {
                    e->textus_licet = VERUM;
                }
                alioquin
                {
                    CanonGenusValoris g;

                    g = genus_legere(attr);
                    si (g == CANON_GENUS_ELECTIO)
                    {
                        e->textus_licet = VERUM;
                    }
                    alioquin si (g != CANON_GENUS_TEXTUS)
                    {
                        e->textus_licet = VERUM;
                        e->textus_genus = g;
                    }
                }
            }

            si (e->radix)
            {
                c->radix = titulus;
            }
            {
                chorda** locus_t;

                locus_t = (chorda**)xar_addere(c->tituli);
                *locus_t = titulus;
            }

            nl = stml_numerus_liberorum(n);
            per (j = ZEPHYRUM; j < nl; j++)
            {
                StmlNodus* l;
                chorda*    lt;

                l = stml_liberum_ad_indicem(n, j);
                si (!l || l->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                lt = stml_attributum_capere(l, "nomen");
                si (!lt)
                {
                    perge;
                }

                si (chorda_aequalis_literis(*l->titulus,
                                            "attributum"))
                {
                    CanonAttributum* a;
                    chorda*          g;
                    i32              no;
                    i32              k;

                    a = (CanonAttributum*)xar_addere(e->attributa);
                    a->titulus  = lt;
                    g = stml_attributum_capere(l, "genus");
                    a->genus = g ? genus_legere(g)
                                 : CANON_GENUS_TEXTUS;
                    g = stml_attributum_capere(l, "necessarium");
                    a->necessarium = (b32)(g &&
                        chorda_aequalis_literis(*g, "verum"));
                    a->optiones = xar_creare(piscina,
                                    (i32)magnitudo(chorda*));

                    no = stml_numerus_liberorum(l);
                    per (k = ZEPHYRUM; k < no; k++)
                    {
                        StmlNodus* o;
                        chorda**   locus;
                        chorda     t;

                        o = stml_liberum_ad_indicem(l, k);
                        si (!o ||
                            o->genus != STML_NODUS_ELEMENTUM ||
                            !chorda_aequalis_literis(*o->titulus,
                                                     "optio"))
                        {
                            perge;
                        }
                        t = chorda_praecidere(
                            stml_textus_internus(o, piscina));
                        locus = (chorda**)xar_addere(a->optiones);
                        *locus = chorda_internare(intern, t);
                    }

                    /* FINES ante ordinarium (spec fines): numerus
                     * solus eos fert; ipsi formae sanae et recto
                     * ordine esse debent - canon mendax de se ipso
                     * fractura clamans est, non documentatio. */
                    g = stml_attributum_capere(l, "fractio");
                    a->fractio = (b32)(g &&
                        chorda_aequalis_literis(*g, "verum"));
                    a->finis_minimus =
                        stml_attributum_capere(l, "minimum");
                    a->finis_maximus =
                        stml_attributum_capere(l, "maximum");
                    si ((a->fractio || a->finis_minimus ||
                         a->finis_maximus) &&
                        a->genus != CANON_GENUS_NUMERUS)
                    {
                        si (causa)
                        {
                            *causa = chorda_ex_literis(
                                "fines generi non numerico",
                                piscina);
                        }
                        redde NIHIL;
                    }
                    si ((a->finis_minimus &&
                         !valor_congruit(a->finis_minimus, a)) ||
                        (a->finis_maximus &&
                         !valor_congruit(a->finis_maximus, a)))
                    {
                        si (causa)
                        {
                            *causa = chorda_ex_literis(
                                "finis formae pravae", piscina);
                        }
                        redde NIHIL;
                    }
                    si (a->finis_minimus && a->finis_maximus &&
                        _numeros_comparare(a->finis_minimus,
                                           a->finis_maximus) >
                            ZEPHYRUM)
                    {
                        si (causa)
                        {
                            *causa = chorda_ex_literis(
                                "fines inversi", piscina);
                        }
                        redde NIHIL;
                    }

                    /* ordinarius= post optiones et fines legendos:
                     * valor praestitutus contra AMBO iudicatur.
                     * Praestitutum generi non congruens = canon
                     * mendax de se ipso - fractura clamans, non
                     * documentatio falsa tacita. */
                    a->praestitutum =
                        stml_attributum_capere(l, "ordinarius");
                    si (a->praestitutum &&
                        (!valor_congruit(a->praestitutum, a) ||
                         !_intra_fines(a->praestitutum, a)))
                    {
                        si (causa)
                        {
                            *causa = chorda_ex_literis(
                                "ordinarius generi attributi "
                                "non congruit", piscina);
                        }
                        redde NIHIL;
                    }
                }
                alioquin si (chorda_aequalis_literis(*l->titulus,
                                                     "liberum"))
                {
                    CanonLiberum* lb;
                    chorda*       m;

                    lb = (CanonLiberum*)xar_addere(e->liberi);
                    lb->titulus = lt;
                    lb->minimum = ZEPHYRUM;
                    lb->maximum = -I;

                    m = stml_attributum_capere(l, "minimum");
                    si (m)
                    {
                        i32 val;

                        si (chorda_ut_i32(*m, &val))
                        {
                            lb->minimum = val;
                        }
                    }
                    m = stml_attributum_capere(l, "maximum");
                    si (m)
                    {
                        i32 val;

                        si (chorda_ut_i32(*m, &val))
                        {
                            lb->maximum = (s32)val;
                        }
                    }
                }
            }

            si (e->intra)
            {
                tabula_dispersa_inserere(c->elementa,
                    clavis_scopi(piscina, e->intra, titulus), e);
            }
            alioquin
            {
                tabula_dispersa_inserere(c->elementa, *titulus, e);
            }
        }
        alioquin si (chorda_aequalis_literis(*n->titulus,
                                             "citatio"))
        {
            CanonCitatio*  ci;
            CanonCitatio** locus;
            chorda*        ad;
            chorda*        super;

            ci = (CanonCitatio*)piscina_allocare(piscina,
                magnitudo(CanonCitatio));
            ci->titulus       = titulus;
            ci->attributum    = stml_attributum_capere(n,
                                                       "attributum");
            ci->intra         = stml_attributum_capere(n, "intra");
            ci->super         = xar_creare(piscina,
                                    (i32)magnitudo(chorda*));
            ci->ad_elementa   = NIHIL;
            ci->ad_attributum = NIHIL;

            /* ad="elementum.../attributum" - solidus OBLIGATORIUS,
             * tituli spatiis separati ante eum (unus = forma
             * simplex). Forma mala canonem totum frangit (clamans),
             * ne citatio muta nihil custodiat custodire visa. */
            ad = stml_attributum_capere(n, "ad");
            si (ad)
            {
                s32 solidus;   /* -1 = absens; i32 signum voraret */
                i32 k;

                solidus = -I;
                per (k = ZEPHYRUM; k < ad->mensura; k++)
                {
                    si ((character)ad->datum[k] == '/')
                    {
                        solidus = (s32)k;
                        frange;
                    }
                }
                si (solidus <= ZEPHYRUM ||
                    (i32)solidus >= ad->mensura - I)
                {
                    si (causa)
                    {
                        *causa = chorda_ex_literis(
                            "citatio 'ad' sine solido "
                            "(forma: elementum/attributum)",
                            piscina);
                    }
                    redde NIHIL;
                }
                {
                    chorda                pars;
                    chorda_fissio_fructus f;
                    i32                   m;

                    pars = chorda_sectio(*ad, ZEPHYRUM,
                                         (i32)solidus);
                    ci->ad_elementa = xar_creare(piscina,
                        (i32)magnitudo(chorda*));
                    f = chorda_fissio(pars, ' ', piscina);
                    per (m = ZEPHYRUM; m < f.numerus; m++)
                    {
                        chorda   t;
                        chorda** locus_t;

                        t = chorda_praecidere(f.elementa[m]);
                        si (t.mensura == ZEPHYRUM)
                        {
                            perge;
                        }
                        locus_t = (chorda**)xar_addere(
                            ci->ad_elementa);
                        *locus_t = chorda_internare(intern, t);
                    }
                    /* album solum ante solidum: clamare, non
                     * tacere - citatio sine clavigero nihil
                     * custodiret custodire visa */
                    si (xar_numerus(ci->ad_elementa) == ZEPHYRUM)
                    {
                        si (causa)
                        {
                            *causa = chorda_ex_literis(
                                "citatio 'ad' sine titulis ante "
                                "solidum (forma: "
                                "elementum.../attributum)",
                                piscina);
                        }
                        redde NIHIL;
                    }
                }
                ci->ad_attributum = chorda_internare(intern,
                    chorda_sectio(*ad, (i32)(solidus + I),
                                  ad->mensura));
            }

            super = stml_attributum_capere(n, "super");
            si (super)
            {
                chorda_fissio_fructus f;
                i32                   k;

                f = chorda_fissio(*super, ' ', piscina);
                per (k = ZEPHYRUM; k < f.numerus; k++)
                {
                    chorda   t;
                    chorda** locus_s;

                    t = chorda_praecidere(f.elementa[k]);
                    si (t.mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    locus_s = (chorda**)xar_addere(ci->super);
                    *locus_s = chorda_internare(intern, t);
                }
            }

            locus = (CanonCitatio**)xar_addere(c->citationes);
            *locus = ci;
        }
        alioquin si (chorda_aequalis_literis(*n->titulus,
                                             "unicitas"))
        {
            CanonUnicitas*  u;
            CanonUnicitas** locus;
            chorda*         super;

            u = (CanonUnicitas*)piscina_allocare(piscina,
                magnitudo(CanonUnicitas));
            u->titulus    = titulus;
            u->attributum = stml_attributum_capere(n, "attributum");
            u->intra      = stml_attributum_capere(n, "intra");
            u->super      = xar_creare(piscina,
                                (i32)magnitudo(chorda*));

            super = stml_attributum_capere(n, "super");
            si (super)
            {
                chorda_fissio_fructus f;
                i32                   k;

                f = chorda_fissio(*super, ' ', piscina);
                per (k = ZEPHYRUM; k < f.numerus; k++)
                {
                    chorda   t;
                    chorda** locus_s;

                    t = chorda_praecidere(f.elementa[k]);
                    si (t.mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    locus_s = (chorda**)xar_addere(u->super);
                    *locus_s = chorda_internare(intern, t);
                }
            }

            locus = (CanonUnicitas**)xar_addere(c->unicitates);
            *locus = u;
        }
    }

    redde c;
}

/* ==================================================
 * Iudicium
 * ================================================== */

/* claves domi cusas + augmentationes colligere transitu UNO.
 * Cusio = valor attributi generis IDENTITAS in elemento noto.
 * Subarbor infixi praetermissa; in augmenta DESCENDIT (cusio intra
 * augmentum rara sed legalis; augmenta nidificata quaeque suo iure
 * colliguntur). */
interior vacuum
_augmenta_cusasque_colligere(
    Canon*           c,
    StmlNodus*       n,
    StmlNodus*       infixus,
    Xar*             augmenta,
    TabulaDispersa*  cusa,
    Piscina*         piscina)
{
    i32 i;
    i32 numerus;

    si (!n || n == infixus || n->genus != STML_NODUS_ELEMENTUM)
    {
        redde;
    }

    si (n->augmentum_clavis)
    {
        StmlNodus** slot;

        slot = (StmlNodus**)xar_addere(augmenta);
        si (slot)
        {
            *slot = n;
        }
    }
    alioquin
    {
        CanonElementum* e;

        e = elementum_quaerere(c, n);
        si (e)
        {
            per (i = ZEPHYRUM; i < xar_numerus(e->attributa); i++)
            {
                CanonAttributum* def;
                chorda*          v;

                def = (CanonAttributum*)xar_obtinere(e->attributa,
                                                     i);
                si (def->genus != CANON_GENUS_IDENTITAS)
                {
                    perge;
                }
                v = stml_attributum_capere(n,
                    chorda_ut_cstr(*def->titulus, piscina));
                si (v && v->mensura > ZEPHYRUM)
                {
                    tabula_dispersa_inserere(cusa, *v, n);
                }
            }
        }
    }

    numerus = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _augmenta_cusasque_colligere(c,
            stml_liberum_ad_indicem(n, i), infixus, augmenta,
            cusa, piscina);
    }
}

/* augmentationem unam iudicare: clavis in scopo (cusae domi ∪
 * claves externae), genus destinatum per genus= clavis externae
 * aut per elementum nodi cudentis, membra ADDITIVA sola - liberum
 * sine maximo legale (recursione plena iudicatum), attributum aut
 * liberum adstrictum = pugna, ignotum = vitium ordinarium. */
interior vacuum
_augmentum_iudicare(
    Canon*           c,
    StmlNodus*       a,
    TabulaDispersa*  cusa,
    Xar*             vitia,
    Piscina*         piscina)
{
    CanonElementum* genus_def;
    vacuum*         valor;
    i32             i;
    i32             numerus;

    genus_def = NIHIL;

    si (tabula_dispersa_invenire(cusa, *a->augmentum_clavis,
                                 &valor))
    {
        /* clavis domi cusa: genus = elementum nodi cudentis. Nodus
         * cudens extra canonem iam suo loco clamavit - hic tacemus */
        genus_def = elementum_quaerere(c, (StmlNodus*)valor);
        si (!genus_def)
        {
            redde;
        }
    }
    alioquin si (c->claves_externae &&
                 tabula_dispersa_invenire(c->claves_externae,
                                          *a->augmentum_clavis,
                                          &valor))
    {
        CanonClavisExterna* ce;

        ce = (CanonClavisExterna*)valor;
        si (ce->genus &&
            tabula_dispersa_invenire(c->elementa, *ce->genus,
                                     &valor))
        {
            genus_def = (CanonElementum*)valor;
        }
        si (!genus_def)
        {
            /* clavis nota, elementum generis absens - bibliotheca
             * ipsa manca (genus= extra canonem) */
            vitium_addere(vitia, CANON_ELEMENTUM_IGNOTUM, a,
                          ce->genus ? ce->genus : a->titulus,
                          a->augmentum_clavis, ZEPHYRUM, ZEPHYRUM);
            redde;
        }
    }
    alioquin
    {
        /* augmentatio inexsistentis */
        vitium_addere(vitia, CANON_CITATIO_IRRITA, a, a->titulus,
                      a->augmentum_clavis, ZEPHYRUM, ZEPHYRUM);
        redde;
    }

    numerus = stml_numerus_liberorum(a);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        b32        legale;
        b32        pugna;
        i32        j;

        l = stml_liberum_ad_indicem(a, i);
        si (!l)
        {
            perge;
        }
        si (l->genus == STML_NODUS_TEXTUS)
        {
            si (l->valor && !album_solum(l->valor))
            {
                vitium_addere(vitia, CANON_TEXTUS_ILLICITUS, l,
                              a->titulus, NIHIL, ZEPHYRUM,
                              ZEPHYRUM);
            }
            perge;
        }
        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (l->augmentum_clavis)
        {
            /* nidificata suo iure iudicatur (collecta transitu
             * eodem) */
            perge;
        }

        legale = FALSUM;
        pugna  = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(genus_def->liberi); j++)
        {
            CanonLiberum* lb;

            lb = (CanonLiberum*)xar_obtinere(genus_def->liberi, j);
            si (chorda_aequalis(*lb->titulus, *l->titulus))
            {
                si (lb->maximum < ZEPHYRUM)
                {
                    legale = VERUM;   /* multiplex - additivum */
                }
                alioquin
                {
                    pugna = VERUM;    /* adstrictum - singulare */
                }
                frange;
            }
        }
        si (!legale && !pugna)
        {
            per (j = ZEPHYRUM;
                 j < xar_numerus(genus_def->attributa); j++)
            {
                CanonAttributum* def;

                def = (CanonAttributum*)xar_obtinere(
                    genus_def->attributa, j);
                si (chorda_aequalis(*def->titulus, *l->titulus))
                {
                    pugna = VERUM;    /* attributum - singulare */
                    frange;
                }
            }
        }

        si (pugna)
        {
            vitium_addere(vitia, CANON_AUGMENTUM_PUGNANS, l,
                          genus_def->titulus, l->titulus,
                          ZEPHYRUM, ZEPHYRUM);
        }
        alioquin si (!legale)
        {
            vitium_addere(vitia, CANON_LIBERUM_ILLICITUM, l,
                          genus_def->titulus, l->titulus,
                          ZEPHYRUM, ZEPHYRUM);
        }
        alioquin
        {
            nodum_iudicare(c, l, vitia, piscina);
        }
    }
}

interior vacuum
nodum_iudicare(
    Canon*      c,
    StmlNodus*  n,
    Xar*        vitia,
    Piscina*    piscina)
{
    CanonElementum* e;
    StmlNodus*      infixus;
    i32             numerus;
    i32             i;

    si (!n || n->genus != STML_NODUS_ELEMENTUM || !n->titulus)
    {
        redde;
    }

    /* augmentatio passu proprio iudicatur (canon_iudicare) - hic
     * numquam vocabulario mensuranda */
    si (n->augmentum_clavis)
    {
        redde;
    }

    /* canon infixus (liberum primum radicis) contractus est, non
     * contentum: pro liberis, licentia, textu INVISIBILIS - alibi
     * eum canon_examen contra canonem canonum iudicat. Radix =
     * elementum sine parente elementari. */
    infixus = NIHIL;
    si (!n->parens || n->parens->genus != STML_NODUS_ELEMENTUM)
    {
        infixus = canon_infixum_invenire(n);
    }

    e = elementum_quaerere(c, n);
    si (!e)
    {
        chorda* sug;

        sug = NIHIL;
        si (xar_numerus(c->tituli) > ZEPHYRUM)
        {
            chorda* acies;
            i32     na;
            i32     ia;

            na = xar_numerus(c->tituli);
            acies = (chorda*)piscina_allocare(piscina,
                magnitudo(chorda) * (memoriae_index)na);
            per (ia = ZEPHYRUM; ia < na; ia++)
            {
                acies[ia] = **(chorda**)xar_obtinere(c->tituli,
                                                     ia);
            }
            sug = _suggestio(n->titulus, acies, na, piscina);
        }
        vitium_addere(vitia, CANON_ELEMENTUM_IGNOTUM, n,
                      n->titulus, sug, ZEPHYRUM, ZEPHYRUM);
        redde;   /* ignotum: liberi eius non iudicantur */
    }

    /* ---- attributa praesentia ---- */
    per (i = ZEPHYRUM; i < xar_numerus(n->attributa); i++)
    {
        StmlAttributum*  a;
        CanonAttributum* def;
        i32              j;

        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        def = NIHIL;
        per (j = ZEPHYRUM; j < xar_numerus(e->attributa); j++)
        {
            CanonAttributum* cand;

            cand = (CanonAttributum*)xar_obtinere(e->attributa, j);
            si (chorda_aequalis(*cand->titulus, *a->titulus))
            {
                def = cand;
                frange;
            }
        }

        si (!def)
        {
            chorda* detail;
            chorda* sug;

            detail = a->titulus;
            sug = NIHIL;
            si (xar_numerus(e->attributa) > ZEPHYRUM)
            {
                chorda* acies;
                i32     na;
                i32     ia;

                na = xar_numerus(e->attributa);
                acies = (chorda*)piscina_allocare(piscina,
                    magnitudo(chorda) * (memoriae_index)na);
                per (ia = ZEPHYRUM; ia < na; ia++)
                {
                    acies[ia] = *((CanonAttributum*)
                        xar_obtinere(e->attributa, ia))->titulus;
                }
                sug = _suggestio(a->titulus, acies, na, piscina);
            }
            si (sug)
            {
                /* "nomen (an X?)" - nomen ignotum SERVATUR */
                detail = (chorda*)piscina_allocare(piscina,
                    magnitudo(chorda));
                detail->mensura = a->titulus->mensura + II +
                                  sug->mensura + I;
                detail->datum = (i8*)piscina_allocare(piscina,
                    (memoriae_index)detail->mensura);
                memcpy(detail->datum, a->titulus->datum,
                       (memoriae_index)a->titulus->mensura);
                memcpy(detail->datum + a->titulus->mensura,
                       " (", II);
                memcpy(detail->datum + a->titulus->mensura + II,
                       sug->datum, (memoriae_index)sug->mensura);
                detail->datum[detail->mensura - I] = (i8)')';
            }
            vitium_addere(vitia, CANON_ATTRIBUTUM_IGNOTUM, n,
                          e->titulus, detail, ZEPHYRUM,
                          ZEPHYRUM);
            perge;
        }
        si (!valor_congruit(a->valor, def))
        {
            vitium_addere(vitia, CANON_VALOR_MALUS, n, e->titulus,
                          a->titulus, ZEPHYRUM, ZEPHYRUM);
        }
        alioquin si (!_intra_fines(a->valor, def))
        {
            vitium_addere(vitia, CANON_VALOR_EXTRA_FINES, n,
                          e->titulus, a->titulus, ZEPHYRUM,
                          ZEPHYRUM);
        }
    }

    /* ---- attributa necessaria ---- */
    per (i = ZEPHYRUM; i < xar_numerus(e->attributa); i++)
    {
        CanonAttributum* def;

        def = (CanonAttributum*)xar_obtinere(e->attributa, i);
        si (def->necessarium &&
            !stml_attributum_capere(n, chorda_ut_cstr(*def->titulus,
                                                      piscina)))
        {
            vitium_addere(vitia, CANON_ATTRIBUTUM_DEEST, n,
                          e->titulus, def->titulus, ZEPHYRUM,
                          ZEPHYRUM);
        }
    }

    /* ---- textus typatus: non vacuus generi congruere debet ---- */
    si (e->textus_licet && e->textus_genus != CANON_GENUS_TEXTUS)
    {
        chorda textus_totus;

        textus_totus = chorda_praecidere(
            stml_textus_normalizatus(n, piscina));
        si (textus_totus.mensura > ZEPHYRUM)
        {
            CanonAttributum tmp;

            tmp.titulus       = NIHIL;
            tmp.genus         = e->textus_genus;
            tmp.necessarium   = FALSUM;
            tmp.optiones      = NIHIL;
            tmp.praestitutum  = NIHIL;
            /* fines in textu typato v1 NON stant (spec fines) */
            tmp.fractio       = FALSUM;
            tmp.finis_minimus = NIHIL;
            tmp.finis_maximus = NIHIL;
            si (!valor_congruit(&textus_totus, &tmp))
            {
                chorda* d;

                d = (chorda*)piscina_allocare(piscina,
                                              magnitudo(chorda));
                *d = textus_totus;
                vitium_addere(vitia, CANON_TEXTUS_MALUS, n,
                              e->titulus, d, ZEPHYRUM, ZEPHYRUM);
            }
        }
    }

    /* ---- liberi: licentia et cardinalitas ---- */
    numerus = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < xar_numerus(e->liberi); i++)
    {
        CanonLiberum* lb;
        i32           computa;
        i32           j;

        lb = (CanonLiberum*)xar_obtinere(e->liberi, i);
        computa = ZEPHYRUM;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(n, j);
            si (l && l != infixus &&
                l->genus == STML_NODUS_ELEMENTUM &&
                chorda_aequalis(*l->titulus, *lb->titulus))
            {
                computa++;
            }
        }

        si (computa < lb->minimum)
        {
            vitium_addere(vitia, CANON_LIBERI_PAUCI, n, e->titulus,
                          lb->titulus, computa, lb->minimum);
        }
        si (lb->maximum >= ZEPHYRUM && computa > (i32)lb->maximum)
        {
            vitium_addere(vitia, CANON_LIBERI_MULTI, n, e->titulus,
                          lb->titulus, computa, (i32)lb->maximum);
        }
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        b32        licet;
        i32        j;

        l = stml_liberum_ad_indicem(n, i);
        si (!l || l == infixus)
        {
            perge;
        }

        si (l->genus == STML_NODUS_TEXTUS)
        {
            si (!e->textus_licet && l->valor &&
                !album_solum(l->valor))
            {
                vitium_addere(vitia, CANON_TEXTUS_ILLICITUS, l,
                              e->titulus, NIHIL, ZEPHYRUM,
                              ZEPHYRUM);
            }
            perge;
        }
        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        /* augmentatio '<% &clavis;>' vocabulario INVISIBILIS (sicut
         * infixus): nec licentia nec cardinalitas eam vident. Passus
         * proprius in canon_iudicare eam contra genus destinatum
         * iudicat - cusa clavium prius TOTA colligenda. */
        si (l->augmentum_clavis)
        {
            perge;
        }

        licet = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
        {
            CanonLiberum* lb;

            lb = (CanonLiberum*)xar_obtinere(e->liberi, j);
            si (chorda_aequalis(*lb->titulus, *l->titulus))
            {
                licet = VERUM;
                frange;
            }
        }
        si (!licet)
        {
            vitium_addere(vitia, CANON_LIBERUM_ILLICITUM, l,
                          e->titulus, l->titulus, ZEPHYRUM,
                          ZEPHYRUM);
        }

        nodum_iudicare(c, l, vitia, piscina);
    }
}

Xar*
canon_iudicare(
    Canon*      canon,
    StmlNodus*  radix,
    Piscina*    piscina)
{
    Xar*       vitia;
    StmlNodus* elementum_radix;
    i32        i;

    si (!canon || !piscina)
    {
        redde NIHIL;
    }

    vitia = xar_creare(piscina, (i32)magnitudo(CanonVitium));
    si (!radix)
    {
        redde vitia;
    }

    /* nodum documenti aut elementum accipimus */
    elementum_radix = radix;
    si (radix->genus != STML_NODUS_ELEMENTUM)
    {
        i32 numerus;

        elementum_radix = NIHIL;
        numerus = stml_numerus_liberorum(radix);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(radix, i);
            si (l && l->genus == STML_NODUS_ELEMENTUM)
            {
                elementum_radix = l;
                frange;
            }
        }
    }
    si (!elementum_radix)
    {
        redde vitia;
    }

    si (canon->radix &&
        !chorda_aequalis(*elementum_radix->titulus, *canon->radix))
    {
        vitium_addere(vitia, CANON_RADIX_MALA, elementum_radix,
                      elementum_radix->titulus, canon->radix,
                      ZEPHYRUM, ZEPHYRUM);
    }

    nodum_iudicare(canon, elementum_radix, vitia, piscina);

    /* ---- unicitates et citationes: per SCOPOS iudicantur.
     * Sine intra= scopus = documentum (mos vetus); cum intra=
     * quaeque instantia scopus suus est (parametra intra
     * quaestionem, status intra machinam). Subarbor infixi
     * ubique praetermissa. ---- */
    {
        StmlNodus* infixus;

        infixus = canon_infixum_invenire(elementum_radix);

        per (i = ZEPHYRUM; i < xar_numerus(canon->unicitates); i++)
        {
            CanonUnicitas*      u;
            Xar*                scopi;
            constans character* attr_cstr;
            i32                 is;

            u = *(CanonUnicitas**)xar_obtinere(canon->unicitates,
                                               i);
            si (!u->attributum)
            {
                perge;
            }
            attr_cstr = chorda_ut_cstr(*u->attributum, piscina);

            scopi = xar_creare(piscina,
                               (i32)magnitudo(StmlNodus*));
            _scopos_colligere(elementum_radix, u->intra, infixus,
                              scopi, piscina);

            per (is = ZEPHYRUM; is < xar_numerus(scopi); is++)
            {
                StmlNodus*      s;
                TabulaDispersa* visa;
                Xar*            nodi;
                i32             j;

                s = *(StmlNodus**)xar_obtinere(scopi, is);
                visa = tabula_dispersa_creare_chorda(piscina,
                                                     XXXII);
                nodi = xar_creare(piscina,
                                  (i32)magnitudo(StmlNodus*));
                _subarborem_colligere(s, u->intra, infixus, nodi);

                per (j = ZEPHYRUM; j < xar_numerus(nodi); j++)
                {
                    StmlNodus* n;
                    b32        sub;
                    i32        m;
                    chorda*    v;

                    n = *(StmlNodus**)xar_obtinere(nodi, j);
                    sub = FALSUM;
                    per (m = ZEPHYRUM; m < xar_numerus(u->super);
                         m++)
                    {
                        chorda** t;

                        t = (chorda**)xar_obtinere(u->super, m);
                        si (chorda_aequalis(*n->titulus, **t))
                        {
                            sub = VERUM;
                            frange;
                        }
                    }
                    si (!sub)
                    {
                        perge;
                    }

                    v = stml_attributum_capere(n, attr_cstr);
                    si (!v)
                    {
                        perge;
                    }
                    si (tabula_dispersa_continet(visa, *v))
                    {
                        vitium_addere(vitia, CANON_NOMEN_BIS,
                            n, n->titulus, v, ZEPHYRUM, ZEPHYRUM);
                    }
                    alioquin
                    {
                        tabula_dispersa_inserere(visa, *v, n);
                    }
                }
            }
        }

        per (i = ZEPHYRUM; i < xar_numerus(canon->citationes); i++)
        {
            CanonCitatio*       ci;
            Xar*                scopi;
            constans character* attr_cstr;
            constans character* ad_attr_cstr;
            i32                 is;

            ci = *(CanonCitatio**)xar_obtinere(canon->citationes,
                                               i);
            /* citatio manca (attributa absentia) INERS relinquitur
             * hic - canon.canon eam in ipso canone clamat */
            si (!ci->attributum || !ci->ad_elementa ||
                !ci->ad_attributum)
            {
                perge;
            }
            attr_cstr    = chorda_ut_cstr(*ci->attributum, piscina);
            ad_attr_cstr = chorda_ut_cstr(*ci->ad_attributum,
                                          piscina);

            scopi = xar_creare(piscina,
                               (i32)magnitudo(StmlNodus*));
            _scopos_colligere(elementum_radix, ci->intra, infixus,
                              scopi, piscina);

            per (is = ZEPHYRUM; is < xar_numerus(scopi); is++)
            {
                StmlNodus*      s;
                TabulaDispersa* claves;
                Xar*            nodi;
                i32             j;

                s = *(StmlNodus**)xar_obtinere(scopi, is);
                claves = tabula_dispersa_creare_chorda(piscina,
                                                       XXXII);
                nodi = xar_creare(piscina,
                                  (i32)magnitudo(StmlNodus*));
                _subarborem_colligere(s, ci->intra, infixus, nodi);

                /* passus I: claves scopi colligere - nodus
                 * clavigerus est si titulus eius in indice
                 * ad_elementa stat (idem mos ac super= infra) */
                per (j = ZEPHYRUM; j < xar_numerus(nodi); j++)
                {
                    StmlNodus* n;
                    chorda*    v;
                    b32        clavigerum;
                    i32        m;

                    n = *(StmlNodus**)xar_obtinere(nodi, j);
                    clavigerum = FALSUM;
                    per (m = ZEPHYRUM;
                         m < xar_numerus(ci->ad_elementa); m++)
                    {
                        chorda** t;

                        t = (chorda**)xar_obtinere(
                            ci->ad_elementa, m);
                        si (chorda_aequalis(*n->titulus, **t))
                        {
                            clavigerum = VERUM;
                            frange;
                        }
                    }
                    si (!clavigerum)
                    {
                        perge;
                    }
                    v = stml_attributum_capere(n, ad_attr_cstr);
                    si (v)
                    {
                        tabula_dispersa_inserere(claves, *v, n);
                    }
                }

                /* passus II: citantes contra claves */
                per (j = ZEPHYRUM; j < xar_numerus(nodi); j++)
                {
                    StmlNodus* n;
                    chorda*    v;

                    n = *(StmlNodus**)xar_obtinere(nodi, j);
                    si (xar_numerus(ci->super) > ZEPHYRUM)
                    {
                        b32 sub;
                        i32 m;

                        sub = FALSUM;
                        per (m = ZEPHYRUM;
                             m < xar_numerus(ci->super); m++)
                        {
                            chorda** t;

                            t = (chorda**)xar_obtinere(ci->super,
                                                       m);
                            si (_super_congruit(n, *t))
                            {
                                sub = VERUM;
                                frange;
                            }
                        }
                        si (!sub)
                        {
                            perge;
                        }
                    }

                    v = stml_attributum_capere(n, attr_cstr);
                    si (!v || v->mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    si ((character)v->datum[ZEPHYRUM] == '.')
                    {
                        /* referentia generis: vocabularium ex
                         * indice ad_elementa - clavibus nihil
                         * opus, ergo etiam in canone moduli
                         * currit (spec canon-referentia par. 5) */
                        chorda corpus;
                        b32    notum;
                        i32    mv;

                        corpus = chorda_sectio(*v, I, v->mensura);
                        notum  = FALSUM;
                        per (mv = ZEPHYRUM;
                             mv < xar_numerus(ci->ad_elementa);
                             mv++)
                        {
                            chorda** t;

                            t = (chorda**)xar_obtinere(
                                ci->ad_elementa, mv);
                            si (chorda_aequalis(corpus, **t))
                            {
                                notum = VERUM;
                                frange;
                            }
                        }
                        si (!notum)
                        {
                            chorda* sug;
                            i32     nv;

                            sug = NIHIL;
                            nv  = xar_numerus(ci->ad_elementa);
                            si (nv > ZEPHYRUM)
                            {
                                chorda* acies_v;
                                i32     iv;

                                acies_v = (chorda*)
                                    piscina_allocare(piscina,
                                        magnitudo(chorda) *
                                        (memoriae_index)nv);
                                per (iv = ZEPHYRUM; iv < nv; iv++)
                                {
                                    acies_v[iv] = **(chorda**)
                                        xar_obtinere(
                                            ci->ad_elementa, iv);
                                }
                                sug = _suggestio(&corpus, acies_v,
                                                 nv, piscina);
                            }
                            vitium_addere(vitia,
                                CANON_VOCABULUM_IGNOTUM, n,
                                n->titulus, sug ? sug : v,
                                ZEPHYRUM, ZEPHYRUM);
                        }
                    }
                    alioquin si (!tabula_dispersa_continet(claves,
                                                           *v) &&
                                 !(canon->claves_externae &&
                                   tabula_dispersa_continet(
                                       canon->claves_externae, *v)))
                    {
                        /* clavis verbatim, signo incluso ('&x;'
                         * post migrationem signorum; forma nuda
                         * canonum sine signis eadem via). Scopus =
                         * claves documenti + claves externae
                         * bibliothecae (librarium W1) */
                        vitium_addere(vitia, CANON_CITATIO_IRRITA,
                            n, n->titulus, v, ZEPHYRUM, ZEPHYRUM);
                    }
                }
            }
        }

        /* ---- augmentationes: <% &clavis;> contra genus
         * destinatum. Passus proprius: cusa clavium domi TOTA
         * prius colligenda (cusio post augmentum in ordine
         * documenti legalis). Scopus idem ac citationum: cusae
         * domi + claves externae bibliothecae (librarium W3). ---- */
        {
            Xar*            augmenta;
            TabulaDispersa* cusa;
            i32             ia;

            augmenta = xar_creare(piscina,
                                  (i32)magnitudo(StmlNodus*));
            cusa = tabula_dispersa_creare_chorda(piscina, XXXII);
            _augmenta_cusasque_colligere(canon, elementum_radix,
                                         infixus, augmenta, cusa,
                                         piscina);
            per (ia = ZEPHYRUM; ia < xar_numerus(augmenta); ia++)
            {
                _augmentum_iudicare(canon,
                    *(StmlNodus**)xar_obtinere(augmenta, ia),
                    cusa, vitia, piscina);
            }
        }
    }

    redde vitia;
}

constans character*
canon_nuntius(
    CanonVitiumGenus genus)
{
    commutatio (genus)
    {
        casus CANON_ELEMENTUM_IGNOTUM:
            redde "elementum extra canonem";
        casus CANON_ATTRIBUTUM_IGNOTUM:
            redde "attributum extra canonem";
        casus CANON_ATTRIBUTUM_DEEST:
            redde "attributum necessarium deest";
        casus CANON_VALOR_MALUS:
            redde "valor generi attributi non congruit";
        casus CANON_LIBERUM_ILLICITUM:
            redde "liberum hic non licet";
        casus CANON_LIBERI_PAUCI:
            redde "liberi pauciores quam minimum";
        casus CANON_LIBERI_MULTI:
            redde "liberi plures quam maximum";
        casus CANON_TEXTUS_ILLICITUS:
            redde "textus ubi non licet";
        casus CANON_TEXTUS_MALUS:
            redde "textus generi elementi non congruit";
        casus CANON_CITATIO_IRRITA:
            redde "citatio non resoluta (clavis absens in scopo)";
        casus CANON_NOMEN_BIS:
            redde "nomen bis in spatio unico";
        casus CANON_RADIX_MALA:
            redde "elementum radicis aliud quam canon poscit";
        casus CANON_VOCABULUM_IGNOTUM:
            redde "vocabulum extra petitum citationis";
        casus CANON_VALOR_EXTRA_FINES:
            redde "valor extra fines declaratos";
        casus CANON_AUGMENTUM_PUGNANS:
            redde "augmentatio membri singularis "
                  "(bibliothecam ipsam muta, non documentum)";
        ordinarius:
            redde "vitium ignotum";
    }
}

/* ==================================================
 * Registrum
 * ================================================== */

chorda
canon_registrum_quaerere_radice(
    chorda            catalogus,
    constans chorda*  radix,
    Piscina*          piscina)
{
    chorda                vacua;
    chorda_fissio_fructus lineae;
    i32                   i;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (!radix || radix->mensura == ZEPHYRUM)
    {
        redde vacua;
    }

    lineae = chorda_fissio(catalogus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda                linea;
        chorda_fissio_fructus campi;

        linea = chorda_praecidere(lineae.elementa[i]);
        si (linea.mensura == ZEPHYRUM ||
            (character)linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }

        campi = chorda_fissio(linea, '\t', piscina);
        si (campi.numerus < II)
        {
            perge;
        }
        {
            chorda clavis;

            clavis = chorda_praecidere(campi.elementa[ZEPHYRUM]);
            si (clavis.mensura >= III &&
                (character)clavis.datum[ZEPHYRUM] == '<' &&
                (character)clavis.datum[clavis.mensura - I] == '>')
            {
                chorda intus;

                intus = chorda_sectio(clavis, I,
                                      clavis.mensura - I);
                si (chorda_aequalis(intus, *radix))
                {
                    redde chorda_praecidere(campi.elementa[I]);
                }
            }
        }
    }

    redde vacua;
}

chorda
canon_registrum_quaerere(
    chorda               catalogus,
    constans character*  via_plagulae,
    Piscina*             piscina)
{
    chorda                vacua;
    chorda_fissio_fructus lineae;
    constans character*   punctum;
    i32                   i;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (!via_plagulae)
    {
        redde vacua;
    }
    punctum = strrchr(via_plagulae, '.');
    si (!punctum)
    {
        redde vacua;
    }

    lineae = chorda_fissio(catalogus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda                linea;
        chorda_fissio_fructus campi;

        linea = chorda_praecidere(lineae.elementa[i]);
        si (linea.mensura == ZEPHYRUM ||
            (character)linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }

        campi = chorda_fissio(linea, '\t', piscina);
        si (campi.numerus < II)
        {
            perge;
        }
        {
            chorda ext;
            chorda via;

            ext = chorda_praecidere(campi.elementa[ZEPHYRUM]);
            via = chorda_praecidere(campi.elementa[I]);
            si (chorda_aequalis_literis(ext, punctum))
            {
                redde via;
            }
        }
    }

    redde vacua;
}
