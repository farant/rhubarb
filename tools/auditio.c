/* auditio.c - praeparata sonitus, cantilenas et carmina audire (sonus
 * S1, S4, S6)
 *
 * Sonitum praeparatum reddit in plagulam WAV et per afplay sonat;
 * cantilenam (cantus.h) per exitum vivum (auditus) sonat -
 * instrumentum aurium, non probatio: probationes numeros iudicant,
 * hoc Franus et Claudius auribus iudicant.
 *
 *   auditio                        praeparata cum longitudinibus
 *   auditio radius [semen]         /tmp/auditio_radius.wav, deinde afplay
 *   auditio radius 7 -via x.wav    plagulam solam scribit (non sonat)
 *   auditio -omnes                 omnia ordine sonat
 *
 *   auditio -cantus "t180 o6 l16 e g# b >e8"     vivum, colore 'tonus'
 *   auditio -cantus "..." -praeparatum radius   notae sine '@' hoc colore
 *   auditio -cantus "..." -intactus             color intactus (tenere
 *                                               FALSUM: campana, ictus)
 *   auditio -cantus "..." -via x.wav            per mixturam offline in
 *                                               WAV stereo (non sonat)
 *   auditio -cantus "..." 7                     semen
 *
 *   auditio -cantuale                  cantilenae libri ordinarii
 *   auditio -genus finis [semen]       cantilena generis sorte (semen
 *                                      absens = horologium)
 *   auditio -cantilena arpeggium       cantilena per titulum
 *
 *   auditio -carmina                   carmina libri ordinarii:
 *                                      titulus, clavis, metrum, tempus,
 *                                      mensurae; partes (instrumentum,
 *                                      notae, ambitus)
 *   auditio -carmen korobeiniki        vivum, semel; mensura currens
 *                                      monstratur
 *   auditio -carmen T -iterare         vivum, iterans (Ctrl-C finit)
 *   auditio -carmen T -pars bassus     pars sola (solus)
 *   auditio -carmen T -via x.wav       in WAV stereo, semel (non sonat)
 *   auditio -carmen T -partitura       partitura ut textus
 *       [-mensurae 9-12] [-pars P]     (partitura.h), sine sono; -pars
 *       [-harmonia]                    hic = sola pars; -harmonia:
 *                                      linea acordorum (harmonia.h)
 *   auditio -carmen T -censura         regulae musicae (censura.h):
 *       [-regulae +dissonantia,-ambitus]  ordinariae mutatae, aut
 *       [-regulae parallelae,lineae]      hae solae; exitus 3 si
 *                                      inventa (0 si nulla)
 *   auditio -liber x.stml -carmina     liber externus (filum),
 *   auditio -liber x.stml -carmen T    iudicatus ut ordinarius; vitium
 *                                      cum carmine, parte, frusto,
 *                                      offset
 *
 *   auditio -carmen T -auscultare      numeri soni (auscultatio.h):
 *                                      apex, tonsura, rms totius,
 *                                      partis solae, mensurae; exitus
 *                                      3 si tonsura aut mensura TACITA
 *
 * Carmen per concentum et cantorem - via eadem ac vicus: pulsus X ms
 * (concentus_agere, cantor_agere). Vivum: exitus = concentus_auditus;
 * WAV et -auscultare: auscultatio_reddere (exitus scripturae, quadrum
 * 0 = pulsus 0) - eadem logica prospectus sine horologio. Finis:
 * cantor non sonat et nulla vox vivit (caudae inclusae).
 *
 * Cantus: voces = notae (potentia II, >= XVI), quia cantus_iussa_parare
 * sedes OMNES statim capit - voces pauciores notas futuras surriperent.
 * Vivum: momentum = nunc + mora + margo; exit cum vox ultima finita
 * (cauda remissionis inclusa). Eadem iussa offline et vivum.
 *
 * Usus: ./bin/aedilis tools/auditio.c && bash
 *   build/aedilis/auditio/struere.sh &&
 *   ./build/aedilis/auditio/auditio radius
 *
 * Exitus: 0 bene, 1 praeparatum ignotum, cantus vitiosus, sine sono aut
 * scriptio fracta, 2 usus, 3 censura invenit.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "processus.h"
#include "sonus.h"
#include "sonitus.h"
#include "sonus_wav.h"
#include "mixtura.h"
#include "auditus.h"
#include "cantus.h"
#include "cantuale.h"
#include "carmen.h"
#include "cantor.h"
#include "concentus.h"
#include "concentus_auditus.h"
#include "partitura.h"
#include "censura.h"
#include "auscultatio.h"
#include "harmonia.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* afplay plus longitudine soni numquam eget; terminus durus contra
 * pendentiam */
#define AUDITIO_MORA_MS (XXX * M)
/* cantus vivus: L ms ultra moram exitus, ne nota prima tarda sit */
#define AUDITIO_MARGO   (II * M + CD)
#define AUDITIO_PRAEFIXUM "auditio: cantus: "

interior vacuum
_praeparata_monstrare (
    vacuum)
{
    i32 k;

    per (k = ZEPHYRUM; k < sonitus_praeparatorum_numerus(); k++)
    {
        constans character* titulus   = sonitus_praeparati_titulus(k);
        constans SonitusParametra* p  = sonitus_praeparatum(titulus);

        imprimere("  %-10s %6.3f s   varietas %.2f\n", titulus,
                  (duplex)sonitus_longitudo(p, I)
                      / (duplex)SONUS_FREQUENTIA,
                  (duplex)p->varietas);
    }
}

/* reddit, scribit, et (si sonare) per afplay sonat; FALSUM si deficit */
interior b32
_audire (
    constans character* titulus,
                   i64  semen,
    constans character* via,
                   b32  sonare,
               Piscina* piscina)
{
     constans SonitusParametra* p = sonitus_praeparatum(titulus);
                           i32  n;
                           f32* exempla;
                           i32  scripta;

    si (p == NIHIL)
    {
        fprintf(stderr, "auditio: praeparatum ignotum '%s'\n", titulus);
        redde FALSUM;
    }
    si (via == NIHIL)
    {
        redde FALSUM;
    }
    n = sonitus_longitudo(p, semen);
    exempla = (f32*)piscina_allocare(piscina,
                                     (memoriae_index)n * magnitudo(f32));
    si (exempla == NIHIL)
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }
    scripta = sonitus_reddere(p, semen, exempla, n);
    si (!sonus_wav_scribere(via, exempla, scripta, I, SONUS_FREQUENTIA,
                            piscina))
    {
        fprintf(stderr, "auditio: scribi non potuit '%s'\n", via);
        redde FALSUM;
    }
    imprimere("%s (semen %lu): %.3f s -> %s\n", titulus,
              (longus insignatus)semen,
              (duplex)scripta / (duplex)SONUS_FREQUENTIA, via);
    si (sonare)
    {
        constans character* argumenta[III];
         ProcessusResultus  r;

        argumenta[ZEPHYRUM] = "afplay";
        argumenta[I] = via;
        argumenta[II] = NIHIL;
        r = processus_exsequi(argumenta, AUDITIO_MORA_MS, piscina);
        si (!r.successus || r.codex_exitus != ZEPHYRUM)
        {
            fprintf(stderr, "auditio: afplay fractum (exitus %u)\n",
                    r.codex_exitus);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* /tmp/auditio_<titulus>.wav in piscina */
interior constans character*
_via_temporaria (
    constans character* titulus,
               Piscina* piscina)
{
    memoriae_index  n    = strlen(titulus) + XXXII;
         character* via  = (character*)piscina_allocare(piscina, n);

    si (via == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(via, "/tmp/auditio_%s.wav", titulus);
    redde via;
}

interior vacuum
_dormire_ms (
    i32 ms)
{
    struct timespec t;

    t.tv_sec   = (time_t)(ms / M);
    t.tv_nsec  = (long)(ms % M) * 1000000L;
    (vacuum)nanosleep(&t, NIHIL);
}

/* textus, deinde '^' sub octeto vitii */
interior vacuum
_vitium_monstrare (
       constans character* textus,
    constans CantusVitium* vitium)
{
    i32 k;

    fprintf(stderr, "%s%s\n", AUDITIO_PRAEFIXUM, textus);
    per (k = ZEPHYRUM;
         k < (i32)strlen(AUDITIO_PRAEFIXUM) + vitium->offset; k++)
    {
        fputc(' ', stderr);
    }
    fprintf(stderr, "^ %s\n", vitium->causa);
}


/* ==================================================
 * Carmen: cantor super concentum (S6d)
 * ================================================== */

/* pulsus hospitis, ms - via eadem ac vicus */
#define AUDITIO_PULSUS_MS X

/* Ctrl-C in vivo: cantor sistitur, voces evanescunt, exitus
 * clauditur */
hic_manens volatilis sig_atomic_t interruptum = ZEPHYRUM;

interior vacuum
_interrumpere (
    integer signum)
{
    (vacuum)signum;
    interruptum = I;
}

/* nota MIDI -> "E4" (littera, signum, octava) in alveo dato */
interior constans character*
_nota_titulus (
          i32  nota,
    character* alveus)
{
    hic_manens constans character* LITTERAE[XII] = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };

    sprintf(alveus, "%s%d", LITTERAE[nota % XII], (integer)nota / XII
        - I);
    redde alveus;
}

/* clavis -> "a minor"; quintae -7..7 */
interior constans character*
_clavis_titulus (
    constans CarmenClavis* clavis)
{
    hic_manens constans character* MAIORES[XV] = {
        "cb", "gb", "db", "ab", "eb", "bb", "f", "c",
        "g", "d", "a", "e", "b", "f#", "c#"
    };
    hic_manens constans character* MINORES[XV] = {
        "ab", "eb", "bb", "f", "c", "g", "d", "a", "e", "b", "f#", "c#",
        "g#", "d#", "a#"
    };

    redde clavis->minor ? MINORES[clavis->quintae + VII]
                        : MAIORES[clavis->quintae + VII];
}

/* mensurae carminis (ultima quae ante finem incipit) */
interior i32
_mensurae (
    constans Carmen* k)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(k, m + I) < k->pulsus)
    {
        m++;
    }
    redde m;
}

/* mensura (1 = prima) in qua pulsus jacet */
interior i32
_mensura_pulsus (
    constans Carmen* k,
                s64  pulsus)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(k, m + I) <= pulsus)
    {
        m++;
    }
    redde m;
}

/* summarium: carmen una linea, pars quaeque una linea - lectori et
 * agenti (sine sono) */
interior vacuum
_carmen_monstrare (
    constans Carmen* k)
{
     constans character* clavis       = "c";
                    b32  minor        = FALSUM;
                    i32  numerator    = IV;
                    i32  denominator  = IV;
                    i32  i;
                    i32  f;
                    i32  n;

    si (k->numerus_clavium > ZEPHYRUM)
    {
        clavis  = _clavis_titulus(&k->claves[ZEPHYRUM]);
        minor   = k->claves[ZEPHYRUM].minor;
    }
    si (k->numerus_metrorum > ZEPHYRUM)
    {
        numerator    = k->metra[ZEPHYRUM].numerator;
        denominator  = k->metra[ZEPHYRUM].denominator;
    }
    imprimere("%.*s: %s %s, %u/%u, %u quartae/min, %u mensurae, "
              "%.3f s\n", (integer)k->titulus.mensura,
              (constans character*)k->titulus.datum, clavis,
              minor ? "minor" : "maior", numerator, denominator,
              k->tempora[ZEPHYRUM].tempus, _mensurae(k),
              (duplex)cantus_quadrum_pulsus(k->tempora,
                  k->numerus_temporum, k->pulsus)
                  / (duplex)SONUS_FREQUENTIA);
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p       = &k->partes[i];
                        i32  infima  = CXXVII;
                        i32  summa   = ZEPHYRUM;
                  character  a[VIII];
                  character  b[VIII];

        n = ZEPHYRUM;
        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            i32 j;

            per (j = ZEPHYRUM; j < p->frusta[f].numerus; j++)
            {
                i32 nota = p->frusta[f].notae[j].nota;

                infima  = nota < infima ? nota : infima;
                summa   = nota > summa ? nota : summa;
                n++;
            }
        }
        imprimere("  %-10.*s %-12.*s lucrum %.2f  notae %3u  "
                  "%s..%s%s%s\n",
            (integer)p->titulus.mensura,
            (constans character*)p->titulus.datum,
            (integer)p->instrumentum.mensura,
            (constans character*)p->instrumentum.datum,
            (duplex)p->lucrum,
            n, n ? _nota_titulus(infima, a) : "-",
            n ? _nota_titulus(summa, b) : "-",
            p->mutus ? "  mutus" : "",
            p->solus ? "  solus" : "");
    }
}

/* liber ordinarius; indicare = carmina monstrat; aliter carmen per
 * titulum (et pars sola si data). NIHIL: vitium monstratum aut index */
interior Carmen*
_ex_carminibus (
     constans character* via_libri,   /* NIHIL = liber ordinarius */
                    b32  indicare,
     constans character* titulus,
     constans character* pars,
                    b32  solum,       /* pars solus fit (sonare) */
                Piscina* piscina,
                    b32* bene)
{
           Carmina  liber;
    CarminisVitium  vitium;
            Carmen* k = NIHIL;
            chorda  textus;
               b32  lectum;
               i32  i;

    *bene = FALSUM;
    si (via_libri != NIHIL)
    {
        textus = filum_legere_totum(via_libri, piscina);
        si (textus.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "auditio: liber '%s' legi non potuit\n",
                    via_libri);
            redde NIHIL;
        }
        lectum = carmina_legere(textus, piscina, &liber, &vitium);
    }
    alioquin
    {
        lectum = carmina_ordinaria(piscina, &liber, &vitium);
    }
    si (!lectum)
    {
        fprintf(stderr,
            "auditio: %s: carmen '%.*s' pars '%.*s' frustum %d "
                "offset %u: %s\n",
                via_libri != NIHIL ? via_libri : "carmina.stml",
                (integer)vitium.carmen.mensura,
                (constans character*)vitium.carmen.datum,
                (integer)vitium.pars.mensura,
                (constans character*)vitium.pars.datum,
                (integer)vitium.frustum, vitium.offset, vitium.causa);
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < liber.numerus; i++)
    {
        si (indicare)
        {
            _carmen_monstrare(&liber.carmina[i]);
        }
        alioquin si (chorda_aequalis_literis(liber.carmina[i].titulus,
                                             titulus))
        {
            k = &liber.carmina[i];
        }
    }
    si (indicare)
    {
        *bene = VERUM;
        redde NIHIL;
    }
    si (k == NIHIL)
    {
        fprintf(stderr, "auditio: nullum carmen '%s'\n", titulus);
        redde NIHIL;
    }
    si (pars != NIHIL)
    {
        b32 inventa = FALSUM;

        per (i = ZEPHYRUM; i < k->numerus_partium; i++)
        {
            si (chorda_aequalis_literis(k->partes[i].titulus, pars))
            {
                k->partes[i].solus  = k->partes[i].solus || solum;
                inventa             = VERUM;
            }
        }
        si (!inventa)
        {
            fprintf(stderr, "auditio: nulla pars '%s'\n", pars);
            redde NIHIL;
        }
    }
    *bene = VERUM;
    redde k;
}

/* instrumenta partium resolvuntur (resolutor ordinarius cantoris):
 * ignotum = vitium hic, non silentium in sonando */
interior b32
_instrumenta_probare (
    constans Carmen* k)
{
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        si (cantor_tibia_ordinaria(NIHIL, k->partes[i].instrumentum)
            == NIHIL)
        {
            fprintf(stderr,
                "auditio: pars '%.*s': instrumentum ignotum "
                    "'%.*s'\n", (integer)k->partes[i].titulus.mensura,
                    (constans character*)k->partes[i].titulus.datum,
                    (integer)k->partes[i].instrumentum.mensura,
                    (constans character*)
                        k->partes[i].instrumentum.datum);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* carmen vivum per concentum (exitus auditus) et cantorem. Pulsus X ms:
 * concentus_agere, deinde cantor_agere; finis = cantor tacet et nulla
 * vox vivit (caudae inclusae); Ctrl-C sistit (evanescentia) */
interior b32
_carmen_sonare (
    constans Carmen* k,
                b32  iterare,
            Piscina* piscina)
{
       MixturaForma  forma;
    ConcentusExitus* exitus;
          Concentus* concentus  = NIHIL;
             Cantor* cantor     = NIHIL;
                s64  nunc       = ZEPHYRUM;
                i32  mensura    = ZEPHYRUM;
                i32  transitus  = I;
                s64  prior      = ZEPHYRUM;
                i32  mensurae   = _mensurae(k);
                b32  sistitur   = FALSUM;

    forma.voces   = LXIV;
    forma.iussa   = CCLVI;
    forma.nuntii  = CXXVIII;
    exitus        = concentus_exitus_auditus(piscina, &forma);
    si (exitus != NIHIL)
    {
        concentus = concentus_creare(piscina, &forma, exitus);
    }
    si (concentus != NIHIL)
    {
        cantor = cantor_creare(piscina, concentus, NIHIL);
    }
    si (cantor == NIHIL)
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }
    cantor_regio(cantor, iterare, ZEPHYRUM, ZEPHYRUM);
    si (!cantor_incipere(cantor, k, ZEPHYRUM))
    {
        fprintf(stderr,
            "auditio: exitus soni non apertus (sine sono)\n");
        concentus_destruere(concentus);
        redde FALSUM;
    }
    (vacuum)signal(SIGINT, _interrumpere);
    per (;;)
    {
        b32 vivit;

        si (interruptum && !sistitur)
        {
            cantor_sistere(cantor);
            sistitur = VERUM;
        }
        vivit = concentus_agere(concentus, nunc);
        vivit = cantor_agere(cantor) || vivit;
        si (cantor_sonat(cantor))
        {
            s64 p = cantor_pulsus(cantor);
            i32 m = _mensura_pulsus(k, p);

            m = m > mensurae ? mensurae : m;   /* finis ipse */
            si (p < prior)
            {
                transitus++;
            }
            prior = p;
            si (m != mensura)
            {
                mensura = m;
                imprimere("transitus %u  mensura %u/%u\n", transitus, m,
                          mensurae);
                fflush(stdout);
            }
        }
        si (!vivit)
        {
            frange;
        }
        _dormire_ms(AUDITIO_PULSUS_MS);
        nunc += AUDITIO_PULSUS_MS;
    }
    concentus_destruere(concentus);
    redde VERUM;
}

/* carmen semel offline (auscultatio_reddere) in WAV stereo */
interior b32
_carmen_scribere (
       constans Carmen* k,
    constans character* via,
               Piscina* piscina)
{
    f32* exempla;
    i32  quadra;

    si (!auscultatio_reddere(k, NIHIL, piscina, &exempla, &quadra))
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }
    si (!sonus_wav_scribere(via, exempla, quadra, II, SONUS_FREQUENTIA,
                            piscina))
    {
        fprintf(stderr, "auditio: scribi non potuit '%s'\n", via);
        redde FALSUM;
    }
    imprimere("carmen -> %s (%.3f s)\n", via,
        (duplex)quadra / (duplex)SONUS_FREQUENTIA);
    redde VERUM;
}

/* amplitudo -> dBFS (20 log10; silentium -120) */
interior duplex
_gradus_soni (
    f32 x)
{
    redde x > 1e-6f ? 20.0 * log10((duplex)x) : -120.0;
}

/* numeri reddendi: totum, pars quaeque sola, mensura quaeque. Exitus 0,
 * 3 si tonsura aut mensura tacita quae sonare debet, 1 memoria */
interior integer
_auscultare (
    constans Carmen* k,
            Piscina* piscina)
{
    AuscultatioSummarium  s;
    AuscultatioSummarium  p;
                     f32* exempla;
                     i32  quadra;
                     i32* tacitae;
                     i32  numerus;
                     i32  i;
                     i32  t = ZEPHYRUM;

    si (   !auscultatio_reddere(k, NIHIL, piscina, &exempla, &quadra)
        || !auscultatio_metiri(k, exempla, quadra, piscina, &s))
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde I;
    }
    imprimere("auscultatio %.*s: %.3f s, apex %.3f (%.1f dBFS), "
              "tonsa %u, rms %.3f (%.1f dBFS)\n",
              (integer)k->titulus.mensura,
              (constans character*)k->titulus.datum,
              (duplex)quadra / (duplex)SONUS_FREQUENTIA, (duplex)s.apex,
              _gradus_soni(s.apex), s.tonsa, (duplex)s.rms,
              _gradus_soni(s.rms));
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        si (!auscultatio_partem(k, i, NIHIL, piscina, &p))
        {
            redde I;
        }
        imprimere("  pars %-10.*s rms %.3f (%.1f dBFS)  apex %.3f\n",
                  (integer)k->partes[i].titulus.mensura,
                  (constans character*)k->partes[i].titulus.datum,
                  (duplex)p.rms, _gradus_soni(p.rms), (duplex)p.apex);
    }
    tacitae = (i32*)piscina_allocare(piscina,
        (memoriae_index)s.numerus * magnitudo(i32));
    numerus = tacitae != NIHIL
        ? auscultatio_tacitae(k, &s, tacitae, s.numerus) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < s.numerus; i++)
    {
        b32 tacita = t < numerus && tacitae[t] == i + I;

        imprimere("  mensura %3u  rms %.3f (%.1f dBFS)%s\n", i + I,
                  (duplex)s.mensurae[i], _gradus_soni(s.mensurae[i]),
                  tacita ? "  TACITA (notae sonant)" : "");
        t += tacita ? I : ZEPHYRUM;
    }
    redde s.tonsa > ZEPHYRUM || numerus > ZEPHYRUM ? III : ZEPHYRUM;
}


/* "-regulae": "+a,-b" ordinarias mutat; "a,b" (sine signis) hae solae.
 * FALSUM: regula ignota (monstrata) */
interior b32
_regulas_legere (
    constans character* index,
          CensuraForma* forma,
               Piscina* piscina)
{
     constans character* p = index;
                    b32  solae = index[ZEPHYRUM] != '+'
                        && index[ZEPHYRUM] != '-';

    censura_ordinaria(forma);
    si (solae)
    {
        memset(forma->activae, ZEPHYRUM, magnitudo(forma->activae));
    }
    dum (*p != '\0')
    {
         constans character* q          = p;
                        b32  accendere  = VERUM;
              CensuraRegula  r;
                     chorda  t;

        si (*q == '+' || *q == '-')
        {
            accendere = *q == '+';
            q++;
        }
        p = q;
        dum (*p != '\0' && *p != ',')
        {
            p++;
        }
        t          = chorda_ex_literis(q, piscina);
        t.mensura  = (i32)(p - q);
        si (!censura_regulam_invenire(t, &r))
        {
            fprintf(stderr,
                "auditio: regula ignota '%.*s' (lineae, clavis, "
                    "ambitus, dissonantia, parallelae, saltus)\n",
                    (integer)t.mensura,
                    (constans character*)t.datum);
            redde FALSUM;
        }
        forma->activae[r] = accendere;
        si (*p == ',')
        {
            p++;
        }
    }
    redde VERUM;
}

/* censura in stdout: linea per inventum, deinde summa. Exitus 0 nulla,
 * 3 inventa, 1 memoria */
interior integer
_censuram_scribere (
          constans Carmen* k,
    constans CensuraForma* forma,
                  Piscina* piscina)
{
      CensuraInventum* inventa;
    ChordaAedificator* a = chorda_aedificator_creare(piscina, M);
                  i32  capacitas = CCLVI;
                  i32  n;
                  i32  i;
               chorda  t;

    inventa = (CensuraInventum*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(CensuraInventum));
    si (inventa == NIHIL || a == NIHIL)
    {
        redde I;
    }
    n = censura_iudicare(k, forma, inventa, capacitas);
    si (n > capacitas)
    {
        capacitas = n;
        inventa = (CensuraInventum*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(CensuraInventum));
        si (inventa == NIHIL)
        {
            redde I;
        }
        n = censura_iudicare(k, forma, inventa, capacitas);
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        (vacuum)censura_inventum_scribere(k, &inventa[i], a);
    }
    t = chorda_aedificator_finire(a);
    fwrite(t.datum, I, (size_t)t.mensura, stdout);
    imprimere("censura: %u inventa (regulae:", n);
    per (i = ZEPHYRUM; i < (i32)CENSURA_REGULAE; i++)
    {
        si (forma->activae[i])
        {
            imprimere(" %s", censura_titulus((CensuraRegula)i));
        }
    }
    imprimere(")\n");
    redde n > ZEPHYRUM ? III : ZEPHYRUM;
}

/* partitura (partitura.h) in stdout; prima/ultima 0 = carmen totum */
interior b32
_partituram_scribere (
       constans Carmen* k,
                   i32  prima,
                   i32  ultima,
    constans character* pars,
                   b32  harmonia,
               Piscina* piscina)
{
       PartituraForma  forma;
    ChordaAedificator* a = chorda_aedificator_creare(piscina, M);
               chorda  t;

    memset(&forma, ZEPHYRUM, magnitudo(forma));
    si (harmonia)
    {
        forma.additae_titulus  = "harmonia";
        forma.addita           = harmonia_lineam_scribere;
    }
    forma.prima         = prima;
    forma.ultima        = ultima;
    forma.pars.datum    = NIHIL;
    forma.pars.mensura  = ZEPHYRUM;
    si (pars != NIHIL)
    {
        forma.pars = chorda_ex_literis(pars, piscina);
    }
    si (a == NIHIL || !partitura_scribere(k, &forma, a))
    {
        fprintf(stderr,
            "auditio: partitura: mensurae extra carmen (I..%u) "
                "aut memoria deest\n", _mensurae(k));
        redde FALSUM;
    }
    t = chorda_aedificator_finire(a);
    fwrite(t.datum, I, (size_t)t.mensura, stdout);
    redde VERUM;
}

/* potentia II minima >= numerus, numquam < XVI */
interior i32
_capacitas (
    i32 numerus)
{
    i32 c = XVI;

    dum (c < numerus)
    {
        c *= II;
    }
    redde c;
}

nomen structura {
          Cantilena  cantilena;
       MixturaForma  forma;
            Mixtura* mixtura;
      MixturaRector* rector;
      MixturaIussum* iussa;
                i32  sonantes;   /* iussa cum ansa non nulla */
                s64  finis;      /* quadrum post caudam ultimam (ab 0) */
} AuditioCantus;

/* legit, mixturam et rectorem creat, iussa ad momentum 0 parat;
 * FALSUM cum vitio monstrato */
interior b32
_cantum_parare (
    constans character* textus,
    constans character* praeparatum,
                   b32  intactus,
                   i64  semen,
               Piscina* piscina,
         AuditioCantus* a)
{
                  CantusForma  forma;
                 CantusVitium  vitium;
    constans SonitusParametra* color;
                          i32  i;

    color = sonitus_praeparatum(praeparatum);
    si (color == NIHIL)
    {
        fprintf(stderr, "auditio: praeparatum ignotum '%s'\n",
                praeparatum);
        redde FALSUM;
    }
    si (!cantus_legere(chorda_ex_literis(textus, piscina), piscina,
                       &a->cantilena, &vitium))
    {
        _vitium_monstrare(textus, &vitium);
        redde FALSUM;
    }
    a->forma.voces   = _capacitas(a->cantilena.numerus);
    a->forma.iussa   = a->forma.voces;
    a->forma.nuntii  = II * a->forma.voces;
    a->mixtura       = mixtura_creare(piscina, &a->forma);
    a->rector        = mixtura_rectorem_creare(piscina, a->forma.voces);
    a->iussa    = (MixturaIussum*)piscina_allocare(piscina,
        (memoriae_index)a->forma.voces * magnitudo(MixturaIussum));
    si (a->mixtura == NIHIL || a->rector == NIHIL || a->iussa == NIHIL)
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }

    forma.color      = color;
    forma.resolutor  = NIHIL;
    forma.contextus  = NIHIL;
    forma.tenere     = !intactus;
    forma.grex       = MIXTURA_GREX_MUSICA;
    forma.lucrum     = 1.0f;
    forma.prioritas  = I;
    forma.semen      = semen;
    forma.momentum   = ZEPHYRUM;
    si (!cantus_iussa_parare(&a->cantilena, &forma, a->rector, a->iussa,
                             &vitium))
    {
        _vitium_monstrare(textus, &vitium);
        redde FALSUM;
    }

    a->sonantes  = ZEPHYRUM;
    a->finis     = a->cantilena.longitudo;
    per (i = ZEPHYRUM; i < a->cantilena.numerus; i++)
    {
        s64 cauda;

        si (mixtura_ansa_nulla(a->iussa[i].ansa))
        {
            perge;
        }
        a->sonantes++;
        cauda = a->iussa[i].momentum
              + (s64)sonitus_longitudo(&a->iussa[i].parametra,
                                       a->iussa[i].semen);
        si (cauda > a->finis)
        {
            a->finis = cauda;
        }
    }
    imprimere("cantus: %u notae, %.3f s (caudis inclusis %.3f s)\n",
              a->cantilena.numerus,
              (duplex)a->cantilena.longitudo / (duplex)SONUS_FREQUENTIA,
              (duplex)a->finis / (duplex)SONUS_FREQUENTIA);
    redde VERUM;
}

/* offline: iussa in mixturam, finis quadra semel redduntur, WAV stereo */
interior b32
_cantum_scribere (
         AuditioCantus* a,
    constans character* via,
               Piscina* piscina)
{
    f32* exitus;
    i32  quadra = (i32)a->finis;
    i32  i;

    per (i = ZEPHYRUM; i < a->cantilena.numerus; i++)
    {
        si (!mixtura_ansa_nulla(a->iussa[i].ansa))
        {
            (vacuum)mixtura_iubere(a->mixtura, &a->iussa[i]);
        }
    }
    exitus = (f32*)piscina_allocare(piscina,
        (memoriae_index)quadra * II * magnitudo(f32));
    si (exitus == NIHIL)
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }
    mixtura_reddere(a->mixtura, exitus, quadra);
    si (!sonus_wav_scribere(via, exitus, quadra, II, SONUS_FREQUENTIA,
                            piscina))
    {
        fprintf(stderr, "auditio: scribi non potuit '%s'\n", via);
        redde FALSUM;
    }
    imprimere("cantus -> %s\n", via);
    redde VERUM;
}

/* vivum: aperit, iubet ad nunc + mora + margo, exspectat donec omnes
 * voces finitae (terminus: finis + I s), claudit */
interior b32
_cantum_sonare (
     AuditioCantus* a,
          Piscina* piscina)
{
      AuditusForma forma;
    MixturaNuntius n;
     MixturaIussum iussum;
               s64 initium;
               s64 terminus;
               i32 finitae = ZEPHYRUM;
               i32 i;

    forma.iussa   = a->forma.iussa;
    forma.nuntii  = a->forma.nuntii;
    si (!auditus_aperire(a->mixtura, &forma, piscina))
    {
        fprintf(stderr,
            "auditio: exitus soni non apertus (sine sono)\n");
        redde FALSUM;
    }
    _dormire_ms(C);
    initium = auditus_momentum() + (s64)auditus_mora()
        + AUDITIO_MARGO;
    terminus = initium + a->finis + (s64)SONUS_FREQUENTIA;
    per (i = ZEPHYRUM; i < a->cantilena.numerus; i++)
    {
        si (mixtura_ansa_nulla(a->iussa[i].ansa))
        {
            perge;
        }
        iussum           = a->iussa[i];
        iussum.momentum  += initium;
        (vacuum)auditus_iubere(&iussum);   /* anulus >= notae */
    }
    dum (finitae < a->sonantes && auditus_momentum() < terminus)
    {
        dum (auditus_nuntium_legere(&n))
        {
            si (n.genus == MIXTURA_NUNTIUS_VOX_FINITA)
            {
                finitae++;
            }
            mixtura_rector_tractare(a->rector, &n);
        }
        _dormire_ms(X);
    }
    auditus_claudere();
    si (finitae < a->sonantes)
    {
        fprintf(stderr, "auditio: voces finitae %u/%u\n", finitae,
                a->sonantes);
        redde FALSUM;
    }
    redde VERUM;
}

/* chorda -> litterae C in piscina */
interior constans character*
_litterae (
     chorda  c,
    Piscina* piscina)
{
    character* b = (character*)piscina_allocare(piscina,
        (memoriae_index)c.mensura + I);

    si (b != NIHIL)
    {
        memcpy(b, c.datum, (size_t)c.mensura);
        b[c.mensura] = '\0';
    }
    redde b;
}

/* liber ordinarius: indicare = cantiones monstrat; aliter cantionem
 * per genus (sorte) aut titulum eligit et textum eius in 'cantus'
 * ponit. FALSUM: vitium monstratum */
interior b32
_ex_cantuali (
                    b32   indicare,
     constans character*  genus,
     constans character*  titulus,
                    i64   semen,
                Piscina*  piscina,
     constans character** cantus,
                    b32*  intactus)
{
            Cantuale  liber;
      CantualeVitium  vitium;
     constans Cantio* c = NIHIL;
                 i32  i;
                 i32  g;

    si (!cantuale_ordinarium(piscina, &liber, &vitium))
    {
        fprintf(stderr, "auditio: cantuale.stml '%.*s' %u: %s\n",
                (integer)vitium.titulus.mensura,
                (constans character*)vitium.titulus.datum,
                vitium.offset, vitium.causa);
        redde FALSUM;
    }
    si (indicare)
    {
        per (i = ZEPHYRUM; i < liber.numerus; i++)
        {
            c = &liber.cantiones[i];
            imprimere("  %-14.*s", (integer)c->titulus.mensura,
                      (constans character*)c->titulus.datum);
            per (g = ZEPHYRUM; g < c->genera_numerus; g++)
            {
                imprimere("%s%.*s", g == ZEPHYRUM ? "" : " ",
                          (integer)c->genera[g].mensura,
                          (constans character*)c->genera[g].datum);
            }
            imprimere("%s\n      %.*s\n",
                c->intactus ? "  (intactus)" : "",
                      (integer)c->textus.mensura,
                      (constans character*)c->textus.datum);
        }
        redde VERUM;
    }
    si (genus != NIHIL)
    {
        Sors sors;

        sors_seminare(&sors, semen, ZEPHYRUM);
        c = cantuale_eligere(&liber, chorda_ex_literis(genus, piscina),
                             &sors, NIHIL);
    }
    alioquin
    {
        c = cantuale_invenire(&liber, chorda_ex_literis(titulus,
            piscina));
    }
    si (c == NIHIL)
    {
        fprintf(stderr, "auditio: nulla cantilena '%s'\n",
                genus != NIHIL ? genus : titulus);
        redde FALSUM;
    }
    imprimere("cantilena: %.*s\n", (integer)c->titulus.mensura,
              (constans character*)c->titulus.datum);
    *cantus    = _litterae(c->textus, piscina);
    *intactus  = *intactus || c->intactus;
    redde *cantus != NIHIL;
}

integer
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
     constans character* titulus         = NIHIL;
     constans character* via             = NIHIL;
     constans character* cantus          = NIHIL;
     constans character* color           = "tonus";
     constans character* genus           = NIHIL;
     constans character* cantilena       = NIHIL;
     constans character* carmen          = NIHIL;
     constans character* pars            = NIHIL;
                    b32  carmina         = FALSUM;
                    b32  iterare         = FALSUM;
     constans character* liber           = NIHIL;
                    b32  partitura       = FALSUM;
                    b32  harmonia        = FALSUM;
                    b32  censura         = FALSUM;
                    b32  auscultare      = FALSUM;
     constans character* regulae         = NIHIL;
                    b32  mensurae_datae  = FALSUM;
                    i32  prima           = ZEPHYRUM;
                    i32  ultima          = ZEPHYRUM;
                    b32  indicare        = FALSUM;
                    b32  intactus        = FALSUM;
                    i64  semen           = I;
                    b32  semen_datum     = FALSUM;
                    b32  omnes           = FALSUM;
                    b32  bene            = VERUM;
                integer  k_exitus;
                integer  k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnes") == ZEPHYRUM)
        {
            omnes = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-via") == ZEPHYRUM
                     && k + I < argc)
        {
            via = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-cantus") == ZEPHYRUM
                     && k + I < argc)
        {
            cantus = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-praeparatum") == ZEPHYRUM
                     && k + I < argc)
        {
            color = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-intactus") == ZEPHYRUM)
        {
            intactus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-cantuale") == ZEPHYRUM)
        {
            indicare = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-genus") == ZEPHYRUM
                     && k + I < argc)
        {
            genus = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-cantilena") == ZEPHYRUM
                     && k + I < argc)
        {
            cantilena = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-carmen") == ZEPHYRUM
                     && k + I < argc)
        {
            carmen = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-carmina") == ZEPHYRUM)
        {
            carmina = VERUM;
        }
        alioquin si (strcmp(argv[k], "-iterare") == ZEPHYRUM)
        {
            iterare = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-liber") == ZEPHYRUM
                     && k + I < argc)
        {
            liber = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-auscultare") == ZEPHYRUM)
        {
            auscultare = VERUM;
        }
        alioquin si (strcmp(argv[k], "-censura") == ZEPHYRUM)
        {
            censura = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-regulae") == ZEPHYRUM
                     && k + I < argc)
        {
            regulae = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-harmonia") == ZEPHYRUM)
        {
            harmonia = VERUM;
        }
        alioquin si (strcmp(argv[k], "-partitura") == ZEPHYRUM)
        {
            partitura = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-mensurae") == ZEPHYRUM
                     && k + I < argc)
        {
            character* finis;

            mensurae_datae  = VERUM;
            prima           = (i32)strtoul(argv[++k], &finis, X);
            ultima          = prima;
            si (*finis == '-')
            {
                ultima = (i32)strtoul(finis + I, &finis, X);
            }
            si (*finis != '\0' || prima == ZEPHYRUM || ultima < prima)
            {
                fprintf(stderr,
                    "auditio: -mensurae N aut N-M (N >= 1)\n");
                redde II;
            }
        }
        alioquin si (   strcmp(argv[k], "-pars") == ZEPHYRUM
                     && k + I < argc)
        {
            pars = argv[++k];
        }
        alioquin si (argv[k][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "auditio: vexillum ignotum '%s'\n",
                argv[k]);
            redde II;
        }
        alioquin si (   titulus == NIHIL && cantus == NIHIL
                     && genus   == NIHIL && cantilena == NIHIL
                     && carmen  == NIHIL)
        {
            titulus = argv[k];
        }
        alioquin
        {
            semen        = (i64)strtoul(argv[k], NIHIL, X);
            semen_datum  = VERUM;
        }
    }
    si (   (iterare || pars != NIHIL || partitura || censura
        || auscultare)
        && (carmen == NIHIL || (via != NIHIL && iterare)))
    {
        fprintf(stderr,
            "auditio: -iterare, -pars, -partitura solum cum "
                "-carmen (-iterare non cum -via)\n");
        redde II;
    }
    si (   ((mensurae_datae || harmonia) && !partitura)
        || (partitura && (iterare || via != NIHIL))
        || (regulae != NIHIL && !censura)
        || (censura && (partitura || iterare || via != NIHIL))
        || (auscultare && (censura || partitura || iterare
                           || via != NIHIL || pars != NIHIL))
        || (liber != NIHIL && carmen == NIHIL && !carmina))
    {
        fprintf(stderr,
            "auditio: -mensurae cum -partitura, -regulae cum -censura "
                "(neutrum cum -iterare, -via); -liber cum -carmen aut "
                "-carmina\n");
        redde II;
    }
    si (   !omnes && !indicare && titulus == NIHIL && cantus == NIHIL
        && genus == NIHIL && cantilena == NIHIL && carmen == NIHIL
        && !carmina)
    {
        imprimere("auditio: praeparata sonitus (auditio TITULUS [semen] "
                  "[-via x.wav] | -omnes | -cantus TEXTUS [-praeparatum T] "
                  "[-intactus] [-via x.wav] [semen] | -cantuale | -genus G "
                  "| -cantilena T | -carmina | -carmen T [-iterare] "
                  "[-pars P] [-via x.wav] [-partitura [-mensurae N-M] "
                  "[-harmonia]] "
                  "[-censura [-regulae R,+R,-R]] [-auscultare] "
                  "[-liber via.stml])\n");
        _praeparata_monstrare();
        redde ZEPHYRUM;
    }

    piscina = piscina_generare_dynamicum("auditio", M * M);
    si (piscina == NIHIL)
    {
        redde I;
    }
    si (carmina || carmen != NIHIL)
    {
        Carmen* k = _ex_carminibus(liber, carmina, carmen, pars,
            !partitura, piscina, &bene);

        si (k != NIHIL && auscultare)
        {
            k_exitus = _auscultare(k, piscina);
            piscina_destruere(piscina);
            redde k_exitus;
        }
        alioquin si (k != NIHIL && censura)
        {
            CensuraForma forma;

            censura_ordinaria(&forma);
            si (   regulae != NIHIL && !_regulas_legere(regulae, &forma,
                                                     piscina))
            {
                piscina_destruere(piscina);
                redde II;
            }
            k_exitus = _censuram_scribere(k, &forma, piscina);
            piscina_destruere(piscina);
            redde k_exitus;
        }
        alioquin si (k != NIHIL && partitura)
        {
            bene = _partituram_scribere(k, prima, ultima, pars,
                harmonia, piscina);
        }
        alioquin si (k != NIHIL)
        {
            _carmen_monstrare(k);
            bene = _instrumenta_probare(k)
                && (via != NIHIL ? _carmen_scribere(k, via, piscina)
                                 : _carmen_sonare(k, iterare, piscina));
        }
        piscina_destruere(piscina);
        redde bene ? ZEPHYRUM : I;
    }
    si (indicare || genus != NIHIL || cantilena != NIHIL)
    {
        si (genus != NIHIL && !semen_datum)
        {
            semen = (i64)time(NIHIL);
        }
        bene = _ex_cantuali(indicare, genus, cantilena, semen, piscina,
                            &cantus, &intactus);
        si (indicare)
        {
            cantus = NIHIL;
        }
    }
    si (!bene || indicare)
    {
        /* vitium iam monstratum, aut index scriptus */
    }
    alioquin si (cantus != NIHIL)
    {
        AuditioCantus a;

        bene = _cantum_parare(cantus, color, intactus, semen, piscina,
            &a)
            && (via != NIHIL ? _cantum_scribere(&a, via, piscina)
                             : _cantum_sonare(&a, piscina));
    }
    alioquin si (omnes)
    {
        per (k = ZEPHYRUM; k
            < (integer)sonitus_praeparatorum_numerus(); k++)
        {
            constans character* t = sonitus_praeparati_titulus((i32)k);

            si (!_audire(t, semen, _via_temporaria(t, piscina), VERUM,
                         piscina))
            {
                bene = FALSUM;
            }
        }
    }
    alioquin
    {
        bene = _audire(titulus, semen,
                       via != NIHIL ? via
                                    : _via_temporaria(titulus, piscina),
                       via == NIHIL, piscina);
    }
    piscina_destruere(piscina);
    redde bene ? ZEPHYRUM : I;
}
