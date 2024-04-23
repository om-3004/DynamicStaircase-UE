// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"

UENUM()
enum EtypeOfStairs {
	OpenStairs UMETA(DisplayName = "Open Stairs"),
	ClosedStairs UMETA(DisplayName = "Closed Stairs"),
	BoxStairs UMETA(DisplayName = "Box Stairs")
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase")
	TEnumAsByte<EtypeOfStairs> stairsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase", meta = (ClampMin = "1", UIMIN = "1"))
	int noOfStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifyStaircase")
	UStaticMesh* stairsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifyStaircase")
	FVector StairsDimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase", meta = (EditCondition = "stairsType==EtypeOfStairs::OpenStairs", ClampMin = "1.01", UIMin = "1.01"))
	float HorizontalSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyStaircase", meta = (EditCondition = "stairsType==EtypeOfStairs::OpenStairs", ClampMin = "1.01", UIMin = "1.01"))
	float VerticalSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyRailing")
	bool Railing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyRailing", meta=(EditCondition="Railing"))
	UStaticMesh* railingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifyRailing", meta = (EditCondition = "Railing"))
	UStaticMesh* railingHandleMesh;

	UPROPERTY()
	USceneComponent* defaultSceneRoot;

	UPROPERTY()
	FString meshName;

	UPROPERTY()
	TArray<UStaticMeshComponent*> stairsMeshComponentsArray;

	UPROPERTY()
	TArray<UStaticMeshComponent*> leftRailingMeshComponentsArray;

	UPROPERTY()
	TArray<UStaticMeshComponent*> rightRailingMeshComponentsArray;
	
	UPROPERTY()
	TArray<UStaticMeshComponent*> leftRailingUpperArray;

	UPROPERTY()
	TArray<UStaticMeshComponent*> rightRailingUpperArray;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FVector railingDimensions;

	UPROPERTY()
	FVector meshDimensions;
	
	UPROPERTY()
	FVector railingMeshDimensions;
	
	UPROPERTY()
	FVector railingHandlemeshDimensions;

	UFUNCTION()
	void destroyComponents();

	UFUNCTION()
	void setMeshDimensions();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
