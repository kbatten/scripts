TANQ_DOWNLOAD_DIR="${TANQ_DOWNLOAD_DIR:=$HOME/.tanq_downloads/}"

source ./script_lib/std.inc

function download() {
    local url=${1}
    local filename=${2}
    local directory=${3}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

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

function install_msi() {
    local filename=${1}
    local directory=${2}
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local filepath="${downloaddir}${directory}/${filename}"

    xvfb-run wine msiexec.exe /q /i ${filepath}
}

function install_exe() {
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

sudo apt-get install -y curl || true
