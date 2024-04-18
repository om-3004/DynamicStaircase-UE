// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//staticMesh = sourceMesh;
	noOfStairs = 1;
	defaultSceneRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent = defaultSceneRoot;
	Location = FVector( 0, 0, 0 );
	//Dimensions = FVector(5.0f, 10.0f, 0.5f);
	Dimensions = FVector(1.0f, 4.0f, 0.3f);
	//railingDimensions = FVector(1.0f, 1.0f, 1.0f);
	railingDimensions = FVector(0.15f, 0.025f, 5.0f);
	
}

// Called when the game starts or when spawned
void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaircaseActor::PreEditChange(FProperty* PropertyAboutToChange) {
	Super::PreEditChange(PropertyAboutToChange);
	UE_LOG(LogTemp, Warning, TEXT("PreEditChange called..."));
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	FVector meshDimensions = stairsMesh->GetBounds().GetBox().GetSize();

	/*for (USceneComponent* Child : RootComponent->GetAttachChildren()) {
		if (Child) {
			Child->DestroyComponent();
			UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
			Child = nullptr;
		}
	}*/

	for (int i = 0; i < leftRailingMeshComponentsArray.Num(); i++) {
		if (leftRailingMeshComponentsArray[i]) {
			leftRailingMeshComponentsArray[i]->DestroyComponent();
			leftRailingMeshComponentsArray[i] = nullptr;
		}

		if (rightRailingMeshComponentsArray[i]) {
			rightRailingMeshComponentsArray[i]->DestroyComponent();
			rightRailingMeshComponentsArray[i] = nullptr;
		}

		if (stairsMeshComponentsArray[i]) {
			stairsMeshComponentsArray[i]->DestroyComponent();
			stairsMeshComponentsArray[i] = nullptr;
		}
	}
	leftRailingMeshComponentsArray.Empty();
	rightRailingMeshComponentsArray.Empty();
	stairsMeshComponentsArray.Empty();

	
	for (int i = 0; i < noOfStairs; i++) {
		UE_LOG(LogTemp, Warning, TEXT("onConstruction called..."));

		meshName = "MeshComponent " + FString::FromInt(i);
		UStaticMeshComponent* stairStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

		stairStaticMeshComponent->SetupAttachment(defaultSceneRoot);
		stairStaticMeshComponent->SetStaticMesh(stairsMesh);

		stairStaticMeshComponent->SetRelativeLocation(FVector(Dimensions.X * meshDimensions.X * i, 0, Dimensions.Z * meshDimensions.Z * i));
		stairStaticMeshComponent->SetRelativeScale3D(Dimensions);

		stairsMeshComponentsArray.Add(stairStaticMeshComponent);
		stairStaticMeshComponent->RegisterComponentWithWorld(GetWorld());

		
		if (Railing) {

			// FOR LEFT SIDE RAILINGS
			meshName = "leftRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			leftRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			leftRailingStaticMeshComponent->SetStaticMesh(railingMesh);

			leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y/2.3, meshDimensions.Z));
			leftRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);

			leftRailingMeshComponentsArray.Add(leftRailingStaticMeshComponent);


			meshName = "rightRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			rightRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			rightRailingStaticMeshComponent->SetStaticMesh(railingMesh);

			rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1, meshDimensions.Z));
			rightRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);

			rightRailingMeshComponentsArray.Add(rightRailingStaticMeshComponent);
		}
	}

}

void AStaircaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty called..."));
}
