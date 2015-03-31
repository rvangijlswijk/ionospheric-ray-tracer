clc;
clear all;
close all;

angles = 10:0.05:80;
frequencies = 5e6:0.05e6:5e6;
ang=0:0.01:2*pi;

data = csvread('../Debug/data.dat', 0, 0, 'A1..E694');

numRays = max(data(:,1));
numDataPoints = length(data);

% h1 = figure;
legend(num2str(angles'));

h = zeros(numRays, length(data));
Ne = zeros(numRays, length(data));

for r=1:numRays
   xCur = data(data(:,1) == r, 2);
   yCur = data(data(:,1) == r, 3);
    h(r,1:length(yCur)) = sqrt(xCur.^2 + yCur.^2) - 3390e3;
   NeCur = data(data(:,1) == r, 5);
   Ne(r, 1:length(NeCur)) = NeCur;
end

plot(Ne, h/1e3, 'rx');
grid on