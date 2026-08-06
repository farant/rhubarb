/* natura.c - onerator bibliothecae semanticae
 *
 * Vide include/natura.h pro doctrina (proiectio, phases duae,
 * profunditas hybrida). Vocabularium clausum et regulae ex
 * natura/METAMODULUS.md par.3/4/8 sumpta sunt.
 */
#include "natura.h"
#include <string.h>

/* ==================================================
 * Regula VIII MIGRAVIT (2026-08-06): vocabularium clausum nunc
 * natura/natura.canon dicit et natura_examen per lib/canon.c
 * iudicat. Onerator plagulam CANONE SANAM praesumit: elementa
 * ignota tacite percurrit, ens sine nomine tacite praeterit
 * (canon utrumque clamat - ATTRIBUTUM_DEEST, ELEMENTUM_IGNOTUM).
 * Ante migrationem vocabularium QUATER stabat (hic, visus,
 * METAMODULUS, canon); nunc semel.
 * ================================================== */

/* sentinella homonymiae in tabula nominum nudorum */
interior NaturaEns SENTINELLA_HOMONYMA;

#define NATURA_CATENA_MAXIMA  LXIV

/* ==================================================
 * Prototypa interiora
 * ================================================== */

interior vacuum diagnosticum_addere(
    NaturaBibliotheca* bib, NaturaDiagnosticumGradus gradus,
    i32 regula, chorda* modulus, chorda* ens,
    constans character* nuntius);
interior chorda clavis_entis(
    Piscina* piscina, constans chorda* modulus,
    constans chorda* titulus);
interior vacuum ens_registrare(
    NaturaBibliotheca* bib, NaturaExemplar* ex,
    NaturaEnsDiscrimen discrimen, vacuum* corpus,
    chorda* titulus);
interior vacuum arborem_legere(
    NaturaBibliotheca* bib, NaturaExemplar* ex,
    StmlNodus* nodus, NaturaGenus* ambiens);
interior StmlNodus* in_catena_invenire(
    NaturaGenus* genus, constans character* continens,
    constans character* elementum, chorda* quaesitum);
interior StmlNodus* in_membris_invenire(
    NaturaGenus* primus, NaturaGenus* etiam,
    constans character* continens, constans character* elementum,
    chorda* quaesitum);
interior b32 dies_bene_formata(constans chorda* d);
interior vacuum qualificationes_probare(
    NaturaBibliotheca* bib, NaturaExemplar* ex,
    Xar* gradus_noti, StmlNodus* nodus, chorda* ens);
interior vacuum arborem_nectere(
    NaturaBibliotheca* bib, NaturaExemplar* ex, Xar* gradus_noti,
    StmlNodus* nodus, NaturaGenus* genus_c, NaturaRes* res_c);
interior NaturaEns* ens_quaerere(
    NaturaBibliotheca* bib, constans chorda* modulus,
    constans chorda* titulus);
interior vacuum apparatui_contribuere(
    Xar* apparatus, NaturaGenus* genus);

/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
diagnosticum_addere(
    NaturaBibliotheca*        bib,
    NaturaDiagnosticumGradus  gradus,
    i32                       regula,
    chorda*                   modulus,
    chorda*                   ens,
    constans character*       nuntius)
{
    NaturaDiagnosticum* d;

    d = (NaturaDiagnosticum*)xar_addere(bib->diagnostica);
    si (!d)
    {
        redde;
    }

    d->gradus  = gradus;
    d->regula  = regula;
    d->modulus = modulus;
    d->ens     = ens;
    d->nuntius = nuntius;
}

/* clavis addressae: "modulus/nomen" */
interior chorda
clavis_entis(
    Piscina*         piscina,
    constans chorda* modulus,
    constans chorda* titulus)
{
    chorda clavis;

    clavis.mensura = modulus->mensura + I + titulus->mensura;
    clavis.datum   = (i8*)piscina_allocare(
        piscina, (memoriae_index)clavis.mensura);

    memcpy(clavis.datum, modulus->datum,
           (memoriae_index)modulus->mensura);
    clavis.datum[modulus->mensura] = (i8)'/';
    memcpy(clavis.datum + modulus->mensura + I, titulus->datum,
           (memoriae_index)titulus->mensura);

    redde clavis;
}

interior NaturaEns*
ens_quaerere(
    NaturaBibliotheca* bib,
    constans chorda*   modulus,
    constans chorda*   titulus)
{
    PiscinaNotatio nota;
    chorda         clavis;
    vacuum*        valor;
    b32            inventum;

    /* clavis temporaria - piscina statim reficitur */
    nota   = piscina_notare(bib->piscina);
    clavis = clavis_entis(bib->piscina, modulus, titulus);
    inventum = tabula_dispersa_invenire(bib->entia, clavis, &valor);
    piscina_reficere(bib->piscina, nota);

    si (!inventum)
    {
        redde NIHIL;
    }

    redde (NaturaEns*)valor;
}

/* ==================================================
 * Creatio
 * ================================================== */

NaturaBibliotheca*
natura_bibliotheca_creare(
    Piscina* piscina)
{
    NaturaBibliotheca* bib;

    si (!piscina)
    {
        redde NIHIL;
    }

    bib = (NaturaBibliotheca*)piscina_allocare(
        piscina, magnitudo(NaturaBibliotheca));

    bib->piscina      = piscina;
    bib->intern       = internamentum_creare(piscina);
    bib->exemplaria   = xar_creare(piscina, (i32)magnitudo(NaturaExemplar*));
    bib->genera_omnia = xar_creare(piscina, (i32)magnitudo(NaturaGenus*));
    bib->res_omnes    = xar_creare(piscina, (i32)magnitudo(NaturaRes*));
    bib->entia        = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    bib->nomina       = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    bib->diagnostica  = xar_creare(piscina,
                                   (i32)magnitudo(NaturaDiagnosticum));
    bib->nexum        = FALSUM;

    si (!bib->intern || !bib->exemplaria || !bib->genera_omnia ||
        !bib->res_omnes || !bib->entia || !bib->nomina ||
        !bib->diagnostica)
    {
        redde NIHIL;
    }

    redde bib;
}

/* ==================================================
 * Phasis I - legere
 * ================================================== */

interior vacuum
ens_registrare(
    NaturaBibliotheca*  bib,
    NaturaExemplar*     ex,
    NaturaEnsDiscrimen  discrimen,
    vacuum*             corpus,
    chorda*             titulus)
{
    NaturaEns* ens;
    chorda     clavis;
    vacuum*    prius;

    ens = (NaturaEns*)piscina_allocare(
        bib->piscina, magnitudo(NaturaEns));
    ens->discrimen = discrimen;
    ens->corpus    = corpus;

    /* Regula XV MIGRAVIT ad canonem (2026-08-06): unicitas
     * 'entia' in natura.canon eam dicit, examen eam ut regulam
     * XV praesentat. Primum-vincit hic manet: ens geminum
     * registrari nequit (integritas graphi, non diagnostica). */
    clavis = clavis_entis(bib->piscina, ex->stirps, titulus);
    si (tabula_dispersa_continet(bib->entia, clavis))
    {
        redde;
    }
    tabula_dispersa_inserere(bib->entia, clavis, ens);

    /* nomen nudum: homonymia trans exemplaria -> sentinella */
    si (tabula_dispersa_invenire(bib->nomina, *titulus, &prius))
    {
        tabula_dispersa_inserere(bib->nomina, *titulus,
                                 &SENTINELLA_HOMONYMA);
    }
    alioquin
    {
        tabula_dispersa_inserere(bib->nomina, *titulus, ens);
    }
}

interior vacuum
arborem_legere(
    NaturaBibliotheca* bib,
    NaturaExemplar*    ex,
    StmlNodus*         nodus,
    NaturaGenus*       ambiens)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;
        chorda*    titulus_e;
        chorda*    titulus_n;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus_e = liberum->titulus;

        /* vocabularium (olim regula VIII) canon iudicat, non
         * onerator - elementum ignotum tacite percurritur */

        titulus_n = stml_attributum_capere(liberum, "nomen");

        si (chorda_aequalis_literis(*titulus_e, "genus"))
        {
            NaturaGenus*  novum;
            NaturaGenus** locus;

            si (!titulus_n)
            {
                /* sine nomine registrari nequit - canon clamat
                 * (ATTRIBUTUM_DEEST), onerator praeterit */
                arborem_legere(bib, ex, liberum, ambiens);
                perge;
            }

            novum = (NaturaGenus*)piscina_allocare(
                bib->piscina, magnitudo(NaturaGenus));
            novum->titulus  = titulus_n;
            novum->modulus  = ex->stirps;
            novum->parens   = ambiens;  /* nidificatio; sub= in nectere */
            novum->liberi   = xar_creare(bib->piscina,
                                 (i32)magnitudo(NaturaGenus*));
            novum->res_suae = xar_creare(bib->piscina,
                                 (i32)magnitudo(NaturaRes*));
            novum->nodus    = liberum;

            locus = (NaturaGenus**)xar_addere(bib->genera_omnia);
            *locus = novum;

            ens_registrare(bib, ex, NATURA_ENS_GENUS, novum,
                           titulus_n);
            arborem_legere(bib, ex, liberum, novum);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "species") ||
                     chorda_aequalis_literis(*titulus_e, "individuum") ||
                     chorda_aequalis_literis(*titulus_e, "cultivar"))
        {
            NaturaRes*  nova;
            NaturaRes** locus;

            si (!titulus_n)
            {
                /* sine nomine registrari nequit - canon clamat
                 * (ATTRIBUTUM_DEEST), onerator praeterit */
                arborem_legere(bib, ex, liberum, ambiens);
                perge;
            }

            nova = (NaturaRes*)piscina_allocare(
                bib->piscina, magnitudo(NaturaRes));
            nova->titulus    = titulus_n;
            nova->modulus    = ex->stirps;
            nova->genus_suum = ambiens;
            nova->nodus      = liberum;

            locus = (NaturaRes**)xar_addere(bib->res_omnes);
            *locus = nova;

            si (ambiens)
            {
                NaturaRes** locus_g;

                locus_g = (NaturaRes**)xar_addere(ambiens->res_suae);
                *locus_g = nova;
            }

            ens_registrare(bib, ex, NATURA_ENS_RES, nova, titulus_n);
            /* genus nidificatum in specie parentem GENUS ambientem
             * accipit (profunditas Porphyriana est taxinomia, non
             * catena apparatus) */
            arborem_legere(bib, ex, liberum, ambiens);
        }
        alioquin
        {
            arborem_legere(bib, ex, liberum, ambiens);
        }
    }
}

b32
natura_legere(
    NaturaBibliotheca*   bib,
    chorda               fons,
    constans character*  stirps)
{
    StmlResultus     resultus;
    NaturaExemplar*  ex;
    NaturaExemplar** locus;
    chorda*          modulus_attr;

    si (!bib || !stirps)
    {
        redde FALSUM;
    }

    resultus = stml_legere(fons, bib->piscina, bib->intern);
    si (!resultus.successus || !resultus.elementum_radix)
    {
        diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, I,
            chorda_internare_ex_literis(bib->intern, stirps), NIHIL,
            "parsatio STML fracta (regula I)");
        redde FALSUM;
    }

    si (!chorda_aequalis_literis(*resultus.elementum_radix->titulus,
                                 "natura"))
    {
        diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, I,
            chorda_internare_ex_literis(bib->intern, stirps), NIHIL,
            "radix non est <natura> (regula I)");
        redde FALSUM;
    }

    /* forma BENE FORMATA super parsationem (regula I): parser
     * consulto lenis est, sed haec documentum TACITE ambiguum
     * redderent - attributum duplicatum praesertim, ubi plagula
     * unum dicit et omnis lector alterum adhibet */
    {
        Xar* vitia;
        i32  i;

        vitia = stml_strictum(resultus.radix, bib->piscina);
        per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
        {
            StmlStrictumVitium* v;

            v = (StmlStrictumVitium*)xar_obtinere(vitia, i);
            diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, I,
                chorda_internare_ex_literis(bib->intern, stirps),
                v->causa, stml_strictum_nuntius(v->genus));
        }
    }

    ex = (NaturaExemplar*)piscina_allocare(
        bib->piscina, magnitudo(NaturaExemplar));
    ex->stirps = chorda_internare_ex_literis(bib->intern, stirps);
    ex->versio = stml_attributum_capere(resultus.elementum_radix,
                                        "versio");
    ex->radix  = resultus.elementum_radix;

    locus = (NaturaExemplar**)xar_addere(bib->exemplaria);
    *locus = ex;

    /* regula XIV: attributum modulus = stirps plagulae */
    modulus_attr = stml_attributum_capere(resultus.elementum_radix,
                                          "modulus");
    si (!modulus_attr ||
        !chorda_aequalis_literis(*modulus_attr, stirps))
    {
        diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XIV,
            ex->stirps, modulus_attr,
            "attributum modulus stirpi plagulae non aequat (regula XIV)");
    }

    arborem_legere(bib, ex, resultus.elementum_radix, NIHIL);

    bib->nexum = FALSUM;  /* lectio nova nexuram novam poscit */

    redde VERUM;
}

/* ==================================================
 * Phasis II - nectere
 * ================================================== */

/* quaerere elementum nominatum in genere aut maioribus:
 * continens NIHIL = liberum directum generis (machina_statuum) */
interior StmlNodus*
in_catena_invenire(
    NaturaGenus*         genus,
    constans character*  continens,
    constans character*  elementum,
    chorda*              quaesitum)
{
    NaturaGenus* g;
    i32          gradus_catenae;

    gradus_catenae = ZEPHYRUM;
    per (g = genus; g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        StmlNodus* sedes;
        i32        numerus;
        i32        i;

        sedes = g->nodus;
        si (continens)
        {
            sedes = stml_invenire_liberum(g->nodus, continens);
        }

        si (sedes)
        {
            numerus = stml_numerus_liberorum(sedes);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                StmlNodus* liberum;
                chorda*    titulus_n;

                liberum = stml_liberum_ad_indicem(sedes, i);
                si (!liberum ||
                    liberum->genus != STML_NODUS_ELEMENTUM ||
                    !chorda_aequalis_literis(*liberum->titulus,
                                             elementum))
                {
                    perge;
                }

                titulus_n = stml_attributum_capere(liberum, "nomen");
                si (titulus_n && chorda_aequalis(*titulus_n,
                                                 *quaesitum))
                {
                    redde liberum;
                }
            }
        }

        gradus_catenae++;
    }

    redde NIHIL;
}

/* utramque catenam membri probare: sub= et etiam= (par.3/4).
 * Membrum duplex ESSENTIALE est - res utriusque generis vere est,
 * ergo apparatum utriusque legitime adhibet */
interior StmlNodus*
in_membris_invenire(
    NaturaGenus*         primus,
    NaturaGenus*         etiam,
    constans character*  continens,
    constans character*  elementum,
    chorda*              quaesitum)
{
    StmlNodus* inventum;

    inventum = NIHIL;
    si (primus)
    {
        inventum = in_catena_invenire(primus, continens, elementum,
                                      quaesitum);
    }
    si (!inventum && etiam)
    {
        inventum = in_catena_invenire(etiam, continens, elementum,
                                      quaesitum);
    }

    redde inventum;
}

/* yyyy | yyyy-mm | yyyy-mm-dd, cum mense <= XII et die <= XXXI */
interior b32
dies_bene_formata(
    constans chorda* d)
{
    i32 i;
    i32 mensis;
    i32 dies_n;

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
        mensis = (i32)(d->datum[V] - '0') * X +
                 (i32)(d->datum[VI] - '0');
        si (mensis < I || mensis > XII)
        {
            redde FALSUM;
        }
    }

    si (d->mensura == X)
    {
        dies_n = (i32)(d->datum[VIII] - '0') * X +
                 (i32)(d->datum[IX] - '0');
        si (dies_n < I || dies_n > XXXI)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* regulae VI (certitudo), VII (valens_a/valens_ad).
 * Regula V (fons -> clavis) MIGRAVIT ad canonem (2026-08-06):
 * citatio 'fontium' in natura.canon - resolutio intra documentum
 * gradus II est, non III. */
interior vacuum
qualificationes_probare(
    NaturaBibliotheca* bib,
    NaturaExemplar*    ex,
    Xar*               gradus_noti,
    StmlNodus*         nodus,
    chorda*            ens)
{
    chorda* certitudo_attr;
    chorda* valens_a;
    chorda* valens_ad;

    certitudo_attr = stml_attributum_capere(nodus, "certitudo");
    si (certitudo_attr && gradus_noti)
    {
        i32 i;
        b32 inventum;

        inventum = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(gradus_noti); i++)
        {
            chorda** gradus_n;

            gradus_n = (chorda**)xar_obtinere(gradus_noti, i);
            si (chorda_aequalis(*certitudo_attr, **gradus_n))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, VI,
                ex->stirps, ens,
                "certitudo non species gradus_assensus (regula VI)");
        }
    }

    /* Forma dierum (dimidium regulae VII) MIGRAVIT ad canonem:
     * valens_a/valens_ad genus="dies" in natura.canon ferunt.
     * ORDO solus hic manet - comparatio trans attributa est
     * (co-occurrentia), quam canon consulto non dicit.
     * dies_bene_formata custos manet: dies malas comparare
     * nihil significat (canon eas iam clamat). */
    valens_a  = stml_attributum_capere(nodus, "valens_a");
    valens_ad = stml_attributum_capere(nodus, "valens_ad");
    si (valens_a && valens_ad &&
        dies_bene_formata(valens_a) && dies_bene_formata(valens_ad) &&
        chorda_comparare(*valens_a, *valens_ad) > ZEPHYRUM)
    {
        diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, VII,
            ex->stirps, ens,
            "valens_a post valens_ad (regula VII)");
    }
}

/* Regula XII (transitus -> status machinae suae) MIGRAVIT ad
 * canonem (2026-08-06): citationes 'transitus-a'/'transitus-ad'
 * cum intra="machina_statuum" in natura.canon - scopus per
 * instantiam citationis opus est, quod canon nunc praestat. */

interior vacuum
arborem_nectere(
    NaturaBibliotheca* bib,
    NaturaExemplar*    ex,
    Xar*               gradus_noti,
    StmlNodus*         nodus,
    NaturaGenus*       genus_c,
    NaturaRes*         res_c)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus*   liberum;
        chorda*      titulus_e;
        chorda*      titulus_n;
        NaturaGenus* pertinens;
        NaturaGenus* pertinens_etiam;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus_e = liberum->titulus;
        titulus_n = stml_attributum_capere(liberum, "nomen");
        pertinens = res_c ? res_c->genus_suum : genus_c;
        pertinens_etiam = res_c ? res_c->genus_etiam : NIHIL;

        si (chorda_aequalis_literis(*titulus_e, "genus"))
        {
            NaturaEns* ens;

            ens = titulus_n ?
                ens_quaerere(bib, ex->stirps, titulus_n) : NIHIL;
            si (ens && ens->discrimen == NATURA_ENS_GENUS)
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                (NaturaGenus*)ens->corpus, NIHIL);
            }
            alioquin
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, res_c);
            }
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "species") ||
                     chorda_aequalis_literis(*titulus_e,
                                             "individuum") ||
                     chorda_aequalis_literis(*titulus_e, "cultivar"))
        {
            NaturaEns* ens;

            ens = titulus_n ?
                ens_quaerere(bib, ex->stirps, titulus_n) : NIHIL;
            si (ens && ens->discrimen == NATURA_ENS_RES)
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, (NaturaRes*)ens->corpus);
            }
            alioquin
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, res_c);
            }
        }
        alioquin si (chorda_aequalis_literis(*titulus_e,
                                             "machina_statuum"))
        {
            /* regula XII apud canonem; ramus vacuus descensum
             * veterem servat (in machinam non descendimus) */
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "proprietas"))
        {
            chorda* modulus_attr;
            chorda* genus_attr;

            /* regula III: proprietas typo bibliothecae */
            modulus_attr = stml_attributum_capere(liberum, "modulus");
            genus_attr   = stml_attributum_capere(liberum, "genus");
            si (modulus_attr && genus_attr)
            {
                NaturaEns* scopus;

                scopus = ens_quaerere(bib, modulus_attr, genus_attr);
                si (!scopus)
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        III, ex->stirps, titulus_n,
                        "proprietas typum bibliothecae non invenit (regula III)");
                }
                alioquin si (scopus->discrimen != NATURA_ENS_GENUS)
                {
                    /* SOLVIT sed gradu inusitato. Regula II
                     * (relationes) genus AUT rem dictionarii iam
                     * accipit, et par.4b addressam gradus
                     * NESCIAM esse iubet - ergo non vulnus.
                     * Monitum tamen, quia apparatus rei citatae
                     * SCHEMA describere potest, non valorem:
                     * 'opacus' de isbn ut ratione verum est, de
                     * isbn huius libri nihil dicit */
                    diagnosticum_addere(bib, NATURA_GRADUS_MONITUM,
                        III, ex->stirps, titulus_n,
                        "proprietas rem dictionarii typum habet, non genus (regula III)");
                }
            }
            arborem_nectere(bib, ex, gradus_noti, liberum,
                            genus_c, res_c);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "relatio"))
        {
            chorda* ad_attr;
            chorda* modulus_attr;
            chorda* externum_attr;

            si (stml_attributum_capere(liberum, "inversa"))
            {
                perge;  /* documentatio, non arcus alter */
            }

            ad_attr = stml_attributum_capere(liberum, "ad");
            si (!ad_attr)
            {
                diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, II,
                    ex->stirps, titulus_n,
                    "relatio sine ad nec inversa (regula II)");
                perge;
            }
            si (chorda_aequalis_literis(*ad_attr, "*"))
            {
                perge;  /* apertum consulto */
            }

            modulus_attr  = stml_attributum_capere(liberum,
                                                   "modulus");
            externum_attr = stml_attributum_capere(liberum,
                                                   "externum");
            {
                NaturaEns* scopus;

                scopus = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : ex->stirps,
                    ad_attr);
                si (externum_attr &&
                    chorda_aequalis_literis(*externum_attr, "verum"))
                {
                    si (scopus)
                    {
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS, XVI,
                            ex->stirps, ad_attr,
                            "externum stat, scopus iam descriptus (regula XVI)");
                    }
                }
                alioquin si (!scopus)
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        II, ex->stirps, ad_attr,
                        "relatio scopum non invenit (regula II)");
                }
            }
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "relatum"))
        {
            chorda* ad_attr;
            chorda* modulus_attr;
            chorda* externum_attr;

            /* regula XI: relatio declarata in catena */
            si (titulus_n && pertinens &&
                !in_membris_invenire(pertinens, pertinens_etiam,
                                     "relationes", "relatio",
                                     titulus_n))
            {
                diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XI,
                    ex->stirps, titulus_n,
                    "relatum relationem non declaratam nominat (regula XI)");
            }

            ad_attr       = stml_attributum_capere(liberum, "ad");
            modulus_attr  = stml_attributum_capere(liberum,
                                                   "modulus");
            externum_attr = stml_attributum_capere(liberum,
                                                   "externum");
            si (ad_attr && !chorda_aequalis_literis(*ad_attr, "*"))
            {
                NaturaEns* scopus;

                scopus = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : ex->stirps,
                    ad_attr);
                si (externum_attr &&
                    chorda_aequalis_literis(*externum_attr, "verum"))
                {
                    si (scopus)
                    {
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS, XVI,
                            ex->stirps, ad_attr,
                            "externum stat, scopus iam descriptus (regula XVI)");
                    }
                }
                alioquin si (!scopus)
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        II, ex->stirps, ad_attr,
                        "relatum scopum non invenit (regula II)");
                }
            }

            qualificationes_probare(bib, ex, gradus_noti, liberum,
                                    titulus_n);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "valor"))
        {
            StmlNodus* proprietas_n;

            proprietas_n = NIHIL;
            si (titulus_n && pertinens)
            {
                proprietas_n = in_membris_invenire(pertinens,
                    pertinens_etiam, "proprietates", "proprietas",
                    titulus_n);

                /* regula IX: proprietas AUT machina_statuum */
                si (!proprietas_n &&
                    !in_membris_invenire(pertinens, pertinens_etiam,
                                         NIHIL, "machina_statuum",
                                         titulus_n))
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        IX, ex->stirps, titulus_n,
                        "valor proprietatem non declaratam nominat (regula IX)");
                }
            }

            /* regula X: electio optionem declaratam poscit */
            si (proprietas_n)
            {
                chorda* genus_attr;

                genus_attr = stml_attributum_capere(proprietas_n,
                                                    "genus");
                si (genus_attr &&
                    chorda_aequalis_literis(*genus_attr, "electio"))
                {
                    chorda valor_textus;
                    i32    n_opt;
                    i32    j;
                    b32    inventum;

                    valor_textus = chorda_praecidere(
                        stml_textus_internus(liberum, bib->piscina));
                    inventum = FALSUM;
                    n_opt = stml_numerus_liberorum(proprietas_n);
                    per (j = ZEPHYRUM; j < n_opt; j++)
                    {
                        StmlNodus* optio_n;
                        chorda     optio_textus;

                        optio_n = stml_liberum_ad_indicem(
                            proprietas_n, j);
                        si (!optio_n ||
                            optio_n->genus != STML_NODUS_ELEMENTUM ||
                            !chorda_aequalis_literis(
                                *optio_n->titulus, "optio"))
                        {
                            perge;
                        }
                        optio_textus = chorda_praecidere(
                            stml_textus_internus(optio_n,
                                                 bib->piscina));
                        si (chorda_aequalis(valor_textus,
                                            optio_textus))
                        {
                            inventum = VERUM;
                            frange;
                        }
                    }
                    si (!inventum)
                    {
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS, X,
                            ex->stirps, titulus_n,
                            "valor electionis optioni nulli aequat (regula X)");
                    }
                }
            }

            qualificationes_probare(bib, ex, gradus_noti, liberum,
                                    titulus_n);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "eventum"))
        {
            chorda* actio_attr;

            /* regula XIII: actio declarata in catena */
            actio_attr = stml_attributum_capere(liberum, "actio");
            si (actio_attr && pertinens &&
                !in_membris_invenire(pertinens, pertinens_etiam,
                                     "actiones", "actio",
                                     actio_attr))
            {
                diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XIII,
                    ex->stirps, actio_attr,
                    "eventum actionem non declaratam nominat (regula XIII)");
            }

            qualificationes_probare(bib, ex, gradus_noti, liberum,
                pertinens ? pertinens->titulus : NIHIL);
        }
        alioquin
        {
            arborem_nectere(bib, ex, gradus_noti, liberum,
                            genus_c, res_c);
        }
    }
}

i32
natura_nectere(
    NaturaBibliotheca* bib)
{
    i32  i;
    i32  vulnera;
    Xar* gradus_noti;

    si (!bib)
    {
        redde ZEPHYRUM;
    }

    si (!bib->nexum)
    {
        /* parentes: sub= vincit nidificationem */
        per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
        {
            NaturaGenus* g;
            chorda*      sub_attr;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
            sub_attr = stml_attributum_capere(g->nodus, "sub");
            si (sub_attr)
            {
                chorda*    modulus_attr;
                NaturaEns* ens;

                modulus_attr = stml_attributum_capere(g->nodus,
                                                      "modulus");
                ens = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : g->modulus,
                    sub_attr);
                si (ens && ens->discrimen == NATURA_ENS_GENUS)
                {
                    g->parens = (NaturaGenus*)ens->corpus;
                }
                alioquin
                {
                    g->parens = NIHIL;
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        IV, g->modulus, g->titulus,
                        "sub parentem non invenit (regula IV)");
                }
            }
        }

        /* etiam=: membrum essentiale duplex (species solae) */
        per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
        {
            NaturaRes* r;
            chorda*    etiam_attr;

            r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
            etiam_attr = stml_attributum_capere(r->nodus, "etiam");
            si (etiam_attr)
            {
                chorda*    modulus_attr;
                NaturaEns* ens;

                modulus_attr = stml_attributum_capere(r->nodus,
                                                      "modulus");
                ens = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : r->modulus,
                    etiam_attr);
                si (ens && ens->discrimen == NATURA_ENS_GENUS)
                {
                    r->genus_etiam = (NaturaGenus*)ens->corpus;
                }
                alioquin
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        IV, r->modulus, r->titulus,
                        "etiam genus non invenit (regula IV)");
                }
            }
        }

        /* index reversus + custodia catenae cyclicae */
        per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
        {
            NaturaGenus*  g;
            NaturaGenus*  maior;
            i32           gradus_catenae;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
            si (g->parens)
            {
                NaturaGenus** locus;

                locus = (NaturaGenus**)xar_addere(g->parens->liberi);
                *locus = g;
            }

            gradus_catenae = ZEPHYRUM;
            per (maior = g->parens;
                 maior && gradus_catenae < NATURA_CATENA_MAXIMA;
                 maior = maior->parens)
            {
                gradus_catenae++;
            }
            si (maior)
            {
                diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, IV,
                    g->modulus, g->titulus,
                    "catena parentum cyclica aut nimis alta (regula IV)");
            }
        }

        /* species gradus_assensus pro regula VI */
        gradus_noti = NIHIL;
        {
            chorda*    modulus_i;
            chorda*    titulus_g;
            NaturaEns* ens;

            modulus_i = chorda_internare_ex_literis(bib->intern,
                                                    "iudicium");
            titulus_g = chorda_internare_ex_literis(bib->intern,
                                                    "gradus_assensus");
            ens = ens_quaerere(bib, modulus_i, titulus_g);
            si (ens && ens->discrimen == NATURA_ENS_GENUS)
            {
                NaturaGenus* g;
                i32          j;

                g = (NaturaGenus*)ens->corpus;
                gradus_noti = xar_creare(bib->piscina,
                                         (i32)magnitudo(chorda*));
                per (j = ZEPHYRUM; j < xar_numerus(g->res_suae); j++)
                {
                    NaturaRes* r;
                    chorda**   locus;

                    r = *(NaturaRes**)xar_obtinere(g->res_suae, j);
                    locus = (chorda**)xar_addere(gradus_noti);
                    *locus = r->titulus;
                }
            }
        }

        /* percursus regularum per exemplaria */
        per (i = ZEPHYRUM; i < xar_numerus(bib->exemplaria); i++)
        {
            NaturaExemplar* ex;

            ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, i);
            arborem_nectere(bib, ex, gradus_noti, ex->radix,
                            NIHIL, NIHIL);
        }

        bib->nexum = VERUM;
    }

    vulnera = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(bib->diagnostica); i++)
    {
        NaturaDiagnosticum* d;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, i);
        si (d->gradus == NATURA_GRADUS_VULNUS)
        {
            vulnera++;
        }
    }

    redde vulnera;
}

/* ==================================================
 * Quaestiones
 * ================================================== */

NaturaGenus*
natura_genus(
    NaturaBibliotheca*   bib,
    constans character*  titulus)
{
    vacuum*    valor;
    NaturaEns* ens;

    si (!bib || !titulus)
    {
        redde NIHIL;
    }

    si (!tabula_dispersa_invenire_literis(bib->nomina, titulus,
                                          &valor))
    {
        redde NIHIL;
    }

    ens = (NaturaEns*)valor;
    si (ens == &SENTINELLA_HOMONYMA ||
        ens->discrimen != NATURA_ENS_GENUS)
    {
        redde NIHIL;
    }

    redde (NaturaGenus*)ens->corpus;
}

NaturaEns*
natura_ens_in(
    NaturaBibliotheca*   bib,
    constans character*  modulus,
    constans character*  titulus)
{
    chorda  clavis;
    i8      buffer[CCLVI];
    i32     mensura_m;
    i32     mensura_t;
    vacuum* valor;

    si (!bib || !modulus || !titulus)
    {
        redde NIHIL;
    }

    mensura_m = (i32)strlen(modulus);
    mensura_t = (i32)strlen(titulus);
    si (mensura_m + I + mensura_t > CCLVI)
    {
        redde NIHIL;
    }

    memcpy(buffer, modulus, (memoriae_index)mensura_m);
    buffer[mensura_m] = (i8)'/';
    memcpy(buffer + mensura_m + I, titulus,
           (memoriae_index)mensura_t);

    clavis.datum   = buffer;
    clavis.mensura = mensura_m + I + mensura_t;

    si (!tabula_dispersa_invenire(bib->entia, clavis, &valor))
    {
        redde NIHIL;
    }

    redde (NaturaEns*)valor;
}

NaturaGenus*
natura_genus_in(
    NaturaBibliotheca*   bib,
    constans character*  modulus,
    constans character*  titulus)
{
    NaturaEns* ens;

    ens = natura_ens_in(bib, modulus, titulus);
    si (!ens || ens->discrimen != NATURA_ENS_GENUS)
    {
        redde NIHIL;
    }

    redde (NaturaGenus*)ens->corpus;
}

Xar*
natura_maiores(
    NaturaGenus* genus,
    Piscina*     piscina)
{
    Xar*         maiores;
    NaturaGenus* g;
    i32          gradus_catenae;

    si (!genus || !piscina)
    {
        redde NIHIL;
    }

    maiores = xar_creare(piscina, (i32)magnitudo(NaturaGenus*));
    gradus_catenae = ZEPHYRUM;
    per (g = genus->parens;
         g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        NaturaGenus** locus;

        locus = (NaturaGenus**)xar_addere(maiores);
        *locus = g;
        gradus_catenae++;
    }

    redde maiores;
}

/* ==================================================
 * Apparatus
 * ================================================== */

interior vacuum
apparatui_contribuere(
    Xar*         apparatus,
    NaturaGenus* genus)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(genus->nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;
        chorda*    titulus_e;

        liberum = stml_liberum_ad_indicem(genus->nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus_e = liberum->titulus;

        si (chorda_aequalis_literis(*titulus_e, "machina_statuum"))
        {
            NaturaApparatusMembrum* membrum;

            membrum = (NaturaApparatusMembrum*)xar_addere(apparatus);
            membrum->nodus  = liberum;
            membrum->auctor = genus;
        }
        alioquin si (
            chorda_aequalis_literis(*titulus_e, "proprietates") ||
            chorda_aequalis_literis(*titulus_e, "partes") ||
            chorda_aequalis_literis(*titulus_e, "actiones") ||
            chorda_aequalis_literis(*titulus_e, "relationes"))
        {
            i32 n_intus;
            i32 j;

            n_intus = stml_numerus_liberorum(liberum);
            per (j = ZEPHYRUM; j < n_intus; j++)
            {
                StmlNodus*              intus;
                NaturaApparatusMembrum* membrum;

                intus = stml_liberum_ad_indicem(liberum, j);
                si (!intus || intus->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }

                membrum = (NaturaApparatusMembrum*)xar_addere(
                    apparatus);
                membrum->nodus  = intus;
                membrum->auctor = genus;
            }
        }
    }
}

Xar*
natura_apparatus(
    NaturaBibliotheca*  bib,
    NaturaGenus*        genus,
    Piscina*            piscina)
{
    Xar*         apparatus;
    NaturaGenus* g;
    i32          gradus_catenae;

    si (!bib || !genus || !piscina)
    {
        redde NIHIL;
    }

    apparatus = xar_creare(piscina,
                           (i32)magnitudo(NaturaApparatusMembrum));

    gradus_catenae = ZEPHYRUM;
    per (g = genus; g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        apparatui_contribuere(apparatus, g);
        gradus_catenae++;
    }

    redde apparatus;
}
