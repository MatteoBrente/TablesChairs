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

	// Vectors under the main ones (bottom of the tabletop)
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

	// Section where I draw each piece of the table
	DrawCube(TableVertices);
	DrawLeg(BelowBL, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowBR, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowTR, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowTL, LegHeight, LegThickness, BottomLeft, BottomRight);

	StopDrawing();

	UpdateTableSize();

	// Drawing chairs begins here
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
	// Update chair numbers on all axis
	UpdateNumberOfChairsOnY();
	UpdateNumberOfChairsOnX();

	// Set quaternions for later chair creation calls
	FQuat Forward  = FQuat{ 0.f, 0.f, 0.f, 0.f };
	FQuat Backward = FQuat{ 0.f, 0.f, 180.f, 0.f };
	FQuat Right = FQuat{ FRotator{0.f, 90.f, 0.f} };
	FQuat Left	   = FQuat{ FRotator{0.f, -90.f, 0.f} };

	const FTransform TableTransform = this->GetTransform();

	//Spawn Chairs on Y Axis
	for (int i = 0; i < NumberOfChairsOnY; i++)
	{
		AChair* NewChair = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChair->Init(SetChairPositionOnY(i), Forward);
		NewChair->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChair);

		AChair* NewChairOpposite = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChairOpposite->Init(SetChairPositionOnY(i, true), Backward);
		NewChairOpposite->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChairOpposite);
	}

	//Spawn Chairs on X Axis
	for (int i = 0; i < NumberOfChairsOnX; i++)
	{
		AChair* NewChair = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChair->Init(SetChairPositionOnX(i), Right);
		NewChair->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChair);

		AChair* NewChairOpposite = World->SpawnActorDeferred<AChair>(AChair::StaticClass(), TableTransform);
		NewChairOpposite->Init(SetChairPositionOnX(i, true), Left);
		NewChairOpposite->FinishSpawning(TableTransform);
		MyChairs.push_back(NewChairOpposite);
	}
}

void ATable::DeleteAllChairs()
{
	for (AChair* Chair : MyChairs)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%i"), MyChairs.size());
		Chair->Destroy();
	}
	MyChairs.clear();
}

void ATable::UpdateNumberOfChairsOnY()
{
	// Variable I need just for this calculation
	float ChairDistanceSum = ChairSize + ChairMinDistance;

	NumberOfChairsOnY = (int)(UsableTableY / (ChairDistanceSum - (ChairMinDistance / ChairDistanceSum)));
}

void ATable::UpdateNumberOfChairsOnX()
{
	// Variable I need just for this calculation
	float ChairDistanceSum = ChairSize + ChairMinDistance;

	NumberOfChairsOnX = (int)(UsableTableX / (ChairDistanceSum - (ChairMinDistance / ChairDistanceSum)));
}

FVector ATable::SetChairPositionOnY(int CurrentChairNumber, bool IsOpposite)
{
	// Change the chair distance on the Y axis
	CurrentChairDistanceOnY = (UsableTableY - NumberOfChairsOnY * 40) / (NumberOfChairsOnY + 1);

	// Calculates the position for chairs at the bottom of the table
	float ChairDistanceSum = ChairSize + CurrentChairDistanceOnY;
	FVector CurrentChairPosition = { BottomLeft.X - ChairMinDistance, BottomLeft.Y + (LegThickness + ChairDistanceSum * CurrentChairNumber + CurrentChairDistanceOnY), 0 };
	
	// Calculates the position for the ones at the top of the table
	if (IsOpposite)
	{
		CurrentChairPosition.X += TableX + 2 * ChairMinDistance;
		CurrentChairPosition.Y += 40;
	}

	return CurrentChairPosition;
}

FVector ATable::SetChairPositionOnX(int CurrentChairNumber, bool IsOpposite)
{
	// Change the chair distance on the X axis
	CurrentChairDistanceOnX = (UsableTableX - NumberOfChairsOnX * 40) / (NumberOfChairsOnX + 1);

	// Calculates the position for chairs at the right of the table
	float ChairDistanceSum = ChairSize + CurrentChairDistanceOnX;
	FVector CurrentChairPosition = { BottomLeft.X + (LegThickness + ChairDistanceSum * CurrentChairNumber + CurrentChairDistanceOnX), BottomLeft.Y - ChairMinDistance, 0 };
	
	// Calculates the position for the ones at the left of the table
	if (IsOpposite)
	{
		CurrentChairPosition.Y += TableY + 2 * ChairMinDistance;
	}
	else
	{
		CurrentChairPosition.X += 40;
	}

	return CurrentChairPosition;
}