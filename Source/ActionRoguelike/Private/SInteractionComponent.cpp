// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract() const
{

	AActor* MyOwner = GetOwner();

	FVector EyeLocation{};
	FRotator EyeRotation{};
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End{EyeLocation + (EyeRotation.Vector() * 200.f)};

	FCollisionObjectQueryParams ObjectQueryParams{};
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// FHitResult Hit{};
	// bool bBlockingHit{GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams)};

	float Radius{60.f};
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	TArray<FHitResult> Hits;
	bool bBlockingHit{GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
	 ObjectQueryParams, Shape)};
	FColor LineColor{bBlockingHit ? FColor::Red : FColor::Green};

	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 8, LineColor, false, 2.f);
		if (AActor* HitActor{Hit.GetActor()})
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn{Cast<APawn>(MyOwner)};
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);
}
