// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	DoorOwner = GetOwner();
	if (!PressurePlate) { UE_LOG(LogTemp, Warning, TEXT("Pressureplate is missing")); }
	// ...


	
}

void UOpenDoor::OpenDoor()
{

	// Set rotation 
	if (!DoorOwner) { return; }
	DoorOwner->SetActorRotation(FRotator(0.0f, fOpenAngle, 0.0f));
}
void UOpenDoor::CloseDoor()
{
	

	// Set rotation 
	if (!DoorOwner) { return; }
	DoorOwner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the door every frame

	if(GetTotalMassOFActorsOnPlate() > 40.f) //TODO make into parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// check if its time to close the door
	if (GetWorld()->GetTimeSeconds()  - LastDoorOpenTime >  DoorCloseDelay)
	{
		CloseDoor();
	}

 
}

float UOpenDoor::GetTotalMassOFActorsOnPlate()
{
	float TotalMass = 0.0f;
	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { 

		return 0.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning,TEXT("%s"),*(Actor->GetName()))
	}
	return TotalMass;
}

