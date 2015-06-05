colorscheme darkblue

" Some Linux distributions set filetype in /etc/vimrc.
" Clear filetype flags before changing runtimepath to force Vim to reload them.
filetype off
filetype plugin off
set runtimepath+=/usr/share/vim/addons/
filetype plugin on
syntax on

" always use spaces and tabstop 4 except where specified
set expandtab
set tabstop=4 softtabstop=4 shiftwidth=4

" show color column in insert mode
highlight ColorColumn ctermbg=4
augroup ColorcolumnOnlyInInsertMode
  autocmd!
  autocmd InsertEnter * setlocal colorcolumn=79
  autocmd InsertLeave * setlocal colorcolumn=0
augroup END

autocmd FileType go setlocal noexpandtab
autocmd FileType make setlocal noexpandtab

set wildmenu
set hlsearch

" Use case insensitive search, except when using capital letters
set ignorecase
set smartcase
