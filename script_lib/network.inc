# get the default network interface
function network_get_default_interface() {
    ip route | awk '$1 == "default" {print $5}'
}

# get the default network address
function network_get_default_address() {
    ip route | awk '$1 == "default" {print $9}'
}

# modify /etc/sysctl.conf to source a script
function sysctl_set() {
  local key=$1
  local value=$2
  if sudo grep "^$key\W*=.*" "/etc/sysctl.conf" &> /dev/null ; then
    sudo sed -i 's/^'"$key"'\W*=.*$/'"$key"'='"$value"'/g' "/etc/sysctl.conf"
  else
    echo "$key=$value" | sudo tee -a "/etc/sysctl.conf" > /dev/null
  fi
}

