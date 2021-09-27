// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMUE5Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "libsm64inc.h"

//////////////////////////////////////////////////////////////////////////
// ASMUE5Character

void ASMUE5Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timer += DeltaSeconds;

	const FRotator Rotation = Controller->GetControlRotation();
	Inputs.CamLook = Rotation;

	if(Timer > 0)
	{
		Timer = -(1.0f / 30.0f);
		Mario->TickMario(Inputs);
		
		PMesh.bEnableCollision = false;
		PMesh.bSectionVisible = true;
		PMesh.ProcVertexBuffer.Reserve(Mario->buffers.numTrianglesUsed * 3);
		PMesh.ProcIndexBuffer.Reserve(Mario->buffers.numTrianglesUsed * 3);

		TArray<FVector> Points;
		Points.Add(FVector(-99999));
		Points.Add(FVector(99999));
		
		for(int i = 0; i < Mario->buffers.numTrianglesUsed; i++)
		{
			FProcMeshVertex v1, v2, v3;
			v1.Position = FVector(Mario->buffers.position[(i*3*3) + 0], Mario->buffers.position[(i*3*3) + 1], -Mario->buffers.position[(i*3*3) + 2]);
			v2.Position = FVector(Mario->buffers.position[(i*3*3) + 3], Mario->buffers.position[(i*3*3) + 4], -Mario->buffers.position[(i*3*3) + 5]);
			v3.Position = FVector(Mario->buffers.position[(i*3*3) + 6], Mario->buffers.position[(i*3*3) + 7], -Mario->buffers.position[(i*3*3) + 8]);

			v1.Normal = FVector(Mario->buffers.normal[(i*3*3) + 0], Mario->buffers.normal[(i*3*3) + 1], -Mario->buffers.normal[(i*3*3) + 2]);
			v2.Normal = FVector(Mario->buffers.normal[(i*3*3) + 3], Mario->buffers.normal[(i*3*3) + 4], -Mario->buffers.normal[(i*3*3) + 5]);
			v3.Normal = FVector(Mario->buffers.normal[(i*3*3) + 6], Mario->buffers.normal[(i*3*3) + 7], -Mario->buffers.normal[(i*3*3) + 8]);

			v1.Color = FColor(Mario->buffers.color[(i*3*3) + 0] * 255, Mario->buffers.color[(i*3*3) + 1] * 255, Mario->buffers.color[(i*3*3) + 2] * 255);
			v2.Color = FColor(Mario->buffers.color[(i*3*3) + 3] * 255, Mario->buffers.color[(i*3*3) + 4] * 255, Mario->buffers.color[(i*3*3) + 5] * 255);
			v3.Color = FColor(Mario->buffers.color[(i*3*3) + 6] * 255, Mario->buffers.color[(i*3*3) + 7] * 255, Mario->buffers.color[(i*3*3) + 8] * 255);

			v1.UV0 = FVector2D(Mario->buffers.uv[(i*3*2) + 0], Mario->buffers.uv[(i*3*2) + 1]);
			v2.UV0 = FVector2D(Mario->buffers.uv[(i*3*2) + 2], Mario->buffers.uv[(i*3*2) + 3]);
			v3.UV0 = FVector2D(Mario->buffers.uv[(i*3*2) + 4], Mario->buffers.uv[(i*3*2) + 5]);
		
			PMesh.ProcVertexBuffer.Add(v1);
			PMesh.ProcVertexBuffer.Add(v2);
			PMesh.ProcVertexBuffer.Add(v3);

			PMesh.ProcIndexBuffer.Add((i*3) + 0);
			PMesh.ProcIndexBuffer.Add((i*3) + 1);
			PMesh.ProcIndexBuffer.Add((i*3) + 2);
		}

		FBox LocalBox = FBox(Points);

		PMesh.SectionLocalBox = LocalBox;

		MarioMesh->SetProcMeshSection(0, PMesh);

		//Points.Empty();
		PMesh.ProcVertexBuffer.Empty();
		PMesh.ProcIndexBuffer.Empty();

		Points.Empty();
	}
	
	MarioMesh->SetWorldLocation(FVector(0), false);
}

ASMUE5Character::ASMUE5Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Mario = CreateDefaultSubobject<UMarioComponent>(TEXT("Mario"));
	MarioMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MarioMesh"));
	MarioMesh->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASMUE5Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASMUE5Character::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASMUE5Character::StopJump);

	PlayerInputComponent->BindAction("B", IE_Pressed, this, &ASMUE5Character::StartB);
	PlayerInputComponent->BindAction("B", IE_Released, this, &ASMUE5Character::StopB);
	
	PlayerInputComponent->BindAction("Z", IE_Pressed, this, &ASMUE5Character::StartZ);
	PlayerInputComponent->BindAction("Z", IE_Released, this, &ASMUE5Character::StopZ);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASMUE5Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASMUE5Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASMUE5Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASMUE5Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASMUE5Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASMUE5Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASMUE5Character::OnResetVR);
}


void ASMUE5Character::OnResetVR()
{
	// If SMUE5 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in SMUE5.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASMUE5Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	//Jump();
	Inputs.ButtonA = true;
}

void ASMUE5Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	Inputs.ButtonA = false;
	//StopJumping();
}

void ASMUE5Character::StartJump()
{
	Inputs.ButtonA = true;
}

void ASMUE5Character::StopJump()
{
	Inputs.ButtonA = false;
}

void ASMUE5Character::StartB()
{
	Inputs.ButtonB = true;
}

void ASMUE5Character::StopB()
{
	Inputs.ButtonB = false;
}

void ASMUE5Character::StartZ()
{
	Inputs.ButtonZ = true;
}

void ASMUE5Character::StopZ()
{
	Inputs.ButtonZ = false;
}

void ASMUE5Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASMUE5Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASMUE5Character::MoveForward(float Value)
{
	/*
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}*/
	Inputs.Stick.X = Value;
}

void ASMUE5Character::MoveRight(float Value)
{
	/*
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}*/
	Inputs.Stick.Y = Value;
}
