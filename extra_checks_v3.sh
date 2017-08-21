# names of all tests
tests=(v3_def v3_ptCutDght v3_dEdxCutDght v3_minRad1 v3_minRad3 v3_minRad7 v3_minRad9 v3_minRad12 v3_minRad15 v3_tightV0Rad9DedxPt)

for((i = 0; i < ${#tests[@]}; ++i)); do
   mkdir ${tests[$i]}
   cd ${tests[$i]}
   ln -s ../vn_SPV0AC_K0L_hirCombL13L4_extraChks.root
   cp ../getHistVnPtK0.C .
   root -b -q 'getHistVnPtK0.C("vn_SPV0AC_K0L_hirCombL13L4_extraChks.root", "'${tests[$i]}'", "'${tests[$i]}'", 1)'
   cd ..
done


for((i = 0; i < ${#tests[@]}; ++i)); do
   root -b -q 'makeRatios.C("'${tests[0]}'", "'${tests[$i]}'")'
done


for((i = 0; i < ${#tests[@]}; ++i)); do
   root -b -q 'makePlots.C("'${tests[$i]}'")'
done
