clc
T=readtable("serial_data.csv");
x=T.Time;
x=x';
y=y';
y=T.Value;
plot(x,y);
grid on;
[pks, locs] = findpeaks(y,x);
periods = diff(locs); 
average_period = mean(periods);
frequency = 1 / average_period;