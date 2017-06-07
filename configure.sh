#!/bin/bash

plugins=(
chen3feng/toft
chen3feng/thirdparty
chen3feng/typhoon-blade
)

github_ssh='git@github.com:'
github_https='https://github.com/'

function add_submodule() {
    local plugins=$@;
    for plugin in ${plugins[@]}; do
        plugin_name=`echo ${plugin}|awk -F'/' '{print $2}'`
        if [ -e .vim/bundle/${plugin_name} ]; then
            echo "${plugin_name} already exists"
        else
            git submodule add ${github_https}${plugin}.git ${plugin_name}
        fi
    done
}

function checkout_submodule() {
    git submodule update --init --recursive
    git checkout
}

function usage()
{
    cat << HELP_END
./configure.sh [
    add_sub         add submodule
    co_sub          checkout submodules
HELP_END
}

LOCAL=`pwd`
#set -x

case $1 in
    add_sub)
        add_submodule ${plugins[@]}
    ;;
    co_sub)
        checkout_submodule $plugins
    ;;
    --help|-h|*)
        usage
    ;;
esac
