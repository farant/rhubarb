#!/bin/bash
# Generate .docx files from episode beat sheet markdown files
# Usage: ./generate_docs.sh
# Requires: pandoc (brew install pandoc)

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

if ! command -v pandoc &> /dev/null; then
    echo "Error: pandoc not found. Install with: brew install pandoc"
    exit 1
fi

# Generate individual episode docs
for f in 1x*.md; do
    echo "Converting $f..."
    pandoc "$f" -o "${f%.md}.docx"
done

# Generate combined document
echo "Generating combined document..."
pandoc 1x*.md -o novum-fundamentum-season1.docx

echo "Done. Generated $(ls 1x*.docx 2>/dev/null | wc -l | tr -d ' ') episode docs + novum-fundamentum-season1.docx"
