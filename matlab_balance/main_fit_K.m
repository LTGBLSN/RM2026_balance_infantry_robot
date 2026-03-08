%% 主程序：计算不同腿长下的K矩阵并进行多项式拟合
clear; clc;
tic

% --- 第一步：获取模型函数句柄 ---
[fA_handle, fB_handle] = get_model_handles();

% --- 第二步：参数初始化 ---
leg = 0.1:0.01:0.4;  
num_steps = length(leg);

% 定义存储数组，确保逻辑与你原始代码完全一致
k11 = zeros(1, num_steps); k12 = zeros(1, num_steps); k13 = zeros(1, num_steps);
k14 = zeros(1, num_steps); k15 = zeros(1, num_steps); k16 = zeros(1, num_steps);
k21 = zeros(1, num_steps); k22 = zeros(1, num_steps); k23 = zeros(1, num_steps);
k24 = zeros(1, num_steps); k25 = zeros(1, num_steps); k26 = zeros(1, num_steps);

Ts = 0.001; 
%步兵较软参数 起立参数
%Q_theta     = 500;
%Q_theta_dot = 100;%防止腿部关节过冲
%Q_x         = 500;
%Q_x_dot     = 50;
%Q_phi       = 40000;
%Q_phi_dot   = 10;
%Q=diag([Q_theta Q_theta_dot Q_x Q_x_dot Q_phi Q_phi_dot]);
%R=diag([240 12]);  

% Q_theta     = 3000;
% Q_theta_dot = 150;%防止腿部关节过冲
% Q_x         = 500;
% Q_x_dot     = 400;
% Q_phi       = 80000;
% Q_phi_dot   = 10;
% Q=diag([Q_theta Q_theta_dot Q_x Q_x_dot Q_phi Q_phi_dot]);
% R=diag([300 8]);  

%Q_theta     = 3000;
%Q_theta_dot = 150;%防止腿部关节过冲
%Q_x         = 500;
%Q_x_dot     = 400;
%Q_phi       = 150000;
%Q_phi_dot   = 10;
%Q=diag([Q_theta Q_theta_dot Q_x Q_x_dot Q_phi Q_phi_dot]);
%R=diag([300 8]); 

%这个是之前拍视频还算不错的参数，但是腿可能有点过冲
%Q = diag([320, 2500, 1200, 500, 200000, 1])
%R = [40 0; 0 10]


%这个是调高了PITCH，腿的过冲稍微小了一点，但是抖动变大了
%Q = diag([320, 4000, 1200, 500, 600000, 1])
%R = [30 0; 0 10]

Q = diag([320, 4500, 1200, 500, 200000, 1])
R = [40 0; 0 10]
fprintf('开始数值循环计算 K 矩阵...\n');

% --- 第三步：数值计算循环 ---
for j = 1:num_steps
    L_curr = leg(j);
    
    % 使用句柄计算数值 A 和 B
    A = fA_handle(L_curr);
    B = fB_handle(L_curr);
    
    % 离散化 (与 get_k_length 逻辑完全一致)
    sys_c = ss(A, B, eye(6), 0);
    sys_d = c2d(sys_c, Ts, 'zoh'); 
    Ad = sys_d.A;
    Bd = sys_d.B;

    % 求解离散 LQR
    [K, ~, ~] = dlqr(Ad, Bd, Q, R);
    
    % --- 重点：采用最原始的赋值方式，防止索引出错 ---
    k11(j) = K(1,1); k12(j) = K(1,2); k13(j) = K(1,3);
    k14(j) = K(1,4); k15(j) = K(1,5); k16(j) = K(1,6);

    k21(j) = K(2,1); k22(j) = K(2,2); k23(j) = K(2,3);
    k24(j) = K(2,4); k25(j) = K(2,5); k26(j) = K(2,6);

    if mod(j, 10) == 0
        fprintf('进度: leg_length = %.2f\n', L_curr);
    end
end

% --- 第四步：多项式拟合 ---
a11=polyfit(leg,k11,3); a12=polyfit(leg,k12,3); a13=polyfit(leg,k13,3);
a14=polyfit(leg,k14,3); a15=polyfit(leg,k15,3); a16=polyfit(leg,k16,3);

a21=polyfit(leg,k21,3); a22=polyfit(leg,k22,3); a23=polyfit(leg,k23,3);
a24=polyfit(leg,k24,3); a25=polyfit(leg,k25,3); a26=polyfit(leg,k26,3);

fprintf('K矩阵计算完毕！总耗时：\n');
toc

% --- 第五步：显示多项式方程 (改回 t3, t2, t1 格式) ---
fprintf('\n--- 拟合多项式结果 ---\n');
display_polynomial(a11, 'k[0][0]');
display_polynomial(a12, 'k[0][1]');
display_polynomial(a13, 'k[0][2]');
display_polynomial(a14, 'k[0][3]');
display_polynomial(a15, 'k[0][4]');
display_polynomial(a16, 'k[0][5]');
fprintf('\n');
display_polynomial(a21, 'k[1][0]');
display_polynomial(a22, 'k[1][1]');
display_polynomial(a23, 'k[1][2]');
display_polynomial(a24, 'k[1][3]');
display_polynomial(a25, 'k[1][4]');
display_polynomial(a26, 'k[1][5]');

% ===================== 内部函数 =====================
function display_polynomial(coefficients, name)
    % 恢复原来的 t%d 输出格式
    equation = sprintf('%s = ', name);
    n = length(coefficients);
    for i = 1:n
        if coefficients(i) ~= 0
            if i == n
                term = sprintf('%.4ff', coefficients(i));
            else
                term = sprintf('%.4ff*t%d', coefficients(i), n-i);
            end
            if i > 1 && coefficients(i) > 0
                term = [' + ', term];
            end
            equation = [equation, term];
        end
    end
    fprintf('%s;\n', equation);
end