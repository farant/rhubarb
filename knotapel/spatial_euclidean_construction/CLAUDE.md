## Spatial Euclidean Construction — Paged Layout System

Paginated two-page spread viewer for a 23-book treatise on 3D compass-and-straightedge construction. The left page shows book content (definitions, propositions, theorems, etc.). The right page shows facing content — comprehension questions, intuition checks, challenges, and connection prompts rendered as accordion items with type-based color coding.

The source document is `elements.html`. The paged viewer is `elements-paged.html`.


## Euclidean Layout System

The core innovation: page layout is determined by compass-and-straightedge constructions. Starting from page corners (TL, TR, BL, BR), you draw lines and circles, use their intersection points to define anchor positions, then place content blocks between pairs of anchor points. Form mirrors content — a treatise about geometric construction is literally laid out by geometric construction.

Pages that use a layout get zero-padding on the page body and a `#construction-wrap` div (position:relative, 100% width/height) that serves as the coordinate origin for both placed elements and the SVG overlay.


## Files

- `euclidean-layout.js` — Engine (geometry + intersection math), createOverlay (SVG debug visualization), applyPlacement (positions `[place]` elements), bindHover (highlight anchor points on mouseover)
- `elements-paged.html` — CSS, layout definitions, book content (book-page elements with facing-page children), and the paginator script
- `construction-diagram.js` — separate, handles inline construction diagrams within page content
- `elements.html` — the raw source treatise (not paginated)
- `formulae.md` — mathematical reference for the treatise content


## Custom Elements

All defined in `euclidean-layout.js` as no-op custom elements (just for valid markup parsing).

**IMPORTANT: Custom HTML elements CANNOT be self-closing. Must use `<tag></tag>` not `<tag />`.**

- `<euclidean-layout id="...">` — defines a reusable construction. Contains line/circle/point children. Set `display:none` in CSS; only consumed by the Engine.
- `<straightedge-line id="..." from="..." to="...">` — a line through two named points
- `<compass-circle id="..." center="..." through="...">` — a circle centered at one point, passing through another
- `<page-point name="..." at="...">` — aliases an intersection expression to a human-readable name
- `<book-page layout="..." book="..." title="...">` — marks a page break. `layout` references a `euclidean-layout` id. Contains the left-page content directly and right-page content inside a `<facing-page>`.
- `<facing-page>` — child of `book-page`, contains right-page content (`<page-q>` elements or arbitrary HTML)
- `<page-q type="..." q="...">` — a question for the facing page. Types: `comprehension`, `intuition`, `challenge`, `connection`. Body text is the answer. Rendered as an accordion item.


## Point Naming Convention

Predefined points:
- `TL`, `TR`, `BL`, `BR` — page corners

Implicit page edge lines (participate in intersections but are NOT drawn in the overlay):
- `edge-top`, `edge-right`, `edge-bottom`, `edge-left`

Intersection syntax:
- `line1~line2` — intersection of two lines (single point)
- `line1~circle1.a` / `.b` — line-circle intersections (two points, sorted by parameter `t` along the line; smaller t = `.a`)
- `circle1~circle2.a` / `.b` — circle-circle intersections (sorted by x then y; smaller = `.a`)

When there is only one intersection point, both the bare name (`line1~line2`) and `.a` suffix resolve to it.


## Placement

Add `place="pointA pointB"` to any div inside a `book-page` that has a layout. The element is positioned with:
- upper-left corner at `pointA`
- lower-right corner at `pointB`
- `position:absolute`, `overflow:auto`

Example from the "halves" layout:
```html
<div place="TL center">upper-left quadrant content</div>
<div place="top-mid mid-right">upper-right quadrant content</div>
```


## Grid Overlay

Toggle with the `g` key or the Grid button in the nav bar.

Visual coding:
- Blue lines — straightedge constructions
- Red circles — compass constructions
- Green dots + labels — all resolved points (named + intersection)
- Orange highlight — anchor points of whatever `[place]` element the mouse is hovering over (dots enlarge, labels bold)

The overlay is an SVG element appended to `#construction-wrap` with `pointer-events:none` and `z-index:50`. Lines are extended to fill the bounding box via parametric clipping. A `<clipPath>` keeps circles from bleeding outside the page.

**Key detail:** The SVG needs `margin:0` in its inline style to avoid the global `svg { margin: 0.8em auto; }` rule that exists for inline construction diagrams.


## Paginator Architecture

The `<script>` block at the bottom of `elements-paged.html` (line 951):

1. **init()** — queries all `<book-page>` elements, extracts left content (everything except `<facing-page>`) and right content (`<facing-page>` children), stores as `{title, book, layoutId, leftHTML, rightHTML}` in `allPages[]`.

2. **renderPage(idx)** — sets innerHTML on `#left-body` and `#right-body`. If the page has a `layoutId`:
   - Creates `#construction-wrap` div (zero padding, position:relative)
   - Moves all left-body children into it
   - Sets `leftBody.style.padding = "0"` (overrides default 20px 32px padding)
   - Uses `requestAnimationFrame` to wait for layout, then builds Engine from the referenced `<euclidean-layout>` element and applies placement

3. **showOverlay()** — rebuilds Engine fresh from DOM (not cached), creates SVG overlay, appends to `#construction-wrap`, binds hover

4. **Navigation** — arrow keys (left/right/up/down), Grid button (`g` key), book selector dropdown. Page number shown in nav bar.

5. **togglePQ()** — global function, toggles `.open` class on accordion sections (CSS controls visibility via `.pq-section.open .pq-body { display: block }`)


## Layout Shell Structure

```
#spread (flex row, 100vw x 100vh)
  .page.page-left
    .page-header (title + book name)
    .page-body#left-body
      #construction-wrap (only when layout active, zero padding)
        [place] elements (absolute positioned)
        SVG overlay (when grid visible)
    .page-footer (page number)
  .page.page-right
    .page-header
    .page-body#right-body
      .pq-section accordion items
    .page-footer
#nav-bar (fixed bottom, prev/next/grid/book-select/page-info)
```


## Content Block Styling

Left page content uses semantic classes with color-coded left borders:
- `.definition` — green (#4a7c59)
- `.postulate` — brown (#8b4513)
- `.proposition` — blue (#2e5090)
- `.theorem` — red (#b22222)
- `.corollary` — purple (#6a5acd)
- `.scholium` — gold (#cc8800)
- `.note-block` — gray (#999)

Right page question types:
- `comprehension` — green background
- `connection` — blue background
- `challenge` — red background
- `intuition` — gold background


## Example: Defining and Using a Layout

```html
<!-- Define the construction (hidden, consumed by Engine) -->
<euclidean-layout id="halves">
  <straightedge-line id="d1" from="TL" to="BR"></straightedge-line>
  <straightedge-line id="d2" from="TR" to="BL"></straightedge-line>
  <compass-circle id="ct1" center="TL" through="TR"></compass-circle>
  <compass-circle id="ct2" center="TR" through="TL"></compass-circle>
  <straightedge-line id="vmid" from="ct1~ct2.a" to="ct1~ct2.b"></straightedge-line>
  <page-point name="center" at="d1~d2"></page-point>
  <page-point name="top-mid" at="vmid~edge-top"></page-point>
</euclidean-layout>

<!-- Use it on a page -->
<book-page layout="halves" book="Book I" title="Introduction">
  <div place="TL center">
    <h3>Left column</h3>
    <p>Content positioned by construction.</p>
  </div>
  <facing-page>
    <page-q type="comprehension" q="What determines the layout?">
      The compass-and-straightedge construction.
    </page-q>
  </facing-page>
</book-page>
```
