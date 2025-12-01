" Vim syntax file
" Language: Sputnik
" Maintainer: Fran Arant
" Latest Revision: 2025

if exists("b:current_syntax")
  finish
endif

" Keywords - Declarationes
syn keyword sputnikDeclaration sit constans functio
syn keyword sputnikDeclaration entitas

" Keywords - Control Flow
syn keyword sputnikConditional si alioquin
syn keyword sputnikRepeat dum per
syn keyword sputnikStatement redde frange perge

" Keywords - Boolean et Nihil
syn keyword sputnikBoolean verum falsum
syn keyword sputnikNull nihil

" Keywords - Built-in Functions
syn keyword sputnikBuiltin print len typeof

" Keywords - Global Objects
syn keyword sputnikGlobal REPO

" Comments
syn match sputnikLineComment "\/\/.*$"
syn region sputnikBlockComment start="/\*" end="\*/" contains=sputnikTodo
syn keyword sputnikTodo contained TODO FIXME XXX NOTE

" Strings - Double quoted
syn region sputnikString start=/"/ skip=/\\"/ end=/"/ contains=sputnikEscape

" Strings - Single quoted
syn region sputnikString start=/'/ skip=/\\'/ end=/'/ contains=sputnikEscape

" Template strings with interpolation
syn region sputnikTemplate start=/`/ skip=/\\`/ end=/`/ contains=sputnikInterpolation,sputnikEscape

" Interpolation ${...}
syn region sputnikInterpolation matchgroup=sputnikInterpolationDelim start=/\${/ end=/}/ contained contains=TOP

" Escape sequences
syn match sputnikEscape /\\[nrtv\\'"`$\\]/ contained

" Numbers - Integer
syn match sputnikNumber /\<\d\+\>/

" Numbers - Float
syn match sputnikNumber /\<\d\+\.\d*\>/
syn match sputnikNumber /\<\.\d\+\>/

" Currency (Pecunia) - e.g., 9.99$, 10$
syn match sputnikCurrencySymbol /\$/ contained
syn match sputnikCurrency /\<\d\+\(\.\d*\)\?\$\>/ contains=sputnikCurrencySymbol

" Operators
syn match sputnikOperator /[+\-*/%]/
syn match sputnikOperator /[<>]=\?/
syn match sputnikOperator /[!=]==\?/
syn match sputnikOperator /&&\|||/
syn match sputnikOperator /!/
syn match sputnikOperator /=>/
syn match sputnikOperator /++\|--/
syn match sputnikOperator /[+\-*/%]=/
syn match sputnikOperator /=/

" Punctuation
syn match sputnikBracket /[{}\[\]()]/
syn match sputnikDelimiter /[,;:]/
syn match sputnikDot /\./

" Tags/Signa #tag
syn match sputnikTag /#\w\+/

" Method calls (for highlighting common methods)
syn match sputnikMethod /\.\(push\|pop\|shift\|unshift\|indexOf\|includes\|slice\|concat\|join\|length\)\>/
syn match sputnikMethod /\.\(map\|filter\|find\|forEach\)\>/
syn match sputnikMethod /\.\(charAt\|substring\|toUpperCase\|toLowerCase\|split\)\>/
syn match sputnikMethod /\.\(add_relation\|related\|related_all\|has_tag\|add_tag\|remove_tag\)\>/
syn match sputnikMethod /\.\(get\|query\|scaffold\|tagged\|search\)\>/

" Entity properties
syn match sputnikProperty /\.\(id\|genus\|titulus\)\>/

" Highlighting links
hi def link sputnikDeclaration Keyword
hi def link sputnikConditional Conditional
hi def link sputnikRepeat Repeat
hi def link sputnikStatement Statement
hi def link sputnikBoolean Boolean
hi def link sputnikNull Constant
hi def link sputnikBuiltin Function
hi def link sputnikGlobal Constant

hi def link sputnikLineComment Comment
hi def link sputnikBlockComment Comment
hi def link sputnikTodo Todo

hi def link sputnikString String
hi def link sputnikTemplate String
hi def link sputnikInterpolationDelim Special
hi def link sputnikEscape Special

hi def link sputnikNumber Number
hi def link sputnikCurrency Number
hi def link sputnikCurrencySymbol Special

hi def link sputnikOperator Operator
hi def link sputnikBracket Delimiter
hi def link sputnikDelimiter Delimiter
hi def link sputnikDot Delimiter

hi def link sputnikTag Label
hi def link sputnikMethod Function
hi def link sputnikProperty Identifier

let b:current_syntax = "sputnik"
