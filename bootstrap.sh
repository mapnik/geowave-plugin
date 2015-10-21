#!/usr/bin/env bash

#set -eu

function setup_mason() {
    if [[ ! -d ./.mason ]]; then
        git clone --depth 1 https://github.com/mapbox/mason.git ./.mason
    else
        echo "Updating to latest mason"
        (cd ./.mason && git pull)
    fi
    export MASON_DIR=$(pwd)/.mason
    export PATH=$(pwd)/.mason:$PATH
    export CXX=${CXX:-clang++}
    export CC=${CC:-clang}
}

if [[ $(uname -s) == 'Darwin' ]]; then
    FIND_PATTERN="\/Users\/travis\/build\/mapbox\/mason"
else
    FIND_PATTERN="\/home\/travis\/build\/mapbox\/mason"
fi

REPLACE="$(pwd)"
REPLACE=${REPLACE////\\/}

function install() {
    MASON_PLATFORM_ID=$(mason env MASON_PLATFORM_ID)
    if [[ ! -d ./mason_packages/${MASON_PLATFORM_ID}/${1}/${2} ]]; then
        mason install $1 $2
        mason link $1 $2
        if [[ $3 ]]; then
            LA_FILE=$(${MASON_DIR:-~/.mason}/mason prefix $1 $2)/lib/$3.la
            if [[ -f ${LA_FILE} ]]; then
               perl -i -p -e "s/${FIND_PATTERN}/${REPLACE}/g;" ${LA_FILE}
            else
                echo "$LA_FILE not found"
            fi
        fi
    fi
}

function install_mason_deps() {
    install mapnik 3.0.0 &
    install boost 1.57.0 &
    install boost_libsystem 1.57.0 &
    install boost_libthread 1.57.0 &
    install geowave-jace 0.8.7 &
    wait
}

MASON_LINKED_ABS=$(pwd)/mason_packages/.link
MASON_LINKED_REL=./mason_packages/.link
export C_INCLUDE_PATH="${MASON_LINKED_ABS}/include"
export CPLUS_INCLUDE_PATH="${MASON_LINKED_ABS}/include"
export LIBRARY_PATH="${MASON_LINKED_ABS}/lib"

function make_config() {
    if [[ $(uname -s) == 'Darwin' ]]; then
        local PATH_REPLACE="/Users/travis/build/mapbox/mason/mason_packages:./mason_packages"
    else
        local PATH_REPLACE="/home/travis/build/mapbox/mason/mason_packages:./mason_packages"
    fi

    echo "{
   'variables': {
      'boost_home': '${MASON_LINKED_REL}',
      'geowave_home': '${MASON_LINKED_REL}',
      'java_home': '${JAVA_HOME}',
      'mapnik_config': '${MASON_LINKED_REL}/bin/mapnik-config'
   }
}" > ./config.gypi
}

function main() {
    setup_mason
    install_mason_deps
    make_config
    echo "Ready, now run:"
    echo ""
    echo "    make"
}

main
