% Parameters
T_ambient = 25;    % Ambient temperature (C)
P_comp = 500;      % Compressor cooling power (W)
C_p = 1000;        % Effective heat capacity (J/C)
tau = 300;         % Time constant (s)
delta_t = 1;       % Sampling time (s)
sim_time = 3600;   % Total simulation time (s)

% Initialize
T = zeros(1, sim_time); % Temperature array
T(1) = 5;               % Initial temperature (C)
u = zeros(1, sim_time); % Control input array (0 = off, 1 = on)

% Example control sequence (turn on and off at specific intervals)
u(1:600) = 1;  % Turn on compressor for 10 minutes
u(601:1200) = 0; % Turn off for 10 minutes
% Continue pattern...

% Simulation loop
for k = 1:sim_time-1
    T(k+1) = T(k) + delta_t * ( -(1/tau) * (T(k) - T_ambient) - (P_comp/C_p) * u(k) );
end

% Plotting
time = 0:delta_t:(sim_time-1)*delta_t;
plot(time, T);
xlabel('Time (s)');
ylabel('Temperature (C)');
title('Temperature Response of Refrigeration System');
grid on;
