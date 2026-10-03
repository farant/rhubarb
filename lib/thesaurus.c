/* thesaurus.c - thesaurus contentorum (fabrica plan 2 T3). Vide
 * thesaurus.h.
 *
 * Omnia per filum (lectiones in libro notantur; iudex fabricae
 * radicem thesauri e vestigiis eicit - cache non est ingressus) et
 * sigillum. Hex in disco: LXIV minusculae, numquam truncatae. */
#include "postulata_posix.h"
#include "thesaurus.h"
#include "filum.h"
#include "iter_directoria.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VIA_MAXIMA DXII

structura Thesaurus {
      Piscina* piscina;
    character  radix[VIA_MAXIMA];
          i32  ratio;
          i32  lectiones;
          b32  generatio_parata;
    character  generatio[VIA_MAXIMA];
};

/* numerus scripturarum temporariarum per processum: nomina unica */
hic_manens i32 _temporaria   = ZEPHYRUM;

/* via <radix>/<genus>/<II hex>/<LXII hex>; directorium in 'dir_out'
 * si non NIHIL */
interior vacuum
_viam_struere (
              Thesaurus* thesaurus,
     constans character* genus,
     constans  Sigillum* sigillum,
              character* via_out,
              character* dir_out)
{
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(sigillum, hex);
    sprintf(via_out, "%s/%s/%.2s/%s", thesaurus->radix, genus, hex,
        hex + II);
    si (dir_out != NIHIL)
    {
        sprintf(dir_out, "%s/%s/%.2s", thesaurus->radix, genus, hex);
    }
}

/* SCRIPTURA ATOMICA: temporarium in directorio eodem (pid, numerus),
 * deinde rename. Lector numquam plagulam dimidiam videt. */
interior b32
_atomice_scribere (
    constans character* directorium,
    constans character* via,
                chorda  contentum)
{
    character temporarium[VIA_MAXIMA];

    si (!filum_directorium_creare_cum_parentibus(directorium))
    {
        redde FALSUM;
    }
    _temporaria++;
    sprintf(temporarium, "%s/.temporarium.%ld.%u", directorium,
        (longus)getpid(), (insignatus integer)_temporaria);
    si (!filum_scribere(temporarium, contentum))
    {
        (vacuum)filum_delere(temporarium);
        redde FALSUM;
    }
    si (!filum_movere(temporarium, via))
    {
        (vacuum)filum_delere(temporarium);
        redde FALSUM;
    }
    redde VERUM;
}

interior s32
_hex_valor (
    character c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

/* LXIV hex minusculae -> sigillum; FALSUM si forma fracta */
interior b32
_ex_hex (
        constans i8* hex,
           Sigillum* sigillum_out)
{
    i32 i;
    s32 superior;
    s32 inferior;

    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        superior = _hex_valor((character)hex[i * II]);
        inferior = _hex_valor((character)hex[i * II + I]);
        si (superior < ZEPHYRUM || inferior < ZEPHYRUM)
        {
            redde FALSUM;
        }
        sigillum_out->octeti[i] = (i8)(superior * XVI + inferior);
    }
    redde VERUM;
}

/* plagula blobi verificata: contenta sigillum suum ferunt */
interior b32
_blobum_verificare (
    constans character* via,
    constans  Sigillum* sigillum,
               Piscina* piscina)
{
      chorda contentum;
    Sigillum computatum;

    contentum  = filum_legere_totum(via, piscina);
    computatum = sigillum_computare(contentum.datum,
        (memoriae_index)contentum.mensura);
    redde sigillum_aequale(&computatum, sigillum);
}

Thesaurus*
thesaurus_aperire (
    constans character* radix,
               Piscina* piscina)
{
    Thesaurus* thesaurus;

    si (radix == NIHIL || strlen(radix) + CXXVIII >= VIA_MAXIMA)
    {
        redde NIHIL;
    }
    si (!filum_directorium_creare_cum_parentibus(radix))
    {
        redde NIHIL;
    }
    thesaurus = (Thesaurus*)piscina_allocare(piscina,
        magnitudo(Thesaurus));
    si (thesaurus == NIHIL)
    {
        redde NIHIL;
    }
    thesaurus->piscina           = piscina;
    strcpy(thesaurus->radix, radix);
    thesaurus->ratio             = THESAURUS_VERIFICATIO_RATIO;
    thesaurus->lectiones         = ZEPHYRUM;
    thesaurus->generatio_parata  = FALSUM;
    thesaurus->generatio[0]      = '\0';
    redde thesaurus;
}

vacuum
thesaurus_verificationem_ponere (
    Thesaurus* thesaurus,
          i32  ratio)
{
    thesaurus->ratio = ratio;
}

b32
thesaurus_ponere (
    Thesaurus* thesaurus,
       chorda  octeti,
     Sigillum* sigillum_out)
{
    character via[VIA_MAXIMA];
    character directorium[VIA_MAXIMA];

    *sigillum_out = sigillum_computare(octeti.datum,
        (memoriae_index)octeti.mensura);
    _viam_struere(thesaurus, "blobi", sigillum_out, via, directorium);
    /* praesens: numquam rescribitur (nomen = contenta) */
    si (filum_existit(via))
    {
        redde VERUM;
    }
    redde _atomice_scribere(directorium, via, octeti);
}

b32
thesaurus_via (
             Thesaurus* thesaurus,
    constans  Sigillum* sigillum,
               Piscina* piscina,
                chorda* via_out)
{
    character via[VIA_MAXIMA];

    _viam_struere(thesaurus, "blobi", sigillum, via, NIHIL);
    si (!filum_existit(via))
    {
        redde FALSUM;
    }
    /* specimen: lectio una in 'ratio' verificatur (A2) */
    si (thesaurus->ratio > ZEPHYRUM)
    {
        thesaurus->lectiones++;
        si (   thesaurus->lectiones % thesaurus->ratio == ZEPHYRUM
            && !_blobum_verificare(via, sigillum, piscina))
        {
            (vacuum)filum_delere(via);
            redde FALSUM;
        }
    }
    *via_out = chorda_ex_literis(via, piscina);
    redde VERUM;
}

b32
thesaurus_actio_ponere (
             Thesaurus* thesaurus,
    constans  Sigillum* clavis,
    constans       Xar* sigilla_exituum)
{
    character  via[VIA_MAXIMA];
    character  directorium[VIA_MAXIMA];
    character  hex[SIGILLUM_HEX_MENSURA];
           i8* buffer;
       chorda  contentum;
       chorda  praesens;
          i32  i;
          i32  numerus;

    numerus = xar_numerus(sigilla_exituum);
    buffer  = (i8*)piscina_allocare(thesaurus->piscina,
        (memoriae_index)(numerus * LXV + I));
    si (buffer == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        sigillum_hex((constans Sigillum*)xar_obtinere(sigilla_exituum,
            i),
            hex);
        memcpy(buffer + i * LXV, hex, LXIV);
        buffer[i * LXV + LXIV] = '\n';
    }
    contentum.datum    = buffer;
    contentum.mensura  = numerus * LXV;
    _viam_struere(thesaurus, "actiones", clavis, via, directorium);
    /* eadem: numquam rescribitur */
    praesens = filum_legere_totum(via, thesaurus->piscina);
    si (   praesens.mensura == contentum.mensura
        && (contentum.mensura == ZEPHYRUM
            || memcmp(praesens.datum, contentum.datum,
            (memoriae_index)contentum.mensura) == ZEPHYRUM)
        && filum_existit(via))
    {
        redde VERUM;
    }
    redde _atomice_scribere(directorium, via, contentum);
}

b32
thesaurus_actio_capere (
             Thesaurus*  thesaurus,
    constans  Sigillum*  clavis,
               Piscina*  piscina,
                   Xar** sigilla_out)
{
    character  via[VIA_MAXIMA];
       chorda  contentum;
          Xar* sigilla;
          i32  i;

    _viam_struere(thesaurus, "actiones", clavis, via, NIHIL);
    si (!filum_existit(via))
    {
        redde FALSUM;
    }
    contentum = filum_legere_totum(via, piscina);
    si (contentum.mensura % LXV != ZEPHYRUM)
    {
        redde FALSUM;
    }
    sigilla = xar_creare(piscina, (i32)magnitudo(Sigillum));
    si (sigilla == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < contentum.mensura; i += LXV)
    {
        Sigillum* sigillum;

        si (contentum.datum[i + LXIV] != '\n')
        {
            redde FALSUM;
        }
        sigillum = (Sigillum*)xar_addere(sigilla);
        si (   sigillum == NIHIL
            || !_ex_hex(contentum.datum + i, sigillum))
        {
            redde FALSUM;
        }
    }
    *sigilla_out = sigilla;
    redde VERUM;
}

/* GENERATIO = cursus ordinans (bin/fabrica, porta), non processus:
 * THESAURUS_GENERATIO (a tempore UTC incipiens: ordo nominum = ordo
 * temporis) a cursu ponitur; processus omnes eius in indicem eundem
 * appendunt (linea LXV octetorum, O_APPEND). Absens aut forma mala:
 * nullus index - processus solitarius generationes veras e fenestra
 * purgationis non expellit (claves eius tum non servantur).
 * getenv CRUDUM consulto: thesaurus cache est, non ingressus - in
 * libro lectionum non notatur (tools/lectiones_lint.sh exemptio). */
interior b32
_generationem_parare (
    Thesaurus* thesaurus)
{
    constans character* titulus;
             character  directorium[VIA_MAXIMA];
                   i32  i;

    titulus = getenv("THESAURUS_GENERATIO");
    si (titulus == NIHIL || titulus[0] == '\0' || strlen(titulus) > CXX)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c;

        c = titulus[i];
        si (!(   (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
              || (c >= 'A' && c <= 'Z') || c == '-' || c == '_'))
        {
            redde FALSUM;
        }
    }
    sprintf(directorium, "%s/generationes", thesaurus->radix);
    si (!filum_directorium_creare_cum_parentibus(directorium))
    {
        redde FALSUM;
    }
    sprintf(thesaurus->generatio, "%s/%s.lst", directorium, titulus);
    redde VERUM;
}

vacuum
thesaurus_generationem_notare (
             Thesaurus* thesaurus,
    constans  Sigillum* clavis)
{
    character linea[SIGILLUM_HEX_MENSURA + I];

    si (!thesaurus->generatio_parata)
    {
        si (!_generationem_parare(thesaurus))
        {
            redde;
        }
        thesaurus->generatio_parata = VERUM;
    }
    sigillum_hex(clavis, linea);
    linea[LXIV]  = '\n';
    linea[LXV]   = '\0';
    (vacuum)filum_appendere_literis(thesaurus->generatio, linea);
}

/* ---------------------------------------------------- purgatio */

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

/* nomina plagularum directorii (sine . et ..) */
interior Xar*
_nomina (
    constans character* directorium,
               Piscina* piscina)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
                     Xar* nomina;

    nomina    = xar_creare(piscina, (i32)magnitudo(chorda));
    iterator  = directorium_iterator_aperire(directorium, piscina);
    si (iterator == NIHIL || nomina == NIHIL)
    {
        redde nomina;
    }
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        *(chorda*)xar_addere(nomina) = chorda_transcribere(
            introitus->titulus, piscina);
    }
    directorium_iterator_claudere(iterator);
    redde nomina;
}

/* claves vivae (ex indicibus servatis) in tabulam; blobi vivi ex
 * actionibus earum */
interior vacuum
_vivos_colligere (
             Thesaurus* thesaurus,
    constans character* via_indicis,
        TabulaDispersa* claves,
        TabulaDispersa* blobi,
               Piscina* piscina)
{
      chorda index;
         i32 i;

    index = filum_legere_totum(via_indicis, piscina);
    per (i = ZEPHYRUM; i + LXV <= index.mensura; i += LXV)
    {
          chorda  hex;
        Sigillum  clavis;
             Xar* exitus;
             i32  j;

        hex.datum    = index.datum + i;
        hex.mensura  = LXIV;
        si (   tabula_dispersa_continet(claves, hex)
            || !_ex_hex(hex.datum, &clavis))
        {
            perge;
        }
        (vacuum)tabula_dispersa_inserere(claves, hex, NIHIL);
        si (!thesaurus_actio_capere(thesaurus, &clavis, piscina,
            &exitus))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(exitus); j++)
        {
            character* textus;

            textus = (character*)piscina_allocare(piscina,
                SIGILLUM_HEX_MENSURA);
            sigillum_hex((constans Sigillum*)xar_obtinere(exitus, j),
                textus);
            (vacuum)tabula_dispersa_inserere(blobi,
                chorda_ex_literis(textus, piscina), NIHIL);
        }
    }
}

/* genus (actiones|blobi): plagulae quarum hex (II + LXII) non viva
 * delentur; temporaria relicta quoque. blobi sub 'verificare'
 * verificantur. */
interior i32
_genus_purgare (
             Thesaurus* thesaurus,
    constans character* genus,
        TabulaDispersa* vivi,
                   b32  verificare,
               Piscina* piscina)
{
    character  directorium[VIA_MAXIMA];
    character  sub[VIA_MAXIMA];
    character  via[VIA_MAXIMA];
    character  hex[SIGILLUM_HEX_MENSURA];
          Xar* praefixa;
          i32  deleta;
          i32  i;

    deleta = ZEPHYRUM;
    sprintf(directorium, "%s/%s", thesaurus->radix, genus);
    praefixa = _nomina(directorium, piscina);
    per (i = ZEPHYRUM; praefixa != NIHIL && i < xar_numerus(praefixa);
         i++)
    {
        chorda  praefixum;
           Xar* plagulae;
           i32  j;

        praefixum = *(chorda*)xar_obtinere(praefixa, i);
        si (praefixum.mensura != II)
        {
            perge;
        }
        sprintf(sub, "%s/%.2s", directorium,
            (character*)praefixum.datum);
        plagulae = _nomina(sub, piscina);
        per (j = ZEPHYRUM; plagulae != NIHIL
            && j < xar_numerus(plagulae);
             j++)
        {
              chorda titulus;
              chorda totum;
            Sigillum sigillum;

            titulus = *(chorda*)xar_obtinere(plagulae, j);
            sprintf(via, "%s/%.*s", sub, (integer)titulus.mensura,
                (character*)titulus.datum);
            si (titulus.mensura != LXII)
            {
                /* temporarium relictum (scriptor mortuus) */
                si (   titulus.mensura > ZEPHYRUM
                    && titulus.datum[0] == '.')
                {
                    si (filum_delere(via))
                    {
                        deleta++;
                    }
                }
                perge;
            }
            memcpy(hex, praefixum.datum, II);
            memcpy(hex + II, titulus.datum, LXII);
            hex[LXIV]      = '\0';
            totum.datum    = (i8*)hex;
            totum.mensura  = LXIV;
            si (!tabula_dispersa_continet(vivi, totum))
            {
                si (filum_delere(via))
                {
                    deleta++;
                }
                perge;
            }
            si (   verificare
                && _ex_hex((i8*)hex, &sigillum)
                && !_blobum_verificare(via, &sigillum, piscina))
            {
                si (filum_delere(via))
                {
                    deleta++;
                }
            }
        }
    }
    redde deleta;
}

b32
thesaurus_purgare (
    Thesaurus* thesaurus,
          i32  generationes_servandae,
          b32  verificare,
          i32* deleta_out)
{
          Piscina* piscina;
        character  directorium[VIA_MAXIMA];
        character  via[VIA_MAXIMA];
              Xar* omnia;
              Xar* indices;
   TabulaDispersa* claves;
   TabulaDispersa* blobi;
              i32  deleta;
              i32  i;
              i32  primus_servatus;

    piscina  = thesaurus->piscina;
    deleta   = ZEPHYRUM;
    sprintf(directorium, "%s/generationes", thesaurus->radix);
    omnia    = _nomina(directorium, piscina);
    indices  = xar_creare(piscina, (i32)magnitudo(chorda));
    claves   = tabula_dispersa_creare_chorda(piscina, MXXIV);
    blobi    = tabula_dispersa_creare_chorda(piscina, MXXIV);
    si (   omnia == NIHIL || indices == NIHIL || claves == NIHIL
        || blobi == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(omnia); i++)
    {
        chorda titulus;

        titulus = *(chorda*)xar_obtinere(omnia, i);
        si (   titulus.mensura > IV
            && memcmp(titulus.datum + titulus.mensura - IV, ".lst", IV)
            == ZEPHYRUM)
        {
            *(chorda*)xar_addere(indices) = titulus;
        }
    }
    /* nomina incipiunt a tempore: ordo nominum = ordo temporis */
    xar_ordinare(indices, _chordas_comparare);
    primus_servatus = ZEPHYRUM;
    si ((i32)xar_numerus(indices) > generationes_servandae)
    {
        primus_servatus = xar_numerus(indices) - generationes_servandae;
    }
    per (i = ZEPHYRUM; i < xar_numerus(indices); i++)
    {
        chorda titulus;

        titulus = *(chorda*)xar_obtinere(indices, i);
        sprintf(via, "%s/%.*s", directorium, (integer)titulus.mensura,
            (character*)titulus.datum);
        si (i < primus_servatus)
        {
            si (filum_delere(via))
            {
                deleta++;
            }
        }
        alioquin
        {
            _vivos_colligere(thesaurus, via, claves, blobi, piscina);
        }
    }
    deleta += _genus_purgare(thesaurus, "actiones", claves, FALSUM,
        piscina);
    deleta += _genus_purgare(thesaurus, "blobi", blobi, verificare,
        piscina);
    *deleta_out = deleta;
    redde VERUM;
}
