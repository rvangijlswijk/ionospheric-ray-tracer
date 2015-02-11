clc;
clear all;
close all;

angles = 20:20:60;
frequencies = 4e6:1e6:5e6;
ang=0:0.01:2*pi;

load ../Debug/data.dat;
%data = data_IonosphereTest;

numRays = max(data(:,1));

figure;
%plot(Ne,h)
legend(num2str(angles'));

x = zeros(length(angles), length(data));
h = zeros(length(angles), length(data));
s = zeros(numRays, length(data));
h2 = zeros(numRays, length(data));
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
    plot(x'/1000,h'/1000, 'Color', [0.5 0.5 0.5])
    plot(0 + 3390*cos(ang), 3390 * sin(ang), 'r', 'LineWidth', 2)
    plot(0 + (3390+80)*cos(ang), (3390+80) * sin(ang), 'LineStyle', '--', 'Color', 'black')
    plot(0 + (3390+200)*cos(ang), (3390+200) * sin(ang), 'LineStyle', '--', 'Color', 'black')
    title(['frequency: ' num2str(frequencies(f)/1e6) ' MHz']);
    grid on
    %xlabel('distance [km]')
    ylabel('altitude [km]')
    xlim([-50 1000]);
    ylim([3390 - 100 3390 + 250]);
end


% Plot mars
% plot(0 + 3390*cos(ang), 3390 * sin(ang), 'r')
% plot(0 + (3390+80)*cos(ang), (3390+80) * sin(ang), 'LineStyle', '--', 'Color', 'black')
% plot(0 + (3390+200)*cos(ang), (3390+200) * sin(ang), 'LineStyle', '--', 'Color', 'black')

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
hold on
for f=frequencies
    mu = data(abs(data(:,6) - deg2rad(50)) < 0.01 & data(:,7) == f, 5);
    mu2 = mu(mu > 0);
    criticalAngle = zeros(length(mu2), 1);
    for k=2:length(mu2)
        if (mu2(k-1) <= mu2(k))
            criticalAngle(k) = asin(mu2(k-1)/mu2(k));
        else
            criticalAngle(k) = asin(mu2(k)/mu2(k-1));
        end
    end
    %plot(criticalAngle*180/pi)
end

figure;
hold on;
for r = 1:numRays   
    sCur = data(data(:,1) == r, 9);
    hCur = data(data(:,1) == r, 3);
    s(r,1:length(sCur)) = sCur;
    h2(r,1:length(sCur)) = hCur;
    sCur(sCur == 0) = nan;
    plot(-sCur, (hCur - 3390e3)/1000);
end
xlabel('Cumulative loss [dB]');
ylabel('Altitude [km]');