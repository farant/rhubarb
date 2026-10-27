/* stb_imago.h - declarationes C89 mundae functionum stb_image quas
 * lib/imago.c adhibet (sex). Corpus: vendor/stb_imago.c, quod
 * stb_image.h cum STB_IMAGE_IMPLEMENTATION includit et vexillis
 * venditoriis laxis compilatur (aedilis.stml <vendor>; conventio
 * aedilis: X.h vendicatum -> X.c eodem loco).
 *
 * Cur: stb_image.h commentaria '//' fert. clang sub -std=c89 ea ut
 * extensionem tolerat; gcc sub -std=c89 ea omnino non legit (non
 * monitum - error parsurae), et nullum pragma id sanat. Ergo stb in
 * obiecto suo vivit, ut sqlite et bearssl.
 *
 * ANTE latina.h includendum (stb typos C nudos adhibet). Typi hic
 * cum stb_image.h congruere DEBENT: stb_imago.c utrumque includit,
 * ergo discrepantia = error compilationis, non mendacium tacitum. */
#ifndef STB_IMAGO_H
#define STB_IMAGO_H

extern unsigned char* stbi_load (char const* filename, int* x, int* y,
    int* comp, int req_comp);
extern unsigned char* stbi_load_from_memory (unsigned char const* buffer,
    int len, int* x, int* y, int* comp, int req_comp);
extern void stbi_image_free (void* retval_from_stbi_load);
extern const char* stbi_failure_reason (void);
extern int stbi_info (char const* filename, int* x, int* y, int* comp);
extern int stbi_info_from_memory (unsigned char const* buffer, int len,
    int* x, int* y, int* comp);

#endif /* STB_IMAGO_H */
