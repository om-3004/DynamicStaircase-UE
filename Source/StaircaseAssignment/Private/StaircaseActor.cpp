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

void AStaircaseActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


	if (stairsMesh) {
		meshDimensions = stairsMesh->GetBounds().GetBox().GetSize();
	}

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

	
	for (int i = 0; i < noOfStairs; i++) {
		UE_LOG(LogTemp, Warning, TEXT("onConstruction called..."));

		meshName = "MeshComponent " + FString::FromInt(i);
		UStaticMeshComponent* stairStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

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
			stairStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X, StairsDimensions.Y, StairsDimensions.Z * (i+1)));
			stairStaticMeshComponent->SetRelativeLocation(FVector(StairsDimensions.X * meshDimensions.X * i, 0, ((StairsDimensions.Z * (i) * meshDimensions.Z) / 2)));
		}

		stairsMeshComponentsArray.Add(stairStaticMeshComponent);
		stairStaticMeshComponent->RegisterComponentWithWorld(GetWorld());

		
		if (Railing) {
			// FOR LEFT SIDE RAILINGS
			meshName = "leftRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			leftRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			leftRailingStaticMeshComponent->SetStaticMesh(railingMesh);

			if (stairsType == typeOfStairs::BoxStairs) {
				leftRailingStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i+1)));
				leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1, 
					(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z * (i+1)) / 2)) / (meshDimensions.Z * StairsDimensions.Z * (i+1))) * 100)));
				UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *leftRailingStaticMeshComponent->GetRelativeLocation().ToString());
			}
			else {
				leftRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);
				leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1, 
					(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}		

			leftRailingMeshComponentsArray.Add(leftRailingStaticMeshComponent);
			leftRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			// FOR RIGHT SIDE RAILINGS
			meshName = "rightRailingComponent " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			rightRailingStaticMeshComponent->SetupAttachment(stairStaticMeshComponent);
			rightRailingStaticMeshComponent->SetStaticMesh(railingMesh);

			if (stairsType == typeOfStairs::BoxStairs) {
				rightRailingStaticMeshComponent->SetRelativeScale3D(FVector(railingDimensions.X, railingDimensions.Y, railingDimensions.Z / (i + 1)));
				rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3,
					(((((meshDimensions.Z* StairsDimensions.Z* railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z * (i + 1)) / 2)) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
			}
			else {
				rightRailingStaticMeshComponent->SetRelativeScale3D(railingDimensions);
				rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3, 
					(((((meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) / 2) + ((meshDimensions.Z * StairsDimensions.Z) / 2)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
				//       (mesh * dimension of railing relative of stairs)-
			}

			/*UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *rightRailingStaticMeshComponent->GetRelativeLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("Scale: %s"), *rightRailingStaticMeshComponent->GetRelativeScale3D().ToString());*/

			rightRailingMeshComponentsArray.Add(rightRailingStaticMeshComponent);
			rightRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			// UPPER TITLED PART - LEFT
			meshName = "leftRailingUpper " + FString::FromInt(i);
			UStaticMeshComponent* leftRailingUpperStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			leftRailingUpperStaticMeshComponent->SetupAttachment(stairStaticMeshComponent); // leftRailingStaticMeshComponent -> relative.
			leftRailingUpperStaticMeshComponent->SetStaticMesh(railingMesh); // railingMesh -> change

			if (stairsType == typeOfStairs::BoxStairs) {

				double tanInverse = atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X));
				UE_LOG(LogTemp, Warning, TEXT("tanInverse: %lg"), tanInverse);

				double Cos = cos(tanInverse);
				UE_LOG(LogTemp, Warning, TEXT("Cos: %lg"), Cos);

				double val = 100 / Cos;
				UE_LOG(LogTemp, Warning, TEXT("Val: %lg"), val);

				//leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X + (StairsDimensions.X * 0.05), railingDimensions.Y, 0.15 / (i + 1)));
				leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(1.04403, railingDimensions.Y, 0.15 / (i + 1)));  // val / (StairsDimension.X * meshDimension.X) = 104.403/(1*100) = 1.04403

				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1,
					(((((0.15 * meshDimensions.Z* StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15 * (i+1))) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
			}
			else if (stairsType == typeOfStairs::ClosedStairs) {
				leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(1.04403, railingDimensions.Y, 0.15 / (i + 1)));  // val / (StairsDimension.X * meshDimension.X) = 104.403/(1*100) = 1.04403

				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1,
					(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}
			else { // OpenStairs
				if (StairsDimensions.X >= 1) {
					leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X * 2, railingDimensions.Y, 0.15));
				}
				else {
					leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X / 0.5, railingDimensions.Y, 0.15));  // >= 0.7
				}

				//leftRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X * 1.05, railingDimensions.Y, 0.15));

				leftRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3 * -1,
					(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}

			leftRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z * meshDimensions.Z)/(StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));

			leftRailingUpperArray.Add(leftRailingUpperStaticMeshComponent);
			leftRailingUpperStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			// UPPER TITLED PART - RIGHT
			meshName = "rightRailingUpper " + FString::FromInt(i);
			UStaticMeshComponent* rightRailingUpperStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *meshName);

			rightRailingUpperStaticMeshComponent->SetupAttachment(stairStaticMeshComponent); // leftRailingStaticMeshComponent -> relative.
			rightRailingUpperStaticMeshComponent->SetStaticMesh(railingMesh); // railingMesh -> change

			if (stairsType == typeOfStairs::BoxStairs) {

				double tanInverse = atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X));
				UE_LOG(LogTemp, Warning, TEXT("tanInverse: %lg"), tanInverse);

				double Cos = cos(tanInverse);
				UE_LOG(LogTemp, Warning, TEXT("Cos: %lg"), Cos);

				double val = 100 / Cos;
				UE_LOG(LogTemp, Warning, TEXT("Val: %lg"), val);

				//rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(diff, railingDimensions.Y, 0.15 / (i + 1)));
				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(1.04403, railingDimensions.Y, 0.15 / (i + 1)));  // val / (StairsDimension.X * meshDimension.X) = 104.403/(1*100) = 1.04403
				UE_LOG(LogTemp, Warning, TEXT("Scale: %s"), *rightRailingUpperStaticMeshComponent->GetRelativeScale3D().ToString());

				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3,
					(((((0.15 * meshDimensions.Z * StairsDimensions.Z) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15 * (i + 1))) / (meshDimensions.Z * StairsDimensions.Z * (i + 1))) * 100)));
			}
			else if (stairsType == typeOfStairs::ClosedStairs) {
				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(1.04403, railingDimensions.Y, 0.15 / (i + 1)));  // val / (StairsDimension.X * meshDimension.X) = 104.403/(1*100) = 1.04403

				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3,
					(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}
			else if (stairsType == typeOfStairs::OpenStairs) {
				double tanInverse1 = atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X));
				UE_LOG(LogTemp, Warning, TEXT("tanInverse: %lg"), tanInverse1);

				double Cos1 = cos(tanInverse1);
				UE_LOG(LogTemp, Warning, TEXT("Cos: %lg"), Cos1);

				double val1 = 100 / Cos1;
				UE_LOG(LogTemp, Warning, TEXT("Val1: %lg"), val1);

				double x = (StairsDimensions.X * meshDimensions.X * openStairsSpacing.X) - (StairsDimensions.X * meshDimensions.X);
				double z = (StairsDimensions.Z * meshDimensions.Z * openStairsSpacing.Z) - (StairsDimensions.Z * meshDimensions.Z);

				double tanInverse2 = atan(z / x);
				double Cos2 = cos(tanInverse2);
				double val2 = 20 / Cos2;
				UE_LOG(LogTemp, Warning, TEXT("Val2: %lg"), val2);

				//rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X * 2, railingDimensions.Y, 0.15));
				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(1.04403 + (val2/100), railingDimensions.Y, 0.15));
				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3,
						(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}
			else {

				double tanInverse1 = atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X));
				UE_LOG(LogTemp, Warning, TEXT("tanInverse: %lg"), tanInverse1);

				double Cos1 = cos(tanInverse1);
				UE_LOG(LogTemp, Warning, TEXT("Cos: %lg"), Cos1);

				double val1 = 100 / Cos1;
				UE_LOG(LogTemp, Warning, TEXT("Val: %lg"), val1);

				//double tanInverse2 = atan(() / ());

				rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(20+1.04403, railingDimensions.Y, 0.15));
				//if (StairsDimensions.X >= 1) {
				//	rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X * 2, railingDimensions.Y, 0.15));
				//}
				//else {
				//	rightRailingUpperStaticMeshComponent->SetRelativeScale3D(FVector(StairsDimensions.X / 0.5, railingDimensions.Y, 0.15));  // >= 0.7
				//}

				rightRailingUpperStaticMeshComponent->SetRelativeLocation(FVector(0, meshDimensions.Y / 2.3,
					(((((meshDimensions.Z * StairsDimensions.Z * 0.15) / 2) + (meshDimensions.Z * StairsDimensions.Z * railingDimensions.Z) + (meshDimensions.Z * 0.15)) / (meshDimensions.Z * StairsDimensions.Z)) * 100)));
			}

			rightRailingUpperStaticMeshComponent->SetRelativeRotation(FRotator((atan((StairsDimensions.Z * meshDimensions.Z) / (StairsDimensions.X * meshDimensions.X)) * 180) / 3.1415, 0.0f, 0.0f));

			rightRailingUpperArray.Add(rightRailingUpperStaticMeshComponent);
			rightRailingUpperStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		}
	}

}

void AStaircaseActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty called..."));
}
