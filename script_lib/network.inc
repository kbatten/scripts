# get the default network interface
function network_get_default_interface() {
    ip route | awk '$1 == "default" {print $5}'
}

# get the default network address
function network_get_default_address() {
    ip route | awk '$1 == "default" {print $9}'
}
