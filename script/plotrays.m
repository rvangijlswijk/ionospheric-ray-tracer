clc;
clear all;
close all;

numRays = 5;

load ../Debug/data.dat;
%data = data_IonosphereTest;
x = zeros(numRays, length(data));
h = zeros(numRays, length(data));
for n=1:numRays
    xCur = data(abs(data(:,6) - deg2rad(15*n)) < 0.01, 1);
    x(n,1:length(xCur)) = xCur;
    hCur = data(abs(data(:,6) - deg2rad(15*n)) < 0.01, 2);
    h(n,1:length(hCur)) = hCur;
    %omega_p = data(:,3);
    %Ne = data(:,4);
    %mu_r_sqrt = data(:,5);
end

% Dont draw back to 0
x(x == 0) = nan;
h(h == 0) = nan;

plot(x'/1000,h'/1000)
axis equal
%plot(Ne,h)
grid on
xlabel('distance [km]')
ylabel('altitude [km]')
xlim([0 1000]);
ylim([0 250]);