
%%Calculating the geometry of the 2V geodesic dome
clear; clc; close all; 

format long

% golden ratio
GR = (1+sqrt(5))/2; 

radius = 1500; 
theta = 36; %The base of the geodesic dome is decagon so that the angle ...
             ... made by base strut at the centre is 36 degree.

% using the law of cosine to find strut length B 
B = sqrt(2*radius^2-2*radius^2*cosd(36))

% calculating the distance to the center of the pentagon
% find the sidelength of the regular icosahedron middle of pentagon to
% middle of pentagon
kantLengde = ((2*radius)/sqrt(GR^2+1)); 
% use trigonometry to find angle fram origo to the icosahedron side g
% and to find the length of A
theta = asind((kantLengde/2)/radius);
utstikkerFraIkosahedron = radius-(radius*cosd(theta)); 
A = sqrt(utstikkerFraIkosahedron^2+(kantLengde/2)^2)

vinkelForA = atand(utstikkerFraIkosahedron/(kantLengde/2))
vinkelForB = 360/20

% The values have been checked and veryfied with the chord factors given on
% the internet for a 2v geodesic dome. 

% kilde: https://en.wikipedia.org/wiki/Regular_icosahedron
% Kilde: https://drishti-svnit.github.io/CIVIL/documentation/geodesic%20dome%20(1)-converted.pdf
% Kilde: http://hilaroad.com/camp/projects/dome/dome1.html


%vinkel mellom bakke og planet til pentagon
theta = asind(1275.976212/1426.584774)


length_A_total=30*A %mm
length_B_total=25*B %mm
length_total=length_A_total+length_B_total %mm


%% Calculating the right TX Power for LoRa
clear; clc; close all; 



ERPmW = 10; %dBm
antennaGain = 0.7 - 2.15; %dBd
attenuationFactor = 93/100; %dB/m 
cableLength = 0.15; %m 
cableLoss = attenuationFactor * cableLength; %dB

txPower = ERPmW-antennaGain+cableLoss

%% Plotting camera positions from geodome

clear; clc; close all; 

positionCordinates = readtable("Coordinates_geodome.csv", 'VariableNamingRule', 'preserve', 'NumHeaderLines', 0, 'DecimalSeparator','.'); 

disp(positionCordinates)

figure(1); 
hold on; 
grid on;
axis equal;

%ploting origo
x = [250, 0, 0];
y = [0, 250, 0];
z = [0, 0, 250]; 

quiver3(0, 0, 0, x(1), x(2), x(3), 'r', 'LineWidth', 2);
quiver3(0, 0, 0, y(1), y(2), y(3), 'g', 'LineWidth', 2);
quiver3(0, 0, 0, z(1), z(2), z(3), 'b', 'LineWidth', 2);

%floor layer
for i = 1:9
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='red');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(10, 1),positionCordinates.y(10, 1), positionCordinates.z(10, 1), '.', MarkerSize=30, Color='red');
line([positionCordinates.x(1), positionCordinates.x(10)], [positionCordinates.y(1), positionCordinates.y(10)],  [positionCordinates.z(1), positionCordinates.z(10)],  'Color', 'cyan');

%middle layer
for i = 11:19
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='green');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(20, 1),positionCordinates.y(20, 1), positionCordinates.z(20, 1), '.', MarkerSize=30, Color='green');
line([positionCordinates.x(11), positionCordinates.x(20)], [positionCordinates.y(11), positionCordinates.y(20)],  [positionCordinates.z(11), positionCordinates.z(20)],  'Color', 'cyan');


%top layer
for i = 21:24
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='blue');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(25, 1),positionCordinates.y(25, 1), positionCordinates.z(25, 1), '.', MarkerSize=30, Color='blue');
line([positionCordinates.x(21), positionCordinates.x(25)], [positionCordinates.y(21), positionCordinates.y(25)],  [positionCordinates.z(21), positionCordinates.z(25)],  'Color', 'cyan');


%the top
for i = 26
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='magenta');
end

xlabel('X [mm]'); 
ylabel('Y [mm]'); 
zlabel('Z [mm]'); 


positionCordinates = readtable("new_coordinates_geodome.csv", 'VariableNamingRule', 'preserve', 'NumHeaderLines', 1, 'DecimalSeparator',','); 

disp(positionCordinates)

figure(2); 
hold on; 
grid on;
axis equal;

%ploting origo
x = [250, 0, 0];
y = [0, 250, 0];
z = [0, 0, 250]; 

quiver3(0, 0, 0, x(1), x(2), x(3), 'r', 'LineWidth', 2);
quiver3(0, 0, 0, y(1), y(2), y(3), 'g', 'LineWidth', 2);
quiver3(0, 0, 0, z(1), z(2), z(3), 'b', 'LineWidth', 2);

%floor layer
for i = 1:9
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='red');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(10, 1),positionCordinates.y(10, 1), positionCordinates.z(10, 1), '.', MarkerSize=30, Color='red');
line([positionCordinates.x(1), positionCordinates.x(10)], [positionCordinates.y(1), positionCordinates.y(10)],  [positionCordinates.z(1), positionCordinates.z(10)],  'Color', 'cyan');

%middle layer
for i = 11:19
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='green');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(20, 1),positionCordinates.y(20, 1), positionCordinates.z(20, 1), '.', MarkerSize=30, Color='green');
line([positionCordinates.x(11), positionCordinates.x(20)], [positionCordinates.y(11), positionCordinates.y(20)],  [positionCordinates.z(11), positionCordinates.z(20)],  'Color', 'cyan');


%top layer
for i = 21:24
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='blue');
    line([positionCordinates.x(i), positionCordinates.x(i+1)], [positionCordinates.y(i), positionCordinates.y(i+1)],  [positionCordinates.z(i), positionCordinates.z(i+1)],  'Color', 'cyan');
end
plot3(positionCordinates.x(25, 1),positionCordinates.y(25, 1), positionCordinates.z(25, 1), '.', MarkerSize=30, Color='blue');
line([positionCordinates.x(21), positionCordinates.x(25)], [positionCordinates.y(21), positionCordinates.y(25)],  [positionCordinates.z(21), positionCordinates.z(25)],  'Color', 'cyan');


%the top
for i = 26
    plot3(positionCordinates.x(i, 1),positionCordinates.y(i, 1), positionCordinates.z(i, 1), '.', MarkerSize=30, Color='magenta');
end

xlabel('X [mm]'); 
ylabel('Y [mm]'); 
zlabel('Z [mm]'); 

%% Calculating WL. IO/CAB-NBPUF-4UTP-26AWG-R3

clear; clc; close all; 

PowerRequirement = 5*2 %W Power for underwater installation

outInjector_V = 56; %V
outInjector_I = 0.625; %A

outPowerInjector = outInjector_I*outInjector_V %W

transmittetCurrent = PowerRequirement/outInjector_V %A

wireCS = 0.14; %mm^2

wireDiameter = sqrt(wireCS/pi)*2; 

wireResistance = 0.127; %ohm/m

wireLegth = 50; %m

vDrop = 2 * transmittetCurrent * wireResistance * wireLegth
vDropPercent = vDrop/outInjector_V*100 
vEnd = outInjector_V-vDrop

%% Plot from LoRa test

clear; clc; close all; 

values = readtable("LoRa_test.csv", 'VariableNamingRule', 'preserve', 'NumHeaderLines', 1, 'DecimalSeparator',','); 

values(10, :)= values(11, :); 
values(11, :)= [];
values(11, :)= [];

disp(values)

p = polyfit(values.Distance, values.RSSI, 4);
fitted_y = polyval(p, values.Distance);

figure(1); 
hold on; 
grid on;
plot(values.Distance, values.RSSI, '.', MarkerSize=30, Color=[1, 0.5, 0]);
plot(values.Distance, fitted_y, '--k', 'Color', [0.5, 0.5, 0.5], 'LineWidth', 2);
xlabel('Distance from Buoy [m]', 'Interpreter','latex', 'FontSize',30); 
ylabel('RSSI [-dBm]', 'Interpreter','latex', 'FontSize',30); 
title('LoRa Signal Strength Test','Interpreter','latex','FontSize',40); 
legend('Signal Test', 'Trendline');
lgd = legend;
lgd.FontSize = 14;
for i = 1:length(values.Ping)
    text(values.Distance(i), values.RSSI(i), num2str(values.Ping(i)), ...
    'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'right', 'FontSize',20, Color=[1, 0.5, 0]);
end


