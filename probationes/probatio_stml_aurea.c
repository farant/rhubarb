/* probatio_stml_aurea.c - PONS AUREUS accessoris stml_textus_internus
 *
 * TESTIMONIUM MORIS VETERIS (spec: project-specs/stml-trivia-spec.md
 * §7.1): ante chirurgiam triviae, quid internus HODIE reddat super
 * corpore vero capitur; post quamque tranchem chirurgiae aequalitas
 * stricta asseritur. Divergentia UNA nominata exspectatur (exceptio
 * §1.3: elementum cuius contentum totum spatium album lineam-ferens
 * erat - '<a>\n</a>' - vacuum fit). Cum ea tranche cadat, haec
 * probatio RUBET consulto: index divergentiarum IPSE est enumeratio
 * quam spec poscit. Ordo tunc: differentias legere (probatio
 * '.novum' scribit - 'diff' eas enumerat), contra spec §1.3/§2/§6
 * iudicare, in worklog notare, aurea regenerare per
 * tools/stml_aurea_generare.sh. NUMQUAM regenerare sine iudicio -
 * aurum est testimonium, non speculum.
 *
 * FORMA AUREI: pro plagulis corporis lineae per-nodum D/E solum
 * (mensura + fnv - octeti pleni O(textus x profunditas) inflarent;
 * mutatio textus quaelibet lineam parentis perturbat, ergo tegumen
 * integrum). IDENTITAS = '@positus_initium', non index praeordinis -
 * extensiones trans chirurgiam §1.3 stabiles manent (trivia extra
 * extensiones utrimque), index laberetur et diff strepitu mergeretur.
 * Valores spatii albi solius <= LXIV octetorum in 'A:' hexadecimali
 * (recensio quid elisum sit VIDET). Litterae authoratae (casus spec
 * §8 qui hodie parsant) omnes nodos D/E/T cum octetis PLENIS ferunt
 * ('V' bloci) - ibi legibilitas vincit.
 *
 * ORACULUM: memcmp buffri regenerati contra plagulam commissam -
 * via una codicis ambobus modis, nihil parsatur praeter indicem
 * viarum ('F' lineae). Fontes mutati (mensura/fnv fontis in linea F)
 * eodem memcmp cadunt - deriva fontis post capturam = RUBRUM
 * nominatum, non silentium.
 *
 * PRIVATIO NOMINATA: monstra fontium > 1MB (librarium bis,
 * gutenberg_index) EXCLUSA - pretium suitae quaque cursione;
 * exclusio in tools/stml_aurea_generare.sh vivit et in capite aurei
 * nominatur. Fidelitas eorum a probationibus circuitus non hic
 * tegitur.
 *
 * MODI: sine argumentis = iudicare (aurum abesse RUBET - aurum
 * absens ≡ porta mortua); '-generare via...' = aurum scribere
 * (numquam a suita vocatum).
 */
#include "latina.h"
#include "stml.h"
#include "credo.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

#define AUREUM_VIA "probationes/fixa/stml_aurea/internus.aurum"
#define NOVUM_VIA  "probationes/fixa/stml_aurea/internus.novum"

/* Casus authorati: quisque hodie parsat; forma arboris et valores
 * internus HODIERNI capiuntur, quicumque sint (etiam ubi captio
 * spatium album numerat - id ipsum vitium est cuius mutationem
 * recensio nominare debet). */
nomen structura {
    constans character* titulus;
    constans character* fons;
} LitteraAurea;

interior constans LitteraAurea LITTERAE_AUREAE[] = {
    { "mixtus_inline",
      "<p>salve <b>munde</b> iterum</p>" },
    { "sep_spatia",
      "<sep>   </sep>" },
    { "vacuum_linea",
      "<a>\n</a>" },
    { "marginibus",
      "<a>\n  prima\n  secunda\n</a>" },
    { "spatia_ubique",
      "<radix>\n  <a/>\n  <b>x</b>\n</radix>\n" },
    { "cauda_documenti",
      "<radix/>\n\n" },
    { "captio_ante_linea",
      "<radix><a (>\n<b/></radix>" },
    { "captio_ante_recta",
      "<radix><a (><b/><c/></radix>" },
    { "captio_multiplex",
      "<radix><a ((>\n<b/>\n<c/>\n</radix>" },
    { "captio_retro",
      "<radix><b/>\n<) a></radix>" },
    { "farcimen",
      "<radix><x/>\n<= a =>\n<y/></radix>" },
    { "captio_commentum",
      "<radix><a (><!-- nota --><b/></radix>" },
    { "cruda_linea",
      "<radix><imp! (>crudus textus\n<post/></radix>" },
    { "cruda_linea_cauda",
      "<radix><imp! (>crudus\n</radix>" },
    { "crudus_plenus",
      "<code!>a < b && c > d</code>" },
    { "entia",
      "<a>salve &lt;mundus&gt; &amp; amici</a>" },
    { "commentum_inter",
      "<radix>ante<!-- c -->post</radix>" },
    { "fragmentum",
      "<#pars attr=\"1\"><a/>textus</#>" },
    { "fragmentum_captans",
      "<radix><#f (><a/><reliquum/></radix>" },
    { "transclusio",
      "<radix>ante <<.via>> post</radix>" },
    { "augmentum",
      "<radix><% &clavis_probata;>textus additus</%></radix>" },
    { "titulus_punctatus",
      "<.species>x</.species>" },
    { "clausura_anonyma",
      "<radix><a>x</></radix>" },
    { "processio",
      "<?xml version=\"1.0\"?>\n<radix><a/></radix>\n" },
    { "attributa_multilinea",
      "<a\n  b=\"1\"\n  c=\"2\">x</a>" }
};

/* FNV-1a XXXII-bitorum */
interior i32
_fnv (
    constans i8* datum,
            i32  mensura)
{
    i32 h;
    i32 i;

    h = 2166136261U;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        h = h ^ (i32)(insignatus character)datum[i];
        h = h * 16777619U;
    }
    redde h;
}

interior b32
_album_solum (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character c;

        c = (character)s.datum[i];
        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Plagulam totam in piscinam legere */
interior b32
_fasciculum_legere (
    constans character* via,
               Piscina* piscina,
                chorda* exitus)
{
      FILE* f;
    longus  mensura;
        i8* buffer;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }

    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }
    mensura = ftell(f);
    si (mensura < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    si (fseek(f, 0L, SEEK_SET) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    si (mensura == 0L)
    {
        fclose(f);
        redde VERUM;  /* plagula vacua - chorda vacua */
    }

    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (buffer == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    si (fread(buffer, I, (size_t)mensura, f) != (size_t)mensura)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);

    exitus->datum    = buffer;
    exitus->mensura  = (i32)mensura;
    redde VERUM;
}

interior character
_genus_littera (
    StmlNodusGenus genus)
{
    commutatio (genus)
    {
        casus STML_NODUS_DOCUMENTUM:  redde 'D';
        casus STML_NODUS_ELEMENTUM:   redde 'E';
        casus STML_NODUS_TEXTUS:      redde 'T';
        ordinarius:                   redde '\0';
    }
}

/* Ambulatio praeordinis: lineam 'N' per nodum emittere.
 *
 * IDENTITAS NODI = '@positus_initium' (extensio fontis), NON index
 * praeordinis: chirurgia §1.3 nodos spatii albi delebit, et index
 * quisque post nodum deletum LABERETUR - divergentia una milia
 * linearum insequentium rubras faceret et recensio ('diff') strepitu
 * mergeretur. Extensiones contra trans mutationem STABILES manent
 * (trivia EXTRA extensiones utrimque - spec §3), ergo linea nodi
 * superstitis eadem manet et diff deletiones/mutationes SOLAS
 * ostendit.
 *
 * FONTES: nodi D/E soli notantur (plene=FALSUM) - mutatio textus
 * quaelibet lineam parentis sui perturbat, ergo tegumen integrum
 * manet, et deletiones T milium sub §1.3 (consulto factae) diff non
 * inundant; summa 'nodi:' in linea arboris eas per fontem numerat.
 * LITTERAE: omnes D/E/T cum blocis 'V' (octeti pleni) - ibi fabula
 * subtilis legibiliter narratur. Spatium album solum <= LXIV
 * octetorum in 'A:' hexadecimali (quid elisum sit VIDETUR). */
interior vacuum
_nodos_notare (
            StmlNodus* nodus,
                  i32* nodi,
                  i32* notati,
    ChordaAedificator* aed,
              Piscina* piscina,
                  b32  plene)
{
    character linea_buf[CCLVI];
    character littera;
       chorda internus;
          i32 i;
          i32 num;

    si (nodus == NIHIL)
    {
        redde;
    }

    *nodi = *nodi + I;

    littera = _genus_littera(nodus->genus);
    si (littera != '\0' && (plene || littera != 'T'))
    {
        internus = stml_textus_internus(nodus, piscina);

        sprintf(linea_buf,
            "N @%lu %c %lu %lu %08lx",
            (insignatus longus)nodus->positus_initium,
            littera,
            (insignatus longus)nodus->linea,
            (insignatus longus)internus.mensura,
            (insignatus longus)_fnv(internus.datum, internus.mensura));
        chorda_aedificator_appendere_literis(aed, linea_buf);

        si (   internus.mensura > ZEPHYRUM
            && internus.mensura <= LXIV
            && _album_solum(internus))
        {
            chorda_aedificator_appendere_literis(aed, " A:");
            per (i = ZEPHYRUM; i < internus.mensura; i++)
            {
                sprintf(linea_buf, "%02x",
                    (insignatus integer)
                    (insignatus character)internus.datum[i]);
                chorda_aedificator_appendere_literis(aed, linea_buf);
            }
        }
        chorda_aedificator_appendere_character(aed, '\n');

        si (plene)
        {
            sprintf(linea_buf, "V %lu\n",
                (insignatus longus)internus.mensura);
            chorda_aedificator_appendere_literis(aed, linea_buf);
            chorda_aedificator_appendere_chorda(aed, internus);
            chorda_aedificator_appendere_character(aed, '\n');
        }

        *notati = *notati + I;
    }

    si (nodus->liberi != NIHIL)
    {
        num = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus** cella;

            cella = (StmlNodus**)xar_obtinere(nodus->liberi, i);
            si (cella != NIHIL)
            {
                _nodos_notare(*cella, nodi, notati, aed, piscina,
                              plene);
            }
        }
    }
}

/* Fontem parsare et sectionem arboris emittere (linea 'arbor' +
 * lineae 'N'). Piscina vocantis - vocans eam post sectionem
 * destruit ubi fontes magni sunt. */
interior vacuum
_arborem_notare (
               chorda  fons,
    ChordaAedificator* aed,
              Piscina* piscina,
                  b32  plene)
{
    InternamentumChorda* intern;
           StmlResultus  res;
              character  linea_buf[CCLVI];
                    i32  nodi;
                    i32  notati;

    intern = internamentum_creare(piscina);

    res.radix      = NIHIL;
    res.status     = STML_ERROR_VACUUM_INPUT;
    res.successus  = FALSUM;
    si (intern != NIHIL && fons.mensura > ZEPHYRUM)
    {
        res = stml_legere(fons, piscina, intern);
    }

    nodi    = ZEPHYRUM;
    notati  = ZEPHYRUM;
    si (res.radix != NIHIL)
    {
        /* linea 'arbor' summas fert et ANTE lineas 'N' stare debet -
         * ambulatio in aedificatorem TEMPORARIUM scribitur, deinde
         * assuitur. */
        ChordaAedificator* corpus_aed;
                   chorda  corpus;

        corpus_aed = chorda_aedificator_creare(piscina, DXII);
        _nodos_notare(res.radix, &nodi, &notati, corpus_aed,
                      piscina, plene);
        corpus = chorda_aedificator_finire(corpus_aed);

        sprintf(linea_buf,
            "arbor successus:%lu status:%lu nodi:%lu notati:%lu\n",
            (insignatus longus)(res.successus ? I : ZEPHYRUM),
            (insignatus longus)res.status,
            (insignatus longus)nodi,
            (insignatus longus)notati);
        chorda_aedificator_appendere_literis(aed, linea_buf);
        chorda_aedificator_appendere_chorda(aed, corpus);
    }
    alioquin
    {
        sprintf(linea_buf,
            "arbor successus:0 status:%lu nodi:0 notati:0\n",
            (insignatus longus)res.status);
        chorda_aedificator_appendere_literis(aed, linea_buf);
    }
}

/* Sectionem unius fontis emittere: linea 'F' (identitas + deriva)
 * tum arbor. Piscina propria - post sectionem destruitur. */
interior b32
_fontem_notare (
    constans character* via,
     ChordaAedificator* aed)
{
      Piscina* p;
    character  linea_buf[DXII];
       chorda  fons;
          b32  bene;

    p = piscina_generare_dynamicum("aurea_fons", 1048576);
    si (p == NIHIL)
    {
        redde FALSUM;
    }

    bene = _fasciculum_legere(via, p, &fons);
    si (!bene)
    {
        imprimere("FRACTA lectio fontis: %s\n", via);
        piscina_destruere(p);
        redde FALSUM;
    }

    sprintf(linea_buf, "F %s mensura:%lu fnv:%08lx\n",
        via,
        (insignatus longus)fons.mensura,
        (insignatus longus)_fnv(fons.datum, fons.mensura));
    chorda_aedificator_appendere_literis(aed, linea_buf);

    _arborem_notare(fons, aed, p, FALSUM);

    piscina_destruere(p);
    redde VERUM;
}

/* Buffrum aureum totum generare - via UNA codicis ambobus modis */
interior b32
_aureum_generare (
                Piscina*  piscina,
     constans character** viae,
                    i32   numerus_viarum,
                 chorda*  exitus)
{
    ChordaAedificator* aed;
            character  linea_buf[CCLVI];
                  i32  numerus_litterarum;
                  i32  i;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;

    aed = chorda_aedificator_creare(piscina, 262144);
    si (aed == NIHIL)
    {
        redde FALSUM;
    }

    numerus_litterarum = (i32)(magnitudo(LITTERAE_AUREAE)
                               / magnitudo(LITTERAE_AUREAE[ZEPHYRUM]));

    chorda_aedificator_appendere_literis(aed,
        "AUREA STML INTERNUS I\n"
        "MONSTRA FONTIUM >1MB EXCLUSA"
        " (vide tools/stml_aurea_generare.sh)\n");

    sprintf(linea_buf, "LITTERAE %lu\n",
        (insignatus longus)numerus_litterarum);
    chorda_aedificator_appendere_literis(aed, linea_buf);

    per (i = ZEPHYRUM; i < numerus_litterarum; i++)
    {
         Piscina* p;
          chorda  fons;

        sprintf(linea_buf, "@ %s\n", LITTERAE_AUREAE[i].titulus);
        chorda_aedificator_appendere_literis(aed, linea_buf);

        p = piscina_generare_dynamicum("aurea_littera", 16384);
        si (p == NIHIL)
        {
            redde FALSUM;
        }
        fons = chorda_ex_literis(LITTERAE_AUREAE[i].fons, p);
        _arborem_notare(fons, aed, p, VERUM);
        piscina_destruere(p);
    }

    sprintf(linea_buf, "FONTES %lu\n",
        (insignatus longus)numerus_viarum);
    chorda_aedificator_appendere_literis(aed, linea_buf);

    per (i = ZEPHYRUM; i < numerus_viarum; i++)
    {
        si (!_fontem_notare(viae[i], aed))
        {
            redde FALSUM;
        }
    }

    chorda_aedificator_appendere_literis(aed, "FINIS\n");

    *exitus = chorda_aedificator_finire(aed);
    redde VERUM;
}

/* Vias fontium ex aureo commisso extrahere (lineae 'F <via> ...') */
interior i32
_vias_ex_aureo (
                 chorda    aureum,
                Piscina*   piscina,
     constans character*** viae_exitus)
{
     constans character** viae;
                    i32   numerus;
                    i32   i;
                    i32   transitus;

    numerus  = ZEPHYRUM;
    viae     = NIHIL;

    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        i32 n;

        n = ZEPHYRUM;
        per (i = ZEPHYRUM; i < aureum.mensura; i++)
        {
            b32 initium_lineae;

            initium_lineae = (b32)(i == ZEPHYRUM
                || (character)aureum.datum[i - I] == '\n');
            si (   initium_lineae
                && i + I < aureum.mensura
                && (character)aureum.datum[i]     == 'F'
                && (character)aureum.datum[i + I] == ' ')
            {
                si (transitus == I)
                {
                          i32  initium;
                          i32  finis;
                    character* copia;

                    initium  = i + II;
                    finis    = initium;
                    dum (   finis < aureum.mensura
                         && (character)aureum.datum[finis] != ' '
                         && (character)aureum.datum[finis] != '\n')
                    {
                        finis++;
                    }
                    copia = (character*)piscina_allocare(piscina,
                        (memoriae_index)(finis - initium + I));
                    si (copia != NIHIL)
                    {
                        memcpy(copia, aureum.datum + initium,
                               (size_t)(finis - initium));
                        copia[finis - initium]  = '\0';
                        viae[n]                 = copia;
                    }
                }
                n++;
            }
        }

        si (transitus == ZEPHYRUM)
        {
            numerus = n;
            si (numerus == ZEPHYRUM)
            {
                frange;
            }
            viae = (constans character**)piscina_allocare(piscina,
                (memoriae_index)numerus
                    * (memoriae_index)magnitudo(character*));
            si (viae == NIHIL)
            {
                redde ZEPHYRUM;
            }
        }
    }

    *viae_exitus = viae;
    redde numerus;
}

/* Divergentiam primam nominare: numerus lineae + contextus ambarum
 * partium; buffrum novum in NOVUM_VIA scribere ut 'diff' recensioni
 * serviat. */
interior vacuum
_divergentiam_imprimere (
    chorda vetus,
    chorda novum)
{
    i32 minima;
    i32 primus;
    i32 linea_num;
    i32 initium_lineae;
    i32 i;
    i32 finis_v;
    i32 finis_n;

    minima = (vetus.mensura < novum.mensura)
        ? vetus.mensura : novum.mensura;
    primus = minima;
    per (i = ZEPHYRUM; i < minima; i++)
    {
        si (vetus.datum[i] != novum.datum[i])
        {
            primus = i;
            frange;
        }
    }

    linea_num       = I;
    initium_lineae  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < primus; i++)
    {
        si ((character)vetus.datum[i] == '\n')
        {
            linea_num++;
            initium_lineae = i + I;
        }
    }

    finis_v = initium_lineae;
    dum (finis_v < vetus.mensura && finis_v < initium_lineae + CC)
    {
        si (   (character)vetus.datum[finis_v] == '\n'
            && finis_v > primus)
        {
            frange;
        }
        finis_v++;
    }
    finis_n = initium_lineae;
    dum (finis_n < novum.mensura && finis_n < initium_lineae + CC)
    {
        si (   (character)novum.datum[finis_n] == '\n'
            && finis_n > primus)
        {
            frange;
        }
        finis_n++;
    }

    imprimere("DIVERGENTIA ad octetum %lu (linea %lu aurei):\n",
        (insignatus longus)primus, (insignatus longus)linea_num);
    imprimere("  AURUM:  %.*s\n",
        (integer)(finis_v - initium_lineae),
        (constans character*)(vetus.datum + initium_lineae));
    imprimere("  NOVUM:  %.*s\n",
        (integer)(finis_n - initium_lineae),
        (constans character*)(novum.datum + initium_lineae));
    imprimere("  (mensurae: aurum %lu, novum %lu)\n",
        (insignatus longus)vetus.mensura,
        (insignatus longus)novum.mensura);

    {
        FILE* f;

        f = fopen(NOVUM_VIA, "wb");
        si (f != NIHIL)
        {
            fwrite(novum.datum, I, (size_t)novum.mensura, f);
            fclose(f);
            imprimere("  buffrum novum scriptum: %s\n", NOVUM_VIA);
            imprimere("  recensio: diff %s %s\n", AUREUM_VIA,
                NOVUM_VIA);
        }
    }
}

s32
principale (
          s32   argc,
    character** argv)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_stml_aurea",
                                         4194304);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }


    /* ==================================================
     * MODUS GENERANDI: -generare via1 via2 ...
     * ================================================== */

    si (argc >= II && strcmp(argv[I], "-generare") == ZEPHYRUM)
    {
                 chorda aureum;
                    b32 bene;
                   FILE* f;
    constans character** viae;
                    i32  numerus;
                    i32  i;

        si (argc < III)
        {
            imprimere("usus: probatio_stml_aurea -generare "
                      "<viae fontium...>\n");
            redde I;
        }

        /* argv in tabulam constantem copiare (-Wcast-qual:
         * character** in constans character** directe verti nequit) */
        numerus = (i32)(argc - II);
        viae = (constans character**)piscina_allocare(piscina,
            (memoriae_index)numerus
                * (memoriae_index)magnitudo(character*));
        si (viae == NIHIL)
        {
            imprimere("FRACTA: allocatio viarum\n");
            redde I;
        }
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            viae[i] = argv[(i32)II + i];
        }

        bene = _aureum_generare(piscina, viae, numerus, &aureum);
        si (!bene)
        {
            imprimere("FRACTA: generatio aurei\n");
            redde I;
        }

        f = fopen(AUREUM_VIA, "wb");
        si (f == NIHIL)
        {
            imprimere("FRACTA: scriptio %s (num directorium "
                      "exsistit?)\n", AUREUM_VIA);
            redde I;
        }
        fwrite(aureum.datum, I, (size_t)aureum.mensura, f);
        fclose(f);

        imprimere("aurum scriptum: %s (%lu octeti, %lu fontes)\n",
            AUREUM_VIA,
            (insignatus longus)aureum.mensura,
            (insignatus longus)(argc - II));
        redde ZEPHYRUM;
    }


    /* ==================================================
     * MODUS IUDICANDI (suita): aurum legere, regenerare,
     * memcmp. Aurum absens = RUBRUM (porta mortua aliter).
     * ================================================== */

    credo_aperire(piscina);

    imprimere("\n--- pons aureus: stml_textus_internus ---\n");
    {
                     chorda   aureum;
                     chorda   novum;
         constans character** viae;
                        i32   numerus_viarum;
                        b32   bene;

        bene = _fasciculum_legere(AUREUM_VIA, piscina, &aureum);
        si (!bene || aureum.mensura == ZEPHYRUM)
        {
            imprimere("AURUM ABEST: %s\n", AUREUM_VIA);
            imprimere("genera: ./compile_tests.sh stml_aurea, tum "
                      "tools/stml_aurea_generare.sh\n");
        }
        CREDO_VERUM (bene && aureum.mensura > ZEPHYRUM);

        si (bene && aureum.mensura > ZEPHYRUM)
        {
            viae            = NIHIL;
            numerus_viarum  = _vias_ex_aureo(aureum, piscina, &viae);

            /* mensura teguminis a se ipsa: fontes > 0 */
            CREDO_VERUM (numerus_viarum > ZEPHYRUM);
            imprimere("fontes in auro: %lu\n",
                (insignatus longus)numerus_viarum);

            si (numerus_viarum > ZEPHYRUM)
            {
                bene = _aureum_generare(piscina, viae,
                                        numerus_viarum, &novum);
                CREDO_VERUM (bene);

                si (bene)
                {
                    b32 par;

                    par = (b32)(aureum.mensura == novum.mensura
                        && memcmp(aureum.datum, novum.datum,
                                  (size_t)aureum.mensura)
                            == ZEPHYRUM);
                    si (!par)
                    {
                        _divergentiam_imprimere(aureum, novum);
                    }
                    CREDO_VERUM (par);
                }
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
