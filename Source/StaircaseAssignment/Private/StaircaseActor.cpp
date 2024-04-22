// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	noOfStairs = 1;
	defaultSceneRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent = defaultSceneRoot;
	Location = FVector(0, 0, 30);
	StairsDimensions = FVector(1.0f, 4.0f, 0.3f);
	railingDimensions = FVector(0.15f, 0.025f, 5.0f);
	openStairsSpacing = FVector(1.2f, 0, 1.2f);
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

void AStaircaseActor::destroyComponents() {
	for (int i = 0; i < leftRailingUpperArray.Num(); i++) {
		if (leftRailingUpperArray[i]) {
			leftRailingUpperArray[i]->DestroyComponent();
			leftRailingUpperArray[i] = nullptr;
		}
	}
	leftRailingUpperArray.Empty();

	for (int i = 0; i < rightRailingUpperArray.Num(); i++) {
		if (rightRailingUpperArray[i]) {
			rightRailingUpperArray[i]->DestroyComponent();
			rightRailingUpperArray[i] = nullptr;
		}
	}
	rightRailingUpperArray.Empty();

	for (int i = 0; i < leftRailingMeshComponentsArray.Num(); i++) {
		if (leftRailingMeshComponentsArray[i]) {
			leftRailingMeshComponentsArray[i]->DestroyComponent();
			leftRailingMeshComponentsArray[i] = nullptr;
		}
	}
	leftRailingMeshComponentsArray.Empty();

	for (int i = 0; i < rightRailingMeshComponentsArray.Num(); i++) {
		if (rightRailingMeshComponentsArray[i]) {
			rightRailingMeshComponentsArray[i]->DestroyComponent();
			rightRailingMeshComponentsArray[i] = nullptr;
		}
	}
	rightRailingMeshComponentsArray.Empty();

	for (int i = 0; i < stairsMeshComponentsArray.Num(); i++) {
		if (stairsMeshComponentsArray[i]) {
			stairsMeshComponentsArray[i]->DestroyComponent();
			stairsMeshComponentsArray[i] = nullptr;
		}
	}
	stairsMeshComponentsArray.Empty();
}

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (stairsMesh) {
		meshDimensions = stairsMesh->GetBounds().GetBox().GetSize();
	}
	if (railingMesh) {
		railingMeshDimensions = railingMesh->GetBounds().GetBox().GetSize();	
	}
	if (railingHandleMesh) {
		railingHandlemeshDimensions = railingHandleMesh->GetBounds().GetBox().GetSize();
	}
	
	destroyComponents();
	
	for (int i = 0; i < noOfStairs; i++) {
		UE_LOG(LogTemp, Warning, TEXT("onConstruction called..."));

		meshName = "stairsMeshComponent " + FString::FromInt(i);

		UStaticMeshComponent* stairStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);
		/*FVector AbsLenOfStairs = FVector(StairsDimensions.X * stairsMeshDimensions.X, StairsDimensions.Y * stairsMeshDimensions.Y, StairsDimensions.Z * stairsMeshDimensions.Z);
		FVector AbsLenOfRailing = FVector(railingDimensions.X * StairsDimensions.X * meshDimensions.X, railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y, railingDimensions.Z * StairsDimensions.Z * meshDimensions.Z);*/

		if(stairStaticMeshComponent) {
			stairStaticMeshComponent->SetupAttachment(defaultSceneRoot);
			stairStaticMeshComponent->SetStaticMesh(stairsMesh);

			if (stairsType == typeOfStairs::ClosedStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(StairsDimensions);
				stairStaticMeshComponent->SetRelativeLocation(FVector(StairsDimensions.X * meshDimensions.X * i, 0, StairsDimensions.Z * meshDimensions.Z * i));
			}
			else if (stairsType == typeOfStairs::OpenStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(StairsDimensions);
				stairStaticMeshComponent->SetRelativeLocation(FVector(StairsDimensions.X * meshDimensions.X * i * openStairsSpacing.X, 0, StairsDimensions.Z * meshDimensions.Z * i * openStairsSpacing.Z));
			}
			else if (stairsType == typeOfStairs::BoxStairs) {
				stairStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X, StairsDimensions.Y, StairsDimensions.Z * (i + 1)));
				stairStaticMeshComponent->SetRelativeLocation(FVector(StairsDimensions.X * meshDimensions.X * i, 0, ((StairsDimensions.Z * meshDimensions.Z * i) / 2)));
			}

			stairsMeshComponentsArray.Add(stairStaticMeshComponent);
			stairStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		}

		
		if (Railing) {
			// FOR LEFT SIDE RAILINGS
			meshName = "leftRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			if(leftRailingStaticMeshComponent) {
				leftRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
				leftRailingStaticMeshComponent->SetStaticMesh(railingMesh);

				if (stairsType == typeOfStairs::BoxStairs) {
					leftRailingStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i + 1)));

					leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((StairsDimensions.Y * meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * -100),
						(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z * (i + 1)) / 2)) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
				}
				else {
					leftRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);
					leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0,
						((((StairsDimensions.Y * meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * -100),
						(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
				}

				leftRailingMeshComponentsArray.Add(leftRailingStaticMeshComponent);
				leftRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
			}


			//------------------------------- FOR RIGHT SIDE RAILINGS -----------------------------------//
			meshName = "rightRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			rightRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			rightRailingStaticMeshComponent->SetStaticMesh(railingMesh);

			if (stairsType == typeOfStairs::BoxStairs) {
				rightRailingStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i + 1)));
				rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0,
					((((StairsDimensions.Y * meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * 100),
					(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z * (i + 1)) / 2)) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
			}
			else {
				rightRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);
				rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0, 
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * 100),
					(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}

			rightRailingMeshComponentsArray.Add(rightRailingStaticMeshComponent);
			rightRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			//--------------------------------- UPPER TITLED PART - LEFT ---------------------------------//
			meshName = "leftRailingUpper " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingUpperStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			leftRailingUpperStaticMeshComponent->SetupAttachment(stairStaticMeshComponent); // leftRailingStaticMeshComponent -> relative.
			leftRailingUpperStaticMeshComponent->SetStaticMesh(railingHandleMesh); // railingMesh -> change

			double tanInverse1 = atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X));
			double Cos1 = cos(tanInverse1);
			double val1 = (StairsDimensions.X * meshDimensions.X) / Cos1;

			if (stairsType == typeOfStairs::BoxStairs) {

				leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15 / (i + 1)));
				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0,
					((((StairsDimensions.Y * meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * -100),
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((StairsDimensions.Z * meshDimensions.Z * (i + 1))/2)) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
				leftRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z* meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));

				UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *leftRailingStaticMeshComponent->GetRelativeLocation().ToString());
			}
			else if (stairsType == typeOfStairs::ClosedStairs) {

				leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15));

				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, 
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * -100),
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((StairsDimensions.Z * meshDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
				leftRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));
			}
			else if (stairsType == typeOfStairs::OpenStairs) {

				double x = (StairsDimensions.X * meshDimensions.X * openStairsSpacing.X) - (StairsDimensions.X * meshDimensions.X);
				double z = (StairsDimensions.Z * meshDimensions.Z * openStairsSpacing.Z) - (StairsDimensions.Z * meshDimensions.Z);
				double tanInverse2 = atan(z / x);
				double Cos2 = cos(tanInverse2);
				double val2 = x / Cos2;

				leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)) + (val2 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15));

				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0,
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * -100),
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((meshDimensions.Z * StairsDimensions.Z)/2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
				leftRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan(((StairsDimensions.Z * meshDimensions.Z) + z) / ((StairsDimensions.X * meshDimensions.X) + x)) * 180) / 3.1415, 0.0f, 0.0f));

			}

			

			leftRailingUpperArray.Add(leftRailingUpperStaticMeshComponent);
			leftRailingUpperStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			//--------------------------------------- UPPER TITLED PART - RIGHT ---------------------------------//
			meshName = "rightRailingUpper " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingUpperStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			rightRailingUpperStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			rightRailingUpperStaticMeshComponent->SetStaticMesh(railingHandleMesh);

			if (stairsType == typeOfStairs::BoxStairs) {

				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15 / (i + 1)));

				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0,
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * 100),
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((StairsDimensions.Z * meshDimensions.Z * (i + 1)) / 2)) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));

				rightRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));
			}
			else if (stairsType == typeOfStairs::ClosedStairs) {

				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15));

				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, 
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * 100),
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((StairsDimensions.Z * meshDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));

				rightRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));
			}
			else if (stairsType == typeOfStairs::OpenStairs) {

				double x = (StairsDimensions.X * meshDimensions.X * openStairsSpacing.X) - (StairsDimensions.X * meshDimensions.X);
				UE_LOG(LogTemp, Warning, TEXT("x: %lg"), x);
				double z = (StairsDimensions.Z * meshDimensions.Z * openStairsSpacing.Z) - (StairsDimensions.Z * meshDimensions.Z);
				UE_LOG(LogTemp, Warning, TEXT("z: %lg"), z);
				double tanInverse2 = atan(z / x);
				UE_LOG(LogTemp, Warning, TEXT("tanInverse2: %lg"), tanInverse2);
				double Cos2 = cos(tanInverse2);
				UE_LOG(LogTemp, Warning, TEXT("Cos2: %lg"), Cos2);
				double val2 = x / Cos2;
				UE_LOG(LogTemp, Warning, TEXT("val2: %lg"), val2);

				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector((val1 / (StairsDimensions.X * meshDimensions.X)) + (val2 / (StairsDimensions.X * meshDimensions.X)), railingDimensions.Y, 0.15));


				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0,
					((((StairsDimensions.Y* meshDimensions.Y) / 2 - (railingDimensions.Y * StairsDimensions.Y * meshDimensions.Y * 2.5)) / (StairsDimensions.Y * meshDimensions.Y)) * 100),
					(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + ((meshDimensions.Z * StairsDimensions.Z)/2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
				rightRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan(((StairsDimensions.Z* meshDimensions.Z) + z) / ((StairsDimensions.X * meshDimensions.X) + x)) * 180) / 3.1415, 0.0f, 0.0f));
			}

			rightRailingUpperArray.Add(rightRailingUpperStaticMeshComponent);
			rightRailingUpperStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		}
	}

}

void AStaircaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty called..."));
}
