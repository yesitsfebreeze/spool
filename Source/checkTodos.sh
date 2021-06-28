TAGS="TODO:|FIXME:|NOTE:"
ERRORTAG="ERROR:"
find "./Source" \( -name "*.h" -or -name "*.cpp"  \) -print0 | xargs -0 egrep --with-filename --line-number --only-matching "($TAGS).*\$|($ERRORTAG).*\$" | perl -p -e "s/($TAGS)/ warning: \$1/" | perl -p -e "s/($ERRORTAG)/ error: \$1/"
