function X=Rzyx_solve(eta1,eta2,eta3)

%Declare patch as extrinsic
coder.extrinsic('Rzyx'); 

X=Rzyx(eta1,eta2,eta3);