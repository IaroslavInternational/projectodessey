function X=Smtrx_solve(rg)

%Declare patch as extrinsic
coder.extrinsic('Smtrx'); 

X=Smtrx(rg);