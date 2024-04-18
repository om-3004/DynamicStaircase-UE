// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"

UCLASS()
class STAIRCASEASSIGNMENT_API AStaircaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaircaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifyStaircase")
	int noOfStairs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifyStaircase")
	UStaticMesh* stairsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifyStaircase")
	FVector Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase")
	bool Railing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase", meta=(EditCondition="Railing"))
	UStaticMesh* railingMesh;

	USceneComponent* defaultSceneRoot;
	FString meshName;
	TArray<UStaticMeshComponent*> stairsMeshComponentsArray;
	TArray<UStaticMeshComponent*> leftRailingMeshComponentsArray;
	TArray<UStaticMeshComponent*> rightRailingMeshComponentsArray;
	FVector Location;
	FVector railingDimensions;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
