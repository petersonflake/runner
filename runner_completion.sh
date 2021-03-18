__runner()
{
    local cur prev longopts shortopts fileopts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    longopts="--help --verbose --quiet --show --file --all --exec --dry-run"
    shortopts="-h -q -f -v -s -e -a -d"
    ids="$(runner -c)"
    fileopts="--file"

    if [[ ${prev} =~ ${fileopts} || ${prev} == -f ]]; then
        COMPREPLY=( $(compgen -f -- ${cur}) )
        return 0
    fi

    if [[ ${cur} =~ -- ]]; then
        COMPREPLY=( $(compgen -W "${longopts}" -- ${cur}) )
        return 0
    elif [[ ${cur} =~ - ]]; then
        COMPREPLY=( $(compgen -W "${shortopts}" -- ${cur}) )
        return 0
    elif [[ ${cur} == * ]] ; then
        COMPREPLY=( $(compgen -W "${ids}" -- ${cur}) )
        return 0
    fi
}

complete -F __runner runner
