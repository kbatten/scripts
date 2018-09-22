# link basic dotfiles
function link_dotfiles() {
  ln -s -f "$PWD/dotfiles/$@" "$HOME/$@"
}

# link dev bins
function link_bin() {
  local source=$1
  local dest=${2-$1}
  ln -s -f "$PWD/bin/${source}" "$HOME/bin/${dest}"
}
