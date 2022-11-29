// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

AChair::AChair()
{

}

void AChair::Init(FVector Point, FQuat Quaternion)
{
	// Set Chair pivot position
	this->ChairSpawningPoint = Point;

	this->SetPivotOffset(ChairSpawningPoint);
	ChairSpawningPoint.Z += 20;
	
	ThisMesh->SetWorldLocation(ChairSpawningPoint);


	// Set Chair rotation quaternion
	ChairRotation = Quaternion;
}

void AChair::BeginPlay()
{
	Super::BeginPlay();
	
	//Chair seat vertices
	FVector ChairSeatBL = { 0, 0, ChairSeatHeight};

	FVector ChairSeatBR = { 0,			ChairWidth,  ChairSeatBL.Z };
	FVector ChairSeatTR = { ChairWidth, ChairWidth,  ChairSeatBL.Z };
	FVector ChairSeatTL = { ChairWidth, 0,			 ChairSeatBL.Z };

	FVector SeatBottomBL = { CalculatePointBelow(ChairSeatBL, ChairElementsThickness) };
	FVector SeatBottomBR = { CalculatePointBelow(ChairSeatBR, ChairElementsThickness) };
	FVector SeatBottomTR = { CalculatePointBelow(ChairSeatTR, ChairElementsThickness) };
	FVector SeatBottomTL = { CalculatePointBelow(ChairSeatTL, ChairElementsThickness) };


	FVector ChairSeatVertices[8] =
	{
		ChairSeatBL,
		ChairSeatBR,
		ChairSeatTR,
		ChairSeatTL,
		SeatBottomBL,
		SeatBottomBR,
		SeatBottomTR,
		SeatBottomTL
	};

	//Backrest Vertices
	FVector BackrestTR = { ChairSeatBL.X, ChairSeatBL.Y + ChairWidth, ChairSeatBL.Z + ChairSeatHeight };
	FVector BackrestTL = { ChairSeatBL.X, ChairSeatBL.Y,		       ChairSeatBL.Z + ChairSeatHeight };

	//Front of backrest
	FVector BackrestFrontTR = { BackrestTR.X + ChairElementsThickness, BackrestTR.Y, BackrestTR.Z };
	FVector BackrestFrontTL = { BackrestTL.X + ChairElementsThickness, BackrestTL.Y, BackrestTL.Z };


	FVector BackrestVertices[8] =
	{
		ChairSeatBL,
		ChairSeatBR,
		BackrestTR,
		BackrestTL,
		CalculatePointBelow(BackrestFrontTL, ChairSeatHeight),
		CalculatePointBelow(BackrestFrontTR, ChairSeatHeight),
		BackrestFrontTR,
		BackrestFrontTL
	};

	//Drawing 

	StartDrawing();

	DrawCube(ChairSeatVertices);
	DrawCube(BackrestVertices);

	DrawLeg(SeatBottomBL, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomBR, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomTR, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomTL, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);

	StopDrawing();

	// Rotate chair after it's created
	ThisMesh->AddLocalRotation(ChairRotation);
}