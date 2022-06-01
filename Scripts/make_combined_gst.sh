main() {
export RECURSIVE=false
export OTHER=""
cp /Users/noaheverett/Library/Mobile\ Documents/com~apple~CloudDocs/Documents/ANNIE/ANNIE_Analysis/Scripts/combineTrees.C /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros

for i in "$@"; do
  case $i in
    -r*                    ) export RECURSIVE=true        shift    ;;
    -h*|--help*            ) usage;                       return 1 ;;
    -*                     ) echo "Unknown option \"$i\"" return 1 ;;
    *                      ) export OTHER="${OTHER} $i"   shift    ;;
  esac
done

if [ $RECURSIVE == true ]; then
  for folder in */; do
    echo cd ${folder}
    cd ${folder}
    for file in gntp.$(echo ${OTHER} | tr -d '\\\ ').ghep.root; do
      if [ -f "$file" ]; then
        echo root -l -q combineTrees.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\",{\"$(echo ${OTHER} | tr -d '\\\ ')\"}\)
             root -l -q combineTrees.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\",{\"$(echo ${OTHER} | tr -d '\\\ ')\"}\)
      fi
    done
    echo cd -
    cd -
  done
else
  for file in gntp.$(echo ${OTHER} | tr -d '\\\ ').gst.root; do
    if [ -f "$file" ]; then
      echo root -l -q combineTrees.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\",{\"$(echo ${OTHER} | tr -d '\\\ ')\"}\)
           root -l -q combineTrees.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\",{\"$(echo ${OTHER} | tr -d '\\\ ')\"}\)
    fi
  done
fi

rm /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros/combineTrees.C
}

usage() {
cat >&2 <<EOF
make_combined_gst.sh -r                 (recursive (directories))
                     -h|--help          (display the usage statement (this output))
                     <ghep file number> (number (numbers with '*') of the ghep file to convert to gst)
If your usage of this command did not work, use "source $S/make_genie_gst.sh "##\*""
EOF
}

main "$@"
