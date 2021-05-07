%% 模型载入
model = load_system('target_drone');

%% 参数指定
% 编译直接生成的可执行文件名
gen_name = 'target_drone.exe';
param_jcloud_ip = '[10,119,6,245]';

%% 静止靶机生成，静止靶机模块保留
set_param('target_drone/sfunc_target_recv', 'commented', 'off');
set_param('target_drone/sfunc_target_gen',  'commented', 'off');
set_param('target_drone/sfunc_target_send', 'commented', 'off');
set_param('target_drone/sfunc_kine_recv',   'commented', 'on' );
set_param('target_drone/sfunc_kine_gen',    'commented', 'on' );
set_param('target_drone/sfunc_kine_send',   'commented', 'on' );

%% Jcloud 静止靶机生成，具体实例
% 生成实例数量
instance_count = 2;
for c = 1 : instance_count
    % 生成实例名，如 jcloud_static_target_0000.exe
    assign_name = sprintf('jcloud_static_target_%04d.exe', c);
    fprintf('*****\n静止靶机 %s 生成中*****\n', assign_name);
    % 修改 simulink 模型回调函数 PreSaveFcn 以修改模块参数
    % 参数包括 连接的服务器IP(ip_addr)，实例类型(role_type)，实例类型标签(role_tag)，实例ID(role_id)
    param_str = sprintf('ip_addr = %s; role_type = 0; role_tag = 00; role_id = %04d;', param_jcloud_ip, c);
    set_param(model, 'PreSaveFcn', param_str);
    % 保存模型以调用 PreSaveFcn
    save_system;
    % Simulink Coder 代码生成并编译可执行文件
    rtwbuild(model);
    % 若生成实例的目标文件名存在（旧版）则删除之
    if isfile(assign_name) == 1
        delete(assign_name);
        fprintf('%s 旧版已删除\n', assign_name);
    end
    % 调用系统接口重命名生成的可执行文件
    eval(['!rename' 32 gen_name 32 assign_name]);
    fprintf('*****静止靶机 %s 生成结束*****\n', assign_name);
end

%% 运动学模型机生成，运动学模块保留
set_param('target_drone/sfunc_target_recv', 'commented', 'on' );
set_param('target_drone/sfunc_target_gen',  'commented', 'on' );
set_param('target_drone/sfunc_target_send', 'commented', 'on' );
set_param('target_drone/sfunc_kine_recv',   'commented', 'off');
set_param('target_drone/sfunc_kine_gen',    'commented', 'off');
set_param('target_drone/sfunc_kine_send',   'commented', 'off');

%% Jcloud 运动学模型机生成，具体实例
instance_count = 2;
for c = 1 : instance_count
    % 生成实例名，如 jcloud_kine_1000.exe
    assign_name = sprintf('jcloud_kine_1%03d.exe', c);
    fprintf('*****\n运动学模型机 %s 生成中*****\n', assign_name);
    % 修改 simulink 模型回调函数 PreSaveFcn 以修改模块参数
    % 参数包括 连接的服务器IP(ip_addr)，实例类型(role_type)，实例类型标签(role_tag)，实例ID(role_id)
    param_str = sprintf('ip_addr = %s; role_type = 1; role_tag = 00; role_id = 1%03d;', param_jcloud_ip, c);
    set_param(model, 'PreSaveFcn', param_str);
    % 保存模型以调用 PreSaveFcn
    save_system;
    % Simulink Coder 代码生成并编译可执行文件
    rtwbuild(model);
    % 若生成实例的目标文件名存在（旧版）则删除之
    if isfile(assign_name) == 1
        delete(assign_name);
        fprintf('%s 旧版已删除\n', assign_name);
    end
    % 调用系统接口重命名生成的可执行文件
    eval(['!rename' 32 gen_name 32 assign_name]);
    fprintf('*****运动学模型机 %s 生成结束*****\n', assign_name);
end
