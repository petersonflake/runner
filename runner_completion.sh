#!/usr/bin/bash
text="$(./testing -c)"
complete -W "$text" testing
