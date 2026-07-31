/* fixura renominationis membrorum: structurae duae membro
 * homonymo, typedef-baptizata et tag-nominata, catena, monstrator */

typedef struct {
    int pondus;
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

    localis.pondus = s->pondus + pondus;
    c->pondus = localis.pondus;
    return c->intus.pondus + c->pondus;
}

int membra_summa(void);

int
membra_summa(void)
{
    Saccus s;
    struct cista c;

    s.pondus = 1;
    s.mensura = 2;
    c.pondus = 3;
    c.intus = s;
    return ponderare(&s, &c) + pondus;
}
