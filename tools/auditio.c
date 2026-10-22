/* auditio.c - praeparata sonitus et cantilenas audire (sonus S1, S4)
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
 * scriptio fracta, 2 usus.
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
     constans character* titulus      = NIHIL;
     constans character* via          = NIHIL;
     constans character* cantus       = NIHIL;
     constans character* color        = "tonus";
     constans character* genus        = NIHIL;
     constans character* cantilena    = NIHIL;
                    b32  indicare     = FALSUM;
                    b32  intactus     = FALSUM;
                    i64  semen        = I;
                    b32  semen_datum  = FALSUM;
                    b32  omnes        = FALSUM;
                    b32  bene         = VERUM;
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
        alioquin si (argv[k][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "auditio: vexillum ignotum '%s'\n",
                argv[k]);
            redde II;
        }
        alioquin si (   titulus == NIHIL && cantus == NIHIL
                     && genus   == NIHIL && cantilena == NIHIL)
        {
            titulus = argv[k];
        }
        alioquin
        {
            semen        = (i64)strtoul(argv[k], NIHIL, X);
            semen_datum  = VERUM;
        }
    }
    si (   !omnes && !indicare && titulus == NIHIL && cantus == NIHIL
        && genus == NIHIL && cantilena == NIHIL)
    {
        imprimere("auditio: praeparata sonitus (auditio TITULUS [semen] "
                  "[-via x.wav] | -omnes | -cantus TEXTUS [-praeparatum T] "
                  "[-intactus] [-via x.wav] [semen] | -cantuale | -genus G "
                  "| -cantilena T)\n");
        _praeparata_monstrare();
        redde ZEPHYRUM;
    }

    piscina = piscina_generare_dynamicum("auditio", M * M);
    si (piscina == NIHIL)
    {
        redde I;
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
