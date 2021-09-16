#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Tranform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	FVector InterpolateDerivative(float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACINGGAME_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

	public:	
	// Sets default values for this component's properties
	UGoKartMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	float ClientSimulatedTime;
	FVector ClientStartVelocity;
	FTransform ClientStartTransform;
	TArray<FGoKartMove> UnacknowledgedMoves;

	UPROPERTY()
	UGoKartMovementComponent* MovementComponent;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UPROPERTY()
	USceneComponent* MeshOffsetRoot;
	
	void ClearAcknowledgeMoves(FGoKartMove LastMove);
	void UpdateServerState(const FGoKartMove& Move);
	void ClientTick(float DeltaTime);
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void InterpolateLocation(const FHermiteCubicSpline &Spline, float LerpRatio);
	void InterpolateVelocity(const FHermiteCubicSpline &Spline, float LerpRatio);
	void InterpolateRotation(float LerpRatio);
	float VelocityToDerivative();
	FHermiteCubicSpline CreateSpline();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	UFUNCTION()
	void OnRep_ServerState();

	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
};