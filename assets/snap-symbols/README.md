# Snap Circuits symbol assets

These SVGs are lossless crops of the symbols in
`/home/dbodden/Downloads/snap_symbols.pdf`. They preserve the original vector
art, text, colors, and any embedded bitmap details. The PDF's white page
background is removed during generation, so the area around every part is
transparent and parts can overlap snap wires without masking them.

The filenames use the printed part ID where one exists. Jumper wires and the
fan use descriptive names. `symbols.csv` records the source page and crop box
for every asset.

To regenerate the assets after replacing the source PDF:

```sh
./generate.sh /path/to/snap_symbols.pdf
```
