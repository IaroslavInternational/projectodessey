clc;
clear;
close all;

%%

global tau

initData = jsondecode(fileread('init.json'));

eta  = [initData.modelState.x;    initData.modelState.z;     initData.modelState.y; 
        initData.modelState.roll; initData.modelState.pitch; initData.modelState.yaw];
v0   = [initData.modelVelocity.x;    initData.modelVelocity.z;     initData.modelVelocity.y; 
        initData.modelVelocity.roll; initData.modelVelocity.pitch; initData.modelVelocity.yaw];
tmax = initData.timeSpan.max;
tmin = initData.timeSpan.min;
step = initData.timeSpan.step;
tau  = [initData.tau.x;    initData.tau.y;     initData.tau.z;
        initData.tau.roll; initData.tau.pitch; initData.tau.yaw];

clear initData;

%%

EV0     = [eta; v0];
tspan   = tmin:step:tmax;
[t,EV]  = ode45(@right62, tspan, EV0);

figure('Name','Вывод графика угла вокруг оси Х, оси Y, оси Z')
plot(t,EV(:,4),t,EV(:,5),t,EV(:,6));grid on;

figure('Name','Вывод графика угловой скорости вокруг оси Х, оси Y, оси Z')
plot(t,EV(:,10),t,EV(:,11),t,EV(:,12));grid on

figure('Name','Вывод графика положения в пространстве')
plot3(EV(:,1),EV(:,2),-EV(:,3),'-');grid on
xlabel('X')
ylabel('Y')
zlabel('Z')

data_pos    = EV(:,1:3);
data_orient = EV(:,4:6);
data_time   = t;

%%

save3arr('position.txt',    data_pos');
save3arr('orientation.txt', data_orient');
save1arr('time.txt',        data_time);