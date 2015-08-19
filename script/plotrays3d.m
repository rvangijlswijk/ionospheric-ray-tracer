clc;
clear all;
% close all;

angles = 15:15:75;
frequencies = 5e6:0.5e6:5e6;

% constants
radius = 3390e3;
radiusKm = radius / 1e3;

% column definitions
colX = 2;
colY = 3;
colZ = 4;
colOmega_p = 5;
colN_e = 6;
colMu_r_sqrt = 7;
colTheta0 = 8;
colFrequency = 9;
colSignalPower = 10;
colTimeOfFlight = 11;
colCollisionType = 12;
colBeaconId = 13;
colAzimuth_0 = 14;

load ../Debug/data.dat;
%data = data_IonosphereTest;

numRays = max(data(:,1));
numDataPoints = length(data);

handle = figure;
set(handle, 'Position', [100, 100, 1024, 800]);
  
legend(num2str(angles'));

for f=1:length(frequencies)
    x = zeros(length(angles), length(data));
    z = zeros(length(angles), length(data));
    h = zeros(length(angles), length(data));
    for n=1:length(angles)
        xCur = data(abs(data(:,colTheta0) - deg2rad(angles(n))) < 0.1 & data(:,colFrequency) == frequencies(f), colX);
        x(n,1:length(xCur)) = xCur;
        yCur = data(abs(data(:,colTheta0) - deg2rad(angles(n))) < 0.1 & data(:,colFrequency) == frequencies(f), colY);
        h(n,1:length(yCur)) = yCur;
        zCur = data(abs(data(:,colTheta0) - deg2rad(angles(n))) < 0.1 & data(:,colFrequency) == frequencies(f), colZ);
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
    
    plot3(x/1e3, z/1e3, h/1e3 - radiusKm, '.','MarkerSize', 5,  'Color', [0.5 0.5 0.5])
    grid on
    xlabel('Longitude [km] (x)')
    ylabel('Latitude [km] (y)')
    zlabel('Altitude above pole [km] (z)')
    xlim([-500 500])
    ylim([-500 500])
    zlim([-50 150])
    
    hold on
    angxy=-0.18+pi/2:0.001:0.18+pi/2;
    angyz=-0.18+pi/2:0.001:0.18+pi/2;
    plot3(0 + radiusKm*cos(angxy), zeros(length(angxy), 1), radiusKm * sin(angxy)-radiusKm, 'r', 'LineWidth', 2);
    plot3(zeros(length(angyz), 1), 0 + radiusKm*cos(angyz), radiusKm * sin(angyz)-radiusKm, 'r', 'LineWidth', 2);
    
    % Plot celestial sphere
    LOD = 270;
    [xs ys zs] = sphere(LOD);
    xs = xs * radiusKm;
    ys = ys * radiusKm;
    zs = zs * radiusKm - radiusKm;
    zs(find(zs<-50)) = NaN;
    %xs(find(xs<-100)) = NaN;
    ys(find(ys<-500)) = NaN;
    ys(find(ys>500)) = NaN;
    
    C = zeros(LOD, LOD, 3);
    C(:,:,1) = 1;
    C(:,:,2) = 0;
    C(:,:,3) = 0;
    surf(xs, ys, zs, C);
    alpha(0.8);
    set(gca,'xdir','reverse')
    set(gca,'ydir','reverse')
end