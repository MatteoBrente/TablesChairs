// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProceduralMeshComponent.h"

#include "FurnitureRenderer.generated.h"

UCLASS()
class TABLESCHAIRS_API AFurnitureRenderer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFurnitureRenderer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;

	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* ThisMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartDrawing();
	void StopDrawing();

	void DrawQuad(FVector A, FVector B, FVector C, FVector D, int32& Trindex, FProcMeshTangent Tangent);
	void DrawCube(FVector CubeVertices[8]);
	void DrawLeg(FVector StartingPoint, float LegHeight, float LegThickness, FVector BL, FVector TR);
	FVector CalculatePointBelow(FVector Point, float Height);

private:

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;

	int32 TriangleIndexCount;
	FProcMeshTangent TangentSetup;
};