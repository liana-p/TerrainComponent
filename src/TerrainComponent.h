// Made by Liana Pigeot - https://github.com/nialna

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerlinNoiseComponent.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "Components/ActorComponent.h"
#include "TerrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWATER_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTerrainComponent();
	UFUNCTION(BlueprintCallable)
	void GenerateMap();

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	int NoiseResolution = 300;
	int TotalSizeToGenerate = 12000;
	int NoiseSamplesPerLine = TotalSizeToGenerate / NoiseResolution;
	int VerticesArraySize = NoiseSamplesPerLine * NoiseSamplesPerLine;

	float NoiseInputScale = 0.05; // Making this smaller will "stretch" the perlin noise terrain
	float NoiseOutputScale = 2000; // Making this bigger will scale the terrain's height

	UPerlinNoiseComponent* Noise;
	URuntimeMeshComponentStatic* RuntimeMesh;

	void GenerateVertices();
	void GenerateTriangles();
	void GenerateMesh();

	float GetNoiseValueForGridCoordinates(int x, int y);
	int GetIndexForGridCoordinates(int x, int y);
	FVector2D GetPositionForGridCoordinates(int x, int y);

	// Other things needed to generate the mesh
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> UV;
	TArray<FColor> VertexColors;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
