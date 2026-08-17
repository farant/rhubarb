/* silva_unitates.c - corpus scansoris unitatum (vide caput).
 *
 * Functiones lineam/unitatem_finire VERBATIM ex amalgamatore motae
 * (logica intacta - porta byte-identica id probat); scandere =
 * involucrum enumerationis novum pro differre.
 */

#include "silva_unitates.h"
#include "silva_lexema.h"
#include "chorda_aedificator.h"

interior SilvaToken*
_ad (Xar* lexemata, i32 i);

interior SilvaToken*
_ad (Xar* lexemata, i32 i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

i32
silva_lineam_finire (Xar* lexemata, i32 i, i32 n)
{
    i32 j = i + I;

    dum (j < n)
    {
        SilvaToken* t = _ad(lexemata, j);

        si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        j++;
    }
    redde j;
}

i32
silva_unitatem_finire (Xar* lexemata, i32 i, i32 n,
    chorda* titulus_out, b32* est_functio_out, b32* est_statica_out,
    b32* est_definitio_out, b32* est_typedef_out,
    b32* est_tag_def_out)
{
    i32 pp = ZEPHYRUM;
    i32 pb = ZEPHYRUM;
    b32 vidit_clausam = FALSUM;
    b32 est_typedef = FALSUM;
    b32 titulus_fixus = FALSUM;
    chorda ultimus;
    SilvaLexemaGenus prius = SILVA_LEX_EOF;
    SilvaLexemaGenus prius_prius = SILVA_LEX_EOF;
    i32 j;

    titulus_out->datum = NIHIL;
    titulus_out->mensura = ZEPHYRUM;
    ultimus.datum = NIHIL;
    ultimus.mensura = ZEPHYRUM;
    *est_functio_out = FALSUM;
    *est_statica_out = FALSUM;
    *est_definitio_out = FALSUM;
    *est_tag_def_out = FALSUM;

    {
        SilvaToken* primum = _ad(lexemata, i);

        si (primum->genus == SILVA_LEX_STATIC)
        {
            *est_statica_out = VERUM;
        }
        alioquin si (primum->genus == SILVA_LEX_IDENTIFICATOR
            && (chorda_aequalis_literis(primum->valor, "interior")
                || chorda_aequalis_literis(primum->valor,
                       "hic_manens")
                || chorda_aequalis_literis(primum->valor,
                       "staticus")
                || chorda_aequalis_literis(primum->valor,
                       "universalis")))
        {
            *est_statica_out = VERUM;
        }
        si (primum->genus == SILVA_LEX_TYPEDEF
            || (primum->genus == SILVA_LEX_IDENTIFICATOR
                && chorda_aequalis_literis(primum->valor, "nomen")))
        {
            est_typedef = VERUM;
        }

        /* Definitio tag (structura X {...};) - titulus = tag ipse */
        si (!est_typedef
            && (primum->genus == SILVA_LEX_STRUCT
                || primum->genus == SILVA_LEX_UNION
                || primum->genus == SILVA_LEX_ENUM
                || (primum->genus == SILVA_LEX_IDENTIFICATOR
                    && (chorda_aequalis_literis(primum->valor,
                            "structura")
                        || chorda_aequalis_literis(primum->valor,
                               "unio")
                        || chorda_aequalis_literis(primum->valor,
                               "enumeratio"))))
            && i + I < n)
        {
            SilvaToken* secundum = _ad(lexemata, i + I);

            si (secundum->genus == SILVA_LEX_IDENTIFICATOR)
            {
                *titulus_out = secundum->valor;
                titulus_fixus = VERUM;
                *est_tag_def_out = VERUM;
            }
        }
    }
    *est_typedef_out = est_typedef;

    per (j = i; j < n; j++)
    {
        SilvaToken* t = _ad(lexemata, j);

        commutatio (t->genus)
        {
            casus SILVA_LEX_EOF:
                redde j;
            casus SILVA_LEX_PAREN_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !est_typedef
                    && !titulus_fixus && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    *est_functio_out = VERUM;
                    titulus_fixus = VERUM;
                }
                pp++;
                frange;
            casus SILVA_LEX_PAREN_CLAUSA:
                pp--;
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    vidit_clausam = VERUM;
                }
                frange;
            casus SILVA_LEX_BRACE_APERTA:
                si (pb == ZEPHYRUM && pp == ZEPHYRUM && vidit_clausam
                    && *est_functio_out)
                {
                    *est_definitio_out = VERUM;
                }
                pb++;
                frange;
            casus SILVA_LEX_BRACE_CLAUSA:
                pb--;
                si (pb == ZEPHYRUM && *est_definitio_out)
                {
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_SEMICOLON:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    si (titulus_out->datum == NIHIL)
                    {
                        *titulus_out = ultimus;
                    }
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_ASSIGNATIO:
            casus SILVA_LEX_QUADRA_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !titulus_fixus
                    && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    titulus_fixus = VERUM;
                }
                frange;
            casus SILVA_LEX_IDENTIFICATOR:
                /* typedef indicis functionis: nomen ... (*IDENT)(...) */
                si (est_typedef && !titulus_fixus
                    && prius == SILVA_LEX_STAR
                    && prius_prius == SILVA_LEX_PAREN_APERTA)
                {
                    *titulus_out = t->valor;
                    titulus_fixus = VERUM;
                }
                ultimus = t->valor;
                frange;
            ordinarius:
                frange;
        }
        prius_prius = prius;
        prius = t->genus;
    }
    redde n;
}

/* titulus directivae: define/undef/ifdef/ifndef -> identificator
 * sequens; include -> "via" verbatim aut <...> textu conserto */
interior chorda
_directivae_titulus (Piscina* piscina, Xar* lexemata, i32 i,
    i32 finis);

interior chorda
_directivae_titulus (Piscina* piscina, Xar* lexemata, i32 i,
    i32 finis)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    si (i + II >= finis)
    {
        redde vacua;
    }
    {
        SilvaToken* nomen_dir = _ad(lexemata, i + I);
        SilvaToken* tertium = _ad(lexemata, i + II);

        si (nomen_dir->genus != SILVA_LEX_IDENTIFICATOR)
        {
            redde vacua;
        }
        si (chorda_aequalis_literis(nomen_dir->valor, "define")
            || chorda_aequalis_literis(nomen_dir->valor, "undef")
            || chorda_aequalis_literis(nomen_dir->valor, "ifdef")
            || chorda_aequalis_literis(nomen_dir->valor, "ifndef"))
        {
            si (tertium->genus == SILVA_LEX_IDENTIFICATOR)
            {
                redde tertium->valor;
            }
            redde vacua;
        }
        si (chorda_aequalis_literis(nomen_dir->valor, "include"))
        {
            si (tertium->genus == SILVA_LEX_STRING_LIT)
            {
                redde tertium->valor;
            }
            /* forma angulata: valores lexematum a signo minoris
             * usque ad signum maioris conseruntur */
            si (tertium->genus == SILVA_LEX_MINOR)
            {
                ChordaAedificator* aed = chorda_aedificator_creare(
                    piscina, (memoriae_index)32);
                i32 j;

                per (j = i + II; j < finis; j++)
                {
                    SilvaToken* t = _ad(lexemata, j);

                    chorda_aedificator_appendere_chorda(aed,
                        t->valor);
                    si (t->genus == SILVA_LEX_MAIOR)
                    {
                        frange;
                    }
                }
                redde chorda_aedificator_finire(aed);
            }
        }
    }
    redde vacua;
}

Xar*
silva_unitates_scandere (Piscina* piscina, Xar* lexemata)
{
    Xar* unitates = xar_creare(piscina,
        (i32)magnitudo(SilvaUnitas));
    i32  n = xar_numerus(lexemata);
    i32  i = ZEPHYRUM;

    si (unitates == NIHIL)
    {
        redde NIHIL;
    }
    dum (i < n)
    {
        SilvaToken*  t = _ad(lexemata, i);
        SilvaUnitas* u;

        si (t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        u = (SilvaUnitas*)xar_addere(unitates);
        si (u == NIHIL)
        {
            redde NIHIL;
        }
        u->titulus.datum = NIHIL;
        u->titulus.mensura = ZEPHYRUM;
        u->lexema_primum = i;
        u->est_directiva = FALSUM;
        u->est_functio = FALSUM;
        u->est_statica = FALSUM;
        u->est_definitio = FALSUM;
        u->est_typedef = FALSUM;
        u->est_tag_def = FALSUM;

        si (t->genus == SILVA_LEX_CANCELLUM && t->initium_lineae)
        {
            i32 lf = silva_lineam_finire(lexemata, i, n);

            u->est_directiva = VERUM;
            u->titulus = _directivae_titulus(piscina, lexemata, i,
                lf);
            u->lexema_finis = lf;
            i = lf;
        }
        alioquin
        {
            u->lexema_finis = silva_unitatem_finire(lexemata, i, n,
                &u->titulus, &u->est_functio, &u->est_statica,
                &u->est_definitio, &u->est_typedef,
                &u->est_tag_def);
            i = u->lexema_finis;
        }
    }
    redde unitates;
}
