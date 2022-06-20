function X=Tzyx_solve(eta1,eta2)

%Declare patch as extrinsic
coder.extrinsic('Tzyx'); 

X=Tzyx(eta1,eta2);