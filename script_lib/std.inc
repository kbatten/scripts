function extname() {
    local filename="$(basename "${1}")"
    if [[ "$(echo "${filename}" | grep "\..*\." | sed 's/^.*\.\([^.]*\)\.[^.]*$/\1/')" == "tar" ]] ; then
        echo -n "tar."
    fi
    echo "${filename##*.}"
}

function rootname() {
    local filename="$(basename "${1}")"
    local extension="$(extname "${filename}")"
    echo "${filename%.${extension}}"
}

function scriptpath() {
    local relativepath="$(dirname "$0")"
    echo "$(cd "$relativepath" ; pwd)"
}

function extract() {
    local filename=${1}
    local directory=${2}
    local extension=$(extname "${filename}")

    case "${extension}" in
        "tar.gz" | "tgz" | "tar.bz2" | "tar.xz")
            echo "  extracting..."
            tar xf "${filename}" &> /dev/null
            local directory_orig=$(basename $(tar tf "${filename}" | head -n1))
            if [[ "${directory_orig}" != "${directory}" ]]; then
                mv "${directory_orig}" "${directory}" &> /dev/null
            fi
            ;;
        "zip")
            echo "  extracting..."
            unzip -d "${directory}" "${filename}"
            ;;
        "7z")
            echo "  extracting..."
            7z x -o"${directory}" "${filename}"
            ;;
    esac
}
