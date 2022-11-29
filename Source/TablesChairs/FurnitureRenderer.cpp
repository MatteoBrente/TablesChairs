// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureRenderer.h"

// Sets default values
AFurnitureRenderer::AFurnitureRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ThisMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFurnitureRenderer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFurnitureRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFurnitureRenderer::DrawQuad(FVector A, FVector B, FVector C, FVector D, int32& Trindex, FProcMeshTangent Tangent)
{
	// Add four points to the triangle index
	int32 Point1 = Trindex++;
	int32 Point2 = Trindex++;
	int32 Point3 = Trindex++;
	int32 Point4 = Trindex++;

	// Add all vertices in the occluder array
	Vertices.Add(A);
	Vertices.Add(B);
	Vertices.Add(C);
	Vertices.Add(D);

	// First triangle
	Triangles.Add(Point1);
	Triangles.Add(Point2);
	Triangles.Add(Point3);

	// Second triangle
	Triangles.Add(Point4);
	Triangles.Add(Point1);
	Triangles.Add(Point3);

	// Calculate Normals
	FVector ThisNormal = FVector::CrossProduct((A - C), (A - D)).GetSafeNormal();

	// Add normals, tangents and color to all points in the array
	for (size_t i = 0; i < 3; i++)
	{
		Normals.Add(ThisNormal);
		Tangents.Add(Tangent);
		Colors.Add(FLinearColor::Green);
	}

	// Add all points to the UV Array
	UVs.Add(FVector2D(0.f, 1.f)); // Bottom Left = A
	UVs.Add(FVector2D(0.f, 0.f)); // Bottom Right = B
	UVs.Add(FVector2D(1.f, 0.f)); // Top Right = C
	UVs.Add(FVector2D(1.f, 1.f)); // Top Left = D
}

void AFurnitureRenderer::DrawCube(FVector CubeVertices[8])
{
	// Top side
	DrawQuad(CubeVertices[0], CubeVertices[1], CubeVertices[2], CubeVertices[3], TriangleIndexCount, TangentSetup);

	// Bottom Side
	DrawQuad(CubeVertices[7], CubeVertices[6], CubeVertices[5], CubeVertices[4], TriangleIndexCount, TangentSetup);

	// Left Side
	DrawQuad(CubeVertices[5], CubeVertices[6], CubeVertices[2], CubeVertices[1], TriangleIndexCount, TangentSetup);

	// Right Side
	DrawQuad(CubeVertices[7], CubeVertices[4], CubeVertices[0], CubeVertices[3], TriangleIndexCount, TangentSetup);

	//Front Side
	DrawQuad(CubeVertices[4], CubeVertices[5], CubeVertices[1], CubeVertices[0], TriangleIndexCount, TangentSetup);

	//Back Side
	DrawQuad(CubeVertices[6], CubeVertices[7], CubeVertices[3], CubeVertices[2], TriangleIndexCount, TangentSetup);
}

void AFurnitureRenderer::StartDrawing()
{
	ThisMesh->Deactivate();

	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Colors.Reset();

	TriangleIndexCount = 0;
	TangentSetup = FProcMeshTangent(0.f, 1.f, 0.f);
}

void AFurnitureRenderer::StopDrawing()
{
	ThisMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AFurnitureRenderer::DrawLeg(FVector StartingPoint, float LegHeight, float LegThickness, FVector BL, FVector TR)
{
	FVector LegCenter = { StartingPoint.X, StartingPoint.Y, LegHeight };
	float HalfLegThickness = LegThickness / 2;

	if (StartingPoint.X > ((TR.X + BL.X) / 2))
		LegCenter.X -= HalfLegThickness;
	else
		LegCenter.X += HalfLegThickness;


	if (StartingPoint.Y > ((TR.Y + BL.Y) / 2))
		LegCenter.Y -= HalfLegThickness;
	else
		LegCenter.Y += HalfLegThickness;

	FVector LegBL = { LegCenter.X - HalfLegThickness, LegCenter.Y - HalfLegThickness, LegCenter.Z };
	FVector LegBR = { LegCenter.X - HalfLegThickness, LegCenter.Y + HalfLegThickness, LegCenter.Z };
	FVector LegTR = { LegCenter.X + HalfLegThickness, LegCenter.Y + HalfLegThickness, LegCenter.Z };
	FVector LegTL = { LegCenter.X + HalfLegThickness, LegCenter.Y - HalfLegThickness, LegCenter.Z };


	FVector LegVertices[8] =
	{
		LegBL,
		LegBR,
		LegTR,
		LegTL,
		CalculatePointBelow(LegBL, LegHeight),
		CalculatePointBelow(LegBR, LegHeight),
		CalculatePointBelow(LegTR, LegHeight),
		CalculatePointBelow(LegTL, LegHeight)
	};

	DrawCube(LegVertices);
}

FVector AFurnitureRenderer::CalculatePointBelow(FVector Point, float Height)
{
	return FVector(Point.X, Point.Y, (Point.Z - Height));
}