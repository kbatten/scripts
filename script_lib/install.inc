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

function install_rust() {
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source "$HOME/.cargo/env"
    rustup update
}

function download_github() {
    local url="${1}"
    local downloaddir="${TANQ_DOWNLOAD_DIR}"

    local dirname="$(rootname $url)"

    mkdir -p "${downloaddir}" &> /dev/null
    pushd "$downloaddir"
    if [[ -e "$dirname" ]] ; then
        cd "$dirname"
        git pull
    else
        git clone "$url"
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
    sudo apt-get install -y winetricks
    wine winecfg -v win10
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

function install_wine_c() {
    sudo apt-get install -y winetricks
    #sudo winetricks --self-update -y

    ## cmake
    #download https://github.com/Kitware/CMake/releases/download/v4.2.1/cmake-4.2.1-windows-x86_64.msi
    #install_msi cmake-4.2.1-windows-x86_64.msi

    install_wine
    wineboot && sudo ls &> /dev/null
    xvfb-run winetricks -q corefonts vcrun2015
    xvfb-run winetricks -q msxml6
    xvfb-run winetricks -q arial d3dcompiler_47 && sudo ls &> /dev/null
    xvfb-run winetricks -q dotnet35 && sudo ls &> /dev/null
    xvfb-run winetricks -q dotnet46 && sudo ls &> /dev/null
    xvfb-run winetricks -q dotnet472 && sudo ls &> /dev/null
    xxvfb-run winetricks -q dotnet48 && sudo ls &> /dev/null
    xvfb-run winetricks -q dotnet8 --silent && sudo ls &> /dev/null
    xvfb-run winetricks -q vstools2019 && sudo ls &> /dev/null
    wine winecfg -v win10

    echo "*** PAUSED ***"
    read

    install_wine_c_vs_BuildTools
    install_wine_c_visualcppbuildtools_full
    install_wine_c_buildtools
    install_wine_c_VisualStudioSetup

    find ~ -iname cmake.exe ; find ~ -iname nmake.exe
    read
}

function install_wine_c_VisualStudioSetup() {
    download "https://c2rsetup.officeapps.live.com/c2r/downloadVS.aspx?sku=community&channel=stable&version=VS18&source=VSLandingPage&cid=2500:5b395c81dcbf47f8b547f64c9a340170" VisualStudioSetup.exe
    #downlaod "https://c2rsetup.officeapps.live.com/c2r/downloadVS.aspx?sku=community&channel=stable&version=VS18&source=VSLandingPage&cid=2500:067eae3e75eb40fc8159e6389951b9c4" VisualStudioSetup.exe

    #install_exe VisualStudioSetup.exe --quiet
    echo "export WINEARCH=$WINEARCH; export WINEPREFIX=$WINEPREFIX ; wine VisualStudioSetup.exe &> VisualStudioSetup.log" > "${TANQ_DOWNLOAD_DIR}"/VisualStudioSetup.sh
    echo "bash ./VisualStudioSetup.sh"
    read
}

function install_wine_c_vs_BuildTools() {
    #download "https://download.visualstudio.microsoft.com/download/pr/6efb3484-905b-485c-8b5f-9d3a5f39e731/07908cd6d91e75b8ea4339d8f2cfa6e8d8bb4fd706af7b918ae391cd6fc2a066/vs_BuildTools.exe"
    download "https://download.visualstudio.microsoft.com/download/pr/a80deb24-6a28-4d30-b99f-13b6e89c9727/5607a31b3fdb3436ba6d8ad26c1c67e4f7bdba79884daf45d59dc6be23801909/vs_BuildTools.exe"

    #install_exe vs_BuildTools.exe --quiet
    echo "export WINEARCH=$WINEARCH; export WINEPREFIX=$WINEPREFIX ; wine vs_BuildTools.exe &> vs_BuildTools.log" > "${TANQ_DOWNLOAD_DIR}"/vs_BuildTools.sh
    echo "bash ./vs_BuildTools.sh"
    read
}

function install_wine_c_visualcppbuildtools_full() {
    download https://download.microsoft.com/download/5/F/7/5F7ACAEB-8363-451F-9425-68A90F98B238/visualcppbuildtools_full.exe

    #install_exe "visualcppbuildtools_full.exe" "/Full /Q /NoRefresh /NoRestart"
    rm -f visualcppbuildtools_full.log &> /dev/null || true
    echo "export WINEARCH=$WINEARCH; export WINEPREFIX=$WINEPREFIX ; wine visualcppbuildtools_full.exe /Full /Passive /Log visualcppbuildtools_full.log" > "${TANQ_DOWNLOAD_DIR}"/visualcppbuildtools_full.sh
    echo "bash ./visualcppbuildtools_full.sh"
    read
}

function install_wine_c_buildtools() {
    download https://download.microsoft.com/download/e/e/d/eedf18a8-4aed-4ce0-bebe-70a83094fc5a/BuildTools_Full.exe

    echo "export WINEARCH=$WINEARCH; export WINEPREFIX=$WINEPREFIX ; wine BuildTools_Full.exe &> BuildTools_Full.log" > "${TANQ_DOWNLOAD_DIR}"/BuildTools_Full.sh
    echo "bash ./BuildTools_Full.sh"
    read
}

sudo apt-get install -y curl || true
