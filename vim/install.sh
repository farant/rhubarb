#!/bin/bash
# install.sh - Installa Sputnik syntax highlighting pro Neovim/Vim
# Idempotens: potest currere pluries sine problemate

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
NVIM_CONFIG="${HOME}/.config/nvim"
VIM_CONFIG="${HOME}/.vim"

# Colores
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo "=== Sputnik Vim/Neovim Syntax Installation ==="
echo ""

# Detectare si Neovim vel Vim
if command -v nvim &> /dev/null; then
    CONFIG_DIR="${NVIM_CONFIG}"
    EDITOR_NAME="Neovim"
elif command -v vim &> /dev/null; then
    CONFIG_DIR="${VIM_CONFIG}"
    EDITOR_NAME="Vim"
else
    echo "Error: Neque Neovim neque Vim inventum est"
    exit 1
fi

echo "Detectum: ${EDITOR_NAME}"
echo "Config: ${CONFIG_DIR}"
echo ""

# Creare directories si non existunt
mkdir -p "${CONFIG_DIR}/ftdetect"
mkdir -p "${CONFIG_DIR}/syntax"

# Functio pro symlink idempotens
link_file() {
    local src="$1"
    local dst="$2"
    local name="$(basename "$src")"

    if [ -L "$dst" ]; then
        local current_target="$(readlink "$dst")"
        if [ "$current_target" = "$src" ]; then
            echo -e "  ${GREEN}✓${NC} ${name} (iam linkatum)"
            return 0
        else
            echo -e "  ${YELLOW}↻${NC} ${name} (relinkando...)"
            rm "$dst"
        fi
    elif [ -f "$dst" ]; then
        echo -e "  ${YELLOW}!${NC} ${name} (backup: ${dst}.bak)"
        mv "$dst" "${dst}.bak"
    fi

    ln -s "$src" "$dst"
    echo -e "  ${GREEN}✓${NC} ${name} (linkatum)"
}

echo "Installando ftdetect..."
link_file "${SCRIPT_DIR}/ftdetect/sputnik.vim" "${CONFIG_DIR}/ftdetect/sputnik.vim"

echo "Installando syntax..."
link_file "${SCRIPT_DIR}/syntax/sputnik.vim" "${CONFIG_DIR}/syntax/sputnik.vim"

echo ""
echo -e "${GREEN}=== Installatio Completa ===${NC}"
echo ""
echo "Aperi .sputnik file et syntax highlighting operabit!"
