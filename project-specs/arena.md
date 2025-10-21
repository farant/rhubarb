**arenae**

**Mea sententia est quod arenae sunt alternativa ad malloc granularem.**
my understanding is that arenas are an alternative to granular malloc.

**Certe potes etiam habere arenam stack-allocatam (usque ad certam magnitudinem?) sed praesertim cum multae allocationes / deallocationes sint, quae eandem vitam habere probabiliter sunt, tunc arenae sunt optimae.**
of course you can also have a stack allocated arena (up to a certain size?) but its especially when you have many allocations / deallocations that are probably going to have the same lifecycle anyway that arenas are great.

**Modus utendi arenas est quod bibliothecae necesse est ut sint arenarum compatibiles.**
the usage pattern for arenas is that libraries need to be arena compatible. 

**Eis dabitur arena quam debent uti ad allocationiem memoriae dynamicam.**
they will get passed the arena they are supposed to use for dynamic memory allocation.

**Hoc est quod intellego esse API linguae C:**
this is what i understand the C api to be:

**Habes structuram Arena opacam.**
you have an opaque Arena struct

```c

Arena* 
arena_generare_dynamicum (
        character* nomen_arenae, 
    memoriae_index mensura_initialis);

/* 
 * nomen_arenae describitur et ab arena possidetur 
 * aliquo modo si ad quidquam adhibetur.
 */
Arena* 
arena_generare_certae_magnitudinis (
        character* nomen_arenae, 
    memoriae_index mensura_buffer);

vacuum 
arena_destruere (
    Arena* arena);

vacuum* 
arena_allocare (
            Arena* arena, 
    memoriae_index mensura);

vacuum* 
arena_conari_allocare (
            Arena* arena, 
    memoriae_index mensura);

vacuum* 
arena_allocare_ordinatum (
            Arena* arena, 
    memoriae_index mensura, 
    memoriae_index ordinatio);

/* 
 * ordinatio de eo agit utrum nova allocatio in multiplo certo
 * adiacentiarum ordinata sit necne
 */
vacuum* 
arena_conari_allocare_ordinatum (
            Arena* arena, 
    memoriae_index mensura, 
    memoriae_index ordinatio);

/* vacare sed servare buffer */
vacuum 
arena_reinitializare (
    Arena* arena); 

/* Reinitializare + memoria nihilata */
vacuum 
arena_vacare (
    Arena* arena); 

memoriae_index 
arena_summa_usus (
    constans Arena* arena);

memoriae_index 
arena_summa_reliqua (
    constans Arena* arena);

memoriae_index 
arena_summa_apex (
    constans Arena* arena);
```

**Cum arenam nomen des cum allocatur, mea sententia est quod vel nomen abicere potest vel si tu debugging aliquo modo, potes videre arenas cum nomine / proposito suo signatas**
when you give the arena a name when it is allocated my idea is that either it can throw away the name or if you are debugging somehow you can see arenas labeled with their name / purpose. 

**Hoc etiam utile esset add debugging quia tu poteras accendere logging pro arena et deinde imprimere omnem allocationem quae accidit pro ea, etc.**
this would also be helpful for debugging because you could turn on logging for an arena and then print every alloc that happens for it, etc
