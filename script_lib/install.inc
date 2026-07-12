TANQ_DOWNLOAD_DIR="${TANQ_DOWNLOAD_DIR:=$HOME/.tanq_downloads/}"

source ./script_lib/std.inc

function download() {
    local url=${1}
    local filename=${2}
    local directory=${3}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    # get the actual download url from mediafire, very fragile
    if echo "$url" | grep mediafire && echo "$url" | grep "file$" ; then
        download_mediafire $1 $2 $3
    fi

    if [[ -z "${filename}" ]] ; then
        filename=$(basename "${url}")
    fi
    if [[ -z "${directory}" ]] ; then
        directory=$(rootname "${filename}")
    fi

    echo " ${filename}"

    # download and extract if needed
    mkdir -p "${downloaddir}" &> /dev/null
    pushd "${downloaddir}" &> /dev/null
    if [[ ! -e "${directory}" ]] ; then
        if [[ ! -e "${filename}" ]] ; then
            echo "  downloading..."
            curl -L -o "${filename}" "${url}" &> /dev/null
        fi
        extract "${filename}" "${directory}"
    fi

    popd &> /dev/null
}

function download_mediafire() {
    url=$(curl $1 | grep 'id="downloadButton"' | sed 's/.*href="//g' | sed 's/".*//g')
    download "$url" $2 $3
}

function install_msi() {
    source ./script_lib/graphics.inc

    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    xvfb-run wine msiexec.exe /q /i ${filepath}
}

function install_exe() {
    source ./script_lib/graphics.inc

    local filename=${1}
    local params="${2}"
    local directory=${3}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    xvfb-run wine ${filepath} ${params}
}

function install_go() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local dirname="$(rootname ${filename})"
    local dirpath="${downloaddir}${directory}/${dirname}"
    local destpath=${HOME}/opt/

    if [[ ! -e "${destpath}/${dirname}" ]]; then
        cp -r $dirpath ${destpath}
    fi
}

function install_deb() {
    local filename=${1}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${filename}"

    local packagename="$(dpkg --info ${filepath} | grep Package: | awk '{print $2}')"
    if ! dpkg -s "${packagename}" &> /dev/null ; then
        echo "  installing..."
        $SUDO dpkg -i "$filepath"
    fi
}

function install_ytdlp() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local dirname="$(rootname ${filename})"
    local dirpath="${downloaddir}${directory}/${dirname}"
    local destpath=${HOME}/opt/

    if [[ ! -e "${destpath}/${dirname}" ]]; then
        cp -r $dirpath ${destpath}
    fi
}

function install_rust() {
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source "$HOME/.cargo/env"
    rustup update
}

function download_github() {
    local url="${1}"
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local dirname="$(rootname $url)"

    pushd . &> /dev/null

    mkdir -p "${downloaddir}" &> /dev/null
    pushd "$downloaddir"
    if [[ -e "$dirname" ]] ; then
        cd "$dirname"
        git reset HEAD --hard
        git pull
    else
        git clone "$url"
    fi
    popd &> /dev/null
}

function run() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    #xvfb-run wine ${filepath}
    wine ${filepath}
}

function run() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    #xvfb-run wine ${filepath}
    wine ${filepath}
}

function run_bash() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    bash ${filepath}
}

function run_sudo_bash() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    $SUDO bash ${filepath}
}

function apply_patch() {
    local patchname="$1"
    local patchdest="$2"
    local patch="$(scriptpath)/${patchname}"
    pushd "${patchdest}"
    git apply "${patch}"
    popd
}

function install_wine_latest()
{
    $SUDO dpkg --add-architecture i386
    $SUDO apt-get remove -y wine
    $SUDO apt-get -y autoremove

    download https://dl.winehq.org/wine-builds/winehq.key
    $SUDO mkdir -pm755 /etc/apt/keyrings
    $SUDO rm -f /etc/apt/keyrings/winehq-archive.key
    $SUDO gpg -o /etc/apt/keyrings/winehq-archive.key --dearmor "${TANQ_DOWNLOAD_DIR}/winehq.key"

    local debian_version=$(get_debian_version)
    download "https://dl.winehq.org/wine-builds/debian/dists/${debian_version}/winehq-${debian_version}.sources"
    $SUDO cp "${TANQ_DOWNLOAD_DIR}/winehq-${debian_version}.sources" /etc/apt/sources.list.d/

    $SUDO apt update -y
    $SUDO apt install winehq-stable --install-recommends -y
    $SUDO apt-get install -y winbind
    $SUDO apt-get install -y winetricks
}

function install_wine()
{
    $SUDO dpkg --add-architecture i386
    $SUDO apt-get install -y wine
    $SUDO apt-get install -y wine64
    $SUDO apt-get update
    $SUDO apt-get install -y wine32
    $SUDO apt-get install -y winbind
    $SUDO apt-get install -y winetricks
}

function install_wine_java8() {
    download https://corretto.aws/downloads/latest/amazon-corretto-8-x64-windows-jdk.msi
    install_msi amazon-corretto-8-x64-windows-jdk.msi
}

function install_wine_gecko() {
    local gecko_version=2.47.4

    download http://dl.winehq.org/wine/wine-gecko/${gecko_version}/wine-gecko-${gecko_version}-x86_64.msi
    install_msi wine-gecko-${GECKO_VERSION}-x86_64.msi

    download http://dl.winehq.org/wine/wine-gecko/${gecko_version}/wine-gecko-${gecko_version}-x86.msi
    install_msi wine-gecko-${GECKO_VERSION}-x86.msi
}

function get_debian_version() {
    if [[ -e /etc/apt/sources.list ]] ; then
        cat /etc/apt/sources.list | grep "^deb" | head -n1 | awk '{print $3}'
    elif [[ -e /etc/apt/sources.list.d/debian.sources ]] ; then
        cat /etc/apt/sources.list.d/debian.sources  | grep Suites | head -n1 | cut -f 2 -d " "
    else
        echo "unknown"
    fi
}

function setup_install() {
    $SUDO apt-get install -y curl || true
}

setup_install
