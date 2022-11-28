// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"

// Sets default values
ATable::ATable()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	APawn::PrimaryActorTick.bCanEverTick = true;

	World = GetWorld();
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	APawn::BeginPlay();

	GenerateTable();
}

void ATable::GenerateTable()
{
	StartDrawing();

	FVector BelowBL = CalculatePointBelow(BottomLeft, TableTopHeight);
	FVector BelowBR = CalculatePointBelow(BottomRight, TableTopHeight);
	FVector BelowTR = CalculatePointBelow(TopRight, TableTopHeight);
	FVector BelowTL = CalculatePointBelow(TopLeft, TableTopHeight);

	FVector TableVertices[8] =
	{
		BottomLeft,	  // Top Back  Right
		BottomRight,  // Top Front Right
		TopRight,	  // Top Front Left
		TopLeft,	  // Top Back  
		BelowBL,	  // Bottom Back  Right 
		BelowBR,	  // Bottom Front Right
		BelowTR,      // Bottom Front Left
		BelowTL		  // Bottom Back  Left
	};

	DrawCube(TableVertices);
	DrawLeg(BelowBL, LegHeight, LegThickness);
	DrawLeg(BelowBR, LegHeight, LegThickness);
	DrawLeg(BelowTR, LegHeight, LegThickness);
	DrawLeg(BelowTL, LegHeight, LegThickness);

	StopDrawing();

	UpdateTableSize();

	DeleteAllChairs();
	SpawnChairs();

	GEngine->ForceGarbageCollection();

	TopRight.Y++;
	BottomRight.Y++;
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	APawn::Tick(DeltaTime);

	GenerateTable();
}

// Called to bind functionality to input
void ATable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATable::UpdateTableSize()
{
	TableX = FVector::Dist(BottomLeft, TopLeft);
	TableY = FVector::Dist(BottomLeft, BottomRight);

	UsableTableX = TableX - LegThickness * 2;
	UsableTableY = TableY - LegThickness * 2;
}

void ATable::SpawnChairs()
{
	UpdateNumberOfChairsOnY();
	UpdateNumberOfChairsOnX();

	const FTransform TableTransform = this->GetTransform();

	for (int i = 0; i < NumberOfChairsOnY; i++)
	{
		AChair* NewChair = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChair->Init(SetChairPositionOnY(i));
		NewChair->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChair);
	}

	for (int i = 0; i < NumberOfChairsOnX; i++)
	{
		AChair* NewChair = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChair->Init(SetChairPositionOnX(i));
		NewChair->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChair);
	}
}

void ATable::DeleteAllChairs()
{
	for (AChair* Chair : MyChairs)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%i"), MyChairs.size());
		Chair->Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("%i"), MyChairs.size());
	MyChairs.clear();
}

void ATable::UpdateNumberOfChairsOnY()
{
	float ChairDistanceSum = ChairSize + ChairMinDistance;

	NumberOfChairsOnY = (int)(UsableTableY / (ChairDistanceSum - (ChairMinDistance / ChairDistanceSum)));
}

void ATable::UpdateNumberOfChairsOnX()
{
	float ChairDistanceSum = ChairSize + ChairMinDistance;

	NumberOfChairsOnX = (int)(UsableTableX / (ChairDistanceSum - (ChairMinDistance / ChairDistanceSum)));
}

void ATable::UpdateCurrentChairDistanceOnY()
{
	CurrentChairDistanceOnY = (UsableTableY - NumberOfChairsOnY * 40) / (NumberOfChairsOnY + 1);
}

void ATable::UpdateCurrentChairDistanceOnX()
{
	CurrentChairDistanceOnX = (UsableTableX - NumberOfChairsOnX * 40) / (NumberOfChairsOnX + 1);
}

FVector ATable::SetChairPositionOnY(int CurrentChairNumber)
{

	UpdateCurrentChairDistanceOnY();
	float ChairDistanceSum = ChairSize + CurrentChairDistanceOnY;

	FVector CurrentChairPosition = { BottomLeft.X, BottomLeft.Y + (LegThickness + ChairDistanceSum * CurrentChairNumber + CurrentChairDistanceOnY), 0 };

	return CurrentChairPosition;
}

FVector ATable::SetChairPositionOnX(int CurrentChairNumber)
{

	UpdateCurrentChairDistanceOnX();
	float ChairDistanceSum = ChairSize + CurrentChairDistanceOnX;

	FVector CurrentChairPosition = { BottomLeft.X + (LegThickness + ChairDistanceSum * CurrentChairNumber + CurrentChairDistanceOnX), BottomLeft.Y, 0 };

	return CurrentChairPosition;
}