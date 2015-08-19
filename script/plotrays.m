clc;
clear all;
close all;

angles = 0:2:75;
frequencies = 2.5e6:1e6:2.5e6;
ang=0:0.01:2*pi;

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

numRays = max(data(:,1));
numDataPoints = length(data);

% calculate plasma frequency
% plasmaFreq = max(data(data(:, colOmega_p) ~= 0, colOmega_p)) / (2*pi);

%% Plotting rays
figure;
for f=1:length(frequencies)
    x = zeros(numRays, length(data));
    h = zeros(numRays, length(data));
    for n=1:numRays
        xCur = data(data(:,1) == n ...
            & data(:,colFrequency) == frequencies(f) & data(:, colCollisionType) ~= 5, colX);
        x(n,1:length(xCur)) = xCur;
        hCur = data(data(:,1) == n ...
            & data(:,colFrequency) == frequencies(f) & data(:, colCollisionType) ~= 5, colY);
        h(n,1:length(hCur)) = hCur;
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
    plot(x'/1000,h'/1000-3390 ,'Color', [0.5 0.5 0.5])
    plot(0 + 3390*cos(ang), 3390 * sin(ang)-3390, 'k', 'LineWidth', 2)
    
%     title(['frequency: ' num2str(frequencies(f)/1e6) ' MHz']);
    grid on
    xlabel('Cartesian distance [km]')
    ylabel('altitude [km]')
    xlim([-50 600]);
    ylim([-50 175]);

%     f_f0 = plasmaFreq / frequencies(f);
%     irisAngle = asin(sqrt(1 - f_f0^2));
%     fprintf('Iris angle for f=%2.1f MHz: %4.2f\n', frequencies(f)/1e6, rad2deg(irisAngle));

%     line([0 250*tan(irisAngle)],[0, 250], 'LineStyle', '--', 'Color', 'black');
end

SZA = zeros(numRays, 1);
distanceFromTransmitter = zeros(numRays, 1);
powerAtDistance = zeros(numRays, 1);

nBins = 84;
dSza = 0.2; % deg
nBinScale = (1:nBins) .* dSza;
SzaBins = zeros(nBins, 20);

% h3 = figure;
% % hold on;
% for r = 1:numRays
%     hCur = data(data(:,1) == r, 3);
%     xCur = data(data(:,1) == r, 2);  
%     sCur = data(data(:,1) == r, 9); 
%     collision = data(data(:,1) == r, 10);
%     pos = 1;
%     for c = 1:length(collision)
%         if collision(c) == 2 % 2 = Geometry::terrain
%             SZA(r) = atan(xCur(c) / hCur(c));
%             if rad2deg(SZA(r)) <= nBins * dSza
%                 distanceFromTransmitter(r) = SZA(r) * 3390e3;
%                 powerAtDistance(r) = sCur(c);
%                 allocatedBin = ceil(rad2deg(SZA(r))/dSza);
% %                 SzaBins(allocatedBin) = SzaBins(allocatedBin) + powerAtDistance(r);
% %                 plot(deg2rad(allocatedBin*dSza) * 3390, sCur(c), '*')
%                 for q = 1:20
%                    if SzaBins(allocatedBin, q) == 0 
%                        SzaBins(allocatedBin, q) = powerAtDistance(r);
%                       break; 
%                    end
%                 end
%             end
%         end
%     end
% end