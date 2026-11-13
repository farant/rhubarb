/* bibliothecae.c - lineae #include -> plagulae .c interpreti
 * onerandae (vide bibliothecae.h; officina-bibliothecae-spec.md). */

#include "postulata_posix.h"
#include "bibliothecae.h"
#include "aedilis.h"
#include "aedilis_silva.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DIRECTORIUM_SCOPI "build/bibliothecae_scopi"
#define VIA_MAXIMA           1024
#define CAPSA_INDICIS        65536

structura Bibliothecae {
                Piscina* piscina;
              character* radix;
    AedilisConfiguratio* configuratio;
           AedilisSilva* extractor;
};

/* scopus quisque via nova PER PROCESSUM, non per resolutorem:
 * resolutores duo eodem processu vias easdem aliter rescriberent
 * (et reliquias alterius delerent - planta 'scopus relictus') */
hic_manens i32 numerator_scopi = ZEPHYRUM;


/* ==================================================
 * causae
 * ================================================== */

interior vacuum
_causam_ponere (
               Piscina* piscina,
                chorda* causa_out,
    constans character* textus)
{
    si (causa_out != NIHIL)
    {
        *causa_out = chorda_ex_literis(textus, piscina);
    }
}

/* "<caput>" aut "(scopus)" si caput vacuum (annotatio, regula) */
interior vacuum
_caput_appendere (
    ChordaAedificator* a,
               chorda  caput)
{
    si (caput.mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "(scopus)");
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(a, caput);
    }
}


/* ==================================================
 * creatio
 * ================================================== */

Bibliothecae*
bibliothecae_creare (
               Piscina* piscina,
    constans character* radix,
                chorda* causa_out)
{
    Bibliothecae* b;
       character  via_stml[VIA_MAXIMA];

    si (   piscina            == NIHIL || radix == NIHIL
        || strlen(radix) + XX >= magnitudo(via_stml))
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: radix invalida");
        redde NIHIL;
    }
    b = (Bibliothecae*)piscina_allocare(piscina,
        magnitudo(Bibliothecae));
    si (b == NIHIL)
    {
        redde NIHIL;
    }
    memset(b, ZEPHYRUM, magnitudo(Bibliothecae));
    b->piscina = piscina;
    b->radix   = chorda_ut_cstr(chorda_ex_literis(radix, piscina),
        piscina);
    sprintf(via_stml, "%s/aedilis.stml", radix);
    b->configuratio = aedilis_configurationem_legere(piscina,
        via_stml, causa_out);
    si (b->configuratio == NIHIL)
    {
        redde NIHIL;
    }
    b->extractor = aedilis_silva_creare(piscina, b->configuratio,
        NIHIL, NIHIL, NIHIL);
    si (b->extractor == NIHIL)
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: extractor silvae parari nequit");
        redde NIHIL;
    }
    redde b;
}


/* ==================================================
 * resolutio: derivatio bina + index negationis
 * ================================================== */

/* caput implementationem CONVENTIONIS (include/x.h -> lib/x_*)
 * in fructu habet? Obiecta per annotationem corporis non numerantur:
 * fenestra.h corpus fenestra_textus.c fert, sed implementatio eius
 * vera (fenestra_macos.m) sub posix nulla est. */
interior b32
_fons_conventionis (
    constans AedilisFructus* fructus,
                     chorda  caput)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(fructus->obiecta); k++)
    {
        AedilisObiectum* o = (AedilisObiectum*)xar_obtinere(
            fructus->obiecta, k);

        si (   o->origo == AEDILIS_ORIGO_DERIVATUM
            && chorda_aequalis(o->caput, caput))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_viam_addere (
         Xar* viae,
      chorda  via)
{
       i32  k;
    chorda* novum;

    per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(viae, k), via))
        {
            redde VERUM;
        }
    }
    novum = (chorda*)xar_addere(viae);
    si (novum == NIHIL)
    {
        redde FALSUM;
    }
    *novum = via;
    redde VERUM;
}

interior b32
_est_m (
    chorda via)
{
    redde via.mensura >= II
        && via.datum[via.mensura - II] == (i8)'.'
        && via.datum[via.mensura - I] == (i8)'m';
}

/* recusatio "<caput> solum implementationem macOS habet (<via>)" */
interior vacuum
_recusare_macos (
            Piscina* piscina,
             chorda* causa_out,
    AedilisObiectum* o)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CXXVIII);

    _caput_appendere(a, o->caput);
    chorda_aedificator_appendere_literis(a,
        " solum implementationem macOS habet (");
    chorda_aedificator_appendere_chorda(a, o->via);
    chorda_aedificator_appendere_literis(a,
        ") - machinula Objective-C non currit");
    si (causa_out != NIHIL)
    {
        *causa_out = chorda_aedificator_finire(a);
    }
}

/* scopus radici relativus; prima (NIHIL licet) = via fructus prima */
interior Xar*
_resolvere (
          Bibliothecae* b,
    constans character* scopus,
    constans character* prima,
               Piscina* piscina,
                chorda* causa_out)
{
    AedilisFructus* onus;
    AedilisFructus* detectio;
               Xar* viae;
               i32  k;

    onus = aedilis_derivare(piscina, b->configuratio, scopus, "posix",
        aedilis_silva_extrahere, b->extractor, causa_out);
    si (onus == NIHIL)
    {
        redde NIHIL;
    }
    detectio = aedilis_derivare(piscina, b->configuratio, scopus,
        NIHIL, aedilis_silva_extrahere, b->extractor, causa_out);
    si (detectio == NIHIL)
    {
        redde NIHIL;
    }

    /* I. capita quorum implementatio sola .m est: in detectione
     * .m, in onere (posix) caput sine implementatione conventionis */
    per (k = ZEPHYRUM; k < xar_numerus(detectio->obiecta); k++)
    {
        AedilisObiectum* o = (AedilisObiectum*)xar_obtinere(
            detectio->obiecta, k);

        si (   _est_m(o->via)
            && !_fons_conventionis(onus, o->caput))
        {
            _recusare_macos(piscina, causa_out, o);
            redde NIHIL;
        }
    }

    /* II. capita citata nusquam inventa */
    si (xar_numerus(onus->inresolutae) > ZEPHYRUM)
    {
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            LXIV);

        chorda_aedificator_appendere_literis(a, "caput non inventum: ");
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(onus->inresolutae, ZEPHYRUM));
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(a);
        }
        redde NIHIL;
    }

    /* III. fontes vendoris */
    si (xar_numerus(onus->vendores) > ZEPHYRUM)
    {
        AedilisVendor* v = (AedilisVendor*)xar_obtinere(onus->vendores,
            ZEPHYRUM);
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            LXIV);

        chorda_aedificator_appendere_literis(a,
            "fons vendoris non interpretatur: ");
        chorda_aedificator_appendere_chorda(a, v->fons);
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(a);
        }
        redde NIHIL;
    }

    /* IV. obiecta oneris: absens, .m, magnitudo */
    viae = xar_creare(piscina, (i32)magnitudo(chorda));
    si (viae == NIHIL)
    {
        redde NIHIL;
    }
    si (   prima != NIHIL
        && !_viam_addere(viae, chorda_ex_literis(prima, piscina)))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(onus->obiecta); k++)
    {
           AedilisObiectum* o = (AedilisObiectum*)xar_obtinere(
               onus->obiecta, k);
                 character  plena[VIA_MAXIMA];
            memoriae_index  mensura;
         ChordaAedificator* a;

        si (o->absens)
        {
            a = chorda_aedificator_creare(piscina, LXIV);
            chorda_aedificator_appendere_literis(a,
                "obiectum absens (generatum nondum?): ");
            chorda_aedificator_appendere_chorda(a, o->via);
            chorda_aedificator_appendere_literis(a, ", inclusum per ");
            _caput_appendere(a, o->caput);
            si (causa_out != NIHIL)
            {
                *causa_out = chorda_aedificator_finire(a);
            }
            redde NIHIL;
        }
        si (_est_m(o->via))
        {
            _recusare_macos(piscina, causa_out, o);
            redde NIHIL;
        }
        si (strlen(b->radix) + o->via.mensura + II >= magnitudo(plena))
        {
            _causam_ponere(piscina, causa_out,
                "bibliothecae: via nimis longa");
            redde NIHIL;
        }
        sprintf(plena, "%s/%.*s", b->radix, (int)o->via.mensura,
            (constans character*)o->via.datum);
        mensura = filum_mensura(plena);
        si (mensura > (memoriae_index)BIBLIOTHECAE_MENSURA_MAXIMA)
        {
            character numerus[XXXII];

            sprintf(numerus, "%lu", (unsigned long)mensura);
            a = chorda_aedificator_creare(piscina, CXXVIII);
            chorda_aedificator_appendere_chorda(a, o->via);
            chorda_aedificator_appendere_literis(a, " nimis magna (");
            chorda_aedificator_appendere_literis(a, numerus);
            chorda_aedificator_appendere_literis(a,
                " octeti > 4194304), inclusa per ");
            _caput_appendere(a, o->caput);
            si (causa_out != NIHIL)
            {
                *causa_out = chorda_aedificator_finire(a);
            }
            redde NIHIL;
        }
        si (!_viam_addere(viae, o->via))
        {
            redde NIHIL;
        }
    }
    redde viae;
}

Xar*
bibliothecae_ex_textu (
     Bibliothecae* b,
           chorda  textus,
          Piscina* piscina,
           chorda* causa_out)
{
    character  directorium[VIA_MAXIMA];
    character  relativa[VIA_MAXIMA];
    character  plena[VIA_MAXIMA];
          Xar* viae;

    si (b == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (strlen(b->radix) + C >= magnitudo(plena))
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: radix nimis longa");
        redde NIHIL;
    }
    sprintf(directorium, "%s/%s", b->radix, DIRECTORIUM_SCOPI);
    si (!filum_directorium_creare_si_necesse(directorium))
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: directorium scoporum creari nequit");
        redde NIHIL;
    }
    sprintf(relativa, "%s/scopus_%ld_%lu.c", DIRECTORIUM_SCOPI,
        (long)getpid(), (unsigned long)numerator_scopi);
    numerator_scopi++;
    sprintf(plena, "%s/%s", b->radix, relativa);
    si (!filum_scribere(plena, textus))
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: scopus scribi nequit");
        redde NIHIL;
    }
    viae = _resolvere(b, relativa, NIHIL, piscina, causa_out);
    (vacuum)filum_delere(plena);
    redde viae;
}

Xar*
bibliothecae_ex_via (
          Bibliothecae* b,
    constans character* via,
               Piscina* piscina,
                chorda* causa_out)
{
    si (b == NIHIL || via == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    redde _resolvere(b, via, via, piscina, causa_out);
}


/* ==================================================
 * mundus totus: clausura posix omnium capitum include/
 * ================================================== */

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (s32)chorda_comparare(*(constans chorda*)a,
        *(constans chorda*)b);
}

/* "include/<caput>: <causa>" in omissa */
interior vacuum
_omissum_addere (
          Xar* omissa,
      Piscina* piscina,
       chorda  caput,
       chorda  causa)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CXXVIII);
               chorda* novum;

    chorda_aedificator_appendere_literis(a, "include/");
    chorda_aedificator_appendere_chorda(a, caput);
    chorda_aedificator_appendere_literis(a, ": ");
    chorda_aedificator_appendere_chorda(a, causa);
    novum = (chorda*)xar_addere(omissa);
    si (novum != NIHIL)
    {
        *novum = chorda_aedificator_finire(a);
    }
}

Xar*
bibliothecae_mundum_totum (
     Bibliothecae*  b,
          Piscina*  piscina,
              Xar** omissa_out,
           chorda*  causa_out)
{
               character  directorium[VIA_MAXIMA];
     DirectoriumIterator* iter;
    DirectoriumIntroitus* e;
                     Xar* viae;
                     Xar* omissa;
                     Xar* capita;
                     i32  k;

    si (b == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (strlen(b->radix) + XVI >= magnitudo(directorium))
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: radix nimis longa");
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(chorda));
    omissa  = xar_creare(piscina, (i32)magnitudo(chorda));
    capita  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (viae == NIHIL || omissa == NIHIL || capita == NIHIL)
    {
        redde NIHIL;
    }

    /* capita include/ *.h (ordine alphabetico: omissa quoque
     * determinata) */
    sprintf(directorium, "%s/include", b->radix);
    iter = directorium_iterator_aperire(directorium, piscina);
    si (iter == NIHIL)
    {
        _causam_ponere(piscina, causa_out,
            "bibliothecae: include/ legi nequit");
        redde NIHIL;
    }
    dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
    {
        chorda* novum;

        si (   e->genus != INTROITUS_FILUM || e->titulus.mensura < III
            || e->titulus.datum[e->titulus.mensura - II] != (i8)'.'
            || e->titulus.datum[e->titulus.mensura - I] != (i8)'h')
        {
            perge;
        }
        novum = (chorda*)xar_addere(capita);
        si (novum == NIHIL)
        {
            directorium_iterator_claudere(iter);
            redde NIHIL;
        }
        *novum = chorda_transcribere(e->titulus, piscina);
    }
    directorium_iterator_claudere(iter);
    si (xar_numerus(capita) > I)
    {
        xar_ordinare(capita, _chordas_comparare);
    }

    /* derivatio per caput in piscina temporaria: arbores aedilis
     * cum ea pereunt, viae solae in piscinam vocantis transcribuntur
     * (extractor tamen parsuras per viam in b->piscina servat) */
    per (k = ZEPHYRUM; k < xar_numerus(capita); k++)
    {
             chorda  caput = *(chorda*)xar_obtinere(capita, k);
            Piscina* temporaria;
  ChordaAedificator* textus;
             chorda  causa;
                Xar* clausura;
                i32  j;

        temporaria = piscina_generare_dynamicum("mundus_totus",
            65536);
        si (temporaria == NIHIL)
        {
            redde NIHIL;
        }
        textus = chorda_aedificator_creare(temporaria, LXIV);
        chorda_aedificator_appendere_literis(textus, "#include \"");
        chorda_aedificator_appendere_chorda(textus, caput);
        chorda_aedificator_appendere_literis(textus, "\"\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        clausura = bibliothecae_ex_textu(b,
            chorda_aedificator_finire(textus), temporaria, &causa);
        si (clausura == NIHIL)
        {
            _omissum_addere(omissa, piscina, caput, causa);
        }
        alioquin
        {
            per (j = ZEPHYRUM; j < xar_numerus(clausura); j++)
            {
                chorda via       = *(chorda*)xar_obtinere(clausura, j);
                   b32 praesens  = FALSUM;
                   i32 m;

                per (m = ZEPHYRUM; m < xar_numerus(viae); m++)
                {
                    si (chorda_aequalis(*(chorda*)xar_obtinere(viae,
                            m), via))
                    {
                        praesens = VERUM;
                        frange;
                    }
                }
                si (!praesens)
                {
                    chorda* novum = (chorda*)xar_addere(viae);

                    si (novum == NIHIL)
                    {
                        piscina_destruere(temporaria);
                        redde NIHIL;
                    }
                    *novum = chorda_transcribere(via, piscina);
                }
            }
        }
        piscina_destruere(temporaria);
    }
    si (xar_numerus(viae) > I)
    {
        xar_ordinare(viae, _chordas_comparare);
    }
    si (omissa_out != NIHIL)
    {
        *omissa_out = omissa;
    }
    redde viae;
}


/* ==================================================
 * sedes functionis ex indice symbolorum
 * ================================================== */

interior b32
_incipit (
                chorda  s,
    constans character* praefixum)
{
    memoriae_index m = strlen(praefixum);

    redde s.mensura >= m && memcmp(s.datum, praefixum, m) == ZEPHYRUM;
}

interior b32
_terminatur (
                chorda  s,
    constans character* suffixum)
{
    memoriae_index m = strlen(suffixum);

    redde s.mensura >= m
        && memcmp(s.datum + s.mensura - m, suffixum, m) == ZEPHYRUM;
}

/* linea indicis: "<functio>\tsedes\tfunctio\t<via>\t..." -> via
 * capitis (include/x.h) aut corporis (lib/x.c) colligitur */
interior vacuum
_lineam_iudicare (
              i8* linea,
             s32  mensura,
     constans i8* praefixum,
             s32  mensura_praefixi,
         Piscina* piscina,
             Xar* capita,
             Xar* corpora)
{
    chorda via;
       s32 finis;

    si (   mensura <= mensura_praefixi
        || memcmp(linea, praefixum, (memoriae_index)mensura_praefixi)
            != ZEPHYRUM)
    {
        redde;
    }
    finis = mensura_praefixi;
    dum (finis < mensura && linea[finis] != (i8)'\t')
    {
        finis++;
    }
    via = chorda_ex_buffer(linea + mensura_praefixi,
        (i32)(finis - mensura_praefixi));
    si (_incipit(via, "include/") && _terminatur(via, ".h"))
    {
        (vacuum)_viam_addere(capita, chorda_transcribere(via, piscina));
    }
    alioquin si (_incipit(via, "lib/") && _terminatur(via, ".c"))
    {
        (vacuum)_viam_addere(corpora, chorda_transcribere(via,
            piscina));
    }
}

interior vacuum
_candidatos_nominare (
    ChordaAedificator* a,
                  Xar* viae)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
    {
        chorda_aedificator_appendere_literis(a, k == ZEPHYRUM ? " "
            : ", ");
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(viae, k));
    }
}

BibliothecaeSedes
bibliothecae_sedem_quaerere (
    constans character* via_indicis,
    constans character* functio,
               Piscina* piscina,
                chorda* via_out,
                chorda* causa_out)
{
     FilumOctetorumLector* lector;
                       i8* capsa;
                       i8* praefixum;
                      s32  mensura_praefixi;
                      s32  retenti = ZEPHYRUM;
                      Xar* capita;
                      Xar* corpora;
                      Xar* electi;

    si (via_indicis == NIHIL || functio == NIHIL || piscina == NIHIL)
    {
        redde BIBLIOTHECAE_SEDES_IGNOTA;
    }
    capita   = xar_creare(piscina, (i32)magnitudo(chorda));
    corpora  = xar_creare(piscina, (i32)magnitudo(chorda));
    capsa   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(II * CAPSA_INDICIS));
    /* "<functio>\tsedes\tfunctio\t" = functio + XV octeti */
    praefixum = (i8*)piscina_allocare(piscina,
        (memoriae_index)strlen(functio) + XVI);
    si (   capita    == NIHIL || corpora == NIHIL || capsa == NIHIL
        || praefixum == NIHIL)
    {
        redde BIBLIOTHECAE_SEDES_IGNOTA;
    }
    sprintf((character*)praefixum, "%s\tsedes\tfunctio\t", functio);
    mensura_praefixi  = (s32)strlen((character*)praefixum);
    lector            = filum_octeti_aperire(via_indicis, piscina);
    si (lector == NIHIL)
    {
        _causam_ponere(piscina, causa_out,
            "index symbolorum absens (./silva/nexus.sh -renovare)");
        redde BIBLIOTHECAE_SEDES_IGNOTA;
    }
    /* per partes: capsa = [retenti (linea fracta)][lecti] */
    per (;;)
    {
        s32 lecti;
        s32 initium = ZEPHYRUM;
        s32 k;

        lecti = filum_octeti_legere(lector, capsa + retenti,
            (i32)CAPSA_INDICIS);
        si (lecti < ZEPHYRUM)
        {
            frange;
        }
        si (lecti == ZEPHYRUM)
        {
            si (retenti > ZEPHYRUM)
            {
                _lineam_iudicare(capsa, retenti, praefixum,
                    mensura_praefixi, piscina, capita, corpora);
            }
            frange;
        }
        lecti += retenti;
        per (k = ZEPHYRUM; k < lecti; k++)
        {
            si (capsa[k] == (i8)'\n')
            {
                _lineam_iudicare(capsa + initium, k - initium,
                    praefixum, mensura_praefixi, piscina, capita,
                    corpora);
                initium = k + I;
            }
        }
        retenti = lecti - initium;
        si (retenti >= (s32)CAPSA_INDICIS)
        {
            retenti = ZEPHYRUM;   /* linea immanis: omittitur */
        }
        alioquin si (retenti > ZEPHYRUM)
        {
            memmove(capsa, capsa + initium, (memoriae_index)retenti);
        }
    }
    filum_octeti_claudere(lector);

    /* caput unum vincit (corpora variantium sub eo licent); sine
     * capite corpus unum */
    electi = xar_numerus(capita) > ZEPHYRUM ? capita : corpora;
    si (xar_numerus(electi) == ZEPHYRUM)
    {
        _causam_ponere(piscina, causa_out,
            "functio in indice non inventa");
        redde BIBLIOTHECAE_SEDES_IGNOTA;
    }
    si (xar_numerus(electi) > I)
    {
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            CXXVIII);

        chorda_aedificator_appendere_literis(a, "functio ambigua:");
        _candidatos_nominare(a, electi);
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(a);
        }
        redde BIBLIOTHECAE_SEDES_AMBIGUA;
    }
    si (via_out != NIHIL)
    {
        *via_out = *(chorda*)xar_obtinere(electi, ZEPHYRUM);
    }
    redde BIBLIOTHECAE_SEDES_INVENTA;
}
