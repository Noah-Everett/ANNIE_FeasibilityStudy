main() {
export RECURSIVE=false
export OTHER=""
cp /Users/noaheverett/Library/Mobile\ Documents/com~apple~CloudDocs/Documents/ANNIE/ANNIE_Analysis/Scripts/getTgt.C /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros

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
      echo root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"\) | tee tgt.list
           root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"\) | tee tgt.list
    echo cd -
    cd -
  done
else
  echo root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"\) | tee tgt.list
       root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"\) | tee tgt.list
fi

rm /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros/getTgt.C
}

usage() {
cat >&2 <<EOF
make_combined_gst.sh -r                 (recursive (directories))
                     -h|--help          (display the usage statement (this output))
                     <ghep file number> (number (numbers with '*') of the ghep file to convert to gst)
If your usage of this command did not work, use "source $S/make_genie_gst.sh "##\*"
EOF
}

main "$@"
