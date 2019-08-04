# modify .bashrc to source a script
function bashrc_source_script() {
  local tag=$1
  shift
  grep "^source.*# $tag$" "$HOME/.bashrc" &> /dev/null || echo "source $@  # $tag" >> "$HOME/.bashrc"
}

# link basic dotfiles
function link_dotfiles() {
  ln -s -f "$PWD/dotfiles/$@" "$HOME/$@"
}

# link bash completions
function link_bash_completion() {
  [ ! -d "$HOME/.bash_completion.d" ] && ln -s -f "$PWD/dotfiles/.bash_completion.d" "$HOME/.bash_completion.d"
  bashrc_source_script "link_bash_completion" "$HOME/.bash_completion.d/*"
}

# link dev bins
function link_bin() {
  local source=$1
  local dest=${2-$1}
  ln -s -f "$PWD/bin/${source}" "$HOME/bin/${dest}"
}

# link external bins
function link_bin_external() {
  local source=$(readlink -f $1)
  local dest=$2
  ln -s -f "${source}" "$HOME/bin/${dest}"
}
