# Xar: Arrays That Don't Screw You Over

Look, I'm gonna be straight with you here. You know what the problem with dynamic arrays is? They lie to you. You call `realloc()`, it says "sure, I'll grow your array," and then what does it do? Moves the whole thing. Every pointer you had? Dead. Every reference? Invalid. You're sitting there wondering why your perfectly good pointer is now pointing at garbage.

I've had three people ask me about this TODAY. Three. "Why did my pointer break when I resized?" Because `realloc` moved your data, that's why. It's not complicated. It's just terrible.

So here's **xar** - "tabula exponentialis" if you want to be fancy about it, which means "exponential array" in Latin. You know why it's called exponential? Because that's how the segments grow. But more importantly, you know what it DOESN'T do? Move your data. Ever.

Once you put something in a xar, it stays where you put it. The pointer you get back? That's good for the life of the array. Stable indices. Stable pointers. No surprises. That's the deal.

## What Even IS This Thing?

An exponential array. A segmented array. A dynamic array that doesn't relocate your elements when it grows. Call it what you want - the point is it solves the problem that regular dynamic arrays cause.

Here's how a regular dynamic array works:
1. You allocate an array of size 16
2. You add 17 elements
3. It allocates a new array of size 32
4. It copies all 16 elements to the new location
5. Every pointer you had to those elements? Invalid now. Hope you weren't holding onto any.

Here's how xar works:
1. You allocate a xar with an initial segment of 16 elements
2. You add 17 elements
3. It allocates a NEW segment (also 16 elements)
4. Element 17 goes in the new segment
5. Elements 0-15? Still exactly where you left them. Every pointer still valid.

See the difference? No copying. No moving. Just allocate more segments as you need them.

```c
Piscina* p = piscina_generare_dynamicum("test", 4096);
Xar* xar = xar_creare(p, sizeof(i32));

i32* first = (i32*)xar_addere(xar);
*first = 42;

/* Add a thousand more elements... */
per (i32 i = 0; i < 1000; i++)
{
    i32* elem = (i32*)xar_addere(xar);
    *elem = i;
}

/* first is STILL VALID. Still points to 42. */
imprimere("%d\n", *first);  /* Prints 42 */
```

That's the whole point. Stable indices. Stable pointers. You put something at index 5, it stays at index 5. You get a pointer to it, that pointer stays valid.

## Quick Latin Lesson (Because We're Doing This)

Before we go further, let me teach you the vocabulary you're gonna need:

- **xar** = this isn't actually Latin, it's just a short name. But think "ex-array" or something
- **tabula** = table, tablet (an array is basically a table)
- **exponentialis** = exponential (the segments grow exponentially)
- **creare** = to create
- **destruere** = to destroy
- **addere** = to add
- **obtinere** = to obtain, to get
- **ponere** = to place, to put
- **invenire** = to find, to discover
- **quaerere** = to search, to seek
- **segmentum** = segment (this one's easy)
- **magnitudo** = size, magnitude
- **numerus** = number
- **capacitas** = capacity
- **elementum** = element
- **index** = index (also basically the same)
- **vacare** = to empty, to be empty
- **iterator** = iterator (yep, same word)

There. Now when you see `xar_addere` you know it means "xar add." When you see `xar_obtinere` that's "xar get." Not that hard.

## How The Segments Work (The Part You Actually Need To Understand)

Alright, pay attention because this is the clever bit. The xar is split into segments. Each segment is a contiguous chunk of memory that holds elements.

**The growth pattern:**
- Segment 0: N elements (you specify N, default is 16)
- Segment 1: N elements (same size as segment 0)
- Segment 2: 2N elements (double)
- Segment 3: 4N elements (double again)
- Segment 4: 8N elements (double again)
- And so on...

Why are the first two segments the same size? Because for small arrays (less than 32 elements with default size), you don't want to waste space with exponential growth. But after that, doubling makes sense - you get O(log n) segments total.

Let's say you have N=16 (the default first segment size):

```
Segment 0: [16 elements]  -> indices 0-15
Segment 1: [16 elements]  -> indices 16-31
Segment 2: [32 elements]  -> indices 32-63
Segment 3: [64 elements]  -> indices 64-127
Segment 4: [128 elements] -> indices 128-255
...
```

When you add element at index 0, it goes in segment 0. When you add element at index 50, it goes in segment 2. The xar figures out which segment and which offset within that segment using some bit manipulation magic. And it does it in O(1) time.

**Here's the key insight:** Once a segment is allocated, it never moves. Element 0 will always be at the start of segment 0. Element 50 will always be at offset 18 in segment 2. Your pointers stay valid.

This is different from a regular dynamic array where everything moves when you realloc. And it's different from a linked list where you have terrible cache locality. You get stable pointers AND reasonable cache performance.

## Creating a Xar (Finally, Some Code)

You've got three ways to create a xar. They're all pretty similar, just different levels of control.

### The Simple Way

```c
Piscina* piscina = piscina_generare_dynamicum("arrays", 8192);
Xar* xar = xar_creare(piscina, sizeof(MeusTypus));
```

That's it. You give it a piscina (because all allocations come from there) and the size of your element type. It creates a xar with default settings - first segment is 16 elements, memory gets zeroed out when allocated.

Latin breakdown:
- `xar_creare` = "xar create"
- `piscina` = pool (memory pool)
- `sizeof` = magnitudo in Latin, but we're stuck with C's `sizeof`

### The "I Know What I'm Doing" Way

```c
Xar* xar = xar_creare_cum_magnitudine(piscina, sizeof(MeusTypus), 64);
```

Same thing, but you specify the first segment size. If you know you're gonna have hundreds of elements, start with a bigger first segment. Saves allocations.

- `cum_magnitudine` = "with size"
- That last argument (64) is how many elements in the first segment

### The "I Really Know What I'm Doing" Way

```c
Xar* xar = xar_creare_cum_vexillis(
    piscina,
    sizeof(MeusTypus),
    64,
    XAR_VEXILLUM_SINE_ZEPHYRUM
);
```

Now you're getting fancy. You can pass flags (vexilla) to control behavior.

- `vexillis` = flags, banners
- `XAR_VEXILLUM_ORDINARIUS` = default flags (memory gets zeroed)
- `XAR_VEXILLUM_SINE_ZEPHYRUM` = don't zero memory (faster, but you get garbage)

Why would you skip zeroing? Speed. If you're gonna immediately write to every element anyway, why waste time zeroing memory you're about to overwrite?

### Destroying a Xar (The Easy Part)

```c
xar_destruere(xar);
```

Just kidding. You don't actually need to do anything. Everything's allocated from the piscina, so when you destroy the piscina, all the xar memory goes away automatically.

```c
/* This frees everything - the xar, all its segments, everything */
piscina_destruere(piscina);
```

That's the beauty of arena allocation. No tracking individual frees. No memory leaks. Just nuke the whole pool when you're done.

## Adding Elements (The Bread And Butter)

### Adding One Element

```c
i32* elem = (i32*)xar_addere(xar);
si (!elem)
{
    /* Allocation failed somehow */
    redde FALSUM;
}
*elem = 42;
```

`xar_addere` (xar add) allocates space for a new element at the end of the array and returns a pointer to it. You write your data through that pointer. Done.

The function returns the pointer to the new element. If it returns `NIHIL` (NULL), allocation failed - probably means your piscina ran out of space.

**Important:** The pointer you get back is stable. You can hold onto it. It won't change when you add more elements.

```c
i32* first = (i32*)xar_addere(xar);
*first = 100;

i32* second = (i32*)xar_addere(xar);
*second = 200;

/* first is still valid and still points to 100 */
imprimere("First is still %d\n", *first);
```

### Adding Multiple Elements (The Fast Way)

What if you've got an array of elements and you want to dump them all into the xar at once?

```c
i32 numeri[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
i32 quantos = xar_addere_multos(xar, numeri, 10);

si (quantos != 10)
{
    imprimere("Only added %d elements\n", quantos);
}
```

`xar_addere_multos` (xar add many) copies multiple elements in one shot. It's optimized - it figures out which segments it needs, allocates them if necessary, then does bulk memcpy operations per segment instead of element-by-element copies.

Returns how many elements it successfully added. Usually that's all of them, but if allocation fails partway through, you get a partial count.

This is way faster than calling `xar_addere` in a loop. Use it when you've got bulk data to load.

## Getting Elements (Reading Your Data Back)

### Getting By Index

```c
i32* elem = (i32*)xar_obtinere(xar, 5);
si (!elem)
{
    imprimere("Index 5 doesn't exist or is out of bounds\n");
    redde FALSUM;
}
imprimere("Element 5 is %d\n", *elem);
```

`xar_obtinere` (xar obtain/get) returns a pointer to the element at the given index. If the index is out of bounds or something's wrong, you get `NIHIL` back.

This is O(1). Not "amortized O(1)" or "basically O(1)" - actually O(1). The code figures out which segment and which offset using bit shifts and arithmetic. No loops.

### Setting By Index

```c
i32 valor = 999;
b32 ok = xar_ponere(xar, 10, &valor);
si (!ok)
{
    imprimere("Failed to set index 10\n");
}
```

`xar_ponere` (xar put/place) writes an element at a specific index. If that index doesn't exist yet, it allocates all the segments necessary to reach it. Then it copies your data there.

Returns `VERUM` (true) on success, `FALSUM` (false) on failure.

**Note:** This can create a sparse array. If you do `xar_ponere(xar, 1000, &valor)` on an empty xar, it allocates all segments up to index 1000, but only index 1000 has your data. Indices 0-999 exist but contain whatever garbage (or zeros if you didn't disable zeroing).

### Getting Or Creating

```c
i32* elem = (i32*)xar_obtinere_vel_creare(xar, 50);
si (!elem)
{
    imprimere("Allocation failed\n");
    redde FALSUM;
}
*elem = 777;
```

`xar_obtinere_vel_creare` (xar get or create) is like `xar_obtinere` but if the index doesn't exist, it creates it. Allocates all necessary segments, returns a pointer to the element.

This is useful for sparse arrays or when you're not adding sequentially.

## Iterating (Walking Through All Elements)

Yeah, you can just use indices in a loop:

```c
per (i32 i = 0; i < xar_numerus(xar); i++)
{
    i32* elem = (i32*)xar_obtinere(xar, i);
    imprimere("%d\n", *elem);
}
```

But there's a faster way using an iterator:

```c
XarIterator iter = xar_iterator_initium(xar);
dum (!xar_iterator_finis(&iter))
{
    i32* elem = (i32*)xar_iterator_proximum(&iter);
    imprimere("%d\n", *elem);
}
```

Why is this faster? Because the iterator walks through segments directly instead of recalculating segment/offset for every index. It just increments through memory until it hits the end of a segment, then jumps to the next segment.

Latin notes:
- `initium` = beginning, start
- `finis` = end
- `proximum` = next
- `dum` = while

The iterator pattern:
1. Initialize: `xar_iterator_initium(xar)`
2. Check if done: `xar_iterator_finis(&iter)`
3. Get next: `xar_iterator_proximum(&iter)`
4. Loop until done

The iterator is read-only. You can modify the data through the pointers you get, but don't add/remove elements while iterating. That's asking for trouble.

## Searching (Finding Stuff)

### Linear Search

```c
/* Define a comparator function */
interior integer comparare_i32(constans vacuum* a, constans vacuum* b)
{
    i32 aa = *(constans i32*)a;
    i32 bb = *(constans i32*)b;
    redde (aa < bb) ? -I : (aa > bb) ? I : ZEPHYRUM;
}

/* Search for an element */
i32 clavis = 42;
i32* resultus = (i32*)xar_invenire(xar, &clavis, comparare_i32);
si (resultus)
{
    imprimere("Found: %d\n", *resultus);
}
alioquin
{
    imprimere("Not found\n");
}
```

`xar_invenire` (xar find) does a linear search through the array. You give it a key and a comparator function. It walks through every element, calling your comparator, until it finds a match or runs out of elements.

Returns a pointer to the matching element, or `NIHIL` if not found.

If you want the index instead of the pointer:

```c
s32 index = xar_invenire_index(xar, &clavis, comparare_i32);
si (index >= 0)
{
    imprimere("Found at index %d\n", index);
}
alioquin
{
    imprimere("Not found (index = -1)\n");
}
```

### Binary Search (For Sorted Arrays)

If your array is sorted, use binary search - it's O(log n) instead of O(n):

```c
i32 clavis = 42;
i32* resultus = (i32*)xar_quaerere_binarie(xar, &clavis, comparare_i32);
si (resultus)
{
    imprimere("Found: %d\n", *resultus);
}
alioquin
{
    imprimere("Not found\n");
}
```

`xar_quaerere_binarie` (xar search binary) does a binary search. Same comparator interface, but assumes the array is sorted. If it's not sorted, you'll get garbage results.

Also has an `_index` variant:

```c
s32 index = xar_quaerere_binarie_index(xar, &clavis, comparare_i32);
```

**The comparator function:** Your comparator needs to return:
- Negative number if `a < b`
- Zero if `a == b`
- Positive number if `a > b`

This is the standard C comparator convention. Same as `qsort`. Same as every other sort/search function you've ever seen.

## Utility Functions (Useful Stuff)

### How Many Elements?

```c
i32 numerus = xar_numerus(xar);
imprimere("Array has %d elements\n", numerus);
```

`xar_numerus` returns the number of elements in the array. This is the logical size, not the capacity.

### What's The Capacity?

```c
i32 capacitas = xar_capacitas(xar);
imprimere("Can hold %d elements without allocating\n", capacitas);
```

`xar_capacitas` returns how many elements can fit in the currently allocated segments. This is typically larger than the number of elements unless you just filled a segment.

### Is It Empty?

```c
si (xar_vacuum_est(xar))
{
    imprimere("Array is empty\n");
}
```

`xar_vacuum_est` (xar is empty) returns `VERUM` if the array has zero elements.

Latin note: `vacuum` means empty or void. `est` means "is". So "vacuum est" = "is empty".

### Clear Everything

```c
xar_vacare(xar);
```

`xar_vacare` (xar empty) clears the array - sets the element count to zero. But the segments stay allocated. So if you had 1000 elements across multiple segments, after `vacare` you have zero elements but all those segments are still there, ready to be reused.

This is useful if you're reusing a xar in a loop:

```c
per (i32 i = 0; i < numerus_filos; i++)
{
    xar_vacare(xar);  /* Clear from previous iteration */

    /* Load new data */
    filum_processum(filos[i], xar);
}
```

Much faster than destroying and recreating the xar every iteration.

### Truncate

```c
xar_truncare(xar, 10);
```

`xar_truncare` (xar truncate) shrinks the array to N elements. If it has 100 elements and you truncate to 10, now it has 10 elements. Elements 10-99 are gone (logically - the memory is still there, but they're past the end of the array).

### Copy To Regular Array

```c
i32 destinatio[10];
i32 quantos = xar_copiare_ad_tabulam(xar, destinatio, 5, 10);
imprimere("Copied %d elements\n", quantos);
```

`xar_copiare_ad_tabulam` (xar copy to table) copies elements from the xar into a regular C array. Arguments:
- `xar`: source xar
- `destinatio`: destination array
- `initium`: starting index in xar (5 in this example)
- `numerus`: how many elements to copy (10 in this example)

Returns how many elements were actually copied. Usually it's what you asked for, but if you ask for more than exist, you get a partial copy.

### Debug Print

```c
xar_status_imprimere(xar);
```

`xar_status_imprimere` prints out debug info about the xar - number of elements, element size, capacity, how many segments are allocated, memory usage statistics, etc.

Useful when you're trying to figure out if your array is doing something weird.

```
Xar 'MyArray' Status:
  Numerus elementorum: 50
  Magnitudo elementi: 4 bytes
  Capacitas totalis: 64
  Numerus segmentorum: 3

  Segmenta:
    [0]: 16 elementa (64 bytes) - ALLOCATUS
    [1]: 16 elementa (64 bytes) - ALLOCATUS
    [2]: 32 elementa (128 bytes) - ALLOCATUS

  Usus memoriae:
    Allocatus: 64 elementa (256 bytes)
    Usus: 50 elementa (200 bytes)
    Utilitas: 78.1%
```

## Common Patterns (How People Actually Use This)

### Building An Array Incrementally

```c
Xar* xar = xar_creare(piscina, sizeof(Persona));

/* Read from file or network or whatever */
dum (legere_lineam(filum, linea))
{
    Persona* p = (Persona*)xar_addere(xar);
    parsere_persona(linea, p);
}

imprimere("Loaded %d personas\n", xar_numerus(xar));
```

### Keeping Stable Pointers

This is the killer feature. You add elements to the array and keep pointers to them, and those pointers stay valid.

```c
structura Nodus
{
    i32 valor;
    structura Nodus* parens;
    structura Nodus* pueri[10];
    i32 numerus_puerorum;
};

Xar* nodus_xar = xar_creare(piscina, sizeof(Nodus));

/* Create root */
Nodus* radix = (Nodus*)xar_addere(nodus_xar);
radix->valor = 0;
radix->parens = NIHIL;
radix->numerus_puerorum = 0;

/* Add children - root pointer stays valid */
per (i32 i = 0; i < 5; i++)
{
    Nodus* puer = (Nodus*)xar_addere(nodus_xar);
    puer->valor = i + 1;
    puer->parens = radix;  /* This pointer is stable */
    puer->numerus_puerorum = 0;

    radix->pueri[radix->numerus_puerorum++] = puer;
}

/* radix is still valid. All child pointers are still valid. */
```

Try doing that with a regular dynamic array. You can't. The realloc would invalidate all those pointers.

### Sparse Arrays

You can create sparse arrays by using `xar_obtinere_vel_creare` or `xar_ponere` at arbitrary indices:

```c
Xar* xar = xar_creare(piscina, sizeof(i32));

/* Put values at scattered indices */
i32 val = 100;
xar_ponere(xar, 10, &val);

val = 200;
xar_ponere(xar, 100, &val);

val = 300;
xar_ponere(xar, 1000, &val);

/* Now xar has 1001 elements (indices 0-1000) */
/* But only indices 10, 100, and 1000 have meaningful data */
```

This works because xar allocates segments on demand. You pay for the segment capacity, but segments are allocated lazily. If you never touch indices 500-799, the segment that would hold them might not even be allocated (depends on growth pattern).

### Processing In Batches

```c
Xar* xar = xar_creare(piscina, sizeof(Task));

per (i32 ficharius = 0; ficharius < numerus_ficharios; ficharius++)
{
    xar_vacare(xar);  /* Clear previous batch */

    /* Load batch */
    carcare_tasks(ficharius, xar);

    /* Process batch */
    XarIterator iter = xar_iterator_initium(xar);
    dum (!xar_iterator_finis(&iter))
    {
        Task* task = (Task*)xar_iterator_proximum(&iter);
        processum_task(task);
    }
}
```

The segments stay allocated between iterations, so there's no allocation overhead after the first batch.

## The O(1) Lookup Algorithm (For The Nerds)

You're probably wondering how `xar_obtinere` is O(1). Here's the trick.

Given index I and first segment size N:

**Fast path for first two segments:**
```
if (I < 2N):
    segment = 0 if I < N else 1
    offset = I % N
```

**For later segments (the clever bit):**
```
adjusted_index = I - 2N
segment = 2
segment_size = 2N

while (adjusted_index >= segment_size):
    adjusted_index -= segment_size
    segment_size <<= 1  /* Double it */
    segment++

offset = adjusted_index
```

That loop is O(log n) in the number of elements, but more importantly, it's O(1) in the number of segments you need to check - typically 3-6 iterations even for huge arrays.

Why? Because the segments double. With N=16:
- After 6 segments, you can hold 16+16+32+64+128+256 = 512 elements
- After 10 segments, you can hold 8,192 elements
- After 20 segments, you can hold over 8 million elements

The code actually does this computation in like 4-8 instructions using bit shifts. It's fast.

Compare this to a linked list where finding index I requires traversing I nodes - O(n). Or a hash table where you need to compute a hash and resolve collisions. Xar gives you direct indexing with stable pointers. Best of both worlds.

## API Reference (The Boring Part)

Alright, here's every function, what it does, what it takes, what it returns. You've seen most of this already but I'm listing it all for completeness.

### Creation

#### `xar_creare()`
```c
Xar* xar_creare(Piscina* piscina, i32 magnitudo_elementi);
```
Creates a xar with default settings (first segment: 16 elements, memory zeroed).

**Parameters:**
- `piscina`: Memory pool to allocate from
- `magnitudo_elementi`: Size of each element in bytes (use `sizeof`)

**Returns:** Pointer to new xar, or `NIHIL` on allocation failure

**Example:**
```c
Xar* xar = xar_creare(p, sizeof(i32));
```

---

#### `xar_creare_cum_magnitudine()`
```c
Xar* xar_creare_cum_magnitudine(
    Piscina* piscina,
    i32 magnitudo_elementi,
    i32 magnitudo_primi
);
```
Creates a xar with a custom first segment size.

**Parameters:**
- `piscina`: Memory pool
- `magnitudo_elementi`: Element size in bytes
- `magnitudo_primi`: First segment size (number of elements)

**Returns:** Pointer to new xar, or `NIHIL` on failure

**Example:**
```c
Xar* xar = xar_creare_cum_magnitudine(p, sizeof(i32), 64);
```

---

#### `xar_creare_cum_vexillis()`
```c
Xar* xar_creare_cum_vexillis(
    Piscina* piscina,
    i32 magnitudo_elementi,
    i32 magnitudo_primi,
    i32 vexilla
);
```
Creates a xar with custom first segment size and behavior flags.

**Parameters:**
- `piscina`: Memory pool
- `magnitudo_elementi`: Element size in bytes
- `magnitudo_primi`: First segment size
- `vexilla`: Flags controlling behavior

**Flags:**
- `XAR_VEXILLUM_ORDINARIUS` (0): Zero memory when allocating
- `XAR_VEXILLUM_SINE_ZEPHYRUM` (1): Don't zero memory (faster)

**Returns:** Pointer to new xar, or `NIHIL` on failure

**Example:**
```c
Xar* xar = xar_creare_cum_vexillis(
    p,
    sizeof(i32),
    64,
    XAR_VEXILLUM_SINE_ZEPHYRUM
);
```

---

#### `xar_destruere()`
```c
vacuum xar_destruere(Xar* xar);
```
Destroys a xar. Since everything is allocated from piscina, this is a no-op. Just destroy the piscina when you're done.

**Parameters:**
- `xar`: The xar to destroy (can be ignored)

---

#### `xar_ponere_vexilla()`
```c
vacuum xar_ponere_vexilla(Xar* xar, i32 vexilla);
```
Changes the behavior flags on an existing xar.

**Parameters:**
- `xar`: The xar to modify
- `vexilla`: New flags

---

### Adding Elements

#### `xar_addere()`
```c
vacuum* xar_addere(Xar* xar);
```
Adds a new element to the end of the array. Returns a pointer to the element.

**Parameters:**
- `xar`: The xar to add to

**Returns:** Pointer to the new element, or `NIHIL` on allocation failure

**Example:**
```c
i32* elem = (i32*)xar_addere(xar);
*elem = 42;
```

---

#### `xar_addere_multos()`
```c
i32 xar_addere_multos(
    Xar* xar,
    constans vacuum* elementa,
    i32 numerus
);
```
Adds multiple elements at once (bulk operation). Optimized for speed.

**Parameters:**
- `xar`: The xar to add to
- `elementa`: Pointer to array of elements to copy
- `numerus`: How many elements to add

**Returns:** Number of elements successfully added

**Example:**
```c
i32 numeri[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
i32 additi = xar_addere_multos(xar, numeri, 10);
```

---

### Getting Elements

#### `xar_obtinere()`
```c
vacuum* xar_obtinere(constans Xar* xar, i32 index);
```
Gets the element at the specified index. O(1) lookup.

**Parameters:**
- `xar`: The xar to get from
- `index`: Index of element (0-based)

**Returns:** Pointer to element, or `NIHIL` if index out of bounds

**Example:**
```c
i32* elem = (i32*)xar_obtinere(xar, 5);
si (elem) imprimere("%d\n", *elem);
```

---

#### `xar_obtinere_vel_creare()`
```c
vacuum* xar_obtinere_vel_creare(Xar* xar, i32 index);
```
Gets the element at the specified index, allocating segments if necessary.

**Parameters:**
- `xar`: The xar to get from
- `index`: Index of element

**Returns:** Pointer to element, or `NIHIL` on allocation failure

**Example:**
```c
i32* elem = (i32*)xar_obtinere_vel_creare(xar, 100);
*elem = 999;
```

---

#### `xar_ponere()`
```c
b32 xar_ponere(Xar* xar, i32 index, constans vacuum* elementum);
```
Writes an element at the specified index, allocating segments if necessary.

**Parameters:**
- `xar`: The xar to write to
- `index`: Index to write at
- `elementum`: Pointer to element data to copy

**Returns:** `VERUM` on success, `FALSUM` on failure

**Example:**
```c
i32 valor = 777;
xar_ponere(xar, 10, &valor);
```

---

### Iteration

#### `xar_iterator_initium()`
```c
XarIterator xar_iterator_initium(constans Xar* xar);
```
Initializes an iterator at the beginning of the array.

**Parameters:**
- `xar`: The xar to iterate over

**Returns:** Iterator positioned at start

**Example:**
```c
XarIterator iter = xar_iterator_initium(xar);
```

---

#### `xar_iterator_proximum()`
```c
vacuum* xar_iterator_proximum(XarIterator* iter);
```
Gets the next element from the iterator and advances it.

**Parameters:**
- `iter`: Pointer to iterator

**Returns:** Pointer to next element, or `NIHIL` when done

**Example:**
```c
dum (!xar_iterator_finis(&iter))
{
    i32* elem = (i32*)xar_iterator_proximum(&iter);
    processum(elem);
}
```

---

#### `xar_iterator_finis()`
```c
b32 xar_iterator_finis(XarIterator* iter);
```
Checks if the iterator is at the end.

**Parameters:**
- `iter`: Pointer to iterator

**Returns:** `VERUM` if done, `FALSUM` if more elements remain

---

### Searching

#### `xar_invenire()`
```c
vacuum* xar_invenire(
    constans Xar* xar,
    constans vacuum* clavis,
    XarComparator comparator
);
```
Linear search for an element matching the key.

**Parameters:**
- `xar`: The xar to search
- `clavis`: Pointer to key to search for
- `comparator`: Comparison function

**Returns:** Pointer to matching element, or `NIHIL` if not found

**Example:**
```c
i32 clavis = 42;
i32* elem = (i32*)xar_invenire(xar, &clavis, comparare_i32);
```

---

#### `xar_invenire_index()`
```c
s32 xar_invenire_index(
    constans Xar* xar,
    constans vacuum* clavis,
    XarComparator comparator
);
```
Linear search, returns index instead of pointer.

**Parameters:**
- `xar`: The xar to search
- `clavis`: Pointer to key
- `comparator`: Comparison function

**Returns:** Index of matching element, or -1 if not found

---

#### `xar_quaerere_binarie()`
```c
vacuum* xar_quaerere_binarie(
    constans Xar* xar,
    constans vacuum* clavis,
    XarComparator comparator
);
```
Binary search for an element (assumes array is sorted).

**Parameters:**
- `xar`: The xar to search (must be sorted)
- `clavis`: Pointer to key
- `comparator`: Comparison function

**Returns:** Pointer to matching element, or `NIHIL` if not found

---

#### `xar_quaerere_binarie_index()`
```c
s32 xar_quaerere_binarie_index(
    constans Xar* xar,
    constans vacuum* clavis,
    XarComparator comparator
);
```
Binary search, returns index instead of pointer.

**Parameters:**
- `xar`: The xar to search (must be sorted)
- `clavis`: Pointer to key
- `comparator`: Comparison function

**Returns:** Index of matching element, or -1 if not found

---

### Utility

#### `xar_numerus()`
```c
i32 xar_numerus(constans Xar* xar);
```
Returns the number of elements in the array.

**Returns:** Element count

---

#### `xar_capacitas()`
```c
i32 xar_capacitas(constans Xar* xar);
```
Returns the total capacity (how many elements can fit in allocated segments).

**Returns:** Capacity

---

#### `xar_vacuum_est()`
```c
b32 xar_vacuum_est(constans Xar* xar);
```
Checks if the array is empty.

**Returns:** `VERUM` if empty, `FALSUM` otherwise

---

#### `xar_vacare()`
```c
vacuum xar_vacare(Xar* xar);
```
Clears the array (sets element count to zero, but segments stay allocated).

**Parameters:**
- `xar`: The xar to clear

---

#### `xar_truncare()`
```c
vacuum xar_truncare(Xar* xar, i32 numerus_novus);
```
Truncates the array to a smaller size.

**Parameters:**
- `xar`: The xar to truncate
- `numerus_novus`: New element count (must be less than current count)

---

#### `xar_copiare_ad_tabulam()`
```c
i32 xar_copiare_ad_tabulam(
    constans Xar* xar,
    vacuum* destinatio,
    i32 initium,
    i32 numerus
);
```
Copies elements from the xar into a regular C array.

**Parameters:**
- `xar`: Source xar
- `destinatio`: Destination array
- `initium`: Starting index in xar
- `numerus`: How many elements to copy

**Returns:** Number of elements actually copied

---

#### `xar_status_imprimere()`
```c
vacuum xar_status_imprimere(constans Xar* xar);
```
Prints debug information about the xar to stdout.

**Parameters:**
- `xar`: The xar to inspect

---

## FAQ (Questions I'm Tired Of Answering)

### Q: Why not just use realloc?

Because `realloc` moves your data. Every pointer you had to elements? Invalid. Every reference? Gone. You have to track and update every single pointer, which is basically impossible in any non-trivial program.

Xar gives you stable pointers. Put something at index 5, get a pointer to it, that pointer stays valid no matter how much you grow the array.

### Q: Why not use a linked list?

Linked lists have terrible cache performance. Every node is a separate allocation, scattered all over memory. Your CPU's cache prefetcher can't help you.

Xar gives you contiguous segments. Within a segment, elements are right next to each other in memory. Your cache loves this. You get way better performance than linked lists.

Plus, xar has O(1) indexed access. Linked lists are O(n).

### Q: What's the memory overhead?

The xar structure itself is small - maybe 300 bytes including the segment table. Each segment is exactly sized for the elements it holds - no per-element overhead.

The "waste" comes from unused capacity in allocated segments. If you have 50 elements and your segments hold 16+16+32=64 capacity, you're wasting 14 elements worth of space. That's about 22% overhead for this case.

As the array grows, this percentage goes down. With 1000 elements, waste is typically under 10%.

Compare this to a linked list where every node has a "next" pointer (8 bytes on 64-bit) plus malloc overhead (16+ bytes). For small elements, that's way more overhead than xar.

### Q: Can I sort a xar?

Not directly with `qsort` because the elements aren't in one contiguous array.

You have three options:
1. Copy to a regular array with `xar_copiare_ad_tabulam`, sort that, copy back
2. Write a custom sort that works with the xar element access functions
3. Build an index array of pointers, sort that

Honestly, if you need to sort frequently, xar might not be the right choice. Use it when you need stable pointers more than you need sorting.

### Q: Is it thread-safe?

No. If you want multiple threads accessing a xar, you need your own locking.

Better option: give each thread its own xar. No contention, no locks, fast.

### Q: What happens if allocation fails?

Functions that allocate (like `xar_addere`, `xar_ponere`) return `NIHIL` or `FALSUM` on failure. You can check the return value and handle it.

Most of the time, allocation failure means your piscina is out of space. If you're using a dynamic piscina, that probably means you're out of actual RAM.

### Q: Can I shrink a xar to free memory?

Not really. Segments stay allocated once created. The only way to free memory is to destroy the piscina, which frees everything.

If you need to free memory incrementally, arena allocation (which xar uses) isn't the right tool. Use malloc/free instead and deal with all the pain that brings.

### Q: Why are the first two segments the same size?

Optimization for small arrays. If you have 20 elements and you did pure exponential growth (1, 2, 4, 8, 16...), you'd end up with a bunch of tiny segments. Waste of overhead.

By making the first two segments the same size N, small arrays (up to 2N elements) have just two segments. After that, exponential growth kicks in and is more efficient.

It's a practical compromise. Works well in practice.

### Q: What's the maximum size?

Limited by three things:
1. Maximum number of segments: 64 (hardcoded constant `XAR_MAXIMUS_SEGMENTORUM`)
2. Maximum index: 2^31-1 (indices are `i32`)
3. Available memory in your piscina

With default settings (first segment = 16), you can have about 2^37 elements across 64 segments. That's way more than you have RAM for. You'll run out of memory first.

### Q: Can I remove elements?

Not efficiently. You can truncate from the end with `xar_truncare`, but there's no "remove element at index 5" function.

If you need frequent removals, xar isn't the right structure. Use a different data structure that supports efficient removal.

### Q: Why is it called "xar"?

Short name. Easy to type. "Exponential array" is too long. "Xar" is three letters. Efficient.

The Latin name is "tabula exponentialis" if you want to be fancy about it.

### Q: Do I need to free elements?

No. Everything's allocated from the piscina. When you destroy the piscina (or it goes out of scope), all xar memory is freed automatically.

That's the whole point of arena allocation - no tracking individual allocations, no calling free on every element, no memory leaks. One cleanup call handles everything.

### Q: What if my elements contain pointers?

The xar doesn't care. It's just storing bytes. If your element type has pointers, those are your responsibility to manage.

```c
structura Persona
{
    character* nomen;  /* You manage this */
    i32 aetas;
};

Xar* xar = xar_creare(p, sizeof(Persona));
Persona* p = (Persona*)xar_addere(xar);
p->nomen = piscina_allocare(piscina, 64);  /* You allocate */
strcpy(p->nomen, "Marcus");

/* When you destroy the piscina, both the Persona
   and the nomen string are freed */
```

If your elements point to things OUTSIDE the piscina (like malloc'd memory), you need to free those yourself before destroying the piscina.

### Q: Can I have a xar of xars?

Sure. Why not.

```c
Xar* meta_xar = xar_creare(p, sizeof(Xar));
per (i32 i = 0; i < 10; i++)
{
    Xar* sub_xar = (Xar*)xar_addere(meta_xar);
    *sub_xar = *xar_creare(p, sizeof(i32));
}
```

I'm not sure why you'd want to do this, but it works. Everything's allocated from the piscina, so cleanup is still one call.

---

Alright, that's xar. It's an array that grows without moving your data. Stable pointers, O(1) indexed access, reasonable memory efficiency. Use it when you need stable pointers to elements and don't want to deal with `realloc` invalidating everything.

Questions? Too bad. Read the docs again. I've explained this fifty times today already.
