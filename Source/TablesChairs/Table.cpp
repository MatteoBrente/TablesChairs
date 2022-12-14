// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"

// Sets default values
ATable::ATable() : Super()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	APawn::PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	World = GetWorld();

	// Create Camera and Widget Objects
	TableCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TableCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	APawn::BeginPlay();

	// Calculate initial distance to correctly spawn chairs
	TableX = FVector::Dist(BottomLeft, TopLeft);
	TableY = FVector::Dist(BottomLeft, BottomRight);

	// Set Camera Position and Rotation
	TableCamera->SetRelativeLocation(FVector(-300.0f, 0.0f, 500.0f));
	TableCamera->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
		PC->bShowMouseCursor = true;
}

void ATable::DrawTable()
{
	StartDrawing();

	// Vectors of the bottom of the tabletop
	FVector BelowBL = CalculatePointBelow(BottomLeft, TableTopHeight);
	FVector BelowBR = CalculatePointBelow(BottomRight, TableTopHeight);
	FVector BelowTR = CalculatePointBelow(TopRight, TableTopHeight);
	FVector BelowTL = CalculatePointBelow(TopLeft, TableTopHeight);

	FVector TableTopVertices[8] =
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

	// Draw the tabletop and then the legs
	DrawCube(TableTopVertices);
	DrawLeg(BelowBL, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowBR, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowTR, LegHeight, LegThickness, BottomLeft, BottomRight);
	DrawLeg(BelowTL, LegHeight, LegThickness, BottomLeft, BottomRight);

	StopDrawing();
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	APawn::Tick(DeltaTime);

	// Draw table and chairs
	DrawTable();
	DeleteMyChairs();
	DrawChairs();

	// Mouse input and table size change
	InputComponent->BindAction("SelectBL", IE_Pressed, this, &ATable::StartMovingBL);
	InputComponent->BindAction("SelectBR", IE_Pressed, this, &ATable::StartMovingBR);
	InputComponent->BindAction("SelectTR", IE_Pressed, this, &ATable::StartMovingTR);
	InputComponent->BindAction("SelectTL", IE_Pressed, this, &ATable::StartMovingTL);

	InputComponent->BindAction("LeftClick", IE_Released, this, &ATable::StopMoving);
	
	if (ShouldMove && MovingPoint != nullptr)
		MoveTablePoints();

	// Leaves no weird stuff after all the chair deletions
	GEngine->ForceGarbageCollection();
}

// Called to bind functionality to input
void ATable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATable::StartMovingBL()
{
	MovingPoint = &BottomLeft;
	PointWithSameX = &BottomRight;
	PointWithSameY = &TopLeft;
	ShouldMove = true;
	
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(StartingMouseY, StartingMouseX))
		return;
}

void ATable::StartMovingBR()
{
	MovingPoint = &BottomRight;
	PointWithSameX = &BottomLeft;
	PointWithSameY = &TopRight;
	ShouldMove = true;

	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(StartingMouseY, StartingMouseX))
		return;
}

void ATable::StartMovingTR()
{
	MovingPoint = &TopRight;
	PointWithSameX = &TopLeft;
	PointWithSameY = &BottomRight;
	ShouldMove = true;

	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(StartingMouseY, StartingMouseX))
		return;
}

void ATable::StartMovingTL()
{
	MovingPoint = &TopLeft;
	PointWithSameX = &TopRight;
	PointWithSameY = &BottomLeft;
	ShouldMove = true;

	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(StartingMouseY, StartingMouseX))
		return;
}

void ATable::MoveTablePoints()
{
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(NewMouseY, NewMouseX))
		return;

	if (MovingPoint && PointWithSameX && PointWithSameY)
	{
		MoveOnX();
		MoveOnY();

		// Set the starting points to the new position
		StartingMouseX = NewMouseX;
		StartingMouseY = NewMouseY;
	}
	
	// Calculate new distances
	TableX = FVector::Dist(BottomLeft, TopLeft);
	TableY = FVector::Dist(BottomLeft, BottomRight);
}

void ATable::MoveOnX()
{
	// Calculate new position
	float NewXPosition = (NewMouseX - StartingMouseX);

	if ((MovingPoint->X - PointWithSameY->X) > 0)
	{
		if ((TableX - NewXPosition) >= TableMin && (TableX - NewXPosition) <= TableMax)
		{
			// Remove the X difference (the mouse moves in the opposite direction of the world coordinates)
			MovingPoint->X -= NewXPosition;
			PointWithSameX->X -= NewXPosition;
		}
	}
	else
	{
		if ((TableX + NewXPosition) >= TableMin && (TableX + NewXPosition) <= TableMax)
		{
			// Remove the X difference (the mouse moves in the opposite direction of the world coordinates)
			MovingPoint->X -= NewXPosition;
			PointWithSameX->X -= NewXPosition;
		}
	}
}

void ATable::MoveOnY()
{
	// Calculate new position
	float NewYPosition = (NewMouseY - StartingMouseY);

	if ((MovingPoint->Y - PointWithSameX->Y) > 0)
	{
		if ((TableY + NewYPosition) >= TableMin && (TableY + NewYPosition) <= TableMax)
		{
			// Add the Y difference
			MovingPoint->Y += NewYPosition;
			PointWithSameY->Y += NewYPosition;
		}
	}
	else
	{
		if ((TableY - NewYPosition) >= TableMin && (TableY - NewYPosition) <= TableMax)
		{
			// Add the Y difference
			MovingPoint->Y += NewYPosition;
			PointWithSameY->Y += NewYPosition;
		}
	}
}

void ATable::StopMoving()
{
	ShouldMove = false;

	MovingPoint = nullptr;
	PointWithSameX = nullptr;
	PointWithSameY = nullptr;
}

void ATable::DrawChairs()
{
	UsableTableX = TableX - LegThickness * 2;
	UsableTableY = TableY - LegThickness * 2;

	// Update chair numbers on all axis
	UpdateNumberOfChairsOnY();
	UpdateNumberOfChairsOnX();

	// Quaternions used for the chair creation calls
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

void ATable::DeleteMyChairs()
{
	for (AChair* Chair : MyChairs)
	{
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