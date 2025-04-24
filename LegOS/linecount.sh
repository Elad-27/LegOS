#!/bin/bash
shopt -s globstar

c_lines=$(find . -type f -name "**.c" -exec cat {} + | wc -l)
h_lines=$(find . -type f -name "**.h" -exec cat {} + | wc -l)
s_lines=$(find . -type f -name "**.s" -exec cat {} + | wc -l)
asm_lines=$(find . -type f -name "**.asm" -exec cat {} + | wc -l)

echo "C lines: $c_lines"
echo "Header lines: $h_lines"
echo "Assembly(AT&T) lines: $s_lines"
echo "Assembly(NASM) lines: $asm_lines"
echo "-----------------------------------------"

total=$((c_lines + h_lines + s_lines + asm_lines))
echo "Total: $total"
