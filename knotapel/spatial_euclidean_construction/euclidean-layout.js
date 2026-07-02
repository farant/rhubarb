/* euclidean-layout.js
 *
 * Compass-and-straightedge page layout engine.
 *
 * Custom elements:
 *   <euclidean-layout id="...">  — defines a construction (non-rendering)
 *   <straightedge-line id="..." from="..." to="..." />
 *   <compass-circle id="..." center="..." through="..." />
 *   <page-point name="..." at="..." />  — alias an intersection to a readable name
 *
 * Point naming convention:
 *   TL, TR, BL, BR         — page corners (predefined)
 *   line1~line2             — intersection of two lines (single point)
 *   line1~circle1.a / .b   — line-circle intersections (two points)
 *   circle1~circle2.a / .b — circle-circle intersections (two points)
 *
 * Usage on <book-page>:
 *   <book-page layout="layout-id" ...>
 *     <div place="pointA pointB">content</div>
 *   </book-page>
 *
 *   place="UL LR" positions the element with upper-left at UL, lower-right at LR.
 */
(function() {
  "use strict";

  var EPS = 1e-10;

  /* ══════════════════════════════════════════════════════════════
     GEOMETRY
     ══════════════════════════════════════════════════════════════ */

  function dist(a, b) {
    var dx = b.x - a.x, dy = b.y - a.y;
    return Math.sqrt(dx * dx + dy * dy);
  }

  /* Line(p1→p2) ∩ Line(p3→p4) → [point] or [] */
  function isectLL(p1, p2, p3, p4) {
    var d1x = p2.x - p1.x, d1y = p2.y - p1.y;
    var d2x = p4.x - p3.x, d2y = p4.y - p3.y;
    var denom = d1x * d2y - d1y * d2x;
    if (Math.abs(denom) < EPS) return [];
    var t = ((p3.x - p1.x) * d2y - (p3.y - p1.y) * d2x) / denom;
    return [{ x: p1.x + t * d1x, y: p1.y + t * d1y }];
  }

  /* Line(p1→p2) ∩ Circle(center, r) → 0–2 points
     sorted by parameter t along the line (smaller t = .a) */
  function isectLC(p1, p2, center, r) {
    var dx = p2.x - p1.x, dy = p2.y - p1.y;
    var fx = p1.x - center.x, fy = p1.y - center.y;
    var a = dx * dx + dy * dy;
    var b = 2 * (fx * dx + fy * dy);
    var c = fx * fx + fy * fy - r * r;
    var disc = b * b - 4 * a * c;
    if (disc < -EPS) return [];
    if (disc < 0) disc = 0;
    var sq = Math.sqrt(disc);
    var t1 = (-b - sq) / (2 * a);
    var t2 = (-b + sq) / (2 * a);
    var pts = [{ x: p1.x + t1 * dx, y: p1.y + t1 * dy }];
    if (Math.abs(t1 - t2) > EPS) {
      pts.push({ x: p1.x + t2 * dx, y: p1.y + t2 * dy });
    }
    return pts;
  }

  /* Circle(c1,r1) ∩ Circle(c2,r2) → 0–2 points
     sorted by x then y (smaller = .a) */
  function isectCC(c1, r1, c2, r2) {
    var dx = c2.x - c1.x, dy = c2.y - c1.y;
    var d = Math.sqrt(dx * dx + dy * dy);
    if (d < EPS) return [];
    if (d > r1 + r2 + EPS) return [];
    if (d < Math.abs(r1 - r2) - EPS) return [];
    var a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    var h2 = r1 * r1 - a * a;
    if (h2 < 0) h2 = 0;
    var h = Math.sqrt(h2);
    var mx = c1.x + a * dx / d;
    var my = c1.y + a * dy / d;
    var pts = [{ x: mx + h * dy / d, y: my - h * dx / d }];
    if (h > EPS) {
      pts.push({ x: mx - h * dy / d, y: my + h * dx / d });
    }
    if (pts.length === 2) {
      if (pts[0].x > pts[1].x + EPS ||
          (Math.abs(pts[0].x - pts[1].x) < EPS && pts[0].y > pts[1].y + EPS)) {
        var tmp = pts[0]; pts[0] = pts[1]; pts[1] = tmp;
      }
    }
    return pts;
  }

  /* ══════════════════════════════════════════════════════════════
     ENGINE
     ══════════════════════════════════════════════════════════════ */

  function Engine(layoutEl, w, h) {
    this.w = w;
    this.h = h;
    this.pts = {};
    this.objs = {};
    this.ops = [];

    /* page corners */
    this.pts["TL"] = { x: 0, y: 0 };
    this.pts["TR"] = { x: w, y: 0 };
    this.pts["BL"] = { x: 0, y: h };
    this.pts["BR"] = { x: w, y: h };

    /* implicit page edges (participate in intersections, not drawn) */
    this.objs["edge-top"]    = { type: "line", p1: this.pts["TL"], p2: this.pts["TR"], id: "edge-top" };
    this.objs["edge-right"]  = { type: "line", p1: this.pts["TR"], p2: this.pts["BR"], id: "edge-right" };
    this.objs["edge-bottom"] = { type: "line", p1: this.pts["BL"], p2: this.pts["BR"], id: "edge-bottom" };
    this.objs["edge-left"]   = { type: "line", p1: this.pts["TL"], p2: this.pts["BL"], id: "edge-left" };

    /* process construction steps in document order */
    var ch = layoutEl.children;
    for (var i = 0; i < ch.length; i++) {
      var tag = ch[i].tagName.toLowerCase();
      if (tag === "straightedge-line") this._addLine(ch[i]);
      else if (tag === "compass-circle") this._addCircle(ch[i]);
      else if (tag === "page-point") this._addAlias(ch[i]);
    }
  }

  /* resolve a point name to {x,y}, computing intersections lazily */
  Engine.prototype.resolve = function(name) {
    if (this.pts[name]) return this.pts[name];

    /* parse intersection syntax: id1~id2 or id1~id2.a or id1~id2.b */
    var clean = name;
    var dot = name.lastIndexOf(".");
    if (dot !== -1) {
      var tail = name.substring(dot + 1);
      if (tail === "a" || tail === "b") {
        clean = name.substring(0, dot);
      }
    }

    var tilde = clean.indexOf("~");
    if (tilde === -1) return null;

    var id1 = clean.substring(0, tilde);
    var id2 = clean.substring(tilde + 1);
    var o1 = this.objs[id1], o2 = this.objs[id2];
    if (!o1 || !o2) return null;

    var pts = this._intersect(o1, o2);

    /* cache all results under canonical names */
    if (pts.length >= 1) {
      this.pts[clean + ".a"] = pts[0];
      this.pts[clean] = pts[0];
    }
    if (pts.length >= 2) {
      this.pts[clean + ".b"] = pts[1];
    }

    return this.pts[name] || null;
  };

  Engine.prototype._intersect = function(o1, o2) {
    if (o1.type === "line" && o2.type === "line")
      return isectLL(o1.p1, o1.p2, o2.p1, o2.p2);
    if (o1.type === "line" && o2.type === "circle")
      return isectLC(o1.p1, o1.p2, o2.center, o2.r);
    if (o1.type === "circle" && o2.type === "line")
      return isectLC(o2.p1, o2.p2, o1.center, o1.r);
    if (o1.type === "circle" && o2.type === "circle")
      return isectCC(o1.center, o1.r, o2.center, o2.r);
    return [];
  };

  Engine.prototype._addLine = function(el) {
    var id = el.getAttribute("id");
    if (!id) return;
    var p1 = this.resolve(el.getAttribute("from"));
    var p2 = this.resolve(el.getAttribute("to"));
    if (!p1 || !p2) return;
    var obj = { type: "line", p1: p1, p2: p2, id: id };
    this.objs[id] = obj;
    this.ops.push(obj);
  };

  Engine.prototype._addCircle = function(el) {
    var id = el.getAttribute("id");
    if (!id) return;
    var c = this.resolve(el.getAttribute("center"));
    var t = this.resolve(el.getAttribute("through"));
    if (!c || !t) return;
    var obj = { type: "circle", center: c, r: dist(c, t), id: id };
    this.objs[id] = obj;
    this.ops.push(obj);
  };

  /* <page-point name="center" at="d1~d2" /> — alias a constructed point */
  Engine.prototype._addAlias = function(el) {
    var alias = el.getAttribute("name");
    var target = el.getAttribute("at");
    if (!alias || !target) return;
    var pt = this.resolve(target);
    if (pt) this.pts[alias] = pt;
  };

  /* ══════════════════════════════════════════════════════════════
     VISUALIZATION OVERLAY
     ══════════════════════════════════════════════════════════════ */

  var SVG_NS = "http://www.w3.org/2000/svg";

  function createOverlay(engine) {
    var w = engine.w, h = engine.h;
    var svg = document.createElementNS(SVG_NS, "svg");
    svg.setAttribute("width", w);
    svg.setAttribute("height", h);
    svg.setAttribute("viewBox", "0 0 " + w + " " + h);
    svg.style.cssText =
      "position:absolute;top:0;left:0;margin:0;pointer-events:none;z-index:50;overflow:hidden;";

    /* clipping rectangle */
    var defs = document.createElementNS(SVG_NS, "defs");
    var clip = document.createElementNS(SVG_NS, "clipPath");
    clip.setAttribute("id", "elc-clip");
    var cr = document.createElementNS(SVG_NS, "rect");
    cr.setAttribute("x", "0"); cr.setAttribute("y", "0");
    cr.setAttribute("width", w); cr.setAttribute("height", h);
    clip.appendChild(cr);
    defs.appendChild(clip);
    svg.appendChild(defs);

    var g = document.createElementNS(SVG_NS, "g");
    g.setAttribute("clip-path", "url(#elc-clip)");
    svg.appendChild(g);

    /* ── draw construction operations ── */
    var i, op;
    for (i = 0; i < engine.ops.length; i++) {
      op = engine.ops[i];
      if (op.type === "line") {
        var ext = _extendLine(op.p1, op.p2, w, h);
        if (ext) {
          var ln = document.createElementNS(SVG_NS, "line");
          ln.setAttribute("x1", ext.a.x); ln.setAttribute("y1", ext.a.y);
          ln.setAttribute("x2", ext.b.x); ln.setAttribute("y2", ext.b.y);
          ln.setAttribute("stroke", "rgba(46,80,144,0.55)");
          ln.setAttribute("stroke-width", "1.2");
          g.appendChild(ln);
        }
      } else if (op.type === "circle") {
        var ci = document.createElementNS(SVG_NS, "circle");
        ci.setAttribute("cx", op.center.x);
        ci.setAttribute("cy", op.center.y);
        ci.setAttribute("r", op.r);
        ci.setAttribute("fill", "none");
        ci.setAttribute("stroke", "rgba(178,34,34,0.4)");
        ci.setAttribute("stroke-width", "1.0");
        g.appendChild(ci);
      }
    }

    /* ── draw all resolved points ── */
    var names = Object.keys(engine.pts);
    var seen = {};  /* avoid duplicate dots for same coords */
    for (i = 0; i < names.length; i++) {
      var name = names[i];
      var pt = engine.pts[name];

      /* skip off-page points */
      if (pt.x < -10 || pt.x > w + 10 || pt.y < -10 || pt.y > h + 10) continue;

      var key = Math.round(pt.x) + "," + Math.round(pt.y);
      if (seen[key]) {
        /* append name to existing label and data-points */
        seen[key].label += "  " + name;
        seen[key].el.textContent = seen[key].label;
        seen[key].el.setAttribute("data-points", seen[key].el.getAttribute("data-points") + " " + name);
        seen[key].dot.setAttribute("data-points", seen[key].dot.getAttribute("data-points") + " " + name);
        continue;
      }

      /* dot */
      var dot = document.createElementNS(SVG_NS, "circle");
      dot.setAttribute("cx", pt.x);
      dot.setAttribute("cy", pt.y);
      dot.setAttribute("r", "4");
      dot.setAttribute("fill", "rgba(74,124,89,0.85)");
      dot.setAttribute("data-points", name);
      g.appendChild(dot);

      /* label */
      var lbl = document.createElementNS(SVG_NS, "text");
      lbl.setAttribute("x", pt.x + 6);
      lbl.setAttribute("y", pt.y - 6);
      lbl.setAttribute("font-size", "9");
      lbl.setAttribute("fill", "rgba(74,124,89,0.95)");
      lbl.setAttribute("font-family", "monospace");
      lbl.textContent = name;
      lbl.setAttribute("data-points", name);
      g.appendChild(lbl);

      seen[key] = { label: name, el: lbl, dot: dot };
    }

    return svg;
  }

  /* extend line to fill bounding box [0,0]–[w,h] */
  function _extendLine(p1, p2, w, h) {
    var dx = p2.x - p1.x, dy = p2.y - p1.y;
    if (Math.abs(dx) < EPS && Math.abs(dy) < EPS) return null;

    var tMin = -1e6, tMax = 1e6;

    if (Math.abs(dx) > EPS) {
      var tx0 = -p1.x / dx, txw = (w - p1.x) / dx;
      if (dx > 0) { tMin = Math.max(tMin, tx0); tMax = Math.min(tMax, txw); }
      else        { tMin = Math.max(tMin, txw); tMax = Math.min(tMax, tx0); }
    } else if (p1.x < 0 || p1.x > w) { return null; }

    if (Math.abs(dy) > EPS) {
      var ty0 = -p1.y / dy, tyh = (h - p1.y) / dy;
      if (dy > 0) { tMin = Math.max(tMin, ty0); tMax = Math.min(tMax, tyh); }
      else        { tMin = Math.max(tMin, tyh); tMax = Math.min(tMax, ty0); }
    } else if (p1.y < 0 || p1.y > h) { return null; }

    if (tMin > tMax) return null;
    return {
      a: { x: p1.x + tMin * dx, y: p1.y + tMin * dy },
      b: { x: p1.x + tMax * dx, y: p1.y + tMax * dy }
    };
  }

  /* ══════════════════════════════════════════════════════════════
     PLACEMENT
     ══════════════════════════════════════════════════════════════ */

  /* position all elements with place="pointA pointB" inside container */
  function applyPlacement(container, engine) {
    var els = container.querySelectorAll("[place]");
    if (els.length === 0) return;
    container.style.position = "relative";

    for (var i = 0; i < els.length; i++) {
      var el = els[i];
      var parts = el.getAttribute("place").split(/\s+/);
      if (parts.length < 2) continue;

      var ul = engine.resolve(parts[0]);
      var lr = engine.resolve(parts[1]);
      if (!ul || !lr) continue;

      el.style.position = "absolute";
      el.style.left   = ul.x + "px";
      el.style.top    = ul.y + "px";
      el.style.width  = (lr.x - ul.x) + "px";
      el.style.height = (lr.y - ul.y) + "px";
      el.style.overflow = "auto";
    }
  }

  /* ══════════════════════════════════════════════════════════════
     HOVER INTERACTION
     ══════════════════════════════════════════════════════════════ */

  var HOVER_COLOR = "rgba(230,140,20,1)";
  var HOVER_BG    = "rgba(230,140,20,0.08)";

  /* bind mouseover/mouseout on [place] elements to highlight anchor points */
  function bindHover(container, svgOverlay) {
    var els = container.querySelectorAll("[place]");
    if (!els.length || !svgOverlay) return;

    for (var i = 0; i < els.length; i++) {
      (function(el) {
        var names = el.getAttribute("place").split(/\s+/);

        el.addEventListener("mouseenter", function() {
          if (svgOverlay.style.display === "none") return;
          el.style.backgroundColor = HOVER_BG;
          el.style.outline = "2px solid rgba(230,140,20,0.5)";
          _highlightPoints(svgOverlay, names, true);
        });

        el.addEventListener("mouseleave", function() {
          el.style.backgroundColor = "";
          el.style.outline = "";
          _highlightPoints(svgOverlay, names, false);
        });
      })(els[i]);
    }
  }

  function _highlightPoints(svg, names, on) {
    var dots = svg.querySelectorAll("circle[data-points]");
    var labels = svg.querySelectorAll("text[data-points]");

    for (var i = 0; i < dots.length; i++) {
      var pts = dots[i].getAttribute("data-points").split(/\s+/);
      var match = false;
      for (var j = 0; j < names.length; j++) {
        if (pts.indexOf(names[j]) !== -1) { match = true; break; }
      }
      if (match) {
        dots[i].setAttribute("fill", on ? HOVER_COLOR : "rgba(74,124,89,0.85)");
        dots[i].setAttribute("r", on ? "7" : "4");
      }
    }
    for (var k = 0; k < labels.length; k++) {
      var lpts = labels[k].getAttribute("data-points").split(/\s+/);
      var lmatch = false;
      for (var m = 0; m < names.length; m++) {
        if (lpts.indexOf(names[m]) !== -1) { lmatch = true; break; }
      }
      if (lmatch) {
        labels[k].setAttribute("fill", on ? HOVER_COLOR : "rgba(74,124,89,0.95)");
        labels[k].setAttribute("font-size", on ? "12" : "9");
        labels[k].setAttribute("font-weight", on ? "bold" : "normal");
      }
    }
  }

  /* ══════════════════════════════════════════════════════════════
     CUSTOM ELEMENTS (no-op, just for valid markup)
     ══════════════════════════════════════════════════════════════ */

  if (window.customElements) {
    ["euclidean-layout", "straightedge-line", "compass-circle", "page-point"].forEach(
      function(tag) {
        if (!customElements.get(tag)) {
          customElements.define(tag, class extends HTMLElement {});
        }
      }
    );
  }

  /* ══════════════════════════════════════════════════════════════
     PUBLIC API
     ══════════════════════════════════════════════════════════════ */

  window.EuclideanLayout = {
    Engine: Engine,
    createOverlay: createOverlay,
    applyPlacement: applyPlacement,
    bindHover: bindHover
  };

})();
