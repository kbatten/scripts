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
        sudo dpkg -i "$filepath"
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

    sudo bash ${filepath}
}

function apply_patch() {
    local patchname="$1"
    local patchdest="$2"
    local patch="$(scriptpath)/${patchname}"
    pushd "${patchdest}"
    git apply "${patch}"
    popd
}

function install_wine()
{
    sudo dpkg --add-architecture i386
    sudo apt-get install -y wine
    sudo apt-get install -y wine64
    sudo apt-get update
    sudo apt-get install -y wine32
    sudo apt-get install -y winbind
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

sudo apt-get install -y curl || true
