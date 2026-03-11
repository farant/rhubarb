/*
 * construction-diagram.js
 *
 * A web component for rendering 3D geometric construction diagrams
 * (points, lines, planes, spheres) as inline SVG.
 *
 * Usage:
 *   <construction-diagram view="25,40" scale="120" width="400" height="300">
 *     <c-point name="O" at="0,0,0" />
 *     <c-point name="A" at="1,0,0" color="#c44" />
 *     <c-line from="O" to="A" />
 *     <c-line from="O" to="A" extend="2" dashed />
 *     <c-sphere center="O" through="A" color="#48a" />
 *     <c-plane through="O,A,B" color="#4a4" opacity="0.12" />
 *     <c-arc center="O" from="A" to="B" color="#888" />
 *   </construction-diagram>
 *
 * Attributes on <construction-diagram>:
 *   view     - "azimuth,elevation" in degrees (default "25,20")
 *   scale    - pixels per unit (default "120")
 *   width    - SVG width in px (default "400")
 *   height   - SVG height in px (default "300")
 *   labels   - "true" or "false" to show point labels (default "true")
 *
 * No dependencies. Vanilla JS. Registers as a custom element.
 */

(function () {
  "use strict";

  /* ---- vector math (3D) ---- */

  function v3(x, y, z) { return [x, y, z]; }

  function v3_sub(a, b) { return [a[0]-b[0], a[1]-b[1], a[2]-b[2]]; }
  function v3_add(a, b) { return [a[0]+b[0], a[1]+b[1], a[2]+b[2]]; }
  function v3_scale(v, s) { return [v[0]*s, v[1]*s, v[2]*s]; }
  function v3_dot(a, b) { return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; }
  function v3_cross(a, b) {
    return [
      a[1]*b[2] - a[2]*b[1],
      a[2]*b[0] - a[0]*b[2],
      a[0]*b[1] - a[1]*b[0]
    ];
  }
  function v3_len(v) { return Math.sqrt(v3_dot(v, v)); }
  function v3_norm(v) {
    var l = v3_len(v);
    return l < 1e-12 ? [0, 0, 0] : v3_scale(v, 1/l);
  }
  function v3_lerp(a, b, t) {
    return [a[0]+(b[0]-a[0])*t, a[1]+(b[1]-a[1])*t, a[2]+(b[2]-a[2])*t];
  }

  /* ---- projection ---- */

  /*
   * Orthographic projection with azimuth (rotation around Z)
   * and elevation (tilt toward viewer).
   *
   * Returns { x, y, depth } in screen coordinates.
   * x = right, y = up (SVG y is flipped later).
   */
  function makeProjection(azDeg, elDeg) {
    var az = azDeg * Math.PI / 180;
    var el = elDeg * Math.PI / 180;
    var ca = Math.cos(az), sa = Math.sin(az);
    var ce = Math.cos(el), se = Math.sin(el);

    return function project(p) {
      /* rotate around Z by azimuth */
      var x1 = p[0] * ca - p[1] * sa;
      var y1 = p[0] * sa + p[1] * ca;
      var z1 = p[2];

      /* tilt by elevation (rotate around the new X axis) */
      var x2 = x1;
      var y2 = -z1 * ce + y1 * se;
      var z2 = z1 * se + y1 * ce;

      return { x: x2, y: -y2, depth: z2 };
    };
  }

  /* ---- SVG helpers ---- */

  var SVG_NS = "http://www.w3.org/2000/svg";

  function svgEl(tag, attrs) {
    var el = document.createElementNS(SVG_NS, tag);
    if (attrs) {
      var keys = Object.keys(attrs);
      for (var i = 0; i < keys.length; i++) {
        el.setAttribute(keys[i], attrs[keys[i]]);
      }
    }
    return el;
  }

  /* ---- parsing helpers ---- */

  function parseCoords(s) {
    var parts = s.split(",");
    return v3(parseFloat(parts[0]), parseFloat(parts[1]), parseFloat(parts[2]));
  }

  /* ---- the web component ---- */

  class ConstructionDiagram extends HTMLElement {
    connectedCallback() {
      /* Defer render so the HTML parser has time to add child elements
         (c-point, c-line, etc.) before we try to read them. */
      var self = this;
      requestAnimationFrame(function () { self.render(); });
    }

    render() {
      var self = this;

    /* ---- read attributes ---- */
    var viewAttr = (this.getAttribute("view") || "25,20").split(",");
    var azimuth = parseFloat(viewAttr[0]);
    var elevation = parseFloat(viewAttr[1]);
    var scale = parseFloat(this.getAttribute("scale") || "120");
    var svgW = parseFloat(this.getAttribute("width") || "400");
    var svgH = parseFloat(this.getAttribute("height") || "300");
    var showLabels = this.getAttribute("labels") !== "false";

    var project = makeProjection(azimuth, elevation);
    var cx = svgW / 2;
    var cy = svgH / 2;

    function toSVG(p3) {
      var p = project(p3);
      return { x: cx + p.x * scale, y: cy + p.y * scale, depth: p.depth };
    }

    /* ---- collect named points ---- */
    var points = {};
    var pointEls = this.querySelectorAll("c-point");
    for (var i = 0; i < pointEls.length; i++) {
      var pe = pointEls[i];
      var name = pe.getAttribute("name");
      var coords = parseCoords(pe.getAttribute("at") || "0,0,0");
      var color = pe.getAttribute("color") || "#222";
      var radius = parseFloat(pe.getAttribute("radius") || "3.5");
      points[name] = { coords: coords, color: color, radius: radius, name: name };
    }

    function resolvePoint(nameOrCoords) {
      if (points[nameOrCoords]) return points[nameOrCoords].coords;
      return parseCoords(nameOrCoords);
    }

    /* ---- build render list (objects with depth for sorting) ---- */
    var renderList = [];

    /* planes (render first / behind everything) */
    var planeEls = this.querySelectorAll("c-plane");
    for (i = 0; i < planeEls.length; i++) {
      var ple = planeEls[i];
      var throughStr = ple.getAttribute("through") || "";
      var pnames = throughStr.split(",");
      if (pnames.length >= 3) {
        var p0 = resolvePoint(pnames[0].trim());
        var p1 = resolvePoint(pnames[1].trim());
        var p2 = resolvePoint(pnames[2].trim());
        var planeColor = ple.getAttribute("color") || "#4a7c59";
        var planeOpacity = parseFloat(ple.getAttribute("opacity") || "0.12");
        var planeExtent = parseFloat(ple.getAttribute("extent") || "1.5");

        /* build a quad around the centroid of the 3 points */
        var centroid = v3_scale(v3_add(v3_add(p0, p1), p2), 1/3);
        var u = v3_norm(v3_sub(p1, p0));
        var n = v3_norm(v3_cross(v3_sub(p1, p0), v3_sub(p2, p0)));
        var v = v3_cross(n, u);
        var e = planeExtent;
        var corners = [
          v3_add(centroid, v3_add(v3_scale(u, -e), v3_scale(v, -e))),
          v3_add(centroid, v3_add(v3_scale(u,  e), v3_scale(v, -e))),
          v3_add(centroid, v3_add(v3_scale(u,  e), v3_scale(v,  e))),
          v3_add(centroid, v3_add(v3_scale(u, -e), v3_scale(v,  e)))
        ];

        var avgDepth = 0;
        var svgCorners = [];
        for (var ci = 0; ci < 4; ci++) {
          var sc = toSVG(corners[ci]);
          svgCorners.push(sc);
          avgDepth += sc.depth;
        }
        avgDepth /= 4;

        renderList.push({
          depth: avgDepth,
          type: "plane",
          corners: svgCorners,
          color: planeColor,
          opacity: planeOpacity
        });
      }
    }

    /* spheres */
    var sphereEls = this.querySelectorAll("c-sphere");
    for (i = 0; i < sphereEls.length; i++) {
      var se = sphereEls[i];
      var centerName = se.getAttribute("center") || "";
      var throughName = se.getAttribute("through") || "";
      var sCenter = resolvePoint(centerName);
      var sThrough = resolvePoint(throughName);
      var sRadius = v3_len(v3_sub(sThrough, sCenter));
      var sColor = se.getAttribute("color") || "#2e5090";
      var sOpacity = parseFloat(se.getAttribute("opacity") || "0.08");
      var sFill = se.getAttribute("fill") || sColor;

      var sc2 = toSVG(sCenter);
      var screenRadius = sRadius * scale;

      renderList.push({
        depth: sc2.depth,
        type: "sphere",
        cx: sc2.x, cy: sc2.y,
        r: screenRadius,
        color: sColor,
        fillColor: sFill,
        opacity: sOpacity
      });
    }

    /* lines */
    var lineEls = this.querySelectorAll("c-line");
    for (i = 0; i < lineEls.length; i++) {
      var le = lineEls[i];
      var fromName = le.getAttribute("from") || "";
      var toName = le.getAttribute("to") || "";
      var lFrom = resolvePoint(fromName);
      var lTo = resolvePoint(toName);
      var lColor = le.getAttribute("color") || "#555";
      var lWidth = parseFloat(le.getAttribute("stroke-width") || "1.5");
      var lDashed = le.hasAttribute("dashed");
      var lExtend = parseFloat(le.getAttribute("extend") || "0");

      /* optionally extend the line beyond endpoints */
      if (lExtend > 0) {
        var dir = v3_norm(v3_sub(lTo, lFrom));
        lFrom = v3_sub(lFrom, v3_scale(dir, lExtend));
        lTo = v3_add(lTo, v3_scale(dir, lExtend));
      }

      var sf = toSVG(lFrom);
      var st = toSVG(lTo);
      var avgD = (sf.depth + st.depth) / 2;

      renderList.push({
        depth: avgD,
        type: "line",
        x1: sf.x, y1: sf.y,
        x2: st.x, y2: st.y,
        color: lColor,
        width: lWidth,
        dashed: lDashed
      });
    }

    /* arcs (for showing angles, circles in planes, etc) */
    var arcEls = this.querySelectorAll("c-arc");
    for (i = 0; i < arcEls.length; i++) {
      var ae = arcEls[i];
      var arcCenter = resolvePoint(ae.getAttribute("center") || "");
      var arcFrom = resolvePoint(ae.getAttribute("from") || "");
      var arcTo = resolvePoint(ae.getAttribute("to") || "");
      var arcColor = ae.getAttribute("color") || "#888";
      var arcWidth = parseFloat(ae.getAttribute("stroke-width") || "1.2");
      var arcDashed = ae.hasAttribute("dashed");
      var arcSegments = parseInt(ae.getAttribute("segments") || "24", 10);

      /* interpolate arc from arcFrom to arcTo around arcCenter */
      var rFrom = v3_sub(arcFrom, arcCenter);
      var rTo = v3_sub(arcTo, arcCenter);
      var arcRadius = v3_len(rFrom);
      var arcNormal = v3_norm(v3_cross(rFrom, rTo));
      var u1 = v3_norm(rFrom);
      var v1 = v3_cross(arcNormal, u1);

      /* angle between from and to */
      var cosAngle = v3_dot(v3_norm(rFrom), v3_norm(rTo));
      cosAngle = Math.max(-1, Math.min(1, cosAngle));
      var totalAngle = Math.acos(cosAngle);

      var arcPoints = [];
      var arcDepthSum = 0;
      for (var ai = 0; ai <= arcSegments; ai++) {
        var angle = totalAngle * ai / arcSegments;
        var pt = v3_add(arcCenter,
          v3_add(v3_scale(u1, arcRadius * Math.cos(angle)),
                 v3_scale(v1, arcRadius * Math.sin(angle))));
        var sp = toSVG(pt);
        arcPoints.push(sp);
        arcDepthSum += sp.depth;
      }

      renderList.push({
        depth: arcDepthSum / (arcSegments + 1),
        type: "arc",
        points: arcPoints,
        color: arcColor,
        width: arcWidth,
        dashed: arcDashed
      });
    }

    /* points (rendered last / on top, but still depth-sorted among themselves) */
    var pointKeys = Object.keys(points);
    for (i = 0; i < pointKeys.length; i++) {
      var pt = points[pointKeys[i]];
      var sp = toSVG(pt.coords);
      renderList.push({
        depth: sp.depth + 0.001,  /* slight bias so points draw over lines */
        type: "point",
        x: sp.x, y: sp.y,
        color: pt.color,
        radius: pt.radius,
        name: pt.name
      });
    }

    /* ---- sort by depth (back to front) ---- */
    renderList.sort(function (a, b) { return a.depth - b.depth; });

    /* ---- create SVG ---- */
    var svg = svgEl("svg", {
      width: svgW, height: svgH,
      viewBox: "0 0 " + svgW + " " + svgH,
      style: "display:block; margin:1em auto; background:none;"
    });

    /* render each object */
    for (i = 0; i < renderList.length; i++) {
      var obj = renderList[i];

      if (obj.type === "plane") {
        var pathD = "M " + obj.corners[0].x + " " + obj.corners[0].y;
        for (var pi = 1; pi < obj.corners.length; pi++) {
          pathD += " L " + obj.corners[pi].x + " " + obj.corners[pi].y;
        }
        pathD += " Z";
        svg.appendChild(svgEl("path", {
          d: pathD,
          fill: obj.color,
          "fill-opacity": obj.opacity,
          stroke: obj.color,
          "stroke-opacity": Math.min(obj.opacity * 3, 0.5),
          "stroke-width": "0.8"
        }));
      }

      else if (obj.type === "sphere") {
        /* filled circle for the sphere silhouette */
        svg.appendChild(svgEl("circle", {
          cx: obj.cx, cy: obj.cy, r: obj.r,
          fill: obj.fillColor,
          "fill-opacity": obj.opacity,
          stroke: obj.color,
          "stroke-width": "1.2",
          "stroke-opacity": "0.4"
        }));
        /* equator ellipse (rough wireframe effect) */
        svg.appendChild(svgEl("ellipse", {
          cx: obj.cx, cy: obj.cy,
          rx: obj.r, ry: obj.r * 0.35,
          fill: "none",
          stroke: obj.color,
          "stroke-width": "0.8",
          "stroke-opacity": "0.25",
          "stroke-dasharray": "4,3"
        }));
      }

      else if (obj.type === "line") {
        var lineAttrs = {
          x1: obj.x1, y1: obj.y1,
          x2: obj.x2, y2: obj.y2,
          stroke: obj.color,
          "stroke-width": obj.width,
          "stroke-linecap": "round"
        };
        if (obj.dashed) {
          lineAttrs["stroke-dasharray"] = "6,4";
        }
        svg.appendChild(svgEl("line", lineAttrs));
      }

      else if (obj.type === "arc") {
        var arcPathD = "M " + obj.points[0].x + " " + obj.points[0].y;
        for (var aj = 1; aj < obj.points.length; aj++) {
          arcPathD += " L " + obj.points[aj].x + " " + obj.points[aj].y;
        }
        var arcAttrs = {
          d: arcPathD,
          fill: "none",
          stroke: obj.color,
          "stroke-width": obj.width,
          "stroke-linecap": "round"
        };
        if (obj.dashed) {
          arcAttrs["stroke-dasharray"] = "4,3";
        }
        svg.appendChild(svgEl("path", arcAttrs));
      }

      else if (obj.type === "point") {
        svg.appendChild(svgEl("circle", {
          cx: obj.x, cy: obj.y, r: obj.radius,
          fill: obj.color,
          stroke: "#fff",
          "stroke-width": "1"
        }));

        if (showLabels) {
          var label = svgEl("text", {
            x: obj.x + obj.radius + 4,
            y: obj.y - obj.radius - 2,
            fill: obj.color,
            "font-family": "Georgia, serif",
            "font-size": "13",
            "font-style": "italic"
          });
          label.textContent = obj.name;
          svg.appendChild(label);
        }
      }
    }

    /* ---- insert SVG ---- */
    /* clear any previous render */
    var existing = self.querySelector("svg");
    if (existing) self.removeChild(existing);
    self.appendChild(svg);
    }
  }

  /* ---- register the component ---- */
  if (typeof customElements !== "undefined") {
    customElements.define("construction-diagram", ConstructionDiagram);
  }

})();
