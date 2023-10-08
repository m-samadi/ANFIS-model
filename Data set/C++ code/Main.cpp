#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<cstdlib>
#include<math.h>
#include<ctime>
#include<string.h>
#include <sstream>

using namespace std;

string ConvertFloatToString(float Number){
    ostringstream Buffer;
    Buffer<<Number;
    
	return Buffer.str();   
}

int main(){
	int SamplesCount=10000;
	float Data[SamplesCount][3],MaximumDistance=25;
	FILE *F;
	string Str;
	
	srand(time(0));
	
	// Generate random numbers for the left distance
	for (int i=0;i<SamplesCount;i++)
		Data[i][0]=(float)(rand()%1000)/10;
		
	// Generate random numbers for the right distance
	for (int j=0;j<SamplesCount;j++)
		Data[j][1]=(float)(rand()%1000)/10;	
		
	// Generate random numbers for the front distance
	for (int k=0;k<SamplesCount;k++)
		Data[k][2]=(float)(rand()%1000)/10;				

	// Write data into the file
	F=fopen("Data.dat","w");	
	for (int l=0;l<SamplesCount;l++){
		Str=ConvertFloatToString(Data[l][0])+","+ConvertFloatToString(Data[l][1])+","+ConvertFloatToString(Data[l][2]);

		for (int m=0;m<Str.length();m++)
			putc(Str[m],F);
	
		if (l<(SamplesCount-1))
			putc('\n',F);
	}
	fclose(F);
	
	// Write angles into the file
	F=fopen("Angles.dat","w");	
	for (int n=0;n<SamplesCount;n++){
		// Scenario 1
		if ((Data[n][0]<=MaximumDistance)and(Data[n][1]>MaximumDistance)and(Data[n][2]>MaximumDistance))
			fputs("45",F);
		// Scenario 2
		else if ((Data[n][0]>MaximumDistance)and(Data[n][1]>MaximumDistance)and(Data[n][2]<=MaximumDistance))
			fputs("90",F);
		// Scenario 3
		else if ((Data[n][0]>MaximumDistance)and(Data[n][1]<=MaximumDistance)and(Data[n][2]>MaximumDistance))
			fputs("-45",F);
		// Scenario 4
		else if ((Data[n][0]<=MaximumDistance)and(Data[n][1]>MaximumDistance)and(Data[n][2]<=MaximumDistance))
			fputs("90",F);
		// Scenario 5
		else if ((Data[n][0]>MaximumDistance)and(Data[n][1]<=MaximumDistance)and(Data[n][2]<=MaximumDistance))
			fputs("-90",F);
		// Scenario 6
		else if ((Data[n][0]<=MaximumDistance)and(Data[n][1]<=MaximumDistance)and(Data[n][2]<=MaximumDistance))
			fputs("180",F);	
		// The remainder scenarios
		else
			fputs("0",F);																				
	
		if (n<(SamplesCount-1))
			putc('\n',F);
	}
	fclose(F);	
				
	// Write data and classes into the dataset
	F=fopen("Dataset.dat","w");	
	for (int o=0;o<SamplesCount;o++){
		Str=ConvertFloatToString(Data[o][0])+","+ConvertFloatToString(Data[o][1])+","+ConvertFloatToString(Data[o][2]);

		// Scenario 1
		if ((Data[o][0]<=MaximumDistance)and(Data[o][1]>MaximumDistance)and(Data[o][2]>MaximumDistance))
			Str=Str+","+"Turn-Right-Slightly";
		// Scenario 2
		else if ((Data[o][0]>MaximumDistance)and(Data[o][1]>MaximumDistance)and(Data[o][2]<=MaximumDistance))
			Str=Str+","+"Turn-Right-Sharply";
		// Scenario 3
		else if ((Data[o][0]>MaximumDistance)and(Data[o][1]<=MaximumDistance)and(Data[o][2]>MaximumDistance))
			Str=Str+","+"Turn-Left-Slightly";
		// Scenario 4
		else if ((Data[o][0]<=MaximumDistance)and(Data[o][1]>MaximumDistance)and(Data[o][2]<=MaximumDistance))
			Str=Str+","+"Turn-Right-Sharply";
		// Scenario 5
		else if ((Data[o][0]>MaximumDistance)and(Data[o][1]<=MaximumDistance)and(Data[o][2]<=MaximumDistance))
			Str=Str+","+"Turn-Left-Sharply";
		// Scenario 6
		else if ((Data[o][0]<=MaximumDistance)and(Data[o][1]<=MaximumDistance)and(Data[o][2]<=MaximumDistance))
			Str=Str+","+"Turn-Back";	
		// The remainder scenarios
		else
			Str=Str+","+"Go-Forward";

		for (int p=0;p<Str.length();p++)
			putc(Str[p],F);
	
		if (o<(SamplesCount-1))
			putc('\n',F);
	}
	fclose(F);
	
	cout<<"The data set was generated successfully.";			
		
	getch();
	return 0;
}
