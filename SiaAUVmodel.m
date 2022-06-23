function [xdot,U] = SiaAUVmodel(x,ui)
%1.参考《潜艇操纵性》P167：格特勒潜艇标准运动方程
%2.参考fossen handbook MSS工具箱
% [xdot,U] = SiaAUVmodel(x,ui) returns the speed U in m/s (optionally) and the 
% time derivative of the state vector: x = [ u v w p q r x y z phi theta psi ]' for
% our using auv.
% The size of the AUV is L = 2.934 m, B=0.968, H=0.616m.
% while the state vector is defined as:
% u     = surge velocity          (m/s)
% v     = sway velocity           (m/s)
% w     = heave velocity          (m/s)
% p     = roll velocity           (rad/s)
% q     = pitch velocity          (rad/s)
% r     = yaw velocity            (rad/s)
% xpos  = position in x-direction (m)
% ypos  = position in y-direction (m)
% zpos  = position in z-direction (m)
% phi   = roll angle              (rad)
% theta = pitch angle             (rad)
% psi   = yaw angle               (rad)
%
% The input vector is :
% ui  = [ nsL nsR nhB nhs nbBL nbBR nbS ]'  where
% nsL =  左艉推转速，最大转速1100，控制水平面直线运动和航向角，
% nsR =  右艉推转速，最大转速1100，控制水平面直线运动和航向角
% nhB =  前水平槽道推进器转速，最大转速1000，控制侧向速度稳定为0
% nhS =  后水平槽道推进器转速，最大转速1000，控制侧向速度稳定为0
% nbBL = 前左垂直槽道推进器转速，最大转速1000，控制纵倾角和深度，没有控制横摇角
% nbBR = 前右垂直槽道推进器转速，最大转速1000，控制纵倾角和深度，没有控制横摇角
% nbS =  后垂直槽道推进器转速，最大转速1000，控制纵倾角和深度，没有控制横摇角

% Check of input and state dimensions
if (length(x) ~= 12),error('x-vector must have dimension 12!');end
if (length(ui) ~= 7),error('ui-vector must have dimension 7!');end

% Dimensional states
u   = x(1);  v     = x(2);  w   = x(3);
p   = x(4);  q     = x(5);  r   = x(6);
phi = x(10); theta = x(11); psi = x(12);

max_ui = zeros(7,1);
max_ui(1)  = 1100;  max_ui(2)  = 1100;
max_ui(3)  = 1000;  max_ui(4)  = 1000;
max_ui(5)  = 1000;  max_ui(6)  = 1000;  max_ui(7)  = 1000;
for i=1:1:7
    %限制不超过最大转速
   if abs(ui(i)) > max_ui(i)
       ui(i)=sign(ui(i)) * max_ui(i);
   end
%    %限制的最小转速为10转，因为低于最低转速时艉推进器的公式失效
%    if abs(ui(i)) < 10
%        ui(i)=sign(ui(i)) * 10;
%    end
end

nsL=ui(1);   nsR=ui(2);
nhB =  ui(3);   nhS =  ui(4);
nbBL=  ui(5);   nbBR=  ui(6);   nbS =  ui(7);

c1 = cos(phi);
c2 = cos(theta);
c3 = cos(psi);
s1 = sin(phi);
s2 = sin(theta);
s3 = sin(psi);
t2 = tan(theta);

%main dimensions
L   = 2.934;
g   = 9.8;
xG  = 0;      yG  = 0;      zG = 0.012;
xB  = 0;      yB  = 0;      zB = 0;%以浮心为坐标原点
rho = 1027.7;
ThetaT = 13*pi/180;%rad
m = 1207.4;
W   = 11807.52;  B   = W; %W重力，B浮力实际值11832.52,仿真中需要设置为中性浮力
Ix  = 108.14;   Iy  = 620.53;  Iz  = 663.6;
Ixy = 0; Iyz = 0; Ixz = -0.158;

%hydrodynamic derivatives，参考《潜艇操纵性》P167：格特勒潜艇标准运动方程
r2 = rho*L^2/2;
r3 = rho*L^3/2;
r4 = rho*L^4/2;
r5 = rho*L^5/2;

Xudot = -19.459e-3; Xq2 = -0.06722e-3;  Xr2 = -5.06367e-3;
Xrp = 10.83937e-3;  Xvr = 46.75933e-3;  Xwq = -65.7415e-3;
Xw2 = -4.67582e-3;  Xv2 = -38.9615e-3;  Xu2 = -17.5582e-3;

Ypabsp = 7.39663e-3;    Ypq = -11.1071e-3;  Yqr = -1.71998e-3;
Yvq = 6.692951e-3;  Ywp = 114.5256e-3;  Ywr = -4.0379e-3;
Yur = 18.41458e-3;  Yup = -3.58749e-3;  Yvabsr = -20.5011e-3;
Yu2 = 0.351169e-3;  Yuv = -114.848e-3;  Yvabsv = -74.2288e-3;
Yvw = -12.6272e-3;  
Yvdot = -66.1643e-3;    Ypdot = 0.060941e-3;    Yrdot = 2.064763e-3;

Zp2 = 3.354883e-3;  Zr2 = -2.53649e-3;  Zrp = 14.1277e-3;
Zvr = -7.83261e-3; Zvp = -84.0462e-3; Zuq = -46.6785e-3;
Zwabsq = -64.9351e-3;  Zu2 = 4.880111e-3;  Zuw = -191.277e-3;
Zwabsw = -133.053e-3;  Zuv = -3.70547e-3; Zuabsw = -1.38329e-3;
Zw2 = -10.2749e-3;  Zv2 = 7.55362e-3;
Zwdot = -138.68e-3; Zqdot = -2.85694e-3;

Kqr = 2.357583e-3;  Kpq = 0.091453e-3;  Kpabsp = 0.187666e-3;
Kup = -1.94209e-3; Kur = -0.39165e-3; Kvq = -1.01474e-3;
Kwp = -1.93344e-3; Kwr = 1.474016e-3; Ku2 = -0.00023e-3; 
Kuv = 1.123393e-3; Kvabsv = -1.64839e-3;  Kvw = -66.3314e-3;
Kvdot = -0.07104e-3;    Kpdot = -0.39542e-3;    Krdot = 0.027937e-3;

Mp2 = 2.843382e-3;  Mr2 = -0.98189e-3;  Mrp = 5.209237e-3;
Mqabsq = -2.7554e-3;    Mvr = -3.32664e-3;  Mvp = -11.0556e-3;
Muq = -25.6643e-3;  Mabswq = -4.91085e-3;   Mu2 = 1.420507e-3;
Muw = 54.30752e-3;  Mwabsw=-29.405e-3; Muabsw = -0.0722e-3;
Mw2 = 3.375474e-3; Mv2 = -0.81596e-3;  
Mwdot = -3.41661e-3;    Mqdot = -6.0748e-3;

Npq = -1.29065e-3;  Nqr = -1.5022e-3;  Nrabsr = -3.22266e-3;
Nwr = -3.39053e-3;  Nwp = 0.299687e-3;  Nvq = -0.70204e-3;
Nup = -1.62993e-3;  Nur = -15.2076e-3;  Nabsvr = 4.579522e-3;
Nu2 = -0.04856e-3;  Nuv = -13.5556e-3;  Nvabsv = 12.28311e-3;
Nvw = 3.997094e-3;
Nvdot = 2.144051e-3;    Npdot = 0.104852e-3;    Nrdot = -3.47144e-3;

%Thrusters' forces and moments
%暂时假设来流速度为0.0m/s
Vcur = 0.0;
%左右艉推F=A*Vcurr^2+B*Vcurr*n+C*n^2，当进速系数大于0.5或小于0时不再适用
FsL = -4.210*Vcur^2-0.309*Vcur*nsL+0.052*nsL^2;    
FsR = -4.210*Vcur^2-0.309*Vcur*nsR+0.052*nsR^2;
%坐标
xstn = 1.530;   ystn = 0.26;   zstn = 0;
%水平槽道推进器
FhB = 5.6924e-8*nhB^3+4.4051e-7*nhB^2+0.0237*nhB;   
FhS = 5.6924e-8*nhS^3+4.4051e-7*nhS^2+0.0237*nhS;
%坐标
xhB = 0.672;    xhS = 0.628;
%垂直槽道推进器
FbBL = 5.6924e-8*nbBL^3+4.4051e-7*nbBL^2+0.0237*nbBL;   
FbBR = 5.6924e-8*nbBR^3+4.4051e-7*nbBR^2+0.0237*nbBR;  
FbS = 5.6924e-8*nbS^3+4.4051e-7*nbS^2+0.0237*nbS;
%坐标
xb = 0.884; yb = 0.347; zb = 0;
xbow = 1.15;

% forces and moments
X = r4*(Xq2*q^2+Xr2*r^2+Xrp*r*p)+...
    r3*(Xvr*v*r+Xwq*w*q)+...
    r2*(Xu2*u^2+Xv2*v^2+Xw2*w^2)-...
    (W-B)*s2+(FsL+FsR)*cos(ThetaT)-...
    m*(-v*r+w*q-xG*(q^2+r^2)+yG*p*q+zG*p*r);

Y = r4*(Ypabsp*p*abs(p)+Ypq*p*q+Yqr*q*r)+...
    r3*(Yvq*v*q+Ywp*w*p+Ywr*w*r)+...
    r3*(Yur*u*r+Yup*u*p+Yvabsr*v/(abs(v)+eps)*abs(sqrt(v^2+w^2))*abs(r))+...
    r3*(Yu2*u^2+Yuv*u*v+Yvabsv*v*abs(sqrt(v^2+w^2)))+...
    r2*Yvw*v*w+...
    (W-B)*c2*s1+FhB+FhS+(FsL-FsR)*sin(ThetaT)-...
    m*(-w*p+u*r-yG*(r^2+p^2)+zG*q*r+xG*q*p);

Z = r4*(Zp2*p^2+Zr2*r^2+Zrp*r*p)+...
    r3*(Zvr*v*r+Zvp*v*p)+...
    r3*(Zuq*u*q+Zwabsq*w/(abs(w)+eps)*abs(sqrt(v^2+w^2))*abs(q))+...
    r2*(Zu2*u^2+Zuw*u*w+Zwabsw*w*abs(sqrt(w^2+v^2)))+...
    r2*(Zuv*u*v+Zuabsw*u*abs(w)+Zw2*abs(w)*abs(sqrt(v^2+w^2))+Zv2*v^2)+...
    (W-B)*c2*c1+FbBL+FbBR+FbS-...
    m*(-u*q+v*p-zG*(p^2+q^2)+xG*r*p+yG*r*q);

K = r5*(Kqr*q*r+Kpq*p*q+Kpabsp*p*abs(p))+...
    r4*(Kup*u*p+Kur*u*r+Kvq*v*q+Kwp*w*p+Kwr*w*r)+...
    r3*(Ku2*u^2+Kuv*u*v+Kvabsv*v*abs(sqrt(v^2+w^2))+Kvw*v*w)+...
    (yG*W-yB*B)*c2*c1-(zG*W-zB*B)*c2*s1+...
    FbBL*yb-FbBR*yb-...
    (Iz-Iy)*q*r+p*q*Ixz-(r^2-q^2)*Iyz-p*r*Ixy-...
    m*(yG*(-u*q+v*p)-zG*(-w*p+u*r));

M = r5*(Mp2*p^2+Mr2*r^2+Mrp*r*p+Mqabsq*q*abs(q))+...
    r4*(Mvr*v*r+Mvp*v*p+Muq*u*q+Mabswq*abs(sqrt(v^2+w^2))*q)+...
    r3*(Mu2*u^2+Muw*u*w+Mwabsw*w*abs(sqrt(v^2+w^2)))+...
    r3*(Muabsw*u*abs(w)+Mw2*abs(w)*abs(sqrt(v^2+w^2))+Mv2*v^2)-...
    (xG*W-xB*B)*c1*c2-(zG*W-zB*B)*s2-...
    FbS*xbow+FbBL*xb+FbBR*xb-...
    (Ix-Iz)*r*p+q*r*Ixy-(p^2-r^2)*Ixz-q*p*Iyz-...
    m*(zG*(-v*r+w*q)-xG*(-u*q+v*p));

N = r5*(Npq*p*q+Nqr*q*r+Nrabsr*r*abs(r))+...
    r4*(Nwr*w*r+Nwp*w*p+Nvq*v*q)+...
    r4*(Nup*u*p+Nur*u*r+Nabsvr*abs(sqrt(v^2+w^2))*r)+...
    r3*(Nu2*u^2+Nuv*u*v+Nvabsv*v*abs(sqrt(v^2+w^2))+Nvw*v*w)+...
    (xG*W-xB*B)*c2*s1+(yG*W-yB*B)*s2+...
    FhB*xhB-FhS*xhS+(FsL-FsR)*ystn*cos(ThetaT)-(FsL-FsR)*xstn*sin(ThetaT)-...
    (Iy-Ix)*p*q+r*p*Iyz-(q^2-p^2)*Ixy-r*q*Ixz-...
    m*(xG*(-w*p+u*r)-yG*(-v*r+w*q));

%Matrix of mass
% MassMatrix = [m-r3*Xudot 0 0 0 m*zG -m*yG;...
%     0 m-r3*Yvdot 0 -m*zG-r4*Kvdot 0 m*xG-r4*Nvdot;...
%     0 0 m-r3*Zwdot m*yG -m*xG-r4*Mwdot 0;...
%     0 -m*zG-r4*Ypdot m*yG Ix-r5*Kpdot -Ixy -Ixz-r5*Npdot;...
%     m*zG 0 -m*xG-r4*Zqdot -Ixy Iy-r5*Mqdot -Iyz;...
%     -m*yG m*xG-r4*Yrdot 0 -Ixz-r5*Krdot -Iyz Iz-r5*Nrdot];
%
% Dimensional state derivatives (xdot = in(M)*f(x) is expanded to avoid inv(M))
% xdot = [X Y Z K M N] * inv(MassMatrix);
xdot = [6.850e-4*X+3.317e-7*Z-7.667e-6*M
    4.858e-4*Y+5.638e-5*K+3.648e-5*N
    2.774e-7*X+3.337e-4*Z-2.795e-5*M
    3.832e-5*Y+6.571e-3*K+2.138e-5*N
    -7.667e-6*X-3.342e-5*Z+7.726e-4*M
    3.814e-5*Y+7.660e-5*K+9.541e-4*N
    c3*c2*u + (c3*s2*s1-s3*c1)*v + (s3*s1+c3*c1*s2)*w
    s3*c2*u + (c1*c3+s1*s2*s3)*v + (c1*s2*s3-c3*s1)*w
    -s2*u + c2*s1*v + c1*c2*w 
    p + s1*t2*q + c1*t2*r 
    (c1*q - s1*r) 
    s1/(c2+eps)*q + c1/(c2+eps)*r];

U = sqrt(u^2+v^2+w^2); % speed

end











