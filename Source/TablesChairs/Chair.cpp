// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

AChair::AChair()
{

}

void AChair::Init(FVector Point)
{
	this->ChairSpawningPoint = Point;
}

void AChair::BeginPlay()
{
	Super::BeginPlay();
	
	//Chair seat vertices
	FVector ChairSeatBL = { ChairSpawningPoint.X,		ChairSpawningPoint.Y,		 ChairSpawningPoint.Z + ChairSeatHeight };
	FVector ChairSeatBR = { ChairSeatBL.X,				ChairSeatBL.Y + ChairWidth,  ChairSeatBL.Z };
	FVector ChairSeatTR = { ChairSeatBL.X + ChairWidth, ChairSeatBL.Y + ChairWidth,  ChairSeatBL.Z };
	FVector ChairSeatTL = { ChairSeatBL.X + ChairWidth, ChairSeatBL.Y,				 ChairSeatBL.Z };
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

	float HalfLength = ChairWidth / 2;

	DrawLeg(SeatBottomBL, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomBR, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomTR, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);
	DrawLeg(SeatBottomTL, (ChairSeatHeight - ChairElementsThickness), ChairElementsThickness, ChairSeatBL, ChairSeatTR);

	StopDrawing();


	//this->SetPivotOffset(ChairSpawningPoint);
	//this->SetActorRelativeRotation(FQuat{ 0.f, 0.f, 180.f, 0.f });

	//ThisMesh->AddLocalRotation(this->GetActorRotation();
}