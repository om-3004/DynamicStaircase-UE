// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor() : noOfStairs{ 1 }, defaultSceneRoot{ CreateDefaultSubobject<USceneComponent>("Default Scene Root") }, Location{ 0, 0, 30 }, StairsDimensions{ 1.0f, 4.0f, 0.3f }, railingDimensions{ 0.15f, 0.025f, 5.0f }, HorizontalSpacing{ 1.2f }, VerticalSpacing{ 1.2f }
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = defaultSceneRoot;
}

// Called when the game starts or when spawned
void AStaircaseActor::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void AStaircaseActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AStaircaseActor::PreEditChange(FProperty* PropertyAboutToChange) {
	Super::PreEditChange(PropertyAboutToChange);
	UE_LOG(LogTemp, Warning, TEXT("PreEditChange called..."));
}

void AStaircaseActor::destroyComponents() {
	for (int i = 0; i < leftRailingHandleMeshComponentsArray.Num(); i++) {
		if (leftRailingHandleMeshComponentsArray[i]) {
			leftRailingHandleMeshComponentsArray[i]->DestroyComponent();
			leftRailingHandleMeshComponentsArray[i] = nullptr;
		}
	}
	leftRailingHandleMeshComponentsArray.Empty();

	for (int i = 0; i < rightRailingHandleMeshComponentsArray.Num(); i++) {
		if (rightRailingHandleMeshComponentsArray[i]) {
			rightRailingHandleMeshComponentsArray[i]->DestroyComponent();
			rightRailingHandleMeshComponentsArray[i] = nullptr;
		}
	}
	rightRailingHandleMeshComponentsArray.Empty();

	for (int i = 0; i < leftRailingPillarMeshComponentsArray.Num(); i++) {
		if (leftRailingPillarMeshComponentsArray[i]) {
			leftRailingPillarMeshComponentsArray[i]->DestroyComponent();
			leftRailingPillarMeshComponentsArray[i] = nullptr;
		}
	}
	leftRailingPillarMeshComponentsArray.Empty();

	for (int i = 0; i < rightRailingPillarMeshComponentsArray.Num(); i++) {
		if (rightRailingPillarMeshComponentsArray[i]) {
			rightRailingPillarMeshComponentsArray[i]->DestroyComponent();
			rightRailingPillarMeshComponentsArray[i] = nullptr;
		}
	}
	rightRailingPillarMeshComponentsArray.Empty();

	for (int i = 0; i < stairsMeshComponentsArray.Num(); i++) {
		if (stairsMeshComponentsArray[i]) {
			stairsMeshComponentsArray[i]->DestroyComponent();
			stairsMeshComponentsArray[i] = nullptr;
		}
	}
	stairsMeshComponentsArray.Empty();
}

void AStaircaseActor::setMeshDimensions() {
	if (stairsMesh) {
		stairMeshDimensions = stairsMesh->GetBounds().GetBox().GetSize();
	}
	if (railingMesh) {
		railingMeshDimensions = railingMesh->GetBounds().GetBox().GetSize();
	}
	if (railingHandleMesh) {
		railingHandlemeshDimensions = railingHandleMesh->GetBounds().GetBox().GetSize();
	}
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	destroyComponents();
	setMeshDimensions();

	FVector AbsLenOfStairs = { StairsDimensions.X * stairMeshDimensions.X , StairsDimensions.Y * stairMeshDimensions.Y , StairsDimensions.Z * stairMeshDimensions.Z };
	FVector AbsLenOfRailingPillar = { railingDimensions.X * StairsDimensions.X * railingMeshDimensions.X, railingDimensions.Y * StairsDimensions.Y * railingMeshDimensions.Y, railingDimensions.Z * StairsDimensions.Z * railingMeshDimensions.Z };
	FVector AbsLenofRailingHandle = {};
	
	for (int i = 0; i < noOfStairs; i++) {
		UE_LOG(LogTemp, Warning, TEXT("onConstruction called...%d"), i);

		meshName = "stairsMeshComponent " + FString::FromInt(i);

		UStaticMeshComponent* stairStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

		if(stairStaticMeshComponent) {
			stairStaticMeshComponent->SetupAttachment(defaultSceneRoot);
			stairStaticMeshComponent->SetStaticMesh(stairsMesh);

			if (stairsType == EtypeOfStairs::ClosedStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(StairsDimensions);
				stairStaticMeshComponent->SetRelativeLocation(FVector(AbsLenOfStairs.X * i, 0, AbsLenOfStairs.Z * i));
			}
			else if (stairsType == EtypeOfStairs::OpenStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(StairsDimensions);
				stairStaticMeshComponent->SetRelativeLocation(FVector(AbsLenOfStairs.X * i * HorizontalSpacing, 0, AbsLenOfStairs.Z * i * VerticalSpacing));
			}
			else if (stairsType == EtypeOfStairs::BoxStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X, StairsDimensions.Y, StairsDimensions.Z * (i + 1)));
				stairStaticMeshComponent->SetRelativeLocation(FVector(AbsLenOfStairs.X * i, 0, ((AbsLenOfStairs.Z * i) / 2)));
			}

			stairsMeshComponentsArray.Add(stairStaticMeshComponent);
			stairStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		}

		
		if (Railing) {
			// FOR LEFT SIDE RAILINGS
			meshName = "leftRailingPillar " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingPillarStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			if(leftRailingPillarStaticMeshComponent) {
				leftRailingPillarStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
				leftRailingPillarStaticMeshComponent->SetStaticMesh(railingMesh);

				if (stairsType == EtypeOfStairs::BoxStairs) {
					leftRailingPillarStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i + 1)));

					leftRailingPillarStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * -100),
						(((((AbsLenOfRailingPillar.Z) / 2) + ((AbsLenOfStairs.Z * (i + 1)) / 2)) / (AbsLenOfStairs.Z * (i + 1))) * 100)));
				}
				else {
					leftRailingPillarStaticMeshComponent->SetRelativeScale3D(railingDimensions);
					leftRailingPillarStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * -100),
						(((((AbsLenOfRailingPillar.Z) / 2) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));
				}

				leftRailingPillarMeshComponentsArray.Add(leftRailingPillarStaticMeshComponent);
				leftRailingPillarStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			}


			//------------------------------- FOR RIGHT SIDE RAILINGS -----------------------------------//
			meshName = "rightRailingPillar " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingPillarStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			if(rightRailingPillarStaticMeshComponent) {
				rightRailingPillarStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
				rightRailingPillarStaticMeshComponent->SetStaticMesh(railingMesh);

				if (stairsType == EtypeOfStairs::BoxStairs) {
					rightRailingPillarStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i + 1)));
					rightRailingPillarStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * 100),
						(((((AbsLenOfRailingPillar.Z) / 2) + ((AbsLenOfStairs.Z * (i + 1)) / 2)) / (AbsLenOfStairs.Z * (i + 1))) * 100)));
				}
				else {
					rightRailingPillarStaticMeshComponent->SetRelativeScale3D(railingDimensions);
					rightRailingPillarStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * 100),
						(((((AbsLenOfRailingPillar.Z) / 2) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));
				}

				rightRailingPillarMeshComponentsArray.Add(rightRailingPillarStaticMeshComponent);
				rightRailingPillarStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			}


			//--------------------------------- UPPER TITLED PART - LEFT ---------------------------------//
			meshName = "leftRailingHandle " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingHandleStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			if(leftRailingHandleStaticMeshComponent) {
				leftRailingHandleStaticMeshComponent->SetupAttachment(stairStaticMeshComponent); // leftRailingStaticMeshComponent -> relative.
				leftRailingHandleStaticMeshComponent->SetStaticMesh(railingHandleMesh); // railingMesh -> change

				double tanInverse1 = atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X));
				double Cos1 = cos(tanInverse1);
				double val1 = (AbsLenOfStairs.X) / Cos1;

				if (stairsType == EtypeOfStairs::BoxStairs) {

					leftRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15 / (i + 1)));
					leftRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * -100),
						(((((0.15 * AbsLenOfStairs.Z) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z * (i + 1)) / 2)) / (AbsLenOfStairs.Z * (i + 1))) * 100)));

					leftRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X)) * 180) / 3.1415, 0.0f, 0.0f));

					//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *leftRailingStaticMeshComponent->GetRelativeLocation().ToString());
				}
				else if (stairsType == EtypeOfStairs::ClosedStairs) {

					leftRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15));

					leftRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * -100),
						(((((0.15 * AbsLenOfStairs.Z) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));
					leftRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X)) * 180) / 3.1415, 0.0f, 0.0f));
				}
				else if (stairsType == EtypeOfStairs::OpenStairs) {

					double x = (AbsLenOfStairs.X * HorizontalSpacing) - (AbsLenOfStairs.X);
					double z = (AbsLenOfStairs.Z * VerticalSpacing) - (AbsLenOfStairs.Z);
					double tanInverse2 = atan(z / x);
					double Cos2 = cos(tanInverse2);
					double val2 = x / Cos2;

					leftRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)) + (val2 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15));

					leftRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * -100),
						(((((0.15 * AbsLenOfStairs.Z) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));
					leftRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan(((AbsLenOfStairs.Z) + z) / ((AbsLenOfStairs.X) + x)) * 180) / 3.1415, 0.0f, 0.0f));

				}

				leftRailingHandleMeshComponentsArray.Add(leftRailingHandleStaticMeshComponent);
				leftRailingHandleStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			}


			//--------------------------------------- UPPER TITLED PART - RIGHT ---------------------------------//
			meshName = "rightRailingHandle " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingHandleStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			if(rightRailingHandleStaticMeshComponent) {
				rightRailingHandleStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
				rightRailingHandleStaticMeshComponent->SetStaticMesh(railingHandleMesh);

				double tanInverse1 = atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X));
				double Cos1 = cos(tanInverse1);
				double val1 = (AbsLenOfStairs.X) / Cos1;

				if (stairsType == EtypeOfStairs::BoxStairs) {

					rightRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15 / (i + 1)));

					rightRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * 100),
						(((((0.15 * AbsLenOfStairs.Z) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z * (i + 1)) / 2)) / (AbsLenOfStairs.Z * (i + 1))) * 100)));

					rightRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X)) * 180) / 3.1415, 0.0f, 0.0f));
				}
				else if (stairsType == EtypeOfStairs::ClosedStairs) {

					rightRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15));

					rightRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * 100),
						(((((0.15 * AbsLenOfStairs.Z) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));

					rightRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan((AbsLenOfStairs.Z) / (AbsLenOfStairs.X)) * 180) / 3.1415, 0.0f, 0.0f));
				}
				else if (stairsType == EtypeOfStairs::OpenStairs) {

					double x = (AbsLenOfStairs.X * HorizontalSpacing) - (AbsLenOfStairs.X);
					UE_LOG(LogTemp, Warning, TEXT("x: %lg"), x);
					double z = (AbsLenOfStairs.Z * VerticalSpacing) - (AbsLenOfStairs.Z);
					UE_LOG(LogTemp, Warning, TEXT("z: %lg"), z);
					double tanInverse2 = atan(z / x);
					UE_LOG(LogTemp, Warning, TEXT("tanInverse2: %lg"), tanInverse2);
					double Cos2 = cos(tanInverse2);
					UE_LOG(LogTemp, Warning, TEXT("Cos2: %lg"), Cos2);
					double val2 = x / Cos2;
					UE_LOG(LogTemp, Warning, TEXT("val2: %lg"), val2);

					rightRailingHandleStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (AbsLenOfStairs.X)) + (val2 / (AbsLenOfStairs.X)), railingDimensions.Y, 0.15));


					rightRailingHandleStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((AbsLenOfStairs.Y) / 2 - (AbsLenOfRailingPillar.Y * 2.5)) / (AbsLenOfStairs.Y)) * 100),
						(((((AbsLenOfStairs.Z * 0.15) / 2) + (AbsLenOfRailingPillar.Z) + ((AbsLenOfStairs.Z) / 2)) / (AbsLenOfStairs.Z)) * 100)));
					rightRailingHandleStaticMeshComponent->SetRelativeRotation(FRotator((atan(((AbsLenOfStairs.Z) + z) / ((AbsLenOfStairs.X) + x)) * 180) / 3.1415, 0.0f, 0.0f));
				}

				rightRailingHandleMeshComponentsArray.Add(rightRailingHandleStaticMeshComponent);
				rightRailingHandleStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			}
		}
	}

}

void AStaircaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty called..."));
}
