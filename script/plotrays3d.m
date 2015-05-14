clc;
clear all;
close all;

angles = 10:10:80;
frequencies = 5e6:0.5e6:5e6;
Radius = 3390;

load ../Debug/data.dat;
%data = data_IonosphereTest;

numRays = max(data(:,1));
numDataPoints = length(data);

handle = figure;
set(handle, 'Position', [100, 100, 1024, 800]);
  
legend(num2str(angles'));

x = zeros(length(angles), length(data));
z = zeros(length(angles), length(data));
h = zeros(length(angles), length(data));
for f=1:length(frequencies)
    for n=1:length(angles)
        xCur = data(abs(data(:,8) - deg2rad(angles(n))) < 0.1 & data(:,9) == frequencies(f), 2);
        x(n,1:length(xCur)) = xCur;
        hCur = data(abs(data(:,8) - deg2rad(angles(n))) < 0.1 & data(:,9) == frequencies(f), 3);
        h(n,1:length(hCur)) = hCur;
        zCur = data(abs(data(:,8) - deg2rad(angles(n))) < 0.1 & data(:,9) == frequencies(f), 4);
        z(n,1:length(zCur)) = zCur;
        %omega_p = data(:,3);
        %Ne = data(:,4);
        %mu_r_sqrt = data(:,5);
    end

    % Dont draw back to 0
    x(x == 0) = nan;
    h(h == 0) = nan;
    hs = subplot(length(frequencies),1,f);
    axis equal;
    
    plot3(x/1e3, z/1e3, h/1e3 - Radius, '*b')
    grid on
    xlabel('Longitude [km] (x)')
    ylabel('Latitude [km] (y)')
    zlabel('Altitude above pole [km] (z)')
    xlim([-100 1000])
    ylim([-500 500])
    zlim([-50 300])
    
    hold on
    angxy=-0.287+pi/2:0.001:0.03+pi/2;
    angyz=-0.12+pi/2:0.001:0.12+pi/2;
    plot3(0 + Radius*cos(angxy), zeros(length(angxy), 1), Radius * sin(angxy)-Radius, 'r', 'LineWidth', 2);
    plot3(zeros(length(angyz), 1), 0 + Radius*cos(angyz), Radius * sin(angyz)-Radius, 'r', 'LineWidth', 2);
    
    % Plot celestial sphere
    LOD = 270;
    [xs ys zs] = sphere(LOD);
    xs = xs * Radius;
    ys = ys * Radius;
    zs = zs * Radius - Radius;
    zs(find(zs<-100)) = NaN;
    xs(find(xs<-100)) = NaN;
    ys(find(ys<-300)) = NaN;
    ys(find(ys>300)) = NaN;
    
    C = zeros(LOD, LOD, 3);
    C(:,:,1) = 1;
    C(:,:,2) = 0;
    C(:,:,3) = 0;
    surf(xs, ys, zs, C);
    alpha(0.8);
    set(gca,'xdir','reverse')
    set(gca,'ydir','reverse')
end