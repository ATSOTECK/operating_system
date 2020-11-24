@echo off
cloc . --by-file --exclude-dir=bin,.idea,.git, --not-match-f="^.*\.(bat)$|scc" --quiet --report-file=scc.txt