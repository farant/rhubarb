// service worker

onmessage = function (e) {
    const result = dither(
        e.data.imageData,
        e.data.pixelSize,
        e.data.cutoff,
        e.data.dither_variant,
        e.data.custom_colors
    );
    const reply = {};
    reply.imageData = result;
    reply.pixelSize = e.data.pixelSize;
    reply.cutoff = e.data.cutoff;
    reply.repaint_id = e.data.repaint_id;
    postMessage(reply);
};

function dither(imageData, scaleFactor, cutoff, dither_variant, custom_colors) {
    let output = new ImageData(
        imageData.width * scaleFactor,
        imageData.height * scaleFactor
    );
    for (let i = 0; i < imageData.data.length; i += 4) {
        imageData.data[i] =
            imageData.data[i + 1] =
            imageData.data[i + 2] =
                Math.floor(
                    imageData.data[i] * 0.3 +
                        imageData.data[i + 1] * 0.59 +
                        imageData.data[i + 2] * 0.11
                );
    }

    // most implementations I see just distribute error into the existing image, wrapping around edge pixels
    // this implementation uses a sliding window of floats for more accuracy (probably not needed really)

    let slidingErrorWindow = [
        new Float32Array(imageData.width),
        new Float32Array(imageData.width),
        new Float32Array(imageData.width),
    ];
    const offsets = [
        [1, 0],
        [2, 0],
        [-1, 1],
        [0, 1],
        [1, 1],
        [0, 2],
    ];

    let COLORS = {
        duotone_colors: `
    rgb(240, 233, 210)
    rgb(223, 210, 175)
    rgb(103, 137, 131)
    rgb(24, 29, 49)
  `,
        basic_monotone: `
    rgb(246, 242, 228)
    rgb(223, 210, 175)
    rgb(192, 153, 98)
    rgb(74, 46, 22)
  `,
        browns: `
    rgb(136, 74, 57)
rgb(195, 129, 84)
rgb(255, 194, 111)
rgb(249, 224, 187)
    `,
        pink: `
    rgb(255, 120, 196)
rgb(225, 174, 255)
rgb(255, 189, 247)
rgb(255, 236, 236)
    `,
        blues: `
    rgb(14, 41, 84)
    rgb(31, 110, 140)
    rgb(46, 138, 153)
    rgb(132, 167, 161)
    `,
    };

    let raw_colors = COLORS.duotone_colors;

    if (COLORS[dither_variant]) {
        raw_colors = COLORS[dither_variant];
    } else if (custom_colors[0]) {
        raw_colors = `
    ${custom_colors[0]}
    ${custom_colors[1]}
    ${custom_colors[2]}
    ${custom_colors[3]}
  `;
    }

    console.log({ dither_variant });
    /*
    raw_colors = `
      rgb(247, 236, 222)
  rgb(233, 218, 193)
  rgb(158, 210, 198)
  rgb(84, 186, 185)
      `;
      */
    //rgb(188, 145, 103)
    //rgb(255, 254, 219)
    //

    let colors = raw_colors
        .trim()
        .split("\n")
        .map((raw) => {
            return raw.split(",").map((c) => +c.replace(/\D/g, ""));
        });

    colors.sort((a, b) => {
        return a[0] + a[1] + a[2] - (b[0] + b[1] + b[2]);
    });

    console.log({ colors });

    for (let y = 0, limY = imageData.height; y < limY; ++y) {
        for (let x = 0, limX = imageData.width; x < limX; ++x) {
            let i = (y * limX + x) * 4;
            let accumulatedError = Math.floor(slidingErrorWindow[0][x]);
            let expectedMono = imageData.data[i] + accumulatedError;
            let monoValue = expectedMono;

            // let breakpoints = [120 /*95*/, 160 /*140*/, 192];
            let breakpoints = [95, 145, 192];
            let grays = [0, 85, 170, 255];
            let color_index = 0;

            if (monoValue < breakpoints[0]) {
                monoValue = grays[0];
                color_index = 0;
            } else if (monoValue < breakpoints[1]) {
                monoValue = grays[1];
                color_index = 1;
            } else if (monoValue < breakpoints[2]) {
                monoValue = grays[2];
                color_index = 2;
            } else {
                monoValue = grays[3];
                color_index = 3;
            }

            /*
        if (monoValue <= Math.floor(cutoff * 255)) {
          monoValue = 0;
        } else {
          monoValue = 255;
        }
              */

            let error = (expectedMono - monoValue) / 8.0;
            for (let q = 0; q < offsets.length; ++q) {
                let offsetX = offsets[q][0] + x;
                let offsetY = offsets[q][1] + y;
                if (offsetX >= 0 && offsetX < slidingErrorWindow[0].length)
                    slidingErrorWindow[offsets[q][1]][offsetX] += error;
            }

            // this is stupid but we have to do the pixel scaling ourselves because safari insists on interpolating putImageData
            // which gives us blurry pixels (and it doesn't support the createImageBitmap call with an ImageData instance which
            // would make this easy)

            for (let scaleY = 0; scaleY < scaleFactor; ++scaleY) {
                let pixelOffset =
                    ((y * scaleFactor + scaleY) * output.width +
                        x * scaleFactor) *
                    4;
                for (let scaleX = 0; scaleX < scaleFactor; ++scaleX) {
                    output.data[pixelOffset] = colors[color_index][0];
                    output.data[pixelOffset + 1] = colors[color_index][1];
                    output.data[pixelOffset + 2] = colors[color_index][2];
                    // output.data[pixelOffset] =
                    //   output.data[pixelOffset + 1] =
                    //   output.data[pixelOffset + 2] =
                    //     monoValue;

                    if (color_index == 3) {
                        output.data[pixelOffset + 3] = 0;
                    } else {
                        output.data[pixelOffset + 3] = 255;
                    }

                    pixelOffset += 4;
                }
            }
        }
        // move the sliding window
        slidingErrorWindow.push(slidingErrorWindow.shift());
        slidingErrorWindow[2].fill(0, 0, slidingErrorWindow[2].length);
    }
    return output;
}

// web component


const DITHERED_IMAGE_STYLE = `
.ditheredImageStyle {
    width: 100%;
    height: 100%;
    padding: 0;
    margin: 0;
    image-rendering: pixelated;
}

.offscreenCanvas {
    position:relative;
    left: -10000px;
    top: -10000px;
}
`;

/* #-- <inri-as-dithered-image> :: ASDitheredImage class */

class ASDitheredImage extends HTMLElement {
    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: constructor() */

    constructor() {
        super();

        this.original_image_ = undefined;
        this.force_refresh_ = false;
        this.crunchFactor_ = this.getAutoCrunchFactor();
        this.canvas_ = undefined;
        this.context_ = undefined;
        this.image_loading_ = false;
        this.ignore_next_resize_ = false;

        /* #-- <inri-as-dithered-image> :: ASDitheredImage :: new Worker("/ditherworker") */
        this.worker_ = new Worker("/js/ditherworker.js");

        this.cutoff_ = 0.5;

        this.worker_.onmessage = ((e) => {
            const imageData = e.data.imageData;
            if (e.data.repaint_id === this.repaint_id) {
                this.context_.putImageData(imageData, 0, 0);
                this.canvas_.classList.remove("offscreenCanvas");
            }
        }).bind(this);

        this.resizing_timeout_ = undefined;

        this.last_draw_state_ = {
            width: 0,
            height: 0,
            crunchFactor: 0,
            imageSrc: "",
        };
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: connectedCallback() */

    connectedCallback() {
        if (!this.isConnected) {
            return;
        }

        const shadowDOM = this.attachShadow({ mode: "open" });

        const style = document.createElement("style");
        style.innerHTML = DITHERED_IMAGE_STYLE;
        shadowDOM.appendChild(style);

        this.canvas_ = document.createElement("canvas");
        this.canvas_.setAttribute("role", "image");
        this.canvas_.setAttribute("aria-label", this.getAttribute("alt"));
        this.canvas_.classList.add("ditheredImageStyle");
        this.canvas_.classList.add("offscreenCanvas");
        shadowDOM.appendChild(this.canvas_);

        this.context_ = this.canvas_.getContext("2d", {
            willReadFrequently: true,
        });

        window.setTimeout(() => {
            const resizeObserver = new ResizeObserver(
                ((entries) => {
                    // browsers generated lots of resize events but we don't want to start refreshing until
                    // the user has stopped resizing the page

                    if (entries.length > 0) {
                        if (entries[0].contentBoxSize) {
                            if (this.ignore_next_resize_ == true) {
                                this.ignore_next_resize_ = false;
                                return;
                            }
                            if (this.resizing_timeout_ != undefined) {
                                clearTimeout(this.resizing_timeout_);
                            }
                            this.resizing_timeout_ = setTimeout(
                                (() => {
                                    this.resizing_timeout_ = undefined;
                                    this.force_refresh_ = true;
                                    this.requestUpdate();
                                }).bind(this),
                                200
                            );
                        }
                    }
                }).bind(this)
            );

            resizeObserver.observe(this.canvas_);
        }, 300);

        // since we avoid drawing the image if the element of far offscreen we need to use
        // an IntersectionObserver to notify use when the element is likely to be displayed
        const intersectionObserver = new IntersectionObserver(
            ((intersections) => {
                if (intersections.length > 0) {
                    if (intersections[0].isIntersecting) {
                        this.force_refresh_ = true;
                        this.requestUpdate();
                    }
                }
            }).bind(this),
            { root: null, rootMargin: "1000px", threshold: [0] }
        );
        intersectionObserver.observe(this);

        this.force_refresh_ = true;
        this.requestUpdate();
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: observedAttributes() */

    static get observedAttributes() {
        return ["src", "crunch", "alt", "cutoff"];
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: attributeChangedCallback() */

    attributeChangedCallback(name, oldValue, newValue) {
        if (oldValue === newValue) return;

        if (name === "src") {
            this.force_refresh_ = true;
            this.original_image_ = undefined;
            this.requestUpdate();
        } else if (name === "crunch") {
            if (newValue === "auto") {
                this.crunchFactor_ = this.getAutoCrunchFactor();
            } else if (newValue === "pixel") {
                this.crunchFactor_ = 1.0 / this.getDevicePixelRatio();
            } else {
                this.crunchFactor_ = parseInt(newValue, 10);
                if (isNaN(this.crunchFactor_)) {
                    this.crunchFactor_ = this.getAutoCrunchFactor();
                }
            }
            this.force_refresh_ = true;
            this.requestUpdate();
        } else if (name === "alt") {
            this.altText = newValue;
            if (this.canvas != undefined) {
                let currentAltText = this.canvas.getAttribute("aria-label");
                if (currentAltText != newValue) {
                    this.canvas.setAttribute("aria-label", newValue);
                }
            }
        } else if (name === "cutoff") {
            this.cutoff_ = parseFloat(newValue);
            if (isNaN(this.cutoff_)) {
                this.cutoff_ = 0.5;
            }
            this.cutoff_ = Math.min(1.0, Math.max(0.0, this.cutoff_));
            this.force_refresh_ = true;
            this.requestUpdate();
        }
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: getAutoCrunchFactor() */

    // The crunch factor defaults 1 css pixel to 1 dither pixel which I think looks best when the device pixel ratio is 1 or 2
    // If the pixel ratio is 3 or above (like on my iPhone) then even css pixels are too small to make dithering
    // look effective, so I double the pixels again
    getAutoCrunchFactor() {
        if (this.getDevicePixelRatio() < 3) {
            return 1;
        } else {
            return 2;
        }
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: getDevicePixelRation() */

    getDevicePixelRatio() {
        // this should always be an integer for the dithering code to work
        return window.devicePixelRatio;
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: isInOrNearViewport() */

    isInOrNearViewport() {
        // this only handles vertical scrolling, could be extended later to handle horizontal
        // but it probably doesn't matter
        const margin = 1500;
        const r = this.getBoundingClientRect();

        const viewHeight = Math.max(
            document.documentElement.clientHeight,
            window.innerHeight
        );
        const above = r.bottom + margin < 0;
        const below = r.top - margin > viewHeight;

        return !above && !below;
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: requestUpdate() */

    // all drawing is funneled through requestUpdate so that multiple calls are coalesced to prevent
    // processing the image multiple times for no good reason
    requestUpdate() {
        if (this.original_image_ != undefined) {
            if (this.isInOrNearViewport() == false) {
                return; // suppress update, the intersection observer will call us back as the element scrolls into view
            }
        }

        window.requestAnimationFrame(
            ((timestamp) => {
                if (this.force_refresh_ == false) {
                    return;
                }
                if (this.original_image_ == undefined) {
                    this.loadImage();
                    return;
                }
                if (this.force_refresh_) {
                    this.repaintImage();
                }
            }).bind(this)
        );
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: loadImage() */

    async loadImage() {
        if (this.image_loading_ == true) {
            return;
        }
        this.image_loading_ = true;
        const image = new Image();
        image.src = this.getAttribute("src");
        this.has_border = this.getAttribute("border") == "true";

        let decoding_border = null;
        let border_image = null;

        if (this.has_border) {
            border_image = new Image();
            //border 2
            //border_image.src = "/uploads/RXDcTMa2mNZOz6lF6wcoa";

            //border 1
            // border_image.src = "/uploads/uA0NrZpeeH045oluAKqUR";

            //border 3
            // border_image.src = "/uploads/eh6dnDJk54A0EVU2oHjcX";

            let grunge_images = [
                "/images/border-1.png",
                "/images/border-2.png",
                "/images/border-3.png",
                "/images/border-4.png",
                "/images/border-5.png",
            ];

            border_image.src =
                grunge_images[Math.floor(Math.random() * grunge_images.length)];

            decoding_border = border_image.decode();
        }

        // image.onerror is old and (literally) busted - it does not file on decode errors (ie if the src does not point to a valid image)
        // The new way is promise based - possibly better
        try {
            await image.decode();

            if (this.has_border) {
                await decoding_border;
                this.border_image_ = border_image;
            }

            this.original_image_ = image;
            this.ignore_next_resize_ = true;
            this.canvas_.style.aspectRatio =
                this.original_image_.width + "/" + this.original_image_.height;
            this.force_refresh_ = true;
            this.requestUpdate();
        } catch (decodeError) {
            console.log("Error decoding image: ", decodeError);
            this.original_image_ = undefined;
        } finally {
            this.image_loading_ = false;
        }
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: repaintImage() */

    repaintImage() {
        const rect = this.canvas_.getBoundingClientRect();
        let screenPixelsToBackingStorePixels = this.getDevicePixelRatio();
        let fractionalPart =
            screenPixelsToBackingStorePixels -
            Math.floor(screenPixelsToBackingStorePixels);

        // that's it! I am officially giving up on trying to account for all the weird pixelDeviceRatios that Chrome likes
        // to serve up at different zoom levels. I can understand nice fractions like 2.5 but 1.110004 and 0.89233 are just stupid
        // If the fractional part doesn't make sense then just ignore it. This will give incorrect results but they still look
        // pretty good if you don't look too closely.
        if (1.0 / fractionalPart > 3) {
            fractionalPart = 0;
            screenPixelsToBackingStorePixels = Math.round(
                screenPixelsToBackingStorePixels
            );
        }
        if (fractionalPart != 0) {
            screenPixelsToBackingStorePixels = Math.round(
                screenPixelsToBackingStorePixels *
                    Math.round(1.0 / fractionalPart)
            );
        }

        const calculatedWidth = Math.round(
            rect.width * screenPixelsToBackingStorePixels
        );
        const calculatedHeight = Math.round(
            rect.height * screenPixelsToBackingStorePixels
        );
        let adjustedPixelSize =
            screenPixelsToBackingStorePixels * this.crunchFactor_;

        // double check - we may have already painted this image
        if (
            this.last_draw_state_.width == calculatedWidth &&
            this.last_draw_state_.height == calculatedHeight &&
            this.last_draw_state_.adjustedPixelSize == adjustedPixelSize &&
            this.last_draw_state_.imageSrc == this.original_image_.currentSrc &&
            this.last_draw_state_.cutoff == this.cutoff_
        ) {
            return; // nothing to do
        }

        this.canvas_.width = calculatedWidth;
        this.canvas_.height = calculatedHeight;

        this.last_draw_state_.width = this.canvas_.width;
        this.last_draw_state_.height = this.canvas_.height;
        this.last_draw_state_.adjustedPixelSize = adjustedPixelSize;
        this.last_draw_state_.imageSrc = this.original_image_.currentSrc;
        this.last_draw_state_.cutoff = this.cutoff_;

        let border_x = 0;
        let border_top = border_x;
        let border_bottom = border_x;

        this.context_.imageSmoothingEnabled = true;
        this.canvas_.classList.add("offscreenCanvas");

        if (this.has_border) {
            this.context_.drawImage(
                this.original_image_,
                border_x / adjustedPixelSize,
                border_top / adjustedPixelSize,
                (this.canvas_.width - border_x) / adjustedPixelSize,
                (this.canvas_.height - border_bottom) / adjustedPixelSize
            );
            this.context_.drawImage(
                this.border_image_,
                0,
                0,
                this.canvas_.width / adjustedPixelSize,
                this.canvas_.height / adjustedPixelSize
            );
        } else {
            this.context_.drawImage(
                this.original_image_,
                0,
                0,
                this.canvas_.width / adjustedPixelSize,
                this.canvas_.height / adjustedPixelSize
            );
        }

        const originalData = this.context_.getImageData(
            0,
            0,
            this.canvas_.width / adjustedPixelSize,
            this.canvas_.height / adjustedPixelSize
        );
        this.context_.fillStyle = "rgba(0,0,0,0)";
        this.context_.fillRect(0, 0, this.canvas_.width, this.canvas_.height);
        // TODO: look at transferring the data in a different datastructure to prevent copying
        // unfortunately Safari has poor support for createImageBitmap - using it with ImageData doesn't work
        const msg = {};
        msg.imageData = originalData;
        msg.pixelSize = adjustedPixelSize;
        msg.cutoff = this.cutoff_;

        if (this.getAttribute("dither-variant")) {
            msg.dither_variant = this.getAttribute("dither-variant");
            console.log({ msg });
        }

        let custom_colors = [
            this.get_rgb_value(this.get_css_variable("--dither-color-1")),
            this.get_rgb_value(this.get_css_variable("--dither-color-2")),
            this.get_rgb_value(this.get_css_variable("--dither-color-3")),
            this.get_rgb_value(this.get_css_variable("--dither-color-4")),
        ];

        this.repaint_id = crypto.randomUUID();
        msg.custom_colors = custom_colors;
        msg.repaint_id = this.repaint_id;

        console.log({ custom_colors });

        this.worker_.postMessage(msg);

        this.force_refresh_ = false;
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: get_rgb_value() */

    get_rgb_value(value) {
        if (!value) return value;

        // CREATE TEMPORARY ELEMENT
        let el = document.createElement("div");

        // APPLY COLOR TO TEMPORARY ELEMENT
        el.style.color = value;

        // APPEND TEMPORARY ELEMENT
        document.body.appendChild(el);

        // RESOLVE COLOR AS RGB() VALUE
        let rgbValue = window.getComputedStyle(el).color;

        // REMOVE TEMPORARY ELEMENT
        document.body.removeChild(el);

        return rgbValue;
    }

    /* #-- <inri-as-dithered-image> :: ASDitheredImage :: get_css_variable() */
    get_css_variable(name) {
        return window
            .getComputedStyle(document.documentElement)
            .getPropertyValue(name);
    }
}
