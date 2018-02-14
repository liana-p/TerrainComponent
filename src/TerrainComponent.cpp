// Made by Liana Pigeot - https://github.com/nialna

#include "TerrainComponent.h"


// Sets default values for this component's properties
UTerrainComponent::UTerrainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTerrainComponent::GenerateMap()
{
	Normals.Init(FVector(0, 0, 1), VerticesArraySize);
	Tangents.Init(FRuntimeMeshTangent(0, -1, 0), VerticesArraySize);
	UV.Init(FVector2D(0, 0), VerticesArraySize);
	VertexColors.Init(FColor::White, VerticesArraySize);

	GenerateVertices();
	GenerateTriangles();
	GenerateMesh();
}

void UTerrainComponent::GenerateVertices() {
	Vertices.Init(FVector(0, 0, 0), VerticesArraySize);
	for (int y = 0; y < NoiseSamplesPerLine; y++) {
		for (int x = 0; x < NoiseSamplesPerLine; x++) {
			float NoiseResult = GetNoiseValueForGridCoordinates(x, y);
			int index = GetIndexForGridCoordinates(x, y);
			FVector2D Position = GetPositionForGridCoordinates(x, y);
			Vertices[index] = FVector(Position.X, Position.Y, NoiseResult);
			UV[index] = FVector2D(x, y);
		}
	}
}

void UTerrainComponent::GenerateMesh() {
	RuntimeMesh->CreateMeshSection(0,
		Vertices,
		Triangles,
		Normals,
		UV,
		VertexColors,
		Tangents,
		true, EUpdateFrequency::Infrequent);
}

void UTerrainComponent::GenerateTriangles() {
	int QuadSize = 6; // This is the number of triangle indexes making up a quad (square section of the grid)
	int NumberOfQuadsPerLine = NoiseSamplesPerLine - 1; // We have one less quad per line than the amount of vertices, since each vertex is the start of a quad except the last ones
														// In our triangles array, we need 6 values per quad
	int TrianglesArraySize = NumberOfQuadsPerLine * NumberOfQuadsPerLine * QuadSize;
	Triangles.Init(0, TrianglesArraySize);

	for (int y = 0; y < NumberOfQuadsPerLine; y++) {
		for (int x = 0; x < NumberOfQuadsPerLine; x++) {
			int QuadIndex = x + y * NumberOfQuadsPerLine;
			int TriangleIndex = QuadIndex * QuadSize;

			// Getting the indexes of the four vertices making up this quad
			int bottomLeftIndex = GetIndexForGridCoordinates(x, y);
			int topLeftIndex = GetIndexForGridCoordinates(x, y + 1);
			int topRightIndex = GetIndexForGridCoordinates(x + 1, y + 1);
			int bottomRightIndex = GetIndexForGridCoordinates(x + 1, y);

			// Assigning the 6 triangle points to the corresponding vertex indexes, by going counter-clockwise.
			Triangles[TriangleIndex] = bottomLeftIndex;
			Triangles[TriangleIndex + 1] = topLeftIndex;
			Triangles[TriangleIndex + 2] = topRightIndex;
			Triangles[TriangleIndex + 3] = bottomLeftIndex;
			Triangles[TriangleIndex + 4] = topRightIndex;
			Triangles[TriangleIndex + 5] = bottomRightIndex;
		}
	}
}

// Returns the scaled noise value for grid coordinates [x,y]
float UTerrainComponent::GetNoiseValueForGridCoordinates(int x, int y) {
	return Noise->GetValue(
		(x * NoiseInputScale) + 0.1,
		(y * NoiseInputScale) + 0.1,
		1.0
	) * NoiseOutputScale;
}

int UTerrainComponent::GetIndexForGridCoordinates(int x, int y) {
	return x + y * NoiseSamplesPerLine;
}

FVector2D UTerrainComponent::GetPositionForGridCoordinates(int x, int y) {
	return FVector2D(
		x * NoiseResolution,
		y * NoiseResolution
	);
}

// Called when the game starts
void UTerrainComponent::BeginPlay()
{
	Super::BeginPlay();
	Noise = Cast<UPerlinNoiseComponent>(GetOwner()->GetComponentByClass(UPerlinNoiseComponent::StaticClass()));
	RuntimeMesh = Cast<URuntimeMeshComponent>(GetOwner()->GetComponentByClass(URuntimeMeshComponent::StaticClass()));
	// ...
	
}


// Called every frame
void UTerrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

