clc;
clear all;
close all;

angles = 10:5:80;
frequencies = 5e6:0.5e6:5e6;
ang=0:0.01:2*pi;

load ../Debug/data.dat;
%data = data_IonosphereTest;

numRays = max(data(:,1));
numDataPoints = length(data);

handle = figure;
set(handle, 'Position', [100, 100, 1024, 800]);
  
legend(num2str(angles'));

x = zeros(length(angles), length(data));
h = zeros(length(angles), length(data));
for f=1:length(frequencies)
    for n=1:length(angles)
        xCur = data(abs(data(:,7) - deg2rad(angles(n))) < 0.01 & data(:,8) == frequencies(f), 2);
        x(n,1:length(xCur)) = xCur;
        hCur = data(abs(data(:,7) - deg2rad(angles(n))) < 0.01 & data(:,8) == frequencies(f), 3);
        h(n,1:length(hCur)) = hCur;
        %omega_p = data(:,3);
        %Ne = data(:,4);
        %mu_r_sqrt = data(:,5);
    end

    % Dont draw back to 0
    x(x == 0) = nan;
    h(h == 0) = nan;
    hs = subplot(length(frequencies),1,f);
    axis equal;
     p = get(hs, 'pos');
     p(4) = p(4) + 0.02;
     set(hs, 'pos', p);
    hold on
    plot(x'/1000,h'/1000-3390, 'Color', [0.5 0.5 0.5])
    plot(0 + 3390*cos(ang), 3390 * sin(ang)-3390, 'r', 'LineWidth', 2)
    plot(0 + (3390+70)*cos(ang), (3390+70) * sin(ang)-3390, 'LineStyle', '--', 'Color', 'black')
    plot(0 + (3390+108)*cos(ang), (3390+108) * sin(ang)-3390, 'LineStyle', '--', 'Color', 'black')
    plot(0 + (3390+200)*cos(ang), (3390+200) * sin(ang)-3390, 'LineStyle', '--', 'Color', 'black')
    
    title(['frequency: ' num2str(frequencies(f)/1e6) ' MHz']);
    grid on
    xlabel('Cartesian distance [km]')
    ylabel('altitude [km]')
    xlim([-50 1000]);
    ylim([-100 250]);
end