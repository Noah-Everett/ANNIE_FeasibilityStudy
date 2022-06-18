main() {
export RECURSIVE=""
export LOCATION=""
export GEOMETRY=""
export VERBOSITY=""
export OTHER=""
export _TARGETS=""
cp /Users/noaheverett/Library/Mobile\ Documents/com~apple~CloudDocs/Documents/ANNIE/ANNIE_Analysis/Scripts/getTgt.C /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros

for i in "$@"; do
  case $i in
    -r*                    ) export RECURSIVE=true        shift    ;;
    -l*                    ) export LOCATION=",true"      shift    ;;
    -g=*                   ) export GEOMETRY="${i#*=}"    shift    ;;
    -v*                    ) export VERBOSITY=",true"     shift    ;;
    -h*|--help*            ) usage;                       return 1 ;;
    -*                     ) echo "Unknown option \"$i\"" return 1 ;;
    *                      ) export OTHER="${OTHER} $i"   shift    ;;
  esac
done

if [ -n "${LOCATION}" ] && [ -z "${GEOMETRY}" ]; then
  echo "To use \`-l\` to get interaction locations, provide a gdml geometry file with \`-g=\`."
  return 1
fi 

if [ -n "${LOCATION}" ]; then
  export _TARGETS="_volumes"
fi



if [ $RECURSIVE == true ]; then
  for folder in */; do
    echo cd ${folder}
    cd ${folder}
    echo root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"${LOCATION}",\""$(basename ${GEOMETRY})"\""${VERBOSITY}\) | tee tgt${_TARGETS}.list
         root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"${LOCATION}",\""$(basename ${GEOMETRY})"\""${VERBOSITY}\) | tee tgt${_TARGETS}.list
    cd -
  done
else
  echo root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"${LOCATION}",\""$(basename ${GEOMETRY})"\""${VERBOSITY}\) | tee tgt${_TARGETS}.list
       root -l -q getTgt.C\(\"$(echo ${OTHER} | tr -d '\\\ ')\"${LOCATION}",\""$(basename ${GEOMETRY})"\""${VERBOSITY}\) | tee tgt${_TARGETS}.list
fi

rm /opt/homebrew/Cellar/root/6.26.02_1/share/root/macros/getTgt.C


}

usage() {
cat >&2 <<EOF
make_combined_gst.sh -r                 (recursive (directories))
                     -l                 (display locations)
                     -g=</path/to/geom> (path to geometry file (needed if using \'-l\'))
                     -v                 (verbose)
                     -h|--help          (display the usage statement (this output))
                     <ghep file number> (number (numbers with '*') of the ghep file to convert to gst)
If your usage of this command did not work, use "source $S/make_tgt_list.sh "\*"
EOF
}

main "$@"
