#!/usr/bin/bash
text="$(runner -c)"
complete -W "$text" runner
