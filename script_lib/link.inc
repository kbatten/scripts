# link basic dotfiles
function link_dotfiles() {
  ln -s -f "$PWD/dotfiles/$@" "$HOME/$@"
}

# link dev bins
function link_bin() {
  ln -s -f "$PWD/bin/$@" "$HOME/bin/$@"
}
