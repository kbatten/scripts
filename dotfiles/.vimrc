colorscheme darkblue

" Disable visual insert
set mouse-=a

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
autocmd FileType html setlocal tabstop=2 softtabstop=2
autocmd FileType javascript setlocal tabstop=2 softtabstop=2

set wildmenu
set hlsearch

" don't automatically continue comments
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" Use case insensitive search, except when using capital letters
set ignorecase
set smartcase
