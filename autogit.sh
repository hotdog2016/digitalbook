#bin/bash

CURRENT=$(date "+%Y-%m-%d %H:%M:%S")
git commit -am "$CURRENT" && git push digitalbook master
################################################################
