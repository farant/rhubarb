/* sonda: lector contra documenta mala / limites scalarium */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <string.h>

#define H "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
#define P(x) H "<plist version=\"1.0\">" x "</plist>\n"
#define CASUS(t, s) { t, s, sizeof(s) - 1 }

typedef struct { const char* t; const char* s; unsigned long n; } Casus;

static Casus tab[] = {
    CASUS("post_radicem", P("<string>a</string>") "<sordes/>"),
    CASUS("post_radicem_plist2", P("<string>a</string>") "<plist version=\"1.0\"><integer>1</integer></plist>"),
    CASUS("ante_radicem", "xyz junk " P("<string>a</string>")),
    CASUS("nul_post", P("<string>a</string>") "\0junk"),
    CASUS("string_elem", P("<string>a<b/>c</string>")),
    CASUS("string_elem_txt", P("<string>a<b>ZZZ</b>c</string>")),
    CASUS("true_txt", P("<true>junk</true>")),
    CASUS("true_dict", P("<true><dict/></true>")),
    CASUS("key_elem", P("<dict><key>a<b/>c</key><integer>1</integer></dict>")),
    CASUS("c0_in_string", P("<string>a\001b</string>")),
    CASUS("nul_in_string", P("<string>a\0b</string>")),
    CASUS("ncr_dec", P("<string>&#65;</string>")),
    CASUS("ncr_hex", P("<string>&#x41;</string>")),
    CASUS("ent_ignota", P("<string>&foo;</string>")),
    CASUS("amp_nudus", P("<string>a & b</string>")),
    CASUS("lt_in_attr", P("<string>a</string>")),
    CASUS("dup_key", P("<dict><key>a</key><integer>1</integer><key>b</key><integer>3</integer><key>a</key><integer>2</integer></dict>")),
    CASUS("dup_version", H "<plist version=\"1.0\" version=\"2.0\"><string>a</string></plist>"),
    CASUS("sine_version", H "<plist><string>a</string></plist>"),
    CASUS("version_0.9", H "<plist version=\"0.9\"><string>a</string></plist>"),
    CASUS("string_attr", P("<string foo=\"x\">a</string>")),
    CASUS("int_trail_ws", P("<integer>5 </integer>")),
    CASUS("int_lead_ws", P("<integer> 5</integer>")),
    CASUS("int_hex", P("<integer>0x1F</integer>")),
    CASUS("int_u64max", P("<integer>18446744073709551615</integer>")),
    CASUS("int_s64max+1", P("<integer>9223372036854775808</integer>")),
    CASUS("int_s64min", P("<integer>-9223372036854775808</integer>")),
    CASUS("int_vacuum", P("<integer/>")),
    CASUS("int_plus", P("<integer>+5</integer>")),
    CASUS("int_mm", P("<integer>--5</integer>")),
    CASUS("int_lead0", P("<integer>007</integer>")),
    CASUS("real_nan", P("<real>nan</real>")),
    CASUS("real_inf", P("<real>inf</real>")),
    CASUS("real_pinfinity", P("<real>+infinity</real>")),
    CASUS("real_1e400", P("<real>1e400</real>")),
    CASUS("real_hex", P("<real>0x1p3</real>")),
    CASUS("real_trail_ws", P("<real>1.5 </real>")),
    CASUS("real_dot5", P("<real>.5</real>")),
    CASUS("real_1dot", P("<real>1.</real>")),
    CASUS("real_1e", P("<real>1e</real>")),
    CASUS("real_vacuum", P("<real/>")),
    CASUS("real_denorm", P("<real>4.9e-324</real>")),
    CASUS("data_eq_medio", P("<data>QQ==QUJD</data>")),
    CASUS("data_eq_A", P("<data>QQ=A</data>")),
    CASUS("data_eq_Q", P("<data>QQ=Q</data>")),
    CASUS("data_bits", P("<data>QR==</data>")),
    CASUS("data_ws_solum", P("<data> \n\t </data>")),
    CASUS("data_sine_pad", P("<data>QQ</data>")),
    CASUS("data_pad_extra", P("<data>QQ===</data>")),
    CASUS("data_malum", P("<data>Q!==</data>")),
    CASUS("date_ok", P("<date>2026-10-17T12:00:00Z</date>")),
    CASUS("date_off", P("<date>2026-10-17T12:00:00+02:00</date>")),
    CASUS("date_sine_Z", P("<date>2026-10-17T12:00:00</date>")),
    CASUS("date_solum", P("<date>2026-10-17</date>")),
    CASUS("date_feb30", P("<date>2026-02-30T00:00:00Z</date>")),
    CASUS("date_h24", P("<date>2026-10-17T24:00:00Z</date>")),
    CASUS("date_s60", P("<date>2016-12-31T23:59:60Z</date>")),
    CASUS("date_t_min", P("<date>2026-10-17t12:00:00Z</date>")),
    CASUS("date_frac", P("<date>2026-10-17T12:00:00.5Z</date>")),
    CASUS("date_y0", P("<date>0000-01-01T00:00:00Z</date>")),
    CASUS("key_sine_val", P("<dict><key>a</key></dict>")),
    CASUS("val_sine_key", P("<dict><integer>1</integer></dict>")),
    CASUS("key_key", P("<dict><key>a</key><key>b</key><integer>1</integer></dict>")),
    CASUS("key_extra_dict", P("<array><key>a</key></array>")),
    CASUS("plist_vacua", H "<plist version=\"1.0\"/>"),
    CASUS("radix_duo", P("<string>a</string><string>b</string>")),
    CASUS("non_plist", H "<dict/>"),
    CASUS("commentum", P("<array><!-- c --><integer>1</integer><?pi x?></array>")),
    CASUS("cdata", P("<string><![CDATA[x]]></string>")),
    CASUS("sordes", P("<sordes/>")),
    CASUS("bplist01", "bplist01xxxxxxxx"),
    CASUS("bplist15", "bplist15xxxxxxxx"),
    CASUS("bplist_brevis", "bplist0"),
    CASUS("bom", "\xef\xbb\xbf" P("<string>a</string>")),
    CASUS("ws_solum", "   \n "),
    CASUS("sine_prolog", "<plist version=\"1.0\"><string>a</string></plist>"),
    CASUS("tag_impar", P("<string>a</strin>")),
};

static void descr(PlistValor* v)
{
    i32 k;
    if (!v) { printf("NIHIL"); return; }
    switch (v->genus) {
    case PLIST_CHORDA: case PLIST_DATA:
        printf("%s[%u]:", v->genus == PLIST_CHORDA ? "S" : "D", (unsigned)v->textus.mensura);
        for (k = 0; k < v->textus.mensura; k++) {
            unsigned char b = (unsigned char)v->textus.datum[k];
            if (b >= 0x21 && b < 0x7f) printf("%c", b); else printf("\\x%02x", b);
        }
        break;
    case PLIST_INTEGER: printf("I:%lld", (long long)v->integrum); break;
    case PLIST_REALIS: printf("R:%.17g", v->realis); break;
    case PLIST_VERITAS: printf("B:%u", (unsigned)v->veritas); break;
    case PLIST_DIES: printf("T:%04d-%02d-%02dT%02d:%02d:%02d", (int)v->dies.dies.annus, (int)v->dies.dies.mensis,
        (int)v->dies.dies.dies, (int)v->dies.hora.hora, (int)v->dies.hora.minutum, (int)v->dies.hora.secundum); break;
    case PLIST_LISTA: printf("A{"); for (k = 0; k < plist_numerus(v); k++) { descr(plist_ad_indicem(v, k)); printf(","); } printf("}"); break;
    case PLIST_DICTIO: printf("D{");
        for (k = 0; k < plist_numerus(v); k++) { PlistPar* par = (PlistPar*)xar_obtinere(v->liberi, k);
            printf("%.*s=", (int)par->clavis.mensura, (const char*)par->clavis.datum); descr(par->valor); printf(","); }
        printf("}"); break;
    default: printf("?");
    }
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    unsigned idx;
    for (idx = 0; idx < sizeof(tab) / sizeof(tab[0]); idx++) {
        chorda c; PlistResultus r; char via[256]; FILE* f;
        c.datum = (i8*)tab[idx].s; c.mensura = (i32)tab[idx].n;
        sprintf(via, "build/sonda/plist_rt/m_%s.plist", tab[idx].t);
        f = fopen(via, "wb"); if (f) { fwrite(tab[idx].s, 1, tab[idx].n, f); fclose(f); }
        r = plist_legere(c, p, intern);
        printf("%s\tstatus=%d\tsemita=%.*s\tcausa=%.*s\t", tab[idx].t, (int)r.status,
               (int)r.semita.mensura, (const char*)r.semita.datum, (int)r.causa.mensura, (const char*)r.causa.datum);
        descr(r.radix); printf("\n");
    }
    return 0;
}
