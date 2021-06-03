%% 参数指定
% jcloud 和 se 通讯服务器的IP
param_jcloud_ip = '[10,119,6,245]';
param_se_ip     = '[202,120,40,8]';

%% 使用函数生成模型
%% jcloud
% jcloud kine
instance_generate('model_kinematics', 'jcloud', param_jcloud_ip, 2, 'general');
% jcloud target
instance_generate('model_target', 'jcloud', param_jcloud_ip, 2, 'static');

%% se
%% se kine
instance_generate('model_kinematics', 'se', param_se_ip, 2, 'general');
%% se target
instance_generate('model_target', 'se', param_se_ip, 2, 'static');

%% 定义模型生成函数
function instance_generate(model_name, location, ip_addr, count, tag_str)
    %% 参数表
    arguments
        % 使用的模型名称
        model_name (1,:) char {mustBeMember(model_name, {'model_kinematics', 'model_target'})}
        % 生成模型对应的服务器位置
        location (1,:) char {mustBeMember(location, {'jcloud', 'se'})}
        % 生成模型对应的服务器IP，实际上与位置绑定
        ip_addr  (1,:) char
        % 生成模型数量
        count    (1,1) int32
        % 生成的该模型子标签，可选
        tag_str  (1,:) char {mustBeMember(tag_str,  {'general', 'static'})} = 'general'
    end
    % 函数功能：生成模型实例
    
    %% 函数代码体 - 准备阶段
    fprintf('*****位于 %s 的 %s_%s 模型生成准备中，数量 %d 个*****\n', location, tag_str, model_name, count);
    % 载入模型
    model = load_system(model_name);
    % 设置模型 solver
    set_param(model_name, 'StopTime', 'inf');
    set_param(model_name, 'Solver', 'FixedStepDiscrete');
    set_param(model_name, 'FixedStep', '100');
    % 确保模型内的模块是启用状态，即注释commented 是 off
    switch model_name
        case 'model_kinematics'
            % 运动学模型机生成，运动学模块启用
            set_param([model_name, '/sfunc_kine_recv'],   'commented', 'off');
            set_param([model_name, '/sfunc_kine_gen'],    'commented', 'off');
            set_param([model_name, '/sfunc_kine_send'],   'commented', 'off');
            % type_str 用于生成模型的名称字段
            type_str = 'kine';
        case 'model_target'
            % 靶机生成，靶机模块启用
            set_param([model_name, '/sfunc_target_recv'], 'commented', 'off');
            set_param([model_name, '/sfunc_target_gen'],  'commented', 'off');
            set_param([model_name, '/sfunc_target_send'], 'commented', 'off');
            % type_str 用于生成模型的名称字段
            type_str = 'target';
        otherwise
            disp('model_name ERROR');
            return;
    end
    
    % 使用 type_str 确定对应的 role_type(int32)
    % role_type 用于模型内部 TCP 模块与通讯服务器交互时确认模型角色类型
    switch type_str
        case 'target'
            role_type = int32(0);
        case 'kine'
            role_type = int32(1);
        otherwise
            disp('type_str ERROR');
            return;
    end
    % 使用参数 tag_str 确定对应的 role_tag(int32)
    % role_tag 用于模型内部确认该模型的具体动作
    % 目前只有 靶机（target）有静止（static）这一标签
    switch tag_str
        case 'general'
            role_tag = int32(0);
        case 'static'
            role_tag = int32(1);
        otherwise
            disp('tag_str ERROR');
            return;
    end
    
    % 直接生成的模型名称
    gen_name = strcat(model_name, '.exe');
    
    %% 函数代码体 - 循环生成阶段
    for c = 1 : count
        % 生成实例名，如 jcloud_general_kine_1001.exe，其中
        %   location = jcloud
        %   tag_str  = general
        %   type_str = kine
        %   role_type = 1
        %   role_id   = [role_type, role_count] = [1, 001], role_count 为三位数
        assign_name = sprintf('%s_%s_%s_%d%03d.exe', location, tag_str, type_str, role_type, c);
        fprintf('*****模型 %s 生成开始*****\n', assign_name);
        % 修改 simulink 模型回调函数 PreSaveFcn 以修改模块参数
        % 参数包括 
        %   连接的服务器IP(ip_addr)，
        %   实例类型(role_type)，
        %   实例类型标签(role_tag)，
        %   实例ID(role_id，实例ID第一个数字表征实例类型，便于通讯服务器识别)
        param_str = sprintf('ip_addr = %s; role_type = %d; role_tag = %02d; role_id = %d%03d;', ip_addr, role_type, role_tag, role_type, c);
        fprintf('模型 PreSaveFcn: %s\n', param_str);
        set_param(model_name, 'PreSaveFcn', param_str);
        % 保存模型以调用 PreSaveFcn 修改模块参数
        save_system(model_name);
        
        % Simulink Coder 代码生成并编译可执行文件
        rtwbuild(model);
        
        % 若生成实例的目标文件名在模型文件夹（models）中存在（旧版）则删除之
        dest_name = strcat('.\models\', assign_name);
        % 将生成的模型文件移动到模型文件夹中，同时更名为 assign_name
        movefile(gen_name, dest_name);
        
        fprintf('*****模型 %s 生成结束*****\n', assign_name);
    end
    
    fprintf('*****位于 %s 的 %s_%s 模型生成完成，数量 %d 个*****\n', location, tag_str, model_name, count);
end
