# names of all tests
tests=(v2_ptCutDght v2_dEdxCutDght v2_minRad1 v2_minRad3 v2_minRad7 v2_minRad9 v2_minRad12 v2_minRad15 v2_tightV0Rad9DedxPt)

for((i = 0; i < ${#tests[@]}; ++i)); do
   mkdir ${tests[$i]}
   cd ${tests[$i]}
   ln -s ../../vn_SPV0AC_K0L_hirCombL13L4_extraChks.root
   cp ../../getHistVnPtK0.C .
   root -b -q 'getHistVnPtK0.C("vn_SPV0AC_K0L_hirCombL13L4_extraChks.root", "'${tests[$i]}'", "'${tests[$i]}'", 0, 0)'
   cd ..
done

#mkdir ratios
#cp ../makeRatios.C . 
#for((i = 0; i < ${#tests[@]}; ++i)); do
#   root -b -q 'makeRatios.C("'${tests[0]}'", "'${tests[$i]}'")'
#done

#mkdir v2plots
#cp ../makePlots.C . 
#for((i = 0; i < ${#tests[@]}; ++i)); do
#   root -b -q 'makePlots.C("'${tests[$i]}'")'
#done
