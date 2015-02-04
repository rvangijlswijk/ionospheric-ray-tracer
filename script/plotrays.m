clc;
clear all;
close all;

angles = 60:10:60;
frequencies = 4e6:1e6:6e6;

load ../Debug/data.dat;
%data = data_IonosphereTest;
x = zeros(length(angles), length(data));
h = zeros(length(angles), length(data));
for n=1:length(angles)
    xCur = data(abs(data(:,6) - deg2rad(angles(n))) < 0.01 & data(:,7) == 5e6, 1);
    x(n,1:length(xCur)) = xCur;
    hCur = data(abs(data(:,6) - deg2rad(angles(n))) < 0.01 & data(:,7) == 5e6, 2);
    h(n,1:length(hCur)) = hCur;
    %omega_p = data(:,3);
    %Ne = data(:,4);
    %mu_r_sqrt = data(:,5);
end

% Dont draw back to 0
x(x == 0) = nan;
h(h == 0) = nan;

figure;
plot(x'/1000,h'/1000)
axis equal
%plot(Ne,h)
grid on
xlabel('distance [km]')
ylabel('altitude [km]')
xlim([0 1000]);
ylim([3390 - 100 3390 + 250]);

% Plot mars
hold on;
ang=0:0.01:2*pi;
plot(0 + 3390*cos(ang), 3390 * sin(ang), 'r')

plasmaFreq = data(data(:,3) ~= 0, 3);
plasmaFreq = plasmaFreq(1);
f_f0 = frequencies / (plasmaFreq / (2*pi));

%figure;
hold on;
max_x2 = zeros(length(angles),1);
for a=1:length(angles)
    x2 = zeros(length(frequencies), 1);
    for n=1:length(frequencies)
        xCur = data(abs(data(:,6) - deg2rad(angles(a))) < 0.01 & data(:,7) == frequencies(n), 1);
        if length(xCur) > 0
            x2(n, 1) = max(xCur);
        else
            x2(n, 1) = nan;
        end
    end
    %plot(x2(:,1), f_f0);
    max_x2(a,1) = max(x2);
end
