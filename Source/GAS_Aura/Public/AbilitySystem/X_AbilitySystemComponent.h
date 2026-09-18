// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS_Aura/UtilityClasses/X_CustomDelegates.h"
#include "X_AbilitySystemComponent.generated.h"

/**
 * <summary>
 * Extends UAbilitySystemComponent to support Enhanced Input routing, MVC UI decoupling, and Server-to-Client combat data synchronization.</summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>Input Routing:</b> Maps Gameplay Abilities to Enhanced Input Actions using Gameplay Tags.</description></item>
 * <item><description><b>UI Decoupling:</b> Intercepts and rebroadcasts Gameplay Effect Asset Tags to Controllers.</description></item>
 * <item><description><b>Network Sync:</b> Uses lightweight Client RPCs (<c>Client_GameplayEffectAppliedToSelf</c>) to push pre-filtered Server-side Asset Tags down to the Client, guaranteeing UI combat feedback without net buffer saturation.</description></item>
 * </list>
 * </remarks>
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_AURA_API UX_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region UI Decoupling
	
public:

	/**
	 * <summary>
	 * Binds function <c>CheckGameplayEffectHasMatchingAssetTags</c> to the native ASC Gameplay Effect delegate (<c>OnGameplayEffectAppliedDelegateToSelf</c>) to intercept Gameplay Effects applied to this ASC on the server.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>When to Call (Player):</b> Immediately after <c>InitAbilityActorInfo</c> inside <c>InitAbilitySystemServerSide</c> (via <c>PossessedBy</c>) and <c>InitAbilitySystemClientSide</c> (via <c>OnRep_PlayerState</c> / <c>OnRep_Controller</c>).</description></item>
	 * <item><description><b>When to Call (NPC):</b> Immediately after <c>InitAbilityActorInfo</c> inside <c>InitAbilitySystemServerAndClientSide</c> (via <c>PossessedBy</c> for the Server, and <c>PostInitializeComponents</c> for the Client).</description></item>
	 * <item><description><b>Safety:</b> Uses <c>bDelegatesBound</c> boolean to prevent duplicate memory bindings upon Character re-possession.</description></item>
	 * </list>
	 * </remarks>
	 */
	void BindToGameplayEffectDelegate();
	
	/**
	 * <summary>
	 * Client RPC (Server communicating only with the owner of this ASC) triggered when a Gameplay Effect with UI message tags is applied <b>TO</b> this ASC. Captures and broadcasts the Gameplay Effect's Asset Tags via delegate <c>OnIncomingGameplayEffectAssetTags</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>The Payload:</b> Transmits a lightweight <c>FGameplayTagContainer</c> containing only pre-filtered Asset Tags rather than the full heavy <c>FGameplayEffectSpec</c> struct.</description></item>
	 * <item><description><b>Player Execution:</b> Bypasses the replication limitation of Instant Duration Gameplay Effects by forcing the Server to send the Gameplay Effect's Asset Tags to the local Client for UI feedback.</description></item>
	 * <item><description><b>RPC Implementation:</b> Due to Unreal Header Tool (UHT) network serialization, the C++ definition for this function must be suffixed with <c>_Implementation</c>, while callers continue to use the standard name.</description></item>
	 * <item><description><b>Client_ Prefix:</b> Naming convention for RPC functions suggests using prefix "Client_" for function names.</description></item> 
	 * </list>
	 * </remarks>
	 * <param name="AssetTags">[<c>const FGameplayTagContainer&amp;</c>]: Pre-filtered collection of Asset Tags extracted on the server from the applied Gameplay Effect spec.</param>
	 */
	UFUNCTION(Client, Reliable)
	void Client_GameplayEffectAppliedToSelf(const FGameplayTagContainer& AssetTags);
	
	/**
	 * <summary>
	 * Broadcasts the Asset Tags of type "UI.Message" from any Gameplay Effect applied <b>TO</b> this ASC (by its owner or another Actor).
	 * </summary>
	 */
	FX_GameplayEffectAssetTagsSignature OnIncomingGameplayEffectAssetTags;
	
protected:
	
	/**
	 * <summary>
	 * Server-side callback function executed when <c>OnGameplayEffectAppliedDelegateToSelf</c> broadcasts. Filters for UI message tags before initiating network RPC serialization.
	 * </summary>
	 * <param name="ASC">[<c>UAbilitySystemComponent*</c>]: Pointer to the ASC receiving the Gameplay Effect.</param>
	 * <param name="EffectSpec">[<c>const FGameplayEffectSpec&amp;</c>]: The complete specification of the applied Gameplay Effect, containing its duration, context, and asset tags.</param>
	 * <param name="ActiveEffectHandle">[<c>FActiveGameplayEffectHandle</c>]: Active handle assigned to the applied effect.</param>
	 */
	void CheckGameplayEffectHasMatchingAssetTags(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
private:
	
	/**
	 * <summary>
	 * Protects this class from duplicate delegate bindings if a Character (Player or NPC) is initialized multiple times.
	 * </summary>
	 */
	bool bDelegatesBound = false;
	
#pragma endregion UI Decoupling
	
#pragma region Input Routing
	
public:

	/**
	 * <summary>
	 * Translates Enhanced Input 'Triggered' trigger events into Gameplay Ability activations.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called continuously by the Player Controller while the Input Action remains in the "Triggered" state.</description></item>
	 * <item><description>Spam protection is handled inside the Gameplay Abilities themselves (Cooldowns, Tags, Costs), <b>NOT</b> here.</description></item>
	 * <item><description>GAS only requires "Pressed" and "Released" states since "Held" is naturally handled by remaining in the "Pressed" state.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InputTag">[<c>const FGameplayTag&</c>]: The Gameplay Tag routed from the Player Controller's Enhanced Input mapping.</param>
	 */
	void AbilityInputTagPressedAndHeld(const FGameplayTag& InputTag);
	
	/**
	 * <summary>
	 * Translates Enhanced Input 'Released' trigger events into Gameplay Ability state changes (ex: dropping a continuous shield block, or firing a fully charged bow).
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called exactly once from the <c>X_PlayerController</c> when the Enhanced Input Action enters the 'Completed' state.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InputTag">[<c>const FGameplayTag&</c>]: The Gameplay Tag routed from the Player Controller's Enhanced Input mapping.</param>
	 */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
protected:
	
private:

#pragma endregion Input Routing

};
