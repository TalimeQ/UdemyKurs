// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FindPhysicsComponent();
	SetupInputComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics handel attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{


				// move the object that we are holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
	// See what we hit
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab is pressed"))
		auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
		// try and reach any actors with physics body collision channel set
		// if we hit something then attach a physics handle
		//TODO attach handle
	if (!PhysicsHandle) { return; }
	if (ActorHit) {
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None, // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allows rotation
		);
	}
}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release is pressed"))
		// release physcis handle
		if (!PhysicsHandle) { return; }
		PhysicsHandle->ReleaseComponent();
}
void UGrabber::FindPhysicsComponent()
{
	/// look for attached physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// physicshandle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physical handle missing for %s"), *(this->GetOwner()->GetName()))
	}
}
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// input component is found
		// bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component missing for %s"), *(this->GetOwner()->GetName()))
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{


	// Setup Query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	/// Ray-cast out to reach a distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is hit"), *(ActorHit->GetName()))
	}
	return Hit;
}

const FVector UGrabber::GetLineTraceEnd()
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//UE_LOG(LogTemp,Error,TEXT("Player view point location %s , player view point rotation %s"),*PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString())
	///Draw a red trace in the world to visualise
	
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * fReach;
}

const FVector UGrabber::GetLineTraceStart()
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//UE_LOG(LogTemp,Error,TEXT("Player view point location %s , player view point rotation %s"),*PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString())
	///Draw a red trace in the world to visualise

	return PlayerViewPointLocation;
}
