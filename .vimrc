syntax on
set number
set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab
set colorcolumn=110
highlight ColorColumn ctermbg=darkgray

set wildmode=full
" let g:neocomplcache_enable_at_startup = 1

syn region String start=/"/ end=/"/

highlight Comment ctermfg=darkgrey
highlight String ctermfg=darkgreen
highlight Character ctermfg=green
highlight Type ctermfg=darkyellow
highlight Conditional ctermfg=darkyellow
highlight Repeat ctermfg=darkyellow
highlight Label ctermfg=yellow
highlight Keyword ctermfg=red
highlight Identifier ctermfg=red
highlight Function ctermfg=red
highlight Statement ctermfg=white
