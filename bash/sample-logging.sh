#!/bin/bash

# Script paths
SCRIPT=$(readlink -f $0)
SCRIPT_PATH=$(dirname $SCRIPT)

RE_INTEGER="^([0-9]+)$"

function usage()
{
    log  "Script for <something>"
    log  ""
    log  "Usage: $0 [Options]"
    log  "================================================================================================="
    log  "         -h|--help        - print usage"
    log  "         -a|--a-flag      - option for something"
    log  ""
}

function script_main()
{
    parse_args "$@"
    log m "option a : ${opt_a}"

    execute_cmd "command name"
    exit 0
}

function parse_args()
{
    if [ $# -eq 0 ] ; then
        usage
        exit 1
    fi
    while [ "$*" ] ; do
    case $1 in
        -a|--a-flag)
            opt_a=$2
            shift
            ;;
        *|-h|--help)
            usage
            exit 1
        esac
        shift
    done
    validate_args
}

function execute_cmd()
{
    log "doing something ..."
    log "$1"
    eval "$1"
    build_result=$(echo $?)
    if [ $build_result -eq 0 ] ; then
        log ok "Success!"
    else
        log err "Failure, code ${build_result}."
        exit 1
    fi
}

function validate_args()
{
    if [[ ! $opt_a =~ $RE_INTEGER ]] ; then
        log wrn "Enter an integer."
        exit 1
    fi
}

# Log function
green_color="\e[1;32m"
yellow_color="\e[1;33m"
red_color="\e[1;31m"
blue_color="\e[1;34m"
magenta_color="\e[1;35m"
reset_color="\e[0m"

function log()
{
    case $1 in
        ok)  echo -e "[${green_color}ok ${reset_color}]" "${@:2}"; ;;
        wrn) echo -e "[${yellow_color}wrn${reset_color}]" "${@:2}"; ;;
        err) echo -e "[${red_color}err${reset_color}]" "${@:2}"; ;;
        m)   echo "${@:2}"; ;;
        *)   echo -e "[${blue_color}log${reset_color}]" "$@"; ;;
    esac
}

script_main "$@"

