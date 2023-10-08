%%%%%%%%%% Simulation of the ANFIS model
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%% Initialization
clear all;
close all;
clc;


%%%%% Load data and angles
Data=load('Data.dat');
Angles=load('Angles.dat');

NormData=zscore(Data); % Normalize the data using Z score
Data=[Data Angles];

TrainData=Data(1:7000,:); % Training data
TestData=Data(7001:9500,:); % Testing data
CheckData=Data(9501:10000,:); % Checking data


%%%%% Build membership functions
MaxDim=3; % The number of inputs

MyFIS3Dim=genfis1(TrainData,MaxDim,'gbellmf');

MyFIS3Dim.input(1,1).name='Left';
MyFIS3Dim.input(1,1).mf(1,1).name='near';
MyFIS3Dim.input(1,1).mf(1,2).name='medium';
MyFIS3Dim.input(1,1).mf(1,3).name='far';
    
MyFIS3Dim.input(1,2).name='Right';
MyFIS3Dim.input(1,2).mf(1,1).name='near';
MyFIS3Dim.input(1,2).mf(1,2).name='medium';
MyFIS3Dim.input(1,2).mf(1,3).name='far';
    
MyFIS3Dim.input(1,3).name='Front';
MyFIS3Dim.input(1,3).mf(1,1).name='near';
MyFIS3Dim.input(1,3).mf(1,2).name='medium';
MyFIS3Dim.input(1,3).mf(1,3).name='far';

% The range of membership functions
for t=1:MaxDim,
    MyFIS3Dim.input(1,t).range=[min(Data(:,t)),max(Data(:,t))];
end

% Plot the membership functions
figure(1);
subplot(2,2,1);
plotmf(MyFIS3Dim,'input',1);
subplot(2,2,2);
plotmf(MyFIS3Dim,'input',2);
subplot(2,2,3);
plotmf(MyFIS3Dim,'input',3);


%%%%% Train the network
Epochs=[300 0 0.01 0.9 1.1]; % The number of epochs
[MyFIS3Dim1,error1,ss,MyFIS3Dim2,error2]=anfis(TrainData,MyFIS3Dim,Epochs,[],CheckData);

% Show details
ruleview(MyFIS3Dim2);
getfis(MyFIS3Dim2);
disp('');
showrule(MyFIS3Dim2)
disp('');
showfis(MyFIS3Dim2);

% Write FIS into a file
File=which('Simulation.m');
[FilePath,Name,Ext]=fileparts(File);
writefis(MyFIS3Dim2,strcat(FilePath,'\FIS'));

%%%%% Test the network
Max_index=size(TestData);
Max_index=Max_index(1,1);

ANFIS_output=evalfis(TestData(:,1:MaxDim),MyFIS3Dim2);

% Plot testing data and FIS output
figure(2);
hold on;
plot([1:Max_index],ANFIS_output,'r*');
plot([1:Max_index],TestData(:,MaxDim+1),'b.');
hold off;
title('ANFIS output & Testing data');
xlabel('Index');
ylabel('Output');
legend('ANFIS output','Testing data');

Error=TestData(:,MaxDim+1)-ANFIS_output;

% Plot error
figure(3);
plot([1:Max_index],Error,'b');
xlabel('Index');
ylabel('Difference');
title('Error');

Angle_goLeft=0;
Angle_goRight=0;
Angle_goFront=0;
MIS_index=[0,0];

%%%%% Classification table
for index=1:Max_index
    if ((ANFIS_output(index)<(TestData(index,MaxDim+1)-0.5)) | (ANFIS_output(index)<(TestData(index,MaxDim+1)+0.5)))
        if (TestData(index,MaxDim+1)==2)
            Angle_goLeft=Angle_goLeft+1;
        elseif (TestData(index,MaxDim+1)==4)
            Angle_goRight=Angle_goRight+1;
        end
    else
        Angle_goFront=Angle_goFront+1;
        if (TestData(index,MaxDim+1)==2)
            MIS_index=[MIS_index;2,index];
        else
            MIS_index=[MIS_index;4,index];
        end
    end
end

% Percent Misclassified
Front_per=(Angle_goFront/Max_index)*100;
Right_per=(Angle_goRight/Max_index)*100;
Left_per=(Angle_goLeft/Max_index)*100;
