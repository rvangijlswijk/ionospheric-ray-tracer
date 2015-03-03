clc;
clear all;
clf;

angles = 30:10:60;
frequencies = 5e6:0.05e6:5e6;
ang=0:0.01:2*pi;

load ../Debug/data.dat;
%data = data_IonosphereTest;

numRays = max(data(:,1));
numDataPoints = length(data);

% h1 = figure;
%plot(Ne,h)
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
    plot(0 + (3390+80)*cos(ang), (3390+80) * sin(ang)-3390, 'LineStyle', '--', 'Color', 'black')
    plot(0 + (3390+200)*cos(ang), (3390+200) * sin(ang)-3390, 'LineStyle', '--', 'Color', 'black')
    
    title(['frequency: ' num2str(frequencies(f)/1e6) ' MHz']);
    grid on
    xlabel('Cartesian distance [km]')
    ylabel('altitude [km]')
    xlim([-50 1000]);
    ylim([-100 250]);
end

plasmaFreq = data(data(:,3) ~= 0, 3);
plasmaFreq = plasmaFreq(1);
f_f0 = frequencies / (plasmaFreq / (2*pi));

%figure;
% hold on;
% max_x2 = zeros(length(angles),1);
% for a=1:length(angles)
%     x2 = zeros(length(frequencies), 1);
%     for n=1:length(frequencies)
%         xCur = data(abs(data(:,6) - deg2rad(angles(a))) < 0.01 & data(:,7) == frequencies(n), 1);
%         if length(xCur) > 0
%             x2(n, 1) = max(xCur);
%         else
%             x2(n, 1) = nan;
%         end
%     end
%     %plot(x2(:,1), f_f0);
%     max_x2(a,1) = max(x2);
% end

%figure;
% hold on
% for f=frequencies
%     mu = data(abs(data(:,6) - deg2rad(50)) < 0.01 & data(:,7) == f, 5);
%     mu2 = mu(mu > 0);
%     criticalAngle = zeros(length(mu2), 1);
%     for k=2:length(mu2)
%         if (mu2(k-1) <= mu2(k))
%             criticalAngle(k) = asin(mu2(k-1)/mu2(k));
%         else
%             criticalAngle(k) = asin(mu2(k)/mu2(k-1));
%         end
%     end
%     %plot(criticalAngle*180/pi)
% end



% s = zeros(numRays, numDataPoints);
% h2 = zeros(numRays, numDataPoints);
SZA = zeros(numRays, 1);
distanceFromTransmitter = zeros(numRays, 1);
powerAtDistance = zeros(numRays, 1);

nBins = 84;
dSza = 0.2; % deg
nBinScale = (1:nBins) .* dSza;
SzaBins = zeros(nBins, 20);

h3 = figure;
hold on;
for r = 1:numRays
    hCur = data(data(:,1) == r, 3);
    xCur = data(data(:,1) == r, 2);  
    sCur = data(data(:,1) == r, 9); 
    collision = data(data(:,1) == r, 10);
    pos = 1;
    for c = 1:length(collision)
        if collision(c) == 2 % 2 = Geometry::terrain
            SZA(r) = atan(xCur(c) / hCur(c));
            if rad2deg(SZA(r)) <= nBins * dSza
                distanceFromTransmitter(r) = SZA(r) * 3390e3;
                powerAtDistance(r) = sCur(c);
                allocatedBin = ceil(rad2deg(SZA(r))/dSza);
%                 SzaBins(allocatedBin) = SzaBins(allocatedBin) + powerAtDistance(r);
%                 plot(deg2rad(allocatedBin*dSza) * 3390, sCur(c), '*')
                for q = 1:20
                   if SzaBins(allocatedBin, q) == 0 
                       SzaBins(allocatedBin, q) = powerAtDistance(r);
                      break; 
                   end
                end
            end
        end
    end
end

% Figure out standarddeviation of signal strength
stddevs = zeros(nBins, 1);
means = zeros(nBins, 1);
pdfs = zeros(nBins, 20);
distances = deg2rad(nBinScale)*3390;
for s = 1:nBins
    test = SzaBins(s, :);
    test(test == 0) = [];
    stddevs(s) = std(test);
    means(s) = mean(test);
    pdfs(s, :) = pdf('norm', SzaBins(s,:), means(s), stddevs(s));
    if isempty(test) | stddevs(s) == 0
       distances(s) = nan; 
       means(s) = nan; 
       stddevs(s) = nan;       
    end
end
distances(isnan(distances)) = [];
means(isnan(means)) = [];
stddevs(isnan(stddevs)) = [];
pdfs = pdfs(~any(isnan(pdfs),2),:);

erb = errorbar(distances, means, stddevs, 'rx');
errorbarT(erb, 1, 1);
ylim([-20 0])
xlim([0 1200])
xlabel('Distance [km]');
ylabel('Attenuation [dB]');
grid on;

[p, S] = polyfit(distances, means', 3);
[meanY, delta] = polyconf(p, distances, S, 'alpha', 0.04);
hold on
plot(distances, meanY, 'b')
plot(distances, meanY+delta, '--')
plot(distances, meanY-delta, '--')

h4 = figure;
plot(distanceFromTransmitter/1e3, powerAtDistance, 'r*');
ylim([-12 0])
hold on;
grid on
plot(distances, meanY)
plot(distances, meanY+delta, '--')
plot(distances, meanY-delta, '--')
xlabel('distance [km]')
ylabel('Attenuation [dB]')

% Chebyshev's inequality 5su_d for 96%
su_d = delta / 5;
