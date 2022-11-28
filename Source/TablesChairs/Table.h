// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FurnitureRenderer.h"
#include "Components/SceneComponent.h"
#include "FurnitureRenderer.h"
#include "Chair.h"
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
		float ChairMinDistance = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateTable();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector BottomLeft = FVector(-TableX, -TableY, TableZ);
	FVector BottomRight = FVector(-TableX, TableY, TableZ);
	FVector TopRight = FVector(TableX, TableY, TableZ);
	FVector TopLeft = FVector(TableX, -TableY, TableZ);

	float LegHeight = TableZ - TableTopHeight;

	std::vector<AChair*> MyChairs = {};

	float UsableTableX = TableX - LegThickness * 2;
	float UsableTableY = TableY - LegThickness * 2;

	int NumberOfChairsOnX = 0;
	int NumberOfChairsOnY = 0;
	float CurrentChairDistanceOnY = 0;
	float CurrentChairDistanceOnX = 0;


	void UpdateTableSize();

	void SpawnChairs();
	void DeleteAllChairs();

	void UpdateNumberOfChairsOnY();
	void UpdateNumberOfChairsOnX();

	void UpdateCurrentChairDistanceOnY();
	void UpdateCurrentChairDistanceOnX();

	FVector SetChairPositionOnY(int CurrentChairNumber);
	FVector SetChairPositionOnX(int CurrentChairNumber);
};