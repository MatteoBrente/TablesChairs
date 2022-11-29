// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FurnitureRenderer.h"

#include "Chair.generated.h"


UCLASS()
class TABLESCHAIRS_API AChair : public AFurnitureRenderer
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChair();

	UPROPERTY(EditAnywhere)
		float ChairSeatHeight = 50.f;

	UPROPERTY(EditAnywhere)
		float ChairWidth = 40.f;

	UPROPERTY(EditAnywhere)
		float ChairElementsThickness = 5.f;

	void Init(FVector Point, FQuat Quaternion);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector ChairSpawningPoint = {};
	FQuat ChairRotation = FQuat{};
};