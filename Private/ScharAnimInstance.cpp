// Fill out your copyright notice in the Description page of Project Settings.


#include "ScharAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Schar.h"

void UScharAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Schar = Cast<ASchar>(Pawn);
		}
	}
}
void UScharAnimInstance::UpdateAnimationProperties()
{

	if (Pawn ==nullptr) 
		Pawn = TryGetPawnOwner();
	
	if (Pawn){


		const FRotator Rotation = Pawn->GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		FVector Speed = Pawn->GetVelocity();

		FVector LaterSpeed = FVector(Speed.X, Speed.Y, 0.f);

		MovementSpeed = LaterSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (Schar == nullptr)
		{
			Schar = Cast<ASchar>(Pawn);
		}
	}


}