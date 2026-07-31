/* fixura renominationis membrorum: structurae duae membro
 * homonymo, typedef-baptizata et tag-nominata, catena, monstrator */

typedef struct {
    int onus;
    int mensura;
} Saccus;

struct cista {
    int pondus;
    Saccus intus;
};

static int pondus = 7;

static int ponderare(Saccus* s, struct cista* c);

static int
ponderare(Saccus* s, struct cista* c)
{
    Saccus localis;

    localis.onus = s->onus + pondus;
    c->pondus = localis.onus;
    return c->intus.onus + c->pondus;
}

int membra_summa(void);

int
membra_summa(void)
{
    Saccus s;
    struct cista c;

    s.onus = 1;
    s.mensura = 2;
    c.pondus = 3;
    c.intus = s;
    return ponderare(&s, &c) + pondus;
}
