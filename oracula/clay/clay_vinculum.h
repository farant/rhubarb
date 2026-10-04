/* clay_vinculum.h - vinculum C purum ad Clay (oraculum dispositionis)
 *
 * ORACULA/: glutinum circa implementationes alienas quae ut oracula
 * adhibentur. Extra iudicium domus (examen C89, lint Latinus,
 * formator) PER NOMEN DIRECTORII - lingua aliena (C99, identificatores
 * Clay) necessitate, non neglegentia. Nihil hinc in lib/ intrat.
 *
 * Caput C purum (sine latina.h): includitur et a vinculo C99 (quod
 * clay.h includit) et a ductore Latino C89 (qui latina.h includit) -
 * ergo NULLUM verbum macri latini hic (interior, per, si, ...).
 * Oraculum: ../clay @ e6cc369 (zlib/libpng).
 */
#ifndef CLAY_VINCULUM_H
#define CLAY_VINCULUM_H

#define VINCULUM_APTA      0   /* fit */
#define VINCULUM_CRESCENS  1   /* grow */
#define VINCULUM_FIXA      2   /* fixed */
#define VINCULUM_PARS      3   /* percent (valor 0..1) */

#define VINCULUM_INITIUM   0
#define VINCULUM_MEDIUM    1
#define VINCULUM_FINIS     2

typedef struct {
    int   directio;            /* 0 linea (sinistrorsum), 1 columna */
    int   genus_x, genus_y;    /* VINCULUM_APTA ... */
    float valor_x, valor_y;    /* FIXA: mensura; PARS: 0..1 */
    float minimum_x, maximum_x;
    float minimum_y, maximum_y;    /* maximum 0 = sine fine */
    int   spatium_sinistrum, spatium_dextrum;
    int   spatium_superum, spatium_inferum;
    int   intervallum;
    int   allineatio_x, allineatio_y;   /* VINCULUM_INITIUM ... */
    int   praecidere_x, praecidere_y;
} VinculumForma;

/* Clay initiare cum superficie (radix implicita: linea, ea magnitudine) */
int  vinculum_initiare (float latitudo, float altitudo);
void vinculum_incipere (void);
/* mensor textus (D3): latitudo octetorum UTF-8 in cellulis; a ductore
 * datur ut Clay et dispositio eadem mensura utantur (runae) */
void vinculum_mensorem_ponere (int (*mensor)(const char* octeti,
                                             int mensura));
/* textum in nodo aperto ponere (primus liber Clay; sine involutione,
 * altitudo I) - post vinculum_aperire, ante liberos */
void vinculum_textum (const char* octeti, int mensura);
/* nodum ordine praeordinis aperire (index = id) */
void vinculum_aperire (int index, const VinculumForma* forma);
void vinculum_claudere (void);
/* layout computare; 0 si Clay errorem nuntiavit */
int  vinculum_finire (void);
/* fines nodi; 0 si nodus non inventus */
int  vinculum_fines (int index, float* x, float* y, float* latitudo,
                     float* altitudo);

#endif
