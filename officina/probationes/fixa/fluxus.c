int classis(int x)
{
    int genus = 0;
    switch (x) {
        case 0:
            genus = 1;
            break;
        case 1:
        case 2:
            genus = 2;
            break;
        default:
            genus = 3;
    }
    return genus;
}
int dum_probat(int n)
{
    int i = 0;
    int s = 0;
    while (i < n) {
        i = i + 1;
        if (i == 3) continue;
        if (i > 7) break;
        s = s + i;
    }
    do { s = s - 1; } while (s > 100);
    return s;
}
int salit(int n)
{
    int s = 0;
initium:
    s = s + n;
    if (s < 10) goto initium;
    return s;
}
void vacua(int n)
{
    if (n) { return; }
}
