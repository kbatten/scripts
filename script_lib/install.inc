function download() {
    local url=${1}
    local filename=${2}
    local directory=${3}
    local downloaddir="./downloads/"

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
    local downloaddir="./downloads/"

    local filepath="${downloaddir}${directory}/${filename}"

    xvfb-run wine msiexec.exe /q /i ${filepath}
}

function install_go() {
    local filename=${1}
    local directory=${2}
    local downloaddir="./downloads/"

    local dirname="$(rootname ${filename})"
    local dirpath="${downloaddir}${directory}/${dirname}"
    local destpath=${HOME}/opt/

    if [[ ! -e "${destpath}/${dirname}" ]]; then
        cp -r $dirpath ${destpath}
    fi
}

function install_deb() {
    local filename=${1}
    local downloaddir="./downloads/"

    local filepath="${downloaddir}${filename}"

    local packagename="$(dpkg --info ${filepath} | grep Package: | awk '{print $2}')"
    if ! dpkg -s "${packagename}" &> /dev/null ; then
        echo "  installing..."
        sudo dpkg -i "$filepath"
    fi
}

function run() {
    local filename=${1}
    local directory=${2}
    local downloaddir="./downloads/"

    local filepath="${downloaddir}${directory}/${filename}"

    #xvfb-run wine ${filepath}
    wine ${filepath}
}

sudo apt-get install -y curl
