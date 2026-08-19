/* natura.c - onerator bibliothecae semanticae
 *
 * Vide include/natura.h pro doctrina (proiectio, phases duae,
 * profunditas hybrida). Vocabularium clausum et regulae ex
 * natura/METAMODULUS.md par.3/4/8 sumpta sunt.
 */
#include "natura.h"
#include <string.h>
#include <stdio.h>


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
    StmlNodus* nodus, NaturaGenus* ambiens,
    NaturaRes* ambiens_res);
interior StmlNodus* in_catena_invenire(
    NaturaGenus* genus, constans character* continens,
    constans character* elementum, constans character* clavis,
    chorda* quaesitum);
interior StmlNodus* in_membris_invenire(
    NaturaGenus* primus, NaturaGenus* etiam,
    constans character* continens, constans character* elementum,
    constans character* clavis, chorda* quaesitum);
interior NaturaNecessitudo* necessitudinem_invenire(
    NaturaBibliotheca* bib, constans chorda* scriptura);
interior vacuum scripturam_inserere(
    NaturaBibliotheca* bib, NaturaNecessitudo* declarata,
    chorda* scriptura);
interior NaturaGenus* genus_per_chordam(
    NaturaBibliotheca* bib, constans chorda* titulus);
interior chorda* finem_effectivum(
    NaturaNecessitudo* declarata, constans character* finis);
interior b32 genus_subsumitur(
    NaturaBibliotheca* bib, NaturaGenus* genus,
    constans chorda* latius);
interior vacuum sedem_ligare(
    NaturaBibliotheca* bib, NaturaExemplar* ex,
    NaturaGenus* possessor, Xar* insoluta,
    StmlNodus* sedes, chorda* titulus_sedis);
interior constans character* nuntium_insolutarum_fingere(
    Piscina* piscina, Xar* insoluta);
interior b32 dies_bene_formata(constans chorda* d);
interior vacuum qualificationes_probare(
    NaturaBibliotheca* bib, NaturaExemplar* ex,
    Xar* gradus_noti, StmlNodus* nodus, chorda* ens);
interior vacuum arborem_nectere(
    NaturaBibliotheca* bib, NaturaExemplar* ex, Xar* gradus_noti,
    StmlNodus* nodus, NaturaGenus* genus_c, NaturaRes* res_c,
    Xar* insoluta);
interior NaturaEns* ens_quaerere(
    NaturaBibliotheca* bib, constans chorda* modulus,
    constans chorda* titulus);
interior vacuum apparatui_contribuere(
    Xar* apparatus, NaturaGenus* genus);


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
diagnosticum_addere (
           NaturaBibliotheca* bib,
    NaturaDiagnosticumGradus  gradus,
                         i32  regula,
                      chorda* modulus,
                      chorda* ens,
          constans character* nuntius)
{
    NaturaDiagnosticum* d;

    d = (NaturaDiagnosticum*)xar_addere(bib->diagnostica);
    si (!d)
    {
        redde;
    }

    d->gradus   = gradus;
    d->regula   = regula;
    d->modulus  = modulus;
    d->ens      = ens;
    d->nuntius  = nuntius;
}

/* clavis addressae: "modulus/nomen" */
interior chorda
clavis_entis (
            Piscina* piscina,
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
ens_quaerere (
    NaturaBibliotheca* bib,
      constans chorda* modulus,
      constans chorda* titulus)
{
    PiscinaNotatio  nota;
            chorda  clavis;
            vacuum* valor;
               b32  inventum;

    /* clavis temporaria - piscina statim reficitur */
    nota      = piscina_notare(bib->piscina);
    clavis    = clavis_entis(bib->piscina, modulus, titulus);
    inventum  = tabula_dispersa_invenire(bib->entia, clavis, &valor);
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
natura_bibliotheca_creare (
    Piscina* piscina)
{
    NaturaBibliotheca* bib;

    si (!piscina)
    {
        redde NIHIL;
    }

    bib = (NaturaBibliotheca*)piscina_allocare(
        piscina, magnitudo(NaturaBibliotheca));

    bib->piscina = piscina;
    bib->intern = internamentum_creare(piscina);
    bib->exemplaria = xar_creare(piscina, (i32)magnitudo(NaturaExemplar*));
    bib->genera_omnia = xar_creare(piscina, (i32)magnitudo(NaturaGenus*));
    bib->res_omnes = xar_creare(piscina, (i32)magnitudo(NaturaRes*));
    bib->entia = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    bib->nomina = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    bib->necessitudines_omnes = xar_creare(piscina,
                                   (i32)magnitudo(NaturaNecessitudo*));
    bib->scriptiones  = NIHIL;  /* nectere aedificat */
    bib->diagnostica  = xar_creare(piscina,
                                   (i32)magnitudo(NaturaDiagnosticum));
    bib->sedes_ligatae = xar_creare(piscina,
                                   (i32)magnitudo(NaturaSedesLigata));
    bib->nexum        = FALSUM;

    si (   !bib->intern || !bib->exemplaria || !bib->genera_omnia
        || !bib->res_omnes || !bib->entia || !bib->nomina
        || !bib->necessitudines_omnes || !bib->diagnostica
        || !bib->sedes_ligatae)
    {
        redde NIHIL;
    }

    redde bib;
}


/* ==================================================
 * Phasis I - legere
 * ================================================== */

interior vacuum
ens_registrare (
     NaturaBibliotheca* bib,
        NaturaExemplar* ex,
    NaturaEnsDiscrimen  discrimen,
                vacuum* corpus,
                chorda* titulus)
{
    NaturaEns* ens;
       chorda  clavis;
       vacuum* prius;

    ens = (NaturaEns*)piscina_allocare(
        bib->piscina, magnitudo(NaturaEns));
    ens->discrimen  = discrimen;
    ens->corpus     = corpus;

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
arborem_legere (
    NaturaBibliotheca* bib,
       NaturaExemplar* ex,
            StmlNodus* nodus,
          NaturaGenus* ambiens,
            NaturaRes* ambiens_res)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;
           chorda* titulus_e;
           chorda* titulus_n;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus_e = liberum->titulus;

        /* vocabularium (olim regula VIII) canon iudicat, non
         * onerator - elementum ignotum tacite percurritur */

        titulus_n = stml_attributum_capere(liberum, "nomen");

        /* arbor porphyriana (2026-08-10): .genus/.species/.cultivar
         * UNUM genus registrant - nomen elementi munus solum fert
         * (species = subdivisio inscripta, parens e nidificatione;
         * genus = forma libera, sub= licitum). individuum solum res
         * manet - linea absoluta universale/particulare. */
        si (   chorda_aequalis_literis(*titulus_e, ".genus")
            || chorda_aequalis_literis(*titulus_e, ".species")
            || chorda_aequalis_literis(*titulus_e, ".cultivar"))
        {
            NaturaGenus*  novum;
            NaturaGenus** locus;

            si (!titulus_n)
            {
                /* sine nomine registrari nequit - canon clamat
                 * (ATTRIBUTUM_DEEST), onerator praeterit */
                arborem_legere(bib, ex, liberum, ambiens,
                               ambiens_res);
                perge;
            }

            novum = (NaturaGenus*)piscina_allocare(
                bib->piscina, magnitudo(NaturaGenus));
            novum->titulus  = titulus_n;
            novum->modulus  = ex->stirps;
            novum->parens   = ambiens;  /* nidificatio; sub= in
                                             * nectere (generi soli) */
            novum->parens_etiam = NIHIL;    /* etiam= in nectere */
            novum->liberi       = xar_creare(bib->piscina,
                                     (i32)magnitudo(NaturaGenus*));
            novum->res_suae     = xar_creare(bib->piscina,
                                     (i32)magnitudo(NaturaRes*));
            novum->nodus        = liberum;

            locus   = (NaturaGenus**)xar_addere(bib->genera_omnia);
            *locus  = novum;

            ens_registrare(bib, ex, NATURA_ENS_GENUS, novum,
                           titulus_n);
            arborem_legere(bib, ex, liberum, novum, NIHIL);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "individuum"))
        {
            NaturaRes*  nova;
            NaturaRes** locus;

            si (!titulus_n)
            {
                /* sine nomine registrari nequit - canon clamat
                 * (ATTRIBUTUM_DEEST), onerator praeterit */
                arborem_legere(bib, ex, liberum, ambiens,
                               ambiens_res);
                perge;
            }

            nova = (NaturaRes*)piscina_allocare(
                bib->piscina, magnitudo(NaturaRes));
            nova->titulus     = titulus_n;
            nova->modulus     = ex->stirps;
            nova->genus_suum  = ambiens;
            nova->continens   = ambiens_res;
            nova->res_suae   = xar_creare(bib->piscina,
                                 (i32)magnitudo(NaturaRes*));
            nova->nodus      = liberum;

            locus   = (NaturaRes**)xar_addere(bib->res_omnes);
            *locus  = nova;

            si (ambiens)
            {
                NaturaRes** locus_g;

                locus_g   = (NaturaRes**)xar_addere(ambiens->res_suae);
                *locus_g  = nova;
            }

            /* catena TAXINOMIAE: res nidificata continenti suae
             * quoque adscribitur (catena apparatus supra intacta
             * manet - genus->res_suae planum est CONSULTO) */
            si (ambiens_res)
            {
                NaturaRes** locus_r;

                locus_r = (NaturaRes**)xar_addere(
                              ambiens_res->res_suae);
                *locus_r = nova;
            }

            ens_registrare(bib, ex, NATURA_ENS_RES, nova, titulus_n);
            /* genus nidificatum in specie parentem GENUS ambientem
             * accipit (profunditas Porphyriana est taxinomia, non
             * catena apparatus) - sed continens ipsa NUNC
             * traditur, ut taxinomia dici possit */
            arborem_legere(bib, ex, liberum, ambiens, nova);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e,
                                             "necessitudo"))
        {
            NaturaNecessitudo*  declarata;
            NaturaNecessitudo** locus;

            si (!titulus_n)
            {
                /* sine nomine registrari nequit - canon clamat */
                perge;
            }

            declarata = (NaturaNecessitudo*)piscina_allocare(
                bib->piscina, magnitudo(NaturaNecessitudo));
            declarata->titulus   = titulus_n;
            declarata->conversum = stml_attributum_capere(liberum,
                                                          "conversum");
            declarata->modulus  = ex->stirps;
            declarata->parens   = NIHIL;  /* sub= in nectere */
            declarata->nodus    = liberum;

            locus = (NaturaNecessitudo**)xar_addere(
                bib->necessitudines_omnes);
            *locus = declarata;

            ens_registrare(bib, ex, NATURA_ENS_NECESSITUDO,
                           declarata, titulus_n);
            /* non descendimus: scriptiones nectere legit */
        }
        alioquin
        {
            arborem_legere(bib, ex, liberum, ambiens, ambiens_res);
        }
    }
}

b32
natura_legere (
     NaturaBibliotheca* bib,
                chorda  fons,
    constans character* stirps)
{
      StmlResultus   resultus;
    NaturaExemplar*  ex;
    NaturaExemplar** locus;
            chorda*  modulus_attr;

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

    locus   = (NaturaExemplar**)xar_addere(bib->exemplaria);
    *locus  = ex;

    /* regula XIV: attributum modulus = stirps plagulae */
    modulus_attr = stml_attributum_capere(resultus.elementum_radix,
                                          "modulus");
    si (   !modulus_attr
        || !chorda_aequalis_literis(*modulus_attr, stirps))
    {
        diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XIV,
            ex->stirps, modulus_attr,
            "attributum modulus stirpi plagulae non aequat (regula XIV)");
    }

    /* regula XXV: modulus genus AUT necessitudinem declarare debet
     * (canon minimum='1' generis 2026-08-10 sustulit - regula
     * co-occurrentiae supra canonem vivit, ut 'ad AUT a') */
    {
        i32 genera_ante;
        i32 nexus_ante;

        genera_ante  = xar_numerus(bib->genera_omnia);
        nexus_ante   = xar_numerus(bib->necessitudines_omnes);

        arborem_legere(bib, ex, resultus.elementum_radix, NIHIL,
                       NIHIL);

        si (   xar_numerus(bib->genera_omnia)         == genera_ante
            && xar_numerus(bib->necessitudines_omnes) == nexus_ante)
        {
            diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XXV,
                ex->stirps, NIHIL,
                "modulus nec genus nec necessitudinem declarat (regula XXV)");
        }
    }

    bib->nexum = FALSUM;  /* lectio nova nexuram novam poscit */

    redde VERUM;
}


/* ==================================================
 * Phasis II - nectere
 * ================================================== */

/* quaerere elementum nominatum in genere aut maioribus:
 * continens NIHIL = liberum directum generis (machina_statuum);
 * clavis = attributum quo elementum nominatur ("nomen" plerumque,
 * "munus" pro termino compagis) */
interior StmlNodus*
in_catena_invenire (
           NaturaGenus* genus,
    constans character* continens,
    constans character* elementum,
    constans character* clavis,
                chorda* quaesitum)
{
    NaturaGenus* g;
            i32  gradus_catenae;

    gradus_catenae = ZEPHYRUM;
    per (g = genus; g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        StmlNodus* sedes;
              i32  numerus;
              i32  i;

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
                   chorda* titulus_n;

                liberum = stml_liberum_ad_indicem(sedes, i);
                si (   !liberum
                    || liberum->genus != STML_NODUS_ELEMENTUM
                    || !chorda_aequalis_literis(*liberum->titulus,
                                             elementum))
                {
                    perge;
                }

                titulus_n = stml_attributum_capere(liberum, clavis);
                si (   titulus_n && chorda_aequalis(*titulus_n,
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
in_membris_invenire (
           NaturaGenus* primus,
           NaturaGenus* etiam,
    constans character* continens,
    constans character* elementum,
    constans character* clavis,
                chorda* quaesitum)
{
    StmlNodus* inventum;

    inventum = NIHIL;
    si (primus)
    {
        inventum = in_catena_invenire(primus, continens, elementum,
                                      clavis, quaesitum);
    }
    si (!inventum && etiam)
    {
        inventum = in_catena_invenire(etiam, continens, elementum,
                                      clavis, quaesitum);
    }

    redde inventum;
}


/* ==================================================
 * Necessitudines - ligatio (spec necessitudines 2026-08-10)
 * ================================================== */

/* scriptura quaevis -> declarationem; sentinella si ambigua */
interior NaturaNecessitudo*
necessitudinem_invenire (
    NaturaBibliotheca* bib,
      constans chorda* scriptura)
{
    vacuum* valor;

    si (!bib->scriptiones)
    {
        redde NIHIL;
    }
    si (!tabula_dispersa_invenire(bib->scriptiones, *scriptura,
                                  &valor))
    {
        redde NIHIL;
    }
    si (valor == (vacuum*)&SENTINELLA_HOMONYMA)
    {
        redde NIHIL;
    }

    redde (NaturaNecessitudo*)valor;
}

/* scripturam in indicem inserere; collisio trans declarationes
 * DUAS = ambiguitas (regula XXI) + sentinella */
interior vacuum
scripturam_inserere (
    NaturaBibliotheca* bib,
    NaturaNecessitudo* declarata,
               chorda* scriptura)
{
    vacuum* prius;

    si (tabula_dispersa_invenire(bib->scriptiones, *scriptura,
                                 &prius))
    {
        si ((NaturaNecessitudo*)prius != declarata)
        {
            tabula_dispersa_inserere(bib->scriptiones, *scriptura,
                                     (vacuum*)&SENTINELLA_HOMONYMA);
            diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XXI,
                declarata->modulus, scriptura,
                "scriptura necessitudinis ambigua (regula XXI)");
        }
        redde;
    }

    tabula_dispersa_inserere(bib->scriptiones, *scriptura,
                             declarata);
}

interior NaturaGenus*
genus_per_chordam (
    NaturaBibliotheca* bib,
      constans chorda* titulus)
{
       vacuum* valor;
    NaturaEns* ens;

    si (!tabula_dispersa_invenire(bib->nomina, *titulus, &valor))
    {
        redde NIHIL;
    }
    ens = (NaturaEns*)valor;
    si (   ens            == &SENTINELLA_HOMONYMA
        || ens->discrimen != NATURA_ENS_GENUS)
    {
        redde NIHIL;
    }

    redde (NaturaGenus*)ens->corpus;
}

/* finis effectivus ("a" aut "ad"): proximus non-'*' in catena
 * sub= necessitudinis; NIHIL = apertum */
interior chorda*
finem_effectivum (
     NaturaNecessitudo* declarata,
    constans character* finis)
{
    NaturaNecessitudo* n;
                  i32  gradus_catenae;

    gradus_catenae = ZEPHYRUM;
    per (n = declarata;
         n && gradus_catenae < NATURA_CATENA_MAXIMA;
         n = n->parens)
    {
        chorda* attr;

        attr = stml_attributum_capere(n->nodus, finis);
        si (attr && !chorda_aequalis_literis(*attr, "*"))
        {
            redde attr;
        }
        gradus_catenae++;
    }

    redde NIHIL;
}

/* genus intra finem nominatum? LENIS ubi nomen non resolvitur
 * (homonyma, res) - custodia, non carcer */
interior b32
genus_subsumitur (
    NaturaBibliotheca* bib,
          NaturaGenus* genus,
      constans chorda* latius)
{
    NaturaGenus* meta;
    NaturaGenus* g;
            i32  gradus_catenae;

    si (chorda_aequalis(*genus->titulus, *latius))
    {
        redde VERUM;
    }
    meta = genus_per_chordam(bib, latius);
    si (!meta)
    {
        redde VERUM;
    }

    gradus_catenae = ZEPHYRUM;
    per (g = genus; g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        si (g == meta)
        {
            redde VERUM;
        }
        gradus_catenae++;
    }

    redde FALSUM;
}

/* sedem (relatio aut terminus) ad necessitudinem ligare:
 * citatio explicita (necessitudo=) aut praesumptio per titulum;
 * XXI citatio fracta, XXII finis excessus, insoluta collecta */
interior vacuum
sedem_ligare (
    NaturaBibliotheca* bib,
       NaturaExemplar* ex,
          NaturaGenus* possessor,
                  Xar* insoluta,
            StmlNodus* sedes,
               chorda* titulus_sedis)
{
               chorda* nex_attr;
    NaturaNecessitudo* ligata;

    nex_attr  = stml_attributum_capere(sedes, "necessitudo");
    ligata    = NIHIL;

    si (nex_attr)
    {
        ligata = necessitudinem_invenire(bib, nex_attr);
        si (!ligata)
        {
            diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XXI,
                ex->stirps, nex_attr,
                "necessitudo citata non resolvit (regula XXI)");
            redde;  /* citatio fracta clamavit - non insoluta quoque */
        }
    }
    alioquin si (titulus_sedis)
    {
        ligata = necessitudinem_invenire(bib, titulus_sedis);
        si (!ligata)
        {
            i32 k;
            b32 iam;

            si (!insoluta)
            {
                redde;
            }
            iam = FALSUM;
            per (k = ZEPHYRUM; k < xar_numerus(insoluta); k++)
            {
                chorda** nomen_k;

                nomen_k = (chorda**)xar_obtinere(insoluta, k);
                si (chorda_aequalis(**nomen_k, *titulus_sedis))
                {
                    iam = VERUM;
                    frange;
                }
            }
            si (!iam)
            {
                chorda** locus;

                locus   = (chorda**)xar_addere(insoluta);
                *locus  = titulus_sedis;
            }
            redde;
        }
    }

    /* regula XXII: sedes ad= intra finem declaratum - et ordo
     * tabulae sedium pro OMNI sede ligata (fundamentum '-sedes') */
    si (ligata)
    {
        NaturaSedesLigata* series;
                   chorda* ad_attr;
                   chorda* nomen_efficax;
                      b32  conversa;

        /* directio: nomen efficax fines commutat - citatio
         * DIRECTIONEM nominat, non familiam (decretum Franis
         * 2026-08-11); sine citatione verbum sedis ipsum
         * nomen est */
        nomen_efficax = nex_attr ? nex_attr : titulus_sedis;
        conversa = (ligata->conversum && nomen_efficax
            && chorda_aequalis(*nomen_efficax,
                                    *ligata->conversum))
                   ? VERUM : FALSUM;

        ad_attr = stml_attributum_capere(sedes, "ad");

        series = (NaturaSedesLigata*)xar_addere(bib->sedes_ligatae);
        series->exemplar = ex;
        series->possessor = possessor;
        series->nodus = sedes;
        series->titulus = titulus_sedis;
        series->ligata = ligata;
        series->conversa = conversa;
        series->ad_attr = ad_attr;
        series->verdictum = NATURA_SEDES_APERTA;

        si (ad_attr && !chorda_aequalis_literis(*ad_attr, "*"))
        {
            constans character* quod;
                        chorda* finis_decl;

            quod        = conversa ? "a" : "ad";
            finis_decl  = finem_effectivum(ligata, quod);
            si (finis_decl)
            {
                     chorda* modulus_attr;
                  NaturaEns* scopus;
                NaturaGenus* scopus_genus;

                modulus_attr = stml_attributum_capere(sedes,
                                                      "modulus");
                scopus = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : ex->stirps,
                    ad_attr);
                scopus_genus = NIHIL;
                si (   scopus
                    && scopus->discrimen == NATURA_ENS_GENUS)
                {
                    scopus_genus = (NaturaGenus*)scopus->corpus;
                }
                alioquin si (   scopus
                             && scopus->discrimen == NATURA_ENS_RES)
                {
                    scopus_genus =
                        ((NaturaRes*)scopus->corpus)->genus_suum;
                }
                si (scopus_genus)
                {
                    si (genus_subsumitur(bib, scopus_genus,
                                         finis_decl))
                    {
                        series->verdictum = NATURA_SEDES_INTRA;
                    }
                    alioquin
                    {
                        series->verdictum = NATURA_SEDES_EXCEDIT;
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS,
                            XXII, ex->stirps, ad_attr,
                            "sedes finem necessitudinis excedit (regula XXII)");
                    }
                }
            }
        }
    }
}

/* nuntius monitorum XX: numerus + nomina prima V (mensura capta) */
interior constans character*
nuntium_insolutarum_fingere (
    Piscina* piscina,
        Xar* insoluta)
{
    character* buffer;
      integer  off;
          i32  k;
          i32  ostensa;

    buffer = (character*)piscina_allocare(piscina, CCLVI);
    si (!buffer)
    {
        redde "nomina relationum insoluta (regula XX)";
    }

    off = sprintf(buffer,
                  "nomina relationum insoluta %d (regula XX):",
                  (integer)xar_numerus(insoluta));
    ostensa = ZEPHYRUM;
    per (k = ZEPHYRUM;
         k < xar_numerus(insoluta) && ostensa < V; k++)
    {
         chorda* nomen_k;
        integer  mensura;

        nomen_k = *(chorda**)xar_obtinere(insoluta, k);
        mensura = (integer)nomen_k->mensura;
        si (mensura > XXXI)
        {
            mensura = XXXI;
        }
        off += sprintf(buffer + off, " %.*s", mensura,
                       (constans character*)nomen_k->datum);
        ostensa++;
    }
    si (xar_numerus(insoluta) > V)
    {
        off += sprintf(buffer + off, " et %d alia",
                       (integer)(xar_numerus(insoluta) - V));
    }
    (vacuum)off;

    redde buffer;
}

/* yyyy | yyyy-mm | yyyy-mm-dd, cum mense <= XII et die <= XXXI */
interior b32
dies_bene_formata (
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
        mensis = (i32)(d->datum[V] - '0') * X
            + (i32)(d->datum[VI] - '0');
        si (mensis < I || mensis > XII)
        {
            redde FALSUM;
        }
    }

    si (d->mensura == X)
    {
        dies_n = (i32)(d->datum[VIII] - '0') * X
            + (i32)(d->datum[IX] - '0');
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
qualificationes_probare (
    NaturaBibliotheca* bib,
       NaturaExemplar* ex,
                  Xar* gradus_noti,
            StmlNodus* nodus,
               chorda* ens)
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
    valens_a   = stml_attributum_capere(nodus, "valens_a");
    valens_ad  = stml_attributum_capere(nodus, "valens_ad");
    si (   valens_a && valens_ad
        && dies_bene_formata(valens_a) && dies_bene_formata(valens_ad)
        && chorda_comparare(*valens_a, *valens_ad) > ZEPHYRUM)
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
arborem_nectere (
    NaturaBibliotheca* bib,
       NaturaExemplar* ex,
                  Xar* gradus_noti,
            StmlNodus* nodus,
          NaturaGenus* genus_c,
            NaturaRes* res_c,
                  Xar* insoluta)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
          StmlNodus* liberum;
             chorda* titulus_e;
             chorda* titulus_n;
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
        pertinens_etiam = res_c ? res_c->genus_etiam :
            (genus_c ? genus_c->parens_etiam : NIHIL);

        si (   chorda_aequalis_literis(*titulus_e, ".genus")
            || chorda_aequalis_literis(*titulus_e, ".species")
            || chorda_aequalis_literis(*titulus_e, ".cultivar"))
        {
            NaturaEns* ens;

            ens = titulus_n ?
                ens_quaerere(bib, ex->stirps, titulus_n) : NIHIL;
            si (ens && ens->discrimen == NATURA_ENS_GENUS)
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                (NaturaGenus*)ens->corpus, NIHIL,
                                insoluta);
            }
            alioquin
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, res_c, insoluta);
            }
        }
        alioquin si (chorda_aequalis_literis(*titulus_e,
                                             "individuum"))
        {
            NaturaEns* ens;

            ens = titulus_n ?
                ens_quaerere(bib, ex->stirps, titulus_n) : NIHIL;
            si (ens && ens->discrimen == NATURA_ENS_RES)
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, (NaturaRes*)ens->corpus,
                                insoluta);
            }
            alioquin
            {
                arborem_nectere(bib, ex, gradus_noti, liberum,
                                genus_c, res_c, insoluta);
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
            modulus_attr  = stml_attributum_capere(liberum, "modulus");
            genus_attr    = stml_attributum_capere(liberum, "genus");
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
                            genus_c, res_c, insoluta);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "relatio"))
        {
            chorda* ad_attr;
            chorda* modulus_attr;
            chorda* externum_attr;

            /* LIGATIO ante omnia - etiam sedes inversae ligantur
             * (aliter cohors inversarum numquam ligabilis esset) */
            sedem_ligare(bib, ex, genus_c, insoluta, liberum,
                         titulus_n);

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
                si (   externum_attr
                    && chorda_aequalis_literis(*externum_attr, "verum"))
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
                alioquin si (scopus->discrimen
                             == NATURA_ENS_NECESSITUDO)
                {
                    /* foramen clausum eadem commissione qua
                     * necessitudines in entia venerunt: ad=
                     * ens petit, non genus relationis */
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        II, ex->stirps, ad_attr,
                        "ad necessitudinem nominat, non ens (regula II)");
                }
            }
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "relatum"))
        {
            chorda* ad_attr;
            chorda* modulus_attr;
            chorda* externum_attr;

            /* regula XI: relatio declarata in catena - sedes
             * relationum AUT terminus compagis (clavis munus) */
            si (   titulus_n && pertinens
                && !in_membris_invenire(pertinens, pertinens_etiam,
                                     "relationes", "relatio",
                                     "nomen", titulus_n)
                && !in_membris_invenire(pertinens, pertinens_etiam,
                                     "termini", "terminus",
                                     "munus", titulus_n))
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
                si (   externum_attr
                    && chorda_aequalis_literis(*externum_attr, "verum"))
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
                alioquin si (scopus->discrimen
                             == NATURA_ENS_NECESSITUDO)
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        II, ex->stirps, ad_attr,
                        "ad necessitudinem nominat, non ens (regula II)");
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
                    "nomen", titulus_n);

                /* regula IX: proprietas AUT machina_statuum */
                si (   !proprietas_n
                    && !in_membris_invenire(pertinens, pertinens_etiam,
                                         NIHIL, "machina_statuum",
                                         "nomen", titulus_n))
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
                si (   genus_attr
                    && chorda_aequalis_literis(*genus_attr, "electio"))
                {
                    chorda valor_textus;
                       i32 n_opt;
                       i32 j;
                       b32 inventum;

                    valor_textus = chorda_praecidere(
                        stml_textus_internus(liberum, bib->piscina));
                    inventum  = FALSUM;
                    n_opt     = stml_numerus_liberorum(proprietas_n);
                    per (j = ZEPHYRUM; j < n_opt; j++)
                    {
                        StmlNodus* optio_n;
                           chorda  optio_textus;

                        optio_n = stml_liberum_ad_indicem(
                            proprietas_n, j);
                        si (   !optio_n
                            || optio_n->genus != STML_NODUS_ELEMENTUM
                            || !chorda_aequalis_literis(
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
            si (   actio_attr && pertinens
                && !in_membris_invenire(pertinens, pertinens_etiam,
                                     "actiones", "actio",
                                     "nomen", actio_attr))
            {
                diagnosticum_addere(bib, NATURA_GRADUS_VULNUS, XIII,
                    ex->stirps, actio_attr,
                    "eventum actionem non declaratam nominat (regula XIII)");
            }

            qualificationes_probare(bib, ex, gradus_noti, liberum,
                pertinens ? pertinens->titulus : NIHIL);
        }
        alioquin si (chorda_aequalis_literis(*titulus_e, "termini"))
        {
            i32 n_t;
            i32 j;

            n_t = stml_numerus_liberorum(liberum);

            per (j = ZEPHYRUM; j < n_t; j++)
            {
                StmlNodus* terminus_j;
                   chorda* munus_j;
                      i32  k;

                terminus_j = stml_liberum_ad_indicem(liberum, j);
                si (   !terminus_j
                    || terminus_j->genus != STML_NODUS_ELEMENTUM
                    || !chorda_aequalis_literis(*terminus_j->titulus,
                                             "terminus"))
                {
                    perge;
                }
                munus_j = stml_attributum_capere(terminus_j,
                                                 "munus");
                si (!munus_j)
                {
                    /* canon clamat (ATTRIBUTUM_DEEST) */
                    perge;
                }

                /* regula XXIII: munus intra compagem unicum */
                per (k = ZEPHYRUM; k < j; k++)
                {
                    StmlNodus* terminus_k;
                       chorda* munus_k;

                    terminus_k = stml_liberum_ad_indicem(liberum,
                                                         k);
                    si (   !terminus_k
                        || terminus_k->genus != STML_NODUS_ELEMENTUM
                        || !chorda_aequalis_literis(
                            *terminus_k->titulus, "terminus"))
                    {
                        perge;
                    }
                    munus_k = stml_attributum_capere(terminus_k,
                                                     "munus");
                    si (   munus_k
                        && chorda_aequalis(*munus_j, *munus_k))
                    {
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS, XXIII,
                            ex->stirps, munus_j,
                            "munus intra compagem geminum (regula XXIII)");
                        frange;
                    }
                }

                /* ligatio termini (clavis munus) */
                sedem_ligare(bib, ex, genus_c, insoluta,
                             terminus_j, munus_j);
            }
        }
        alioquin
        {
            arborem_nectere(bib, ex, gradus_noti, liberum,
                            genus_c, res_c, insoluta);
        }
    }
}

i32
natura_nectere (
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
                 chorda* sub_attr;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
            sub_attr = stml_attributum_capere(g->nodus, "sub");
            si (sub_attr)
            {
                   chorda* modulus_attr;
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

        /* etiam= GENERUM: membrum essentiale duplex portatum a
         * rebus (arbor porphyriana - casus scriptum_conchae) */
        per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
        {
            NaturaGenus* g;
                 chorda* etiam_attr;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
            etiam_attr = stml_attributum_capere(g->nodus, "etiam");
            si (etiam_attr)
            {
                   chorda* modulus_attr;
                NaturaEns* ens;

                modulus_attr = stml_attributum_capere(g->nodus,
                                                      "modulus");
                ens = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : g->modulus,
                    etiam_attr);
                si (ens && ens->discrimen == NATURA_ENS_GENUS)
                {
                    g->parens_etiam = (NaturaGenus*)ens->corpus;
                }
                alioquin
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        IV, g->modulus, g->titulus,
                        "etiam genus non invenit (regula IV)");
                }
            }
        }

        /* etiam= individuorum */
        per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
        {
            NaturaRes* r;
               chorda* etiam_attr;

            r           = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
            etiam_attr  = stml_attributum_capere(r->nodus, "etiam");
            si (etiam_attr)
            {
                   chorda* modulus_attr;
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

        /* necessitudines: parens (sub=), index scripturarum
         * (nomen / conversum / scriptio -> declarationem), et
         * fines hereditati (regula XXII in declarationibus) */
        bib->scriptiones = tabula_dispersa_creare_chorda(
            bib->piscina, CXXVIII);
        per (i = ZEPHYRUM;
             i < xar_numerus(bib->necessitudines_omnes); i++)
        {
            NaturaNecessitudo* declarata;
                       chorda* sub_attr;

            declarata = *(NaturaNecessitudo**)xar_obtinere(
                bib->necessitudines_omnes, i);

            sub_attr = stml_attributum_capere(declarata->nodus,
                                              "sub");
            si (sub_attr)
            {
                   chorda* modulus_attr;
                NaturaEns* ens;

                modulus_attr = stml_attributum_capere(
                    declarata->nodus, "modulus");
                ens = ens_quaerere(bib,
                    modulus_attr ? modulus_attr : declarata->modulus,
                    sub_attr);
                si (   ens
                    && ens->discrimen == NATURA_ENS_NECESSITUDO)
                {
                    declarata->parens =
                        (NaturaNecessitudo*)ens->corpus;
                }
                alioquin
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        IV, declarata->modulus, declarata->titulus,
                        "sub parentem necessitudinis non invenit (regula IV)");
                }
            }

            scripturam_inserere(bib, declarata,
                                declarata->titulus);
            si (declarata->conversum)
            {
                scripturam_inserere(bib, declarata,
                                    declarata->conversum);
            }
            {
                i32 j;
                i32 n_s;

                n_s = stml_numerus_liberorum(declarata->nodus);
                per (j = ZEPHYRUM; j < n_s; j++)
                {
                    StmlNodus* scriptio_n;
                       chorda  textus;
                       chorda* stabilis;

                    scriptio_n = stml_liberum_ad_indicem(
                        declarata->nodus, j);
                    si (   !scriptio_n
                        || scriptio_n->genus != STML_NODUS_ELEMENTUM
                        || !chorda_aequalis_literis(
                            *scriptio_n->titulus, "scriptio"))
                    {
                        perge;
                    }
                    textus = chorda_praecidere(stml_textus_internus(
                        scriptio_n, bib->piscina));
                    si (textus.mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    stabilis = (chorda*)piscina_allocare(
                        bib->piscina, magnitudo(chorda));
                    *stabilis = textus;
                    scripturam_inserere(bib, declarata, stabilis);
                }
            }
        }

        /* fines hereditati: post parentes omnes resolutos */
        per (i = ZEPHYRUM;
             i < xar_numerus(bib->necessitudines_omnes); i++)
        {
             NaturaNecessitudo* declarata;
            constans character* fines[II];
                           i32  f;

            declarata = *(NaturaNecessitudo**)xar_obtinere(
                bib->necessitudines_omnes, i);
            si (!declarata->parens)
            {
                perge;
            }

            fines[ZEPHYRUM]  = "a";
            fines[I]         = "ad";
            per (f = ZEPHYRUM; f < II; f++)
            {
                chorda* meus;
                chorda* latius;

                meus = stml_attributum_capere(declarata->nodus,
                                              fines[f]);
                latius = finem_effectivum(declarata->parens,
                                          fines[f]);
                si (!latius || !meus)
                {
                    perge;  /* parens apertus aut hereditat */
                }
                si (chorda_aequalis_literis(*meus, "*"))
                {
                    diagnosticum_addere(bib, NATURA_GRADUS_VULNUS,
                        XXII, declarata->modulus,
                        declarata->titulus,
                        "necessitudo finem parentis dilatat (regula XXII)");
                }
                alioquin
                {
                    NaturaGenus* g;

                    g = genus_per_chordam(bib, meus);
                    si (g && !genus_subsumitur(bib, g, latius))
                    {
                        diagnosticum_addere(bib,
                            NATURA_GRADUS_VULNUS, XXII,
                            declarata->modulus, declarata->titulus,
                            "necessitudo finem parentis excedit (regula XXII)");
                    }
                }
            }
        }

        /* regula XXIV: fines impares nomina bina postulant.
         * necessitudo sine converso cuius fines effectivi
         * discrepant dimidiam identitatem innominatam habet -
         * sedes citans directionem suam dicere non potest.
         * monitum in DECLARATIONE (res una, monitum unum), non
         * in sedibus */
        per (i = ZEPHYRUM;
             i < xar_numerus(bib->necessitudines_omnes); i++)
        {
            NaturaNecessitudo* declarata;
                       chorda* finis_a;
                       chorda* finis_ad;

            declarata = *(NaturaNecessitudo**)xar_obtinere(
                bib->necessitudines_omnes, i);
            si (declarata->conversum)
            {
                perge;
            }
            finis_a   = finem_effectivum(declarata, "a");
            finis_ad  = finem_effectivum(declarata, "ad");
            si (   (finis_a || finis_ad)
                && (!finis_a || !finis_ad
                || !chorda_aequalis(*finis_a, *finis_ad)))
            {
                diagnosticum_addere(bib, NATURA_GRADUS_MONITUM,
                    XXIV, declarata->modulus, declarata->titulus,
                    "fines impares sine converso - directio innominata (regula XXIV)");
            }
        }

        /* index reversus + custodia catenae cyclicae */
        per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
        {
            NaturaGenus* g;
            NaturaGenus* maior;
                    i32  gradus_catenae;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
            si (g->parens)
            {
                NaturaGenus** locus;

                locus   = (NaturaGenus**)xar_addere(g->parens->liberi);
                *locus  = g;
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

        /* vocabularium gradus_assensus pro regula VI: membra post
         * arborem porphyrianam GENERA sunt (liberi), individua
         * residua quoque admissa (unio) */
        gradus_noti = NIHIL;
        {
               chorda* modulus_i;
               chorda* titulus_g;
            NaturaEns* ens;

            modulus_i = chorda_internare_ex_literis(bib->intern,
                                                    "iudicium");
            titulus_g = chorda_internare_ex_literis(bib->intern,
                                                    "gradus_assensus");
            ens = ens_quaerere(bib, modulus_i, titulus_g);
            si (ens && ens->discrimen == NATURA_ENS_GENUS)
            {
                NaturaGenus* g;
                        i32  j;

                g = (NaturaGenus*)ens->corpus;
                gradus_noti = xar_creare(bib->piscina,
                                         (i32)magnitudo(chorda*));
                per (j = ZEPHYRUM; j < xar_numerus(g->liberi); j++)
                {
                    NaturaGenus*  l;
                         chorda** locus;

                    l = *(NaturaGenus**)xar_obtinere(g->liberi, j);
                    locus = (chorda**)xar_addere(gradus_noti);
                    *locus = l->titulus;
                }
                per (j = ZEPHYRUM; j < xar_numerus(g->res_suae); j++)
                {
                    NaturaRes*  r;
                       chorda** locus;

                    r = *(NaturaRes**)xar_obtinere(g->res_suae, j);
                    locus = (chorda**)xar_addere(gradus_noti);
                    *locus = r->titulus;
                }
            }
        }

        /* percursus regularum per exemplaria; nomina insoluta per
         * exemplar colliguntur et in monitum UNUM aggregantur
         * (regula XX) - modulus semper positus (filtrum -plagula
         * modulum NIHIL incondite ostenderet). Tacet dum nulla
         * necessitudo declarata est: metrum sine norma nihil
         * mensurat. */
        per (i = ZEPHYRUM; i < xar_numerus(bib->exemplaria); i++)
        {
            NaturaExemplar* ex;
                       Xar* insoluta;

            ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, i);
            insoluta = xar_creare(bib->piscina,
                                  (i32)magnitudo(chorda*));
            arborem_nectere(bib, ex, gradus_noti, ex->radix,
                            NIHIL, NIHIL, insoluta);
            si (   xar_numerus(bib->necessitudines_omnes) > ZEPHYRUM
                && insoluta && xar_numerus(insoluta) > ZEPHYRUM)
            {
                diagnosticum_addere(bib, NATURA_GRADUS_MONITUM, XX,
                    ex->stirps, NIHIL,
                    nuntium_insolutarum_fingere(bib->piscina,
                                                insoluta));
            }
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

b32
natura_finem_superponere (
     NaturaBibliotheca* bib,
    constans character* familia,
    constans character* finis,
    constans character* valor)
{
    i32 i;

    si (!bib || bib->nexum || !familia || !finis || !valor)
    {
        redde FALSUM;
    }
    si (   strcmp(finis, "a")  != ZEPHYRUM
        && strcmp(finis, "ad") != ZEPHYRUM)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM;
         i < xar_numerus(bib->necessitudines_omnes); i++)
    {
        NaturaNecessitudo* declarata;
                   chorda* novus;
                      i32  j;
                      i32  n_a;

        declarata = *(NaturaNecessitudo**)xar_obtinere(
            bib->necessitudines_omnes, i);
        si (!chorda_aequalis_literis(*declarata->titulus, familia))
        {
            perge;
        }

        /* attributa internata sunt - monstratorem valoris
         * resedere, numquam rem monstratam rescribere */
        novus = (chorda*)piscina_allocare(bib->piscina,
                                          magnitudo(chorda));
        si (!novus)
        {
            redde FALSUM;
        }
        *novus = chorda_ex_literis(valor, bib->piscina);

        si (declarata->nodus->attributa)
        {
            n_a = xar_numerus(declarata->nodus->attributa);
            per (j = ZEPHYRUM; j < n_a; j++)
            {
                StmlAttributum* attr;

                attr = (StmlAttributum*)xar_obtinere(
                    declarata->nodus->attributa, j);
                si (   attr && attr->titulus
                    && chorda_aequalis_literis(*attr->titulus,
                                            finis))
                {
                    attr->valor = novus;
                    redde VERUM;
                }
            }
        }
        redde stml_attributum_addere(declarata->nodus,
                                     bib->piscina, bib->intern,
                                     finis, valor);
    }

    redde FALSUM;
}

chorda*
natura_finem_effectivum (
     NaturaNecessitudo* declarata,
    constans character* finis)
{
    si (!declarata || !finis)
    {
        redde NIHIL;
    }
    redde finem_effectivum(declarata, finis);
}


/* ==================================================
 * Quaestiones
 * ================================================== */

NaturaGenus*
natura_genus (
     NaturaBibliotheca* bib,
    constans character* titulus)
{
       vacuum* valor;
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
    si (   ens            == &SENTINELLA_HOMONYMA
        || ens->discrimen != NATURA_ENS_GENUS)
    {
        redde NIHIL;
    }

    redde (NaturaGenus*)ens->corpus;
}

NaturaNecessitudo*
natura_necessitudo (
     NaturaBibliotheca* bib,
    constans character* titulus)
{
       vacuum* valor;
    NaturaEns* ens;

    si (!bib || !titulus)
    {
        redde NIHIL;
    }

    /* scriptiones (nectere aedificatae) primae: conversum et
     * scripturae alterae ibi solae resolvunt */
    si (bib->scriptiones)
    {
        si (tabula_dispersa_invenire_literis(bib->scriptiones,
                                             titulus, &valor))
        {
            si (valor == (vacuum*)&SENTINELLA_HOMONYMA)
            {
                redde NIHIL;
            }
            redde (NaturaNecessitudo*)valor;
        }
    }

    si (!tabula_dispersa_invenire_literis(bib->nomina, titulus,
                                          &valor))
    {
        redde NIHIL;
    }

    ens = (NaturaEns*)valor;
    si (   ens            == &SENTINELLA_HOMONYMA
        || ens->discrimen != NATURA_ENS_NECESSITUDO)
    {
        redde NIHIL;
    }

    redde (NaturaNecessitudo*)ens->corpus;
}

NaturaEns*
natura_ens_in (
     NaturaBibliotheca* bib,
    constans character* modulus,
    constans character* titulus)
{
    chorda  clavis;
        i8  buffer[CCLVI];
       i32  mensura_m;
       i32  mensura_t;
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

    clavis.datum    = buffer;
    clavis.mensura  = mensura_m + I + mensura_t;

    si (!tabula_dispersa_invenire(bib->entia, clavis, &valor))
    {
        redde NIHIL;
    }

    redde (NaturaEns*)valor;
}

NaturaGenus*
natura_genus_in (
     NaturaBibliotheca* bib,
    constans character* modulus,
    constans character* titulus)
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
natura_maiores (
    NaturaGenus* genus,
        Piscina* piscina)
{
            Xar* maiores;
    NaturaGenus* g;
            i32  gradus_catenae;

    si (!genus || !piscina)
    {
        redde NIHIL;
    }

    maiores         = xar_creare(piscina, (i32)magnitudo(NaturaGenus*));
    gradus_catenae  = ZEPHYRUM;
    per (g = genus->parens;
         g && gradus_catenae < NATURA_CATENA_MAXIMA;
         g = g->parens)
    {
        NaturaGenus** locus;

        locus   = (NaturaGenus**)xar_addere(maiores);
        *locus  = g;
        gradus_catenae++;
    }

    redde maiores;
}


/* ==================================================
 * Apparatus
 * ================================================== */

interior vacuum
apparatui_contribuere (
            Xar* apparatus,
    NaturaGenus* genus)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(genus->nodus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;
           chorda* titulus_e;

        liberum = stml_liberum_ad_indicem(genus->nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus_e = liberum->titulus;

        si (   chorda_aequalis_literis(*titulus_e, "machina_statuum")
            || chorda_aequalis_literis(*titulus_e, "valor")
            || chorda_aequalis_literis(*titulus_e, "relatum"))
        {
            /* valor/relatum in genere = DETERMINATIO (proprietatem
             * aut relationem hereditatam claudit pro membris
             * omnibus) - apparatus eas ostendit cum auctore
             * (arbor porphyriana 2026-08-10) */
            NaturaApparatusMembrum* membrum;

            membrum = (NaturaApparatusMembrum*)xar_addere(apparatus);
            membrum->nodus = liberum;
            membrum->auctor = genus;
        }
        alioquin si (
                     chorda_aequalis_literis(*titulus_e, "proprietates")

                     || chorda_aequalis_literis(*titulus_e, "partes")

                     || chorda_aequalis_literis(*titulus_e, "actiones")

                     || chorda_aequalis_literis(*titulus_e, "relationes")

                     || chorda_aequalis_literis(*titulus_e, "termini"))
        {
            i32 n_intus;
            i32 j;

            n_intus = stml_numerus_liberorum(liberum);
            per (j = ZEPHYRUM; j < n_intus; j++)
            {
                             StmlNodus* intus;
                NaturaApparatusMembrum* membrum;

                intus = stml_liberum_ad_indicem(liberum, j);
                si (!intus || intus->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }

                membrum = (NaturaApparatusMembrum*)xar_addere(
                    apparatus);
                membrum->nodus   = intus;
                membrum->auctor  = genus;
            }
        }
    }
}

Xar*
natura_apparatus (
    NaturaBibliotheca* bib,
          NaturaGenus* genus,
              Piscina* piscina)
{
            Xar* apparatus;
    NaturaGenus* g;
            i32  gradus_catenae;

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

    /* catena altera: etiam= (membrum essentiale duplex - apparatus
     * AMBARUM catenarum generi debetur, ut rebus antea) */
    si (genus->parens_etiam)
    {
        gradus_catenae = ZEPHYRUM;
        per (g = genus->parens_etiam;
             g && gradus_catenae < NATURA_CATENA_MAXIMA;
             g = g->parens)
        {
            apparatui_contribuere(apparatus, g);
            gradus_catenae++;
        }
    }

    redde apparatus;
}
