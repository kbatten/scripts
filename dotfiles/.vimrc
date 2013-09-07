colorscheme darkblue

" Some Linux distributions set filetype in /etc/vimrc.
" Clear filetype flags before changing runtimepath to force Vim to reload them.
filetype off
filetype plugin off
set runtimepath+=/usr/share/vim/addons/
filetype plugin on
syntax on

autocmd FileType go set tabstop=4 colorcolumn=79
autocmd FileType python set tabstop=4 expandtab colorcolumn=79

set wildmenu
set hlsearch

" Use case insensitive search, except when using capital letters
set ignorecase
set smartcase
