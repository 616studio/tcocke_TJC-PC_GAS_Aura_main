// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/X_GAS_DataTypes.h"
#include "X_GameplayEffectActor_Base.generated.h"

class UGameplayEffect;
/**
 * <summary>
 * Actor used to apply Gameplay Effects to targets upon overlapping.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Retrieves a target's <c>UAbilitySystemComponent</c> upon overlap and tells it to apply a <c>UGameplayEffect</c> to itself.</description></item>
 * <item><description><b>UGameplayEffect (The Blueprint):</b> The static data definition. It just says "Reduce Health by 50." It does nothing on its own.</description></item>
 * <item><description><b>FGameplayEffectSpec (The Payload):</b> Takes the static Blueprint and injects your current level and stats into it. It says, "The Actor is Level 10, so reduce Health by 500."</description></item>
 * <item><description><b>FActiveGameplayEffectHandle (The Receipt):</b> The ID tag of the math after it has been successfully attached to a player. You use this ID to find the math and turn it off later.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API AX_GameplayEffectActor_Base : public AActor
{
	GENERATED_BODY()

public:
	AX_GameplayEffectActor_Base();

protected:
	virtual void BeginPlay() override;
	
	/**
	 * <summary>
	 * Native UE5 virtual overlap notifications derived from base AActor.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Invoked by the engine game thread when any attached <c>UPrimitiveComponent</c> (with <c>bGenerateOverlapEvents = true</c>) detects a collision overlap.</description></item>
	 * <item><description>Calling <c>Super::NotifyActorBeginOverlap</c> internally broadcasts the <c>OnActorBeginOverlap</c> dynamic delegate and dispatches the Blueprint <c>ReceiveActorBeginOverlap</c> event.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	/**
	 * <summary>
	 * Triggered via <c>NotifyActorBeginOverlap</c>, which fires when any collision component attached to the root of this Actor triggers an overlap event.
	 * </summary>
	 * <param name="TargetActor">[<c>AActor*</c>]: The Actor overlapping with this GE Actor.</param>
	 */
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	/**
	 * <summary>
	 * Triggered via <c>NotifyActorEndOverlap</c>, which fires when any collision component attached to the root of this Actor triggers an overlap event.
	 * </summary>
	 * <param name="TargetActor">[<c>AActor*</c>]: The Actor ending overlap with this GE Actor.</param>
	 */
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	/**
	 * <summary>
	 * The Level of the Gameplay Effect to be applied. 
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Exposed to Blueprints so designers can scale the power of the Gameplay Effect.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects")
	float GameplayEffectLevel = 1.0f;

	/**
	 * <summary>
	 * Specify whether this Actor should be destroyed once all targets have stopped overlapping with this Actor and their Gameplay Effects are removed.
	 * </summary> 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects")
	bool bDestroyOnEffectRemoval = false;

	/** 
	 * <summary>
	 * Specify whether this Actor should be destroyed once GE application has finished.
	 * </summary> 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects")
	bool bDestroyOnEffectApplication = false;

#pragma region Instant Gameplay Effects
	
	/**
	 * <summary>
	 * INSTANT EFFECTS: Best used for permanent Base Value Attribute changes.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Examples:  taking raw damage from a spike trap, or picking up a health potion to instantly restore flat HP.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Instant")
	EGameplayEffectApplicationPolicy InstantGameplayEffectApplicationPolicy;
	
#pragma endregion Instant Gameplay Effects
	
#pragma region Duration Gameplay Effects
	
	/**
	 * <summary>
	 * DURATION EFFECTS: Best used for temporary buffs or debuffs that expire on their own.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Unlike Infinite effects, Duration effects generally do NOT need a Removal Policy. The engine's internal timer automatically destroys the "Receipt" (Active Handle) the moment the duration expires.</description></item>
	 * <item><description>Examples:  a 5-second speed boost shrine, or a 10-second poison damage-over-time (DoT) tick.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Duration")
	EGameplayEffectApplicationPolicy DurationGameplayEffectApplicationPolicy;
	
#pragma endregion Duration Gameplay Effects
	
#pragma region Infinite Gameplay Effects
	
	/**
	 * <summary>
	 * INFINITE EFFECTS: Best used for zone-based logic or permanent afflictions requiring a cure.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Example:  standing inside a pool of lava (damage applied continuously until you walk out).</description></item>
	 * </list>
	 * </remarks>
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Infinite")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Infinite")
	EGameplayEffectApplicationPolicy InfiniteGameplayEffectApplicationPolicy = EGameplayEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Infinite")
	EGameplayEffectRemovalPolicy InfiniteGameplayEffectRemovalPolicy = EGameplayEffectRemovalPolicy::RemoveOnEndOverlap;
	
	/** 
	 * <summary>
	 * Used to determine the amount of stacks of the infinite effect that should be removed when the Actor leaves the component overlap.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>-1</b> = Remove <b>ALL</b> stacks.</description></item>
	 * <item><description><b>1</b> = Remove a single stack.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|GAS|Gameplay Effects|Infinite")
	int32 InfiniteEffectRemovalStacks = -1;
	
#pragma endregion Infinite Gameplay Effects

	/**
	* <summary>
	* Used to track Infinite Gameplay Effects applied to overlapping actors in order to properly execute their removal when overlapping ends. 
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description>Maps the <c>FActiveGameplayEffectHandle</c> (Gameplay Effect receipt) of the applied Gameplay Effect to the overlapping actor's ASC.</description></item>
	* <item><description>When the actor ends its overlap, we look up their ASC in this map to find their receipt, and use it to cancel the effect (if specified to do so with its associated <c>EGameplayEffectApplicationPolicy</c>).</description></item>
	* <item><description><b>Garbage Collection Safety:</b> Using <c>TWeakObjectPtr</c> is crucial here to prevent crashes in the event the overlapped actor is destroyed while the Gameplay Effect is still applied.</description></item>
	* </list>
	* </remarks>
	*/
	TMap<FActiveGameplayEffectHandle, TWeakObjectPtr<UAbilitySystemComponent>> ActiveGameplayEffectHandles;

	/**
	 * <summary>
	 * The core GAS application pipeline for world-placed actors.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Takes a <c>TargetActor</c> and attempts to find its Ability System Component.</description></item>
	 * <item><description>Builds the <c>FGameplayEffectSpec</c> (the payload) from the provided Blueprint class and our <c>ActorLevel</c>.</description></item>
	 * <item><description>Tells the Target's ASC to apply the spec to itself.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="TargetActor">[<c>AActor*</c>]: The Actor we are attempting to apply the effect to.</param>
	 * <param name="GameplayEffectClassToApply">[<c>TSubclassOf</c>(<c>UGameplayEffect</c>)]: The specific Blueprint Gameplay Effect class to build the Spec from.</param>
	 * <returns>[<c>bool</c>]: <c>TRUE</c> if the effect was successfully applied; <c>FALSE</c> if it failed (ex: target has no ASC).</returns>
	 */
	bool ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClassToApply);

};
