#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void sig(StmlNodus* n)
{
    i32 i, k;
    k = stml_numerus_liberorum(n);
    printf("{");
    per (i = 0; i < k; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);
        if (l->genus == STML_NODUS_ELEMENTUM) { printf("E:%.*s", l->titulus ? (int)l->titulus->mensura : 0, l->titulus ? (char*)l->titulus->datum : ""); sig(l); }
        else printf("%d:[%.*s]", (int)l->genus, l->valor ? (int)l->valor->mensura : 0, l->valor ? (char*)l->valor->datum : "");
        printf(" ");
    }
    printf("}");
}

static void judicare(const char* tag, StmlNodus* r)
{
    chorda w, w2; StmlResultus rr;
    w = stml_scribere(r, pp, FALSUM);
    rr = stml_legere(w, pp, it);
    w2 = rr.radix ? stml_scribere(rr.radix, pp, FALSUM) : w;
    printf("%s\tscriptum=[%.*s]\trelectum_status=%d\tidem_bytes=%d\tarbor_orig=", tag, (int)w.mensura, (char*)w.datum, (int)rr.status,
        (int)(w2.mensura == w.mensura && memcmp(w.datum, w2.datum, w.mensura) == 0));
    sig(r);
    printf("\tarbor_relecta=");
    if (rr.elementum_radix) sig(rr.elementum_radix); else printf("NIHIL");
    printf("\n");
}

static StmlNodus* rad(void) { return stml_elementum_creare(pp, it, "r"); }

int main(void)
{
    StmlNodus* r; StmlNodus* x; chorda c;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);

    r = rad(); stml_liberum_addere(r, stml_commentum_creare(pp, it, "a-->b")); judicare("COMMENTUM_TERMINATOR", r);
    r = rad(); stml_liberum_addere(r, stml_commentum_creare(pp, it, "a-")); judicare("COMMENTUM_FINIS_LINEA", r);
    c.datum = (i8*)"x>>y"; c.mensura = 4;
    r = rad(); stml_liberum_addere(r, stml_transclusionem_creare(pp, it, c)); judicare("TRANSCLUSIO_GT2", r);
    c.datum = (i8*)" sp "; c.mensura = 4;
    r = rad(); stml_liberum_addere(r, stml_transclusionem_creare(pp, it, c)); judicare("TRANSCLUSIO_SPATIA", r);
    r = rad(); x = stml_elementum_creare(pp, it, "a"); printf("ADDERE_QUOTA\tret=%d\n", (int)stml_attributum_addere(x, pp, it, "k", "say \"hi\" n=1")); stml_liberum_addere(r, x); judicare("ATTR_QUOTA", r);
    r = rad(); x = stml_elementum_creare(pp, it, "a"); printf("ADDERE_PUNCTUM\tret=%d\n", (int)stml_attributum_addere(x, pp, it, ".k", "v")); stml_liberum_addere(r, x); judicare("ATTR_PUNCTUM", r);
    r = rad(); x = stml_elementum_creare(pp, it, "a"); stml_attributum_addere(x, pp, it, "k", "true"); stml_liberum_addere(r, x); judicare("ATTR_TRUE_EXPLICITUM", r);
    r = rad(); stml_liberum_addere(r, stml_elementum_creare(pp, it, "9bad")); judicare("TITULUS_9BAD", r);
    r = rad(); stml_liberum_addere(r, stml_elementum_creare(pp, it, "a b")); judicare("TITULUS_SPATIUM", r);
    r = rad(); stml_liberum_addere(r, stml_elementum_creare(pp, it, "")); judicare("TITULUS_VACUUS", r);
    r = rad(); stml_liberum_addere(r, stml_elementum_creare(pp, it, "-x")); judicare("TITULUS_HYPHEN", r);
    r = rad(); stml_textum_addere(r, pp, it, "A"); stml_textum_addere(r, pp, it, "B"); judicare("TEXTUS_ADIACENTES", r);
    r = rad(); stml_textum_addere(r, pp, it, ""); stml_liberum_addere(r, stml_elementum_creare(pp, it, "b")); judicare("TEXTUS_VACUUS", r);
    r = rad(); x = stml_elementum_creare(pp, it, "cap"); x->captio_directio = STML_CAPTIO_ANTE; x->captio_numerus = 1;
        stml_liberum_addere(r, x); stml_liberum_addere(r, stml_elementum_creare(pp, it, "frater")); judicare("CAPTOR_SINE_LIBERIS", r);
    r = rad(); stml_textum_addere(r, pp, it, "x\n  y"); stml_liberum_addere(r, stml_elementum_creare(pp, it, "b")); judicare("TEXTUS_LINEA", r);
    r = rad(); stml_textum_addere(r, pp, it, "   "); stml_liberum_addere(r, stml_elementum_creare(pp, it, "b")); judicare("TEXTUS_ALBUS", r);
    r = rad(); stml_textum_addere(r, pp, it, "\n"); judicare("TEXTUS_NOVA_LINEA_SOLA", r);
    r = rad(); x = stml_elementum_creare(pp, it, "#"); x->fragmentum = VERUM; x->fragmentum_id = chorda_internare_ex_literis(it, "a b"); stml_liberum_addere(r, x); judicare("FRAGMENTUM_ID_SPATIUM", r);
    return 0;
}
