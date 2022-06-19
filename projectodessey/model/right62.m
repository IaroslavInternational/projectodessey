function [EV_]=right62(t, data)

global tau

eta=data(1:6);% x y z phi(крен) teta(тангаж) psi(рыскание) 
v=data(7:12);% u v w p q r

lx1=.15125;
lz1=-.00387;
lx2=lx1*sin(pi/6);
ly2=lx1*cos(pi/6);
lz2=-.00387;
lx3=lx1*sin(pi/6);
ly3=lx1*cos(pi/6);
lz3=-.00387;
lx4=.225;
lx5=lx4*sin(pi/6);
ly5=.19486;
lx6=lx4*sin(pi/6);
ly6=.19486;

K=diag([1 1 1 1 1 1]);%Усилие (1-4 горизонтальные, 5-8 вертикальные)
Ta=[0 cos(pi/6)      cos(pi/6)      0    0    0
    1 sin(pi/6)     -sin(pi/6)      0    0    0
    0 0              0              1    1    1
    lz1 sin(pi/6)*lz2 -sin(pi/6)*lz3  0   ly5  -ly6
    0 cos(pi/6)*lz2  cos(pi/6)*lz3 lx4  -lx5 -lx6
    -lx1 sin(pi/6)*lx2+cos(pi/6)*ly2 -sin(pi/6)*lx3-cos(pi/6)*ly3 0 0 0];

u=inv(Ta*K)*tau;

Xu_=-5.5;% kg
Yv_=-12.7;% kg
Zw_=-14.57;% kg
Kp_=-0.12;% kg*m^2/rad
Mq_=-0.12;% kg*m^2/rad
Nr_=-0.12;% kg*m^2/rad

Xu=-4.03;% Ns/m
Yv=-6.22;% Ns/m
Zw=-5.18;% Ns/m
Kp=-0.07;% Ns/rad
Mq=-0.07;% Ns/rad
Nr=-0.07;% Ns/rad

Xuu=-18.18;% Ns2/m2
Yvv=-21.66;% Ns2/m2
Zww=-36.99;% Ns2/m2
Kpp=-1.55;% Ns2/rad2
Mqq=-1.55;% Ns2/rad2
Nrr=-1.55;% Ns2/rad2

xb=0.00;% m
yb=0.00;% m
zb=0.00;% m

rb=[xb;yb;zb];

xg=0.00;% m
yg=0.00;% m
zg=0.00;% m

rg=[xg;yg;zg];

m=11.5;% kg
Ix=0.16;% kg*m^2
Iy=0.16;% kg*m^2
Iz=0.16;% kg*m^2
W=112.8;%N
B=W;114.8;%N

Mrb11=diag([m m m]);
Mrb12=[0 m*zg 0;-m*zg 0 0;0 0 0];
Mrb21=-Mrb12;
Mrb22=[Ix 0 0;0 Iy 0;0 0 Iz];
Mrb=[Mrb11 Mrb12;Mrb21 Mrb22];
Mrb= [ m*eye(3) -m*Smtrx(rg)
       m*Smtrx(rg) Mrb22];
Ma=-diag([Xu_ Yv_ Zw_ Kp_ Mq_ Nr_]);

M=Mrb+Ma;
Crb = m2c(Mrb,v);

Ca =[0 0 0  0 -Zw_*v(3) Yv_*v(2)*1;
     0 0 0 Zw_*v(3) 0 -Xu_*v(1);
     0 0 0 -Yv_*v(2) Xu_*v(1) 0;
     0 -Zw_*v(3) Yv_*v(2) 0 -Nr_*v(6) Mq_*v(5);
     Zw_*v(3) 0 -Xu_*v(1) Nr_*v(6) 0 -Kp_*v(4);
     -Yv_*v(2) Xu_*v(1) 0 -Mq_*v(5) Kp_*v(4) 0]*0;
 
C=Crb+Ca;

Dlin=-diag([Xu Yv Zw Kp Mq Nr]);
Dquad=-diag([Xuu*abs(v(1)) Yvv*abs(v(2)) Zww*abs(v(3)) Kpp*abs(v(4)) Mqq*abs(v(5)) Nrr*abs(v(6))]);

D=Dlin*1+Dquad*1;

g=[ (W-B)*sin(eta(5));
   -(W-B)*cos(eta(5))*sin(eta(4)); 
   -(W-B)*cos(eta(5))*cos(eta(4)); 
    zg*W*cos(eta(5))*sin(eta(4)); 
    zg*W*sin(eta(5)); 0];
v_=inv(M)*(tau-C*v-D*v-g);
eta_=[Rzyx(eta(4),eta(5),eta(6))*v(1:3); Tzyx(eta(4),eta(5))*v(4:6)];

EV_=[eta_+[0;0;rand(1)*0*sin(t*2)*1e-2;0;0;cos(t/3)*sin(t*10)*0]; v_];

end