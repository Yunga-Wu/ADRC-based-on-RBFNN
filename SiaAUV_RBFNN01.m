function [sys,x0,str,ts] = SiaAUV_RBFNN01(t,x,u,flag)
switch flag
case 0
    [sys,x0,str,ts]=mdlInitializeSizes;
case 1
    sys=mdlDerivatives(t,x,u);
case 3
    sys=mdlOutputs(t,x,u);
case {2,4,9}
    sys=[];
otherwise
    error(['Unhandled flag = ',num2str(flag)]);
end

function [sys,x0,str,ts]=mdlInitializeSizes
global c bn
sizes = simsizes;
sizes.NumContStates  = 5;
sizes.NumDiscStates  = 0; 
sizes.NumOutputs     = 2;
sizes.NumInputs      = 2;
sizes.DirFeedthrough = 1;
sizes.NumSampleTimes = 0;
sys = simsizes(sizes);
x0  = 1.0*[1 1 1 1 1];
c=1.0*[-0.2 0.5 1 0 -0.5;
   1.0 0.5 0.0 -0.5 -0.1];%初始化径向基函数中心矢量c
bn=0.5*ones(5,1);%初始化径向基函数基宽矢量bn
str = [];
ts  = [];

function sys=mdlDerivatives(t,x,u)
global c bn
gama=30;%学习率

e=u(1);
de=u(2);

kd=10;
kp=6;
E=[e,de]';

A=[0 -kp;
   1 -kd];
Q=[200 0;0 200];
P=lyap(A,Q);

xi=[e;de];
h=zeros(5,1);
for j=1:1:5
    h(j)=exp(-norm(xi-c(:,j))^2/(2*bn(j)*bn(j)));
end
W=[x(1) x(2) x(3) x(4) x(5)]';

b=[0;1];
S=-gama*E'*P*b*h;

for i=1:1:5
    sys(i)=S(i);
end
 
function sys=mdlOutputs(t,x,u)
global c bn

e=u(1);
de=u(2);    

W=[x(1) x(2) x(3) x(4) x(5)]';
xi=[e;de];
h=zeros(5,1);
for j=1:1:5
    h(j)=exp(-norm(xi-c(:,j))^2/(2*bn(j)*bn(j)));
end
fxp=W'*h;%RBF估计量     

%控制信号
ut=0;

sys(1)=W(1);
sys(2)=fxp;