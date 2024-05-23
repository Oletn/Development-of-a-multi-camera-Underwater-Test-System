clc; close all; clear; % Clearing the workspace, closing all figures and clearing the command window

jsonData = fileread('mediumHigh.json');
data = jsondecode(jsonData);
prettyJson = jsonencode(data, 'PrettyPrint', true);
fileID = fopen('pretty_output_file.json', 'w');

fprintf(fileID, '%s', prettyJson);

cameras = data.Cameras;

location = zeros(length(cameras), 3);

lengthcamera = length(cameras);



for k = 1 : lengthcamera
    location(k,1) = cameras(k).Transform.x;
    location(k,2) = cameras(k).Transform.y;
    location(k,3) = cameras(k).Transform.z;
   
end

figure;
hold on;
grid on; 
axis equal;

for i = 1:size(location,1)
    plot3(location(i,1),location(i,2), location(i,3), '.', MarkerSize=30);
end

xlabel('X[mm]'); 
ylabel('Y[mm]'); 
zlabel('Z[mm]'); 

%xlim([-4000, 4000])
%ylim([-4000, 4000])
%zlim([-4000,4000])

origo = [0 0 0];
xvec =[1;0;0];
yvec =[0;1;0];
zvec =[0;0;-1];

linelength = 800;

 %% PLotting direction lines
for i = 1 : lengthcamera

    Rot =[cameras(i).Transform.r11, cameras(i).Transform.r12, cameras(i).Transform.r13
          cameras(i).Transform.r21, cameras(i).Transform.r22, cameras(i).Transform.r23
          cameras(i).Transform.r31, cameras(i).Transform.r32, cameras(i).Transform.r33];

    %Rot = quat2rotmat(Rot);

    FOVvec = Rot' * zvec;

    endx = location(i,1) + FOVvec(1) * linelength;
    endy = location(i,2) + FOVvec(2) * linelength;
    endz = location(i,3) + FOVvec(3) * linelength;


    plot3([location(i,1), endx], [location(i,2), endy], [location(i,3), endz], 'r-', LineWidth=2);


end

 %% Plotting cross in middle 
endx = [200, 0, 0];
endy = [0, 200, 0];
endz = [0, 0, 200];

line([origo(1), endx(1)], [origo(2), endx(2)], [origo(3), endx(3)], 'Color', 'k', 'LineStyle', '--');
line([origo(1), endy(1)], [origo(2), endy(2)], [origo(3), endy(3)], 'Color', 'k', 'LineStyle', '--');
line([origo(1), endz(1)], [origo(2), endz(2)], [origo(3), endz(3)], 'Color', 'k', 'LineStyle', '--');



%% Adding pyramid

thetaHorisonal = 37;
thetaVertical = 61;
fovLength = linelength;
gjennomsiktighet = 0.1;

for i=1 : lengthcamera

    cameraPosition = [data.Cameras(i).Transform.x, data.Cameras(i).Transform.y, data.Cameras(i).Transform.z];
    
    Rot =[cameras(i).Transform.r11, cameras(i).Transform.r21, cameras(i).Transform.r31
          cameras(i).Transform.r12, cameras(i).Transform.r22, cameras(i).Transform.r32
          cameras(i).Transform.r13, cameras(i).Transform.r23, cameras(i).Transform.r33];


    directionVector = Rot * zvec;

    addPyramid(cameraPosition, thetaHorisonal, thetaVertical, fovLength, directionVector, gjennomsiktighet)


end

fclose(fileID);

%%Function
function addPyramid(cameraPosition,thetaHorisonal, thetaVertical, fovLength, directionVector, gjennomsiktighet)

% camera positon [1,1,1]
% directionVector [-1,-1,-1]

thetaVertical = deg2rad(thetaVertical);
thetaHorisonal = deg2rad(thetaHorisonal);
globalUp = [0 0 -1];

nVector = normalize(directionVector');

fovWidth = 2 * fovLength * tan(thetaHorisonal/2);
fovHeight = 2 * fovLength * tan(thetaVertical / 2);


widthVector = cross(nVector,globalUp);
widthVector = normalize(widthVector);


heightVector = cross(widthVector, nVector);
heightVector = normalize(heightVector);

calculating_middle_of_pyramid_base = cameraPosition + fovLength * nVector;

calculating_width_of_base_pyramid = (fovWidth/2) * widthVector;

calculating_height_of_base_pyramid = (fovHeight / 2) * heightVector;

%%Calculate the corner points of the pyramid
pyramidcorner1 = calculating_middle_of_pyramid_base + calculating_width_of_base_pyramid + calculating_height_of_base_pyramid;
pyramidcorner2 = calculating_middle_of_pyramid_base - calculating_width_of_base_pyramid + calculating_height_of_base_pyramid;
pyramidcorner3 = calculating_middle_of_pyramid_base - calculating_width_of_base_pyramid - calculating_height_of_base_pyramid;
pyramidcorner4 = calculating_middle_of_pyramid_base + calculating_width_of_base_pyramid - calculating_height_of_base_pyramid;

% Draw FOV pyramid
    drawPyramidEdges(cameraPosition, pyramidcorner1, pyramidcorner2, pyramidcorner3, pyramidcorner4, gjennomsiktighet);

end



%% Other functions


function nVec = normalize(vec)
    nVec = vec / norm(vec);
end

function radians = deg2rad(degrees)
    radians = degrees * (pi / 180);
end

function drawPyramidEdges(cameraPosition, corner1, corner2, corner3, corner4, gjennomsiktighet)
    % Define base color
    bacecolour = [1, 0, 0]; 

    % Draw sides of the pyramid with transparency
    fill3([cameraPosition(1), corner1(1), corner2(1)], [cameraPosition(2), corner1(2), corner2(2)], [cameraPosition(3), corner1(3), corner2(3)], bacecolour, FaceAlpha=gjennomsiktighet);

    fill3([cameraPosition(1), corner2(1), corner3(1)], [cameraPosition(2), corner2(2), corner3(2)], [cameraPosition(3), corner2(3), corner3(3)], bacecolour, FaceAlpha=gjennomsiktighet);

    fill3([cameraPosition(1), corner3(1), corner4(1)], [cameraPosition(2), corner3(2), corner4(2)], [cameraPosition(3), corner3(3), corner4(3)], bacecolour, FaceAlpha=gjennomsiktighet);
    
    fill3([cameraPosition(1), corner4(1), corner1(1)], [cameraPosition(2), corner4(2), corner1(2)], [cameraPosition(3), corner4(3), corner1(3)], bacecolour, FaceAlpha=gjennomsiktighet);

    % Draw base of the pyramid
    fill3([corner1(1), corner2(1), corner3(1), corner4(1)],[corner1(2), corner2(2), corner3(2), corner4(2)], [corner1(3), corner2(3), corner3(3), corner4(3)], bacecolour, FaceAlpha=gjennomsiktighet);
end