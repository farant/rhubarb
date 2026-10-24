# tools/ios/communia.sh - communia scriptorum iOS (source, non exec):
# telephonum per xcrun devicectl (testa-ios T4), frater
# tools/android/communia.sh. Vocans 'apparatus' (exitus 2 cum nuntio)
# ANTE 'source' definit. Status ex --json-output (plutil -extract), non
# ex tabula textus.

_ios_campus () {
    plutil -extract "$2" raw -o - "$1" 2>/dev/null
}

# IOS_TABULA (identificator devicectl), IOS_UDID (ferramenti - in
# ProvisionedDevices profili), IOS_NOMEN, IOS_MODELLUM, IOS_VERSIO
# ponit. IOS_DEVICE (identificator, UDID aut nomen) eligit; aliter
# telephonum UNUM iOS verum (non 'simulated') coniunctum (pairingState
# paired) cum modo auctoris. Xcode 27 (devicectl, 2026-10-24) simulatores
# quoque enumerat et developerModeStatus / marketingName ante nexum non
# refert: modus ABSENS acceptatur (institutio ipsa causam nominat), solum
# valor alius quam 'enabled' recusatur. Nexus (USB aut rete) devicectl
# ipse aperit. Aliter apparatus statum nominans.
ios_tabulam_eligere () {
    local j n i id nomen udid platforma par modus modellum versio apti realitas
    j=$(mktemp -t ios_tabulae) || apparatus "mktemp fractum"
    if ! xcrun devicectl list devices --json-output "$j" > /dev/null 2>&1; then
        rm -f "$j"
        apparatus "xcrun devicectl list devices fractum (Xcode 15+ eget)"
    fi
    n=$(_ios_campus "$j" result.devices) || n=0
    apti=0
    IOS_TABULA=""
    i=0
    while [ "$i" -lt "$n" ]; do
        id=$(_ios_campus "$j" "result.devices.$i.identifier")
        nomen=$(_ios_campus "$j" "result.devices.$i.deviceProperties.name")
        udid=$(_ios_campus "$j" "result.devices.$i.hardwareProperties.udid")
        platforma=$(_ios_campus "$j" "result.devices.$i.hardwareProperties.platform")
        par=$(_ios_campus "$j" "result.devices.$i.connectionProperties.pairingState")
        modus=$(_ios_campus "$j" "result.devices.$i.deviceProperties.developerModeStatus")
        modellum=$(_ios_campus "$j" "result.devices.$i.hardwareProperties.marketingName") \
            || modellum=$(_ios_campus "$j" "result.devices.$i.hardwareProperties.productType")
        realitas=$(_ios_campus "$j" "result.devices.$i.hardwareProperties.reality")
        versio=$(_ios_campus "$j" "result.devices.$i.deviceProperties.osVersionNumber")
        i=$((i + 1))
        [ "$realitas" = simulated ] && continue
        if [ -n "${IOS_DEVICE:-}" ]; then
            [ "$IOS_DEVICE" = "$id" ] || [ "$IOS_DEVICE" = "$udid" ] \
                || [ "$IOS_DEVICE" = "$nomen" ] || continue
            [ "$par" = paired ] || { rm -f "$j"; apparatus "telephonum '$IOS_DEVICE' non coniunctum (pairingState '$par') - Xcode: Window > Devices"; }
            [ -z "$modus" ] || [ "$modus" = enabled ] || { rm -f "$j"; apparatus "telephonum '$IOS_DEVICE': Developer Mode '$modus' - Settings > Privacy & Security > Developer Mode"; }
        else
            [ "$platforma" = iOS ] && [ "$par" = paired ] || continue
            [ -z "$modus" ] || [ "$modus" = enabled ] || continue
        fi
        apti=$((apti + 1))
        IOS_TABULA=$id; IOS_UDID=$udid; IOS_NOMEN=$nomen
        IOS_MODELLUM=$modellum; IOS_VERSIO=$versio
    done
    rm -f "$j"
    if [ -n "${IOS_DEVICE:-}" ]; then
        [ "$apti" -ge 1 ] || apparatus "IOS_DEVICE='$IOS_DEVICE' non inventum (xcrun devicectl list devices)"
    else
        [ "$apti" -ge 1 ] || apparatus "nullum telephonum paratum (coniunctum + Developer Mode) - xcrun devicectl list devices"
        [ "$apti" -eq 1 ] || apparatus "plura telephona parata ($apti) - IOS_DEVICE elige"
    fi
    [ -n "$IOS_UDID" ] || apparatus "UDID telephoni $IOS_TABULA ignotum"
    export IOS_TABULA IOS_UDID IOS_NOMEN IOS_MODELLUM IOS_VERSIO
}

# sera per telephonum (ut Android, recensio B1): cursus plures idem
# telephonum non tangunt; mkdir atomicum + pid; possessor mortuus = sera
# stala, capitur
IOS_SERA_NOSTRA=0
ios_seram_capere () {
    local alius
    IOS_SERA_DIR="$HOME/.rhubarb/ios"
    IOS_SERA="$IOS_SERA_DIR/$IOS_UDID.sera"
    mkdir -p "$IOS_SERA_DIR" || apparatus "directorium serae $IOS_SERA_DIR creari nequit"
    if ! mkdir "$IOS_SERA" 2>/dev/null; then
        alius=$(cat "$IOS_SERA/pid" 2>/dev/null)
        if [ -z "$alius" ] || kill -0 "$alius" 2>/dev/null; then
            apparatus "telephonum $IOS_UDID occupatum (cursus pid ${alius:-ignotus}; sera $IOS_SERA) - exspecta; si nullus cursus vivit: rm -rf $IOS_SERA"
        fi
        { rm -rf "$IOS_SERA" && mkdir "$IOS_SERA" 2>/dev/null; } \
            || apparatus "sera $IOS_SERA capi nequit"
    fi
    echo $$ > "$IOS_SERA/pid"
    IOS_SERA_NOSTRA=1
}

ios_seram_liberare () {
    if [ "$IOS_SERA_NOSTRA" = 1 ]; then
        rm -rf "$IOS_SERA"
        IOS_SERA_NOSTRA=0
    fi
}

# ProvisionedDevices per elementa (plutil -extract; conversio tota in
# JSON data profili - certificata - non fert)
_ios_profilum_continet () {
    local k u
    k=0
    while u=$(_ios_campus "$1" "ProvisionedDevices.$k"); do
        [ "$u" = "$2" ] && return 0
        k=$((k + 1))
    done
    return 1
}

# profilum evolutionis (D7): IOS_PROFILUM (via) aut profilum inter
# ~/Library/MobileDevice/Provisioning Profiles/ cuius
# application-identifier '<turma>.*' est, get-task-allow verum, non
# exspiratum, IOS_UDID continens. Ponit IOS_PROFILUM, IOS_TURMA,
# IOS_PROFILUM_PLIST (decodatum, in directorio $1). Aliter apparatus
# causam nominans (nullum, exspiratum, telephonum deest).
ios_profilum_eligere () {
    local dir p f appid turma exspirat nunc causae candidati
    dir=$1
    nunc=$(date -u +%Y-%m-%dT%H:%M:%SZ)
    causae=""
    if [ -n "${IOS_PROFILUM:-}" ]; then
        candidati=$IOS_PROFILUM
        [ -f "$IOS_PROFILUM" ] || apparatus "IOS_PROFILUM '$IOS_PROFILUM' deest"
    else
        candidati=$(ls "$HOME/Library/MobileDevice/Provisioning Profiles/"*.mobileprovision 2>/dev/null)
        [ -n "$candidati" ] || apparatus "nullum profilum in ~/Library/MobileDevice/Provisioning Profiles/ (Xcode: Settings > Accounts)"
    fi
    while read -r p; do
        [ -n "$p" ] || continue
        f="$dir/profilum.$(basename "$p").plist"
        security cms -D -i "$p" > "$f" 2>/dev/null || { causae="$causae; $(basename "$p"): decodari nequit"; continue; }
        appid=$(_ios_campus "$f" Entitlements.application-identifier)
        turma=$(_ios_campus "$f" TeamIdentifier.0)
        exspirat=$(_ios_campus "$f" ExpirationDate)
        if [ -z "${IOS_PROFILUM:-}" ] && [ "$appid" != "$turma.*" ]; then
            continue    # profilum applicationis alius (e.g. Store)
        fi
        [ "$(_ios_campus "$f" Entitlements.get-task-allow)" = true ] \
            || { causae="$causae; $(basename "$p"): get-task-allow non verum (non evolutionis)"; continue; }
        [[ "$exspirat" > "$nunc" ]] \
            || { causae="$causae; $(basename "$p"): exspiravit $exspirat"; continue; }
        _ios_profilum_continet "$f" "$IOS_UDID" \
            || { causae="$causae; $(basename "$p"): telephonum $IOS_UDID non continet"; continue; }
        IOS_PROFILUM=$p; IOS_TURMA=$turma; IOS_PROFILUM_PLIST=$f
        export IOS_PROFILUM IOS_TURMA IOS_PROFILUM_PLIST
        return 0
    done <<< "$candidati"
    apparatus "nullum profilum evolutionis '<turma>.*' validum pro $IOS_UDID${causae:+ (${causae#; })} - Xcode aut developer.apple.com renovat"
}

# identitas signandi: certificatum profili (DeveloperCertificates) cuius
# SHA-1 in claviario est (security find-identity) - signatura per SHA-1,
# numquam per nomen (nomina similia). Ponit IOS_IDENTITAS (SHA-1) et
# IOS_IDENTITAS_NOMEN. Aliter apparatus.
ios_identitatem_eligere () {
    local identitates k cert sha nomen
    identitates=$(security find-identity -v -p codesigning 2>/dev/null)
    k=0
    while cert=$(_ios_campus "$IOS_PROFILUM_PLIST" "DeveloperCertificates.$k"); do
        sha=$(printf '%s' "$cert" | base64 -D 2>/dev/null | shasum -a 1 | awk '{print toupper($1)}')
        nomen=$(printf '%s\n' "$identitates" | awk -v s="$sha" '$2 == s {sub(/^[^"]*"/, ""); sub(/"$/, ""); print; exit}')
        if [ -n "$nomen" ]; then
            IOS_IDENTITAS=$sha; IOS_IDENTITAS_NOMEN=$nomen
            export IOS_IDENTITAS IOS_IDENTITAS_NOMEN
            return 0
        fi
        k=$((k + 1))
    done
    apparatus "nullum certificatum profili $(basename "$IOS_PROFILUM") in claviario (security find-identity -v -p codesigning)"
}
