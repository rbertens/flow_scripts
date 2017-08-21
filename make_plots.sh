# names of all tests
tests=(v2_def v2_vtx12 v2_vtx5 v2_CL0cent v2_CL1cent v2_nopileup v2_esdDif700 v2_remCh46 v2_def_negEta v2_def_posEta v2_ncls60 v2_ncls80 v2_nclspid60 v2_nclspid90 v2_ncrfind07 v2_ncrfind09 v2_DCATrkPV005 v2_DCATrkPV03 v2_DCADght07 v2_DCADght03 v2_cosPA098 v2_minRad0 v2_minRad4 v2_minRad6 v2_maxRad50 v2_maxRad150 v2_armPodCut v2_nsig1 v2_nsig4 v2_looseV0 v2_tightV0 v2_ptCutDght v2_dEdxCutDght v2_minRad1 v2_minRad3 v2_minRad7 v2_minRad9 v2_minRad12 v2_minRad15 v2_tightV0Rad9DedxPt)


mkdir ratios
cp ../makeRatios.C . 
for((i = 0; i < ${#tests[@]}; ++i)); do
   root -b -q 'makeRatios.C("'${tests[0]}'", "'${tests[$i]}'")'
done

mkdir vnplots
cp ../makePlots.C . 
for((i = 0; i < ${#tests[@]}; ++i)); do
   root -b -q 'makePlots.C("'${tests[$i]}'")'
done

mkdir uncertainties
cp ../makeRelativeSystematics.C .
for((i = 0; i < ${#tests[@]}; ++i)); do
   root -b -q 'makeRelativeSystematics.C("'${tests[0]}'", "'${tests[$i]}'")'
done
