clc; close all;
eta=[0;0;0;0;0;0];
v0=[0;0;0;0;0;0];
EV0=[eta; v0];
tspan=[0:.1:50];%временной отрезок
options = odeset('RelTol',1e-6);
[t,EV]=ode45(@right62,tspan,EV0);

sim('untitled')

figure('Name','Position')
plot3(XYZ(:,2),XYZ(:,3),-XYZ(:,4))
grid on
hold on
plot3(EV(:,1),EV(:,2),-EV(:,3),'-r')%Вывод графика положения в пространстве
xlim([-1 1]);
ylim([-1 1]);
zlim([-2 0]);
xlabel('X')
ylabel('Y')
zlabel('Z')

figure('Name','Angular velocity')
grid on;
plot(AngVel(:, 1),AngVel(:, 2),AngVel(:, 1),AngVel(:, 3),AngVel(:, 1),AngVel(:, 4));
legend('AvelX','AvelY','AvelZ')