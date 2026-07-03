/* saltuarius_claves.h - Dispensatio clavium (Phase A)
 *
 * TesseraEventum -> SaltuariusIussum (iussum = quod app facere
 * debet). PURUM: structurae valorum solum - probationes eventa
 * manu construunt, sine ponte.
 *
 * NB narratio mutata ab INTENTIO: saltus-ad-litteram est
 * f<littera> (motus 'f' vim-dialecti), NON littera nuda - litterae
 * nudae navigationi (hjkl/q/G/gg) iam servitae. In RELATIO
 * notandum.
 *
 * Status pendens: 'g' (gg) et 'f' (f<littera>). Clavis alia
 * pendentia purgat. AMPLITUDO/RESUMPTUM non nostra - principale
 * ea tractat (FALSUM redditur).
 */

#ifndef SALTUARIUS_CLAVES_H
#define SALTUARIUS_CLAVES_H

#include "latina.h"
#include "tessera.h"
#include "saltuarius_res.h"

nomen enumeratio {
    SALT_ACTIO_NIHIL = 0,
    SALT_ACTIO_SURSUM,          /* k, sagitta sursum */
    SALT_ACTIO_DEORSUM,         /* j, sagitta deorsum */
    SALT_ACTIO_PARENS,          /* h, sagitta sinistra */
    SALT_ACTIO_INTRARE,         /* l, sagitta dextra, Reditus */
    SALT_ACTIO_PRIMUM,          /* gg */
    SALT_ACTIO_ULTIMUM,         /* G */
    SALT_ACTIO_PAGINA_SURSUM,   /* ctrl-u */
    SALT_ACTIO_PAGINA_DEORSUM,  /* ctrl-d */
    SALT_ACTIO_SALTARE,         /* f<littera>; runa portat */
    SALT_ACTIO_ROTA_SURSUM,
    SALT_ACTIO_ROTA_DEORSUM,
    SALT_ACTIO_CLICUS,          /* x, y portat */
    SALT_ACTIO_FINIRE,          /* q (in fonte: redire) */
    SALT_ACTIO_SINISTRA,        /* h in fonte: cursor */
    SALT_ACTIO_DEXTRA,          /* l in fonte: cursor */
    SALT_ACTIO_STRATUM,         /* L (Phase C) */
    SALT_ACTIO_ORIGO            /* o (Phase C) */
} SaltuariusActio;

nomen structura {
    SaltuariusActio genus;
    s32             runa;  /* SALTARE */
    s32             x;     /* CLICUS (cella, 0-basata) */
    s32             y;
} SaltuariusIussum;

nomen structura {
    b32 g_pendens;
    b32 f_pendens;
} SaltuariusClaves;

/* Status pendentium purus initio */
vacuum
saltuarius_claves_parare (SaltuariusClaves* claves);

/* Eventum tradere; VERUM si iussum productum (iussum_out
 * impletur), FALSUM si nihil agendum. Modus mappam mutat: in
 * FONTE h/l/sagittae horizontales cursorem movent (SINISTRA/
 * DEXTRA), Reditus INTRARE manet (Phase C: saltus originis),
 * L/o STRATUM/ORIGO fiunt. */
b32
saltuarius_claves_tradere (SaltuariusClaves* claves,
    SaltuariusModus modus, constans TesseraEventum* eventum,
    SaltuariusIussum* iussum_out);

#endif /* SALTUARIUS_CLAVES_H */
