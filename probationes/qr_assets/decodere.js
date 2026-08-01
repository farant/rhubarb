ObjC.import('Foundation');
ObjC.import('CoreImage');
ObjC.import('AppKit');

function run(argv) {
    var via = argv[0];
    var url = $.NSURL.fileURLWithPath(via);
    var img = $.CIImage.imageWithContentsOfURL(url);
    if (!img || img.isNil()) { return 'FRACTUM: imago non lecta'; }
    var det = $.CIDetector.detectorOfTypeContextOptions(
        $('CIDetectorTypeQRCode'), $.CIContext.context, $());
    var fs = det.featuresInImage(img);
    var n = parseInt(String(fs.count), 10);
    if (!n) { return 'NIHIL DETECTUM'; }
    var out = [];
    for (var i = 0; i < n; i++) {
        out.push(ObjC.unwrap(fs.objectAtIndex(i).messageString));
    }
    return 'DETECTA ' + n + ' >>> ' + out.join(' | ');
}
