#!/usr/bin/env bash
set -euo pipefail

source_pdf=${1:-/home/dbodden/Downloads/snap_symbols.pdf}
asset_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
source_dir="$asset_dir/.source"

mkdir -p "$source_dir"
inkscape "$source_pdf" --pdf-page=1 --export-plain-svg \
  --export-filename="$source_dir/page1.svg"
inkscape "$source_pdf" --pdf-page=2 --export-plain-svg \
  --export-filename="$source_dir/page2.svg"

# Cairo emits two full-page white rectangles before the actual PDF artwork.
# They survive viewBox cropping and would mask wires or parts behind a symbol.
# Remove only those known page-background paths; white details inside parts stay.
for source_page in "$source_dir/page1.svg" "$source_dir/page2.svg"; do
  perl -0pi -e \
    's/\s*<path\b(?=[^>]*\bid="path(?:24|26)")(?=[^>]*\bstyle="[^"]*fill:#ffffff)[^>]*\/>//gs' \
    "$source_page"
  perl -0pi -e \
    's/\s*<path\b(?=[^>]*\bid="path(?:34|80)")(?=[^>]*\bstyle="[^"]*fill:#fdfeff)[^>]*\/>//gs' \
    "$source_page"
done

while IFS=, read -r file page x y width height name; do
  [[ $file == file ]] && continue
  cp "$source_dir/page${page}.svg" "$asset_dir/$file"
  perl -0pi -e \
    "s/width=\"1056\"/width=\"$width\"/; s/height=\"816\"/height=\"$height\"/; s/viewBox=\"0 0 1056 816\"/viewBox=\"$x $y $width $height\"/" \
    "$asset_dir/$file"
done < "$asset_dir/symbols.csv"

printf 'Generated %s symbol SVGs in %s\n' "$(( $(wc -l < "$asset_dir/symbols.csv") - 1 ))" "$asset_dir"
