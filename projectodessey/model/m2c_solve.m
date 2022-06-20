function X=m2c_solve(Mrb,v)

%Declare patch as extrinsic
coder.extrinsic('m2c'); 

X=m2c(Mrb,v);