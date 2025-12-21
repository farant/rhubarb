#!/usr/bin/env bash

# query-book.sh - Query LLM for book metadata
# Usage:
#   ./query-book.sh 10048        # Process specific book number
#   ./query-book.sh next         # Process next unprocessed book
#   ./query-book.sh next 10      # Process next 10 unprocessed books

INDEX="gutenberg_index.stml"
RAW_DIR="gutenberg-data/raw"
PROCESSED="gutenberg-data/processed.txt"

mkdir -p "$RAW_DIR"
touch "$PROCESSED"

# Find file path for a book number from the index
find_filum() {
    local numerus="$1"
    # Get the liber block for this numerus, find first filum not in old/
    grep -A 30 "numerus=\"$numerus\"" "$INDEX" | \
        grep -o 'via="[^"]*"' | \
        cut -d'"' -f2 | \
        grep -v '/old/' | \
        head -1
}

# Find next unprocessed book number
find_next() {
    # Extract all book numbers from index
    grep -o 'numerus="[0-9]*"' "$INDEX" | \
        cut -d'"' -f2 | \
        while read num; do
            if ! grep -q "^${num}$" "$PROCESSED"; then
                echo "$num"
                return 0
            fi
        done
}

# Process a single book
process_book() {
    local numerus="$1"
    local filum

    # Check if already done
    if grep -q "^${numerus}$" "$PROCESSED"; then
        echo "Already processed: $numerus"
        return 0
    fi

    # Find file path
    filum=$(find_filum "$numerus")
    if [ -z "$filum" ]; then
        echo "Error: No file found for book $numerus"
        return 1
    fi

    if [ ! -f "$filum" ]; then
        echo "Error: File not found: $filum"
        return 1
    fi

    echo "Processing: $numerus ($filum)"

    # Run LLM query
    head -50 "$filum" | claude -p "this is a text from project gutenberg. i am trying to figure out the title and the author. i also want to figure out best effort year it was originally published (not necessarily year published to gutenberg). so it would be a question of you recognizing the work and yourself knowing the year. i know multiple years can be valid for editions, translations, etc. the use case will be a sort of historical timeline of works so i think the goal is whatever year seems most historically accurate and educational for the text if there is ambiguity. for example if it is just a translation of a work of st augustine without commentary you might choose the year he wrote it, but if it is a work you know has substantial commentary you might choose the year the annotated work was published. so with that can you emit a structure format with 'Title', 'Author' and 'Year' fields? if it is clear there are multiple authors you can do various or comma separated names. i would also love to have a 'Tags' field that can include an array of items, one would be 'fiction', 'non-fiction', 'historical-document', etc. another would be 'appropriate-for-children'. another could be 'catholic' if the work is written by a catholic author and would be considered likely to have a nihil obstat or imprimatur (ie not just random fiction by an author who may have been catholic). i would also like a 'Summary' that explains, based on your knoweldge, a little background on the work, what it's about, etc. Let's also include a 'Notes' field where you can optionally add any notes if you want about context for the year you chose, etc. Can you have the structured output with fields be a toml format inside of an <result> tag?

Please output TOML format inside <result> tags. Use flat keys, not table sections.

Example format:
<result>
Title = \"Book Title\"
Author = \"Author Name\"
Year = 1850

Tags = [\"non-fiction\", \"appropriate-for-children\"]

Summary = \"\"\"
Multi-line summary here.
\"\"\"

Notes = \"\"\"
Optional notes here.
\"\"\"
</result>" > "$RAW_DIR/${numerus}.txt"


    # Mark as processed
    echo "$numerus" >> "$PROCESSED"

    echo "Saved: $RAW_DIR/${numerus}.txt"
    echo ""
}

# Main
if [ -z "$1" ]; then
    echo "Usage:"
    echo "  $0 <numerus>     Process specific book number"
    echo "  $0 next          Process next unprocessed book"
    echo "  $0 next <N>      Process next N unprocessed books"
    exit 1
fi

if [ "$1" = "next" ]; then
    count="${2:-1}"
    echo "Processing next $count book(s)..."
    echo ""

    for ((i=0; i<count; i++)); do
        numerus=$(find_next)
        if [ -z "$numerus" ]; then
            echo "No more unprocessed books!"
            exit 0
        fi
        process_book "$numerus"
    done
else
    process_book "$1"
fi
