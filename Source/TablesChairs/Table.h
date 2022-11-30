// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FurnitureRenderer.h"
#include "Chair.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include <vector>

#include "Table.generated.h"


UCLASS()
class TABLESCHAIRS_API ATable : public AFurnitureRenderer
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATable();

	UPROPERTY(VisibleAnywhere)
		UWorld* World;

	UPROPERTY(EditAnywhere)
		float TableX = 100.f;

	UPROPERTY(EditAnywhere)
		float TableY = 100.f;

	UPROPERTY(EditAnywhere)
		float TableZ = 80.f;

	UPROPERTY(EditAnywhere)
		float TableTopHeight = 8.f;

	UPROPERTY(EditAnywhere)
		float LegThickness = 10.f;

	UPROPERTY(EditAnywhere)
		float TableMin = 80.f;

	UPROPERTY(EditAnywhere)
		float TableMax = 300.f;

	UPROPERTY(EditAnywhere)
		float ChairSize = 40.f;

	UPROPERTY(EditAnywhere)
		float ChairMinDistance = 15.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DrawTable();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Camera
	UCameraComponent* OurCamera;
	
	// Main tabletop vertices
	FVector BottomLeft = FVector(-TableX, -TableY, TableZ);
	FVector BottomRight = FVector(-TableX, TableY, TableZ);
	FVector TopRight = FVector(TableX, TableY, TableZ);
	FVector TopLeft = FVector(TableX, -TableY, TableZ);

	// Table variables
	float UsableTableX = TableX - LegThickness * 2;
	float UsableTableY = TableY - LegThickness * 2;

	float LegHeight = TableZ - TableTopHeight;

	// Movement variables
	float StartingMouseX = 0;
	float StartingMouseY = 0;

	float NewMouseX = 0;
	float NewMouseY = 0;

	FVector* MovingPoint = nullptr;
	bool MouseIsPressed = false;

	// Movement functions
	void StartMoving();
	void MoveTablePoints();
	void StopMoving();

	// Chair variables
	std::vector<AChair*> MyChairs = {};

	int NumberOfChairsOnX = 0;
	int NumberOfChairsOnY = 0;

	float CurrentChairDistanceOnY = 0;
	float CurrentChairDistanceOnX = 0;

	// Chair position vectors
	FVector SetChairPositionOnY(int CurrentChairNumber, bool IsOpposite = false);
	FVector SetChairPositionOnX(int CurrentChairNumber, bool IsOpposite = false);

	// Chair functions
	void DrawChairs();
	void DeleteMyChairs();
	void UpdateNumberOfChairsOnY();
	void UpdateNumberOfChairsOnX();
};
