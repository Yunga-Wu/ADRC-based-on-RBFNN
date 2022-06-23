%% 仿真实验1：航向角控制效果对比实验
clear;
clc;

% simulation step
h=0.005;

%TD's arguments
r0=0.8;%r0越大跟踪速率越快
h0=1.0;%h0越大，滤波效果越好

%NLSEF's arguments
wc=0.5;

%ESO's argument
b0=0.06;
w0=80;

%PID
kp=50;
ki=0.9;
kd=90;

sim("ADRConSiaAUV_ex01");
% y1=8*ones(20001,1);
% y2=9.5*ones(20001,1);
% y3=10.5*ones(20001,1);
figure(1)
p=plot(t, yaw_d,':', t, yaw_pid,'--', t, yaw_adrc,'-.', t, yaw_RBFNN,'-');
% p=plot(t, yaw_d,':', t, yaw_pid,'-.', t, yaw_adrc,'--', t, yaw_RBFNN,'-',t,y1,t,y2,t,y3);
C=lines(7);
p(1).Color=C(1,:);
p(2).Color=C(3,:);
p(3).Color=C(5,:);
p(4).Color=C(7,:);
[p.LineWidth]=deal(2);
grid on
legend({'INPUT','PID','ADRC', 'ADRC+RBFNN'},'Location','NorthEast','FontName','Times New Roman','FontSize',12);
% title('图1 航向角控制效果对比');
xlabel('time(s)','FontName','Times New Roman','FontSize',14);
ylabel('yaw(^。)','FontName','Times New Roman','FontSize',14);
set(gcf,'unit','centimeters','position',[20 40 14 10]);
set(gca,'FontName','Times New Roman','FontSize',14);

%% 仿真实验2：航向角抗干扰控制效果对比实验
clear;
clc;

% simulation step
h=0.005;

%TD's arguments
r0=0.8;%r0越大跟踪速率越快
h0=1;%h0越大，滤波效果越好

%NLSEF's arguments
wc=0.5;

%ESO's argument
b0=0.06;
w0=80;

%PID
kp=50;
ki=0.9;
kd=90;

sim("ADRConSiaAUV_ex02");
% y1=9*ones(20001,1);
% y2=9.5*ones(20001,1);
% y3=10.5*ones(20001,1);
figure(2)
p=plot(t, yaw_d,':', t, yaw_pid,'--', t, yaw_adrc,'-.', t, yaw_RBFNN,'-');
% p=plot(t, yaw_d,':', t, yaw_pid,'-.', t, yaw_adrc,'--', t, yaw_RBFNN,'-',t,y1,t,y2,t,y3);
C=lines(7);
p(1).Color=C(1,:);
p(2).Color=C(3,:);
p(3).Color=C(5,:);
p(4).Color=C(7,:);
[p.LineWidth]=deal(2);
grid on
legend({'INPUT','PID','ADRC', 'ADRC+RBFNN'},'Location','NorthEast','FontName','Times New Roman','FontSize',12);
% title('图2 航向角控制效果对比');
xlabel('time(s)','FontName','Times New Roman','FontSize',14);
ylabel('yaw(^。)','FontName','Times New Roman','FontSize',14);
set(gcf,'unit','centimeters','position',[20 40 14 10]);
set(gca,'FontName','Times New Roman','FontSize',14);

%% 仿真实验3：航向角干扰估计效果对比实验
clear;
clc;

% simulation step
h=0.005;

%TD's arguments
r0=0.8;%r0越大跟踪速率越快
h0=1.0;%h0越大，滤波效果越好

%NLSEF's arguments
wc=0.5;

%ESO's argument
b0=0.06;
w0=80;

sim("ADRConSiaAUV_ex03_0");
sim("ADRConSiaAUV_ex03_1");

figure(3)
p=plot(t,dis,':',t,adrc_f1-adrc_f0,'-.',t,RBF_f1-RBF_f0,'-');
C=lines(7);
p(1).Color=C(1,:);
p(2).Color=C(5,:);
p(3).Color=C(7,:);
[p.LineWidth]=deal(2);
grid on
legend({'Disturbance','ADRC',' ADRC+RBFNN'},'Location','NorthEast','FontName','Times New Roman','FontSize',12);
% title('图3 航向角控制正弦扰动估计对比');
xlabel('time(s)','FontName','Times New Roman','FontSize',14);
ylabel('Estimation Value of Disturbance','FontName','Times New Roman','FontSize',14);
set(gcf,'unit','centimeters','position',[20 40 14 10]);
set(gca,'FontName','Times New Roman','FontSize',14);