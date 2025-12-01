# Sputnik Vim/Neovim Syntax Highlighting

## Installation

### Option 1: Symlink (recommended for development)

```bash
# Neovim
ln -s /path/to/rhubarb/vim/ftdetect ~/.config/nvim/ftdetect
ln -s /path/to/rhubarb/vim/syntax ~/.config/nvim/syntax

# Vim
ln -s /path/to/rhubarb/vim/ftdetect ~/.vim/ftdetect
ln -s /path/to/rhubarb/vim/syntax ~/.vim/syntax
```

### Option 2: Copy files

```bash
# Neovim
cp -r ftdetect syntax ~/.config/nvim/

# Vim
cp -r ftdetect syntax ~/.vim/
```

### Option 3: Add to runtimepath

Add to your `init.vim` or `init.lua`:

```vim
" init.vim
set runtimepath+=/path/to/rhubarb/vim
```

```lua
-- init.lua
vim.opt.runtimepath:append("/path/to/rhubarb/vim")
```

## Features

- Keywords: `sit`, `constans`, `functio`, `si`, `alioquin`, `dum`, `per`, `redde`, `frange`, `perge`
- Literals: `verum`, `falsum`, `nihil`
- Strings: `"..."`, `'...'`, `` `...` `` with `${...}` interpolation
- Numbers: integers, floats, currency (`9.99$`)
- Comments: `//` line comments, `/* */` block comments
- Operators: arithmetic, comparison, logical, arrow (`=>`)
- Built-ins: `print`, `len`, `typeof`, `REPO`
- Methods: array, string, entity methods
- Tags: `#tagname`
