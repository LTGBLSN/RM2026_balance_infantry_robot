function [fA, fB] = get_model_handles()
    % 该函数执行一次性符号推导，生成 A 和 B 矩阵关于腿长 leg_val 的快速数值句柄
    
    fprintf('正在进行符号推导（仅需执行一次）... ');
    
    % 定义符号变量
    syms x(t) T R_sym Iw_sym mw_sym M_sym L_sym LM_sym theta(t) l_sym phi(t) mp_sym g_sym Tp Ip_sym IM_sym
    syms f1 f2 f3 d_theta d_x d_phi theta0 x0 phi0 leg_val

    % --- 物理参数定义 (根据你提供的数据) ---
    R1 = 0.06;         % 驱动轮半径
    l1 = 0.03;        % 机体质心距离转轴距离
    mw1 = 0.2;       % 驱动轮质量
    %mp1 = 2.034;       % 杆质量单腿
    mp1 = 1.0;       % 杆质量单腿
    M1 = 8.0;         % 机体质量(18.5/2)
    IM1 = 0.20547;     % 机体绕质心转动惯量
    g1 = 9.8;          % 重力加速度

    % --- 随腿长 leg_val 变化的参数 ---
    L1 = leg_val/2;                  % 摆杆重心到驱动轮轴距离
    LM1 = leg_val/2;                 % 摆杆重心到其转轴距离
    Iw1 = mw1 * R1^2;                % 驱动轮转动惯量
    Ip1 = mp1 * ((L1 + LM1)^2 + 0.05^2) / 12.0; % 摆杆转动惯量

    % --- 动力学方程推导 ---
    % 计算力项
    NM = M_sym * diff(x + (L_sym + LM_sym)*sin(theta) - l_sym*sin(phi), t, 2);
    N  = NM + mp_sym * diff(x + L_sym*sin(theta), t, 2);
    PM = M_sym * g_sym + M_sym * diff((L_sym + LM_sym)*cos(theta) + l_sym*cos(phi), t, 2);
    P  = PM + mp_sym * g_sym + mp_sym * diff(L_sym*cos(theta), t, 2);

    % 三个广义坐标的方程
    eqn1 = diff(x, t, 2) == (T - N * R_sym) / (Iw_sym / R_sym + mw_sym * R_sym);
    eqn2 = Ip_sym * diff(theta, t, 2) == (P * L_sym + PM * LM_sym) * sin(theta) - (N * L_sym + NM * LM_sym) * cos(theta) - T + Tp;
    eqn3 = IM_sym * diff(phi, t, 2) == Tp + NM * l_sym * cos(phi) + PM * l_sym * sin(phi);

    % 符号替换，方便使用 solve
    % 将 diff(theta,t,2) 换成 f1，将 theta(t) 换成 theta0 等
    old_vars = [diff(theta,t,2), diff(x,t,2), diff(phi,t,2), diff(theta,t), diff(x,t), diff(phi,t), theta, x, phi];
    new_vars = [f1, f2, f3, d_theta, d_x, d_phi, theta0, x0, phi0];
    
    eqn10 = subs(eqn1, old_vars, new_vars);
    eqn20 = subs(eqn2, old_vars, new_vars);
    eqn30 = subs(eqn3, old_vars, new_vars);

    % 求解加速度项 f1, f2, f3
    [sol1, sol2, sol3] = solve(eqn10, eqn20, eqn30, f1, f2, f3);
   
    % --- 线性化构建 A 和 B 矩阵 ---
    % 状态向量: [theta, d_theta, x, d_x, phi, d_phi]
    state_vars = [theta0, d_theta, x0, d_x, phi0, d_phi];
    input_vars = [T, Tp];
    eqs = [d_theta, sol1, d_x, sol2, d_phi, sol3];
    
    % 计算雅可比矩阵
    A_sym = jacobian(eqs, state_vars);
    B_sym = jacobian(eqs, input_vars);

    % 代入物理常数
    A_sub = subs(A_sym, [R_sym, L_sym, LM_sym, l_sym, mw_sym, mp_sym, M_sym, Iw_sym, Ip_sym, IM_sym, g_sym], ...
                       [R1, L1, LM1, l1, mw1, mp1, M1, Iw1, Ip1, IM1, g1]);
    B_sub = subs(B_sym, [R_sym, L_sym, LM_sym, l_sym, mw_sym, mp_sym, M_sym, Iw_sym, Ip_sym, IM_sym, g_sym], ...
                       [R1, L1, LM1, l1, mw1, mp1, M1, Iw1, Ip1, IM1, g1]);

    % 在平衡点 (0,0,0,0,0,0) 处计算数值
    A_final = subs(A_sub, [state_vars, input_vars], zeros(1, 8));
    B_final = subs(B_sub, [state_vars, input_vars], zeros(1, 8));

    % 生成数值函数句柄，输入参数为 leg_val
    fA = matlabFunction(A_final, 'Vars', {leg_val});
    fB = matlabFunction(B_final, 'Vars', {leg_val});
    
    fprintf('完成。\n');
end