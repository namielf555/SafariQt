#!/usr/bin/env bash
# Build a C++ project and package it as an AppImage.
#
# Usage:   ./build-appimage.sh <app-name> [source-dir]
# Env:     QT=1                  bundle Qt (needs qmake6/qmake on PATH)
#          QML_SOURCES_PATHS=dir  QML dir for Qt deps scan (with QT=1)
#          ICON=path.png          app icon (placeholder used if unset)
#          TERMINAL=true          mark as terminal app (CLI tools)
#          CXXFLAGS_EXTRA="..."   extra flags for the plain g++ fallback
#
# Source layout:
#   - CMakeLists.txt present -> cmake build + install (must install bin/<app-name>)
#   - otherwise              -> g++ on all *.cpp under source-dir

set -euo pipefail

NAME="${1:?usage: $0 <app-name> [source-dir]}"
SRC="$(realpath "${2:-.}")"
OUT="$PWD/appimage-out"
BUILD="$OUT/build"
APPDIR="$OUT/AppDir"
TOOLS="$OUT/tools"
ARCH="${ARCH:-$(uname -m)}"
TERMINAL="${TERMINAL:-false}"

rm -rf "$BUILD" "$APPDIR"
mkdir -p "$BUILD" "$APPDIR/usr/bin" "$TOOLS"

# ---------- build ----------
if [[ -f "$SRC/CMakeLists.txt" ]]; then
  cmake -S "$SRC" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build "$BUILD" -j"$(nproc)"
  DESTDIR="$APPDIR" cmake --install "$BUILD"
  if [[ ! -x "$APPDIR/usr/bin/$NAME" ]]; then
    # no install() rule: grab the built binary directly
    BIN="$(find "$BUILD" -type f -name "$NAME" -perm -u+x -not -path '*/CMakeFiles/*' | head -n1)"
    [[ -n "$BIN" ]] || { echo "no executable named '$NAME' in $BUILD; check your CMake target name"; find "$BUILD" -maxdepth 2 -type f -perm -u+x -not -path '*/CMakeFiles/*'; exit 1; }
    install -Dm755 "$BIN" "$APPDIR/usr/bin/$NAME"
  fi
else
  mapfile -t FILES < <(find "$SRC" -name '*.cpp' -not -path '*/build/*' -not -path "$OUT/*")
  [[ ${#FILES[@]} -gt 0 ]] || { echo "no .cpp files in $SRC"; exit 1; }
  g++ -std=c++20 -O2 ${CXXFLAGS_EXTRA:-} "${FILES[@]}" -o "$APPDIR/usr/bin/$NAME"
fi
[[ -x "$APPDIR/usr/bin/$NAME" ]] || { echo "usr/bin/$NAME missing after build/install"; exit 1; }

# ---------- desktop file + icon ----------
cat > "$OUT/$NAME.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=$NAME
Exec=$NAME
Icon=$NAME
Categories=Utility;
Terminal=$TERMINAL
EOF

if [[ -n "${ICON:-}" ]]; then
  cp "$ICON" "$OUT/$NAME.png"
else
  # 256x256 placeholder PNG
  echo 'iVBORw0KGgoAAAANSUhEUgAAAQAAAAEACAYAAABccqhmAAAEcklEQVR42u3Ux3JUMRRF0Tcn55wxnwUm55wzfLm4uIoBQ7Cl022tU7Xmsl/fvSxmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZp2252drMIqLc+wgCqO2t/7BsC5crMMHIfjvw/9R/0DYJVy0wwchcPggBI4fEXD4fgTg+EEE5tm+760Bf3P8IAKOH0TA8YMIOH4QAccPIiAAIADrcvzf6o8AtsXxgwis1/bXo4Gd4fhBBAQABMDxgwisZAC+1kOBLhw/iIAAgAA4fhCBVdiBehQwhgCAAKzI8X+pRwFDCQAIgACAAAgACIAAgAA4fhABAQAB6LiDn1sDMgQABEAAQAAEAARAAEAABAAEoHMAPtVDgAgBAAHI7VA9AsgQABAAAQABEAAQAAEAARgTgI/1ECBCAEAABAAEQABAAEbucD0CyBAAEIBgAD7UQ4AIAQABEAAQAAEAARAAEAABAAHouyPvWwMyBAAEQABAAAQABEAAQAAEAARAAEAAegXgXT0EiBAAEIDcjtYjgAwBAAEQABAAAQABGBqAt/UQIEIAQAAEAARAAEAARu5YPQLIEAAQAAEAAUgE4E09BIgQABAAAQABEAAQAAEAARAAEIC+O/66NSBDAEAABAAEQABAAAQABEAAQAA6B+BVPQSIEAAQgNxO1COADAEAARAAEAABAAEQABCAMQF4WQ8BIgQABEAAQAAEAARg5E7WI4AMAQABCAbgRT0EiBAAEAABAAEQABAAAQABEAAQgL47VY8AMvIBeF4PASIEAARAAEAABAAEQABAAAQABKBzAJ7VQ4AIAQAByO10PQLIEAAQAAEAARAAEIChAXhaDwEiBAAEQABAAAQABEAAQACG7Ew9AsgQABCAYACe1EOACAEAARAAEAABAAEQABAAAQAB6Luzj1sDMgQABEAAQAAEAARAAEAABAAEQABAAHoF4FE9BIgQABCA3M7VI4AMAQABEAAQAAEAARgagIf1ECBCAEAABAAEQABAAEbufD0CyBAAEIBgAB7UQ4AIAQABEAAQAAEAARAAEAABAAEQABCATrtwvzUgQwBAAAQABEAAQAAEAARAAEAAOgfgXj0EiBAAEIDcLtYjgAwBAAEQABAAAQABEAAQgDEBuFsPASIEAARAAEAABAAEYOQu1SOADAEAAQgG4E49BIgQABAAAQABEAAQAAEAARAAEIC+u3y7NSBjWYX5EDDp8QsACIAPAgIACAAwTwBEACY+/q0A3KqHAUMIAAjAau1KPQzoa1nV+TgwcQBEACY+fgGAyQOwFYGb9VhgRy3rMh8LJg6ACMDExy8AMHkAfu9qPRzYnmWd5wPCpMcvADB5ALYisFl/DPBPlt00HxQmPX4RgMmPXwRg8uMXAZj8+EUAJj/+P9u40RrMbpl9fgQ4fBHwo8DxC4EfBw5fCPxYcPjmx4PDt2Xjev1DYU24WDHA0duoXasPAKO4ODMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzPrtV/JrSreqI8XlgAAAABJRU5ErkJggg==' \
    | base64 -d > "$OUT/$NAME.png"
fi

# ---------- tools ----------
GH="https://github.com/linuxdeploy"
fetch() { [[ -x "$TOOLS/$2" ]] || { curl -fL "$1" -o "$TOOLS/$2" && chmod +x "$TOOLS/$2"; }; }

fetch "$GH/linuxdeploy/releases/download/continuous/linuxdeploy-$ARCH.AppImage" "linuxdeploy-$ARCH.AppImage"

PLUGIN_ARGS=()
if [[ "${QT:-0}" == "1" ]]; then
  fetch "$GH/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-$ARCH.AppImage" "linuxdeploy-plugin-qt-$ARCH.AppImage"
  REAL_QMAKE="${QMAKE:-$(command -v qmake6 || command -v qmake)}"

  # linuxdeploy-plugin-qt deploys EVERY imageformats plugin and dies if one has
  # unresolved deps (kimg_jxr -> libjpegxr.so). Point it at a symlink farm of the
  # Qt plugins dir with the kimg_* (KImageFormats) plugins removed.
  REAL_PLUGINS="$("$REAL_QMAKE" -query QT_INSTALL_PLUGINS)"
  FAKE_PLUGINS="$OUT/qt-plugins"
  rm -rf "$FAKE_PLUGINS"; mkdir -p "$FAKE_PLUGINS"
  cp -as "$REAL_PLUGINS"/. "$FAKE_PLUGINS"/
  # drop every plugin with unresolved deps (kimg_jxr -> libjpegxr, position/nmea -> libQt6SerialPort, ...)
  while IFS= read -r p; do
    if ldd "$p" 2>/dev/null | grep -q 'not found'; then
      echo "skipping plugin with missing deps: ${p#$FAKE_PLUGINS/}"
      rm -f "$p"
    fi
  done < <(find "$FAKE_PLUGINS" -name '*.so')

  cat > "$TOOLS/qmake-wrap" <<EOF
#!/usr/bin/env bash
case "\$*" in
  *QT_INSTALL_PLUGINS*) echo "$FAKE_PLUGINS"; exit 0;;
esac
"$REAL_QMAKE" "\$@" | sed "s#^QT_INSTALL_PLUGINS:.*#QT_INSTALL_PLUGINS:$FAKE_PLUGINS#"
exit \${PIPESTATUS[0]}
EOF
  chmod +x "$TOOLS/qmake-wrap"
  export QMAKE="$TOOLS/qmake-wrap"
  export PATH="$TOOLS:$PATH"
  PLUGIN_ARGS=(--plugin qt)
fi

# ---------- package ----------
export APPIMAGE_EXTRACT_AND_RUN=1   # no FUSE needed
export NO_STRIP=1                   # bundled strip is too old for .relr.dyn (new glibc/binutils)
export OUTPUT="$OUT/$NAME-$ARCH.AppImage"
export LINUXDEPLOY_OUTPUT_VERSION="${VERSION:-1.0}"

"$TOOLS/linuxdeploy-$ARCH.AppImage" \
  --appdir "$APPDIR" \
  --executable "$APPDIR/usr/bin/$NAME" \
  --desktop-file "$OUT/$NAME.desktop" \
  --icon-file "$OUT/$NAME.png" \
  "${PLUGIN_ARGS[@]}" \
  --output appimage

mv -f ./"$NAME"*.AppImage "$OUT/" 2>/dev/null || true
echo
echo "Done: $(ls "$OUT"/*.AppImage)"